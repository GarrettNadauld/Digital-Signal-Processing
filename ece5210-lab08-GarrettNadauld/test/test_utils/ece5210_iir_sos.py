import numpy as np
from scipy import signal
from scipy import io as spio

N = 10000
x = np.random.uniform(-1000, 1000, size=N)

sos_coef = np.array(
    [
        [0.9, 0.8, 0.7, 1, 0.6, 0.5],
        [0.6, 0.5, 0.4, 1, 0.4, 0.3],
        [0.3, 0.2, 0.1, 1, 0.2, 0.1],
    ]
)

y = signal.sosfilt(sos_coef, x)

spio.mmwrite("x_out", x[:, None])
spio.mmwrite("y_out", y[:, None])
