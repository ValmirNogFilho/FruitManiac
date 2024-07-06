from PIL import Image

# Abra uma imagem
image = Image.open('apple.png')

# Converta a imagem para o modo RGB (se não estiver nesse modo)
image = image.convert('RGB')

# Obtenha as dimensões da imagem
width, height = image.size


with open("apple.sprite", "a") as f:
    # Acesse e manipule cada pixel
    for x in range(width):
        for y in range(height):
            # Obtenha o valor do pixel (R, G, B)
            r, g, b = image.getpixel((x, y))
            if(r==g==b==0): 
                r, g, b = 192, 224, 224
            f.write(f"{r//32} {g//32} {b//32}\n")