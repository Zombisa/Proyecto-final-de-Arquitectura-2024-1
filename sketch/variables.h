/**
 * @file variables.h
 * @brief Header file containing global variables and definitions.
 */
#ifndef VARIABLES_H
#define VARIABLES_H

#include "StateMachineLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <string.h>
#include "AsyncTaskLib.h"
#include "DHT.h"
#include <OneButton.h>

#define LED_RED 14    /**< Red LED pin. */
#define LED_GREEN 15  /**< Green LED pin. */
#define LED_BLUE 16   /**< Blue LED pin. */
#define DHTPIN 38     /**< Pin connected to the DHT sensor. */
#define DHTTYPE DHT11 /**< Type of DHT sensor (DHT11). */
#define BUZZER 13     /**< Buzzer pin. */
#define buttonPin 1   /**< Button pin. */

/**
 * @brief Default password.
 */
unsigned char varContrasenia[4] = {'1', '2', '3', '4'};

/**
 * @brief Array to store user input.
 */
unsigned char varComparar[4];

/**
 * @brief Counter for entered digits.
 */
unsigned char contadorDigitos = 0;

/**
 * @brief Counter for password attempts.
 */
unsigned char contadorIntentos = 0;

/**
 * @brief Flag indicating system lock state.
 */
int sistemaBloqueado = 0;

/**
 * @brief Placeholder variable.
 */
bool estado;

/**
 * @brief Pins for LCD.
 */
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

/**
 * @brief Pin for photocell sensor.
 */
const int photocellPin = A1;

/**
 * @brief Pin for Hall effect sensor.
 */
const int HallPin = A0;

unsigned long lastToneChangeTime = 0;
int currentToneIndex = 0;

/**
 * @brief Predefined tones for the alarm.
 */
int tones[] = {2000, 2500, 3000, 3500};

/**
 * @brief Durations for each tone in the alarm.
 */
int durations[] = {300, 300, 300, 300};

/**
 * @brief Flag indicating whether a tone is currently playing.
 */
bool tonePlaying = false;

/**
 * @brief DHT sensor object.
 */
DHT dht(DHTPIN, DHTTYPE);

const byte ROWS = 4; /**< Four rows. */
const byte COLS = 4; /**< Four columns. */

/**
 * @brief Key mappings for the keypad.
 */
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

/**
 * @brief Row pins for the keypad.
 */
byte rowPins[ROWS] = {22,24,26,28};

/**
 * @brief Column pins for the keypad.
 */
byte colPins[COLS] = {30,32,34,36};

/**
 * @brief Keypad object.
 */
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/**
 * @brief LCD object.
 */
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/**
 * @brief State alias enumeration.
 */
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


/**
 * @brief Input event enumeration.
 */
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

/**
 * @brief State machine object.
 */
StateMachine stateMachine(6, 13);

/**
 * @brief Global variable to store input event.
 */
Input input;

int tempMax = 40; /**< Maximum temperature limit. */
int tempMin = 0; /**< Minimum temperature limit. */
int luzMax= 300; /**< Maximum light intensity limit. */
int luzMin= 50; /**< Minimum light intensity limit. */
int hallMax = 1000; /**< Maximum value for Hall effect sensor. */

/**
 * @brief Button object.
 */
OneButton btn = OneButton(
  buttonPin,  // Input pin for the button
  false,        // Button is active LOW
  false         // Enable internal pull-up resistor
);

/**
 * @brief Callback function for button press event.
 */
void handleClick() {
  Serial.println("Se ha presionado el boton");
  input = Input::Boton;
}

/**
 * @brief Placeholder variable.
 */
int led = 0;


#endif