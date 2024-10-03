#include "MicroBit.h"
#include "samples/Tests.h"
#include <unistd.h>

MicroBit uBit;

void turnOn() {
    volatile uint32_t *d = (uint32_t *) 0x50000514;
    volatile uint32_t *p = (uint32_t *) 0x50000504;

    *d |= 0x0003FFFF;  //setting bits 0 to 17 to 1 (output)
    *d &= ~(1U << 13); //clearing bit 13 for button (input)
    *p |= 0x0003FFFF; //setting all bits on
}

void setLEDs(uint8_t value) {
    volatile uint32_t *d = (uint32_t *) 0x50000514;
    volatile uint32_t *p = (uint32_t *) 0x50000504;
    *d |= 0x0003FFFF;

    //static uint32_t currentLEDState = *p;  // Read the current LED state
    //currentLEDState &= 0xFFFFE000;
    uint32_t currentLEDState = 0;
    //*p &= ~0x0003FFFF; //setting all bits to 0

    if (value & 0x80) { //if bit 0 is 1
        currentLEDState = 0x4; // 2nd channel to 1
    }
    if (value & 0x40) {
        currentLEDState |= 0x0008;
    }
    if (value & 0x20) {
        currentLEDState |= 0x0010;
    }
    if (value & 0x010) {
        currentLEDState |= 0x0400;
    }
    if (value & 0x8) { 
        currentLEDState |= 0x200;
    }
    if (value & 0x4) {
        currentLEDState |= 0x0040;
    }
    if (value & 0x2) {
        currentLEDState |= 0x020000;
    }
    if (value & 0x1) {
        currentLEDState |= 0x2;
    }
    //ledState = *p;  // Store the current LED state
    *p = currentLEDState;  // Update the LED state
}

void delay(uint32_t milliseconds) {
    for (uint32_t i = 0; i < milliseconds; i++) {
        for (volatile int j = 0; j < 10000; j++) {
            // Adjust this loop for your specific microcontroller's clock speed
            // You may need to fine-tune the number 10000 to achieve the desired delay
        }
    }
}

void rollingCounter() {
    for (int count = 0; count < 256; count++) {
        setLEDs(count);
        delay(200000000);
    }
}

void knightRider() {
    uint32_t position = 1;
    int8_t direction = 1;  // 1 for right, -1 for left

    while (1) {
        setLEDs(1U << position);  // Turn on the LED at the current position

        // Delay to control the scrolling speed (adjust for your timing)
        delay(2000000);

        setLEDs(0);  // Turn off all LEDs

        delay(2000000);

        // Move the position based on the direction
        position += direction;

        // Check for the end and reverse direction
        if (position == 8) {
            direction = -1;  // Reverse to the left
        } else if (position == 0) {
            direction = 1;   // Reverse to the right
        }
    }
} 

int main()
{
    //turnOn();

    setLEDs(31);
    delay(8000000);
    setLEDs(200);

    rollingCounter();

    knightRider();
}