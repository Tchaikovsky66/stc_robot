#include <button.h>
#include <delay.h>

//按键20按下
char Button20_Pressed(void)
{
    if (P20 == 0) // Check if button is pressed (assuming active low)
    {
        DelayMs(20); // Debounce delay
        if (P20 == 0) // Check again to confirm
        {
            while (P20 == 0); // Wait for button release
            DelayMs(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}

char Button21_Pressed(void)
{
    if (P21 == 0) // Check if button is pressed (assuming active low)
    {
        DelayMs(20); // Debounce delay
        if (P21 == 0) // Check again to confirm
        {
            while (P21 == 0); // Wait for button release
            DelayMs(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}

char Button44_Pressed(void)
{
    if (P44 == 0) // Check if button is pressed (assuming active low)
    {
        DelayMs(20); // Debounce delay
        if (P44 == 0) // Check again to confirm
        {
            while (P44 == 0); // Wait for button release
            DelayMs(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}