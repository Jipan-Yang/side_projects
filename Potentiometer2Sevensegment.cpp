/*
 * Use Potentiometer to control digit show on 7 segment
 */

#include "mbed.h"

BusOut display(p5,p6,p7,p8,p9,p10,p11,p12); // segments a,b,c,d,e,f,g,dp
AnalogIn Ain(p20);    //input from Potentiometer

float ADCdata; 
Serial pc(USBTX,USBRX);
//0, 1, 2, 3, 4, 5, 6, 7, 8, 9, .
char ssd[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80};

int main() {
	pc.baud(115200);
	pc.printf("ADC Data Values... \n\r"); 

	while (1) { 
		ADCdata=Ain;     //read 
		pc.printf("%f \n\r",ADCdata); 
		ADCdata = ADCdata>1?1: ADCdata;
		display = ssd[(int)(ADCdata*10)]; //write
		wait (1); 
	} 
}
