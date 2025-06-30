all: app
clean: app_clean
	
app:
	+make -f app.mk

app_clean:
	make -f app.mk clean

app_stlink_fw_flash:
	# @make/scripts/flash_app_fw_stlink.sh

erase_all_stlink:
	# @make/scripts/erase_all_stlink.sh