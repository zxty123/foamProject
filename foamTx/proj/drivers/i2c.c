#include "../../proj/tl_common.h"
#include "../../proj/mcu/watchdog_i.h"
#include "i2c.h"

#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL      GPIO_GP4
#endif

#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA      GPIO_GP5
#endif
extern void (*i2c_irq_handler)(void);
#define TELINKID	0x5c
void i2c_long_wait(void){
	u8 delay=100;
	while(delay--);
}
static inline void i2c_sda_out(int v){
	gpio_set_output_en(PIN_I2C_SDA,1);
	gpio_set_input_en(PIN_I2C_SDA,0);
	gpio_write(PIN_I2C_SDA,v);
}
static inline void i2c_scl_out(int v){
	gpio_set_output_en(PIN_I2C_SCL,1);
	gpio_set_input_en(PIN_I2C_SCL,0);
	gpio_write(PIN_I2C_SCL,v);
}
static inline int i2c_sda_in(void){
	gpio_set_input_en(PIN_I2C_SDA,1);
	gpio_set_output_en(PIN_I2C_SDA,0);
	return gpio_read(PIN_I2C_SDA);
}
static void i2c_wait_busy(void)
{
	u8 Times=0;
	while ((i2c_sda_in() != 0)&&(Times++<200));
}
static void i2c_wirte_bit(int bit)
{
	i2c_scl_out(0);
	i2c_sda_out(bit);
	i2c_long_wait();
	i2c_scl_out(1);
	i2c_long_wait();
}
static int i2c_read_bit(void){
//	i2c_wirte_bit(1);
	i2c_scl_out(0);
//	i2c_sda_out(bit);
	i2c_long_wait();
	i2c_scl_out(1);
	i2c_long_wait();
	return i2c_sda_in();
}
static inline void i2c_scl_init(void){
	gpio_set_func(PIN_I2C_SCL, AS_GPIO);
}
static inline void i2c_sda_init(void){
	gpio_set_func(PIN_I2C_SDA, AS_GPIO);
	gpio_set_input_en(PIN_I2C_SDA, 1);
}

static inline void i2c_scl_idle(void){
	gpio_set_output_en(PIN_I2C_SCL, 0);
	gpio_write(PIN_I2C_SCL, 0);
}

static inline void i2c_sda_idle(void){
	gpio_set_output_en(PIN_I2C_SDA, 0);
	gpio_write(PIN_I2C_SDA, 0);
}
/******************************************
 * 功能：i2c写一个字节
 * 参数：dat   写数据
 * 返回：
 *****************************************/
int i2c_write_byte(u8 dat){
	int i = 0x80;
	while(i){
		i2c_wirte_bit((dat & i));
		i = i >> 1;
	}
	i2c_sda_in();
	return i2c_read_bit();
}
/******************************************
 * 功能：i2c开始数据通信
 * 参数：
 * 返回：
 *****************************************/
void i2c_start(void)
{
	i2c_scl_init();
	i2c_sda_init();
	i2c_sda_idle();
	i2c_scl_idle();
	i2c_sda_out(0);		//sda: 0
}
/******************************************
 * 功能：i2c结束数据通信
 * 参数：
 * 返回：
 *****************************************/
void i2c_stop(void)
{
	i2c_sda_out(0);
	i2c_scl_out(0);
	i2c_scl_out(1);
	i2c_sda_out(1);
}
/******************************************
 * 功能：i2c读一个字节
 * 参数：last  应答
 * 返回：读出一个字节数据
 *****************************************/
u8 i2c_read_byte(int last){
	u8 dat = 0,i;
	for(i=0;i<8;i++){
//		i2c_wirte_bit(1);
		i2c_scl_out(0);
		//	i2c_sda_out(bit);
		i2c_long_wait();
		i2c_scl_out(1);
		i2c_long_wait();
		if(i2c_sda_in()){
			dat =(dat << 1) | 0x01;
		}else{
			dat = dat << 1;
		}
	}
	i2c_wirte_bit(last);
	return dat;
}
void i2c_master_init(void)
{
//	i2c_init();
}
/******************************************
 * 功能：i2c 8位地址模式读数据
 * 参数：id      设备id
 *       addr    读地址
 *       *p      读的数据存入的地址
 *       n       读数据的长度
 * 返回：
 *****************************************/
