/*
* Copyright or © or Copr. 2011, Thomas Vantroys, Michael Hauspie
*
* Author e-mail: firstname DOT lastname AT univ-lille1 DOT fr
*
* This software is a computer program whose purpose is to design an
* efficient Web server for very-constrained embedded system.
*
* This software is governed by the CeCILL license under French law and
* abiding by the rules of distribution of free software.  You can  use,
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info".
*
* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability.
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or
* data to be ensured and,  more generally, to use and operate it in the
* same conditions as regards security.
*
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/
#include "timers.h"
#include "led_common.h"

#define INIT_SERIAL do { rflpc_uart_init_ex(RFLPC_UART2, 52, 0, 1, 2); } while(0) /* 9600 bauds configuration */
#define SEND_SERIAL(c) rflpc_uart_putchar(RFLPC_UART2, ((c)))

#define BUFFER_SIZE		60
#define DISPLAY_WIDTH	10
#define SLIP_END 0xC0

// Pour afficher sur la matrice :

// . 1 1 1 1 1 1 1 
// . 1 . . . . . 1
// . 1 . . . . . 1
// . . 1 . . . 1 .
// . . . 1 1 1 . .
// D
// Prendre un tableau, ex :
// 1 2 3 4 5 6 7 8
// 1 2 3 4 5 6 7 8 
// 1 2 3 4 5 6 7 8 
// 1 2 3 4 5 6 7 8 
// 1 2 3 4 5 6 7 8 
// 1 2 3 4 5 6 7 8 
// 1 2 3 4 5 6 7 8 
// 1 2 3 4 5 6 7 8 
// devient
// 1 1 1 1 1 1 1 1
// 2 2 2 2 2 2 2 2
// 3 3 3 3 3 3 3 3 
// 4 4 4 4 4 4 4 4
// 5 5 5 5 5 5 5 5 
// 6 6 6 6 6 6 6 6
// 7 7 7 7 7 7 7 7 
// 8 8 8 8 8 8 8 8

void shift_tab(const char* tab, const char* tab_result, uint8_t size){
	int x, y;
	for(x = 0; x < size; x++){
		for(y = 0; y < size; y++){
			tab[x][y] = tab_result[y][x];
		} 
	}
}
 

