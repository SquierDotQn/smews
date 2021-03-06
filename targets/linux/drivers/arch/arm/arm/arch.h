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

#ifndef __ARCH_H__
#define __ARCH_H__


/* Architecture dependent macro for arm architecture */

#ifdef __arm__


/* save the process stack pointer in sp[0]  and frame pointer in sp[1] */
/* By convention, r11 is used as frame pointer in arm mode */
#define BACKUP_CTX(sp) do {asm ("mov %0, sp" : "=r"((sp)[0])); asm("mov %0, r11" : "=r"((sp)[1])); }while(0)
/* restore the process stack pointer from sp[0] */
#define RESTORE_CTX(sp) do {asm ("mov sp, %0" :: "r"((sp)[0])); asm("mov r11, %0" :: "r"((sp)[1])); }while(0)
/* push all registers that must not be modified by any function call */
#define PUSHREGS do { asm("push {r4-r11, lr}"); } while(0)
/* pop all registers that must not be modified by any function call */
#define POPREGS do { asm("pop {r4-r11, lr}"); } while (0)

#else
#error "This file is for arm architecture"
#endif


#endif /* __ARCH_H__ */
