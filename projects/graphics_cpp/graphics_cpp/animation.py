from PIL import Image

frames = []
 
for frame_number in range(0, 200):
    frame = Image.open(f'images/image{frame_number}.png')
    frames.append(frame)
 
print(len(frames))

frames[0].save(
    'animation.gif',
    save_all=True,
    append_images=frames[1:],
    optimize=True,
    duration=100,
    loop=0
)
