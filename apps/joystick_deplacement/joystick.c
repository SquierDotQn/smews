/*
<generator>
	<handlers init="init" initGet="initGet" doGet="doGet"/>
	<properties persitence="volatile" channel="joystickChannel"/>
	<args>
		<arg name="joystick" type="uint16" />
		<arg name="pot" type="uint16" />
	</args>
</generator>
*/

#include <stdio.h>

#include "generators.h"
#include "timers.h"
#include "channels.h"

static uint16_t glob_load;
static int oldjoystick;
static int joystick = 0;

static void timer() {
	// BAS
	if(rflpc_gpio_get_pin(MBED_DIP12)){
		joystick = 2;
	}
	// GAUCHE
	if(rflpc_gpio_get_pin(MBED_DIP13)){
		joystick = 4;
	}
	// MILIEU
	if(rflpc_gpio_get_pin(MBED_DIP14)){
		joystick = 5;
	}
	// HAUT
	if(rflpc_gpio_get_pin(MBED_DIP15)){
		joystick = 8;
	}
	// BAS
	if(rflpc_gpio_get_pin(MBED_DIP16)){
		joystick = 6;
	}
	if(joystick != oldjoystick){
		glob_load = (uint16_t)joystick;
		server_push(&joystickChannel);
	}
	if(pot != oldpot){
		server_push
	}
}

static char init(void) {
	return set_timer(&timer,500);
}

static char initGet(struct args_t *args) {
	if(args)
		oldjoystick = args->joystick;
		oldpot      = args->pot;
	return 1;
}

static char doGet(struct args_t *args) {
	out_uint(glob_load);
	return 1;
}
