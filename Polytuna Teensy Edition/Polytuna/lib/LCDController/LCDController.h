#include <LiquidCrystal_I2C.h>

#ifndef __LCDCONTROLLER_H_INCLUDED__
#define __LCDCONTROLLER_H_INCLUDED__

//Crear el objeto lcd (pantalla) con  dirección  0x3F y 20 columnas x 4 filas
static LiquidCrystal_I2C lcd(0x3F, 20, 4); //

void printStaticDisplay();
void lcdSetup();

void printDown20OrMore(int writingPosition);
void printDown20OrLess(int writingPosition);
void printInTune(int writingPosition);
void printUp20OrLess(int writingPosition);
void printUp20OrMore(int writingPosition);

#endif