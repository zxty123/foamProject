
#include "tl_i2c_master.h"
#include "../mcu/clock.h"

#define		TL_I2C_ADDRESS_16BIT		0


//////////////////////////////////////////////////////////////////////
///// tl_i2c_nb: non-blocking access
//////////////////////////////////////////////////////////////////////

static inline int tl_i2c_busy ()
{
	return (reg_i2c_status & FLD_I2C_CMD_BUSY);
}

static inline void tl_i2c_nb_write_adr16_dat (int adr, int dat)
{
	reg_i2c_dat_ctrl = (adr>>8) | ((adr&0xff)<<8) | (dat << 16) |
			((FLD_I2C_CMD_START | FLD_I2C_CMD_STOP | FLD_I2C_CMD_ID |
				FLD_I2C_CMD_ADR | FLD_I2C_CMD_DO | FLD_I2C_CMD_DI) << 24);
}

static inline void tl_i2c_nb_write_adr8_dat (int adr, int dat)
{
	reg_i2c_dat_ctrl = adr | (dat << 16) |
			((FLD_I2C_CMD_START | FLD_I2C_CMD_STOP |
				FLD_I2C_CMD_ID | FLD_I2C_CMD_ADR | FLD_I2C_CMD_DI) << 24);
}

static inline void tl_i2c_nb_write_start8 (int adr, int stop) {
	reg_i2c_dat_ctrl = adr |
			((FLD_I2C_CMD_START | (stop ? FLD_I2C_CMD_STOP : 0) |
				FLD_I2C_CMD_ID | FLD_I2C_CMD_ADR ) << 24);
}

static inline void tl_i2c_nb_write_start16 (int adr, int stop) {
	reg_i2c_dat_ctrl = (adr>>8) | ((adr&0xff)<<8) |
			((FLD_I2C_CMD_START | (stop ? FLD_I2C_CMD_STOP : 0) |
				FLD_I2C_CMD_ID | FLD_I2C_CMD_ADR | FLD_I2C_CMD_DO) << 24);
}

static inline void tl_i2c_nb_write_byte (int dat, int stop) {
	reg_i2c_dat_ctrl = (dat<<8) |
			((FLD_I2C_CMD_DO | (stop ? FLD_I2C_CMD_STOP : 0)) << 24);
}


///////////// for read command ///////////////////////////////////////////////
static inline void tl_i2c_nb_read_byte () {
	reg_i2c_ctrl = FLD_I2C_CMD_START | FLD_I2C_CMD_ID | FLD_I2C_CMD_READ_ID |
			FLD_I2C_CMD_DI | FLD_I2C_CMD_STOP;
}

static inline void tl_i2c_nb_read_start (int stop) {
	reg_i2c_ctrl = FLD_I2C_CMD_START | FLD_I2C_CMD_ID | FLD_I2C_CMD_DI |
			FLD_I2C_CMD_READ_ID | (stop ? FLD_I2C_CMD_STOP | FLD_I2C_CMD_NAK : 0);
}

static inline void tl_i2c_nb_read_next (int stop) {
	reg_i2c_ctrl = FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID |
			(stop ? FLD_I2C_CMD_STOP | FLD_I2C_CMD_NAK : 0);
}



/**
 * nitialize i2c controller, set the speed
 *
 * @param i2c_bit_rate the bit rate for I2C bus
 */

void tl_i2c_init (int i2c_bit_rate)
{
	u32 divider = CLOCK_SYS_CLOCK_HZ/4/i2c_bit_rate;

	reg_i2c_set = divider | (FLD_I2C_MODE_MASTER << 24);
	reg_spi_sp = !FLD_CK_DO_GPIO_MODE;

}

/**
 * write accumulate len bytes from buffer pointed buff to slave addressed adr, non-block
 *
 * @param adr  slave address
 * @param buff data buffer
 * @param len  data length in byte
 * @return  0 on BUSY or success, -1 on NOACK, 1 on done
 */
