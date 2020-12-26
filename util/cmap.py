import numpy as np
from matplotlib import cm

r = np.linspace(0, 1, 257)[:256]
cmapInferno = np.array(cm.inferno(r)*255, dtype=np.uint8)

r = np.linspace(0, 1, 101)
cmapGreys = np.array(cm.Greys(r)*255, dtype=np.uint8)

print("#pragma once\n\nunsigned int CM_GREYS[101] {")
for r, g, b, a in cmapGreys:
    print("    " + str((r << 24) | (g << 16) | (b << 8) | a) + ",")
print("};")

# print("\n\nunsigned int CM_INFERNO[256] {")
# for r, g, b, a in cmapInferno:
#     print("    " + str((r << 24) | (g << 16) | (b << 8) | a) + ",")
# print("};")

# print("logic [255:0][31:0] CM_INFERNO = {")
# for r, g, b, a in cmapInferno[::-1]:
#     print("    {32'h" + hex((r << 24) | (g << 16) | (b << 8) | a)[2:] + "},")
# print("};\n")

