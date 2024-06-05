/**
 * @file seguridad.h
 * @brief Header file containing functions for security management.
 */
#ifndef SEGURIDAD
#define SEGURIDAD

#include "variables.h"

/**
 * @brief Function to initialize security system.
 * It resets variables, clears LCD, and starts necessary tasks.
 */
void seguridad(){
 
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
 * @brief Function to handle correct password input.
 * It displays a success message, activates green LED, and plays a melody.
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
 * @brief Function to handle incorrect password input.
 * It displays an error message, activates blue LED, and manages retry attempts.
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
 * @brief Function to handle security loop.
 * It captures keypad input and verifies the entered password.
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
 * @brief Function to handle retry attempt.
 * It resets variables for re-entering the password and starts necessary tasks.
 */
void again(){
  digitalWrite(LED_BLUE, LOW);
  reiniciarLCD();
  lcd.print("Clave:");      
  contadorIntentos++;
  taskLoop.Start(); 
}

#endif  // SEGURIDAD
