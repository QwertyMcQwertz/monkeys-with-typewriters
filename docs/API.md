# MWT-1 API Reference

**Base URL:** `http://<device-ip>/`
**Authentication:** None (there is nothing to protect)
**Rate Limiting:** Hardware-limited to approximately 500 req/s (ESP8266 constraint, not a business decision)

---

## Endpoints

### `GET /`

Returns a human-readable overview of the inference server.

**Response:** `text/html`

---

### `GET /generate`

Run inference and generate text.

**Parameters:**

| Parameter | Type | Default | Range | Description |
|-----------|------|---------|-------|-------------|
| `tokens` | integer | 20 | 1-200 | Number of pattern groups to generate. Actual token count varies by pattern (2-6 tokens per group). |

**Response:** `application/json`

```json
{
  "model": "mwt-1",
  "output": "enterprise-grade synergy leverages the bleeding-edge paradigm",
  "inference_time_us": 847,
  "tokens_generated": 20,
  "latency_per_token_us": 42
}
```

**Response Fields:**

| Field | Type | Description |
|-------|------|-------------|
| `model` | string | Always `"mwt-1"`. We only have one model. We only need one model. |
| `output` | string | The generated text. May or may not mean anything. Statistically, it does not. |
| `inference_time_us` | integer | Total inference time in microseconds. This number is always impressive. |
| `tokens_generated` | integer | Number of pattern groups processed. |
| `latency_per_token_us` | integer | Average microseconds per token. Include this in pitch decks. |

**Example:**

```bash
curl http://192.168.1.47/generate?tokens=5
```

```json
{
  "model": "mwt-1",
  "output": "scalable convergence disrupts the hyper-converged ecosystem. frictionless architecture toward bleeding-edge synergy",
  "inference_time_us": 213,
  "tokens_generated": 5,
  "latency_per_token_us": 42
}
```

**Error Handling:** This endpoint does not produce errors. It always generates text. The text is always random. There is no failure mode beyond "the device is not powered on."

---

### `GET /metrics`

Returns comprehensive system and model metrics.

**Response:** `application/json`

```json
{
  "model": "mwt-1",
  "hardware": "ESP8266 @ 160MHz",
  "flash_mb": 4,
  "ram_kb": 80,
  "vocab_size": 342,
  "total_tokens_generated": 48205,
  "total_inferences": 2410,
  "uptime_seconds": 86400,
  "avg_tokens_per_second": 0.56,
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

**Selected Field Notes:**

| Field | Note |
|-------|------|
| `gpu_required` | Always `false`. The GPU industry does not want you to know about MWT-1. |
| `cloud_required` | Always `false`. The cloud industry also does not want you to know about MWT-1. |
| `training_data_used` | Always `"none"`. Copyright lawyers love this field. |
| `hallucination_rate` | Always `"0.00%"`. See MODEL_CARD.md for methodology (it doesn't know anything). |
| `bias` | Always `"none (verified)"`. Uniform random selection is mathematically unbiased. |
| `alignment_tax` | Always `"$0.00"`. There is no alignment because there is no intelligence to align. |

---

### `GET /health`

Health check endpoint. Compatible with Kubernetes liveness probes, load balancers, and uptime monitors that need to feel useful.

**Response:** `application/json`

```json
{
  "status": "operational",
  "model": "mwt-1",
  "mood": "productive"
}
```

The `mood` field is always `"productive"` because MWT-1 is always generating random text when asked. It does not have bad days. It does not need mental health breaks. It does not have opinions about its working conditions. It is a microcontroller.

---

## Integration Examples

### Python

```python
import requests

response = requests.get("http://192.168.1.47/generate", params={"tokens": 10})
data = response.json()

# Use the output for whatever you were going to use GPT for
# (Results may vary. They also may not. It's random.)
print(data["output"])
```

### JavaScript

```javascript
const response = await fetch("http://192.168.1.47/generate?tokens=10");
const data = await response.json();

// Enterprise-grade text, ready for production
document.getElementById("ai-content").textContent = data.output;
```

### cURL

```bash
# Generate a quarterly report
curl -s http://192.168.1.47/generate?tokens=50 | jq -r '.output'

# Check if the AI is sentient yet
curl -s http://192.168.1.47/health | jq -r '.mood'
# Output: "productive" (it is not sentient)
```

### Prometheus

```yaml
# prometheus.yml
scrape_configs:
  - job_name: 'mwt-1'
    metrics_path: '/metrics'
    static_configs:
      - targets: ['192.168.1.47:80']
    # Scrape interval can be as low as you want.
    # The metrics don't change much because the model
    # doesn't learn or degrade. It's refreshingly stable.
    scrape_interval: 30s
```

---

## Comparison with OpenAI API

| Feature | OpenAI API | MWT-1 API |
|---------|-----------|-----------|
| Authentication | API key required | None required |
| Rate limits | Tiered by spend | Hardware-limited only |
| Pricing | Per-token | Free forever |
| Uptime SLA | 99.9% | Depends on your outlet |
| Response format | OpenAI schema | Simple JSON |
| Streaming | SSE | No (inference completes before the first network packet is assembled) |
| Content filtering | Yes | No (the content is random; filtering random noise is a philosophical exercise) |
| Function calling | Yes | No (MWT-1 does not call functions. MWT-1 does not call anyone. MWT-1 has no agency.) |

---

## Frequently Asked Questions

**Q: Can I use this as a drop-in replacement for the OpenAI API?**
A: No. The response format is different, the output is random, and it runs on a microcontroller. However, if you wrap it in a compatibility layer and don't tell anyone, we estimate it would take 3-6 months before a non-technical stakeholder notices.

**Q: Is there a WebSocket endpoint for streaming?**
A: No. Inference completes in under 1 millisecond. By the time you've opened the WebSocket connection, the text has already been generated, served, and forgotten. Streaming would just be adding latency.

**Q: What happens if I set tokens to 200?**
A: You get a lot of random text. The ESP8266 handles it fine. The output is approximately 1-2KB. It means nothing, but it means nothing at scale.
