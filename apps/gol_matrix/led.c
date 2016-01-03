#include "led.h"
#include "gameoflife.h"
extern char field[];
void clear_led(){
	int i;
	for(i = 0; i<NLED;i++) {
        SPI_WRITE(CLEAR);        
    }
}

void unie (void){
	int i ;
	for(i = 0 ; i < NLED ; i++) {
		if(field[i])
			SPI_WRITE(BLEU) ;
		else
			SPI_WRITE(CLEAR) ;
	}
}
