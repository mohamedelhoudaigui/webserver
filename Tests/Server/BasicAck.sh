#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <port>"
  exit 1
fi

PORT=$1
counter=0
URL="0.0.0.0:$PORT"

for i in {1..1000}
do
  response=$(curl -s $URL)
  if [ "$response" != "" ]; then
    counter=$((counter + 1))
  fi
done

echo "Total successful responses: $counter"