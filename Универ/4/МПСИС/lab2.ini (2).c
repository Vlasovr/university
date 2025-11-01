#include <msp430.h>

#define NUM_CHECKS 10
#define RED_MODE 0
#define YELLOW_MODE 1
#define BOTH_MODE 2
#define NUM_MODES 3
#define TOGGLE_DELAY 10000
 
const unsigned int debounce_delay = 15;
const unsigned int min_valid_clicks = NUM_CHECKS * 70 / 100;

volatile unsigned int mode = BOTH_MODE;

void delay(unsigned int cycles);
void next_mode(void);
void toggle_current_mode(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
     
    P1DIR |= BIT0;    
    P8DIR |= BIT1;   
    P1OUT |= BIT0;   
    P8OUT |= BIT1;
     
    P1DIR &= ~BIT7;
    P1REN |= BIT7;
    P1OUT |= BIT7;
    P1IE |= BIT7;
    P1IES |= BIT7;  
    P1IFG &= ~BIT7;

    __enable_interrupt();

    for (;;)
    {
        toggle_current_mode();
        delay(TOGGLE_DELAY);
    }
}

void delay(unsigned int cycles)
{
    volatile unsigned long counter;
    for (counter = cycles; counter > 0; --counter);
}
 
void next_mode(void)
{
    mode = (mode + 2) % NUM_MODES;
    P1OUT &= ~BIT0;
    P8OUT &= ~BIT1;
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
 
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if (P1IFG & BIT7)
    {
        unsigned int i, count_valid = 0;

        if (P1IES & BIT7)  
        {
            for (i = 0; i < NUM_CHECKS; ++i) {
                delay(debounce_delay);
                if (!(P1IN & BIT7)) ++count_valid; 
            }
            if (count_valid >= min_valid_clicks) {
                P1IES &= ~BIT7; 
            }
        }
        else   
        {
            for (i = 0; i < NUM_CHECKS; ++i) {
                delay(debounce_delay);
                if (P1IN & BIT7) ++count_valid;  
            }
            if (count_valid >= min_valid_clicks) {
                next_mode();   
            }
            P1IES |= BIT7;         
        }

        P1IFG &= ~BIT7;  
    }
}
