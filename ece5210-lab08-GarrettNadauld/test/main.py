# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.


import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import pandas as pd

data = pd.read_csv('scope_14.csv', encoding= "ISO-8859-1")
frequency = data[[' Frequency (Hz)']].to_numpy()
gain = data[[' Gain (dB)']].to_numpy()
phase = data[[' Phase (°)']].to_numpy()

# Specifications
fp = 2.4e3  # Passband edge frequency in Hz
fs = 3e3    # Stopband edge frequency in Hz
delta_p = 0.1
delta_s = 0.001

gp = 20 * np.log10(delta_p + 1)
gs = -20 * np.log10(delta_s)

# Convert frequencies to digital frequency
wp = fp / 24000
ws = fs / 24000

# Determine filter order using the Elliptic approximation
N, wn = signal.ellipord(wp, ws, gp, gs, analog=False)

# Design the Elliptic filter
b, a = signal.ellip(N, gp, gs, wn, btype='low', output='ba')
# b, a = signal.ellip(N, delta_p, delta_s, wn, btype='lowpass', output='ba')
sos = signal.tf2sos(b, a)
# Plot the frequency response
w, h = signal.freqz(b, a, worN=5000)
freq = w * 48000 / (2 * np.pi)
mag = 20 * np.log10(np.abs(h))
pha = np.angle(h)
frequencies = w * 48000 / (2 * np.pi)  # Convert angular frequency to Hz
magnitude_dB = 20 * np.log10(np.abs(h))  # Convert magnitude to dB
z_ellip, p_ellip, _ = signal.tf2zpk(b, a)

plt.figure(figsize=(10, 6))
plt.subplot(2, 1, 1)
plt.plot(freq, mag, label='Calculated Magnitude')
plt.plot(frequency, gain, label='Measured Magnitude')
#plt.plot(freq*2*np.pi/48000, gain, color = 'g', label = "Measured", linestyle = "--")
plt.title('Magnitude Response')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.xlim(0,5000)
plt.grid(True)
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(freq, pha, label='Calculated Phase')
plt.plot(frequency, np.radians(phase), label='Measured Phase')
plt.title('Phase Response')
plt.xlabel('Frequency (rad/sample)')
plt.ylabel('Phase (radians)')
plt.xlim(0, 5000)
plt.legend()

plt.tight_layout()
plt.savefig('magnitude_phase_plot.png')
plt.grid(True)
plt.show()

# Plot poles and zeros
plt.figure(figsize=(6,6))
theta = np.linspace(0, 2*np.pi, 1000)
x = np.cos(theta)
y = np.sin(theta)
plt.grid(True)

plt.plot(x, y, label='Unit Circle')
plt.plot([-3, 3], [0, 0], 'k-', lw=2)  # x-axis
plt.plot([0, 0], [-3, 3], 'k-', lw=2)  # y-axis
plt.xlim(-3, 3)
plt.ylim(-3, 3)

plt.plot(np.real(p_ellip), np.imag(p_ellip), 'rx', label='Poles')
plt.plot(np.real(z_ellip), np.imag(z_ellip), 'ro', label='Zeros')
plt.title('Poles and Zeros')
plt.xlabel('Real')
plt.ylabel('Imaginary')
plt.legend()
plt.tight_layout()
plt.savefig('poles_zeros_plot.png')
plt.grid(True)
plt.show()
