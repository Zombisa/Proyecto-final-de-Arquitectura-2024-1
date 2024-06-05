/**
 * @file state_machine.h
 * @brief Header file containing the state machine setup and related functions.
 * @author Isabela Mosquera Fernandez
 * @author Isabela Sanchez Saavedra
 */
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "variables.h"
#include "seguridad.h"
#include "menu.h"

/**
 * @brief Sets up the state machine with states, transitions, and actions.
 * 
 * This function defines the states and transitions for the state machine,
 * and sets up actions to be performed on entering and leaving each state.
 */
void setupStateMachine()
{
  // Add transitions
  stateMachine.AddTransition(Inicio, Config, []() { return input == ClaveCorrecta; });
  stateMachine.AddTransition(Inicio, Bloqueo, []() { return input == SistBloqueado; });
  stateMachine.AddTransition(Config, MonAmbien, []() { return input == Boton; });  
  stateMachine.AddTransition(MonAmbien, MonEventos, []() { return input == Time7; });  
  stateMachine.AddTransition(MonAmbien, Config, []() { return input == Boton; });  
  stateMachine.AddTransition(MonAmbien, Alarma, []() { return input == Temp; });  
  stateMachine.AddTransition(MonAmbien, Alarma, []() { return input == Luz; });  
  stateMachine.AddTransition(MonEventos, MonAmbien, []() { return input == Time3; });  
  stateMachine.AddTransition(MonEventos, Config, []() { return input == Boton; });  
  stateMachine.AddTransition(MonEventos, Alarma, []() { return input == Hall; });  
  stateMachine.AddTransition(Alarma, MonAmbien, []() { return input == Time4; }); 
   stateMachine.AddTransition(Alarma, Inicio, []() { return input == Input::Boton;   });
  stateMachine.AddTransition(Bloqueo, Inicio, []() { return input == Time10; }); 

  // Add actions
  stateMachine.SetOnEntering(Inicio, outputIni);
  stateMachine.SetOnEntering(Config, outputConf);
  stateMachine.SetOnEntering(MonAmbien, outputAmb);
  stateMachine.SetOnEntering(MonEventos, outputEve);
  stateMachine.SetOnEntering(Alarma, outputAlarm);
  stateMachine.SetOnEntering(Bloqueo, outputBloq);

  stateMachine.SetOnLeaving(Inicio, salirInicio);
  stateMachine.SetOnLeaving(Config, salirConfig);
  stateMachine.SetOnLeaving(MonAmbien, salirMonAmbien);
  stateMachine.SetOnLeaving(MonEventos, salirMonEventos);
  stateMachine.SetOnLeaving(Alarma, salirAlarma);
  stateMachine.SetOnLeaving(Bloqueo, salirBloqueo);
}

/**
 * @brief Actions to perform when leaving the Inicio state.
 * This function turns off all LEDs and prints a message to the serial monitor.
 */
void salirInicio(){
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
  Serial.println("Saliendo Inicio"); 
  Serial.println();
}

/**
 * @brief Actions to perform when leaving the Config state.
 * This function stops the menu loop task and resets the input to Unknown.
 */
void salirConfig(){
  taskLoopMenu.Stop();
  Serial.println("Saliendo Config"); 
  Serial.println();
  input = Input::Unknown;
}

/**
 * @brief Actions to perform when leaving the MonAmbien state.
 * This function stops the environmental monitoring loop task and resets the input to Unknown.
 */
void salirMonAmbien(){
  Serial.println("Saliendo MonAmbien"); 
  Serial.println();
  taskLoopMonitorAmbiental.Stop();
  input = Input::Unknown;
}

/**
 * @brief Actions to perform when leaving the MonEventos state.
 * This function stops the event monitoring loop task and resets the input to Unknown.
 */
void salirMonEventos(){
  Serial.println("Saliendo MonEventos"); 
  Serial.println();
  taskLoopMonitorHall.Stop();
  
  input = Input::Unknown;
}

/**
 * @brief Actions to perform when leaving the Alarma state.
 * This function stops the alarm task.
 */
void salirAlarma(){
  Serial.println("Saliendo Alarma"); 
  Serial.println();
  
  taskAlarma.Stop();
}

/**
 * @brief Actions to perform when leaving the Bloqueo state.
 * This function prints a message to the serial monitor.
 */
void salirBloqueo(){
  Serial.println("Saliendo Bloqueo"); 
  Serial.println();
}

#endif  // STATE_MACHINE_H