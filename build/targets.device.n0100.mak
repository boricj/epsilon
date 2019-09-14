.PHONY: %_flash
%_flash: $(BUILD_DIR)/%.dfu
	@echo "DFU     $@"
	@echo "INFO    About to flash your device. Please plug your device to your computer"
	@echo "        using an USB cable and press the RESET button the back of your device."
	$(Q) until $(PYTHON) build/device/dfu.py -l | grep -E "0483:a291|0483:df11" > /dev/null 2>&1; do sleep 2;done
	$(Q) $(PYTHON) build/device/dfu.py -m -u $<

.PHONY: %.two_binaries
%.two_binaries: %.elf
	@echo "Building an internal and an external binary for     $<"
	$(Q) $(OBJCOPY) -O binary -j .text.external -j .rodata.external $< $(basename $<).external.bin
	$(Q) $(OBJCOPY) -O binary -R .text.external -R .rodata.external $< $(basename $<).internal.bin
	@echo "Padding $(basename $<).external.bin and $(basename $<).internal.bin"
	$(Q) printf "\xFF\xFF\xFF\xFF" >> $(basename $<).external.bin
	$(Q) printf "\xFF\xFF\xFF\xFF" >> $(basename $<).internal.bin
