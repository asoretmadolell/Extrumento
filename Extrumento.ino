#include "Arduino.h"

/*****************************************************************************/
/*                                                                           */
/* Extrumento                                                                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* Description:                                                              */
/*                                                                           */
/* An Arduino-based project for creating 3D printer filament.                */
/*                                                                           */
/*****************************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Defines
#define BUTTON1 2
#define BUTTON2 3

// Pins
const int dirPin = 4;
const int stepPin = 5;

// Instantiations
LiquidCrystal_I2C myLcd( 0x27, 16, 2 );

// Menu logic
const int numOfPages = 3;
volatile int currentPage = 0;
volatile int parameters[ numOfPages ];
volatile bool screenChanged = true;
String screens[ numOfPages ][ 2 ] =
{
{ "Temperatura", "C" },
{ "Velocidad", "rpm" },
{ "Opcion 3", "Valor 3" } };

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* setup()                                                                   */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
void setup()
{
    pinMode( BUTTON1, INPUT );
    pinMode( BUTTON2, INPUT );
    pinMode( stepPin, OUTPUT );
    pinMode( dirPin, OUTPUT );

    attachInterrupt( 0, button1isr, RISING );
    attachInterrupt( 1, button2isr, RISING );

    Wire.begin();
    myLcd.begin( 16, 2 );

    myLcd.backlight();
    myLcd.print( "Extrumento v0.2" );
    delay( 2000 );
}

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* loop()                                                                    */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
void loop()
{
    if( screenChanged ) printScreen();

    digitalWrite( dirPin, HIGH );

    for( int x = 0; x < 200; x++ )
    {
        digitalWrite( stepPin, HIGH );
        delayMicroseconds(500);
        digitalWrite( stepPin, LOW );
        delayMicroseconds(500);
    }
    delay(1000);

    digitalWrite( dirPin, LOW );

    for( int x = 0; x < 400; x++ )
    {
        digitalWrite( stepPin, HIGH );
        delayMicroseconds(500);
        digitalWrite( stepPin, LOW );
        delayMicroseconds(500);
    }
    delay(1000);
}

/*****************************************************************************/
/*                                                                           */
/* printScreen()                                                             */
/*                                                                           */
/*****************************************************************************/
void printScreen()
{
    //noInterrupts(); // disable interrupts momentarily in case the screen tries to access the variables while they're being modified.

    myLcd.clear();
    myLcd.print( screens[ currentPage ][ 0 ] );
    myLcd.setCursor( 0, 1 );
    myLcd.print( parameters[ currentPage ] );
    myLcd.print( " " );
    myLcd.print( screens[ currentPage ][ 1 ] );
    screenChanged = false;

    //interrupts();
}

/*****************************************************************************/
/*                                                                           */
/* button1isr()                                                              */
/*                                                                           */
/*****************************************************************************/
void button1isr()
{
    if( currentPage == numOfPages - 1 )
    {
        currentPage = 0;
    }
    else
    {
        currentPage++;
    }
    screenChanged = true;
}

/*****************************************************************************/
/*                                                                           */
/* button2isr()                                                              */
/*                                                                           */
/*****************************************************************************/
void button2isr()
{
    parameters[ currentPage ]++;
    screenChanged = true;
}
