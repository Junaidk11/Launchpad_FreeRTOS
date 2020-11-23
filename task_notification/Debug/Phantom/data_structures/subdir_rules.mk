################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Phantom/data_structures/%.obj: ../Phantom/data_structures/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --include_path="/Users/junaidkhan/Documents/Launchpad_FreeRTOS/Phantom/data_structures" --include_path="/Users/junaidkhan/Documents/Launchpad_FreeRTOS/Phantom/Drivers/EEPROM" --include_path="/Users/junaidkhan/Documents/Launchpad_FreeRTOS" --include_path="/Users/junaidkhan/Documents/Launchpad_FreeRTOS/include" --include_path="/Applications/ti/Hercules/F021 Flash API/02.01.01/include" --include_path="/Applications/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile --preproc_dependency="Phantom/data_structures/$(basename $(<F)).d_raw" --obj_directory="Phantom/data_structures" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


