#include "inc/tm4c1294ncpdt.h"
#include "int_handler.h"
#include <stdint.h>

extern volatile uint8_t direction;
//extern volatile uint32_t direction;
int semaphore;



void IntPortDHandler(void) {
    GPIO_PORTD_AHB_ICR_R = 0x02;
    if (GPIO_PORTD_AHB_DATA_R & 0x02) {  // Rising edge detected
        TIMER1_CTL_R |= 0x01;  // Start Timer 1

    } else {  // Falling edge detected

        duration = TIMER1_TAR_R;
        TIMER1_CTL_R &= ~0x01;  // Stop Timer 1
       TIMER1_TAV_R = 0x00000000;


    }


}


void IntPortJHandler(void) { // measure the time of the button being pressed
    if (!(GPIO_PORTJ_AHB_DATA_R & 0x01)) {  // Rising edge detected
        TIMER1_CTL_R |= 0x01;  // Start Timer 1

    } else {  // Falling edge detected


        duration = TIMER1_TAR_R;
        TIMER1_CTL_R &= ~0x01;  // Stop Timer 1
       TIMER1_TAV_R = 0x00000000;


    }
    GPIO_PORTJ_AHB_ICR_R = 0x01;  // Clear PJ0 interrupt

}
/*
    if (GPIO_PORTJ_AHB_RIS_R & 0x01) {
        GPIO_PORTJ_AHB_ICR_R = 0x01;  // Clear interrupt flag
        printf("GPIO Port J Interrupt Rising\n");

        // Add logic here for what happens on Port J interrupt (e.g., toggle LED)
    } */



void config_gpio_interrupts(void) {
        // Configure GPIO Port J for edge-triggered interrupts
        GPIO_PORTJ_AHB_IS_R &= ~0x01;   // Edge-sensitive
        GPIO_PORTJ_AHB_IBE_R |= 0x01;   // Both rising and falling edges
        GPIO_PORTJ_AHB_ICR_R |= 0x01;   // Clear any prior interrupt
        GPIO_PORTJ_AHB_IM_R |= 0x01;    // Unmask the interrupt
        NVIC_EN1_R |= (1 << 19);  // Enable IRQ 51 for Port J 32 + 19 = 51


        GPIO_PORTD_AHB_IS_R &= ~0x02;   // Edge-sensitive
        GPIO_PORTD_AHB_IBE_R |= 0x02;   // Both rising and falling edges
        GPIO_PORTD_AHB_ICR_R |= 0x02;   // Clear any prior interrupt
        GPIO_PORTD_AHB_IM_R |= 0x02;    // Unmask the interrupt
        NVIC_EN0_R |= (1 << 3);



    }



