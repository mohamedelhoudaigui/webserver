#!/usr/bin/env python3
import sys
import os

for key, value in os.environ.items():
    print(f"{key}: {value}")
