#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd (pantalla) con  dirección  0x3F y 20 columnas x 4 filas
LiquidCrystal_I2C lcd(0x3F, 20, 4); //

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

//Valores de referencia para afinar
float HE_REF = 329.63;
float B_REF = 246.94;
float G_REF = 196.00;
float D_REF = 146.83;
float A_REF = 110.00;
float E_REF = 82.41;

float REFS[7] = {E_REF, A_REF, D_REF, G_REF, B_REF, HE_REF};

//multiplicadores de notas
float UP_TONE = 1.0595;
float UP_20_CENT = 1.0116;
float UP_50_CENT = 1.0293;

float DOWN_TONE = 0.9439;
float DOWN_20_CENT = 0.9885;
float DOWN_50_CENT = 0.9715;

byte high_far_arrow[8] = {
  B10001,
  B01010,
  B00100,
  B10001,
  B01010,
  B00100,
  B00000,
  B00000,
};
byte high_middle_arrow[8] = {
  B00000,
  B00000,
  B00000,
  B10001,
  B01010,
  B00100,
  B00000,
  B00000,
};
byte high_almost_arrow[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10001,
  B01010,
  B00100,
};
byte mid_bar[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
};
byte just_right[8] = {
  B00000,
  B10001,
  B01010,
  B00100,
  B00100,
  B01010,
  B10001,
  B00000,
};
byte low_far_arrow[8] = {
  B00000,
  B00000,
  B00100,
  B01010,
  B10001,
  B00100,
  B01010,
  B10001,
};
byte low_middle_arrow[8] = {
  B00000,
  B00000,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
  B00000,
};
byte low_almost_arrow[8] = {
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

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
  int sampleN = 3200; //probar valores. A mayor sea, mejor precisión pero peor rendimiento
  //con 3200 obtenemos un valor cada 0.005 Hz aprox.
  //resolucion tras interpolar = 2xResolution /sampleN = 0.004870
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

void static_display() {
  //imprimir layout basico
  lcd.setCursor(1, 3);
  lcd.print("6E 5A 4D  3G 2B 1e");
  //linea de enmedio
  lcd.setCursor(0, 1);
  lcd.write(62);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(5);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(5);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(60);

  lcd.write(62);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(5);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(5);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(60);

}

void tuning_print(float freq_peaks[]) {
  //j controla la posicion a escribir
  int j = 1;
  for (int i = 0; i < 6; i++) {
    //calculamos los intervalos para las representaciones
    float ref_up_20 = REFS[i] * UP_20_CENT;
    float ref_down_20 = REFS[i] * DOWN_20_CENT;
    //si es menor del tono referencia por mas de 20 cents
    if (freq_peaks[i] < ref_down_20) {
      lcd.setCursor(j, 0);
      lcd.print("  ");
      lcd.setCursor(j, 1);
      lcd.print("  ");
      lcd.setCursor(j, 0);
      lcd.write(byte(4));
      lcd.write(byte(4));
    }
    //si está entre 20 cents y la referencia
    else if (freq_peaks[i] < REFS[i]) {

      lcd.setCursor(j, 0);
      lcd.print("  ");
      lcd.setCursor(j, 1);
      lcd.print("  ");
      lcd.setCursor(j, 0);
      lcd.write(byte(3));
      lcd.write(byte(3));
    }
    //si es igual a la referencia
    else if (freq_peaks[i] == REFS[i]) {
      lcd.setCursor(j, 0);
      lcd.print("  ");
      lcd.setCursor(j, 2);
      lcd.print("  ");
      lcd.setCursor(j, 1);
      lcd.write(2);
      lcd.write(2);
    }
    //si es mayor que la referencia +20
    else if (freq_peaks[i] > ref_up_20) {
      lcd.setCursor(j, 1);
      lcd.print("  ");
      lcd.setCursor(j, 2);
      lcd.print("  ");
      lcd.setCursor(j, 0);
      lcd.write(byte(0));
      lcd.write(byte(0));

    }

    //si esta entre la referencia y la referencia +20
    else if (freq_peaks[i] < ref_up_20) {
      lcd.setCursor(j, 1);
      lcd.print("  ");
      lcd.setCursor(j, 2);
      lcd.print("  ");
      lcd.setCursor(j, 0);
      lcd.write(byte(1));
      lcd.write(byte(1));
    }
    //salvamos el espacio extra de enmedio
    if (j == 7) {
      j++;
    }
    //escribimos en la siguiente posicion
    j = j + 2;

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Testing the circuit");

  //configuracion de la placa de audio
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  AudioMemory(15);
  sgtl5000_1.volume(0.5);
  mixer1.gain(0, 0.5);//habria que ver a que canal se conecta
  mixer1.gain(1, 0.5);
  //delay porque si
  delay(500);


  // Configuracion del LCD

  //Inicializar el LCD
  lcd.init();

  //Encender la luz de fondo.
  lcd.backlight();

  //crear caracteres personalizados
  lcd.createChar(0, high_far_arrow);
  //lcd.createChar(1,high_middle_arrow);
  lcd.createChar(1, high_almost_arrow);
  lcd.createChar(2, just_right);
  lcd.createChar(3, low_almost_arrow);
  //lcd.createChar(6,low_middle_arrow);
  lcd.createChar(4, low_far_arrow);
  lcd.createChar(5, mid_bar);
  //lcd.createChar(8,low_middle_arrow);

  // Escribimos el layout constante en el LCD.
  static_display();
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
    //////////////////// BUSCAR PICO EN CADA INTERVALO DE BUSQUEDA //////////
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

    float freq_peaks_detected [] = {E_freq_peak, A_freq_peak, D_freq_peak, G_freq_peak, B_freq_peak, he_freq_peak};

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
    tuning_print(freq_peaks_detected);
    delay(85);
  }
}