void i2c_addr8_read(u8 id, u8 addr,u8 *p,u8 n)
{
	i2c_start();

	i2c_write_byte(id);
	i2c_wait_busy();
	i2c_write_byte(addr);
	i2c_wait_busy();
	i2c_stop();
	i2c_long_wait();
	i2c_start();
	i2c_write_byte(id | 1);
	i2c_wait_busy();

	for(int k = 0; k < n; ++k){
		*p++ = i2c_read_byte( k ==(n-1) );
	}
	i2c_stop();

}
/******************************************
 * 功能：i2c 16位地址模式读数据
 * 参数：id      设备id
 *       addr    读地址
 *       *p      读的数据存入的地址
 *       n       读数据的长度
 * 返回：
 *****************************************/
void i2c_addr16_read(u8 id, u16 addr,u8 *p,u8 n)
{
	i2c_start();

	i2c_write_byte(id);
	i2c_long_wait();
//	i2c_wait_busy();
	i2c_write_byte(addr>>8);
//	i2c_wait_busy();
	i2c_long_wait();
	i2c_write_byte(addr&0xff);
//	i2c_wait_busy();
	i2c_long_wait();
	i2c_stop();
	i2c_long_wait();
	i2c_start();
	i2c_write_byte(id | 1);
	i2c_long_wait();
//	i2c_wait_busy();
	for(int k = 0; k < n; ++k){
		*p++ = i2c_read_byte( k ==(n-1) );
	}
	i2c_stop();

}
/******************************************
 * 功能：i2c dma模式下的读
 * 参数：id      读设备id
 *      address  读地址
 *      addr_mod 设备的地址模式     0  8bit地址      1   16bit地址
 * 返回：读出的数据
 *****************************************/
u8 i2c_dma_read(u8 id,int address,u8 addr_mod)
{
	u8 dat;
	if(addr_mod==1)
		i2c_addr16_read (id, address, &dat, 1);
	else
		i2c_addr8_read (id, address, &dat, 1);
	return dat;
}
/******************************************
 * 功能：i2c 8位地址模式写数据
 * 参数：id      设备id
 *       addr    读地址
 *       data    写数据
 * 返回：
 *****************************************/
void i2c_addr8_write(u8 id, u8 addr, u8 dat)
{
	i2c_start();
	i2c_write_byte(id&0xfe);
	i2c_wait_busy();
	i2c_write_byte(addr);
	i2c_wait_busy();
	i2c_write_byte(dat);
	i2c_wait_busy();
	i2c_stop();
}
/******************************************
 * 功能：i2c 16位地址模式写数据
 * 参数：id      设备id
 *       addr    读地址
 *       data    写数据
 * 返回：
 *****************************************/
void i2c_addr16_write(u8 id, u16 addr, u8 data)
{
	i2c_start();
	i2c_write_byte(id&0xfe);
	i2c_wait_busy();
	i2c_write_byte(addr>>8);
	i2c_wait_busy();
	i2c_write_byte(addr&0xff);
	i2c_wait_busy();
	i2c_write_byte(data);
	i2c_wait_busy();
	i2c_stop();
}
/******************************************
 * 功能：i2c dma模式下的写
 * 参数：id      写设备id
 *      address  写地址
 *      data     写数据
 *      addr_mod 设备的地址模式     0  8bit地址      1   16bit地址
 * 返回：
 *****************************************/
void i2c_dma_write(u8 id,int address, int data,u8 addr_mod)
{
	static unsigned int prev_wr_timing=0;

	while(!clock_time_exceed(prev_wr_timing, 5000)){
	#if(MODULE_WATCHDOG_ENABLE)
			wd_clear();
	#endif
		};
	if(addr_mod==1)
		i2c_addr16_write(id, address, data);
	else
		i2c_addr8_write(id, address, data);
	prev_wr_timing = clock_time();

//	WaitUs(5500);  //wait 5ms at least
}
/******************************************
 * 功能：i2c map模式的写数据
 * 参数：id     写i2c设备的id
 *       data   写数据
 *       len    写的长度
 * 返回：
 *****************************************/
void i2c_map_write(u8 id,u8 *data,u8 len)
{
	u8 i;
	i2c_start();
	i2c_write_byte(id);
	i2c_wait_busy();
	for(i=0;i<len;i++){
		i2c_write_byte(data[i]);
	}
	i2c_stop();
}
/******************************************
 * 功能：映射模式读数据
 * 参数：id   读i2c设备id
 *       buf  读出数据存入的缓存
 *       len  读数据的长度
 * 返回：
 *****************************************/
