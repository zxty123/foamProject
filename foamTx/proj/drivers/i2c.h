
typedef struct{
	u8 dev_id;//device id
	u8 i2c_port;//select i2c 0  1  2  3
	u8 mode;//i2c mode  0  map mode 1 dma mode
	u8 irq;//i2c slave mode irq, 1   enable ,  0 disable
	void (*i2c_irq_handler)();
}I2C_info_t;

typedef enum {
	I2C_SLAVE_DMA = 0,
	I2C_SLAVE_MAP,
}I2C_SlaveMode;

typedef enum {
	I2C_PORT0 = 0,  //take GP4/GP5 as SCL/SDA
	I2C_PORT1,      //take GP12/GP13 as SCL/SDA
	I2C_PORT2,      //take GP22/GP23 as SCL/SDA
	I2C_PORT3,      //take GP30/GP31 as SCL/SDA
}I2C_PortTypeDef;

typedef enum {
	I2C_IRQ_DISABLE = 0,
	I2C_IRQ_ENABLE,
}I2C_IrqStatus;

typedef enum {
	I2C_IRQ_NONE = 0,
	I2C_IRQ_HOST_PACKET,
	I2C_IRQ_HOST_READ,
	I2C_IRQ_HOST_WRITE,
}I2C_IrqSrc;

I2C_info_t i2c_info;
void i2c_master_init(void);
u8 i2c_dma_read(u8 id,int address,u8 addr_mod);
void i2c_dma_write(u8 id,int address, int data,u8 addr_mod);
void i2c_map_write(u8 id,u8 *data,u8 lengh);
void i2c_slave_8368_init(I2C_info_t i2c,u8 * buf);
void I2C_SlaveInit(I2C_info_t i2c_set,u8 * pBuf);
