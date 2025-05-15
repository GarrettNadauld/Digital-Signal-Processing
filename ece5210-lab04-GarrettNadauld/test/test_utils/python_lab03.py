import random
import re

import numpy as np
from scipy import io as spio

def read_filter():

    filename = "../Core/Inc/filter.h"

    f = open(filename,"r")

    h = []
    for n, line in enumerate(f.readlines()):
        if len(line.split()) != 5:
            continue
        for element in line.split():
            element = re.sub(",","",element)
            element = re.sub("f","",element)
            h.append(float(element))

    return np.array(h)
            
if __name__ == "__main__":
    M = 100
    fs = 48e3

    h_lpf = read_filter()
    h_notch = [1, -1.4142135623730954, 1]
    
    N = np.random.randint(5000,10000)
    x = np.random.uniform(-10000,10000,size=N).astype(np.int16)
    x_float = x.astype(np.float32)
    
    notched = np.convolve(x_float[::M],h_notch)[:-2]

    x_sub = np.zeros_like(x_float)
    x_sub[::M] = notched

    y_float = np.convolve(h_lpf, x_sub)[:-(h_lpf.size-1)]

    y = y_float.astype(np.int16)
    
    spio.mmwrite("x_out",x[:,None])
    spio.mmwrite("y_out",y[:,None])
