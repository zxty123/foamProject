
#include "anareg.h"
#include "analog.h"

static inline void rega_set_ldo_output(int bg, int vol){
#if (MCU_CORE_TYPE == MCU_CORE_3520)
#else
	u8 v = MASK_VAL(FLDA_VOL_BAND_GAP, bg, FLDA_VOL_LDO_OUTPUT, vol) | FLDA_VOL_LDO_RESV;
	analog_write(rega_vol_ldo_ctrl, v);
#endif
}

static inline void rega_poweron_dcdc_6p75(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_3520)
#else
	u8 v = FLDA_DCDC_OUT_6P6_SEL | FLDA_DCDC_RESETB;	// 0x92
	analog_write(rega_dcdc_ctrl, v);
#endif
}

static inline void rega_powerdn_dcdc_6p75(void)
{
#if (MCU_CORE_TYPE == MCU_CORE_3520)
#else
	u8 v =  FLDA_DCDC_6P75_PWDN | FLDA_RNS_PWDN|FLDA_DCDC_RESV;	// 0x92
	analog_write(rega_dcdc_ctrl, v);
#endif
}

