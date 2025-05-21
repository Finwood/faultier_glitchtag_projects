#!/usr/bin/env python3
# obfuscate.py

import sys

def obfuscate_bytes(data: bytes, key: int) -> bytes:
    return bytes(b ^ key for b in data)

def format_c_array(data: bytes, var_name: str) -> str:
    hex_bytes = ', '.join(f'0x{b:02X}' for b in data)
    return f"unsigned char {var_name}[] = {{ {hex_bytes} }};\nunsigned int {var_name}_len = {len(data)};"

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <string> [key]")
        sys.exit(1)

    s = sys.argv[1].encode('utf-8')
    key = int(sys.argv[2]) if len(sys.argv) >= 3 else 0xAA  # default key 0xAA

    obf = obfuscate_bytes(s, key)
    print(f"/* Obfuscated with XOR key 0x{key:02X} */")
    print(format_c_array(obf, "foo"))

if __name__ == "__main__":
    main()