static CONST_VAR(uint8_t ,font[]) = {
   0x00,0x00,0x00,0x00,0x00,//"(0x00)"
   0x00,0x00,0x00,0x00,0x00,//"(0x01)"
   0x00,0x00,0x00,0x00,0x00,//"(0x02)"
   0x00,0x00,0x00,0x00,0x00,//"(0x03)"
   0x00,0x00,0x00,0x00,0x00,//"(0x04)"
   0x00,0x00,0x00,0x00,0x00,//"(0x05)"
   0x00,0x00,0x00,0x00,0x00,//"(0x06)"
   0x00,0x00,0x00,0x00,0x00,//"(0x07)"
   0x00,0x00,0x00,0x00,0x00,//"(0x08)"
   0x00,0x00,0x00,0x00,0x00,//"(0x09)"
   0x00,0x00,0x00,0x00,0x00,//"(0x0A)"
   0x00,0x00,0x00,0x00,0x00,//"(0x0B)"
   0x00,0x00,0x00,0x00,0x00,//"(0x0C)"
   0x00,0x00,0x00,0x00,0x00,//"(0x0D)"
   0x00,0x00,0x00,0x00,0x00,//"(0x0E)"
   0x00,0x00,0x00,0x00,0x00,//"(0x0F)"
   0xFF,0x00,0x00,0x00,0x00,//"(0x10)"
   0xFF,0xFF,0x00,0x00,0x00,//"(0x11)"
   0xFF,0xFF,0xFF,0x00,0x00,//"0x12"
   0xFF,0xFF,0xFF,0xFF,0x00,//"0x13"
   0xFF,0xFF,0xFF,0xFF,0xFF,//"0x14"
   0x00,0xFF,0xFF,0xFF,0xFF,//"0x15"
   0x00,0x00,0xFF,0xFF,0xFF,//"0x16"
   0x00,0x00,0x00,0xFF,0xFF,//"0x17"
   0x00,0x00,0x00,0x00,0xFF,//"0x18"
   0x03,0x07,0x7E,0x20,0x1C,//"0x19"
   0x60,0x66,0x09,0x09,0x09,//"0x1A"
   0x60,0x6F,0x0A,0x0A,0x08,//"0x1B"
   0x10,0x18,0x1C,0x18,0x10,//"0x1C"
   0x00,0x3E,0x1C,0x08,0x00,//"0x1D"
   0x00,0x08,0x1C,0x3E,0x00,//"0x1E"
   0x04,0x0C,0x1C,0x0C,0x04,//"0x1F"
   0x00,0x00,0x00,0x00,0x00,//"0x20"
   0x00,0x00,0x79,0x00,0x00,//"0x21"
   0x00,0x70,0x00,0x70,0x00,//"«  (0x22)"
   0x14,0x7F,0x14,0x7F,0x14,//"#,0x(0x23)"
   0x12,0x2A,0x7F,0x2A,0x24,//"$,0x(0x24)"
   0x62,0x64,0x08,0x13,0x23,//"%,0x(0x25)"
   0x36,0x49,0x55,0x22,0x05,//"&,0x(0x26)"
   0x00,0x50,0x60,0x00,0x00,//"',0x(0x27)"
   0x00,0x1C,0x22,0x41,0x00,//"(,0x(0x28)"
   0x00,0x41,0x22,0x1C,0x00,//"),0x(0x29)"
   0x14,0x08,0x3E,0x08,0x14,//"*,0x(0x2A)"
   0x08,0x08,0x3E,0x08,0x08,//"+,0x(0x2B)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x2C)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x2D)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x2E)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x2F)"
   0x3E,0x45,0x49,0x51,0x3E,//"0,0x(0x30)"
   0x00,0x21,0x7F,0x01,0x00,//"1,0x(0x31)"
   0x21,0x43,0x45,0x49,0x31,//"2,0x(0x32)"
   0x42,0x41,0x51,0x69,0x46,//"3,0x(0x33)"
   0x0C,0x14,0x24,0x7F,0x04,//"4,0x(0x34)"
   0x72,0x51,0x51,0x51,0x4E,//"5,0x(0x35)"
   0x1E,0x29,0x49,0x49,0x06,//"6,0x(0x36)"
   0x40,0x47,0x48,0x50,0x60,//"7,0x(0x37)"
   0x36,0x49,0x49,0x49,0x36,//"8,0x(0x38)"
   0x30,0x49,0x49,0x4A,0x3C,//"9,0x(0x39)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x3A)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x3B)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x3C)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x3D)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x3E)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x3F)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x40)"
   0x3F,0x44,0x44,0x44,0x3F,//"A,0x(0x41)"
// . . 0 0 0 0 0 0 
// . 0 . . . 0 . .
// . 0 . . . 0 . .
// . 0 . . . 0 . .
// . . 0 0 0 0 0 0
   0x7F,0x49,0x49,0x49,0x36,//"B,0x(0x42)"
   0x3E,0x41,0x41,0x41,0x22,//"C,0x(0x43)"
   0x7F,0x41,0x41,0x22,0x1C,//"D,0x(0x44)"
// . 1 1 1 1 1 1 1 
// . 1 . . . . . 1
// . 1 . . . . . 1
// . . 1 . . . 1 .
// . . . 1 1 1 . .
   0x7F,0x49,0x49,0x49,0x41,//"E,0x(0x45)"
   0x7F,0x48,0x48,0x48,0x40,//"F,0x(0x46)"
   0x3E,0x41,0x49,0x49,0x2E,//"G,0x(0x47)"
   0x7F,0x08,0x08,0x08,0x7F,//"H,0x(0x48)"
   0x00,0x41,0x7F,0x41,0x00,//"I,0x(0x49)"
   0x02,0x01,0x41,0x7E,0x40,//"J,0x(0x4A)"
   0x7F,0x08,0x14,0x22,0x41,//"K,0x(0x4B)"
   0x7F,0x01,0x01,0x01,0x01,//"L,0x(0x4C)"
   0x7F,0x20,0x18,0x20,0x7F,//"M,0x(0x4D)"
   0x7F,0x10,0x08,0x04,0x7F,//"N,0x(0x4E)"
   0x3E,0x41,0x41,0x21,0x3E,//"O,0x(0x4F)"
   0x7F,0x48,0x48,0x48,0x30,//"P,0x(0x50)"
   0x3E,0x41,0x45,0x42,0x3D,//"Q,0x(0x51)"
   0x7F,0x48,0x4C,0x4A,0x31,//"R,0x(0x52)"
   0x31,0x49,0x49,0x49,0x46,//"S,0x(0x53)"
   0x40,0x40,0x7F,0x40,0x40,//"T,0x(0x54)"
   0x7E,0x01,0x01,0x01,0x7E,//"U,0x(0x55)"
   0x7C,0x02,0x01,0x02,0x7C,//"V,0x(0x56)"
   0x7E,0x01,0x0E,0x01,0x7E,//"W,0x(0x57)"
   0x63,0x14,0x08,0x14,0x63,//"X,0x(0x58)"
   0x70,0x08,0x07,0x08,0x70,//"Y,0x(0x59)"
   0x43,0x45,0x49,0x51,0x61,//"Z,0x(0x5A)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x5B)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x5C)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x5D)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x5E)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x5F)"
   0x00,0x00,0x00,0x00,0x00,//",0x(0x60)"
   0x02,0x15,0x15,0x15,0x0F,//"a,0x(0x61)"0
   0x7F,0x05,0x09,0x09,0x06,//"b,0x(0x62)"
   0x0E,0x11,0x11,0x11,0x02,//"c,0x(0x63)"
   0x06,0x09,0x09,0x05,0x7F,//"d,0x(0x64)"
   0x0E,0x15,0x15,0x15,0x0C,//"e,0x(0x65)"
   0x08,0x3F,0x48,0x40,0x20,//"f,0x(0x66)"
   0x18,0x25,0x25,0x25,0x3E,//"g,0x(0x67)"
   0x7F,0x08,0x10,0x10,0x0F,//"h,0x(0x68)"
   0x00,0x00,0x2F,0x00,0x00,//"i,0x(0x69)"
   0x02,0x01,0x11,0x5E,0x00,//"j,0x(0x6A)"
   0x7F,0x04,0x0A,0x11,0x00,//"k,0x(0x6B)"
   0x00,0x41,0x7F,0x01,0x00,//"l,0x(0x6C)"
   0x1F,0x10,0x0C,0x10,0x0F,//"m,0x(0x6D)"
   0x1F,0x08,0x10,0x10,0x0F,//"n,0x(0x6E)"
   0x0E,0x11,0x11,0x11,0x0E,//"o,0x(0x6F)"
   0x1F,0x14,0x14,0x14,0x08,//"p,0x(0x70)"
   0x08,0x14,0x14,0x0C,0x1F,//"q,0x(0x71)"
   0x1F,0x08,0x10,0x10,0x08,//"r,0x(0x72)"
   0x09,0x15,0x15,0x15,0x02,//"s,0x(0x73)"
   0x10,0x7E,0x11,0x01,0x02,//"t,0x(0x74)"
   0x1E,0x01,0x01,0x02,0x1F,//"u,0x(0x75)"
   0x1C,0x02,0x01,0x02,0x1C,//"v,0x(0x76)"
   0x1E,0x01,0x06,0x01,0x1E,//"w,0x(0X77)"
   0x11,0x0A,0x04,0x0A,0x11,//"x,0x(0x78)"
   0x18,0x05,0x05,0x05,0x1E,//"y,0x(0x79)"
   0x11,0x13,0x15,0x19,0x11,//"z,0x(0x7A)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0x00,0x00,0x00,0x00,0x00,//"empty,0x(0xfe)"
   0xFF,0xFF,0xFF,0xFF,0xFF //"full,0x(0xff)"
};

