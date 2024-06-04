#ifndef VARIABLES_H
#define VARIABLES_H

#include "StateMachineLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <string.h>
#include "AsyncTaskLib.h"
#include "DHT.h"

#define LED_RED 14
#define LED_GREEN 15
#define LED_BLUE 16
#define DHTPIN 24
#define DHTTYPE DHT11   // DHT 11
#define BUZZER 13
#define buttonPin 1
#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);

unsigned char varContrasenia[4] = {'1', '2', '3', '4'};
unsigned char varComparar[4];
unsigned char contadorDigitos = 0;
unsigned char contadorIntentos = 0;
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

#endif