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
    x = np.ones(7)
    x *= 1/7
    # w = np.linspace(0, np.pi, 10000)
    # X = dtft(x, w)
    # mag = np.abs(X)
    # # mag = 20*np.log(mag)
    # ph = np.angle(X)


    # CSV Data
    data = pd.read_csv('scope_6.csv', encoding='ISO-8859-1')
    # print(data.head())
    freq = data[' Frequency (Hz)'].to_numpy()
    amp = data[' Amplitude (Vpp)'].to_numpy()
    gain = data[' Gain (dB)'].to_numpy()
    phase = data[' Phase (°)'].to_numpy()

    # phase *= np.pi/180
    w = freq*2*np.pi/48000

    X = dtft(x, w)
    mag = np.abs(X)
    mag = 20*np.log10(mag)
    ang = np.angle(X)*180/np.pi

    data = pd.read_csv('sjope_7.csv', encoding='ISO-8859-1', skiprows=1)
    t = data['second'].to_numpy()
    v1 = data['Volt'].to_numpy()
    v2 = data['Volt2'].to_numpy()

    data = pd.read_csv('sjope_8.csv', encoding='ISO-8859-1')
    f = data[' Frequency (Hz)'].to_numpy()
    a = data[' Amplitude (Vpp)'].to_numpy()
    g = data[' Gain (dB)'].to_numpy()
    p = data[' Phase (°)'].to_numpy()
    f = f * 2 * np.pi / 48000

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
    ax1.plot(w, mag, label='Analytical')
    ax1.plot(w, gain, label='Measured')
    ax1.set_title('Magnitude vs Frequency')
    ax1.set_xlabel('Frequency (rad/s)')
    ax1.set_ylabel('Magnitude (db)')
    ax1.legend()
    ax2.plot(w, ang, label='Analytical')
    ax2.plot(w, phase, label='Measured')
    ax2.set_title('Phase vs Frequency')
    ax2.set_xlabel('Frequency (rad/s)')
    ax2.set_ylabel('Phase (degree)')
    ax2.legend()
    plt.tight_layout()
    plt.show()

    plt.plot(t, v1, label='Filtered Signal')
    plt.plot(t, v2, label='Passthrough')
    plt.xlabel("Time (s)")
    plt.ylabel("Voltage (V)")
    plt.title("Voltage vs Time")
    plt.legend()
    plt.show()

    plt.plot(f, p, label='Passthrough')
    # plt.plot(t, v2, label='Passthrough', color='blue')
    plt.xlabel("Frequency (rad/s)")
    plt.ylabel("Phase (degrees)")
    plt.title("Phase vs Frequency")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()