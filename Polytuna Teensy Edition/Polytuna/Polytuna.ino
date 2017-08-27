#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=125,201
AudioMixer4              mixer1;         //xy=335,317
AudioOutputI2S           i2s2;           //xy=336,150
AudioAnalyzePeak         peak1;          //xy=511,360
AudioAnalyzeFFT1024      fft1024;      //xy=522,256
AudioConnection          patchCord1(i2s1, 0, i2s2, 0);
AudioConnection          patchCord2(i2s1, 0, mixer1, 0);
AudioConnection          patchCord3(i2s1, 1, i2s2, 1);
AudioConnection          patchCord4(i2s1, 1, mixer1, 1);
AudioConnection          patchCord5(mixer1, fft1024);
AudioConnection          patchCord6(mixer1, peak1);
AudioControlSGTL5000     sgtl5000_1;     //xy=302,425
// GUItool: end automatically generated code

//Global Variables
float Resolution = 7.7917220745;
//float resolution = 96000000*4/188/256 / 1024; //herzios por bin. el bin X tendra la frecuencia: freqX = X*resolution

//valores mas altos en cada rango
float E_peak_lvl = 0.0;
float A_peak_lvl = 0.0;
float D_peak_lvl = 0.0;
float G_peak_lvl = 0.0;
float B_peak_lvl = 0.0;
float he_peak_lvl = 0.0;
float E_peak = 0.0;
float A_peak = 0.0;
float D_peak = 0.0;
float G_peak = 0.0;
float B_peak = 0.0;
float he_peak = 0.0;
//de momento solo se usa uno hasta que se cambie la frecuencia de muestreo
//devuelve nivel del bin central si este es un pico. Si no, devuelve 0

float interpolate(float x0, float x1, float x2, float b0, float b1, float b2) {
  //interpolacion cuadratica por el metodo de lagrange
  /* Se espera que las x0, x1 y x2 sean los bins, no los valores de frecuencia. Por tanto la variable xMax es un valor de un bin intermedio.
    //Ejemplo: recibe los bins 6, 7 y 8. Se calcularian sampleN valores entre 6 y 8 y se calcularia el maximo (supongamos, en 6.87).
    //para hallar la frecuencia real hay que multiplicar la salida por la resolución del sistema (Resolution)
  */
  //saco estos cocientes constantes fuera para evitar que se realicen todo el rato
  float a0 = b0 / ( (x0 - x1) * (x0 - x2) );
  float a1 = b1 / ( (x1 - x0) * (x1 - x2) );
  float a2 = b2 / ( (x2 - x0) * (x2 - x1) );

  //defino el numero de muestras intermedias
  int sampleN = 1600; //probar valores. A mayor sea, mejor precisión pero peor rendimiento
  //con 1600 obtenemos un valor cada 0.01 Hz aprox.
  //resolucion tras interpolar = 2xResolution /sampleN = 0.009739
  float xMax = 0.00;
  float pxMax = 0.00;
  for (int i = 0; i < sampleN; i++) {

    float x = x0 + ((x2 - x0) * i / sampleN); //calculamos valores entre esos bins

    //los hago aqui para ahorrarme la mitad de las operaciones (Se realizan 2 veces por iteracion cada uno) y por claridad
    float y0 = (x - x0);
    float y1 = (x - x1);
    float y2 = (x - x2);

    float px = y1 * y2 * a0 + y0 * y2 * a1 + y0 * y1 * a2;

    if (px > pxMax) {
      xMax = x;
      pxMax = px;
    }
  }

  return (xMax * Resolution); //Devolvemos el valor de frecuencia directamente, no el bin
}

float getPeak(int centralBin) {
  float peakLevel = fft1024.read(centralBin) * 100.0;
  /////////////////SOLO PARA SERIAL

  if (centralBin < 15) {
    Serial.print(peakLevel, 3);
    Serial.print("  ");
  }

  ////////////////////
  if (peakLevel >= fft1024.read(centralBin - 1)) {
    if (peakLevel >= fft1024.read(centralBin + 1)) {
      return peakLevel;
    }
  }
  return 0;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Testing the circuit");
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);

  AudioMemory(15);
  sgtl5000_1.volume(0.5);



  mixer1.gain(0, 0.5);//habria que ver a que canal se conecta
  mixer1.gain(1, 0.5);

  delay(500);
}

