/*
<generator>
        <handlers doGet="do_patern_led"/>
        <args>
     		<arg name="couleur1" type="uint8" />
     		<arg name="couleur2" type="uint8" />
     		<arg name="patern" type="uint8" />
     		<arg name="tour" type="uint8" />
    	</args>
</generator>
*/

#include "led.h"
 
static char do_patern_led(struct args_t *args) {
	
	if(args) {		
	   /*
	 	* Patern 0 -> clear
	 	* Patern 1 -> unie
	 	* Patern 2 -> defilement
	 	* Patern 3 -> empiler
	 	* Patern 4 -> aller_retour
	 	* Couleur 0 -> Clear
	 	* Couleur 1 -> Bleu
	 	* Couleur 2 -> Rouge
	 	* Couleur 3 -> Vert 
	 	*/    
    	INIT_WAIT;
    	SPI_INIT;
	
		//On clear les LED
		clear_led();
		
		uint16_t coul = get_coul((int) args->couleur1);
		uint16_t coul2 = get_coul((int) args->couleur2);
		int patern = (int) args->patern;
		int tour = (int) args->tour;
		
		switch(patern){
			case 0 :
				clear_led();
				break;
			case 1 :
				unie(coul);
				break;
			case 2 :
				defilement_led(coul);
				//On clear les LED
				clear_led();
				break;
			case 3 :
				empiler(coul, coul2, tour);
				//On clear les LED
				clear_led();
				break;
			case 4 :
				allerRetour(coul, coul2, tour);
				break;
				//On clear les LED
				clear_led();
			default :
				break;			
		}	
	}
	return 0;
}
