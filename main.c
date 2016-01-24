#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "GPIO/gpio.h"
#include "binary.h"

static int initialiseGPIO();
static int getRandomNumber(int maximum);
static int setLedStates(BITS bits);
static int setLedState(int pin, int ledState, int ledIndex);
static void turnOffLeds();
static void tidyUpGPIO();

static int LED_STATE[4] = { 0, 0, 0, 0 };
static const double ONE_SECOND = 1000000;
static const int ITERATIONS = 1000;

/*
 * Program main entry point
 */
int main()
{
    // Seed random number generator
    srand(time(NULL));

    // Initialise GPIO pins for export and output
    if (initialiseGPIO() != EXIT_SUCCESS)
    {
        tidyUpGPIO();
        return EXIT_FAILURE;
    }

    DECIMAL decimal;
    int iteration = 0;
    int returnStatus = EXIT_SUCCESS;

    // Loop for a defined number of iterations, or failure
    while(iteration < ITERATIONS && returnStatus == EXIT_SUCCESS)
    {
        // Generate a number 0 - 15
        decimal.value = getRandomNumber(15);

        // Set LEDs (GPIO Pins 17, 18, 22 and 27) according to binary representation of the decimal value
        returnStatus = setLedStates(decimal.bits);

        // Sleep for a random number of micro seconds (half a second), well at least 100
        usleep((useconds_t)(getRandomNumber(ONE_SECOND/2) + 100));

        iteration++;
    }

    // Turn off all LEDs
    turnOffLeds();

    // Tidy up and exit
    tidyUpGPIO();

    return returnStatus;
}

/*
 * Initialise the GPIO pins for 'export' and output direction
 */
int initialiseGPIO()
{
    // LED on GPIO pin 17
    if (gpioExportAndDirection(GPIO_PIN_17, GPIO_OUTPUT) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // LED on GPIO pin 18
    if (gpioExportAndDirection(GPIO_PIN_18, GPIO_OUTPUT) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // LED on GPIO pin 22
    if (gpioExportAndDirection(GPIO_PIN_22, GPIO_OUTPUT) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // LED on GPIO pin 27
    if (gpioExportAndDirection(GPIO_PIN_27, GPIO_OUTPUT) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/*
 * Get a random number between 0 and the maximum
 */
int getRandomNumber(int maximum)
{
    return rand() % (maximum+1);
}

/*
 * Set the state of all the LEDs
 */
int setLedStates(BITS bits)
{
    // Bit 4 (1000)
    if (setLedState(GPIO_PIN_17, (int)bits.four, 0) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Bit 3 (0100)
    if (setLedState(GPIO_PIN_18, (int)bits.three, 1) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Bit 2 (0010)
    if (setLedState(GPIO_PIN_22, (int)bits.two, 2) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Bit 1 (0001)
    if (setLedState(GPIO_PIN_27, (int)bits.one, 3) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/*
 * Set the state of a single LED
 */
int setLedState(int gpioPin, int ledState, int ledIndex)
{
    // LED already at the correct state, so nothing to do
    if (LED_STATE[ledIndex] == ledState)
        return EXIT_SUCCESS;

    // Set LED to correct new state
    if (gpioWrite(gpioPin, ledState) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    // Remember state of LED
    LED_STATE[ledIndex] = ledState;

    return EXIT_SUCCESS;
}

/*
 * Turn off all four LEDs
 */
void turnOffLeds()
{
    // Ensure all LEDs are off
    gpioWrite(GPIO_PIN_17, GPIO_LOW);
    gpioWrite(GPIO_PIN_18, GPIO_LOW);
    gpioWrite(GPIO_PIN_22, GPIO_LOW);
    gpioWrite(GPIO_PIN_27, GPIO_LOW);
}

/*
 * Tidy up GPIO by 'unexporting'
 */
void tidyUpGPIO()
{
    // Unexport the GPIO pins
    gpioUnexport(GPIO_PIN_17);
    gpioUnexport(GPIO_PIN_18);
    gpioUnexport(GPIO_PIN_22);
    gpioUnexport(GPIO_PIN_27);
}
