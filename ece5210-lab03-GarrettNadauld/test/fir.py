import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def dtft(x, omega, n=None):

    if n is None:
        n = np.arange(len(x))

    X = np.zeros_like(omega, dtype=np.complex128)

    for i, w in enumerate(omega):
        X[i] = np.sum(x * np.exp(-1j * w * n))

    return X

def phase_correct(phase_array, freq_omega_array, nd):

    phase_array = phase_array - freq_omega_array*nd

    return phase_array

def main():
    n = np.arange(0,33)
    # x = (0.8)**n
    x = (0.8**n)/(1-0.8**34)*0.2
    # Z = 1/(1-0.8*np.exp(-1j*w))

    # CSV Data
    data = pd.read_csv('scope_1.csv', encoding='ISO-8859-1')
    freq = data[' Frequency (Hz)'].to_numpy()
    amp = data[' Amplitude (Vpp)'].to_numpy()
    gain = data[' Gain (dB)'].to_numpy()
    phase = data[' Phase (°)'].to_numpy()

    w = freq*2*np.pi/48000

    X = dtft(x, w)
    Z = (0.2/(1-0.8**34))*(1 / (1 - 0.8 * np.exp(-1j * w)))
    mag = np.abs(X)
    mag = 20*np.log10(mag)
    ang = np.angle(X)*180/np.pi

    data = pd.read_csv('scope_0.csv', encoding='ISO-8859-1', skiprows=1)
    t = data['second'].to_numpy()
    v1 = data['Volt'].to_numpy()
    v2 = data['Volt2'].to_numpy()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
    # ax1.plot(w, mag, label='Analytical')
    ax1.plot(freq, gain, label='Measured')
    # ax1.plot(w, 20*np.log10(np.abs(Z)), label='Numerical')
    ax1.set_title('Magnitude vs Frequency')
    ax1.set_xlabel('Frequency (Hz)')
    ax1.set_ylabel('Magnitude (db)')
    ax1.legend()
    # ax2.plot(w, ang, label='Analytical')
    ax2.plot(freq, phase, label='Measured')
    # ax2.plot(w, np.degrees(np.angle(Z)), label='Numerical')
    ax2.set_title('Phase vs Frequency')
    ax2.set_xlabel('Frequency (Hz)')
    ax2.set_ylabel('Phase (degree)')
    ax2.legend()
    plt.tight_layout()
    plt.show()

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
    ax1.plot(t, v1, label='Passthrough Signal')
    ax1.set_title('Voltage vs Time')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Voltage (V)')
    ax1.legend(loc=1)
    ax2.plot(t, v2, label='Filtered Signal')
    ax2.set_title('Voltage vs Time')
    ax2.set_xlabel('Time (s)')
    ax2.set_ylabel('Voltage (V)')
    ax2.legend(loc=1)
    plt.tight_layout()
    plt.show()

    # plt.plot(f, p, label='Passthrough')
    # # plt.plot(t, v2, label='Passthrough', color='blue')
    # plt.xlabel("Frequency (rad/s)")
    # plt.ylabel("Phase (degrees)")
    # plt.title("Phase vs Frequency")
    # plt.legend()
    # plt.show()


if __name__ == "__main__":
    main()