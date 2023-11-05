import tkinter as tk
from PIL import Image, ImageTk

class ImageAdjustmentApp:
    def __init__(self, root, image_path):
        self.root = root
        self.image_path = image_path
        self.original_image = Image.open(image_path)
        self.adjusted_image = self.original_image.copy()

        self.brightness_scale = tk.Scale(root, label="Brightness", from_=-100, to=100, orient="horizontal")
        self.contrast_scale = tk.Scale(root, label="Contrast", from_=0, to=200, orient="horizontal")
        self.overlay_checkbox = tk.Checkbutton(root, text="Overlay", command=self.toggle_overlay)

        self.image_label = tk.Label(root)

        self.brightness_scale.pack()
        self.contrast_scale.pack()
        self.overlay_checkbox.pack()
        self.image_label.pack()

        self.update_image()
        self.brightness_scale.bind("<Motion>", self.update_image)
        self.contrast_scale.bind("<Motion>", self.update_image)

    def update_image(self, _=None):
        brightness = self.brightness_scale.get()
        contrast = self.contrast_scale.get() / 100.0
        overlay = self.overlay_checkbox.instate(['selcted'])

        # Apply brightness and contrast adjustments
        self.adjusted_image = self.original_image.point(lambda p: p * contrast + brightness)

        # Display the updated image
        self.display_image(self.adjusted_image)

    def display_image(self, image):
        photo = ImageTk.PhotoImage(image=image)
        self.image_label.config(image=photo)
        self.image_label.image = photo

    def toggle_overlay(self):
        self.update_image()

def main():
    root = tk.Tk()
    root.title("Image Adjustment App")
    app = ImageAdjustmentApp(root, "images/original_image.jpg")
    root.mainloop()

if __name__ == "__main__":
    main()

