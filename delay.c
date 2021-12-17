#include <pic18f4520.h>
#include "config.h"
#include "delay.h"

void timed(unsigned char ms) {
    volatile unsigned char j, k, w;

    for (w = 0; w < ms; w++) {
        for (w = 0; w < 100; w++) { //Delay
            for (j = 0; j < 41; j++) {
                for (k = 0; k < 3; k++);
            }
        }
    }
}
