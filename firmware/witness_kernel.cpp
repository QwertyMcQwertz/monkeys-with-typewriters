/**
 * SecuraCV Witness Kernel — ESP8266 Implementation
 *
 * Uses Rhys Weatherley's Crypto library for Ed25519 and SHA-256.
 * Chain stored as binary append-only file in SPIFFS.
 */

#include "witness_kernel.h"
#include <SHA256.h>
#include <Ed25519.h>
#include <FS.h>

// ═══════════════════════════════════════════════════
// UTILITIES
// ═══════════════════════════════════════════════════

String bytesToHex(const uint8_t* data, size_t len) {
  String hex;
  hex.reserve(len * 2);
  for (size_t i = 0; i < len; i++) {
    if (data[i] < 0x10) hex += '0';
    hex += String(data[i], HEX);
  }
  return hex;
}

// ═══════════════════════════════════════════════════
// SHA-256 HELPERS
// ═══════════════════════════════════════════════════

void WitnessKernel::sha256(const uint8_t* data, size_t len, uint8_t* hash_out) {
  SHA256 hasher;
  hasher.reset();
  hasher.update(data, len);
  hasher.finalize(hash_out, 32);
}

void WitnessKernel::sha256String(const String& str, uint8_t* hash_out) {
  sha256((const uint8_t*)str.c_str(), str.length(), hash_out);
}

void WitnessKernel::computeEntryHash(uint32_t seq, const uint8_t* prev, const uint8_t* payload, uint8_t* hash_out) {
  SHA256 hasher;
  hasher.reset();
  // Hash: sequence (4 bytes LE) || prev_hash (32 bytes) || payload_hash (32 bytes)
  uint8_t seq_bytes[4];
  seq_bytes[0] = (uint8_t)(seq & 0xFF);
  seq_bytes[1] = (uint8_t)((seq >> 8) & 0xFF);
  seq_bytes[2] = (uint8_t)((seq >> 16) & 0xFF);
  seq_bytes[3] = (uint8_t)((seq >> 24) & 0xFF);
  hasher.update(seq_bytes, 4);
  hasher.update(prev, 32);
  hasher.update(payload, 32);
  hasher.finalize(hash_out, 32);
}

// ═══════════════════════════════════════════════════
// ED25519 SIGNING
// ═══════════════════════════════════════════════════

void WitnessKernel::sign(const uint8_t* hash, uint8_t* signature_out) {
  // Ed25519::sign expects (signature, privateKey, publicKey, message, len)
  Ed25519::sign(signature_out, signing_key, verifying_key, hash, 32);
}

bool WitnessKernel::verify(const uint8_t* hash, const uint8_t* sig) {
  return Ed25519::verify(sig, verifying_key, hash, 32);
}

// ═══════════════════════════════════════════════════
// SPIFFS STORAGE
// ═══════════════════════════════════════════════════

bool WitnessKernel::appendEntry(const WitnessEntry* entry) {
  File f = SPIFFS.open(WITNESS_LOG_PATH, "a");
  if (!f) return false;

  size_t written = f.write((const uint8_t*)entry, WITNESS_ENTRY_SIZE);
  f.close();
  return (written == WITNESS_ENTRY_SIZE);
}

void WitnessKernel::loadChainState() {
  sequence = 0;
  memset(prev_hash, 0, 32);

  if (!SPIFFS.exists(WITNESS_LOG_PATH)) return;

  File f = SPIFFS.open(WITNESS_LOG_PATH, "r");
  if (!f) return;

  size_t file_size = f.size();
  if (file_size == 0 || file_size % WITNESS_ENTRY_SIZE != 0) {
    f.close();
    return;
  }

  uint32_t count = file_size / WITNESS_ENTRY_SIZE;

  // Seek to last entry
  f.seek((count - 1) * WITNESS_ENTRY_SIZE);
  WitnessEntry last;
  f.read((uint8_t*)&last, WITNESS_ENTRY_SIZE);
  f.close();

  sequence = last.sequence + 1;
  memcpy(prev_hash, last.entry_hash, 32);
}

void WitnessKernel::saveChainMeta() {
  // Metadata is reconstructed from the log on init,
  // but we store a quick-load cache for faster startup
  File f = SPIFFS.open(WITNESS_META_PATH, "w");
  if (!f) return;
  f.write((const uint8_t*)&sequence, 4);
  f.write(prev_hash, 32);
  f.close();
}

// ═══════════════════════════════════════════════════
// PUBLIC API
// ═══════════════════════════════════════════════════

