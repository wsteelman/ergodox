import sys

# main
color = int(sys.argv[1], 0)

blue = (color & 0xFF) * 0xFFFF / 0xFF * float(sys.argv[2])
color = color >> 8

green = (color & 0xFF) * 0xFFFF / 0xFF * float(sys.argv[2])
color = color >> 8

red = (color & 0xFF) * 0xFFFF / 0xFF * float(sys.argv[2])
color = color >> 8

print('{red}, {green}, {blue}'.format(red=hex(int(red)), green=hex(int(green)), blue=hex(int(blue))))
