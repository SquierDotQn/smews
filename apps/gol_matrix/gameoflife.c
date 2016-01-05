#include "gameoflife.h"
#include "led.h"
#include <stdlib.h>
 
#define CELL(I,J) (field[size*(I)+(J)])
#define ALIVE(I,J) t[size*(I)+(J)] = 1
#define DEAD(I,J)  t[size*(I)+(J)] = 0
char field[FIELD_SIZE * FIELD_SIZE];
char temp_field[FIELD_SIZE*FIELD_SIZE];
int potard = FAKE;

void delay(int time){
   int c = 1, d = 1;
 
   for ( c = 1 ; c <= time ; c++ )
       for ( d = 1 ; d <= 960000 ; d++ )
       {__asm("nop");}
}


/* set the cell i,j as alive */
#define SCELL(I,J) field[FIELD_SIZE*(I)+(J)] = 1
int count_alive(const char *field, int i, int j, int size){
   int x, y, a=0;
   for(x=i-1; x <= (i+1) ; x++){
      for(y=j-1; y <= (j+1) ; y++){
         if ( (x==i) && (y==j) ) continue;
         if ( (y<size) && (x<size) && (x>=0) && (y>=0) ){
              a += CELL(x,y);
         }
      }
   }
   return a;
}
 
void evolve(const char *field, char *t, int size){
   int i, j, alive, cs;
   for(i=0; i < size; i++){
      for(j=0; j < size; j++){
         alive = count_alive(field, i, j, size);
         cs = CELL(i,j);
         if ( cs ){
            if ( (alive > 3) || ( alive < 2 ) )
                DEAD(i,j);
            else
                ALIVE(i,j);
         } else {
            if ( alive == 3 )
                ALIVE(i,j);
            else
                DEAD(i,j);
         }
      }
   }
   
}

void game(int nb_gens, char in_field[]){
	char t_field[] = {
 	     0,1,0,0,0,0,0,0,
 	     0,0,1,0,0,0,0,0,
 	     1,1,1,0,0,0,0,0,
 	     0,0,0,0,0,0,0,0,
 	     0,0,0,0,0,0,0,0,
 	     0,0,0,0,0,0,0,0,
 	     0,0,0,0,0,0,0,0,
 	     0,0,0,0,0,0,0,0,
	};
	int i;
	int garde = 0;
	int double_garde = 0;
	char old_field[FIELD_SIZE*FIELD_SIZE];
	for(i=0; i<FIELD_SIZE*FIELD_SIZE; i++){	
		//field[i] = in_field[i];
		// Exemple de test :
		field[i] = t_field[i];
	}
	for(i=0; i<nb_gens; i++){
		clear_led();
		unie();
		delay(potard); // TODO : vraie gestion du potard
		evolve(field, temp_field, FIELD_SIZE);
		garde = 0;
		double_garde = 0;
		for(i=0; i<FIELD_SIZE*FIELD_SIZE; i++){
			if(field[i]!=temp_field[i]){
				garde++;
			}
			old_field[i] = field[i];
			field[i] = temp_field[i];
			if(field[i]!=old_field[i] && i>0){
				double_garde++;
			}	
		}
		if(garde == 0 || double_garde == 0){
			break;
		}
	}
}
