#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from PIL import Image, ImageDraw

def gen_color(color: str):
    img = Image.new('RGB', (30, 30), 'white')
    draw = ImageDraw.Draw(img)
    draw.ellipse([ 5, 5, 25, 25 ], fill=color, outline=None)
    img.save('./icon_{0}.png'.format(color))


if __name__ == '__main__':
    gen_color('red')
    gen_color('black')
