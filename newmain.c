#include<proc/pic18f4520.h>
#include"adc.h"
#include"bits.h"
#include"config.h"
#include"serial.h"
#include"ds1307.h"
#include"keypad.h"
#include"lcd.h"
#include"ssd.h"
#include"timer.h"
#include"pwm.h"
#include "rgb.h"


void main(void) {
    
    lcdInit();
    kpInit();
    ssdInit();
    timerInit();
    serialInit();
    adcInit();
    
    lcdString("Oi leo");

}//endmain