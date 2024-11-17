#!/usr/bin/env python3
import sys
import os

# Print headers
print("Content-Type: text/html")
print("Status: 200 OK")
print()  # Empty line to separate headers from body


for key, value in os.environ.items():
    print(f"{key}: {value}")
