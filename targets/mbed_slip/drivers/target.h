/*
* Copyright or © or Copr. 2008, Simon Duquennoy
* 
* Author e-mail: simon.duquennoy@lifl.fr
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

#ifndef __TARGET_H__
#define __TARGET_H__

#include <stdint.h>
#include <string.h>
#include <signal.h>
#include "exports.h"

/* Drivers interface */

#define HARDWARE_INIT hardware_init()
#define HARDWARE_STOP hardware_stop()
#define TIME_MILLIS global_timer
#define DEV_GET(c) {(c) = dev_get();}
#define DEV_PUT(c) dev_put(c)
#define DEV_PREPARE_OUTPUT(length) dev_prepare_output()
#define DEV_OUTPUT_DONE dev_output_done()
#define DEV_DATA_TO_READ dev_data_to_read()

/* Smews states */

#define SMEWS_WAITING { smews_waiting(); }
#define SMEWS_RECEIVING { smews_receiving(); }
#define SMEWS_SENDING { smews_sending(); }
#define SMEWS_ENDING { smews_ending(); }

/* Const and persistent access macros */

#define CONST_VOID_P_VAR const void *
#define CONST_VAR(type,name) type const name
#define PERSISTENT_VAR(type,name) type name
#define CONST_WRITE_NBYTES(dst,src,len) strncpy(dst,src,len)

#define CONST_READ_UI8(x) ((uint8_t)*((uint8_t*)(x)))
#define CONST_READ_UI16(x) ((uint16_t)*((uint16_t*)(x)))
#define CONST_READ_UI32(x) ((uint32_t)*((uint32_t*)(x)))
#define CONST_READ_ADDR(x) ((void*)*((void**)(x)))

#define CONST_UI8(x) CONST_READ_UI8(&(x))
#define CONST_UI16(x) CONST_READ_UI16(&(x))
#define CONST_UI32(x) CONST_READ_UI32(&(x))
#define CONST_ADDR(x) CONST_READ_ADDR(&(x))

/* Endianness */

#define ENDIANNESS LITTLE_ENDIAN

/* Context switching */

#define BACKUP_CTX(sp) asm("mov %0, sp" : "=r"((sp)[0]));
#define RESTORE_CTX(sp) asm("mov sp, %0" :: "r"((sp)[0]));
#define PUSHREGS asm("push {r4-r11, lr}");
#define POPREGS asm("pop {r4-r11, lr}");

/* Smews configuration */

#define OUTPUT_BUFFER_SIZE 256
#define ALLOC_SIZE 28672
#define STACK_SIZE 128

/* Custom Functions */

#endif /* __TARGET_H__ */