void loop() {

  if (fft1024.available()) {
    E_peak_lvl = 0.0;
    A_peak_lvl = 0.0;
    D_peak_lvl = 0.0;
    G_peak_lvl = 0.0;
    B_peak_lvl = 0.0;
    he_peak_lvl = 0.0;
    int centralBin;

    //leo a partir del bin 0(prevBin=0) hasta el 511 (nextBin =512)
    //la FFT es simetrica y solo necesitamos los primeros 512 bins
    /*  for (centralBin = 1; centralBin < 511; centralBin++) {
        float auxPeakLevel = fft1024.read(centralBin) * 100.0;
        /////////////////SOLO PARA SERIAL

        if (centralBin < 15) {
          Serial.print(auxPeakLevel, 3);
          Serial.print("  ");
        }
    */
    //////////////////// FIND HIGHEST FREQUENCY IN EACH RANGE //////////
    //// E string
    for (centralBin = 2; centralBin <= 12; centralBin++) { //central bin entre: 7.79*3 = 23.37 y 7.79*12 = 93.48
      float aux_peak_lvl = fft1024.read(centralBin);
      if (aux_peak_lvl > E_peak_lvl) {
        E_peak_lvl = aux_peak_lvl;
        E_peak = centralBin;
      }
    }
    //// A string
    for (centralBin = 12; centralBin <= 16; centralBin++) { //central bin entre: 7.79*12 = 93.48 y 7.79*17 = 124
      float aux_peak_lvl = fft1024.read(centralBin);
      if (aux_peak_lvl > A_peak_lvl) {
        A_peak_lvl = aux_peak_lvl;
        A_peak = centralBin;
      }
    }
    //// D string
    for (centralBin = 16; centralBin <= 20; centralBin++) { //central bin entre: 124 y 155.8
      float aux_peak_lvl = fft1024.read(centralBin);
      if (aux_peak_lvl > D_peak_lvl) {
        D_peak_lvl = aux_peak_lvl;
        D_peak = centralBin;
      }
    }

    //dejo un margen de 2 bins (15 Hz) entre estas por los armonicos tan altos que tiene la guitarra aqui

    //// G string
    for (centralBin = 22; centralBin <= 27; centralBin++) { //central bin entre: 171.3 y 210.33
      float aux_peak_lvl = fft1024.read(centralBin);
      if (aux_peak_lvl > G_peak_lvl) {
        G_peak_lvl = aux_peak_lvl;
        G_peak = centralBin;
      }
    }
    //estas dos cuerdas tienen suficientes bins para poder solaparse los rangos de busqueda
    //// B string
    for (centralBin = 28; centralBin <= 39; centralBin++) { //central bin entre: 218.12 y 303.81
      float aux_peak_lvl = fft1024.read(centralBin);
      if (aux_peak_lvl > B_peak_lvl) {
        B_peak_lvl = aux_peak_lvl;
        B_peak = centralBin;
      }
    }
    //// he string
    for (centralBin = 36; centralBin <= 55; centralBin++) { //central bin entre: 280 y 428.45
      float aux_peak_lvl = fft1024.read(centralBin);
      if (aux_peak_lvl > he_peak_lvl) {
        he_peak_lvl = aux_peak_lvl;
        he_peak = centralBin;
      }
    }
    ///////////////////
    //mono tuner
    /* if (auxPeakLevel > peakLevel) {
       peakLevel = auxPeakLevel;
       peak = centralBin;
      }*/

    /////////////////////////// INTERPOLATE ///////////////////////
    //A partir de los bins máximos en cada rango, obtengo el pico por interpolacion
    //// E string
    float E_freq_peak = interpolate(E_peak - 1, E_peak, E_peak + 1, fft1024.read(E_peak - 1), fft1024.read(E_peak), fft1024.read(E_peak + 1));
    //// A string
    float A_freq_peak = interpolate(A_peak - 1, A_peak, A_peak + 1, fft1024.read(A_peak - 1), fft1024.read(A_peak), fft1024.read(A_peak + 1));
    //// D string
    float D_freq_peak = interpolate(D_peak - 1, D_peak, D_peak + 1, fft1024.read(D_peak - 1), fft1024.read(D_peak), fft1024.read(D_peak + 1));
    //// G string
    float G_freq_peak = interpolate(G_peak - 1, G_peak, G_peak + 1, fft1024.read(G_peak - 1), fft1024.read(G_peak), fft1024.read(G_peak + 1));
    //// B string
    float B_freq_peak = interpolate(B_peak - 1, B_peak, B_peak + 1, fft1024.read(B_peak - 1), fft1024.read(B_peak), fft1024.read(B_peak + 1));
    //// he string
    float he_freq_peak = interpolate(he_peak - 1, he_peak, he_peak + 1, fft1024.read(he_peak - 1), fft1024.read(he_peak), fft1024.read(he_peak + 1));

//////////////////////////// PRINTING VALUES
    Serial.print(" E -> ");
    Serial.print(E_freq_peak, 2);
    Serial.print("|| A -> ");
    Serial.print(A_freq_peak, 2);
    Serial.print("|| D -> ");
    Serial.print(D_freq_peak, 2);
    Serial.print("|| G -> ");
    Serial.print(G_freq_peak, 2);
    Serial.print("|| B -> ");
    Serial.print(B_freq_peak, 2);
    Serial.print("|| he -> ");
    Serial.println(he_freq_peak, 2);
    //TODO something with the peak
    delay(100);
  }
}
