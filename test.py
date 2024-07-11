import socket

socket_table = []

def main():
	for i in range(0, 10):
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect(("10.13.11.1", 4545))
		socket_table.append(s)
	for i in range(0, 10):
		s.send(f"Hello from {i}\0".encode())


if __name__ == '__main__':
	main()