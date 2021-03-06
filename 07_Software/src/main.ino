/*
 * This is the main Programm of the Ventilator.
 */
#include <Arduino.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h> // By Frank The Brabaner
#include "potentiometer.h"
#include "pressureSensor.h"
#include "dcMotor.h"

/* 
 * Defines for the ventilator, fill in your spezific parameters here.
 */
#define DEBUG true
// ranges for the parameters
#define RANGE_VOLUME 10
#define RANGE_BREATHS 10
#define RANGE_PROPORTION 10
// LCD defines
#define LCD_ADRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
// potentiometer pin defines
#define POTI_1 A0
#define POTI_2 A1
#define POTI_3 A2
// EEPROM defines
#define ADRESS_VOLUME 0
#define ADRESS_BREATHS 1
#define ADRESS_PROPORTIONS 2
// pressure sensor pin defines
#define PRESSURE_ELECTRODE A3
#define PRESSURE_TOPSENSOR 5
#define PRESSURE_BOTTOMENSOR 6
// motor pin defines
#define MOTOR_POWER 4
#define MOTOR_SENSOR 2

// constructors for the parts
potentiometer poti1(A0, RANGE_VOLUME); // poti(pin, range);
potentiometer poti2(A1, RANGE_BREATHS);
potentiometer poti3(A2, RANGE_PROPORTION);
LiquidCrystal_I2C display(LCD_ADRESS, LCD_COLUMNS, LCD_ROWS);						 // display(adress, columns, rows);
dcMotor motor(MOTOR_POWER, MOTOR_SENSOR);											 // motor(motorPin, sensorPin);
pressureSensor sensor(PRESSURE_ELECTRODE, PRESSURE_TOPSENSOR, PRESSURE_BOTTOMENSOR); // sensor(electodePin, topPin, bottomPin);

// variables for the Program sequence

// function for code validating
void testMotor()
{
	if (motor.rotate())
		Serial.println(motor.getRpm());
}
// funtions for programm sequence
void show(String topic, uint8_t value);
void handlePotentiometers(uint8_t &volume, uint8_t &breath, uint8_t &proportions);

void setup()
{
	// variables for the parameters
	static uint8_t volume = EEPROM.read(ADRESS_VOLUME);
	static uint8_t breath = EEPROM.read(ADRESS_BREATHS);
	static uint8_t proportions = EEPROM.read(ADRESS_PROPORTIONS);
	// initialise the display
	display.init();
	display.backlight();
	// initialise the serial port
	Serial.begin(9600);
	//#############################//
	// main loop of the ventilator //
	//#############################//
	while (true)
	{
		handlePotentiometers(volume, breath, proportions);
		testMotor();
	}
}

// test functions
// head functions
void handlePotentiometers(uint8_t &volume, uint8_t &breath, uint8_t &proportions)
{
	if (poti1.isTurned())
	{
		volume = poti1.getValue();
		show("Volume: ", volume);
	}
	if (poti2.isTurned())
	{
		breath = poti2.getValue();
		show("Breath: ", breath);
	}
	if (poti3.isTurned())
	{
		proportions = poti3.getValue();
		show("Proportions: ", proportions);
	}
}
// core functions
void show(String topic, uint8_t value)
{
	display.clear();
	display.setCursor(0, 0); // setCourser(column, row);
	display.print(topic);
	if (value < 10)
		display.setCursor(15, 0);
	else
		display.setCursor(14, 0);
	display.print(value);
}