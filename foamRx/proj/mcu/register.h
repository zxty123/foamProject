#pragma once

#if(MCU_CORE_TYPE == MCU_CORE_5320)
#include "../mcu_spec/register_5320.h"
#elif(MCU_CORE_TYPE == MCU_CORE_5328)
#include "../mcu_spec/register_5328.h"
#elif(MCU_CORE_TYPE == MCU_CORE_5330)
#include "../mcu_spec/register_5330.h"
#elif(MCU_CORE_TYPE == MCU_CORE_5332)
#include "../mcu_spec/register_5332.h"
#elif(MCU_CORE_TYPE == MCU_CORE_3520)
#include "../mcu_spec/register_3520.h"
#endif

