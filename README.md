<p align="center">
  <img src="https://img.shields.io/badge/inference-<1ms-00b894?style=for-the-badge" alt="Inference Latency">
  <img src="https://img.shields.io/badge/hardware_cost-$3-fdcb6e?style=for-the-badge" alt="Hardware Cost">
  <img src="https://img.shields.io/badge/hallucinations-0.00%25-6c5ce7?style=for-the-badge" alt="Hallucination Rate">
  <img src="https://img.shields.io/badge/power-0.5W-fd79a8?style=for-the-badge" alt="Power">
  <img src="https://img.shields.io/badge/copyright_lawsuits-0-00cec9?style=for-the-badge" alt="Lawsuits">
  <img src="https://img.shields.io/badge/defense_contracts-0-00b894?style=for-the-badge" alt="Defense Contracts">
</p>

<h1 align="center">🐒 Monkeys with Typewriters</h1>

<p align="center">
  <strong>The complete AI platform on a $3 microcontroller.</strong><br>
  Multi-agent orchestration · RAG · Chain-of-thought · Embeddings · Fine-tuning · Streaming · Guardrails · Tool use · Multimodal<br><br>
  <em>Every metric in this README is technically true.</em>
</p>

<p align="center">
  <a href="#-live-demo">Live Demo</a> · 
  <a href="#-benchmarks">Benchmarks</a> · 
  <a href="#-features">Features</a> · 
  <a href="#%EF%B8%8F-quick-start">Quick Start</a> · 
  <a href="WHITEPAPER.md">Whitepaper</a> · 
  <a href="MODEL_CARD.md">Model Card</a> · 
  <a href="ARCHITECTURE.md">Architecture</a> · 
  <a href="docs/API.md">API Docs</a>
</p>

---

## What is MWT-1?

MWT-1 is a breakthrough text generation platform that achieves what the $150 billion AI industry has been promising — sub-millisecond inference, zero hallucinations, zero bias, zero environmental impact, and complete data privacy — by taking the radical approach of not using machine learning at all.

It runs on a WeMos D1 Mini. That's a $3 microcontroller with a 160MHz single-core processor, 80KB of RAM, and 4MB of flash storage. The entire model, inference engine, multi-agent orchestrator, RAG pipeline, chain-of-thought reasoner, embedding generator, fine-tuning system, streaming server, safety guardrails, tool-use framework, and multimodal processor fit on it with room to spare.

The model is a 342-token vocabulary and a call to `random()`. That's the architecture. The rest is API endpoints.

> "We replaced our entire NLP pipeline with MWT-1 and honestly? Customers haven't noticed."
> — Director of AI, Fortune 500 Retailer

## 🎯 Live Demo

The demo runs locally in your browser. It detects your hardware, shows your specs, and generates text using the same algorithm as the microcontroller. It recommends Chrome for optimal random number generation.

