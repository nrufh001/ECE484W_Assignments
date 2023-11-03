import cv2
import time
import os
import socket

# Path to the images
original_image_path = '/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/original_image.jpg'
edited_image_path = '/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/edited_image.jpg'

# Window name for display
window_name = 'image'

# Create a socket for receiving brightness and contrast values
UDP_IP = '0.0.0.0'
UDP_PORT = 80
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

brightness = 0  # Initialize brightness
contrast = 0  # Initialize contrast


cv2.imshow(window_name, cv2.imread(original_image_path))

while True:
    # Receive 4 byte string (4 digit number)
    data, addr = sock.recvfrom(4)
    if data == "END":
        # Cleanup or exit as needed
        break

    # Convert data to brightness and contrast values (you may need to adjust this)
    brightness, contrast = data[:2], data[2:]

    # Reading the image
    if brightness == "00" and contrast == "00":
        image = cv2.imread(original_image_path)
    else:
        image = cv2.imread(edited_image_path)

    # Apply brightness and contrast
    brightness_value = int(brightness)
    contrast_value = int(contrast)
    alpha = 1 + brightness_value / 100.0
    beta = contrast_value

    image = cv2.convertScaleAbs(image, alpha=alpha, beta=beta)

    # Display the image
    cv2.imshow(window_name, image)

    # Check for user input to exit the loop
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

    # Sleep for a while before refreshing the image
    time.sleep(1)

# Close all open windows
cv2.destroyAllWindows()

