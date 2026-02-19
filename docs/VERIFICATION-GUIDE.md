# Verification Guide

How to verify that your MWT-1 is doing what it claims to be doing (generating random text with cryptographic integrity).

## 1. Verify the Build

Every CI build produces a VBW attestation bundle as a GitHub Actions artifact.

### Download the Bundle

Go to the Actions tab, find the "VBW Build Witness" workflow run, and download the `vbw-attestation-bundle` artifact.

### Manual Verification

```bash
# Compare firmware binary hash against manifest
sha256sum firmware.bin
python3 -c "import json; print(json.load(open('vbw-bundle/manifest.json'))['artifacts'][0]['digest']['sha256'])"
```

If the hashes match, the binary corresponds to the witnessed build.

### VBW CLI Verification

```bash
# When vbw is installed:
vbw verify vbw-bundle/ --artifact firmware.bin --slsa-mode schema-only
```

## 2. Verify the Witness Chain

### On-Device Verification

```bash
curl http://<device-ip>/witness/verify
```

Response:
```json
{
  "result": "PASS",
  "entries_verified": 142,
  "chain_length": 142,
  "verification_time_us": 847321,
  "device_pubkey": "a1b2c3..."
}
```

### External Verification (Recommended)

The external verifier is independent of the device firmware. It fetches the chain and re-verifies every hash link and signature.

```bash
# Install dependencies
pip install requests pynacl

# Verify from device
python3 firmware/witness_verify.py --host <device-ip>

# Or export and verify offline
curl -o chain.json http://<device-ip>/witness/export
python3 firmware/witness_verify.py --file chain.json
```

Expected output:
```
==================================================
MWT-1 Witness Chain Verifier
SecuraCV PWK — External Verification
==================================================

Device public key: a1b2c3...
Chain length:      142

Verifying chain...

PASS: All 142 entries verified
  Hash chain: intact
  Signatures: valid (Ed25519)
  Device key: a1b2c3...
```

### What the Verifier Checks

For each entry in the chain:

1. **Sequence monotonicity** — entry N has sequence number N
2. **Hash chain integrity** — entry N's `prev_hash` equals entry N-1's `entry_hash`
3. **Entry hash correctness** — recomputes `SHA-256(sequence || prev_hash || payload_hash)` and compares
4. **Ed25519 signature** — verifies the device signature over the entry hash

## 3. Verify Civica Compliance

### Check Policy Version

Every API response includes:
```json
{
  "civica_policy": "1.0.0",
  "civica_framework": "civica-v0.1.0"
}
```

### Check Full Compliance Breakdown

```bash
curl http://<device-ip>/guardrails | python3 -m json.tool
```

The `moderation.civica_compliance` object shows compliance status for each Civica principle.

### Read the Policy Document

The full policy is at `docs/CIVICA-POLICY.md`, mapping every Civica article to MWT-1's architecture.

## 4. Inspect Individual Witness Entries

```bash
# Get witness status
curl http://<device-ip>/witness/status

# Get a specific entry
curl http://<device-ip>/witness/entry?seq=0    # Genesis entry
curl http://<device-ip>/witness/entry?seq=141  # Latest entry

# Export full chain
curl -o chain.json http://<device-ip>/witness/export
```

## 5. Cross-Verify

For maximum assurance, verify from multiple sources:

1. **On-device**: `GET /witness/verify` → PASS
2. **External Python**: `witness_verify.py --host <ip>` → PASS
3. **Manual**: Spot-check individual entries with a SHA-256 calculator
4. **Build**: Compare binary hash against VBW manifest

If all four agree, you have a `random()` call with better integrity guarantees than any production LLM.
