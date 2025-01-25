import socket
import threading
import time
import sys
import random
from datetime import datetime
import queue
import argparse

class HTTPClient:
    def __init__(self, host, port, request_queue):
        self.host = host
        self.port = port
        self.request_queue = request_queue
        self.response = None
        self.error = None
        self.response_time = 0

    def send_request(self):
        start_time = time.time()
        try:
            # Create socket
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(10)  # 10 seconds timeout
            
            # Connect to server
            sock.connect((self.host, self.port))
            
            # Get request from queue
            request = self.request_queue.get()
            
            # Send request
            sock.send(request.encode())
            
            # Receive response
            response = []
            while True:
                data = sock.recv(4096)
                if not data:
                    break
                response.append(data)
            
            self.response = b''.join(response).decode('utf-8', errors='ignore')
            self.response_time = time.time() - start_time
            
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
        
        # Prepare different types of requests
        self.prepare_requests()

    def prepare_requests(self):
        requests = [
            # Basic GET request
            "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n",
            
            # POST request with body
            "POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 23\r\n\r\nuser=test&password=test\r\n",
            
            # GET request with query parameters
            "GET /search?q=test HTTP/1.1\r\nHost: localhost\r\n\r\n",
            
            # DELETE request
            "DELETE /resource HTTP/1.1\r\nHost: localhost\r\n\r\n",
            
            # Chunked transfer encoding request
            "POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n"
            "5\r\nHello\r\n"
            "6\r\n World!\r\n"
            "0\r\n\r\n"
        ]
        
        # Fill the queue with random requests
        for _ in range(self.num_clients):
            self.request_queue.put(random.choice(requests))

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
            if client.error:
                self.failure_count += 1
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
        error_counts = {}
        for client in self.results:
            if client.error:
                error_counts[client.error] = error_counts.get(client.error, 0) + 1

        for error, count in error_counts.items():
            print(f"{error}: {count} occurrences")

def main():
    parser = argparse.ArgumentParser(description='Web Server Load Tester')
    parser.add_argument('--host', default='localhost', help='Server host')
    parser.add_argument('--port', type=int, default=8080, help='Server port')
    parser.add_argument('--clients', type=int, default=1024, help='Number of concurrent clients')
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