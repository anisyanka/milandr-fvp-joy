#
#Executable name
#
EXE_NAME=app

#
#Folder to place compiled objects
#
BUILDDIR = build-app

#
#Verbose mode
#
VERBOSE=no

#
#Colorize ouput
#
COLORIZE=yes

#
#Enable binary creation
#
MAKE_BINARY=yes

#
#Enable executable creation
#
MAKE_EXECUTABLE=yes

#
#Enable shared library creation
#
MAKE_SHARED_LIB=no

#
#Enable static library creation
#
MAKE_STATIC_LIB=no

#
#Enable MAP-file creation
#
CREATE_MAP=yes

#
# Debug or release mode
#
BUILD_DEBUG_MODE=yes

#
#Tool-chain prefix
#
TCHAIN = arm-none-eabi-

#
#CPU specific options
#
MCPU += -mcpu=cortex-m0plus
MCPU += -mthumb

#
#C language dialect
#
CDIALECT = gnu99

#
#C++ language dialect
#
#CXXDIALECT = c++0x

#
#Additional C flags
#
CFLAGS +=-fdata-sections -ffunction-sections 
CFLAGS +=-fsingle-precision-constant
CFLAGS +=-funsafe-math-optimizations
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#
#Additional CPP flags
#
CXXFLAGS +=-fdata-sections -ffunction-sections

#
#Additional linker flags
#
LDFLAGS +=-nostartfiles
LDFLAGS +=-specs=nano.specs
LDFLAGS +=-Wl,--gc-sections
LDFLAGS += -Wl,--print-memory-usage
LDFLAGS += -u _printf_float
LDFLAGS += -u _scanf_float

#
#Additional link objects
#
#EXT_OBJECTS =

#
#Additional static libraries
#
EXT_LIBS +=c
EXT_LIBS +=m
EXT_LIBS +=nosys

#
# Create git info about fw version
#
GIT_HASH=$(shell git rev-parse HEAD)
GIT_HASH_SHORT=$(shell git rev-parse --short HEAD)
GIT_BRANCH=$(shell git branch --show-current)

#
# Preprocessor definitions
#
PPDEFS += GIT_HASH=\"$(GIT_HASH)\"
PPDEFS += GIT_HASH_SHORT=\"$(GIT_HASH_SHORT)\"
PPDEFS += GIT_BRANCH=\"$(GIT_BRANCH)\"

#
#Include directories
#
INCDIR += sources/RTE/Device/MDR1986VE94GI/
INCDIR += make/STDPeriph/Libraries/SPL/MDR32FxQI
INCDIR += make/STDPeriph/Libraries/SPL/MDR32FxQI/inc

