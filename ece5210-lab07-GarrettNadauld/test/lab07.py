import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import freqz, tf2zpk
import pandas as pd

# Load fixed and floating-point coefficients from files
h_fixed = np.loadtxt('fixed_coefficients.txt', dtype=int, skiprows=1)
h_float = np.loadtxt('floating_coefficients.txt', skiprows=1)
h_fixed = ((h_fixed)/(float)(1<<12))
# h_fixed = h_fixed.astype(float)
# h_fixed = h_fixed.astype(float)
# Compute frequency response of the filters
w, h_fixed_freqz = freqz(h_fixed, worN=512)#, whole=True)
_, h_float_freqz = freqz(h_float, worN=512)#, whole=True)
p = [1]

z_fixed, p_fixed, _ = tf2zpk(h_fixed, p)
z_float, p_float, _ = tf2zpk(h_float, p)

data = pd.read_csv('scope_12.csv', encoding='ISO-8859-1')
freq = data[' Frequency (Hz)'].to_numpy()
amp = data[' Amplitude (Vpp)'].to_numpy()
gain = data[' Gain (dB)'].to_numpy()
phase1 = data[' Phase (°)'].to_numpy()

w1 = freq * 2 * np.pi / 48000

plt.figure(figsize=(8, 6))
plt.stem(h_fixed, label='Fixed-point')
plt.stem(h_float, label='Floating-point', markerfmt='ro', linefmt='r--')
plt.title('Filter Coefficients')
plt.xlabel('Coefficient Index')
plt.ylabel('Coefficient Value')
plt.legend()
plt.tight_layout()
plt.savefig('coefficients_plot.png')
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

plt.plot(np.real(p_fixed), np.imag(p_fixed), 'bx', label='Fixed-point Poles')
plt.plot(np.real(z_fixed), np.imag(z_fixed), 'bo', label='Fixed-point Zeros')
plt.plot(np.real(p_float), np.imag(p_float), 'rx', label='Floating-point Poles')
plt.plot(np.real(z_float), np.imag(z_float), 'ro', label='Floating-point Zeros')
plt.plot(0, 0, 'bx')
plt.plot(0, 0, 'rx')
plt.title('Poles and Zeros')
plt.xlabel('Real')
plt.ylabel('Imaginary')
plt.legend()
plt.tight_layout()
plt.savefig('poles_zeros_plot.png')
plt.grid(True)
plt.show()

plt.figure(figsize=(8, 6))
plt.subplot(2, 1, 1)
plt.plot(w, 20*np.log10(np.abs(h_fixed_freqz)), label='Fixed-point Magnitude')
plt.plot(w, 20*np.log10(np.abs(h_float_freqz)), label='Floating-point Magnitude')
plt.plot(w1, gain, label='Linear Measured')#, color='Blue', linestyle='--')
plt.title('Magnitude Response')
plt.xlabel('Frequency (rad/sample)')
plt.ylabel('Magnitude')
plt.grid(True)
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(w, np.angle(h_fixed_freqz), label='Fixed-point Phase')
plt.plot(w, np.angle(h_float_freqz), label='Floating-point Phase')
plt.plot(w1, (np.radians(phase1)), label='Linear Measured')#, color='Blue', linestyle='--')
plt.title('Phase Response')
plt.xlabel('Frequency (rad/sample)')
plt.ylabel('Phase (radians)')
plt.legend()

plt.tight_layout()
plt.savefig('magnitude_phase_plot.png')
plt.grid(True)
plt.show()
