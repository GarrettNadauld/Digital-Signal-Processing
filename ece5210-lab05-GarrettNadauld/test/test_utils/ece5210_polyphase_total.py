import numpy as np
from scipy import signal
from scipy import io as spio


def naive_decimation(x, h, M):
    y = np.convolve(h, x)[: x.size]

    return y[::M]


NUM_TAPS = 900
M = 100

print("your code is being tested with M=%i and NUM_TAPS=%i" % (M, NUM_TAPS))

h_aa = signal.firwin(NUM_TAPS, 1 / M).astype(np.float32)
h_60hz = [1.0, -1.4142135623730954, 1.0]

x = np.random.randint(-15000, 15000, size=M * 100).astype(np.float32)

dec = naive_decimation(x, h_aa, M)

filt = np.convolve(dec, h_60hz)[: dec.size]

x_e = np.zeros_like(x)
x_e[::M] = filt


y = M * np.convolve(x_e, h_aa)[: x.size]


spio.mmwrite("x_out", x[:, None].astype(int))
spio.mmwrite("y_out", y[:, None].astype(int))
