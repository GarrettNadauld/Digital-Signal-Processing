import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import pandas as pd

data = pd.read_csv('scope_22.csv', encoding= "ISO-8859-1")
frequency_hp = data[[' Frequency (Hz)']].to_numpy()
gain_hp = data[[' Gain (dB)']].to_numpy()
phase_hp = data[[' Phase (*)']].to_numpy()

data = pd.read_csv('scope_21.csv', encoding= "ISO-8859-1")
frequency_lp = data[[' Frequency (Hz)']].to_numpy()
gain_lp = data[[' Gain (dB)']].to_numpy()
phase_lp = data[[' Phase (*)']].to_numpy()

# frequency_lp = frequency_lp/3
# frequency_hp = frequency_hp/3

h_coefficients = [
    0.02055109, 0.03641191, 0.03820462, 0.022702, -0.00959248, -0.05405346,
    -0.103017, -0.14716693, -0.17784654, 0.81120165, -0.17784654, -0.14716693,
    -0.103017, -0.05405346, -0.00959248, 0.022702, 0.03820462, 0.03641191,
    0.02055109
]

l_coefficients = [
    0.001456, -0.02666342, -0.0431693, -0.03926709, -0.01154633, 0.03689976,
    0.09646407, 0.15380424, 0.19505105, 0.21009135, 0.19505105, 0.15380424,
    0.09646407, 0.03689976, -0.01154633, -0.03926709, -0.0431693, -0.02666342,
    0.001456
]


freq, l_response = signal.freqz(l_coefficients, worN=8000)
freq, h_response = signal.freqz(h_coefficients, worN=8000)
freq = 24000*freq/np.pi

plt.figure(figsize=(10, 6))
plt.subplot(2, 1, 1)
plt.plot(freq, 20*np.log10(l_response), label='Calculated Low-Pass Magnitude')
plt.plot(freq, 20*np.log10(h_response), label='Calculated High-Pass Magnitude')
plt.plot(frequency_hp, gain_hp, label='Measured High-Pass Magnitude')
plt.plot(frequency_lp, gain_lp, label='Measured Low-Pass Magnitude')
# plt.plot(freq*2*np.pi/48000, gain, color = 'g', label = "Measured", linestyle = "--")
plt.title('Magnitude Response')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.xlim(0,24000)
plt.grid(True)
plt.legend()

plt.subplot(2, 1, 2)
# plt.plot(freq, pha, label='Calculated Phase')
plt.plot(freq, np.angle(l_response), label='Calculated Low-Pass Phase')
plt.plot(freq, np.angle(h_response), label='Calculated High-Pass Phase')
plt.plot(frequency_lp, np.radians(phase_lp), label='Measured Low-Pass Phase')
plt.plot(frequency_hp, np.radians(phase_hp), label='Measured High-Pass Phase')

plt.title('Phase Response')
plt.xlabel('Frequency (rad/sample)')
plt.ylabel('Phase (radians)')
plt.xlim(0, 24000)
plt.legend()

plt.tight_layout()
plt.savefig('magnitude_phase_plot.png')
plt.grid(True)
plt.show()

# Plot poles and zeros
# plt.figure(figsize=(6,6))
# theta = np.linspace(0, 2*np.pi, 1000)
# x = np.cos(theta)
# y = np.sin(theta)
# plt.grid(True)

# plt.plot(x, y, label='Unit Circle')
# plt.plot([-3, 3], [0, 0], 'k-', lw=2)  # x-axis
# plt.plot([0, 0], [-3, 3], 'k-', lw=2)  # y-axis
# plt.xlim(-3, 3)
# plt.ylim(-3, 3)
#
# plt.plot(np.real(p_ellip), np.imag(p_ellip), 'rx', label='Poles')
# plt.plot(np.real(z_ellip), np.imag(z_ellip), 'ro', label='Zeros')
# plt.title('Poles and Zeros')
# plt.xlabel('Real')
# plt.ylabel('Imaginary')
# plt.legend()
# plt.tight_layout()
# plt.savefig('poles_zeros_plot.png')
# plt.grid(True)
# plt.show()
