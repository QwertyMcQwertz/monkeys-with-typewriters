# Architecture

## System Overview

```
┌─────────────────────────────────────┐
│         MWT-1 INFERENCE STACK       │
├─────────────────────────────────────┤
│                                     │
│   ┌───────────────────────────┐     │
│   │      REST API Layer       │     │
│   │    (ESP8266WebServer)     │     │
│   └─────────────┬─────────────┘     │
│                 │                    │
│   ┌─────────────▼─────────────┐     │
│   │    Inference Engine       │     │
│   │  (random() + switch/case) │     │
│   └─────────────┬─────────────┘     │
│                 │                    │
│   ┌─────────────▼─────────────┐     │
│   │    Vocabulary Index       │     │
│   │   (4 arrays, 342 tokens)  │     │
│   └───────────────────────────┘     │
│                                     │
│   Hardware: ESP8266 @ 160MHz        │
│   RAM: 80KB │ Flash: 4MB            │
│   Power: 0.5W │ Cost: $3            │
│                                     │
└─────────────────────────────────────┘
```

## Inference Pipeline

The MWT-1 inference pipeline consists of three stages:

### Stage 1: Pattern Selection

A random integer between 0 and 4 is generated using the hardware random number generator. This integer selects one of five sentence templates. This is the "architecture" of the model.

```c
int pattern = random(0, 5);
```

We acknowledge that calling `random()` an "architecture" is generous. We also note that calling matrix multiplication on billions of parameters an "architecture" is equally generous. At least ours fits on a napkin.

### Stage 2: Token Selection

For each slot in the selected pattern, a token is randomly selected from the corresponding vocabulary array (nouns, verbs, adjectives, or fillers). Selection is uniform random with no weighting, bias, or preference of any kind.

```c
output += adjectives[random(0, NUM_ADJ)];
output += " ";
output += nouns[random(0, NUM_NOUNS)];
```

This is the "forward pass." There is no backward pass. There is nothing to propagate back to.

### Stage 3: Punctuation

A period is inserted with 30% probability at the end of each pattern. This creates the illusion of sentence structure. We call this our "grammar model." It has one rule.

```c
if (random(0, 10) < 3) {
    output += ". ";
}
```

## Comparison with Transformer Architecture

| Component | Transformer | MWT-1 |
|-----------|------------|-------|
| Embedding layer | Learned | Hardcoded string arrays |
| Attention mechanism | Multi-head self-attention | None (tokens don't attend to anything) |
| Feed-forward network | Dense layers with nonlinearity | `switch/case` |
| Layer normalization | Yes | No (there is one layer and it does not need normalizing) |
| Positional encoding | Sinusoidal or learned | None (position is meaningless, like in most corporate communications) |
| Output head | Softmax over vocabulary | `random()` over vocabulary |
| Loss function | Cross-entropy | There is no loss. Only text. |
| Optimizer | Adam, AdaGrad, etc. | None (nothing to optimize) |
| Tokenizer | BPE, SentencePiece | Array index |
| Context window | 2K - 128K+ tokens | 0 tokens (blessed freedom from context) |
| Inference cost | $0.01 - $0.10 per 1K tokens | $0.00 per ∞ tokens |

## Scaling

MWT-1 scales horizontally by purchasing additional $3 microcontrollers. A 10-node MWT-1 cluster costs $30 and can generate over 500,000 tokens per second.

For comparison, a 10-node H100 cluster costs approximately $300,000 and can generate approximately 800 tokens per second. MWT-1 is 625x faster at 0.01% of the cost.

We acknowledge that the H100 cluster's tokens are more likely to mean something. We leave it to the reader to determine how much "meaning" is worth per dollar.

## Reliability

MWT-1 has no failure modes related to model quality because there is no model quality. The output is uniformly random at all times. There is no concept of degraded output, model drift, catastrophic forgetting, or reward hacking.

The only failure modes are:
1. **Power loss** — the ESP8266 loses power and stops generating text
2. **WiFi disconnection** — the API becomes unreachable but inference continues internally, unobserved, like a tree falling in a forest
3. **Heat death of the universe** — estimated impact: moderate

## Security Architecture (v2.1)

MWT-1 v2.1 adds three security layers, each independently verifiable:

```
┌─────────────────────────────────┐
│       REST API Layer            │
│  + witness data in every response│
│  + Civica policy version         │
├─────────────────────────────────┤
│       Witness Kernel             │
│  (SHA-256 hash chain + Ed25519)  │
│  - SPIFFS append-only log        │
│  - ~164 bytes per entry          │
│  - ~6,000 entries before rotation│
├─────────────────────────────────┤
│       Inference Engine           │
│  (random() + switch/case)        │
├─────────────────────────────────┤
│       Vocabulary Index           │
│  (4 arrays, 342 tokens)          │
└─────────────────────────────────┘
```

### Witness Kernel

Ported from the SecuraCV Privacy Witness Kernel (Rust → Arduino C++). Uses the Crypto library by Rhys Weatherley for Ed25519 signing and SHA-256 hashing.

Each witness entry contains:
- **Sequence number** (4 bytes) — monotonic counter
- **Previous hash** (32 bytes) — SHA-256 of previous entry's hash (chain link)
- **Payload hash** (32 bytes) — SHA-256 of the generated text
- **Entry hash** (32 bytes) — SHA-256(sequence || prev_hash || payload_hash)
- **Signature** (64 bytes) — Ed25519 signature of entry_hash

Total: 164 bytes per entry.

### Build Witnessing (VBW)

CI produces a VBW attestation bundle for every build:
- Source tree hash (git ls-files → SHA-256)
- Build environment snapshot
- Dependency manifest
- Output binary hash
- SLSA v1 provenance statement

### Civica Ethical Framework

Policy version embedded in every API response. Full compliance breakdown available at `/guardrails`. See `docs/CIVICA-POLICY.md` for the complete policy.

## Future Architecture

We have no plans to add intelligence to MWT-1. The current architecture represents our design philosophy: if you're going to generate text that nobody reads, you might as well do it efficiently.
