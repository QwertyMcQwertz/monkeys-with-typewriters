# Changelog

All notable changes to MWT-1 will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.1.0] - 2026-02-19

### Added
- **OpenAI-Compatible Chat Completions API** (`POST /v1/chat/completions`)
  - Full streaming (SSE) and non-streaming support
  - Accepts messages, system prompts, tools, temperature, max_tokens
  - Compatible with OpenAI SDKs, LangChain, OpenClaw, and any OpenAI-protocol client
  - Processes all input with the same careful attention as every other endpoint
- **Model List endpoint** (`GET /v1/models`)
- Endpoint count: 14 → 16
- `openai-compatible` added to features list in `/metrics`
- Chat completion counter in metrics

### Not Added
- Actual processing of conversation history
- Awareness of previous messages
- The concept of "context"

## [1.0.0] - 2026-02-17

### Added
- MWT-1 inference engine (stochastic token sequencer)
- 150-token vocabulary (50 nouns, 25 verbs, 25 adjectives, 22 fillers)
- REST API with `/generate`, `/metrics`, and `/health` endpoints
- Sub-millisecond inference latency
- Zero hallucinations
- Zero bias
- Zero training data
- Zero copyright violations
- Zero environmental impact (relative to industry)
- Product landing page with live in-browser demo
- Model card with full evaluation methodology
- Comparison benchmarks against GPT-4o, Claude Opus, and Llama 3
- Three-tier pricing (Chimp, Gorilla, Shakespeare)
- Customer testimonials from enterprises that haven't noticed
- Security policy acknowledging the absence of anything to secure
- Contributing guidelines with vocabulary proposal process

### Not Added
- Machine learning
- Neural networks
- Attention mechanisms
- Training pipeline
- Gradient descent
- Backpropagation
- Loss functions
- Intelligence of any kind

### Known Issues
- Occasionally generates output that sounds like a real LinkedIn post
- Compliance team keeps asking what the model "actually does"
- Shakespeare tier has not yet produced Hamlet (estimated time: 10^50 years at current inference rate)
