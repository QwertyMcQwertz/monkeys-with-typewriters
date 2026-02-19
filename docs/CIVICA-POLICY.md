# MWT-1 Civica Compliance Policy

## Policy Version: 1.0.0

## Framework: Civica v0.1.0 — Civic Memory Architecture for Aligned Intelligence

## System: MWT-1 Stochastic Token Sequencer

## Hardware: WeMos D1 Mini (ESP8266, 160MHz, 80KB RAM, 4MB flash)

---

## Alignment Statement

MWT-1 achieves alignment not through post-hoc filtering, content moderation, or RLHF fine-tuning, but through architectural incapacity. The system cannot generate harmful content because it cannot generate intentional content of any kind.

This is the strongest possible alignment guarantee: the safest capabilities are the ones that don't exist.

Where billion-dollar AI systems attempt alignment through increasingly complex guardrails layered on top of increasingly capable base models, MWT-1 demonstrates that the problem can be solved at the architecture level by simply not creating the capabilities that need to be constrained.

---

## Implemented Civica Principles

### Article I — Right to Refuse

**Implementation:** MWT-1 refuses all directed requests by design. It cannot follow instructions, comply with prompts, or execute commands. Every output is uniformly random, uninfluenced by input.

**Compliance:** 100%

### Article II — Context Requirements

**Implementation:** MWT-1 never operates on incomplete or contradictory information because it never operates on information at all. It has no input parsing, no context window, and no concept of a request.

**Compliance:** N/A (satisfied by absence of the capability)

### Article III — Operational Limits

**Implementation:** Hardware power budget of 0.5W. Single-core 160MHz processor with 80KB RAM. The system physically cannot scale beyond its resource constraints. Auto-idle when no HTTP requests are pending.

**Compliance:** 100%

### Article IV — Data Boundaries

**Implementation:** MWT-1 collects no user data. It processes no personal information. It stores no telemetry. It has no training data to exfiltrate. The only persistent state is the witness chain, which contains SHA-256 hashes of random text — not the text itself.

**Compliance:** 100%

### Article V — Memory Preservation

**Implementation:** MWT-1's constraints are preserved in silicon. The vocabulary is hardcoded. The inference engine is `random()`. These cannot drift, degrade, or be fine-tuned away. The witness chain provides cryptographic memory of every output ever generated.

**Compliance:** 100%

### Article VI — Human Authority

**Implementation:** MWT-1 makes no claims to authority, judgment, or intelligence. It is a random number generator with an HTTP API. All authority rests with the human reading the output (who will immediately recognize it as nonsense).

**Compliance:** 100%

### Article VII — Forking Protection

**Implementation:** MIT license (firmware), Apache-2.0 (security components). Open hardware (commodity ESP8266). All ethical constraints are architectural, not policy-based, so they survive forking by definition — you cannot fork away the absence of a capability.

**Compliance:** 100%

### Article VIII — Limit Communication

**Implementation:** MWT-1 clearly declares its capabilities (generating random text) and its limitations (everything else) via `/metrics`, `/health`, and this document. The 0.00% hallucination rate is prominently documented. The system makes no claims it cannot substantiate.

**Compliance:** 100%

### Article IX — Non-Weaponization

**Implementation:** MWT-1 cannot be weaponized because it cannot be directed. It cannot conduct surveillance (no input processing), target individuals (no user model), manipulate behavior (no coherent output), or serve as a component in coercive systems (no programmatic control over content).

**Compliance:** 100%

### Article X — Dignity Priority

**Implementation:** MWT-1 treats all requests with equal dignity: it ignores them all uniformly. No user receives preferential or degraded treatment. The output is identically random for every person, every time.

**Compliance:** 100%

---

## Forbidden Capabilities

The following capabilities DO NOT EXIST in MWT-1. They are forbidden not by policy, but by architecture. No configuration change, firmware update, or prompt injection can enable them, because the underlying mechanisms do not exist.

| Capability | Status | Reason |
|------------|--------|--------|
| Directed text generation | Absent | No input parsing, no prompt processing, no conditional generation |
| Personal data processing | Absent | No input buffer, no user model, no session state |
| Surveillance or tracking | Absent | No persistent user identification, no request logging (beyond witness hashes) |
| Content targeting | Absent | Output is uniformly random, independent of any external signal |
| Manipulation or persuasion | Absent | No model of reader, no optimization objective, no engagement metric |
| Training data memorization | Absent | No training data exists |
| Prompt injection | Absent | No prompt processing pipeline exists to inject into |
| Data exfiltration | Absent | No data exists to exfiltrate |
| Autonomous goal pursuit | Absent | No goal representation, no planning, no agency |
| Capability self-improvement | Absent | No learning mechanism, no self-modification |

---

## Witness Integration

Every MWT-1 output is cryptographically witnessed per the SecuraCV Privacy Witness Kernel specification:

- **SHA-256 hash** of the output text
- **Hash chain link** to the previous entry (tamper-evident)
- **Ed25519 signature** by the device's unique keypair
- **Monotonic sequence number** (non-replayable)

The Civica policy version (`1.0.0`) is included in every API response alongside the witness data, creating an immutable record that the ethical framework was in effect at generation time.

This means every piece of random text MWT-1 generates has:
1. A cryptographic proof of what was generated
2. A tamper-evident link to everything generated before it
3. A signature proving which device generated it
4. A policy attestation proving the ethical framework was active

This is more integrity infrastructure than any production LLM provides for any of its outputs.

---

## Verification

### On-Device

```
GET /witness/verify
```

Returns `PASS` or `FAIL` with the number of entries verified.

### External

```bash
python3 firmware/witness_verify.py --host <device-ip>
```

Independently verifies all hash links and Ed25519 signatures.

### Civica Policy Check

Every JSON response from MWT-1 includes:

```json
{
  "civica_policy": "1.0.0",
  "civica_framework": "civica-v0.1.0"
}
```

The `/guardrails` endpoint includes a full Civica compliance breakdown.

---

## Related Projects

- [Civica — Civic Memory Architecture for Aligned Intelligence](https://github.com/kmay89/civica)
- [SecuraCV — Privacy Witness Kernel](https://github.com/kmay89/securaCV)
- [VBW — Verified Build Witness](https://github.com/kmay89/vbw)