void i2c_map_read(u8 id,u8 *buf,u8 len)
{
	u8 i;
	i2c_start();
	i2c_write_byte(id|1);
	i2c_wait_busy();
	for(i=0;i<len-1;i++){
		buf[i]=i2c_read_byte(i==(len-1));
//		i2c_sda_out(1);
	}
	i2c_stop();
}
/******************************************
 * 功能：i2c slave模式初始化
 * 参数：i2c_set   i2c的设置模式
 *       buf      i2c在map模式的映射地址
 * 返回：
 *****************************************/
void I2C_SlaveInit(I2C_info_t i2c_set,u8 * pBuf)
{
    unsigned char tmp = 0;

    /******1. Set SCL/SDA Pins******/
    if (i2c_set.i2c_port != I2C_PORT0) {
        //disable GP4/GP5 i2c function
		tmp = read_reg8(0x8005d4); //clear 0x5d4[0]
		tmp &= 0xfe;
		write_reg8(0x8005d4, tmp);

		if (i2c_set.i2c_port != I2C_PORT1) {
            //disable GP12/GP13 i2c function
		    tmp = read_reg8(0x80058e); //set 0x58e[5]
		    tmp |= 0x20;
		    write_reg8(0x80058e, tmp);

		    if (i2c_set.i2c_port != I2C_PORT2) {
                //disable GP22/GP23 i2c function
			    tmp = read_reg8(0x800596); //clear 0x596[7]
			    tmp |= 0x80;
			    write_reg8(0x800596, tmp);

			    //enable GP30/GP31 i2c function
			    tmp = read_reg8(0x80059e); //clear 0x59e[7]
			    tmp &= 0x7f;
			    write_reg8(0x80059e, tmp);
			    //set GP30/GP31 input enable
			    tmp = read_reg8(0x800599);
			    tmp |= 0xc0;
			    write_reg8(0x800599, tmp);
		    }
		    else {
                //enable GP22/GP23 i2c function
			    tmp = read_reg8(0x800596); //clear 0x596[7]
			    tmp &= 0x7f;
			    write_reg8(0x800596, tmp);
			    //set GP22/GP23 input enable
			    tmp = read_reg8(0x800591);
			    tmp |= 0xc0;
			    write_reg8(0x800591, tmp);
		    }
		}
		else {
            //enable GP12/GP13 i2c function
		    tmp = read_reg8(0x80058e); //clear 0x58e[5]
		    tmp &= 0xdf;
		    write_reg8(0x80058e, tmp);
		    //set GP12/GP13 input enable
		    tmp = read_reg8(0x800589);
		    tmp |= 0x30;
		    write_reg8(0x800589, tmp);
		}
    }
    else {
	    /***set GP4/GP5 as SCL/SDA***/
	    //clear 0x586[4] 0x586[5]
	    tmp = read_reg8(0x800586);
	    tmp &= 0xcf;
	    write_reg8(0x800586, tmp);
	    //set 0x5d4[0]
	    tmp = read_reg8(0x8005d4);
	    tmp |= 0x01;
	    write_reg8(0x8005d4, tmp);
	    //set GP4/GP5 input enable
	    tmp = read_reg8(0x800581);
	    tmp |= 0x30;
	    write_reg8(0x800581, tmp);
    }

    /******2. Set I2C Slave Mode(DMA or Mapping)******/
    if (i2c_set.mode == I2C_SLAVE_MAP) {
    	//set 0x03[2] to enable mapping mode
	    tmp = read_reg8(0x800003);
	    tmp |= 0x04;
	    write_reg8(0x800003, tmp);
	    //set buf's address low byte
	    tmp = (unsigned int)pBuf & 0xff;
	    write_reg8(0x80003e, tmp);
	    //set buf's address high byte
	    tmp = ((unsigned int)pBuf>>8) & 0xff;
	    write_reg8(0x80003f, tmp);
    }

    /******3. Set I2C Slave Irq******/
    if (i2c_set.irq == I2C_IRQ_ENABLE) {
        write_reg8(0x800640, read_reg8(0x800640)|0x80); //enable irq_host_cmd
    	write_reg8(0x800643, read_reg8(0x800643)|0x01); //enable irq function
    	i2c_irq_handler=i2c_set.i2c_irq_handler;
    }
    else {
    	write_reg8(0x800640, read_reg8(0x800640)&0x7f); //disable irq_host_cmd
    }
}
