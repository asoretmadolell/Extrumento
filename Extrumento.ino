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

// Instantiations
LiquidCrystal_I2C myLcd( 0x27, 16, 2 );

// Menu logic
const int numOfPages = 3;
volatile int currentPage = 0;
volatile int parameters[ numOfPages ];
volatile bool screenChanged = false;
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
    Wire.begin();
    myLcd.begin( 16, 2 );
    myLcd.backlight();
    myLcd.print( "Extrumento v0.2" );
    delay( 2000 );
    printScreen();

    pinMode( BUTTON1, INPUT );
    pinMode( BUTTON2, INPUT );

    attachInterrupt( 0, button1isr, RISING );
    attachInterrupt( 1, button2isr, RISING );
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
}

/*****************************************************************************/
/*                                                                           */
/* printScreen()                                                             */
/*                                                                           */
/*****************************************************************************/
void printScreen()
{
    myLcd.clear();
    myLcd.print( screens[ currentPage ][ 0 ] );
    myLcd.setCursor( 0, 1 );
    myLcd.print( parameters[ currentPage ] );
    myLcd.print( " " );
    myLcd.print( screens[ currentPage ][ 1 ] );
    screenChanged = false;
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
