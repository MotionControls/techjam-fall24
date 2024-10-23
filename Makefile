ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

# Enable debug mode
export PVSNESLIB_DEBUG=1

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := f24entry

all: bitmaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx
	
#---------------------------------------------------------------------------------
# https://github.com/alekmaul/pvsneslib/tree/master/tools/gfx4snes
# SPRITES
john.pic: res/johnplaceholder.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 32 -o 16 -u 16 -t bmp -i $<

# BACKGROUNDS
bg_icons.pic: res/bg_icons.bmp
	@echo convert bmp ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -t bmp -p -m -i $<

bitmaps : john.pic bg_icons.pic
