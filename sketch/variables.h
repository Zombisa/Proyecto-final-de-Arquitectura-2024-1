#ifndef VARIABLES_H
#define VARIABLES_H

#include "StateMachineLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <string.h>
#include "AsyncTaskLib.h"
#include "DHT.h"
#include <OneButton.h>

#define LED_RED 14
#define LED_GREEN 15
#define LED_BLUE 16
#define DHTPIN 38
#define DHTTYPE DHT11   // DHT 11
#define BUZZER 13
#define buttonPin 1
#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);

unsigned char varContrasenia[4] = {'1', '2', '3', '4'};
unsigned char varComparar[4];
unsigned char contadorDigitos = 0;
unsigned char contadorIntentos = 0;
int sistemaBloqueado = 0;
unsigned char cmp = -1;
char c;
bool estado;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int photocellPin = A1;
const int HallPin = A0;
float outputValuePhoto = 0.0;
float outputValueHall = 0.0;
float humedad = 0.0;
float temperatura = 0.0;
unsigned long lastToneChangeTime = 0;
int currentToneIndex = 0;
int tones[] = {2000, 2500, 3000, 3500};
int durations[] = {300, 300, 300, 300};
bool tonePlaying = false;
DHT dht(DHTPIN, DHTTYPE);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {22,24,26,28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {30,32,34,36}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// State Alias
enum State
{
  //trabaja los estados como enumeración
  Inicio = 0,
  Config = 1,
  MonAmbien = 2,
  MonEventos = 3,  
  Alarma = 4,
  Bloqueo = 5
};

enum Input
{
  ClaveCorrecta = 0,
  Time3 = 1,
  Time4 = 2,
  Time7 = 3,
  Time10 = 4,
  Boton = 5,
  Luz = 6,  
  Temp = 7,
  Hall = 8,
  SistBloqueado = 9,
  Unknown = 10
};

StateMachine stateMachine(6, 13);

Input input;

int tempMax = 40; /**< Temperatura máxima permitida en grados. */
int tempMin = 0; /**< Temperatura mínima permitida en grados. */
int luzMax= 300; /**< Intensidad lumínica máxima permitida. */
int luzMin= 50; /**< Intensidad limínica mínima permitida. */
int hallMax = 1000; /**< Valor máximo del sensor Hall. */


OneButton btn = OneButton(
  buttonPin,  // Input pin for the button
  false,        // Button is active LOW
  false         // Enable internal pull-up resistor
);

void handleClick() {
  Serial.println("Se ha presionado el boton");
  input = Input::Boton;
}

int prendido = 0;

/**
 * @brief Función de bucle principal (loopS).
 *
 * Esta función se ejecuta periódicamente como parte de una tarea asincrónica.
 * Cada ciclo es necesario para la ejecucion de seguridad.
 */
void loopS();

/**
 * @brief Función de manejo de eventos (again).
 *
 * Esta función se ejecuta después de un intervalo específico como parte de una tarea asincrónica.
 * Sirve para volver intentar ingresar una contraseña.
 */
void again();

void runWrongPass();


AsyncTask taskTimeOut30k(5000, false, [](){runWrongPass();});
/**
 *!<Tarea asincrónica para el bucle principal. Ejecuta la función loopS cada 1 milisegundo (true indica repetición infinita).
 */
AsyncTask taskLoop(1, true, loopS);

/**
 *!< Tarea asincrónica para el manejo de eventos. Ejecuta la función again después de un intervalo de 2000 milisegundos (false indica una sola ejecución).
 */
AsyncTask taskAgain(2000, false, again);

/**
 * !<Tarea asincrónica para detener el bucle principal. Ejecuta una función lambda que detiene la tarea taskLoop después de 1 milisegundo.
 */
AsyncTask taskStopLoop(1, false, [] () { taskLoop.Stop();} );

AsyncTask taskOnLedB(800, true, [](){
  if(prendido== 0)
    digitalWrite(LED_BLUE,HIGH);
   else
    digitalWrite(LED_BLUE,LOW);
  prendido = prendido == 0 ? 1:0;
});

/**
 * !<Tarea para manejar la transición a la entrada correcta de la contraseña. 
 Esta tarea se ejecuta una vez después de 2000 ms y actualiza el estado de entrada a Input::correctPassword.
 */
AsyncTask taskCorrect(2000, false, [](){ input = Input::ClaveCorrecta;});

/**
 *!<Tarea para manejar el bloqueo del sistema.
   Esta tarea se ejecuta una vez después de 10 ms y actualiza el estado de entrada a Input::sysBlock.
 */
AsyncTask taskSysBlock(10, false, [] () {input = Input::SistBloqueado;  });

#endif