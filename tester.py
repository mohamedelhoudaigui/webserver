import socket
import threading
import time
import random
from collections import Counter
import queue
import argparse

class HTTPClient:
    def __init__(self, host, port, request_queue):
        self.host = host
        self.port = port
        self.request_queue = request_queue
        self.response = None
        self.status_code = 0
        self.headers = {}
        self.body = ""
        self.error = None
        self.checks_failed = []
        self.response_time = 0

    def parse_response(self, response):
        try:
            # Split into headers and body
            parts = response.split('\r\n\r\n', 1)
            headers_part = parts[0] if len(parts) > 0 else ''
            body = parts[1] if len(parts) > 1 else ''

            # Parse status line
            headers_lines = headers_part.split('\r\n')
            status_line = headers_lines[0]
            status_code = int(status_line.split(' ')[1]) if len(status_line.split(' ')) > 1 else 0

            # Parse headers
            headers = {}
            for line in headers_lines[1:]:
                if ': ' in line:
                    key, value = line.split(': ', 1)
                    headers[key] = value
            return status_code, headers, body.strip()
        except Exception as e:
            return 0, {}, response  # Return raw response on parse failure

    def read_http_response(self, sock):
        data = b''
        while True:
            chunk = sock.recv(4096)
            if not chunk:
                break
            data += chunk
            if b'\r\n\r\n' in data:
                headers_end = data.find(b'\r\n\r\n') + 4
                headers = data[:headers_end].decode()
                body = data[headers_end:].decode()
                break
        return headers + body

    def validate_response(self, test_case):
        # Check status code
        if self.status_code != test_case.get('expected_status', 200):
            self.checks_failed.append(f"Status {self.status_code} != {test_case['expected_status']}")

        # Check required headers
        for hdr in test_case.get('required_headers', []):
            if hdr not in self.headers:
                self.checks_failed.append(f"Missing header: {hdr}")

        # Check body content
        if 'check_body' in test_case and test_case['check_body'] not in self.body:
            self.checks_failed.append("Body content mismatch")

        # Check connection closure
        if test_case.get('check_connection_close'):
            if 'Connection' in self.headers and self.headers['Connection'].lower() != 'close':
                self.checks_failed.append("Connection not closed")

    def send_request(self):
        start_time = time.time()
        try:
            # Create socket
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(10)  # 10 seconds timeout

            # Connect to server
            sock.connect((self.host, self.port))

            # Get test case from queue
            test_case = self.request_queue.get()

            # Send request
            sock.send(test_case['request'].encode())

            # Receive response
            self.response = self.read_http_response(sock)
            self.status_code, self.headers, self.body = self.parse_response(self.response)
            self.response_time = time.time() - start_time

            # Validate response
            self.validate_response(test_case)

        except Exception as e:
            self.error = str(e)
        finally:
            try:
                sock.close()
            except:
                pass

class LoadTester:
    def __init__(self, host, port, num_clients):
        self.host = host
        self.port = port
        self.num_clients = num_clients
        self.results = []
        self.request_queue = queue.Queue()
        self.success_count = 0
        self.failure_count = 0
        self.total_response_time = 0
        self.error_details = []
        self.checks_failed = []

        # Prepare test cases
        self.prepare_requests()

    def prepare_requests(self):
        self.test_cases = [
            {
                'request': "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n",
                'expected_status': 200,
                'required_headers': ['Server', 'Date', 'Content-Type'],
            },
            {
                'request': "GET /nonexistent HTTP/1.1\r\nHost: localhost\r\n\r\n",
                'expected_status': 404,
            },
            {
                'request': "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Length: 23\r\n\r\nuser=test&password=test",
                'expected_status': 200,
            },
            {
                'request': "DELETE /resource HTTP/1.1\r\nHost: localhost\r\n\r\n",
                'expected_status': 204,
            },
            {
                'request': "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n",
                'expected_status': 200,
                'check_connection_close': True
            },
            {
                'request': "POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nHello\r\n6\r\n World\r\n0\r\n\r\n",
                'expected_status': 200,
                'check_body': 'Hello World'
            },
            {
                'request': "INVALID / HTTP/1.1\r\nHost: localhost\r\n\r\n",
                'expected_status': 400
            },
            {
                'request': "GET / HTTP/1.1\r\n\r\n",  # Missing Host header
                'expected_status': 400
            }
        ]

        # Fill the queue with random test cases
        for _ in range(self.num_clients):
            self.request_queue.put(random.choice(self.test_cases))

    def run_test(self):
        print(f"\nStarting load test with {self.num_clients} clients...")
        print(f"Target: {self.host}:{self.port}")
        print("=" * 50)

        # Create and start threads
        threads = []
        start_time = time.time()

        for i in range(self.num_clients):
            client = HTTPClient(self.host, self.port, self.request_queue)
            thread = threading.Thread(target=client.send_request)
            threads.append((thread, client))
            thread.start()

            # Print progress
            if (i + 1) % 100 == 0:
                print(f"Started {i + 1} clients...")

        # Wait for all threads to complete
        for thread, client in threads:
            thread.join()
            if client.error or client.checks_failed:
                self.failure_count += 1
                if client.error:
                    self.error_details.append(client.error)
                self.checks_failed.extend(client.checks_failed)
            else:
                self.success_count += 1
                self.total_response_time += client.response_time
            self.results.append(client)

        total_time = time.time() - start_time
        self.print_results(total_time)

    def print_results(self, total_time):
        print("\nTest Results:")
        print("=" * 50)
        print(f"Total Clients: {self.num_clients}")
        print(f"Successful Requests: {self.success_count}")
        print(f"Failed Requests: {self.failure_count}")
        print(f"Total Time: {total_time:.2f} seconds")

        if self.success_count > 0:
            avg_response_time = self.total_response_time / self.success_count
            print(f"Average Response Time: {avg_response_time:.3f} seconds")
            print(f"Requests per Second: {self.success_count / total_time:.2f}")

        print("\nError Distribution:")
        error_counts = Counter(self.error_details)
        for error, count in error_counts.items():
            print(f"{error}: {count} occurrences")

        print("\nValidation Failures:")
        failure_counts = Counter(self.checks_failed)
        for failure, count in failure_counts.items():
            print(f"{failure}: {count}")

def main():
    parser = argparse.ArgumentParser(description='Web Server Load Tester')
    parser.add_argument('--host', default='localhost', help='Server host')
    parser.add_argument('--port', type=int, default=8080, help='Server port')
    parser.add_argument('--clients', type=int, default=100, help='Number of concurrent clients')
    args = parser.parse_args()

    try:
        tester = LoadTester(args.host, args.port, args.clients)
        tester.run_test()
    except KeyboardInterrupt:
        print("\nTest interrupted by user")
    except Exception as e:
        print(f"\nError: {e}")

if __name__ == "__main__":
    main()