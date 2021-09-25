## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e28FP linker.cmd package/cfg/app_pe28FP.oe28FP

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/app_pe28FP.xdl
	$(SED) 's"^\"\(package/cfg/app_pe28FPcfg.cmd\)\"$""\"C:/Users/a0489849/workspace_v10/F280025C_LP_IndependentStudy_DRV8434S/.config/xconfig_app/\1\""' package/cfg/app_pe28FP.xdl > $@
	-$(SETDATE) -r:max package/cfg/app_pe28FP.h compiler.opt compiler.opt.defs