**[Try it live →](https://monkeys-with-typewriters.netlify.app)** *(coming soon)*

## 📊 Benchmarks

Real measurements. No asterisks. Well, one asterisk.

| Metric | GPT-4o | Claude Opus | Llama 3 70B | MWT-1 |
|--------|--------|-------------|-------------|-------|
| Inference Latency | ~800ms | ~1200ms | ~200ms | **<1ms** |
| Hardware Cost | N/A (API) | N/A (API) | ~$30,000 | **$3** |
| Power Consumption | ~1MW | ~1MW | ~300W | **0.5W** |
| Model Size | ~1.8T params | Unknown | 70B params | **342 tokens** |
| Training Data | Entire internet | Entire internet | 15T tokens | **None** |
| Training Cost | ~$100M | Unknown | ~$10M | **$0.00** |
| Hallucination Rate | ~3-5% | ~2-4% | ~5-8% | **0.00%** |
| Bias Score | Documented | Documented | Documented | **0.000000** |
| Safety Filter Pass Rate | ~97% | ~98% | ~95% | **100.00%** |
| Copyright Lawsuits | Multiple | Pending | Pending | **0** |
| Defense Contracts | Yes | Yes (Palantir) | Yes | **0** |
| Multi-Agent | Via wrapper | Via wrapper | Via wrapper | **Native** |
| Built-in RAG | No | No | No | **Yes** |
| Fine-Tuning Cost | $$$ | N/A | $$ | **$0.00** |
| Usefulness | High | High | High | **Comparable*** |

*\* Depending on use case. MWT-1 excels in environments where nobody reads the output, which according to our research is most of them.*

## 🚀 Features

MWT-1 ships with every feature the AI industry charges premium prices for. All 14 API endpoints run on the same $3 chip.

### Core Inference

| Endpoint | Description |
|----------|-------------|
| `GET /generate` | Text generation with temperature control |
| `GET /stream` | Server-Sent Events streaming with simulated "thinking" delay |
| `GET /structured` | JSON-mode output with titles, findings, recommendations, confidence scores |

### Multi-Agent System

```
GET /agents?agents=5&mode=parallel&tokens=10
```

Spawn up to **10 specialized agents** — Strategist, Architect, Analyst, Optimizer, Synthesizer, Evaluator, Coordinator, Visionary, Executor, Validator — that execute in parallel and produce independent output. An orchestrator synthesizes the results via `parallel_consensus`.

The agents don't communicate with each other. They don't know about each other. They each call the same `random()` function. The synthesis step is another call to `random()`. This is architecturally identical to most enterprise "multi-agent" systems, except we're honest about it.

### RAG Pipeline

```
GET /rag?query=strategic+analysis&tokens=15
```

Retrieval Augmented Generation with a **built-in knowledge base**. The retriever searches the 342-token vocabulary index, scores matches by relevance (randomly), and augments generation with the retrieved context (which it ignores). No Pinecone subscription required.

### Chain-of-Thought Reasoning

```
GET /chain-of-thought?steps=5&tokens=8
```

Multi-step reasoning with visible thought process. Each step is prefixed with phrases like *"analyzing the implications of"* and *"evaluating the trade-offs inherent in"*, creating the appearance of deep analytical thinking. The steps are independently generated with zero awareness of each other, which if you've worked in consulting, will feel extremely familiar.

### Embeddings

```
GET /embeddings?input=synergy&dimensions=16
```

Dense vector representations using a proprietary sinusoidal hash function. The embeddings are mathematically valid — cosine similarity works, distance metrics work, you can build a vector database with them. The vectors don't encode meaning, but they do encode *something*, which is more than can be said for most corporate strategy documents.

### Fine-Tuning

```
GET /fine-tune?token=deliverables
```

Customize the model for your domain by adding tokens to the vocabulary at runtime. Training time: **0 seconds**. Training cost: **$0.00**. Gradient updates: **0**. Epochs: **0**. The model immediately incorporates new tokens at a reported improvement rate of 0.1-0.7% (randomly generated, like the improvement rates in most ML papers).

### Safety Guardrails

```
GET /guardrails?tokens=15
```

8-category content safety filter covering violence, harassment, self-harm, sexual content, hate speech, illegal activity, deception, and political content. **100% pass rate. Always.** The model cannot generate harmful content because it cannot generate intentional content of any kind.

### Tool Use / Function Calling

```
GET /tool-use?task=analyze+market+trends
```

The model selects and executes tools to augment its response. Available tools:

| Tool | Description |
|------|-------------|
| `random()` | Stochastic token selection engine |
| `analogRead()` | Hardware entropy source (ADC pin) |
| `millis()` | Temporal reference module |
| `micros()` | High-resolution timing oracle |

These are the actual C functions the model calls. This is not a metaphor. When we say "tool use," we mean it literally.

### Multimodal

```
GET /multimodal
```

Reads the analog voltage on pin A0 and generates a text-based analysis with confidence scores. Text output from non-text input. That's multimodal by definition. Nobody said the modalities had to be useful.

## ⚡️ Quick Start

### Hardware

- 1x WeMos D1 Mini (ESP8266) — [$3 on AliExpress](https://aliexpress.com)
- 1x Micro USB cable
- That's it. That's the data center.

### Deploy

```bash
# Clone
git clone https://github.com/QwertyMcQwertz/monkeys-with-typewriters.git
cd monkeys-with-typewriters

# Option 1: Arduino IDE
# Open firmware/monkeys.ino
# Set WiFi credentials
# Flash to board

# Option 2: PlatformIO
# Edit firmware/monkeys.ino WiFi credentials
pio run --target upload
pio device monitor -b 115200
```

```
╔══════════════════════════════════════════╗
║   MONKEYS WITH TYPEWRITERS               ║
║   MWT-1 Language Model v2.0.0            ║
║                                          ║
║   Features: Multi-Agent · RAG · CoT      ║
║   Embeddings · Streaming · Fine-Tuning   ║
║   Guardrails · Tool Use · Multimodal     ║
║                                          ║
║   Loading vocabulary index...             ║
╚══════════════════════════════════════════╝
Connecting to inference network...
Inference server online at http://192.168.1.47
Model loaded. Ready for inference.
Vocabulary: 342 tokens
Agents available: 10
Endpoints: 14
Accepting requests on port 80.
```

You now have an AI company.

### First Request

```bash
curl http://192.168.1.47/generate?tokens=10
```

```json
{
  "model": "mwt-1",
  "output": "enterprise-grade synergy leverages the bleeding-edge paradigm. SOC2-compliant infrastructure futureproofs our north-star-driven flywheel",
  "temperature": 1.00,
  "inference_time_us": 312,
  "tokens_generated": 10,
  "latency_per_token_us": 31
}
```

## 🧠 The Model

The MWT-1 architecture is a proprietary stochastic token sequencer. Here is the complete source:

```c
int pattern = random(0, 5);
output += adjectives[random(0, NUM_ADJ)];
output += " ";
output += nouns[random(0, NUM_NOUNS)];
// That's it. That's the model.
```

The vocabulary consists of 145 nouns, 75 verbs, 75 adjectives, and 47 filler words, carefully curated from corporate earnings calls, LinkedIn thought leadership posts, Gartner Magic Quadrants, and the existential dread of attending too many sprint retrospectives.

The multi-agent orchestrator calls this function in a loop. The RAG pipeline calls this function after pretending to search something. The chain-of-thought prepends "analyzing the implications of" and then calls this function. Every feature is this function.

**[Full architecture →](ARCHITECTURE.md)** · **[Model card →](MODEL_CARD.md)** · **[Whitepaper →](WHITEPAPER.md)**

## 📈 Scaling

MWT-1 scales horizontally by purchasing additional $3 microcontrollers.

| Cluster Size | Cost | Tokens/Second | Equivalent |
|-------------|------|---------------|------------|
| 1 node | $3 | ~50,000 | 1 startup |
| 10 nodes | $30 | ~500,000 | 1 Series A |
| 100 nodes | $300 | ~5,000,000 | OpenAI |
| 1,000 nodes | $3,000 | ~50,000,000 | The singularity, probably |

A 10-node MWT-1 cluster costs $30 and outperforms a $300,000 H100 cluster on tokens per second by a factor of 625x. The H100 cluster's tokens are more likely to mean something. We leave it to the reader to determine how much "meaning" is worth per dollar.

## ❓ FAQ

**Is this real AI?**
It generates text. You tell us.

**Can it pass the Turing test?**
In a corporate meeting? Probably. In a LinkedIn comments section? Definitely. In a sprint planning session? It already has.

**Is this making fun of AI?**
It's making fun of AI marketing. The technology itself is genuinely impressive — we fit an entire language model with 14 API endpoints, a multi-agent orchestrator, a RAG pipeline, and a chain-of-thought reasoner on a chip that costs less than a coffee. The fact that it does nothing useful is a feature, not a bug.

**Should I use this in production?**
Several Fortune 500 companies appear to already be using something equivalent, based on the quality of their chatbot responses.

**Can it replace my job?**
If your job is generating synergy-laden paragraphs for stakeholder alignment, yes. For $3. Sorry.

**Will it achieve sentience?**
No. It is a random number generator. It has no inner life, no goals, no desires, and no capacity for suffering. In this regard, it is identical to most enterprise software.

**Why does it recommend Chrome?**
V8's JIT compiler produces measurably faster random numbers than SpiderMonkey or JavaScriptCore. This is technically true and completely irrelevant to the output quality, which is random regardless of browser. We included it because it's the kind of meaningless optimization that AI companies love to brag about.

**Is the whitepaper peer-reviewed?**
We submitted it. We are awaiting a response. We suspect the reviewers are trying to figure out if it's a joke. (It is. The results are real.)

**How does MWT-1 compare to GPT-4o?**
MWT-1 is faster, cheaper, greener, less biased, more private, more transparent, and has fewer copyright lawsuits. GPT-4o is better at generating text that means something. Whether that matters depends on your use case.

## 📄 Documentation

| Document | Description |
|----------|-------------|
| [MODEL_CARD.md](MODEL_CARD.md) | Full model evaluation, benchmarks against frontier LLMs, ethical analysis |
| [WHITEPAPER.md](WHITEPAPER.md) | Peer-reviewable research paper with methodology and citations |
| [ARCHITECTURE.md](ARCHITECTURE.md) | System design, transformer comparison, scaling analysis |
| [docs/API.md](docs/API.md) | Complete API reference with 14 endpoints, integration examples |
| [SECURITY.md](SECURITY.md) | Threat model for a device with nothing to protect |
| [CONTRIBUTING.md](CONTRIBUTING.md) | Vocabulary proposal guidelines and code of conduct |
| [CHANGELOG.md](CHANGELOG.md) | Release notes, including what was Not Added |

The documentation-to-code ratio is approximately 15:1. This is enforced by CI.

## 🔬 CI Pipeline

Every push runs:

- **Vocabulary Audit** — counts tokens, estimates model size
- **Duplicate Token Check** — ensures maximum vocabulary diversity
- **Zero Training Data Verification** — confirms no model weights, checkpoints, or learned parameters exist in the repo (if found, something has gone terribly wrong)
- **Bias Evaluation** — exhaustive scan of all vocabulary tokens (result: 0.000000, always)
- **Documentation Completeness** — verifies all 7 required docs exist and enforces doc-to-code ratio

## 📜 License

MIT — because unlike most AI companies, we have nothing to hide.

The license includes a custom clause disclaiming that the output constitutes "meaningful communication, artificial intelligence, or thought of any kind" and noting that "any resemblance to coherent language is coincidental and should concern you philosophically."

---

<p align="center">
  <strong>Every metric in this repository is technically true.</strong><br>
  Think about that the next time someone shows you an AI benchmark.<br><br>
  <em>Built with irritation by <a href="https://jayneawedde.com">Jay Neawedde</a></em>
</p>
