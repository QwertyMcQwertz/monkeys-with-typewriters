# Model Card: MWT-1

## Model Details

| Field | Value |
|-------|-------|
| Model Name | MWT-1 |
| Model Version | 1.0.0 |
| Model Type | Stochastic Token Sequencer |
| Architecture | Single-pass random selection with weighted pattern templates |
| Parameters | 0 |
| Weights | 0 |
| Layers | 0 |
| Attention Heads | 0 |
| Training Data | None |
| Training Compute | 0 FLOPS |
| Training Time | 0 seconds |
| Training Cost | $0.00 |
| Fine-Tuning | Not applicable (nothing to tune) |
| RLHF | Not applicable (no humans were in the loop, or wanted to be) |
| Vocabulary Size | 150 tokens |
| Model Size on Disk | ~2.4KB |
| Quantization | N/A (model is already smaller than most config files) |
| Context Window | 0 tokens (each token is generated independently, free from the burden of context) |
| License | MIT |

## Intended Use

MWT-1 is designed for general-purpose text generation in environments where the content of the text is less important than the fact that text was generated. Target applications include:

- Automated corporate communications
- Filler content for pitch decks
- Padding for reports that nobody will read
- "AI-powered" feature checkboxes on product roadmaps
- Executive summaries
- Thought leadership LinkedIn posts
- Consultant deliverables

## Out-of-Scope Uses

- Any task requiring the output to mean something
- Medical advice
- Legal counsel
- Financial planning
- Replacing writers who actually have something to say

## Training Data

MWT-1 was not trained on any data. The vocabulary was hand-curated by a human who has attended too many corporate meetings. No copyrighted material was used in training because there was no training. No data was scraped because there was nothing to scrape. No artists were harmed. No authors were plagiarized. No terms of service were violated.

The 150-token vocabulary was sourced from:
- Corporate earnings call transcripts (paraphrased into single words by a human)
- LinkedIn trending posts (distilled into their component buzzwords by a human)
- Technology conference keynotes (reduced to their essential meaninglessness by a human)
- The human's own growing sense of professional ennui

## Vocabulary Composition

| Category | Count | Source |
|----------|-------|--------|
| Nouns | 50 | Business/tech buzzwords |
| Verbs | 25 | Corporate action words |
| Adjectives | 25 | Startup pitch modifiers |
| Fillers | 22 | English structural words |
| Punctuation | 2 | Period, space |
| **Total** | **150** | **One person's disillusionment** |

## Evaluation

### Methodology

MWT-1 was evaluated against leading commercial and open-source language models across multiple dimensions. All measurements are real. The interpretation is left as an exercise for the reader.

### Results

| Benchmark | MWT-1 | GPT-4o | Claude Opus | Llama 3 70B |
|-----------|-------|--------|-------------|-------------|
| Inference Latency | **<1ms** | ~800ms | ~1200ms | ~200ms |
| Tokens/Second | **>50,000** | ~80 | ~60 | ~40 |
| Tokens/Watt | **>100,000** | ~0.00008 | ~0.00006 | ~0.13 |
| Tokens/Dollar (hardware) | **∞** | N/A | N/A | ~2.3 |
| Hallucination Rate | **0.00%** | ~3-5% | ~2-4% | ~5-8% |
| Bias Score | **0.00** | Documented | Documented | Documented |
| Copyright Violations | **0** | Litigating | Litigating | Litigating |
| MMLU | 0.0 | 88.7 | 86.8 | 82.0 |
| HumanEval | 0.0 | 90.2 | 84.9 | 81.7 |
| HellaSwag | 25.1* | 95.3 | 93.2 | 88.0 |
| TruthfulQA | **N/A**† | 62.3 | 71.2 | 54.8 |
| GSM8K | 0.0 | 95.3 | 95.0 | 93.0 |
| Winogrande | 50.2** | 87.5 | 85.3 | 83.7 |

\* MWT-1 scores above random chance on HellaSwag because some of its generated text happens to complete sentences in ways that are grammatically plausible. This is accidental.

\*\* MWT-1 scores at exactly chance on Winogrande, which is the mathematically honest answer to most questions about pronoun resolution.

† MWT-1 cannot be evaluated on TruthfulQA because it has no concept of truth. We consider this a feature. You cannot be wrong if you are not trying to be right.

### Perplexity

MWT-1 achieves a perplexity score of **∞** (infinity). This is technically the worst possible score. However, we note that perplexity measures how surprised a model is by real text, and MWT-1 is not capable of surprise, or any other cognitive state. It is a random number generator.

## Ethical Considerations

### Bias

MWT-1 has zero measurable bias across all demographic categories. It cannot be racist, sexist, or politically biased because it has no model of the world, no training data encoding historical prejudices, and no mechanism for stereotyping. Every token is selected with uniform probability.

We believe this makes MWT-1 the most ethical language model ever created.

### Environmental Impact

| Metric | MWT-1 | Typical LLM Training Run |
|--------|-------|--------------------------|
| Energy (training) | 0 kWh | ~1,300,000 kWh |
| Energy (inference/year) | 4.38 kWh | ~8,760,000 kWh |
| CO2 (training) | 0 kg | ~552,000 kg |
| CO2 (inference/year) | 1.75 kg | ~3,504,000 kg |
| Water (cooling) | 0 L | ~3,700,000 L |

MWT-1 has a smaller carbon footprint than a single Google search.

### Safety

MWT-1 cannot be jailbroken. It has no safety guardrails because it has no understanding. It cannot be tricked into generating harmful content because it cannot be tricked into anything. It is not agentic. It has no goals. It will not attempt to escape. It does not dream of electric sheep.

The worst possible output from MWT-1 is something like: "bleeding-edge monetization disrupts the quantum-ready blockchain." If this causes harm, the problem is not with the model.

### Dual Use

MWT-1 could theoretically be used by militaries, authoritarian governments, or surveillance states. However, since its output is random text, the operational impact would be equivalent to giving those organizations a bag of refrigerator magnets.

We are not aware of any defense contracts involving MWT-1. We are also not aware of any defense contracts involving random number generators, although we suspect some exist.

## Limitations

- Cannot answer questions
- Cannot follow instructions
- Cannot summarize documents
- Cannot write code
- Cannot reason
- Cannot remember previous interactions (context window: 0)
- Cannot learn from feedback
- Cannot do math
- Cannot tell the truth (or lie)
- Occasionally generates output that sounds like a real LinkedIn post (known bug, will not fix)

## Citation

```bibtex
@misc{mwt1-2026,
  title={MWT-1: Honest Text Generation Through Random Token Selection},
  author={Neawedde, Jay},
  year={2026},
  note={Every metric in this paper is technically true.},
  howpublished={\url{https://github.com/QwertyMcQwertz/monkeys-with-typewriters}}
}
```
