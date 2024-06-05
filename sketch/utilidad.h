#ifndef UTILIDAD_H
#define UTILIDAD_H

// Auxiliar function that reads the user input
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



void reiniciarLCD(){
    delay(300);
    lcd.clear();
}

void salirInicio();
void salirConfig();
void salirMonAmbien();
void salirMonEventos();
void salirAlarma();
void salirBloqueo();

void printMonitorAmbiental();

void printMonitorHall();

void alarmaActivada(); 

void loopLiquidMenu();

void incrementarVariableActual();

void decrementarVariableActual();

void alarma();

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
  taskTimeOut30k.Update();
  taskLoop.Update();
  taskAgain.Update();
  taskStopLoop.Update();
  taskOnLedB.Update();
  taskCorrect.Update();
  taskSysBlock.Update();
}

void salirConfig();

bool isButtonPressed() {
  int boton = digitalRead(buttonPin); 
  if (boton == HIGH) {
    Serial.println("Se ha presionado el boton");
    return true;
  }
  return false;
}

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
