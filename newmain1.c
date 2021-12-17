#include<proc/pic18f4520.h>
#include"keypad.h"
#include"lcd.h"
#include"ssd.h"
#include"timer.h"
#include"adc.h"
#include"bits.h"
#include"delay.h"
#include"config.h"
#include"pwm.h"
#include"serial.h"
#include"ds1307.h"
#include "rgb.h"

#define L_CLR 0x01
#define L_L1 0x80 
#define L_L2 0xC0
#define L_ON 0x0F
#define L_OFF 0X08

unsigned long int comidaMax;
unsigned long int potenc;
unsigned char auxSerial;
unsigned long int qtd;
unsigned int tempoParaEncher;

void qtdComidaLeitura() {
    unsigned long int oldValue;
    potenc = adcRead(0);
    if (oldValue != potenc) {
        lcdPosition(1, 7);
        lcdString("  Kg:");

        lcdChar((((200 * potenc) / 1023)) / 100 % 10 + 48);
        lcdChar((((200 * potenc) / 1023)) / 10 % 10 + 48);
        lcdChar((((200 * potenc) / 1023)) % 10 + 48);
    }
    oldValue = potenc;
}

void ShowOnDisplay() {
    
    int mil, cent, dez, un;

    mil = ((comidaMax / 1000) % 10);
    cent = ((comidaMax / 100) % 10);
    dez = ((comidaMax / 10) % 10);
    un = (comidaMax % 10);

    ssdDigit(mil, 0);
    ssdDigit(cent, 1);
    ssdDigit(dez, 2);
    ssdDigit(un, 3);
}

void EraseDisplay() {
    ssdDigit(0, 0);
    ssdDigit(0, 1);
    ssdDigit(0, 2);
    ssdDigit(0, 3);
}

void relogio() {
    
    float tempo;
    unsigned long int cont=0;
        for (cont = tempoParaEncher; cont>0; cont--) {
            
                ssdDigit(((cont / 100) % 10), 3);
                ssdDigit(((cont / 1000) % 6), 2);
                ssdDigit(((cont / 6000) % 10), 1);
                ssdDigit(((cont / 60000) % 10), 0);
       
                ssdUpdate();
                
                for (tempo = 0.0; tempo < 150.0; tempo++); 
    }
    EraseDisplay();
}

void leituraTeclas(void) {
    unsigned char tecla;
    kpDebounce();
    if (kpRead() != tecla) {
        tecla = kpRead();

        switch (kpReadKey()) {

            case 'U':
                PORTD = 0b00000011; // Acende o amarelo
                lcdCommand(L_L1);
                lcdString("CAVALOS   ");
                lcdCommand(L_L2);
                lcdPosition(0, 7);
                lcdString("  L:190Kg");
                lcdPosition(1, 0);
                lcdString("Qtd: 25");
                comidaMax = 190;
                tempoParaEncher = 18000; //3 MINUTOS
                break;

            case 'L':
                lcdCommand(L_L1);
                lcdString("VACAS   ");
                lcdCommand(L_L2);
                lcdPosition(0, 7);
                lcdString("  L:150Kg");
                lcdPosition(1, 0);
                lcdString("Qtd: 15");
                comidaMax = 150;
                tempoParaEncher = 15000;//2 MINUTOS E MEIO
                break;

            case 'D':
                lcdCommand(L_L1);
                lcdString("PORCOS  ");
                lcdCommand(L_L2);
                lcdPosition(0, 7);
                lcdString("  L:110Kg");
                lcdPosition(1, 0);
                lcdString("Qtd: 40");
                comidaMax = 110;
                tempoParaEncher = 13000;// 2 MINUTOS E 10
                break;

            case 'R':
                lcdCommand(L_L1);
                lcdString("GALINHAS  ");
                lcdCommand(L_L2);
                lcdPosition(0, 7);
                lcdString("  L: 30Kg");
                lcdPosition(1, 0);
                lcdString("Qtd: 90 ");
                comidaMax = 30;
                tempoParaEncher = 2000; //20 SEGUNDOS
                break;

            case 'S':
                lcdCommand(L_L1);
                lcdString("OVELHAS  ");
                lcdCommand(L_L2);
                lcdPosition(0, 7);
                lcdString("  L: 50Kg");
                lcdPosition(1, 0);
                lcdString("Qtd: 10 ");
                comidaMax = 50;
                tempoParaEncher = 4000; //40 SEGUNDOS
                break;

            case 'A':
                lcdCommand(L_L1);
                lcdString("TOUROS  ");
                lcdCommand(L_L2);
                lcdPosition(0, 7);
                lcdString("  L:120Kg");
                lcdPosition(1, 0);
                lcdString("Qtd: 8 ");
                comidaMax = 120;
                tempoParaEncher = 10000; // 1 MINUTO E 40 SEGUNDOS
                break;
                
            case 'X':
                turnOn(2);
                lcdCommand(L_L1);
                lcdString("Enchendo o Cocho");
                lcdCommand(L_L2);
                lcdPosition(1,0);
                lcdString("Aguarde");
                relogio();
                lcdCommand(L_CLR);
                lcdString("Cocho Cheio!");
                turnOff(2);
                break;

            default:
                break;
        }
    }
}

