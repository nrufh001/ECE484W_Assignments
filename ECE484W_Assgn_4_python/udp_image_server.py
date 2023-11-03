import socket

# Create a UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(("0.0.0.0", 12345))  # Bind to the port you're listening on

while True:
    # Receive data from Qt C++ application
    data, addr = server_socket.recvfrom(1024)  # Adjust buffer size as needed

    # Process the received image data
    # You may need to deserialize and display/save the image
    # Depending on the format, you may use OpenCV or PIL

    # Example: Saving the received image as a file
    with open("received_image.jpg", "wb") as f:
        f.write(data)

