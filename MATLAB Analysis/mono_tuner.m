close all
clear all
clc

%% lectura del audio
rasgueo = audioread('Guitar - string by string -std.wav');
%rasgueo = resample(rasgueo(:,1),1024,44100);
%t = (b-a)/44100;

Nsamples = length(rasgueo);


plot(rasgueo)
%plot(rasgueo(448000:Nsamples))%todas las cuerdas a la vez
ylabel('Amplitud')
xlabel('Tiempo (muestras)')
title('Forma de onda')
%n=1 %si se ha diezmado a 1024 muestras por segundo
n = 42; %si se usan 44100 muestras por segundo 
rasgueoE = rasgueo(500*n:5200*n);
rasgueoA = rasgueo(6450*n:11800*n);
rasgueoD = rasgueo(13700*n:20000*n);
rasgueoG = rasgueo(21800*n:27700*n);
rasgueoB = rasgueo(29100*n:34100*n);
rasgueoe = rasgueo(35500*n:39300*n);
figure

%%frecuencia sampleo del audio
%fs =1024;
fs = 44100;
%numero muestras fft
N = fs+1;
%fft

fft_rasgueo = zeros(6,N);
rasgueo = {rasgueoe,rasgueoB,rasgueoG,rasgueoD,rasgueoA,rasgueoE};

for i=1:6
    
fft_rasgueo(i,:) = abs(fftshift(fft(rasgueo{i},N)));
%subplot(3,2,i)%cuerda a cuerda
%semilogx(fft_rasgueo(i,fs/2:fs), 'color',[0 0.5 0])%para superponer cn el
%rasgueo
semilogx(fft_rasgueo(i,fs/2:fs))

hold on

 %pause;
[f_value, f] = findpeaks(fft_rasgueo(i,fs/2:fs),'minpeakheight',200);
f
%title(['Cuerda ',num2str(i)]) %cuerda a cuerda
%title(['Todas las cuerdas superpuestas + rasgueo'])%todas superpuestas
title('Detalle armónicos cuerdas 1 y 2')
%axis([70 1000 0 1500])%vista general
axis([600 1100 0 1500])%vista detalle cuerdas 1 y 2
plot(f(1),f_value(1),'or')
hold on
%pause
end
%Rasgueo completo con todas las cuerdas a la vez
a= 450000;%limites de lectura para no leer silencios
b= 750000;

rasgueo_all_once = audioread('Guitar - all strings at once - std.wav',[a,b]);
fft_rasgueo_full(1,:) = abs(fftshift(fft(rasgueo_all_once,N)));

%figure
%semilogx(fft_rasgueo_full(fs/2:fs),':b','linewidth',1.5)

