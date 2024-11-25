import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import freqz

freq = 1000
fs = 44100
r = 0.98
a1 = -2 * r * np.cos(2 * np.pi * freq / fs)
a2 = r ** 2
b0 = 1-np.sqrt(r)

b = [b0, 0, 0]
a = [1, a1, a2]

w, h = freqz(b, a, fs=fs)

fig,ax = plt.subplots()
ax.plot(w, 20 * np.log10(abs(h)))
ax.set_xscale('log')
ax.set_title('Frequency Response')
ax.set_xlabel('Frequency [Hz]')
ax.set_ylabel('Gain [dB]')
ax.grid(which='both', axis='both')

plt.show()

