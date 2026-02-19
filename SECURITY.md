# Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| 1.0.x   | ✅        |

## Reporting a Vulnerability

If you discover a security vulnerability in MWT-1, please consider the following before reporting:

1. **The model generates random text.** If the random text happens to contain sensitive information, this is a statistical coincidence and not a data breach.

2. **The REST API has no authentication.** This is by design. There is nothing to protect. The worst an attacker can do is generate random text slightly faster than intended.

3. **The WiFi credentials are hardcoded in the firmware.** Yes. We know. The device costs $3. If someone steals your WiFi password from it, you have lost $3 and a WiFi password. This is not a CVE.

4. **The ESP8266 has known vulnerabilities.** The ESP8266 was designed in 2014. It has the security posture of a 2014 IoT device, which is to say, none. MWT-1 does not attempt to improve on this because it is a random text generator, not a security appliance.

## Threat Model

MWT-1's threat model is as follows:

- **Confidentiality:** MWT-1 has no secrets. It has 150 words. They're in the source code. They're in the README. They're on the landing page. There is nothing to exfiltrate.
- **Integrity:** An attacker could modify the vocabulary list to include different words. The output would still be random text. The impact is negligible.
- **Availability:** An attacker could denial-of-service the ESP8266 by sending many requests. The device would stop generating random text. We do not consider this a meaningful loss.

## Responsible Disclosure

If, despite all of the above, you still believe you have found a legitimate security vulnerability, please email jamesneawedde@protonmail.com with the subject line "MWT-1 Security" and we will respond with the gravity that the situation warrants.

## Build Integrity (VBW)

MWT-1 firmware builds are witnessed using [Verified Build Witness (VBW)](https://github.com/kmay89/vbw), which produces cryptographic attestations proving the firmware binary matches the audited source.

Every CI build:

1. Hashes the source tree via `git ls-files`
2. Records the build environment (OS, PlatformIO version, toolchain)
3. Records resolved dependencies
4. Builds the firmware and captures a full transcript
5. Hashes the output binary
6. Assembles a SLSA v1 provenance statement and in-toto layout

### Verifying a Release Binary

Download the attestation bundle from the GitHub Actions artifacts for any build, then:

```bash
# Manual verification: compare the firmware hash against the manifest
sha256sum firmware.bin
cat vbw-bundle/manifest.json | python3 -c "import sys,json; print(json.load(sys.stdin)['artifacts'][0]['digest']['sha256'])"

# VBW verification (when vbw CLI is available):
vbw verify vbw-bundle/ --artifact firmware.bin
```

### Build Policy

The `vbw-policy.json` in the repository root enforces:

- **No private network references** — builds must not depend on internal infrastructure
- **No embedded secrets** — no credentials in provenance artifacts
- **Digest required** — all artifacts must include SHA-256 hashes
- **Approved builders only** — restricted to `kmay89/` and `QwertyMcQwertz/` GitHub organizations

## Bug Bounty

We do not have a bug bounty program. The entire project costs $3. We cannot in good conscience offer financial rewards for finding bugs in a random number generator.
