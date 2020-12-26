import sys
import numpy as np
import matplotlib.pyplot as plt


LOG_MAP_SIZE = 8
MATH_BITS = 12

MAP_SIZE = 1 << LOG_MAP_SIZE
MATH_ONE = 1 << MATH_BITS

with open(sys.argv[1]) as f:
    n = [float(t)/MATH_ONE for t in f.read().split()]

n = np.array(n).reshape((MAP_SIZE, MAP_SIZE))
plt.imshow(n, cmap="inferno")
plt.colorbar()
plt.savefig("results/temp.png")

    