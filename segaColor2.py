import numpy as np
from PIL import Image
from scipy.spatial import cKDTree
import itertools
from PyQt6.QtWidgets import (QApplication, QMainWindow, QWidget, QPushButton, 
                             QVBoxLayout, QHBoxLayout, QLabel, QFileDialog, QColorDialog)
from PyQt6.QtGui import QPixmap, QImage, QColor
from PyQt6.QtCore import Qt
import sys

class SegaColorizer(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Sega Genesis Image Colorizer")
        self.setGeometry(100, 100, 1200, 800)
        
        self.zoom_factor = 1.0  # Initialize zoom factor

        # Initialize full Sega color palette
        self.colors = [
            tuple(color)
            for color in itertools.product(range(0, 256, 8), repeat=3)
        ]
        self.converted_image = None
        
        # Create main widget and layout
        main_widget = QWidget()
        self.setCentralWidget(main_widget)
        layout = QHBoxLayout(main_widget)
        
        # Create image areas
        images_widget = QWidget()
        images_layout = QHBoxLayout(images_widget)
        
        # Original image panel
        original_widget = QWidget()
        original_layout = QVBoxLayout(original_widget)
        self.original_label = QLabel("Original Image")
        self.original_image_label = QLabel()
        self.original_image_label.setMinimumSize(400, 400)
        self.original_image_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        original_layout.addWidget(self.original_label)
        original_layout.addWidget(self.original_image_label)
        
        # Converted image panel
        converted_widget = QWidget()
        converted_layout = QVBoxLayout(converted_widget)
        self.converted_label = QLabel("Converted Image")
        self.converted_image_label = QLabel()
        self.converted_image_label.setMinimumSize(400, 400)
        self.converted_image_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        converted_layout.addWidget(self.converted_label)
        converted_layout.addWidget(self.converted_image_label)
        
        images_layout.addWidget(original_widget)
        images_layout.addWidget(converted_widget)
        
        # Buttons under images
        buttons_widget = QWidget()
        buttons_layout = QHBoxLayout(buttons_widget)
        
        load_button = QPushButton("Load Image")
        load_button.clicked.connect(self.load_image)
        save_button = QPushButton("Save Image")
        save_button.clicked.connect(self.save_image)
        
        # Zoom in/out buttons added to the buttons widget
        zoom_in_button = QPushButton("Zoom In")
        zoom_in_button.clicked.connect(self.zoom_in)
        zoom_out_button = QPushButton("Zoom Out")
        zoom_out_button.clicked.connect(self.zoom_out)
        
        buttons_layout.addWidget(load_button)
        buttons_layout.addWidget(save_button)
        buttons_layout.addWidget(zoom_in_button)
        buttons_layout.addWidget(zoom_out_button)
        
        # Create color palette area
        palette_widget = QWidget()
        palette_layout = QVBoxLayout(palette_widget)
        palette_layout.addWidget(QLabel("Color Palette"))
        
        # Create color grid
        color_grid = QWidget()
        grid_layout = QVBoxLayout(color_grid)
        self.color_buttons = []
        
        for row in range(4):
            row_widget = QWidget()
            row_layout = QHBoxLayout(row_widget)
            for col in range(4):
                i = row * 4 + col
                btn = QPushButton()
                btn.setFixedSize(60, 60)
                btn.setStyleSheet(f"background-color: rgb(0,0,0)")
                btn.index = i  # Store the index directly on the button
                btn.clicked.connect(lambda checked, btn=btn: self.change_color(btn.index))
                row_layout.addWidget(btn)
                self.color_buttons.append(btn)
            grid_layout.addWidget(row_widget)
        
        palette_layout.addWidget(color_grid)
        
        # Main layout assembly
        main_layout = QVBoxLayout()
        main_layout.addWidget(images_widget)
        main_layout.addWidget(buttons_widget)
        
        # Add everything to main window
        layout.addLayout(main_layout)
        layout.addWidget(palette_widget)
        
    def load_image(self):
        file_name, _ = QFileDialog.getOpenFileName(self, "Open Image", "", "Image Files (*.png *.jpg *.bmp)")
        if file_name:
            self.current_image = Image.open(file_name)
            self.display_original_image()
            self.convert_and_display()
            
    def display_original_image(self):
        img = self.current_image.convert('RGB')
        data = img.tobytes('raw', 'RGB')
        qimg = QImage(data, img.size[0], img.size[1], QImage.Format.Format_RGB888)
        pixmap = QPixmap.fromImage(qimg)
        scaled_pixmap = pixmap.scaled(400, 400, Qt.AspectRatioMode.KeepAspectRatio)
        self.original_image_label.setPixmap(scaled_pixmap)
    
    def convert_and_display(self):
        if hasattr(self, 'current_image'):
            rgb_image = self.current_image.convert('RGB')
            tree = cKDTree(self.colors)
            pixel_array = np.array(rgb_image)
            
            height, width = pixel_array.shape[:2]
            for y in range(height):
                for x in range(width):
                    pixel = pixel_array[y, x]
                    closest_color_idx = tree.query(pixel[:3])[1]
                    closest_color = self.colors[closest_color_idx]
                    pixel_array[y, x] = closest_color
            
            self.converted_image = Image.fromarray(pixel_array)
            self.converted_image = self.converted_image.quantize(colors=16)

            palette = self.converted_image.getpalette() or []
            # Ensure the palette has 48 values (16 colors * 3)
            if len(palette) < 48:
                palette += [0] * (48 - len(palette))
            
            for i in range(16):
                r, g, b = palette[i*3:i*3+3]
                self.color_buttons[i].setStyleSheet(f"background-color: rgb({r},{g},{b})")
            
            self.update_converted_display()

    def update_converted_display(self):
        # Update display of converted image using the current zoom factor.
        if self.converted_image:
            rgb_converted = self.converted_image.convert('RGB')
            data = rgb_converted.tobytes('raw', 'RGB')
            qimg = QImage(data, rgb_converted.width, rgb_converted.height, QImage.Format.Format_RGB888)
            pixmap = QPixmap.fromImage(qimg)
            # Scale based on zoom factor
            new_width = int(pixmap.width() * self.zoom_factor)
            new_height = int(pixmap.height() * self.zoom_factor)
            scaled_pixmap = pixmap.scaled(new_width, new_height, Qt.AspectRatioMode.KeepAspectRatio)
            self.converted_image_label.setPixmap(scaled_pixmap)
    
    def zoom_in(self):
        self.zoom_factor *= 1.25  # Increase zoom factor by 25%
        self.update_converted_display()

    def zoom_out(self):
        self.zoom_factor /= 1.25  # Decrease zoom factor by 25%
        self.update_converted_display()
        
    def change_color(self, index):
        color = QColorDialog.getColor()
        if color.isValid():
            # Find closest Sega color
            tree = cKDTree(self.colors)
            pixel = (color.red(), color.green(), color.blue())
            closest_color_idx = tree.query(pixel)[1]
            closest_color = self.colors[closest_color_idx]

            # Update palette of the converted image
            palette = list(self.converted_image.getpalette())
            palette[index*3:index*3+3] = closest_color
            self.converted_image.putpalette(palette)

            # Update the button color
            self.color_buttons[index].setStyleSheet(f"background-color: rgb{closest_color}")

            # Refresh the display using the modified converted image
            self.update_converted_display()
        
    def save_image(self):
        if self.converted_image:
            save_path, _ = QFileDialog.getSaveFileName(self, "Save Image", "", "PNG Files (*.png)")
            if save_path:
                self.converted_image.save(save_path)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = SegaColorizer()
    window.show()
    sys.exit(app.exec())
