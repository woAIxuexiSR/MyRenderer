import math
import matplotlib.pyplot as plt
import numpy as np

def gaussian(mu, theta, x):

    return 1.0 / math.sqrt(2 * math.pi) / theta * math.exp(-(x - mu) * (x - mu) / 2 / theta / theta)

x = np.arange(-10, 10, 0.1)
y = np.array([gaussian(0, 5, item) for item in x])

plt.scatter(x, y)
plt.show()