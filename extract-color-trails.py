from PIL import Image
im = Image.open("ColorGrad.png")
(width, height) = im.size
pixels = [im.getpixel((x, 25)) for x in range(width)] # grab 3500 pixels from 7000 pixel image

reds = ["0"] * 4000 + [str(p[0]) for p in pixels] + ["255"] * 3250
greens = ["0"] * 4000 + [str(p[1]) for p in pixels] + ["0"] * 3250
blues = ["255"] * 4000 + [str(p[2]) for p in pixels] + ["0"] * 3250

trail_length = len(blues)
print(trail_length)

with open('blue-hour-baltimore/blue-hour-color-trails.h', 'w') as f:
    f.write("#define TOTAL_FRAMES %s\n\nbyte red_trail[TOTAL_FRAMES] = {%s};\nbyte green_trail[TOTAL_FRAMES] = {%s};\nbyte blue_trail[TOTAL_FRAMES] = {%s};"
        % (trail_length, ",".join(reds), ",".join(greens), ",".join(blues)))
