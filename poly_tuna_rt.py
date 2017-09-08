import numpy as np
from builtins import print
from scipy.io.wavfile import read
from scipy.signal import find_peaks_cwt as findpeaks
from scipy.interpolate import lagrange
import time
import pyaudio
import sys

FS = 44100  # frecuencia de muestreo

# Frecuencias de referencia
HE_FREQ = 329.63 #988.89
B_FREQ = 246.94 #740.82
G_FREQ = 196.00
D_FREQ = 146.83
A_FREQ = 110.00
E_FREQ = 82.41
STRINGS = ['he', 'b', 'g', 'd', 'a', 'E']
# Constantes para Pyaudio

THRESHOLD = 50  # valor minimo a leer
CHUNK_SIZE = 4410  # trozos de 0.1 segundo
FORMAT = pyaudio.paInt16
CHANNELS = 1  # canales a leer (mono)
WIDTH = 2  # bytes de las muestras de audio (16 bits/muestra)


# inicio del programa
print('Afinador de un tono en tiempo real')


# FFT
def fft_func(audio):
	# FFT
	fft = np.abs(np.fft.fftshift(np.fft.fft(audio, FS)))
	# normalizacion de la FFT
	fft = fft / CHUNK_SIZE / 2

	# frecuencias de la FFT y recorte de los valores negativos
	fft_freqs = np.fft.fftshift(np.fft.fftfreq(int(len(fft)), 1 / FS))
	fft = fft[int(len(fft) / 2):]
	fft_freqs = fft_freqs[int(len(fft_freqs) / 2):]

	# eliminacion de los valores bajos para encontrar el pico facilmente y para evitar detecciones aleatorias
	for e in range(len(fft)):
		if THRESHOLD > fft[e]: 
			fft[e] = 0


	# busqueda del pico en cada rango de frecuencias
		

	E_peak = np.argmax(fft[71:95]);
	
	if fft[E_peak]>0:
		E_peak = E_peak +70;
		E_peak = pol_interpolation(E_peak,fft)

	a_peak = np.argmax(fft[95:127]);
	
	if fft[a_peak]>0:	
		a_peak = a_peak + 94;	
		a_peak = pol_interpolation(a_peak,fft)
	
	d_peak = np.argmax(fft[127:155]);
	
	if fft[d_peak]>0:
		d_peak = d_peak + 126;
		d_peak = pol_interpolation(d_peak,fft)

	g_peak = np.argmax(fft[169:226]);

	if fft[g_peak]>0:	
		g_peak = g_peak + 168;
		g_peak = pol_interpolation(g_peak,fft)

	b_peak = np.argmax(fft[699:855]);#220:300

	if fft[b_peak]>0:
		b_peak = b_peak + 698;
		b_peak = pol_interpolation(b_peak,fft)

	he_peak = np.argmax(fft[855:1142]);#300:450
		
	if fft[he_peak]>0:
		he_peak = he_peak + 854;
		he_peak = pol_interpolation(he_peak,fft)

	peaks = [he_peak, b_peak, g_peak, d_peak, a_peak, E_peak]
		



	return peaks

# definition function interpolation
def pol_interpolation(x1,fft):
	#store the 
	x2 = x1+1
	x0 = x1-1
	
	y0 = fft[x0]
	y1 = fft[x1]
	y2 = fft[x2]

	l = lagrange([x0, x1, x2], [y0, y1, y2])
	x=np.linspace(x0,x2,200)
	new_peak = x0 + (np.argmax(l(x)))/100
	return new_peak
	
	

# definicion funcion callback
def callback(in_data, frame_count, time_info, status):
	# convertir a array numpy
	audio_data = np.fromstring(in_data, dtype=np.int16)
	# analizar
	# audio_data = np.where(audio_data < THRESHOLD, 0, audio_data)
	peaks = fft_func(audio=audio_data)
	string_status = comparator(peaks=peaks)
	for peak, string, status in zip(peaks, STRINGS, string_status):
		print('{:6.2f}'.format(peak), ' ', string,' ', status, end=' || ')
	
	print('\n')

	
	# reproduce el audio de entrada
	# no seria necesario, pero no se quitarlo y que funcione aun
	return (in_data, pyaudio.paContinue)


# comparators

def comparator(peaks):
	# comparacion del pico con los valores de referencia


	he_dev = HE_FREQ - peaks[0]
	b_dev = B_FREQ - peaks[1]
	g_dev = G_FREQ - peaks[2]
	d_dev = D_FREQ - peaks[3]
	a_dev = A_FREQ - peaks[4]
	E_dev = E_FREQ - peaks[5]
	
	devs =[he_dev, b_dev, g_dev, d_dev, a_dev, E_dev]
	
	string_status=['','','','','','']
	
	for i in range(0,len(devs)):
		# print(i)
		if devs[i]>0:
			string_status[i] = '>'
		elif devs[i]<0:
			string_status[i] = '<'
		elif devs[i]==0:
			string_status[i] = '-'
	
	return string_status

# Preparacion Pyaudio
p = pyaudio.PyAudio()

# creacion del stream
stream = p.open(format=p.get_format_from_width(WIDTH), channels=CHANNELS, rate=FS, output=True, input=True,
				frames_per_buffer=CHUNK_SIZE, stream_callback=callback)

stream.start_stream()

while stream.is_active():
	time.sleep(0.1)

stream.close()

