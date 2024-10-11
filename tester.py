import socket
import threading
import time
import random

SERVER_IP = '127.0.0.1'
SERVER_PORT = 4545
NUM_CLIENTS = 10000
TEST_DURATION = 200

def client_thread(client_id):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((SERVER_IP, SERVER_PORT))
    
    start_time = time.time()
    requests_sent = 0
    
    try:
        while time.time() - start_time < TEST_DURATION:
            message = f"Client {client_id} message {requests_sent}"
            client.send(message.encode())
            
            response = client.recv(1024)
            if not response:
                break
            
            print(f"Client {client_id} received: {response.decode()}")
            
            requests_sent += 1
            time.sleep(random.uniform(0.1, 0.5))  # Random delay between requests
    except Exception as e:
        print(f"Client {client_id} error: {e}")
    finally:
        client.close()
    
    print(f"Client {client_id} finished. Sent {requests_sent} requests.")

def run_test():
    threads = []
    for i in range(NUM_CLIENTS):
        t = threading.Thread(target=client_thread, args=(i,))
        threads.append(t)
        t.start()
    
    for t in threads:
        t.join()

if __name__ == "__main__":
    print(f"Starting load test with {NUM_CLIENTS} clients for {TEST_DURATION} seconds...")
    run_test()
    print("Load test completed.")