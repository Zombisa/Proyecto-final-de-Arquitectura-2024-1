#ifndef MENU
#define MENU

#include "utilidad.h"
#include "variables.h"
#include <LiquidMenu.h>

#define limiteTMax 50 
#define limiteHMax 1023
#define limiteLMax 1023  
#define limiteTMin 0
#define limiteLMin 0 


LiquidLine line1(0, 1, "TEMP MAX [", tempMax, "]"); 
LiquidLine line2(0, 1, "TEMP MIN [", tempMin, "]"); 
LiquidLine line3(0, 1, "LUZ MAX [", luzMax, "]");
LiquidLine line4(0, 1, "LUZ MIN [", luzMin, "]");
LiquidLine line5(0, 1, "HALL [", hallMax, "]");
LiquidLine line6(0, 1, "RESET"); 

LiquidScreen screen; 
LiquidMenu menu(lcd);


void blankFunction() {
    return;
}
void incrementar(int lineaActual);
void decrementar(int lineaActual);

void outputConf(){
  Serial.println("Ini   Cfg   MonAmb   MonEve   Alm   Blq");
  Serial.println("       X                               ");
  Serial.println();


  
  lcd.clear();

  screen.add_line(line1);
  screen.add_line(line2);
  screen.add_line(line3);
  screen.add_line(line4);
  screen.add_line(line5);
  screen.add_line(line6);

  line1.attach_function(1, blankFunction);
  line2.attach_function(1, blankFunction);
  line3.attach_function(1, blankFunction);
  line4.attach_function(1, blankFunction);
  line5.attach_function(1, blankFunction);
  line6.attach_function(1, blankFunction);

  screen.set_displayLineCount(2);


  line1.set_decimalPlaces(2);

  menu.add_screen(screen);

  menu.update();
  menu.switch_focus();

  taskLoopMenu.Start();

}

void loopLiquidMenu(){
  char key = keypad.getKey();


  if(!key)
    return;

  int lineaActual = menu.get_focusedLine();

  switch (key) {
  case 'A':
    incrementar(lineaActual);
    break;
  case 'B':
    decrementar(lineaActual);
    break;
  case 'C':
    menu.switch_focus();
    if(lineaActual == 5)
      menu.switch_focus();
    break;
  default:
    Serial.println(lineaActual);
  break;
  
  }
    menu.update();

}

void reset(){
  tempMax = 40;
  tempMin = 0;
  luzMax= 300;
  luzMin= 50;
  hallMax = 1000;
}

void incrementar(int lineaActual){
  switch (lineaActual) {
  case 0:
    if(tempMax+1 <= limiteTMax)
      tempMax+=1;
    break;
  case 1:
    if(tempMin+1 < tempMax)
      tempMin+=1; 
    break;
  case 2:
    if(luzMax+10 <= limiteLMax)
      luzMax+=10;
    break;
  case 3:
    if(luzMin+10 < luzMax)
      luzMin+=10;
    break;
  case 4:
    if(hallMax+20 <= limiteHMax)
      hallMax+=20;
    break;
  case 5:
    reset();
    break;
  }

}

void decrementar(int lineaActual){
  switch (lineaActual) {
  case 0:
    if(tempMax-1 > tempMin)
      tempMax-=1;
    break;
  case 1:
    if(tempMin-1 >= limiteTMin)
      tempMin-=1; 
    break;
  case 2:
    if(luzMax-10 > luzMin)
      luzMax-=10;
    break;
  case 3:
    if(luzMin-10 >= limiteLMin)
      luzMin-=10;
    break;
  case 4:
    if(hallMax-20 >= 0)
      hallMax-=20;
    break;
  case 5:
    reset();
    break;
  }

}

#endif  // MENU