#include "variables.h"
#include "utilidad.h"
#include "stateMachine.h"
#include "seguridad.h"
#include "monitor.h"

void setup() 
{
  Serial.begin(9600);

  Serial.println("Starting State Machine...");
  setupStateMachine();  
  Serial.println("Start Machine Started");

  dht.begin();
  // Initial state
  stateMachine.SetState(Inicio, false, true);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  pinMode(buttonPin, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

}

void loop() 
{
  stateMachine.Update();
  updateTask();
  
}