void WitnessKernel::init(const uint8_t* device_seed, size_t seed_len) {
  initialized = false;

  // Derive Ed25519 keypair from seed
  // Hash the seed to get a uniform 32-byte private key seed
  uint8_t seed_hash[32];
  sha256(device_seed, seed_len, seed_hash);
  memcpy(signing_key, seed_hash, 32);

  // Derive public key from private key
  Ed25519::derivePublicKey(verifying_key, signing_key);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("[witness] SPIFFS mount failed");
    return;
  }

  // Load existing chain state
  loadChainState();

  // Handle log rotation if approaching capacity
  if (sequence >= WITNESS_MAX_ENTRIES) {
    // Remove old log and start fresh
    // The old chain is lost — in production you'd export first
    SPIFFS.remove(WITNESS_LOG_PATH);
    SPIFFS.remove(WITNESS_META_PATH);
    sequence = 0;
    memset(prev_hash, 0, 32);
    Serial.println("[witness] Log rotated — capacity reached");
  }

  initialized = true;
  Serial.print("[witness] Kernel initialized. Chain length: ");
  Serial.println(sequence);
  Serial.print("[witness] Device public key: ");
  Serial.println(getPublicKeyHex());
}

WitnessEntry WitnessKernel::witness(const String& payload) {
  WitnessEntry entry;
  memset(&entry, 0, sizeof(entry));

  if (!initialized) return entry;

  entry.sequence = sequence;
  entry.coarse_time = millis();

  // Hash the payload
  sha256String(payload, entry.payload_hash);

  // Copy previous hash (chain link)
  memcpy(entry.prev_hash, prev_hash, 32);

  // Compute entry hash
  computeEntryHash(entry.sequence, entry.prev_hash, entry.payload_hash, entry.entry_hash);

  // Sign the entry hash
  sign(entry.entry_hash, entry.signature);

  // Append to log
  if (appendEntry(&entry)) {
    // Update chain state
    memcpy(prev_hash, entry.entry_hash, 32);
    sequence++;
    saveChainMeta();
  }

  return entry;
}

bool WitnessKernel::verifyChain(uint32_t* verified_count, uint32_t* error_at) {
  *verified_count = 0;
  *error_at = 0;

  if (!SPIFFS.exists(WITNESS_LOG_PATH)) return true;

  File f = SPIFFS.open(WITNESS_LOG_PATH, "r");
  if (!f) return false;

  size_t file_size = f.size();
  if (file_size == 0) { f.close(); return true; }
  if (file_size % WITNESS_ENTRY_SIZE != 0) { f.close(); return false; }

  uint32_t count = file_size / WITNESS_ENTRY_SIZE;
  uint8_t expected_prev[32];
  memset(expected_prev, 0, 32);

  for (uint32_t i = 0; i < count; i++) {
    WitnessEntry entry;
    f.read((uint8_t*)&entry, WITNESS_ENTRY_SIZE);

    // Verify sequence is monotonic
    if (entry.sequence != i) {
      *error_at = i;
      f.close();
      return false;
    }

    // Verify prev_hash links to previous entry
    if (memcmp(entry.prev_hash, expected_prev, 32) != 0) {
      *error_at = i;
      f.close();
      return false;
    }

    // Recompute entry hash and verify
    uint8_t computed_hash[32];
    computeEntryHash(entry.sequence, entry.prev_hash, entry.payload_hash, computed_hash);
    if (memcmp(entry.entry_hash, computed_hash, 32) != 0) {
      *error_at = i;
      f.close();
      return false;
    }

    // Verify Ed25519 signature
    if (!verify(entry.entry_hash, entry.signature)) {
      *error_at = i;
      f.close();
      return false;
    }

    memcpy(expected_prev, entry.entry_hash, 32);
    (*verified_count)++;

    // Yield to avoid watchdog reset on long chains
    if (i % 100 == 0) yield();
  }

  f.close();
  return true;
}

String WitnessKernel::getPublicKeyHex() {
  return bytesToHex(verifying_key, 32);
}

uint32_t WitnessKernel::getChainLength() {
  return sequence;
}

String WitnessKernel::getLastHashHex() {
  return bytesToHex(prev_hash, 32);
}

bool WitnessKernel::getEntry(uint32_t seq, WitnessEntry* entry) {
  if (seq >= sequence) return false;
  if (!SPIFFS.exists(WITNESS_LOG_PATH)) return false;

  File f = SPIFFS.open(WITNESS_LOG_PATH, "r");
  if (!f) return false;

  f.seek(seq * WITNESS_ENTRY_SIZE);
  size_t read = f.read((uint8_t*)entry, WITNESS_ENTRY_SIZE);
  f.close();

  return (read == WITNESS_ENTRY_SIZE);
}

bool WitnessKernel::isReady() {
  return initialized;
}
