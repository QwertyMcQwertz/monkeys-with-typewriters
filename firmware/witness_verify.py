#!/usr/bin/env python3
"""
MWT-1 Witness Chain Verifier — External Verification Tool

Fetches the witness chain from an MWT-1 device and independently
verifies all hash links and Ed25519 signatures.

This is the EXTERNAL verifier — independent of the device firmware.
If this script says PASS and the device says PASS, the chain is intact.
If they disagree, something is very wrong (or very interesting).

Usage:
    python3 witness_verify.py --host 192.168.1.47
    python3 witness_verify.py --file chain_export.json

Dependencies:
    pip install requests pynacl
    (or: pip install requests ed25519)
"""

import argparse
import hashlib
import json
import struct
import sys

try:
    from nacl.signing import VerifyKey
    from nacl.exceptions import BadSignatureError
    NACL_AVAILABLE = True
except ImportError:
    NACL_AVAILABLE = False

try:
    import requests
    REQUESTS_AVAILABLE = True
except ImportError:
    REQUESTS_AVAILABLE = False


def sha256(data: bytes) -> bytes:
    return hashlib.sha256(data).digest()


def compute_entry_hash(sequence: int, prev_hash: bytes, payload_hash: bytes) -> bytes:
    """Compute entry hash matching the ESP8266 firmware implementation.
    SHA-256(sequence_le32 || prev_hash || payload_hash)
    """
    seq_bytes = struct.pack('<I', sequence)  # uint32_t little-endian
    return sha256(seq_bytes + prev_hash + payload_hash)


def hex_to_bytes(hex_str: str) -> bytes:
    return bytes.fromhex(hex_str)


def verify_chain(chain_data: dict) -> bool:
    """Verify the full witness chain.

    Returns True if all entries pass verification.
    """
    pubkey_hex = chain_data.get('device_pubkey', '')
    entries = chain_data.get('entries', [])
    chain_length = chain_data.get('chain_length', 0)

    if not pubkey_hex:
        print("ERROR: No device public key in export")
        return False

    if len(entries) != chain_length:
        print(f"WARNING: Expected {chain_length} entries, got {len(entries)}")

    if not entries:
        print("Chain is empty — nothing to verify")
        return True

    pubkey_bytes = hex_to_bytes(pubkey_hex)

    if NACL_AVAILABLE:
        verify_key = VerifyKey(pubkey_bytes)
    else:
        print("WARNING: PyNaCl not available — skipping signature verification")
        print("         Install with: pip install pynacl")
        verify_key = None

    expected_prev = b'\x00' * 32  # genesis entry has zero prev_hash
    errors = 0

    for i, entry in enumerate(entries):
        seq = entry['seq']
        entry_hash = hex_to_bytes(entry['entry_hash'])
        prev_hash = hex_to_bytes(entry['prev_hash'])
        payload_hash = hex_to_bytes(entry['payload_hash'])
        signature = hex_to_bytes(entry['signature'])

        # Check 1: Sequence is monotonic
        if seq != i:
            print(f"  FAIL [{i}]: Expected sequence {i}, got {seq}")
            errors += 1
            continue

        # Check 2: prev_hash links to previous entry
        if prev_hash != expected_prev:
            print(f"  FAIL [{i}]: prev_hash mismatch")
            print(f"    Expected: {expected_prev.hex()}")
            print(f"    Got:      {prev_hash.hex()}")
            errors += 1
            continue

        # Check 3: Recompute entry hash
        computed_hash = compute_entry_hash(seq, prev_hash, payload_hash)
        if entry_hash != computed_hash:
            print(f"  FAIL [{i}]: entry_hash mismatch")
            print(f"    Expected: {computed_hash.hex()}")
            print(f"    Got:      {entry_hash.hex()}")
            errors += 1
            continue

        # Check 4: Verify Ed25519 signature
        if verify_key is not None:
            try:
                verify_key.verify(entry_hash, signature)
            except BadSignatureError:
                print(f"  FAIL [{i}]: Invalid Ed25519 signature")
                errors += 1
                continue

        # Update chain state for next entry
        expected_prev = entry_hash

        if (i + 1) % 500 == 0:
            print(f"  ... verified {i + 1}/{len(entries)} entries")

    print()
    if errors == 0:
        print(f"PASS: All {len(entries)} entries verified")
        print(f"  Hash chain: intact")
        if verify_key:
            print(f"  Signatures: valid (Ed25519)")
        print(f"  Device key: {pubkey_hex}")
        return True
    else:
        print(f"FAIL: {errors} error(s) in {len(entries)} entries")
        return False


def fetch_from_device(host: str, port: int = 80) -> dict:
    """Fetch witness chain export from device."""
    if not REQUESTS_AVAILABLE:
        print("ERROR: 'requests' library not available")
        print("       Install with: pip install requests")
        sys.exit(1)

    url = f"http://{host}:{port}/witness/export"
    print(f"Fetching chain from {url} ...")

    try:
        resp = requests.get(url, timeout=30)
        resp.raise_for_status()
        return resp.json()
    except requests.RequestException as e:
        print(f"ERROR: Failed to fetch chain: {e}")
        sys.exit(1)


def load_from_file(path: str) -> dict:
    """Load chain export from JSON file."""
    with open(path, 'r') as f:
        return json.load(f)


def main():
    parser = argparse.ArgumentParser(
        description='MWT-1 Witness Chain Verifier — External verification tool'
    )
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('--host', help='Device IP address or hostname')
    group.add_argument('--file', help='Path to chain export JSON file')
    parser.add_argument('--port', type=int, default=80, help='Device port (default: 80)')

    args = parser.parse_args()

    print("=" * 50)
    print("MWT-1 Witness Chain Verifier")
    print("SecuraCV PWK — External Verification")
    print("=" * 50)
    print()

    if args.host:
        chain_data = fetch_from_device(args.host, args.port)
    else:
        chain_data = load_from_file(args.file)

    print(f"Device public key: {chain_data.get('device_pubkey', 'unknown')}")
    print(f"Chain length:      {chain_data.get('chain_length', 0)}")
    print()
    print("Verifying chain...")
    print()

    success = verify_chain(chain_data)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()
