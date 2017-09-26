close all
clear all
clc

freq = 81.5:0.01:82.5;
peak_sum=zeros(1,length(freq));
for i=1:length(freq)
amp=1; 
fs=44100;  % sampling frequency
duration=5;
%freq=82.41;
%freq=82;
values=0:1/fs:duration;
a=amp*sin(2*pi* freq(i)*values);

fft1 = abs(fft(a,fs-1))/fs*2;

%figure
%semilogx(fft1(2:end))

axis([70 100 0 1])

p = polyfit(81:83,fft1(82:84),2);

res = 0.001;
x = 79:res:85;

interp = polyval(p,x);
%hold on
%plot(x,interp,'r')

[peak_amp, peak] = max(interp);
 
peak_sum(i) = 79+(peak-1)*res;
end

plot(peak_sum,1:length(peak_sum))