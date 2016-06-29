#ifndef TL_I2C_MASTER_H_
#define TL_I2C_MASTER_H_

#include "../tl_common.h"


/**
 * nitialize i2c controller, set the speed
 * @param i2c_bit_rate  the bit rate for I2C bus
 */

void tl_i2c_init (int i2c_bit_rate);

/**
 * write len bytes from buffer pointed buff to slave addressed adr
 * the actual work is done by tl_i2c_nb_write
 * @param id         the i2c slave id
 * @param adr        the i2c slave register address for write
 * @param buff       data buffer
 * @param len        data length required to write, count in byte
 * @return            -1 on NOACK, 1 on success
 */
int tl_i2c_burst_write (u32 id, int adr, u8 * buff, int len);
/**
 * Read len bytes slave addressed adr to buffer pointed buff
 *
 * @param id         the i2c slave id
 * @param adr        the i2c slave register address for read
 * @param buff       data buffer
 * @param len        data length required to read, count in byte
 * @return            -1 on NOACK, 1 on success
 */
int tl_i2c_burst_read (u32 id, int adr, u8 * buff, int len);

/**
 * Read 1 bytes for the specific slave addressed
 * @param id       the i2c slave id
 * @param addr     the i2c slave register address for read
 * @return         the data read from the i2c slave address
 */

u8 tl_i2c_read(u32 id, u32 addr);
/**
 * write 1 bytes data to slave addressed adr
 * the actual work is done by tl_i2c_nb_write
 * @param id         the i2c slave id
 * @param adr        the i2c slave register address for write
 * @param data       data to write
 * @return            -1 on NOACK, 1 on success
 */
int tl_i2c_write (u32 id, int adr, u8 data);
#endif
