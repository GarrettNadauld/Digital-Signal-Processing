from scipy.linalg import toeplitz
from scipy.signal import freqz
import numpy as np

def calculate_frequency(L, bands):
    frequency_1 = np.linspace(bands[0][0], bands[0][1], int(L/2))
    frequency_2 = np.linspace(bands[1][0], bands[1][1], int(L/2))
    return np.concatenate((frequency_1, frequency_2))

def calculate_A_and_d(L, M_half, frequency, bands, desired):
    A = np.zeros((L, M_half))
    d = np.zeros(L)

    for i in range(L):
        A[i, :] = [1 if j == 0 else 2 * np.cos(j * frequency[i]) for j in range(M_half)]
        d[i] = desired[1] if frequency[i] >= bands[1][0] else desired[0]

    return A, d

def fir_least_squares(n_taps, bands, desired):
    L = len(desired)*500
    L = int(L)
    M_half = int((n_taps+1)/2)

    frequency = calculate_frequency(L, bands)
    A, d = calculate_A_and_d(L, M_half, frequency, bands, desired)

    h = np.linalg.lstsq(A, d, rcond=None)[0]
    h.flatten()

    return np.concatenate((h[::-1], h[1:]))

def main():
    n_taps = 19
    bands = [[0, 1], [1.5, np.pi]]
    desired = [0, 1]
    coeffs = fir_least_squares(n_taps, bands, desired)
    lp = [[0, 4500/24000*np.pi], [5000/24000*np.pi, np.pi]]
    desired_lp = [1, 0]
    hp = [[0, 4000/24000*np.pi], [4500/24000*np.pi, np.pi]]
    desired_hp = [0, 1]
    coeffs_lp = fir_least_squares(n_taps, lp, desired_lp)
    coeffs_hp = fir_least_squares(n_taps, hp, desired_hp)   
    print(coeffs_hp)
    print(coeffs_lp)

if __name__ == "__main__":
    main() 