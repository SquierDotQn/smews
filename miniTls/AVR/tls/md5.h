/*
 * md5.h
 *
 *  Created on: Jul 30, 2009
 *      Author: alex
 */

#ifndef MD5_H_
#define MD5_H_

#include "target.h"

#define MD5_SIZE 16

struct md5_context {

	uint32_t state[4];
	uint8_t buffer[64];
	uint64_t count;

}; /* 88 bytes */

/* macro needed for both MD5 & SHA1 */
#define ROL(bits, value) (((value) << (bits)) | ((value) >> (32 - (bits))))


void md5_init(struct md5_context*);
void md5_digest(struct md5_context*);
void md5_update(struct md5_context*,const uint8_t*,uint16_t);
void md5_clone(struct md5_context*, struct md5_context*);




#endif /* MD5_H_ */
