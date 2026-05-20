#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// Password sequence
uint8_t password[4] = {1, 2, 3, 4};
uint8_t entered[4];

int main(void)
{
    uint8_t i = 0;

    // LEDs as outputs
    DDRB |= (1 << PB0) | (1 << PB1);

    // Buttons as inputs
    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5));

    // Enable internal pull-up resistors
    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);

    while (1)
    {
        // Read 4 button presses
        if (!(PIND & (1 << PD2)))
        {
            entered[i] = 1;
            i++;
            _delay_ms(300);
        }

        else if (!(PIND & (1 << PD3)))
        {
            entered[i] = 2;
            i++;
            _delay_ms(300);
        }

        else if (!(PIND & (1 << PD4)))
        {
            entered[i] = 3;
            i++;
            _delay_ms(300);
        }

        else if (!(PIND & (1 << PD5)))
        {
            entered[i] = 4;
            i++;
            _delay_ms(300);
        }

        // Check password after 4 inputs
        if (i == 4)
        {
            uint8_t correct = 1;

            for (uint8_t j = 0; j < 4; j++)
            {
                if (entered[j] != password[j])
                {
                    correct = 0;
                    break;
                }
            }

            // Correct password
            if (correct)
            {
                PORTB |= (1 << PB0);   // Green LED ON
                PORTB &= ~(1 << PB1);  // Red LED OFF
            }

            // Wrong password
            else
            {
                PORTB |= (1 << PB1);   // Red LED ON
                PORTB &= ~(1 << PB0);  // Green LED OFF
            }

            _delay_ms(2000);

            // Turn OFF LEDs
            PORTB &= ~(1 << PB0);
            PORTB &= ~(1 << PB1);

            // Reset input counter
            i = 0;
        }
    }
}