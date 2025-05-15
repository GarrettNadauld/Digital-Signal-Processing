import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# def dtft(x, omega, n=None):
#     if n is None:
#         n = np.arange(len(x))
#     X = np.zeros(len(omega), dtype=complex)
#     for i, freq in enumerate(omega):
#         X[i] = np.sum(x * np.exp(-1j * freq * n))
#     return X
#
# def phase_correct(phase_array, freq_omega_array, nd):
#     corr = phase_array - freq_omega_array*nd
#     return corr

def main():
    # Define the impulse response
    # h = [1, -2 * np.cos(0.0025 * np.pi), 1]
    # h_down = [1, -2 * np.cos((120 * np.pi / 480)), 1]

    # CSV Data
    data = pd.read_csv('scope_5.csv', encoding='ISO-8859-1')
    freq = data[' Frequency (Hz)'].to_numpy()
    amp = data[' Amplitude (Vpp)'].to_numpy()
    gain = data[' Gain (dB)'].to_numpy()
    phase = data[' Phase (°)'].to_numpy()

    # nd = np.mean(np.radians(phase)/(np.pi*2*freq/480))
    # phase = phase_correct(phase, freq, nd)

    w = freq#freq*2*np.pi/480
    # W = freq*2*np.pi/48000

    # H_x = dtft(h, W)
    # H_x_down = dtft(h_down, w)

    data = pd.read_csv('scope_6.csv', encoding='ISO-8859-1', skiprows=1)
    t = data['second'].to_numpy()
    v1 = data['Volt'].to_numpy()
    v2 = data['Volt2'].to_numpy()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
    ax1.plot(w, gain, label='Measured')
    # ax1.plot(w, 20*np.log10(np.abs(H_x_down)), label='Analytical')
    ax1.set_title('Magnitude vs Frequency')
    ax1.set_xlabel('Frequency (rad/s)')
    ax1.set_ylabel('Magnitude (db)')
    ax1.legend()
    ax2.plot(w, phase, label='Measured')
    # ax2.plot(w, np.angle(H_x_down,deg=True), label='Analytical')
    ax2.set_title('Phase vs Frequency')
    ax2.set_xlabel('Frequency (rad/s)')
    ax2.set_ylabel('Phase (degree)')
    ax2.legend()
    plt.tight_layout()
    plt.show()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
    ax1.plot(t, v1, label='Passthrough Signal')
    ax1.set_title("ECG Signal with 60 Hz Noise")
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Voltage (V)')
    ax1.legend(loc=1)
    ax2.plot(t, v2, label='Filtered Signal')
    ax2.set_title('Voltage vs Time')
    ax2.set_xlabel('Time (s)')
    ax2.set_ylabel('Voltage (V)')
    ax2.legend(loc=1)
    plt.title("Filtered ECG Signal")
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()