import numpy as np
from builtins import print
from scipy.io.wavfile import read
from scipy.signal import find_peaks_cwt as findpeaks
import time
import pyaudio
import sys

FS = 44100  # frecuencia de muestreo

# Frecuencias de referencia
HE_FREQ = 330
B_FREQ = 247
G_FREQ = 196
D_FREQ = 147
A_FREQ = 110
E_FREQ = 82
STRINGS = ['he', 'b', 'g', 'd', 'a', 'E']
# Constantes para Pyaudio

THRESHOLD = 100  # valor minimo a leer
THRESHOLD2 = 50
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
	# eliminacion de los valores bajos para encontrar el pico facilmente
	for e in range(len(fft)):
		if THRESHOLD2 > fft[e]:
			fft[e] = 0
	# busqueda del pico (el primero es el util)
	peak = 0
	for e in range(1, len(fft) - 1):
		if fft[e - 1] < fft[e] and fft[e + 1] < fft[e]:
			peak = fft_freqs[e]
			
			break
	return peak


# definicion funcion callback
def callback(in_data, frame_count, time_info, status):
	# convertir a array numpy
	audio_data = np.fromstring(in_data, dtype=np.int16)
	# analizar
	# audio_data = np.where(audio_data < THRESHOLD, 0, audio_data)

	#COMIENZO DEL PERIODO DE TIEMPO A MEDIR
	tiempo_inicial = time.time()
	peak = fft_func(audio=audio_data)
	string_name, string_status = comparator(peak=peak)
	tiempo_final = time.time()
	#FIN DEL PERIODO DE TIEMPO A MEDIR
	tiempo_ejecucion = tiempo_final - tiempo_inicial
	print(peak,' ',string_name,' ',string_status,'  Exec time: ', tiempo_ejecucion,'\n')
	
	# reproduce el audio de entrada
	# no seria necesario, pero no se quitarlo y que funcione aun
	return (in_data, pyaudio.paContinue)


# comparators

def comparator(peak):
	# comparacion del pico con los valores de referencia

	he_dev = HE_FREQ - peak
	b_dev = B_FREQ - peak
	g_dev = G_FREQ - peak
	d_dev = D_FREQ - peak
	a_dev = A_FREQ - peak
	E_dev = E_FREQ - peak
	
	devs =[he_dev, b_dev, g_dev, d_dev, a_dev, E_dev]
	
	min_string = np.argmin(np.abs(devs))
	
	if devs[min_string]>0:
		flat_or_high = '>'
	elif devs[min_string]<0:
		flat_or_high = '<'
	elif devs[min_string]==0:
		flat_or_high = '-'
	
	return STRINGS[min_string], flat_or_high


# Preparacion Pyaudio
p = pyaudio.PyAudio()

# creacion del stream
stream = p.open(format=p.get_format_from_width(WIDTH), channels=CHANNELS, rate=FS, output=True, input=True,
				frames_per_buffer=CHUNK_SIZE, stream_callback=callback)

stream.start_stream()

while stream.is_active():
	time.sleep(0.1)

stream.close()
