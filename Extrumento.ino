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

LiquidCrystal_I2C lcd( 0x27, 16, 2 );

// Instantiate all of our peripherals
const int numOfInputs = 3;
const int inputPins[ numOfInputs ] = { 8, 9, 10 };
int inputState[ numOfInputs ];
int lastInputState[ numOfInputs ] = { LOW, LOW, LOW };
bool inputFlags[ numOfInputs ] = { LOW, LOW, LOW };
long lastDebounceTime[ numOfInputs ] = { 0, 0, 0 };
long debounceDelay = 5;

// Menu logic
const int numOfScreens = 3;
int currentScreen = 0;
String screens[ numOfScreens ][ 2 ] =
{
    {"Temperatura", "C"},
    {"Velocidad", "rpm"},
    {"Opcion 3", "Valor 3"}
};
int parameters[ numOfScreens ];

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
    lcd.begin( 16, 2 );
    lcd.backlight();
    lcd.print( "Extrumento v0.2" );
    delay( 2000 );

    for( int i = 0; i < numOfInputs; i++ )
    {
        pinMode( inputPins[ i ], INPUT);
        digitalWrite( inputPins[ i ], HIGH); // pull-up 20k
    }
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
    setInputFlags();
    resolveInputFlags();
}

/*****************************************************************************/
/*                                                                           */
/* setInputFlags()                                                           */
/*                                                                           */
/*****************************************************************************/
void setInputFlags()
{
    for( int i = 0; i < numOfInputs; i++ )
    {
        int reading = digitalRead( inputPins[ i ]);
        if( reading != lastInputState[ i ] )
        {
            lastDebounceTime[ i ] = millis();
        }
        if( ( millis() - lastDebounceTime[ i ] ) > debounceDelay )
        {
            if( reading != inputState[ i ] )
            {
                inputState[ i ] = reading;
                if( inputState[ i ] == HIGH )
                {
                    inputFlags[ i ] = HIGH;
                }
            }
        }
        lastInputState[ i ] = reading;
    }
}

/*****************************************************************************/
/*                                                                           */
/* resolveInputFlags()                                                       */
/*                                                                           */
/*****************************************************************************/
void resolveInputFlags()
{
    for( int i = 0; i < numOfInputs; i++ )
    {
        if( inputFlags[ i ] == HIGH )
        {
            inputAction( i );
            inputFlags[ i ] = LOW;
            printScreen();
        }
    }
}

/*****************************************************************************/
/*                                                                           */
/* inputAction()                                                             */
/*                                                                           */
/*****************************************************************************/
void inputAction( int input )
{
    if( input == 0 )
    {
        if( currentScreen == numOfScreens - 1 )
        {
            currentScreen = 0;
        }
        else
        {
            currentScreen++;
        }
    }
    else if( input == 1 )
    {
        parameterChange( 0 );
    }
    else if( input == 2 )
    {
        parameterChange( 1 );
    }
    /* this commented input is for navigating down
    else if( input == 3 )
    {
        if( currentScreen == 0 )
        {
            currentScreen = numOfScreens - 1;
        }
        else
        {
            currentScreen--;
        }
    }
    */
}

/*****************************************************************************/
/*                                                                           */
/* parameterChange()                                                         */
/*                                                                           */
/*****************************************************************************/
void parameterChange( int key )
{
    if( key == 0 )
    {
        parameters[ currentScreen ]--;
    }
    else if( key == 1 )
    {
        parameters[ currentScreen ]++;
    }
}

/*****************************************************************************/
/*                                                                           */
/* printScreen()                                                             */
/*                                                                           */
/*****************************************************************************/
void printScreen()
{
    lcd.clear();
    lcd.print( screens[ currentScreen ][ 0 ] );
    lcd.setCursor( 0, 1 );
    lcd.print( parameters[ currentScreen] );
    lcd.print(" ");
    lcd.print( screens[ currentScreen ][ 1 ] );
}

