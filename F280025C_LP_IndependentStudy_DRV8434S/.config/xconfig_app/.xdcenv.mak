#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/ti-processor-sdk-rtos-j7200-evm-07_03_00_07/bios_6_83_02_07/packages;C:/ti/ti-processor-sdk-rtos-j7200-evm-07_03_00_07/uia_2_41_00_01/packages;C:/Users/a0489849/workspace_v10/F280025C_SYSBIOS_Typical/.config
override XDCROOT = C:/ti/ccs1031/xdctools_3_62_01_15_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/ti-processor-sdk-rtos-j7200-evm-07_03_00_07/bios_6_83_02_07/packages;C:/ti/ti-processor-sdk-rtos-j7200-evm-07_03_00_07/uia_2_41_00_01/packages;C:/Users/a0489849/workspace_v10/F280025C_SYSBIOS_Typical/.config;C:/ti/ccs1031/xdctools_3_62_01_15_core/packages;..
HOSTOS = Windows
endif
