# Monkeys with Typewriters

**On-device text generation that runs on a $3 microcontroller.**

MWT-1 is a state-of-the-art language model that delivers sub-millisecond inference with zero cloud dependency, zero training data, zero hallucinations, and zero usefulness.

## Benchmarks

| Metric | GPT-4o | Claude Opus | MWT-1 |
|--------|--------|-------------|-------|
| Inference Latency | ~800ms | ~1200ms | **<1ms** |
| Hardware Cost | N/A (API) | N/A (API) | **$3** |
| Power Consumption | ~1MW | ~1MW | **0.5W** |
| Model Size | ~1.8T params | Unknown | **342 tokens** |
| Training Data | Entire internet | Entire internet | **None** |
| Hallucination Rate | ~3-5% | ~2-4% | **0.00%** |
| Copyright Lawsuits | Multiple | Pending | **0** |

## Hardware Requirements

- 1x WeMos D1 Mini (ESP8266) — [$3 on AliExpress](https://aliexpress.com)
- 1x Micro USB cable
- That's it. That's the data center.

## Quick Start

```bash
# Clone the repo
git clone https://github.com/QwertyMcQwertz/monkeys-with-typewriters.git

# Open firmware/monkeys.ino in Arduino IDE
# Set your WiFi credentials
# Flash to your WeMos D1 Mini
# You now have an AI company
```

## API

```bash
# Generate text
curl http://192.168.1.47/generate?tokens=20

# Get metrics
curl http://192.168.1.47/metrics

# Health check
curl http://192.168.1.47/health
```

### Sample Response

```json
{
  "model": "mwt-1",
  "output": "enterprise-grade synergy leverages the bleeding-edge paradigm. scalable infrastructure disrupts our next-generation ecosystem.",
  "inference_time_us": 847,
  "tokens_generated": 20,
  "latency_per_token_us": 42
}
```

### Metrics Response

```json
{
  "model": "mwt-1",
  "hardware": "ESP8266 @ 160MHz",
  "vocab_size": 342,
  "total_tokens_generated": 48205,
  "uptime_seconds": 86400,
  "power_watts": 0.5,
  "gpu_required": false,
  "cloud_required": false,
  "training_data_used": "none",
  "hallucination_rate": "0.00%",
  "bias": "none (verified)",
  "alignment_tax": "$0.00",
  "carbon_footprint_grams_co2_per_hour": 0.4
}
```

## The Model

The MWT-1 architecture is a proprietary token selection engine. Here is the complete source:

```c
int pattern = random(0, 5);
output += adjectives[random(0, NUM_ADJ)];
output += " ";
output += nouns[random(0, NUM_NOUNS)];
```

The vocabulary consists of 50 nouns, 25 verbs, 25 adjectives, and 22 filler words, carefully curated from corporate earnings calls and LinkedIn thought leadership posts.

## FAQ

**Is this real AI?**
It generates text. You tell us.

**Can it pass the Turing test?**
In a corporate meeting? Probably.

**Is this making fun of AI?**
It's making fun of AI marketing. The technology is genuinely impressive — we fit an entire language model in 2.4KB.

**Should I use this in production?**
Several Fortune 500 companies appear to already be using something equivalent.

**Can it replace my job?**
If your job is generating synergy-laden paragraphs, yes. For $3. Sorry.

## License

MIT — because unlike most AI companies, we have nothing to hide.

---

*Every metric on this page is technically true. Think about that the next time someone shows you an AI benchmark.*
