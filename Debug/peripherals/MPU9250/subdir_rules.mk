################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
peripherals/MPU9250/%.obj: ../peripherals/MPU9250/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs2002/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/Applications/ti/ccs2002/ccs/ccs_base/msp430/include" --include_path="/Users/liusongran/workspace_ccstheia/iKVStore" --include_path="/Users/liusongran/workspace_ccstheia/iKVStore/comm" --include_path="/Users/liusongran/workspace_ccstheia/iKVStore/driverlib/MSP430F5xx_6xx" --include_path="/Applications/ti/ccs2002/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --include_path="/Users/liusongran/workspace_ccstheia/iKVStore/peripherals/MPU9250" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="peripherals/MPU9250/$(basename $(<F)).d_raw" --obj_directory="peripherals/MPU9250" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


