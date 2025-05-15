import random
import re

from matplotlib import pyplot as plt
import numpy as np
from scipy import io as spio


if __name__ == "__main__":
    M = 100

    h_notch = [1, -1.4142135623730954, 1]

    N = np.random.randint(5000, 10000)
    x = np.random.uniform(-10000, 10000, size=N).astype(np.int16)

    x_float = x.astype(np.float32)
    x_sub = np.convolve(x_float[::M], h_notch)[:-2]

    h_interp = 1 - np.arange(100) / M
    h_interp = np.hstack((h_interp[::-1], h_interp[1:]))

    x_e = np.zeros(len(x_sub) * M)
    x_e[::M] = x_sub

    x_i = np.convolve(x_e, h_interp)[: len(x)]

    y = x_i.astype(np.int16)

    spio.mmwrite("x_out", x[:, None])
    spio.mmwrite("y_out", y[:, None])
