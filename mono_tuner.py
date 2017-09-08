import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read
from scipy.signal import find_peaks_cwt as findpeaks
from scipy.signal import resample
print('Afinador de un tono')

plt.close("all");#cerrar todos los plots anteriores

cuerdas = read('Guitar - string by string -std.wav')
audio = np.array(cuerdas[1],dtype=float)#leer el audio en formato float

fs= 44100;#frecuencia de muestreo
frs= 3200#resampling frequency

resampleRatio = fs/frs

audio=resample(audio,int(len(audio)/resampleRatio)); #resample to 3200 samples/second


muestras = int(frs/10);#numero de muestras en 100ms
fft_range = int(len(audio)/muestras)-1
plt.plot(audio);
plt.show()
for i in range(0,fft_range):
	
	fft = np.abs(np.fft.fftshift(np.fft.fft(audio[i*muestras:i*muestras+muestras*2], frs)));
	#fft de 100ms, desplazada para que las frecuencias mas bajas esten en 0 y en modulo.

	print('Muestra inicial (Segundos)')
	print(i*muestras/frs)
	fft = fft/muestras/2
	fft_freqs = np.fft.fftshift(np.fft.fftfreq(int(len(fft)),1/frs));
	fft = fft[int(len(fft)/2):]
	fft_freqs = fft_freqs[int(len(fft_freqs)/2):]
	#frecuencias de la fft
	
	for e in range(len(fft)):
		if 150>fft[e] or e<50:
			fft[e]=0;
	#hallar picos en la FFT
	peakind = findpeaks(fft,np.arange(1,15));
	#mostrar picos
	print('Máximos encontrados')
	print(fft_freqs[peakind])
	print('\n')
	plt.show()
