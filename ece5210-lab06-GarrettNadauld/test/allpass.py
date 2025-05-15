import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from scipy import signal

def main():
    theta = np.linspace(0, 2*np.pi, 1000)
    x = np.cos(theta)
    y = np.sin(theta)

    plt.figure(figsize=(6,6))
    plt.plot(x, y, label='Unit Circle')
    plt.plot([-3, 3], [0, 0], 'k-', lw=2)  # x-axis
    plt.plot([0, 0], [-3, 3], 'k-', lw=2)  # y-axis
    plt.xlim(-3, 3)
    plt.ylim(-3, 3)

    hz_zeros = [-1,
                  0.8*np.exp(-1j*np.pi/4),
                  0.8*np.exp(1j*np.pi/4),
                  1.25*np.exp(-1j*np.pi/4),
                  1.25*np.exp(1j*np.pi/4),
                  0.4*np.exp(-1j*4*np.pi/5),
                  0.4*np.exp(1j*4.0*np.pi/5),
                  2.5*np.exp(-1j*4.0*np.pi/5),
                  2.5*np.exp(1j*4.0*np.pi/5)]

    hap_zeros = [1.25*np.exp(-1j*np.pi/4),
                 1.25*np.exp(1j*np.pi/4),
                 2.5 * np.exp(-1j * 4.0 * np.pi / 5),
                 2.5 * np.exp(1j * 4.0 * np.pi / 5)]

    hap_poles = [0.8 * np.exp(-1j*np.pi/4),
                 0.8 * np.exp(1j*np.pi/4),
                 0.4 * np.exp(-1j * 4 * np.pi / 5),
                 0.4 * np.exp(1j * 4.0 * np.pi / 5)]

    hmin_zeros = [(complex)(-1.0 * np.exp(0.0)),
                  (complex)(0.4 * np.exp(-1j*4.0*np.pi/5.0)),
                  (complex)(0.4 * np.exp(1j*4.0*np.pi/5.0)),
                  (complex)(0.8 * np.exp(-1j * np.pi / 4.0)),
                  (complex)(0.8 * np.exp(1j * np.pi / 4.0)),
                  (complex)(0.8 * np.exp(-1j * np.pi / 4.0)),
                  (complex)(0.8 * np.exp(1j * np.pi / 4.0)),
                  (complex)(0.4 * np.exp(-1j * 4.0 * np.pi / 5.0)),
                  (complex)(0.4 * np.exp(1j * 4.0 * np.pi / 5.0))]

    hmin_poles = []

    zeroes_x = np.real(hz_zeros)
    zeroes_y = np.imag(hz_zeros)

    plt.scatter(zeroes_x, zeroes_y, color='red', label='Zeroes')
    plt.xlabel('Real')
    plt.ylabel('Imaginary')
    plt.title('Zeros')
    plt.legend()

    plt.grid(True)
    plt.show()

    hz_poles = []
    b, a = signal.zpk2tf(hz_zeros, hz_poles, 0.1) #scope7
    d, c = signal.zpk2tf(hap_zeros, hap_poles, .1024) #scope9
    f, e = signal.zpk2tf(hmin_zeros, hmin_poles, 1.25**2*2.5**2*0.1) #scope8

    w, h = signal.freqz(b, a)
    mag = 20*np.log10(np.abs(h))
    phase = np.angle(h)

    wap, hap = signal.freqz(d, c)
    apmag = 20*np.log10(np.abs(hap))
    apphase = np.angle(hap)

    wmin, hmin = signal.freqz(f, e)
    minmag = 20*np.log10(np.abs(hmin))
    minphase = np.angle(hmin)

    # CSV Data
    data = pd.read_csv('scope_10.csv', encoding='ISO-8859-1')
    freq = data[' Frequency (Hz)'].to_numpy()
    amp = data[' Amplitude (Vpp)'].to_numpy()
    gain = data[' Gain (dB)'].to_numpy()
    phase_corr = data[' Phase (°)'].to_numpy()

    data = pd.read_csv('scope_7.csv', encoding='ISO-8859-1')
    freq = data[' Frequency (Hz)'].to_numpy()
    amp = data[' Amplitude (Vpp)'].to_numpy()
    gain = data[' Gain (dB)'].to_numpy()
    phase1 = data[' Phase (°)'].to_numpy()

    w1 = freq * 2 * np.pi / 48000

    data = pd.read_csv('scope_8.csv', encoding='ISO-8859-1')
    freq2 = data[' Frequency (Hz)'].to_numpy()
    amp = data[' Amplitude (Vpp)'].to_numpy()
    gain2 = data[' Gain (dB)'].to_numpy()
    phase2 = data[' Phase (°)'].to_numpy()

    w2 = freq2 * 2 * np.pi / 48000

    data = pd.read_csv('scope_9.csv', encoding='ISO-8859-1')
    freq3 = data[' Frequency (Hz)'].to_numpy()
    amp = data[' Amplitude (Vpp)'].to_numpy()
    gain3 = data[' Gain (dB)'].to_numpy()
    phase3 = data[' Phase (°)'].to_numpy()

    w3 = freq3 * 2 * np.pi / 48000

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 10))

    ax1.plot(w, mag, label='H(z)', color='Blue')
    ax1.plot(wap, apmag, label='H_ap(z)', color='Orange')
    ax1.plot(wmin, minmag, label='Hmin(z)', color='Green')
    ax1.plot(w1, gain, label='Linear Measured', color='Blue', linestyle='--')
    ax1.plot(w2, gain2, label='Min Measured', color='Green', linestyle='--')
    ax1.plot(w3, gain3, label='Ap Measured', color='Orange', linestyle='--')
    ax1.set_ylabel('Magnitude (dB)')
    ax1.set_xlabel('Frequency (radians/s)')
    ax1.set_title('Magnitude Response')
    ax1.legend()
    ax1.grid(True)

    ax2.plot(w, phase, label='H(z)', color='Blue')
    ax2.plot(wap, apphase, label='All Pass', color='Orange')
    ax2.plot(wmin, minphase, label='Minimum Phase', color='Green')
    ax2.plot(w1, (np.radians(phase1)), label='Linear Measured', color='Blue', linestyle='--')
    ax2.plot(w2, (np.radians(phase2)), label='Min Measured', color='Green', linestyle='--')
    ax2.plot(w3, (np.radians(phase3)), label='Ap Measured', color='Orange', linestyle='--')
    ax2.set_ylabel('Phase (radians)')
    ax2.set_xlabel('Frequency (radians/s)')
    ax2.set_title('Phase Response')
    ax2.legend()
    ax2.grid(True)
    plt.tight_layout()
    plt.show()

    # hmin_impulse_response = signal.impulse([hmin_zeros, hmin_poles, 1.0])
    # hz_impulse_response = signal.impulse([hz_zeros, hz_poles, 1.0])

    # Plot impulse responses
    plt.figure(figsize=(8, 6))
    plt.stem(np.arange(0, 10, 1), b, label='h[n]', linefmt='b-', markerfmt='bo', basefmt=' ')
    plt.stem(np.arange(0, 10, 1), f, label='h_min[n]', linefmt='r-', markerfmt='r*', basefmt=' ')
    plt.xlabel('n')
    plt.ylabel('Amplitude')
    plt.title('Impulse Response')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()