#!/usr/bin/env bash
# vbw-witness.sh — Build witnessing script for MWT-1 firmware
#
# Assembles a VBW-compatible attestation bundle from build artifacts.
# Produces manifest.json and provenance.json following the VBW/SLSA spec.
#
# Usage:
#   ./scripts/vbw-witness.sh \
#     --source-hash <sha256> \
#     --output-hash <sha256> \
#     --env-file <path> \
#     --deps-file <path> \
#     --transcript <path> \
#     --output-dir <path>

set -euo pipefail

SOURCE_HASH=""
OUTPUT_HASH=""
ENV_FILE=""
DEPS_FILE=""
TRANSCRIPT=""
OUTPUT_DIR="vbw-bundle"

while [[ $# -gt 0 ]]; do
  case $1 in
    --source-hash) SOURCE_HASH="$2"; shift 2 ;;
    --output-hash) OUTPUT_HASH="$2"; shift 2 ;;
    --env-file)    ENV_FILE="$2"; shift 2 ;;
    --deps-file)   DEPS_FILE="$2"; shift 2 ;;
    --transcript)  TRANSCRIPT="$2"; shift 2 ;;
    --output-dir)  OUTPUT_DIR="$2"; shift 2 ;;
    *) echo "Unknown argument: $1" >&2; exit 1 ;;
  esac
done

if [[ -z "$SOURCE_HASH" || -z "$OUTPUT_HASH" ]]; then
  echo "Error: --source-hash and --output-hash are required" >&2
  exit 1
fi

mkdir -p "$OUTPUT_DIR/links"

TIMESTAMP=$(date -u +%Y-%m-%dT%H:%M:%SZ)
GIT_SHA=$(git rev-parse HEAD 2>/dev/null || echo "unknown")
GIT_REF=$(git symbolic-ref HEAD 2>/dev/null || echo "unknown")
BUILDER_ID="${GITHUB_SERVER_URL:-https://github.com}/${GITHUB_REPOSITORY:-local}/actions/runs/${GITHUB_RUN_ID:-0}"

# Read environment JSON if provided
ENV_JSON="{}"
if [[ -n "$ENV_FILE" && -f "$ENV_FILE" ]]; then
  ENV_JSON=$(cat "$ENV_FILE")
fi

# Read dependencies JSON if provided
DEPS_JSON="[]"
if [[ -n "$DEPS_FILE" && -f "$DEPS_FILE" ]]; then
  DEPS_JSON=$(cat "$DEPS_FILE")
fi

# Copy transcript if provided
if [[ -n "$TRANSCRIPT" && -f "$TRANSCRIPT" ]]; then
  cp "$TRANSCRIPT" "$OUTPUT_DIR/transcript.txt"
fi

# Generate manifest.json
cat > "$OUTPUT_DIR/manifest.json" <<MANIFEST_EOF
{
  "vbw_version": "0.1.0",
  "witness_type": "build",
  "timestamp": "$TIMESTAMP",
  "source": {
    "repository": "${GITHUB_REPOSITORY:-local}",
    "ref": "$GIT_REF",
    "sha": "$GIT_SHA",
    "tree_hash": "$SOURCE_HASH"
  },
  "environment": $ENV_JSON,
  "dependencies": $DEPS_JSON,
  "artifacts": [
    {
      "name": "firmware.bin",
      "digest": {
        "sha256": "$OUTPUT_HASH"
      }
    }
  ],
  "builder_id": "$BUILDER_ID"
}
MANIFEST_EOF

# Compute manifest hash
MANIFEST_HASH=$(sha256sum "$OUTPUT_DIR/manifest.json" | cut -d' ' -f1)

# Generate SLSA v1 provenance
cat > "$OUTPUT_DIR/provenance.json" <<PROV_EOF
{
  "_type": "https://in-toto.io/Statement/v1",
  "subject": [
    {
      "name": "firmware.bin",
      "digest": {
        "sha256": "$OUTPUT_HASH"
      }
    }
  ],
  "predicateType": "https://slsa.dev/provenance/v1",
  "predicate": {
    "buildDefinition": {
      "buildType": "https://github.com/kmay89/vbw/build/v1",
      "externalParameters": {
        "source": {
          "uri": "git+https://github.com/${GITHUB_REPOSITORY:-local}@${GIT_REF}",
          "digest": {
            "sha256": "$SOURCE_HASH"
          }
        },
        "builderImage": "platformio/platformio-ci"
      },
      "resolvedDependencies": []
    },
    "runDetails": {
      "builder": {
        "id": "$BUILDER_ID"
      },
      "metadata": {
        "buildStartedOn": "$TIMESTAMP",
        "buildFinishedOn": "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
      }
    }
  }
}
PROV_EOF

# Generate in-toto layout placeholder
cat > "$OUTPUT_DIR/layout.json" <<LAYOUT_EOF
{
  "_type": "layout",
  "steps": [
    {
      "name": "build-firmware",
      "expected_command": ["pio", "run", "--environment", "d1_mini"],
      "expected_materials": [
        ["MATCH", "firmware/*", "WITH", "PRODUCTS", "FROM", "checkout"]
      ],
      "expected_products": [
        ["CREATE", ".pio/build/d1_mini/firmware.bin"]
      ]
    }
  ],
  "inspect": []
}
LAYOUT_EOF

# Generate in-toto link for the build step
cat > "$OUTPUT_DIR/links/vbw-build.link" <<LINK_EOF
{
  "_type": "link",
  "name": "build-firmware",
  "materials": {
    "firmware/monkeys.ino": {
      "sha256": "$(sha256sum firmware/monkeys.ino 2>/dev/null | cut -d' ' -f1 || echo 'unknown')"
    },
    "platformio.ini": {
      "sha256": "$(sha256sum platformio.ini 2>/dev/null | cut -d' ' -f1 || echo 'unknown')"
    }
  },
  "products": {
    ".pio/build/d1_mini/firmware.bin": {
      "sha256": "$OUTPUT_HASH"
    }
  },
  "byproducts": {
    "return-value": 0,
    "stderr": "",
    "stdout": ""
  },
  "environment": {}
}
LINK_EOF

# Copy policy into bundle
if [[ -f "vbw-policy.json" ]]; then
  cp vbw-policy.json "$OUTPUT_DIR/vbw-policy.json"
fi

# Compute bundle hash (deterministic: sort files, hash each, hash the hashes)
BUNDLE_HASH=$(find "$OUTPUT_DIR" -type f | sort | xargs sha256sum | sha256sum | cut -d' ' -f1)

echo ""
echo "=== VBW Build Witness Complete ==="
echo "  Manifest:    $OUTPUT_DIR/manifest.json"
echo "  Provenance:  $OUTPUT_DIR/provenance.json"
echo "  Layout:      $OUTPUT_DIR/layout.json"
echo "  Link:        $OUTPUT_DIR/links/vbw-build.link"
echo "  Policy:      $OUTPUT_DIR/vbw-policy.json"
echo "  Bundle hash: $BUNDLE_HASH"
echo ""
echo "To verify (when vbw is available):"
echo "  vbw verify $OUTPUT_DIR --artifact .pio/build/d1_mini/firmware.bin"
echo ""
echo "To verify manually:"
echo "  sha256sum .pio/build/d1_mini/firmware.bin"
echo "  # Compare with: $(cat "$OUTPUT_DIR/manifest.json" | grep -o '"sha256": "[^"]*"' | head -1)"
