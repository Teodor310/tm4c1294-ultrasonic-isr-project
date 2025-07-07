/* The program is designed to measure the time of flight of the ultrasonic wave emitted by the ultrasonic sensor.
 * It has additional functionaly to test the ISR capabilities of the microcontroller by measuring the time of buttons
 * on the microcontroller being pressed.
 */
#include "inc/tm4c1294ncpdt.h"
#include <stdio.h>
#include <stdint.h>
#include "int_handler.h"

volatile uint8_t direction = original; // 1 for original order PN1, PN0, PF4, PF0. 0 for reverse
#define MAX_DISTANCE_STR_LEN 10
#define SPEED_OF_SOUND_CM_PER_US (0.03316 + 0.004 * 25)


//  Configuring  the Timer for measuring the time of flight of the ultrasonic wave
void configTimer1A(){

    SYSCTL_RCGCTIMER_R |= (1<<1); // timer 1 activated
      while(!(SYSCTL_PRTIMER_R & (1<<1)));
      TIMER1_CTL_R &= ~(1<<0); // stop the timer1A
      TIMER1_CFG_R = 0x00; // 32bits mode
      // TIMER1_TAMR_R = 0x32; // 32 math enabled, upwards, periodic mode
     TIMER1_TAMR_R = 0x11; // match disabled, one shot
     //TIMER1_TAPR_R = 245-1; //prescaler, match disabled, timer counts down
     //TIMER1_TAILR_R= 65306-1; // ILR = ceil(16M/123*0.5)
     //TIMER1_CTL_R |= 0x01; // start the timer

}

// Configure Ports
void configPorts(void) {

    // Configure Ports for the Buttons functionality
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8; // => enabling clock for PORTL, PORTJ
        while (!(SYSCTL_PRGPIO_R & ((1 << 8)))); // Wait for ports to be ready
        GPIO_PORTJ_AHB_PUR_R = 0x01;
        GPIO_PORTJ_AHB_DEN_R = 0x01;
        GPIO_PORTJ_AHB_DIR_R = 0x00;

// Configuring Port D (Echo pin and Trigger pin)
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;  // Enable clock for Port D
        while (!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R3));  // Wait for clock stability

        GPIO_PORTD_AHB_DIR_R |= ULTRASONIC_TRIGGER;      // PD0 as output, trigger pin
        GPIO_PORTD_AHB_DIR_R &= ~ULTRASONIC_ECHO;       // PD1 as input, echo pin
        GPIO_PORTD_AHB_DEN_R |= (ULTRASONIC_TRIGGER | ULTRASONIC_ECHO);  // Enable digital I/O



}

// wait 1 second with Timer 1A
void delayTimer1A(){
    TIMER1_CTL_R |= 0x01;    // starts Timer1A
    while((TIMER1_RIS_R & (1<<0)) == 0);    // waits for timeout flag 1 second
    TIMER1_ICR_R = 0x01;    // clears timeout flag
    TIMER1_CTL_R &= ~0x01; // stops Timer 1A
}


// Check whether timer works,
void TIMER1A_Handler(void) {
    while ((TIMER1_CTL_R & (1 << 0)) == 1) {  // Check if Timer 1 is enabled
        while ((TIMER1_RIS_R & (1 << 0)) == 0);  // Wait for timeout
        duration++;                 // Increment duration on timeout
        TIMER1_ICR_R = (1 << 0);    // Clear timeout interrupt flag
        printf("Timer Works\n");
    }

}

// optional function for printing out the time of flight
void printNumber(){
    /*
    if ((TIMER1_CTL_R & (1 << 0)) == 1){
            printf("Elapsed Time: %f seconds\n",  (duration) / (16000000));
            stopTime = 0 ;
            startTime = 0;
        }
    */

    printf("Elapsed Time: %f seconds\n",  (duration) / (16000000));
   stopTime = 0 ;
   startTime = 0;
}





// Delay function
void delayMicroseconds(uint32_t microseconds) {
    uint32_t counter;
    for (counter = 0; counter < (microseconds * 3); counter++);
}

// Triggering ultrasonic pulse
void triggerUltrasonicPulse() {
    GPIO_PORTD_AHB_DATA_R |= ULTRASONIC_TRIGGER;  // Set trigger high
    delayMicroseconds(10);                         // Wait 10 microseconds
    GPIO_PORTD_AHB_DATA_R &= ~ULTRASONIC_TRIGGER; // Set trigger low
}



int main(void) {
    configTimer1A();
    configPorts();
    config_gpio_interrupts();

    while(1){
      //  printf("Program Flow\n");
       //  TIMER1A_Handler();
       // printNumber();
        triggerUltrasonicPulse();
        delayMicroseconds(500000);
        // Calculating the distance to the object based on the time of flight of the ultrasonic wave
        uint32_t   measuredDistance = ((((float)duration/ 16000000) * 34300) / 2);


        printf("Distance %d  cm\n",  measuredDistance);
        delayMicroseconds(500000);


    }

}
