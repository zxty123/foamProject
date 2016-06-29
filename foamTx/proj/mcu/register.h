#pragma once

#if(MCU_CORE_TYPE == MCU_CORE_8266)
#include "../mcu_spec/register_8266.h"
#elif(MCU_CORE_TYPE == MCU_CORE_8366)
#include "../mcu_spec/register_8366.h"
#elif(MCU_CORE_TYPE == MCU_CORE_8368)
#include "../mcu_spec/register_8368.h"
#endif

