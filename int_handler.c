#include "inc/tm4c1294ncpdt.h"
#include "int_handler.h"
#include <stdint.h>

extern volatile uint8_t direction;
//extern volatile uint32_t direction;
int semaphore;


/* Second pin of Port  D is assigned to be an Echo Pin.  When the sensor is triggered it emits an ultrasonic wave and
 * sets Echo Pin high. When the emitted wave is reflected and returns back to the sensor Echo Pin goes low.
 * ISR ensures that time of Echo Pin being high is measured.
 */
void IntPortDHandler(void) {
    GPIO_PORTD_AHB_ICR_R = 0x02; // clear interrupt fag
    if (GPIO_PORTD_AHB_DATA_R & 0x02) {  // Rising edge detected
        TIMER1_CTL_R |= 0x01;  // Start Timer 1

    } else {  // Falling edge detected

        duration = TIMER1_TAR_R; // time of flight of ultrasonic wave is recorded
        TIMER1_CTL_R &= ~0x01;  // Stop Timer 1
       TIMER1_TAV_R = 0x00000000;


    }


}


// Additional functionality to test the ISR
// It measures the time of the button being pressed
void IntPortJHandler(void) {
    if (!(GPIO_PORTJ_AHB_DATA_R & 0x01)) {  // Rising edge detected
        TIMER1_CTL_R |= 0x01;  // Start Timer 1

    } else {  // Falling edge detected


        duration = TIMER1_TAR_R;
        TIMER1_CTL_R &= ~0x01;  // Stop Timer 1
       TIMER1_TAV_R = 0x00000000;


    }
    GPIO_PORTJ_AHB_ICR_R = 0x01;  // Clear PJ0 interrupt

}




void config_gpio_interrupts(void) {
        // Configure GPIO Port J for edge-triggered interrupts
        GPIO_PORTJ_AHB_IS_R &= ~0x01;   // Edge-sensitive
        GPIO_PORTJ_AHB_IBE_R |= 0x01;   // Both rising and falling edges
        GPIO_PORTJ_AHB_ICR_R |= 0x01;   // Clear any prior interrupt
        GPIO_PORTJ_AHB_IM_R |= 0x01;    // Unmask the interrupt
        NVIC_EN1_R |= (1 << 19);  // Enable IRQ 51 for Port J 32 + 19 = 51

        // Configure GPIO Port D for edge-triggered interrupts
        GPIO_PORTD_AHB_IS_R &= ~0x02;   // Edge-sensitive
        GPIO_PORTD_AHB_IBE_R |= 0x02;   // Both rising and falling edges
        GPIO_PORTD_AHB_ICR_R |= 0x02;   // Clear any prior interrupt
        GPIO_PORTD_AHB_IM_R |= 0x02;    // Unmask the interrupt
        NVIC_EN0_R |= (1 << 3);



    }



