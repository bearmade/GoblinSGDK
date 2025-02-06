
# This script will take a sprite sheet and split it into individual gifs

from PIL import Image
import os

def split_sprite_sheet(image_path):
    # Get the source image
    img = Image.open(image_path)
    
    # Get dimensions from user
    rows = int(input("How many rows? "))
    cols = int(input("How many columns? "))
    
    # Calculate frame dimensions
    width = img.width // cols
    height = img.height // rows
    
    # Create output directory if it doesn't exist
    output_dir = "output_gifs"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Process each row
    for row in range(rows):
        frames = []
        # Get all frames in this row
        for col in range(cols):
            # Calculate frame position
            left = col * width
            top = row * height
            right = left + width
            bottom = top + height
            
            # Extract and append frame
            frame = img.crop((left, top, right, bottom))
            frames.append(frame)
        
        # Save animation for this row
        output_path = f"{output_dir}/row_{row}.gif"
        frames[0].save(
            output_path,
            save_all=True,
            append_images=frames[1:],
            duration=100,
            loop=0
        )

if __name__ == "__main__":
    image_path = input("Enter path to sprite sheet PNG: ")
    split_sprite_sheet(image_path)
