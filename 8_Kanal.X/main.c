
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include"pic16f877a.h"
#include<stdio.h>
#define _XTAL_FREQ 4000000
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
#include<LCD.h>

unsigned char zaman,zaman_sayac,animasyon,timer_giris;
unsigned char adres=0x00;
unsigned char adres2=0x10;
int dizi[20];
int zaman_goster;

void __interrupt() RB_Interrupt(){
     GIE=0;
    
    if(INTCONbits.T0IF){    // Her 25 ms de bir girer 
        timer_giris++;
        if(timer_giris == 4) { //25*4 ms =100 ms de bir islem yapar;
        zaman_sayac++;
        timer_giris=0;
        }  // 100 ms de bir
        RD0^=RD0;
        if(zaman_sayac==zaman)
        {
            if (animasyon==1) PORTC=0xFF; // BUTON DEGERINE GORE ANIMASYON SECIMI
            else if (animasyon==2) PORTC=0x2A;
            else if (animasyon==3) PORTC=0x3B;
            else if (animasyon==4) PORTC=0x4D;
            else if (animasyon==5) PORTC=0x5C;
           zaman_sayac=0; 
        }
         else
             PORTC=0;

        T0IF=0;
        TMR0=160;
    
        GIE=1; 

    }
}
void TMR0_Init(){ // TIMER 0 AYARLARI(Her 25 ms de bir kesme olu?turur)
 GIE = 1; PEIE= 1; T0IE = 1;
 T0CS=0; PSA=0; T0IF=0;
 PS2=1; PS1=1; PS0=1;
 }

void main(void){
TMR0_Init();
 TMR0=160;
TRISBbits.TRISB4=1;TRISBbits.TRISB5=1;TRISBbits.TRISB6=1; // PORT GIRIS CIKIS AYARLARI
TRISC=0x00; PORTC=0;
TRISD=0x00; PORTD=0;
Lcd_Init();
zaman_sayac=0;
zaman=1;
animasyon=1;

while(1){
    
    animasyon=eeprom_read(adres);
    if(RB4){ //  ANIMASYON DEG?ST?R
        animasyon++;
        if(animasyon>5)animasyon=1;
        __delay_ms(500);
        eeprom_write(adres,animasyon); //Son animasyon degerini eeproma kaydetme.
    }
    zaman=eeprom_read(adres2);
    if(RB5){ //  ANIMASYON ZAMAN + HIZ-
        zaman++;
        if(zaman>10) zaman=10;
         __delay_ms(500);
    }
    if(RB6){ //  ANIMASYON ZAMAN - HIZ+
         zaman--;
        if(zaman<1) zaman=1;
         __delay_ms(500);
    }
         eeprom_write(adres2,zaman);     //Son zaman degerini eeproma kaydetme

         // ZAMAN VE ANIMASYON KONTROL ICIN LCD EKRAN
    zaman_goster = zaman*100;
    Lcd_Set_Cursor(1,1);
     sprintf (dizi,"zaman = %.2u ms ",zaman_goster);
     Lcd_Write_String(dizi);

    Lcd_Set_Cursor(2,1);
     sprintf (dizi,"animasyon = %u  ",animasyon);
     Lcd_Write_String(dizi);

}
}


