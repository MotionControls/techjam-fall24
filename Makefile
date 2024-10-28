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
john.pic: res/johnplaceholder2.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 32 -o 16 -u 16 -t bmp -i $<

sheep.pic: res/AngelAngle/sheep_sprites.png
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 32 -o 16 -u 16 -i $<

bullet.pic: res/AngelAngle/bullet_sprites.png
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 16 -o 16 -u 16 -i $<
	mv res/AngelAngle/bullet_sprites.pal res/AngelAngle/bullet_sprites_blue.pal
	
# BACKGROUNDS
bg_icons.pic: res/bg_icons.bmp
	@echo convert bmp ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -t bmp -p -m -i $<

# TILED Stuffs
tiled_testset.pic: res/tiled_testset.png
	@echo convert map tileset ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -m -i $<

tiled_test.m16: res/tiled_test.tmj tiled_testset.pic
	@echo convert map tiled ... $(notdir $@)
	$(TMXCONV) $< res/tiled_testset.map

bitmaps : john.pic bg_icons.pic tiled_test.m16 tiled_testset.pic sheep.pic bullet.pic
