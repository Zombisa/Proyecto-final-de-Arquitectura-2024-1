/**
 * @file utilidad.h
 * @brief Header file containing utility functions and task declarations.
 * @author Isabela Mosquera Fernandez
 * @author Isabela Sanchez Saavedra
 */
#ifndef UTILIDAD_H
#define UTILIDAD_H

/**
 * @brief Reads user input from the serial monitor and returns the corresponding Input enum value.
 * 
 * @return The Input enum value corresponding to the user's input character.
 */
int readInput()
{
  Input currentInput = Input::Unknown;
  if (Serial.available())
  {
    char incomingChar = Serial.read();

    switch (incomingChar)
    {
      case 'X': currentInput = Input::Time3;   break;
      case 'Y': currentInput = Input::Time4;   break;
      case 'Z': currentInput = Input::Time7;   break;
      case 'W': currentInput = Input::Time10;   break;
      case 'C': currentInput = Input::ClaveCorrecta;  break;
      case 'S': currentInput = Input::SistBloqueado; break;
      case 'L': currentInput = Input::Luz; break;
      case 't': currentInput = Input::Temp; break;
      case 'H': currentInput = Input::Hall; break;
      case 'B': currentInput = Input::Boton; break;
      default: break;
    }
  }

  return currentInput;
}

/**
 * @brief Function to reset the LCD screen.
 * It clears the LCD screen after a delay.
 */
void reiniciarLCD(){
    delay(300);
    lcd.clear();
}

/**
 * @brief Function to execute actions when leaving the 'Inicio' state.
 * It turns off LEDs and performs cleanup.
 */
void salirInicio();

/**
 * @brief Function to execute actions when leaving the 'Config' state.
 * It stops menu-related tasks and resets input.
 */
void salirConfig();

/**
 * @brief Function to execute actions when leaving the 'MonAmbien' state.
 * It stops environmental monitoring tasks and resets input.
 */
void salirMonAmbien();

/**
 * @brief Function to execute actions when leaving the 'MonEventos' state.
 * It stops event monitoring tasks and resets input.
 */
void salirMonEventos();

/**
 * @brief Function to execute actions when leaving the 'Alarma' state.
 * It stops alarm-related tasks.
 */
void salirAlarma();

/**
 * @brief Function to execute actions when leaving the 'Bloqueo' state.
 * It performs cleanup.
 */
void salirBloqueo();

/**
 * @brief Function to print environmental parameters to the LCD screen.
 * It includes temperature, humidity, and light intensity.
 */
void printMonitorAmbiental();

/**
 * @brief Function to print magnetic field intensity to the LCD screen.
 * It monitors the magnetic field.
 */
void printMonitorHall();

/**
 * @brief Function to handle menu navigation in the main loop.
 * It processes keypad input and updates the menu accordingly.
 */
void loopLiquidMenu();

/**
 * @brief Function to increment the current variable value.
 * It is used in the menu for adjusting settings.
 */
void incrementarVariableActual();

/**
 * @brief Function to decrement the current variable value.
 * It is used in the menu for adjusting settings.
 */
void decrementarVariableActual();

/**
 * @brief Function to activate the alarm.
 * It plays a predefined melody through the buzzer.
 */
void alarma();

/**
 * @brief Function to handle retry attempt.
 * It resets variables for re-entering the password and starts necessary tasks.
 */
void again();

/**
 * @brief Function to handle incorrect password input.
 * It displays an error message on the LCD screen.
 */
void claveIncorrecta();

/**
 * @brief Function to handle security loop.
 * It captures keypad input and verifies the entered password.
 */
void loopSeguridad();

AsyncTask taskTime5(5000, false, [](){claveIncorrecta();});

AsyncTask taskLoop(1, true, loopSeguridad);

AsyncTask taskAgain(2000, false, again);

AsyncTask taskStopLoop(1, false, [] () { taskLoop.Stop();} );

