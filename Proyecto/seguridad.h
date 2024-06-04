#ifndef SEGURIDAD
#define SEGURIDAD

#include "utilidad.h"
#include "variables.h"

void claveCorrecta() {
  input = Input::ClaveCorrecta;
  contadorIntentos = 0;
  digitalWrite(LED_GREEN, HIGH);
  lcd.setCursor(0, 1);
  reiniciarLCD();
  lcd.print("Correcta :)");

  tone(BUZZER, 1800, 100);
  delay(100);
  tone(BUZZER, 2000, 100);
  delay(100);
  tone(BUZZER, 2200, 100);
  delay(100);
  tone(BUZZER, 2500, 350);
  delay(350);
  noTone(BUZZER);

  delay(1000);
  digitalWrite(LED_GREEN, LOW);
  reiniciarLCD();

  stateMachine.Update();
}

void claveIncorrecta() {
  digitalWrite(LED_BLUE, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("Incorrecta >:(");

  tone(BUZZER, 1000, 200);
  delay(200);
  noTone(BUZZER);

  delay(1000);
  digitalWrite(LED_BLUE, LOW);
  reiniciarLCD();
  lcd.print("Clave:");
  contadorIntentos++;
}

void tiempoAgotado() {
  digitalWrite(LED_BLUE, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("Tiempo agotado...");

  tone(BUZZER, 200, 150);
  delay(200);
  tone(BUZZER, 200, 150);
  delay(150);
  noTone(BUZZER);

  delay(1000);
  digitalWrite(LED_BLUE, LOW);
  reiniciarLCD();
  lcd.print("Clave:");
  contadorIntentos++;
}

boolean comprobarClave() {
  for (int i = 0; i < 4; i++) {
    if (varComparar[i] != varContrasenia[i]) {
      return false;
    }
  }
  return true;
}

bool updateDisplay() {
  if (contadorIntentos == 3) {
    digitalWrite(LED_RED, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Sist. bloqueado");
    if (contadorIntentos == 3) {
      tone(BUZZER, 100, 150);
      delay(150);
      noTone(BUZZER);
      contadorIntentos++;
      digitalWrite(LED_RED, LOW);
      input = Input::SistBloqueado;
      stateMachine.Update();      
      return true;
    }
  }
  else if (contadorDigitos == 4) {
    contadorDigitos = 0;
    estado = comprobarClave();
    if (estado == true) {
      claveCorrecta();
      delay(100);
      return true;
    }
    else {
      claveIncorrecta();
      return false;
    }
  }
}

// Auxiliar output functions that show the state debug
void outputIni()
{
  Serial.println("Ini   Cfg   MonAmb   MonEve   Alm   Blq");
  Serial.println(" X                                     ");
  Serial.println();

  reiniciarLCD();
  lcd.print("Clave:");

  
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);

  while (true) {
    unsigned int startTime;
    unsigned int actualTime;
    //Mostrar cursor
    if (millis() / 250 % 2  == 0 && contadorIntentos < 3)
    {
      lcd.cursor();
    }
    else {
      lcd.noCursor();
    }
    if (updateDisplay()) {
      break;
    }

    //Si se presiona una tecla, se muestra *
    char key = keypad.getKey();
    if (key) {
      //Se guarda el tiempo en el que se presiona una tecla
      startTime = millis();
      lcd.print("*");
      varComparar[contadorDigitos] = key;
      contadorDigitos++;
    }
    //Se actualiza el tiempo
    actualTime = millis();

    if (actualTime - startTime >= 4000 && contadorDigitos > 0) {
      tiempoAgotado();
      startTime = 0; //Se reinicia el tiempo actual
      contadorDigitos = 0;
    }
  }
}

#endif  // SEGURIDAD