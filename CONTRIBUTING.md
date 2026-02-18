# Contributing to Monkeys with Typewriters

Thank you for your interest in improving MWT-1. We welcome contributions from the community.

## How to Contribute

### Vocabulary Proposals

The most impactful contribution is expanding the MWT-1 vocabulary. New tokens are reviewed against the following criteria:

1. **Corporate emptiness.** The word must sound important while conveying minimal information. "Synergy" is the gold standard. "Chair" is too concrete.
2. **Buzzword density.** Preference is given to words that appear in at least 3 of the following: TechCrunch headlines, LinkedIn thought leadership posts, YC demo day pitches, McKinsey reports, or Gartner Magic Quadrants.
3. **Plausible deniability.** When the word appears in MWT-1 output, a reasonable person should not immediately realize the text is randomly generated. If a VP would forward it to their team without reading it, the word qualifies.

### Pull Request Process

1. Fork the repository
2. Add your tokens to the appropriate array in `firmware/monkeys.ino`
3. Update `MODEL_CARD.md` with the new vocabulary count
4. Submit a PR with the subject line: `vocab: add [category] tokens`
5. In the PR description, explain why each proposed token contributes to the project's mission of generating text that is indistinguishable from enterprise communications

### Code Contributions

The inference engine is 47 lines of C. We intend to keep it under 100. If your contribution makes the engine more complex, please justify why a random text generator needs the additional complexity.

Contributions that add machine learning, neural networks, attention mechanisms, or any form of actual intelligence will be rejected. This is a random number generator. It will remain a random number generator. We are not building AGI. We are making a point.

### Documentation

Documentation improvements are always welcome. The bar is that the documentation must maintain a completely serious, professional tone at all times. The humor comes from the contrast between the presentation and the reality. If your documentation winks at the reader, it will be rejected.

## Code of Conduct

Be excellent to each other. Don't be the kind of person who would seriously pitch a random text generator to investors. Unless you're doing it ironically. In which case, we'd like to hear how it went.

## Style Guide

- **Firmware:** Arduino C style. No unnecessary abstractions. The code should be readable by someone who has never programmed before, because the entire model should be readable by someone who has never programmed before. That's the point.
- **Documentation:** AP style. Professional. No exclamation points. We are a serious AI company.
- **Landing page:** Full SaaS startup energy. Gradients. Metrics. Social proof. The works.

## Questions?

Open an issue. We respond to all inquiries with the same attention and care we put into our model architecture, which is to say, randomly.
