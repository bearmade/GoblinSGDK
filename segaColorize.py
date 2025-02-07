# This script will take a PNG image and replace each pixel with the closest Sega Genesis color, 
# then save the result as a new PNG image with only 16 colors.

import numpy as np
from PIL import Image
from scipy.spatial import cKDTree
import itertools

# Generate Sega Genesis color palette using itertools
sega_colors = [
    tuple(color)
    for color in itertools.product(range(0, 256, 8), repeat=3)
]

# Create KD-tree for efficient nearest neighbor search
tree = cKDTree(sega_colors)

# Load and convert the input image to RGB mode
image_path = "./res/backdrop2.png"
image = Image.open(image_path).convert("RGB")

# Iterate through each pixel of the image and replace it with the closest Sega Genesis color
width, height = image.size
pixel_array = np.array(image)  # Convert image to numpy array for efficient pixel manipulation

for y in range(height):
    for x in range(width):
        pixel = pixel_array[y, x]
        closest_color_idx = tree.query(pixel)[1]
        closest_color = sega_colors[closest_color_idx]
        pixel_array[y, x] = closest_color

output_image = Image.fromarray(pixel_array)

# Reduce the image to 16 colors
output_image = output_image.quantize(colors=16)

# Save the modified image
output_path = "./res/backdrop2.png"

output_image.save(output_path)
