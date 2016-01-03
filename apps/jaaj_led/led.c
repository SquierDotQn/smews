/*
<generator>
	<handlers doGet="allumeLed"/>
	<args>
		<arg name="noled" type="uint8" />
	</args>
</generator>
*/

#include <rflpc17xx/rflpc17xx.h>
#define RLFPC_LED_1 (1<<RFLPC_LED_1_PIN)
#define RLFPC_LED_2 (1<<RFLPC_LED_2_PIN)
#define RLFPC_LED_3 (1<<RFLPC_LED_3_PIN)
#define RLFPC_LED_4 (1<<RFLPC_LED_4_PIN)

int etat_1=0;
int etat_2=0;
int etat_3=0;
int etat_4=0;

static char allumeLed(struct args_t *args){
	out_str("toto");
	rflpc_led_init();
	switch(args->noled){
		case 1:
			if(etat_1==0){
				rflpc_led_set(RFLPC_LED_1);
			}else{
				rflpc_led_clr(RFLPC_LED_1);
			}
		break;
		case 2:
			if(etat_2==0){
				rflpc_led_set(RFLPC_LED_2);
			}else{
				rflpc_led_clr(RFLPC_LED_2);
			}
		break;
		case 3:
			if(etat_3==0){
				rflpc_led_set(RFLPC_LED_3);
			}else{
				rflpc_led_clr(RFLPC_LED_3);
			}
		break;
		case 4:
			if(etat_4==0){
				rflpc_led_set(RFLPC_LED_4);
			}else{
				rflpc_led_clr(RFLPC_LED_4);
			}
		break;
		default:
			break;
	}
}