void init_buffer(unsigned char *buffer, int size)
{
   int i;
   for (i=0; i<size; i++)
      buffer[i] = 0;
}

/* convert msg to msg_font */
void convert_msg(const char *msg, unsigned char* msg_font, int msg_size)
{
   int i,j;

   for(i=0; i<msg_size; i++)
   {
      for(j=0; j<5; j++)
         msg_font[6*i+j] = font[msg[i]*5+j];
      msg_font[6*i+6] = 0x00;
   }
}

void print_msg(unsigned char *msg, int msg_size)
{
   int i;

   for(i=0; i<msg_size; i++)
   {
      printf("%x ", msg[i]);
   }
}

static int my_strlen(const char *msg)
{
   int i = 0;
   while (*msg++)
      i++;
   return i;
}

#define MSG_FONT_SIZE (512*6)
/* Global vars for current text properties */
typedef struct
{
   int size;
   int current_pos;
   unsigned char msg_affichage[DISPLAY_WIDTH+2]; 
   unsigned char msg_font[MSG_FONT_SIZE];
   unsigned char buffer[DISPLAY_WIDTH]; // the display matrix
} matrix_t;

static matrix_t matrix;

static void write_serial(const unsigned char *msg, int size)
{
   int i;
   for (i = 0 ; i < size ; ++i)
   {
#ifdef DEBUG_SERIAL
      printf("%02x ", msg[i]);
#endif
      SEND_SERIAL(msg[i]);
   }
#ifdef DEBUG_SERIAL
   printf("\r\n");
#endif
}

