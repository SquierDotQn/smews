#include "led.h"

void defilement_led(uint16_t coul) {
	int i;
	for(i=0;i<NLED;i++) {
		SPI_WRITE(coul);
		RFLPC_DELAY(2000000);
	}
}

void clear_led(){
	int i;
	for(i = 0; i<NLED;i++) {
        SPI_WRITE(CLEAR);        
    }
}

void allerRetour(uint16_t coulBack, uint16_t coulFront, int nbTour){
	  
	int j =0 ;
	int  cpt = 0 ;
	char dir = '+' ;
	  
	int i ;
	while(cpt < nbTour) {	
		for(i = 0 ; i < j ; i++){
			SPI_WRITE(coulBack) ;
		  }
		  
		SPI_WRITE(coulFront) ;
		  
		for(i = j ; i < NLED ; i++){
			SPI_WRITE(coulBack) ;
		}
		  
		RFLPC_DELAY(100000);
		  
		if( dir == '+')
			j++; 
		  
		if( dir == '-')
			j-- ;
		    
	  	if( j == NLED +1 )
			dir = '-' ;
		  
		if( j == -1){
			dir = '+' ;
			cpt ++ ;
		}
	}  
}
  
void empiler(uint16_t coulBack, uint16_t coulFront, int nbTour){
	  
	  int j = 0 ;
	  int k = 0 ;
	  int i, cpt ;
	
	for(cpt = 0 ; cpt < nbTour ; cpt++){
		while(k < NLED){	  
			  for(i = 0 ; i < j ; i++){
				SPI_WRITE(coulBack) ;
			  }
			  
			 SPI_WRITE(coulFront) ;
			  
			  for(i = j ; i < NLED -k ; i++){
				SPI_WRITE(coulBack) ;
			  }
			  
			   for(i = 0 ; i < k ; i++){
				SPI_WRITE(coulFront) ;
			  }
			  
			  RFLPC_DELAY(100000);
			  
			  j++; 
			
			  if( j == NLED-k ){
				j = 0 ;
				    k ++ ;
			  }
		}
		k = 0 ;
	}
}
  
void unie (uint16_t coul){
	int i ;
	for(i = 0 ; i < NLED ; i++) {
		SPI_WRITE(coul) ;
	}
}

uint16_t get_coul(int i){
	uint16_t coul;
	switch(i){
		case 0 :
			coul = CLEAR;
			break;
		case 1 :
			coul = BLEU;
			break;
		case 2 :
			coul = ROUGE;
			break;
		case 3 :
			coul = VERT;
			break;
		default :
			coul = CLEAR;
			break;
	}
	return coul;
}
