# Create glyphs images for a font
import os
import cairo
import gi
gi.require_version('Pango', '1.0')
gi.require_version('PangoCairo', '1.0')
from gi.repository import Pango, PangoCairo
import subprocess

def xec(cmd, decode=True, chomp=True, verbose=False):
  '''Run a command a returns its stdout output.

  decode -- run (utf8) decode of the resulting string
  chomp -- get rid of the last newline (like in perl)
  '''
  with subprocess.Popen(cmd,shell=True,stdout=subprocess.PIPE) as ph:
    if verbose:
      sys.stderr.write(cmd + '\n')
    res = b''
    maxsize = 1024
    while True:
      buf = ph.stdout.read()
      if len(buf)==0:
        break
      res += buf
    ph.wait()
  if decode:
    res=res.decode()
  if chomp:
    res = res[:-1]
  return res


# Directory to save the glyph images
output_dir = "../image-font"
os.makedirs(output_dir, exist_ok=True)

# Font settings
font_family = "ColorTube"  # Replace with your desired font family
font_size = 100  # Adjust the font size as needed

# Loop over the ASCII digits '0'-'9'
for char in '0123456789ABCDEF.:':
    org_char = char
    if char == ':':
      char = '.'

    # Create a temporary surface to measure the text extents
    temp_surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, 1, 1)
    temp_context = cairo.Context(temp_surface)

    # Create a Pango layout
    pango_layout = PangoCairo.create_layout(temp_context)
    pango_layout.set_text(char, -1)

    # Set the font description
    font_desc = Pango.FontDescription(f"{font_family} {font_size}")
    pango_layout.set_font_description(font_desc)

    # Get the text extents
    ink_rect, logical_rect = pango_layout.get_pixel_extents()

    # Create a new image surface with the exact size of the glyph
    width = logical_rect.width
    height = logical_rect.height
    surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, width, height)
    context = cairo.Context(surface)

    # Create a Pango layout for the actual rendering
    pango_layout = PangoCairo.create_layout(context)
    pango_layout.set_text(char, -1)
    pango_layout.set_font_description(font_desc)

    # Move the context to the top-left corner of the glyph
    context.move_to(-logical_rect.x, -logical_rect.y)

    # Render the text
    PangoCairo.show_layout(context, pango_layout)

    # If drawing a colon repeat the dot shifted in y
    if org_char == ':':
        # Move the context to the top-left corner of the glyph
        context.move_to(-logical_rect.x, -logical_rect.y - 50)
        PangoCairo.show_layout(context, pango_layout)
      

    # Save the image with the appropriate filename
    ascii_code = ord(org_char)
    hex_code = f"{ascii_code:02X}"
    image_path = os.path.join(output_dir, f"glyph{hex_code}.png")
    surface.write_to_png(image_path)

    print(f"Saved {image_path}")