void comparacao() {

    qtd = (200 * potenc) / (1024 + 20);
    if (qtd > comidaMax) {
        ShowOnDisplay();
    }else if (qtd < comidaMax) {
        EraseDisplay();
    }
}

serial() {
    auxSerial = serialRead();
    if (auxSerial != 0) {
        switch (auxSerial) {
                serialSend(auxSerial);

            case 'U':
                lcdCommand(L_L1);
                lcdString("CAVALOS");
                lcdCommand(L_L2);
                lcdString("Qtd: 25");
                comidaMax = 190;
                break;

            case 'L':
                lcdCommand(L_L1);
                lcdString("VACAS");
                lcdCommand(L_L2);
                lcdString("Qtd: 15");
                comidaMax = 150;
                break;

            case 'D':
                lcdCommand(L_L1);
                lcdString("PORCOS");
                lcdCommand(L_L2);
                lcdString("Qtd: 40 ");
                comidaMax = 110;
                break;

            case 'R':
                lcdCommand(L_L1);
                lcdString("GALINHAS");
                lcdCommand(L_L2);
                lcdString("Qtd: 90 ");
                comidaMax = 30;
                break;

            case 'S':
                lcdCommand(L_L1);
                lcdString("OVELHAS  ");
                lcdCommand(L_L2);
                lcdString("Qtd: 10 ");
                comidaMax = 50;
                break;

            case 'B':
                lcdCommand(L_L1);
                lcdString("TOUROS  ");
                lcdCommand(L_L2);
                lcdString("Qtd: 8 ");
                comidaMax = 120;
                break;
                
            case 'X':
                lcdCommand(L_L1);
                lcdString("Enchendo o Cocho");
                lcdCommand(L_L2);
                lcdPosition(1,0);
                lcdString("Aguarde");
                relogio();
                lcdCommand(L_CLR);
                lcdString("Cocho Cheio!");
                break;

            default:
                break;

        }
    }
    potenc = adcRead(0);

    serialSend('Kg');
    serialSend(':');
    serialSend(' ');

    serialSend((((200 * potenc) / 1023)) / 100 % 10 + 48);
    serialSend((((200 * potenc) / 1023)) / 10 % 10 + 48);
    serialSend((((200 * potenc) / 1023)) / 1 % 10 + 48);

    serialSend('\n');

}

void main(void) {

    lcdInit();
    kpInit();
    ssdInit();
    timerInit();
    serialInit();
    adcInit();
    rgbInit();

    char Slot;
    
    lcdString("TechFarm ");
    
    for (;;) {
        timerReset(5000);
        switch (Slot) {
            case 0:;
                leituraTeclas();
                ssdUpdate();
                Slot = 1;
                break;
            case 1:
                serial();
                Slot = 2;
                break;
            case 2:
                qtdComidaLeitura();
                Slot = 3;
                break;
            case 3:
                comparacao();
                ssdUpdate();
                Slot = 4;
                break;
            default:
                Slot = 0;
                break;
        }
        timerWait();
    }
}//Main


