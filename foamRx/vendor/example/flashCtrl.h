/*
 * flashCtrl.h
 *
 *  Created on: 2016-6-22
 *      Author: chzu131
 */

#ifndef FLASHCTRL_H_
#define FLASHCTRL_H_

#include "../../proj/tl_common.h"


u32 flash_writeAll(void);
u32 flash_write(u32 addr, u16 id);
u32 flash_read(u32 addr);
void flash_saveData(void);
void flash_ramRst(void);
void flash_BankCheck(void);
u32 flash_readAll(void);


#endif /* FLASHCTRL_H_ */
