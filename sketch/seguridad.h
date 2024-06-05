#ifndef SEGURIDAD
#define SEGURIDAD

#include "variables.h"

/**
* @brief Inicializa las variables para el correcto funcionamiento.
* Tambien inicia la tarea para el loop 
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
* @brief Hace las acciones necesarias para cuando la contraseña es correcta
*/
void runRightPass(){
  reiniciarLCD();
  lcd.print("Clave correcta");
  digitalWrite(LED_GREEN, HIGH);
  taskStopLoop.Start();
  taskCorrect.Start();
  taskTimeOut30k.Stop();
  
}

/**
* @brief Hace las acciones necesarias para un intento fallido, incluyendo validar si es necesario bloquear el sistema
*/
void runWrongPass(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Clave incorrecta");
  digitalWrite(LED_BLUE, HIGH);

  for(size_t i=0; i<4; i++)
    varComparar[i] = '*';
  taskStopLoop.Start(); 
  if(contadorIntentos == 2){
    taskSysBlock.Start();
  }else{
    taskAgain.Start();
    contadorDigitos = 0;
  }

  reiniciarLCD();
}

/**
* @brief Funcion que se va a ejecutar cada vez para validar si se presiono un key y recibir la contraseña
*/
void loopS(){
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

  taskTimeOut30k.Stop();
  taskTimeOut30k.Start();

  // Recibimos caracter ingresado si no hemos alcanzado el límite de 4 caracteres
  if(contadorDigitos < 4) {
    varComparar[contadorDigitos] = key;
    lcd.print("*");
    contadorDigitos++;
  }

  if(contadorDigitos == 4) {
    bool passCorrect = true;
    for(int i = 0; i < 4; i++) {
      if(varComparar[i] != varContrasenia[i]) {
        passCorrect = false;
        break;
      }
    }
    if(passCorrect){
      runRightPass();
    } else {
      runWrongPass();
    }
  }
}


/**
* @brief Hace las acciones para volver a intentar ingresar la contraseña 
*/
void again(){
  digitalWrite(LED_BLUE, LOW);
  reiniciarLCD();
  lcd.print("Clave:");      
  contadorIntentos++;
  taskLoop.Start(); 
}

#endif  // SEGURIDAD
