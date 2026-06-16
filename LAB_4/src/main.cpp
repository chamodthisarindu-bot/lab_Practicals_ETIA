#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SENSOR PD2
#define RESET_BTN PD3

#define GREEN_LED PB0
#define YELLOW_LED PB1
#define RED_LED PB2

int main(void)
{
    // LED pins as output
    DDRB |= (1 << GREEN_LED) |
            (1 << YELLOW_LED) |
            (1 << RED_LED);

    // Sensor and button as input
    DDRD &= ~((1 << SENSOR) | (1 << RESET_BTN));

    // Enable pull-up for reset button
    PORTD |= (1 << RESET_BTN);

    const uint8_t initial_capacity = 10;

    uint8_t vehicles_entered = 0;
    uint8_t available_slots = initial_capacity;

    uint8_t previous_state = 0;
    uint8_t current_state;

    while (1)
    {
        // Read sensor
        current_state = (PIND & (1 << SENSOR)) ? 1 : 0;

        // State change detection
        if (current_state == 1 &&
            previous_state == 0 &&
            vehicles_entered < initial_capacity)
        {
            vehicles_entered++;
            available_slots = initial_capacity - vehicles_entered;

            _delay_ms(200); // debounce
        }

        previous_state = current_state;

        // Reset button pressed
        if (!(PIND & (1 << RESET_BTN)))
        {
            vehicles_entered = 0;
            available_slots = initial_capacity;

            _delay_ms(200);
        }

        // LED Status Logic

        // Green: More than half available
        if (available_slots > (initial_capacity / 2))
        {
            PORTB |= (1 << GREEN_LED);
            PORTB &= ~((1 << YELLOW_LED) |
                       (1 << RED_LED));
        }

        // Yellow: Half or less but not full
        else if (available_slots > 0)
        {
            PORTB |= (1 << YELLOW_LED);
            PORTB &= ~((1 << GREEN_LED) |
                       (1 << RED_LED));
        }

        // Red: Full
        else
        {
            PORTB |= (1 << RED_LED);
            PORTB &= ~((1 << GREEN_LED) |
                       (1 << YELLOW_LED));
        }
    }
}