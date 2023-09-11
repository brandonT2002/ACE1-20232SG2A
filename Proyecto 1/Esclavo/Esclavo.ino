int estado = 0;
#include <LiquidCrystal.h>
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

void setup()
{
  lcd_1.begin(16, 2); // Set up the number of columns and rows on the LCD
}

void loop()
{
  switch(estado){
    case 0:
    lcd_1.setCursor(0, 0);
    lcd_1.print("<GRUPO        2>");
    break;
  }
}
