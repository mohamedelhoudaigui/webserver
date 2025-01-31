#!/bin/bash

SERVER_IP="127.0.0.1" # Replace with your server IP/hostname
SERVER_PORT="2222"    # Replace with your server port

# Test Case 1: Basic GET Request
test_basic_get() {
    echo "Test 1: Basic GET Request"
    printf "GET / HTTP/1.1\r\nHost: $SERVER_IP\r\n\r\n" | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 2: GET with Invalid HTTP Version
test_invalid_version() {
    echo "Test 2: GET with Invalid HTTP Version"
    printf "GET / HTTP/9001\r\nHost: $SERVER_IP\r\n\r\n" | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 3: Missing Headers
test_missing_headers() {
    echo "Test 3: GET with Missing Host Header"
    printf "GET / HTTP/1.1\r\n\r\n" | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 4: Large Headers
test_large_headers() {
    echo "Test 4: GET with Large Headers"
    printf "GET /index HTTP/1.1\r\nHost: $SERVER_IP\r\nrandom: 0GyAQA7ZEQzJWRwxqi6IMU6bJns6FiUsyfmUuuiNEhyrkg1PIQ5MqQPOYrdeKtMNy8SZnBIVOqOt8DituzNzkUDeZ9cgXo8FQ1nQMcfrsnzlZs36uQ83Sp9skTZVHnhNXkpUlf1jFqNNBOkrj0cA1YGdEacyvoVK2qL0rn3cGF1XQRo1Tvve5zV69A3KrheEWBBpgaj2SFBHDudjhqgOZD81pSvS7u0lEK3D0i4tY5EmUyJwoSrV5SIkOJdEaC9W9rykQ7SIGcSfBnwytsver70xD1iLsqivaP8ADHF1cP3SRFQoiT23g8UAeruVbBvFKvLav8kmfjBSeYRYPCtZJgiqaWqxC00v7YYOEcTR10PNTTyu6KsslBnGQ40SYAigFANzeCF9lroChurj\r\n\r\n" | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 5: Malformed Request Line (Extra Spaces)
test_malformed_request_line() {
    echo "Test 5: Malformed Request Line (Extra Spaces)"
    printf "  GET   /   HTTP/1.1  \r\nHost: $SERVER_IP\r\n\r\n" | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 6: Invalid Method
test_invalid_method() {
    echo "Test 6: Invalid HTTP Method"
    printf "FOO / HTTP/1.1\r\nHost: $SERVER_IP\r\n\r\n" | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 7: POST with Body and Chunked Encoding
test_chunked_encoding() {
    echo "Test 7: POST with Chunked Encoding"
    {
        printf "POST /upload HTTP/1.1\r\n"
        printf "Host: $SERVER_IP\r\n"
        printf "Transfer-Encoding: chunked\r\n\r\n"
        printf "5\r\nHello\r\n"
        printf "6\r\n World\r\n"
        printf "0\r\n\r\n"
    } | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 8: Body with No Content-Length or Chunked
test_body_without_cl() {
    echo "Test 8: POST with Body but No Content-Length/Chunked"
    {
        printf "POST / HTTP/1.1\r\n"
        printf "Host: $SERVER_IP\r\n\r\n"
        printf "This body has no Content-Length header"
    } | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 9: Special Characters in Headers/Body
test_special_chars() {
    echo "Test 9: Special Characters in Headers and Body"
    {
        printf "GET / HTTP/1.1\r\n"
        printf "Host: $SERVER_IP\r\n"
        printf "X-Special: \t\n\r\\\"\x00\r\n\r\n"
        printf "Body with \x00 null and \t tabs"
    } | nc -q 1 $SERVER_IP $SERVER_PORT
}

# Test Case 10: Incorrect Content-Length
test_incorrect_cl() {
    echo "Test 10: Incorrect Content-Length"
    {
        printf "POST / HTTP/1.1\r\n"
        printf "Host: $SERVER_IP\r\n"
        printf "Content-Length: 700\r\n\r\n"
        printf "This body is shorter than 100 bytes"
    } | nc -q 1 $SERVER_IP $SERVER_PORT
}

test_large_body() {
    echo "Test 10: Large Body Over 10240 Bytes with Special Characters"
    {
        # Generate headers with special characters
        printf "POST / HTTP/1.1\r\n"
        printf "Host: $SERVER_IP\r\n"
        printf "Content-Length: 10241\r\n\r\n"
        # Generate 10241-byte body with special characters
        for ((i=0; i<640; i++)); do
            printf "Chunk-%04d: \x00\x01\x02\t\n\r\\\"" $i
        done
        printf "X"  # Final byte to reach 10241
    } | nc -q 1 $SERVER_IP $SERVER_PORT
}
# Run all tests

#test_basic_get
#sleep 0.1
# test_invalid_version
# sleep 0.1
# test_missing_headers
# sleep 0.1
# test_large_headers
# sleep 0.1
# test_malformed_request_line
# sleep 0.1
# test_invalid_method
# sleep 0.1
#test_chunked_encoding
#sleep 0.1
# test_body_without_cl
# sleep 0.1
# test_special_chars
# sleep 0.1
 test_incorrect_cl
 sleep 0.1
#test_large_body
echo "All tests sent. Check your server logs to verify parsing."
