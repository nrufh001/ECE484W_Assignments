import cv2
import os

environment_var_overlay_state = os.environ.get("OVERLAY_STATE", "0")

if environment_var_overlay_state == "1":
    print("overlay on")
else:
    print("overlay off")

# load images
original_image = cv2.imread('images/original_image.jpg')
overlay_image = cv2.imread('images/overlay_image.jpg')

# size of images
width, height = 200, 200

# this is super weird, but just resizing the overlay image
# so that everything fits nicely together
ar = float(width) / overlay_image.shape[1] # get the aspect ratio
adjusted_height = int(overlay_image.shape[0] * ar) # get the new height
overlay_image = cv2.resize(overlay_image, (width, adjusted_height)) # and finally resize it

# just grabbing the new overlay demensions
overlay_height, overlay_width, _ = overlay_image.shape

# coordinates to place in top left corner of the image
# can always change to move the image around
x, y = 50, 50

# grabbing pts on original image for the overlay image to be placed
pts = original_image[y:y+overlay_height, x:x+overlay_width]

# placing overlay image onto the original
result = cv2.addWeighted(pts, 0, overlay_image, 1, 0)

# replacing the original pts with the result
original_image[y:y+overlay_height, x:x+overlay_width] = result

# and FINALLY saving the image
cv2.imwrite('images/result_image.jpg', original_image)
