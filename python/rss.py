## Red snake TCP socket server

import socket, keepalive
from ast import literal_eval
import snake

HOST = '0.0.0.0' # Listen on all available interfaces
PORT = 5002     # Port to listen on for red snakes(non-privileged ports are > 1023)
SEG_SIZE = 20

game = snake.SnakeGame('R')

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	keepalive.set(s)
	s.bind((HOST, PORT))
	s.listen()
	print(f"Server listening on {HOST}:{PORT}")
	while True:
		# Accept new connections in an infinite loop.
		client_sock, client_addr = s.accept()
		print('New connection from', client_addr)

		while True:
			data = client_sock.recv(1024)
			if data:
				# print(f"Received: {data.decode()}")
				float_list = literal_eval(data.decode())
				received = data.decode()

				#divide the data received from server from pixel to grid
				yx1, yy1, yx2, yy2, rx1, ry1, rx2, ry2, ax, ay = [int(i)//SEG_SIZE for i in float_list]
				print("red", rx1,ry1, "yellow:", yx1, yy1, "apple:", ax, ay)

				game.update(yx1, yy1, rx1, ry1, ax, ay)
				data_to_proxy = game.move()

				client_sock.sendall(data_to_proxy.encode())	

				# hang up?
				if received == "Close" or received == b'Close':
					# client wants to hang up
					print("hanging up..")
					break
			else:
				break

		client_sock.close()
	s.close()