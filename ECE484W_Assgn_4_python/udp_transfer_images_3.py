import socket

# UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(("0.0.0.0", 12345))  # bind to the port you're listening on

buffer = b"" # stores the data received

complete_marker_original_image = b"transferred_original"
complete_marker_overlay_image = b"transferred_overlay"
marker_overlay_on_and_off = b"overlay"
overlay = 0
print("Listening for message...")

while True:
    # receive data from Qt
    data, addr = server_socket.recvfrom(1024)  # buffer size instructions say to set it to this
    print("data received: ", len(data), "bytes")
    buffer += data
    
    if complete_marker_original_image in data:
        print("original image data received! reconstructing image...")
        image_data = buffer[:-len(complete_marker_original_image)]

        with open("images/original_image.jpg", "wb") as f:
             f.write(image_data)

        buffer = b""
        print("Image received")

    if complete_marker_overlay_image in data:
        print("overlay image data received! reconstructing image...")
        image_data = buffer[:-len(complete_marker_overlay_image)]

        with open("images/overlay_image.jpg", "wb") as f:
             f.write(image_data)
        buffer = b""
        print("Image received")
    
    if marker_overlay_on_and_off in data:
        if overlay == 0:
            print("\nturning overlay on")
            overlay = 1
        else:
            print("\nturning overlay off")
            overlay = 0