#
#Source files
#
SOURCES += sources/RTE/Device/MDR1986VE94GI/startup_MDR1986VE94GI.S
SOURCES += $(wildcard make/STDPeriph/Libraries/SPL/MDR32FxQI/src/*.c)



.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\main.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\debounce.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\board.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\board_common.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\CMSIS\Device\ST\STM32F4xx\Include\stm32f4xx.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\dataconstants.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\CMSIS\Include\core_cmFunc.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\pulses\pulses.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\FatFs\ff.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\FatFs\ffconf.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\tasks.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\tasks.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\rtos.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\opentx.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\translations.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\bluetooth.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\datastructs.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\chksize.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\cli.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\buzzer.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\buzzer.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\CMSIS\Include\core_cm4.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\coocox.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\debug.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gps.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gvars.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gvars.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\opentx.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\disk_cache.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\haptic.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\haptic.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\functions.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\stamp.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\syscalls.c
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\simu.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\keys.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gyro.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\mixer.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\logs.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\CMakeLists.txt
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\timers.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\sbus.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\timers.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\sdcard.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\hal.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\translations\en.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\.gitignore
\Keil_v5\ARM\RV31\INC\Net_Config.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\tx24_v2
\Keil_v5\ARM\RV31\INC\RTL.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\Objects\tx24.sct
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\audio.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\FatFs\..\CoOS\kernel\CoOS.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\core.c
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\OsCore.h
\Keil_v5\ARM\ARMCLANG\include\math.h
\Keil_v5\Packs\ARM\CMSIS\6.1.0\CMSIS\Core\Include\m-profile\cmsis_armclang_m.h
\Keil_v5\Packs\ARM\CMSIS\6.1.0\CMSIS\Core\Include\cmsis_armclang.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\event.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\timer.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\mutex.c
\Keil_v5\Packs\ARM\CMSIS\6.1.0\CMSIS\Core\Include\cmsis_compiler.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\switches.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\curves.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\gui.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\keys.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\keys_driver.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\STM32F4xx_StdPeriph_Driver\inc\stm32f4xx_gpio.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\menus.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\audio.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\..\common\stdlcd\popups.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\board.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\diskio.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\lcd_1bit.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\storage\storage.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\telemetry\telemetry.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\opentx_types.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\RTE\Device\STM32F407VETx\startup_stm32f407xx.s
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\vario.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\telemetry\telemetry_sensors.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\storage\storage_common.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\telemetry\frsky.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\startup_shutdown.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\popups.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\menus.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\menus.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_select.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\navigation\navigation_x7.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\telemetry\telemetry.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\rtc.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\rtc.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\trainer_driver.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\portable\OsArch.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\task.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\lcd_driver_spi.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\pwr_driver.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\led_driver.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\backlight_driver.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\delays.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\menu_model.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\draw_functions.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\adc_driver.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\view_about.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\gui_common.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\gui_common.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\translations.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\radio_setup.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\radio_hardware.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\STM32F4xx_StdPeriph_Driver\src\stm32f4xx_gpio.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\fonts.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\fonts.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_Device_Library\Core\inc\usbd_core.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_Device_Library\Class\hid\src\usbd_hid_core.c
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_Device_Library\Class\hid\inc\usbd_hid_core.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\usb_driver.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\usbd_storage_msd.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\bin_allocator.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\bin_allocator.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\view_telemetry.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\lcd.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_Device_Library\Class\msc\inc\usbd_msc_scsi.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_Device_Library\Class\msc\src\usbd_msc_bot.c
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_Device_Library\Class\msc\inc\usbd_msc_data.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_Device_Library\Core\src\usbd_req.c
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_OTG_Driver\inc\usb_dcd.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\rotary_encoder_driver.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\telemetry_driver.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\STM32F4xx_StdPeriph_Driver\inc\stm32f4xx_usart.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\STM32F4xx_StdPeriph_Driver\inc\stm32f4xx_syscfg.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\STM32F4xx_StdPeriph_Driver\inc\misc.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F2xx_StdPeriph_Lib_V1.1.0\Libraries\CMSIS\Device\ST\STM32F2xx\Include\stm32f2xx.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\STM32F4xx_StdPeriph_Driver\src\stm32f4xx_usart.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\view_statistics.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\RTE\Device\STM32F407VETx\system_stm32f4xx.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_special_functions.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_logical_switches.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\telemetry\telemetry_sensors.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F2xx_StdPeriph_Lib_V1.1.0\Libraries\CMSIS\Include\arm_math.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\tests\gtests.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\telemetry\frsky_sport.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_telemetry.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_telemetry_sensor.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\model_curves.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_curve_edit.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_outputs.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\myeeprom.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\radio_version.cpp
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\draw_functions.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\intmodule_serial_driver.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_setup.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\lua\api_general.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V2\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\draw_functions.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_mix_edit.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\model_mixes.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\model_inputs.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_input_edit.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\model_display.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\portable\GCC\port.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\OsConfig.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F2xx_StdPeriph_Lib_V1.1.0\Libraries\STM32F2xx_StdPeriph_Driver\inc\stm32f2xx_syscfg.h
RTE\Device\MDR1986VE94GI\MDR1986VE94GI.sct
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\mixer_scheduler_driver.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\trainer.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\serial.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\mixer_scheduler.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\usb_driver.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\common\stdlcd\radio_trainer.cpp
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F2xx_StdPeriph_Lib_V1.1.0\Libraries\STM32F2xx_StdPeriph_Driver\inc\stm32f2xx_gpio.h
\Keil_v5\Packs\Milandr\MDR32FxQI\1.2.0\Libraries\CMSIS\MDR32FxQI\DeviceSupport\MDR1986VE94GI\inc\MDR1986VE94GI.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\usb_conf.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32_USB-Host-Device_Lib_V2.2.0\Libraries\STM32_USB_OTG_Driver\inc\usb_regs.h

.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\dmafifo.h











\Keil_v5\Packs\Milandr\MDR32FxQI\1.2.0\Libraries\CMSIS\MDR32FxQI\CoreSupport\CM3\core_cm3.h
\SmartCave\C_bkp\Workbanch\tx24_OPEN_TX_V3_milandr\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\STM32F4xx_DSP_StdPeriph_Lib_V1.4.0\Libraries\STM32F4xx_StdPeriph_Driver\src\misc.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\strhelpers.cpp



RTE\Device\MDR1986VE94GI\MDR32FxQI_config.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\taranis\dwt.h
\Keil_v5\Packs\ARM\CMSIS\6.1.0\CMSIS\Core\Include\core_cm3.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\targets\common\arm\stm32\timers_driver.cpp

.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\pulses\pulses.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\debug.h
RTE\Device\MDR1986VE94GI\system_MDR1986VE94GI.c
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\telemetry\telemetry_holders.h
RTE\Device\MDR1986VE94GI\system_MDR1986VE94GI.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\pwr.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\time.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\globals.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\opentx_constants.h
opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\OsTask.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\hook.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\mixer_scheduler.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\kernel\flag.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\thirdparty\CoOS\portable\arch.c
.\ST7920_SPI-master\ST7920_SPI.cpp
.\ST7920_SPI-master\ST7920_SPI.h
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\opentx_helpers.h
\Keil_v5\ARM\ARMCLANG\include\libcxx\cstddef
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\pulses\ppm.cpp
\Keil_v5\ARM\ARMCLANG\include\libcxx\math.h
.\st7920-main\src\driver_st7920.h
.\st7920-main\interface\driver_st7920_interface.h
.\st7920-main\test\driver_st7920_display_test.h
.\st7920-main\interface\driver_st7920_interface_template.c
.\st7920-main\test\driver_st7920_display_test.c
.\st7920-main\src\driver_st7920.c
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\view_main.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\view_channels.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\widgets.cpp
.\opentx-release-2.3.15\opentx-release-2.3.15\radio\src\gui\128x64\radio_calibration.cpp

#
#Linker scripts
#
LDSCRIPT += make/ldscript/stm32l073v8tx_flash.ld

SELFDEP = app.mk

include core.mk