AsyncTask taskLedAzul(800, true, [](){
  if(led== 0)
    digitalWrite(LED_BLUE,HIGH);
   else
    digitalWrite(LED_BLUE,LOW);
  led = led == 0 ? 1:0;
});

AsyncTask taskClaveCorrecta(200, false, [](){ input = Input::ClaveCorrecta;});

AsyncTask taskSistBloq(10, false, [] () {input = Input::SistBloqueado;  });

AsyncTask taskLoopMenu(100, true, loopLiquidMenu); 

AsyncTask taskBoton(10, false, [] () { input = Input::Boton;}); 

AsyncTask taskTemp(10, false, [](){ input = Input::Temp;});

AsyncTask taskHall(10, false, [](){ input = Input::Hall;});

AsyncTask taskLuz(10, false, [](){ input = Input::Luz;});

AsyncTask taskTime10(10000, false, []() { input = Input::Time10; });

AsyncTask taskTime7(7000, false, [](){input = Input::Time7;});

AsyncTask taskTime4(4000, false, [](){input = Input::Time4;});

AsyncTask taskTime3(3000, false, [](){input = Input::Time3;});

AsyncTask taskLoopMonitorAmbiental(300, true, printMonitorAmbiental);

AsyncTask taskLoopMonitorHall(300, true, printMonitorHall);

AsyncTask taskAlarma(1, true, alarma);

/**
 * @brief Function to update all asynchronous tasks.
 * It updates the state of all tasks in the system.
 */
void updateTask(){
  taskLoopMenu.Update();
  taskBoton.Update();
  taskLoopMonitorAmbiental.Update();
  taskLoopMonitorHall.Update();
  taskTime10.Update();
  taskTime7.Update();
  taskTime4.Update();
  taskTime3.Update();
  taskTemp.Update();
  taskHall.Update();
  taskLuz.Update();
  taskTime5.Update();
  taskLoop.Update();
  taskAgain.Update();
  taskStopLoop.Update();
  taskLedAzul.Update();
  taskClaveCorrecta.Update();
  taskSistBloq.Update();
}

void salirConfig();

void alarma() {
  unsigned long currentTime = millis();
  
  if (!tonePlaying) {
    if (currentTime - lastToneChangeTime >= durations[currentToneIndex]) {
      tone(BUZZER, tones[currentToneIndex], durations[currentToneIndex]);
      lastToneChangeTime = currentTime;
      tonePlaying = true;
    }
  } else {
    if (currentTime - lastToneChangeTime >= durations[currentToneIndex]) {
      noTone(BUZZER);
      currentToneIndex++;
      if (currentToneIndex >= sizeof(tones) / sizeof(tones[0])) {
        currentToneIndex = 0;
      }
      lastToneChangeTime = currentTime;
      tonePlaying = false;
    }
  }
}
void outputAmb()
{  
  Serial.println("Ini   Cfg   MonAmb   MonEve   Alm   Blq");
  Serial.println("              X                        ");
  Serial.println();

  reiniciarLCD();
  taskLoopMonitorAmbiental.Start();
  taskTime7.Start();
}

void outputEve()
{
  Serial.println("Ini   Cfg   MonAmb   MonEve   Alm   Blq");
  Serial.println("                        X              ");
  Serial.println();

  reiniciarLCD();
  taskLoopMonitorHall.Start();
  taskTime3.Start();
}

void outputAlarm()
{
  Serial.println("Ini   Cfg   MonAmb   MonEve   Alm   Blq");
  Serial.println("                               X       ");
  Serial.println();

  reiniciarLCD();
  lcd.print("Alarma activada!");
  taskAlarma.Start();
  taskTime4.Start();

}

void outputBloq()
{
  Serial.println("Ini   Cfg   MonAmb   MonEve   Alm   Blq");
  Serial.println("                                     X ");
  Serial.println();

  lcd.setCursor(0,0);
  lcd.print("Sist bloqueado");
  taskTime10.Start();
}



#endif
