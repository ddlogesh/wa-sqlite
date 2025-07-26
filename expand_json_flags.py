#!/usr/bin/env python3
import sys
import json

def main():
    with open(sys.argv[1], "r", encoding="utf-8") as f:
        data = json.load(f)

    json_str = json.dumps(data, separators=(',', ':'))
    print(json_str)

if __name__ == "__main__":
    main()
