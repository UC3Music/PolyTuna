#include "LCDController.h" 

// Define arrow figures using byte arrays that control the on and off pixels
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

// Print the display with the name and number of each string and the reference marks

void printStaticDisplay()
{
  //Print names and numbers
  lcd.setCursor(1, 3);
  lcd.print("6E 5A 4D  3G 2B 1e");
  //middle line
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

// Full LCD setup routine
void lcdSetup(){
    //Start the LCD
  lcd.init();

  //Turn on the backlight
  lcd.backlight();

  //Create custom symbols such as the arrows
  lcd.createChar(0, high_far_arrow);
  //lcd.createChar(1,high_middle_arrow);
  lcd.createChar(1, high_almost_arrow);
  lcd.createChar(2, just_right);
  lcd.createChar(3, low_almost_arrow);
  //lcd.createChar(6,low_middle_arrow);
  lcd.createChar(4, low_far_arrow);
  lcd.createChar(5, mid_bar);
  //lcd.createChar(8,low_middle_arrow);

  // Add the constant layout to the screen
  printStaticDisplay();
}

// Methods for printing the different characters
void printDown20OrMore(int writingPosition){
        lcd.setCursor(writingPosition, 0);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 1);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 2);
      lcd.write(byte(4));
      lcd.write(byte(4));
}

void printDown20OrLess(int writingPosition){
        lcd.setCursor(writingPosition, 0);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 1);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 2);
      lcd.write(byte(3));
      lcd.write(byte(3));
}

void printInTune(int writingPosition){
        lcd.setCursor(writingPosition, 0);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 2);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 1);
      lcd.write(2);
      lcd.write(2);
}

void printUp20OrLess(int writingPosition){
        lcd.setCursor(writingPosition, 1);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 2);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 0);
      lcd.write(byte(0));
      lcd.write(byte(0));
}

void printUp20OrMore(int writingPosition){
        lcd.setCursor(writingPosition, 1);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 2);
      lcd.print("  ");
      lcd.setCursor(writingPosition, 0);
      lcd.write(byte(1));
      lcd.write(byte(1));
}