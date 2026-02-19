/**
 * SecuraCV Witness Kernel — Lightweight ESP8266 Port
 *
 * Tamper-evident hash-chain logging with Ed25519 signatures.
 * Every MWT-1 output is witnessed: hashed, chained to the previous
 * entry, signed, and stored in SPIFFS.
 *
 * Follows SecuraCV Privacy Witness Kernel invariants:
 *   - Hash chain is append-only
 *   - Each entry includes hash of previous entry
 *   - Each entry is Ed25519 signed
 *   - Sequence numbers are monotonic
 *   - Chain verification is available on-device
 *
 * Crypto: Rhys Weatherley's Crypto library (Ed25519 + SHA256)
 * Storage: SPIFFS binary append-only log
 * Target: ESP8266 (80KB RAM, 4MB flash)
 */

#ifndef WITNESS_KERNEL_H
#define WITNESS_KERNEL_H

#include <Arduino.h>

// Witness log entry — 164 bytes per entry
// At ~1MB usable SPIFFS, supports ~6,000 entries before rotation
struct WitnessEntry {
  uint32_t sequence;           // monotonic counter
  uint8_t  prev_hash[32];     // SHA-256 of previous entry_hash (chain link)
  uint8_t  payload_hash[32];  // SHA-256 of the MWT output text
  uint8_t  entry_hash[32];    // SHA-256(sequence || prev_hash || payload_hash)
  uint8_t  signature[64];     // Ed25519 signature of entry_hash
  uint32_t coarse_time;       // millis() timestamp
};

// Size constant for binary storage
static const size_t WITNESS_ENTRY_SIZE = sizeof(WitnessEntry);

// Maximum chain length before rotation (~1MB / 164 bytes)
static const uint32_t WITNESS_MAX_ENTRIES = 6000;

// SPIFFS log path
static const char* WITNESS_LOG_PATH = "/witness.log";
static const char* WITNESS_META_PATH = "/witness.meta";

class WitnessKernel {
public:
  // Initialize kernel with device-unique seed
  // Derives Ed25519 keypair, loads chain state from SPIFFS
  void init(const uint8_t* device_seed, size_t seed_len);

  // Witness an output string: hash, chain, sign, store
  // Returns the witness entry for inclusion in API response
  WitnessEntry witness(const String& payload);

  // Verify the full chain on-device
  // Returns true if all hash links and signatures are valid
  bool verifyChain(uint32_t* verified_count, uint32_t* error_at);

  // Get hex-encoded public key for external verification
  String getPublicKeyHex();

  // Get current chain length
  uint32_t getChainLength();

  // Get last entry hash as hex string
  String getLastHashHex();

  // Export a single entry by sequence number
  // Returns false if sequence is out of range
  bool getEntry(uint32_t seq, WitnessEntry* entry);

  // Check if kernel is initialized
  bool isReady();

private:
  uint8_t signing_key[32];    // Ed25519 private key seed
  uint8_t verifying_key[32];  // Ed25519 public key
  uint8_t prev_hash[32];      // hash of last entry (chain tip)
  uint32_t sequence;           // next sequence number
  bool initialized;

  // Compute SHA-256 of a buffer
  void sha256(const uint8_t* data, size_t len, uint8_t* hash_out);

  // Compute SHA-256 of a String
  void sha256String(const String& str, uint8_t* hash_out);

  // Compute entry hash: SHA-256(sequence || prev_hash || payload_hash)
  void computeEntryHash(uint32_t seq, const uint8_t* prev, const uint8_t* payload, uint8_t* hash_out);

  // Sign a 32-byte hash with Ed25519
  void sign(const uint8_t* hash, uint8_t* signature_out);

  // Verify an Ed25519 signature
  bool verify(const uint8_t* hash, const uint8_t* signature);

  // Append entry to SPIFFS log
  bool appendEntry(const WitnessEntry* entry);

  // Load chain state (sequence, prev_hash) from SPIFFS
  void loadChainState();

  // Save chain metadata
  void saveChainMeta();
};

// Utility: convert byte array to hex string
String bytesToHex(const uint8_t* data, size_t len);

#endif // WITNESS_KERNEL_H
