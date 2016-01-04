/*
<generator>
        <handlers doGet="launch_game"/>
	   <args>
            <arg name="nbgens" type="uint8" />
            <arg name="field" type="str" size="64" />
	   </args>
</generator>
*/

#include "led.h"
#include "gameoflife.h"

static char launch_game(struct args_t *args) {
    INIT_WAIT;
    SPI_INIT;

    //On clear les LED
    clear_led();
    game(args->nbgens, args->field);

	return 0;
}
