import random

import numpy as np
from scipy import io as spio



if __name__ == "__main__":
    num_taps = 7
    
    N = np.random.randint(1000,5000)

    h = np.ones(num_taps)/num_taps
    x = np.random.uniform(-15000,15000,size=N).astype(np.int16)

    y = np.convolve(x.astype(np.single),
                    h.astype(np.single))[:N].astype(np.int16)

    spio.mmwrite("x_out",x[:,None])
    spio.mmwrite("y_out",y[:,None])
