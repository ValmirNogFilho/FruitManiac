from PIL import Image

# Abra uma imagem
image = Image.open('fim.png')

# Converta a imagem para o modo RGB (se não estiver nesse modo)
image = image.convert('RGBA')

# Obtenha as dimensões da imagem
width, height = image.size

with open("fim.back", "a") as f:
    # Acesse e manipule cada pixel
    for x in range(width):
        for y in range(height):
            # Obtenha o valor do pixel (R, G, B)
            r, g, b, a = image.getpixel((x, y))
            if(a == 0): 
                r, g, b = 192, 224, 224
            f.write(f"{r//32} {g//32} {b//32}\n")