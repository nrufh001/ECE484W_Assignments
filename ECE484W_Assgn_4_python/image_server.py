import socket

HOST = '0.0.0.0'
PORT = 12347

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))

server_socket.listen(1)

print("Server is listening for {}:{}".format(HOST, PORT))

client_socket, client_address = server_socket.accept()
print("Connected to {}".format(client_address))

with open('received_image.png', 'wb') as image_file:
	data = b''
	try:
		while True:
			print("Received data length: {}".format(len(data)))
			chunk = client_socket.recv(4096)
			if not chunk:
				break
			data += chunk
		image_file.write(data)
	except Exception as e:
		print("Error receiving image data: {}".format(e))

client_socket.close()
server_socket.close()

print("Image received and saved.")
