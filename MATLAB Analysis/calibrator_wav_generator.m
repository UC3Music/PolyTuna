close all
clear all 
clc

K=1/(2^(1/1200));%para el calculo de las frecuencias 1 cent inferiores y superiores

gain=1;%para asegurar que la suma de todas no satura el .wav
fs=44100;
duration=15;%duracion en segundos
freq=[82.41, 110.00, 146.83, 196.00, 740.82, 988.89];%referencias usadas
time=0:1/fs:duration;
x= zeros (1,length(time));

for i=1:length(freq)
%%%%generar tono referencia para todas las cuerdas (calibracion python)
% x=gain*sin(2*pi*freq(i)*time);
% semilogx(abs(fft(x,fs)))%para asegurar que el audio es correcto
%filename = ['calibration_audio'  num2str(i) '.wav']
%audiowrite(filename, x,fs);

%generar tono -1 y +1 cent para todas las cuerdas
x=sin(2*pi*freq(i)/K*time);
x=[x,sin(2*pi*freq(i)*K*time)];
filename = ['calibration_audio'  num2str(i) '.wav']
audiowrite(filename, x,fs);

end

%%Generar tono -1 cent y tono ref para la cuerda y 6 (Para python)
%x=sin(2*pi*freq(1)*time);
%x=[x,sin(2*pi*freq(1)*K*time)];
%audiowrite('6th_string_1cent_down.wav', x,fs);
%x=sin(2*pi*freq(2)*time);
%x=[x,sin(2*pi*freq(2)*K*time)];
%audiowrite('5th_string_1cent_down.wav', x,fs);


