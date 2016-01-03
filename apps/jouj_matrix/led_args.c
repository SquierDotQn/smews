/*
<generator>
        <handlers doGet="launch_game"/>
</generator>
*/

#include "led.h"
#include "gameoflife.h"

static char launch_game(struct args_t *args) {
    	INIT_WAIT;
    	SPI_INIT;
	
	//On clear les LED
	clear_led();
		
	//uint16_t coul = get_coul((int) args->couleur1);
	game();
	return 0;
}