static int tl_i2c_nb_write (int adr, u8 * buff, int len) {
	static int status = 0;
	// 0: idle
	// 1: start-id-address
	// 2+: data
	if (status && tl_i2c_busy ()) {
		return 0;		// i2c busy
	}

	if (!status) {	// start i2c write command

		if (TL_I2C_ADDRESS_16BIT) {
			tl_i2c_nb_write_start16 (adr, len == 0);
		}
		else{
			tl_i2c_nb_write_start8 (adr, len == 0);
		}
	}
	else {
		int offset = status - 1;
		if (offset >= len) {
			status = 0;
			if (reg_i2c_status & FLD_I2C_NAK) {
				return -1;
			}
			else {
				return 1;
			}
		}
		else {
			tl_i2c_nb_write_byte (buff[offset], status == len);
		}
	}
	status ++;
	return 0;
}

/**
 * read accumulate len bytes from  slave addressed adr to buffer pointed buff
 *
 * @param adr  slave address
 * @param buff data buffer
 * @param len  data length in byte
 * @return   0 on BUSY or success, -1 on NOACK, 1 on done
 */
static int tl_i2c_nb_read (int adr, u8 * buff, int len) {
	static int status = 0;
	// 0: idle
	// 1: start-write_id-address-stop
	// 2: start-read_id
	// 3+: data
	if (status && tl_i2c_busy ()) {
		return 0;		// i2c busy
	}

	if (!status) {	// start i2c write address command
		if (TL_I2C_ADDRESS_16BIT) {
			tl_i2c_nb_write_start16 (adr, FLD_I2C_CMD_STOP);
		}
		else {
			tl_i2c_nb_write_start8 (adr, FLD_I2C_CMD_STOP);
		}
	}
	else if (status == 1) { // start i2c read command
		if (reg_i2c_status & FLD_I2C_NAK || !len) {
			status = 0;
			return -1;
		}
		tl_i2c_nb_read_start (len == 1);
	}
	else {
		buff[status - 2] = reg_i2c_di;
		if (status > len) {
			status = 0;
			return 1;
		}
		else {
			tl_i2c_nb_read_next (status == len);
		}
	}
	status ++;
	return 0;
}

/**
 * write len bytes from buffer pointed buff to slave addressed adr
 * the actual work is done by tl_i2c_nb_write
 *
 * @param id         the i2c slave id
 * @param adr        the i2c slave register address for write
 * @param buff       data buffer
 * @param len        data length required to write, count in byte
 * @return            -1 on NOACK, 1 on success
 */
int tl_i2c_burst_write (u32 id, int adr, u8 * buff, int len)
{
	int ret;
	reg_i2c_id = id;

	do {
		ret = tl_i2c_nb_write (adr, buff, len);
	} while (ret == 0);
	return ret;
}


/**
 * Read len bytes slave addressed adr to buffer pointed buff
 *
 * @param id         the i2c slave id
 * @param adr        the i2c slave register address for read
 * @param buff       data buffer
 * @param len        data length required to read, count in byte
 * @return            -1 on NOACK, 1 on success
 */
int tl_i2c_burst_read (u32 id, int adr, u8 * buff, int len)
{
	int ret;

	reg_i2c_id = id;

	do {
		ret = tl_i2c_nb_read (adr, buff, len);
	} while (ret == 0);
	return ret;
}

/**
 * Read 1 bytes for the specific slave addressed
 *
 * @param id       the i2c slave id
 * @param addr     the i2c slave register address for read
 * @return         the data read from the i2c slave address
 */

u8 tl_i2c_read(u32 id, u32 addr)
{
	u8 tmp;

	tl_i2c_burst_read(id, addr, &tmp, 1);
	return tmp;
}

/**
 * write 1 bytes data to slave addressed adr
 * the actual work is done by tl_i2c_nb_write
 *
 * @param id         the i2c slave id
 * @param adr        the i2c slave register address for write
 * @param data       data to write
 * @return            -1 on NOACK, 1 on success
 */
int tl_i2c_write (u32 id, int adr, u8 data)
{
	return tl_i2c_burst_write (id, adr, &data, 1);
}