/* Do a display step of the matrix 
   This function is intended to be called at regular intervals to
   generate the text scroll
   matrix_display must have been called first 
*/
static void display_step(void)
{
   int j;

   /* fill the matrix buffer by scrolling the buffer and filling it
    * with next text */
   for(j=0; j<DISPLAY_WIDTH-1; j++)
   {
      matrix.buffer[j] = matrix.buffer[j+1];
   }
   matrix.buffer[DISPLAY_WIDTH-1] = matrix.msg_font[matrix.current_pos];
   
   /* create and send the command to bangles */
   for(j=0; j<DISPLAY_WIDTH; j++)
   {
/*      printf("%x ", matrix.buffer[j]);*/
      matrix.msg_affichage[j+1] = matrix.buffer[j];
   }
   write_serial(matrix.msg_affichage, DISPLAY_WIDTH+2);
   /* Next position */
   matrix.current_pos = (matrix.current_pos + 1) % (6*matrix.size);
}
void matrix_display(const char *msg)
{
   matrix.size = my_strlen(msg);
   matrix.current_pos = 0;
   init_buffer(matrix.buffer, DISPLAY_WIDTH);
   init_buffer(matrix.msg_affichage, DISPLAY_WIDTH+2);
   matrix.msg_affichage[0] = 'd';
   matrix.msg_affichage[DISPLAY_WIDTH+1] = SLIP_END;

   /* convert the message to hexa */
   if (matrix.size >= MSG_FONT_SIZE)
      matrix.size = MSG_FONT_SIZE - 1;
   convert_msg(msg, matrix.msg_font, matrix.size);
   printf("Sending %s\r\n", msg);
   /* print the msg for debug */
/*   print_msg(matrix.msg_font, matrix.size*6);*/
}

void init_serial_matrix(void)
{
   INIT_SERIAL;
   matrix_display(DEFAULT_TEXT);
   set_timer(&display_step, 500);
}
