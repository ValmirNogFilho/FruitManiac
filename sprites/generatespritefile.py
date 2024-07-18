from PIL import Image

image = Image.open('fim.png')

image = image.convert('RGBA')

width, height = image.size

with open("fim.back", "a") as f:
    for x in range(width):
        for y in range(height):
            r, g, b, a = image.getpixel((x, y))
            if(a == 0): 
                r, g, b = 192, 224, 224
            f.write(f"{r//32} {g//32} {b//32}\n")
