# Security Architecture

## Overview

MWT-1 v2.1 wraps a `random()` call in production-grade security infrastructure, proving that even the simplest text generation system can have better build integrity, output witnessing, and ethical guardrails than any production LLM.

```
┌─────────────────────────────────────────────────────┐
│                    BUILD TIME                         │
│                                                       │
│  Source Code ──► VBW Build ──► Signed Attestation     │
│  (monkeys.ino)   (hash src,    (SLSA provenance,     │
│                   hash output,  in-toto layout,       │
│                   record env)   manifest + hashes)    │
└──────────────────────┬──────────────────────────────┘
                       │ verified firmware binary
                       ▼
┌─────────────────────────────────────────────────────┐
│                    RUNTIME (ESP8266)                  │
│                                                       │
│  ┌─────────────┐  ┌──────────────┐  ┌─────────────┐ │
│  │  MWT Core   │  │   Witness    │  │   Civica     │ │
│  │  (random()  │──│   Kernel     │──│   Guardrails │ │
│  │   engine)   │  │  (hash chain,│  │  (refusal    │ │
│  │             │  │   Ed25519    │  │   policy,    │ │
│  │             │  │   signatures)│  │   ethical    │ │
│  │             │  │              │  │   bounds)    │ │
│  └─────────────┘  └──────────────┘  └─────────────┘ │
│         │                │                 │          │
│         ▼                ▼                 ▼          │
│  ┌─────────────────────────────────────────────────┐ │
│  │              Secured API Layer                   │ │
│  │  Every response includes:                       │ │
│  │  - witness_hash (SHA-256 of output)             │ │
│  │  - chain_prev (link to previous hash)           │ │
│  │  - signature (Ed25519 device signature)         │ │
│  │  - civica_policy_version                        │ │
│  └─────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────┘
```

## Layer 1: Build Integrity (VBW)

**Component:** Verified Build Witness workflow + `scripts/vbw-witness.sh`

Every firmware build in CI produces:

| Artifact | Purpose |
|----------|---------|
| `manifest.json` | Source hash, environment, dependencies, output hashes |
| `provenance.json` | SLSA v1 provenance statement (in-toto Statement v1 envelope) |
| `layout.json` | in-toto layout defining expected build steps |
| `links/vbw-build.link` | in-toto link metadata for the build step |
| `vbw-policy.json` | Independence policy (no private networks, no secrets, digests required) |

The `vbw-policy.json` enforces:
- Builds only from approved GitHub organizations
- No private network references in provenance
- No embedded secrets or credentials
- All artifacts must include SHA-256 digests

## Layer 2: Output Witnessing (SecuraCV)

**Component:** `firmware/witness_kernel.h` + `firmware/witness_kernel.cpp`

Every text output is witnessed:

1. **Payload hash** — SHA-256 of the generated text
2. **Chain link** — SHA-256 of the previous entry's hash (append-only)
3. **Entry hash** — SHA-256(sequence || prev_hash || payload_hash)
4. **Signature** — Ed25519 signature of the entry hash
5. **Sequence** — Monotonic counter (non-replayable)

### Invariants (from SecuraCV spec)

- Hash chain is append-only (no deletion, no modification)
- Each entry includes hash of previous entry (tamper-evident)
- Each entry is Ed25519 signed (device-authenticated)
- Sequence numbers are strictly monotonic
- Chain verification is available on-device (`/witness/verify`)

### Storage

- Binary append-only file in SPIFFS (`/witness.log`)
- 164 bytes per entry
- ~6,000 entries before rotation (~1MB)
- Rotation discards oldest entries (export first via `/witness/export`)

### Crypto

- **Ed25519** — Rhys Weatherley's Crypto library for Arduino
- **SHA-256** — Same library
- ~50ms per Ed25519 sign operation at 160MHz
- ~1ms per SHA-256 hash of typical MWT output

## Layer 3: Ethical Framework (Civica)

**Component:** `docs/CIVICA-POLICY.md` + policy fields in API responses

Every API response includes:
```json
{
  "civica_policy": "1.0.0",
  "civica_framework": "civica-v0.1.0"
}
```

The `/guardrails` endpoint includes a full Civica compliance breakdown covering:
- Right to refuse (100% — refuses all directed requests by design)
- Non-extraction (100% — no training data, no user data, no telemetry)
- Alignment over obedience (100% — cannot obey, only generates)
- Forkability (100% — open source, open hardware)

See `docs/CIVICA-POLICY.md` for the complete policy document.

## API Endpoints

| Endpoint | Layer | Description |
|----------|-------|-------------|
| `GET /witness/status` | Witnessing | Chain metadata, device public key, capacity |
| `GET /witness/verify` | Witnessing | On-device chain verification (PASS/FAIL) |
| `GET /witness/export` | Witnessing | Full chain as JSON (for external verification) |
| `GET /witness/entry?seq=N` | Witnessing | Single entry by sequence number |
| `GET /guardrails` | Ethics | Safety filter + Civica compliance breakdown |

All other endpoints (`/generate`, `/agents`, `/rag`, etc.) include witness data and Civica policy version in their responses.

## External Verification

```bash
# Verify witness chain from device
python3 firmware/witness_verify.py --host <device-ip>

# Verify build attestation
# (when vbw CLI is available)
vbw verify vbw-bundle/ --artifact firmware.bin
```

## Threat Model Additions

The original MWT-1 threat model (SECURITY.md) remains accurate. The security additions address:

| Threat | Mitigation |
|--------|-----------|
| Binary tampering | VBW build attestation proves binary-to-source correspondence |
| Output repudiation | Witness chain provides non-repudiable record of all outputs |
| Chain tampering | Hash links + Ed25519 signatures detect any modification |
| Device impersonation | Ed25519 device key uniquely identifies each MWT-1 unit |
| Ethical drift | Civica policy version is immutably recorded with every output |

## Dependencies

| Component | Library | Size Impact |
|-----------|---------|-------------|
| Ed25519 signing | Crypto by Rhys Weatherley | ~8KB flash |
| SHA-256 hashing | Crypto by Rhys Weatherley | ~4KB flash |
| Chain storage | ESP8266 SPIFFS (built-in) | 0 additional |
