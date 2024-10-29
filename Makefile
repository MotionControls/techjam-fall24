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
	-mv res/AngelAngle/bullet_sprites.pal res/AngelAngle/bullet_sprites_blue.pal

soul.pic: res/AngelAngle/soul_sprites.png
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 32 -o 16 -u 16 -i $<
	-mv res/AngelAngle/soul_sprites.pal res/AngelAngle/soul_sprites_blue.pal
	
# BACKGROUNDS
# bg_icons.pic: res/bg_icons.bmp
	# @echo convert bmp ... $(notdir $@)
	# $(GFXCONV) -s 8 -o 16 -u 16 -e 0 -t bmp -p -m -i $<

level1.pic: res/LevelConcepts/concept1.png
	@echo convert bmp ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -i $<
level2.pic: res/LevelConcepts/concept2.png
	@echo convert bmp ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -i $<
level3.pic: res/LevelConcepts/concept3.png
	@echo convert bmp ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -i $<
level4.pic: res/LevelConcepts/concept4.png
	@echo convert bmp ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -i $<
level5.pic: res/LevelConcepts/concept6.png
	@echo convert bmp ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 0 -p -m -i $<


# TILED Stuffs
# tiled_testset.pic: res/tiled_testset.png
	# @echo convert map tileset ... $(notdir $@)
	# $(GFXCONV) -s 8 -o 16 -u 16 -p -m -i $<

# tiled_test.m16: res/tiled_test.tmj tiled_testset.pic
	# @echo convert map tiled ... $(notdir $@)
	# $(TMXCONV) $< res/tiled_testset.map

# wall_tileset.pic:	res/LevelConcepts/WallTileset.png
	# @echo convert map tileset ... $(notdir $@)
	# $(GFXCONV) -s 8 -o 16 -u 16 -p -m -i $<

# tldLevel1.m16: res/LevelConcepts/concept1.tmj wall_tileset.pic
	# @echo convert map tiled ... $(notdir $@)
	# $(TMXCONV) $< res/LevelConcepts/WallTileset.map

bitmaps : john.pic sheep.pic bullet.pic soul.pic level1.pic level2.pic level3.pic level4.pic level5.pic
