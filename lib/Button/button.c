#include <button.h>
#include <delay.h>

//按键20按下
char Button20_Pressed(void)
{
    if (P2_0 == 0) // Check if button is pressed (assuming active low)
    {
        DelayMs(20); // Debounce delay
        if (P2_0 == 0) // Check again to confirm
        {
            while (P2_0 == 0); // Wait for button release
            DelayMs(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}

char Button21_Pressed(void)
{
    if (P2_1 == 0) // Check if button is pressed (assuming active low)
    {
        DelayMs(20); // Debounce delay
        if (P2_1 == 0) // Check again to confirm
        {
            while (P2_1 == 0); // Wait for button release
            DelayMs(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}

char Button44_Pressed(void)
{
    if (P4_4 == 0) // Check if button is pressed (assuming active low)
    {
        DelayMs(20); // Debounce delay
        if (P4_4 == 0) // Check again to confirm
        {
            while (P4_4 == 0); // Wait for button release
            DelayMs(20); // Debounce delay
            return 1; // Button press confirmed
        }
    }
    return 0; // No button press
}