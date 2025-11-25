// гр. 250541 Власов Роман Евгеньевич
// лабораторная работа №1
// Тема: «Среда разработки Code Composer Studio. Плата MSP-EXP430F5529. Цифровой ввод-вывод»

#include <msp430.h>

#define NUM_CHECKS 10
#define RED_MODE 0
#define YELLOW_MODE 1
#define BOTH_MODE 2
#define NUM_MODES 3
#define INACTIVE 0
#define ACTIVE 1
#define TOGGLE_DELAY 10000

const unsigned int debounce_delay = 15;
const unsigned int min_valid_clicks = NUM_CHECKS * 70 / 100;

unsigned int mode = BOTH_MODE;
unsigned int btn_prev_state = INACTIVE;

void delay(unsigned int cycles);
unsigned char debounce_button(void);
void toggle_current_mode(void);
void next_mode(void);
void handle_button_press(void);
void manage_led_mode(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;
    P8DIR |= BIT1;
    P1DIR &= ~BIT7;
    P1REN |= BIT7;
    P1OUT |= BIT7;
    P1OUT |= BIT0;
    P8OUT |= BIT1;

    for (;;)
    {
        if (mode == BOTH_MODE)
            handle_button_press();
        else
            manage_led_mode();
    }
}

void delay(unsigned int cycles)
{
    volatile unsigned long counter;
    for (counter = cycles; counter > 0; --counter);
}

void handle_button_press(void) {
    if (debounce_button())
    {
        next_mode();
    }
}

void manage_led_mode(void) {
    toggle_current_mode();
    unsigned int i = 0;
    for (i = 0; i < TOGGLE_DELAY; ++i)
    {
        if (debounce_button())
        {
            next_mode();
            if (mode == BOTH_MODE) toggle_current_mode();
            break;
        }
    }
}

void next_mode(void) { 
    mode = (mode + 2) % NUM_MODES;
    P1OUT &= ~BIT0;
    P8OUT &= ~BIT1;
}

unsigned char debounce_button(void)
{
    if (!(P1IN & BIT7) && !btn_prev_state)
    {
        unsigned int count_valid = 0;
        unsigned int i;
        for (i = 0; i < NUM_CHECKS; ++i) {
            delay(debounce_delay);
            if (!(P1IN & BIT7))
                ++count_valid;
        }
        if (min_valid_clicks <= count_valid)
        {
            btn_prev_state = ACTIVE;
            return INACTIVE;
        }
    }
    else if ((P1IN & BIT7) && btn_prev_state)
    {
        btn_prev_state = INACTIVE;
        return ACTIVE;
    }
    return INACTIVE;
}

void toggle_current_mode(void)
{
    switch (mode)
    {
    case RED_MODE:
        P1OUT ^= BIT0;
        break;

    case YELLOW_MODE:
        P8OUT ^= BIT1;
        break;

    case BOTH_MODE:
        P1OUT |= BIT0;
        P8OUT |= BIT1;
        break;
    }
}
