import random

import numpy as np
from scipy import io as spio

if __name__ == "__main__":
    SIZE = 35

    
    N = np.random.randint(1000,5000)

    n = np.arange(SIZE)
    h = (0.8)**n
    h /= sum(h)
    
    x = np.random.uniform(-10000,10000,size=N).astype(np.int16)

    y = np.convolve(x.astype(np.single),
                    h.astype(np.single))[:N].astype(np.int16)

    spio.mmwrite("x_out",x[:,None])
    spio.mmwrite("y_out",y[:,None])
