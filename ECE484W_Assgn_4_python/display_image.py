import cv2
import time

original_image_path = r'/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/original_image.jpg'
edited_image_path = r'/home/root/ECE484W_Assignments/ECE484W_Assgn_4_python/edited_image.jpg'

# window name for display
window_name = 'image'

while True:
    # reading the image
    image = cv2.imread(original_image_path)

    # displaying the image
    cv2.imshow(window_name, image)

    # check for user input to exit the loop
    key = cv2.waitKey(1) & 0xFF

    # if 'q' is pressed, exit the loop
    if key == ord('q'):
        break

    # sleep for a while before refreshing image
    time.sleep(1)

# close all open windows
cv2.destroyAllWindows()

