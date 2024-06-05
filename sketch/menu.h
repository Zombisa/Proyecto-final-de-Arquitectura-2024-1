/**
 * @file menu.h
 * @brief Header file containing menu configurations and functions.
 * @author Isabela Mosquera Fernandez
 * @author Isabela Sanchez Saavedra
 */

#ifndef MENU
#define MENU

#include "utilidad.h"
#include "variables.h"
#include <LiquidMenu.h>

/**
 * @def limiteTMax
 * @brief Maximum temperature limit.
 */
#define limiteTMax 50 

/**
 * @def limiteHMax
 * @brief Maximum hall sensor value limit.
 */
#define limiteHMax 1023

/**
 * @def limiteLMax
 * @brief Maximum light sensor value limit.
 */
#define limiteLMax 1023  

/**
 * @def limiteTMin
 * @brief Minimum temperature limit.
 */
#define limiteTMin 0

/**
 * @def limiteLMin
 * @brief Minimum light sensor value limit.
 */
#define limiteLMin 0 

/**
 * @brief First line of liquid crystal display (LCD).
 */
LiquidLine line1(0, 1, "TEMP MAX [", tempMax, "]"); 

/**
 * @brief Second line of liquid crystal display (LCD).
 */
LiquidLine line2(0, 1, "TEMP MIN [", tempMin, "]"); 

/**
 * @brief Third line of liquid crystal display (LCD).
 */
LiquidLine line3(0, 1, "LUZ MAX [", luzMax, "]");

/**
 * @brief Fourth line of liquid crystal display (LCD).
 */
LiquidLine line4(0, 1, "LUZ MIN [", luzMin, "]");

/**
 * @brief Fifth line of liquid crystal display (LCD).
 */
LiquidLine line5(0, 1, "HALL [", hallMax, "]");

/**
 * @brief Sixth line of liquid crystal display (LCD).
 */
LiquidLine line6(0, 1, "RESET"); 

/**
 * @brief LCD screen.
 */
LiquidScreen screen; 

/**
 * @brief LiquidMenu object for menu navigation.
 */
LiquidMenu menu(lcd);

/**
 * @brief A function that does nothing but is important to attatch_function.
 */
void blankFunction(){
    return;
};

/**
 * @brief Function to output configurations to serial monitor.
 */
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
/**
 * @brief Function to reset all configuration values to default.
 */
void reset(){
  tempMax = 40;
  tempMin = 0;
  luzMax= 300;
  luzMin= 50;
  hallMax = 1000;
}

/**
 * @brief Function to increment the configuration values.
 * @param lineaActual The current line index in the menu.
 */
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

/**
 * @brief Function to decrement the configuration values.
 * @param lineaActual The current line index in the menu.
 */
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

/**
 * @brief Function to handle menu navigation in the main loop.
 */
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



#endif  // MENU