import socket

# UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(("0.0.0.0", 12345))  # bind to the port you're listening on

buffer = b"" # stores the data received

complete_marker = b"image transferred"

while True:
    # receive data from Qt
    data, addr = server_socket.recvfrom(1024)  # buffer size instructions say to set it to this

    buffer += data
    
    if complete_marker in data:
        image_data = buffer[:-len(complete_marker)]

        with open("received_image.jpg", "wb") as f:
             f.write(data)

        buffer = b""
        print("Image received")

