/**
 * @file seguridad.h
 * @brief Header file containing functions for security management.
 * @author Isabela Mosquera Fernandez
 * @author Isabela Sanchez Saavedra
 */
#ifndef SEGURIDAD
#define SEGURIDAD

#include "variables.h"

/**
 * @brief Initializes the security system.
 * 
 * This function resets counters and states, updates the LCD to prompt for the password,
 * starts the loop task for security checks, and initializes the password comparison array.
 */
void outputIni(){
 
  Serial.println("Ini   Cfg   MonAmb   MonEve   Alm   Blq");
  Serial.println(" X                                     ");

  contadorIntentos = 0;
  contadorDigitos = 0;
  sistemaBloqueado = 0;
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  reiniciarLCD();
  lcd.print("Clave:"); 
  
  taskLoop.Start();
  for(size_t i=0; i<4; i++)
    varComparar[i] = '*';
}

/**
 * @brief Actions to perform when the correct password is entered.
 * 
 * This function updates the LCD, turns on the green LED, plays a success tone sequence,
 * stops the loop task, starts the task for correct password, and stops the timer task.
 */
void claveCorrecta(){
  reiniciarLCD();
  lcd.print("Clave correcta");
  digitalWrite(LED_GREEN, HIGH);

  tone(BUZZER, 1800, 100);
  delay(100);
  tone(BUZZER, 2000, 100);
  delay(100);
  tone(BUZZER, 2200, 100);
  delay(100);
  tone(BUZZER, 2500, 350);
  delay(350);
  noTone(BUZZER);

  taskStopLoop.Start();
  taskClaveCorrecta.Start();
  taskTime5.Stop();
  
}

/**
 * @brief Actions to perform when an incorrect password is entered.
 * 
 * This function updates the LCD, turns on the blue LED, plays an error tone,
 * resets the password comparison array, stops the loop task, and either starts the 
 * system block task or the retry task based on the number of attempts.
 */
void claveIncorrecta(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Clave incorrecta");
  digitalWrite(LED_BLUE, HIGH);

  tone(BUZZER, 1000, 200);
  delay(200);
  noTone(BUZZER);

  for(size_t i=0; i<4; i++)
    varComparar[i] = '*';
  taskStopLoop.Start(); 
  if(contadorIntentos == 2){
    taskSistBloq.Start();
  }else{
    taskAgain.Start();
    contadorDigitos = 0;
  }

  reiniciarLCD();
}

/**
 * @brief Main loop for the security system to read and process user input.
 * 
 * This function reads keypad input, updates the display, and checks the entered
 * password against the stored password. It handles digit entry, password comparison,
 * and starts appropriate tasks based on the comparison result.
 */
void loopSeguridad(){
  lcd.setCursor(contadorDigitos, 1);
  char key = keypad.getKey();
  if (contadorDigitos < 4)
    {
      lcd.cursor();
    }
    else {
      lcd.noCursor();
    }

  if(!key) return;

  taskTime5.Stop();
  taskTime5.Start();

  if(contadorDigitos < 4) {
    varComparar[contadorDigitos] = key;
    lcd.print("*");
    contadorDigitos++;
  }

  if(contadorDigitos == 4) {
    bool clvCorrecta = true;
    for(int i = 0; i < 4; i++) {
      if(varComparar[i] != varContrasenia[i]) {
        clvCorrecta = false;
        break;
      }
    }
    if(clvCorrecta){
      claveCorrecta();
    } else {
      claveIncorrecta();
    }
  }
}

/**
 * @brief Resets the security system for another password attempt.
 * 
 * This function turns off the blue LED, resets the LCD, and starts the loop task
 * for another password attempt, incrementing the attempt counter.
 */
void again(){
  digitalWrite(LED_BLUE, LOW);
  reiniciarLCD();
  lcd.print("Clave:");      
  contadorIntentos++;
  taskLoop.Start(); 
}

#endif  // SEGURIDAD
