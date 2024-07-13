# import socket

# socket_table = []

# def main():
# 	for i in range(0, 10):
# 		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 		s.connect(("10.13.11.1", 4545))
# 		socket_table.append(s)
# 		while True:
# 			for i in range(0, 10):
# 				s.send(f"Hello from {i}\0".encode())


# if __name__ == '__main__':
# 	main()

import socket
import threading
import time

# Configuration
SERVER_IP = '127.0.0.1'  # Replace with your server's IP address
SERVER_PORT = 4545     # Replace with your server's port
NUMBER_OF_CLIENTS = 400  # Number of simultaneous client connections
NUMBER_OF_REQUESTS = 1 # Number of requests per client

def client_thread(client_id):
    try:
        # Create a socket object
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((SERVER_IP, SERVER_PORT))

        for i in range(NUMBER_OF_REQUESTS):
            # Send data
            message = f"Client {client_id}: Message {i}"
            client_socket.sendall(message.encode())

            # Receive response
            response = client_socket.recv(1024)
            print(f"Client {client_id} received: {response.decode()}")

        # Close the socket
        client_socket.close()
    except Exception as e:
        print(f"Client {client_id} encountered an error: {e}")

def main():
    threads = []

    # Create and start client threads
    for i in range(NUMBER_OF_CLIENTS):
        thread = threading.Thread(target=client_thread, args=(i,))
        thread.start()
        threads.append(thread)
        time.sleep(0.01)  # Small delay to stagger client connections

    # Wait for all threads to complete
    for thread in threads:
        thread.join()

if __name__ == "__main__":
    main()
