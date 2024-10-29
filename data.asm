.include "hdr.asm"

.section ".rodata1" superfree

;;;;;;;;;;;
; Sprites ;
;;;;;;;;;;;
johnspr:
.incbin "res/johnplaceholder2.pic"
johnspr_end:

johnpal:
.incbin "res/johnplaceholder2.pal"
johnpal_end:

;;;;;;;;;;;;;;;
; Backgrounds ;
;;;;;;;;;;;;;;;
cloudsbg:
.incbin "res/iomor.pic"
cloudsbg_end:

cloudspal:
.incbin "res/iomor.pal"
cloudspal_end:

cloudsmap:
.incbin "res/iomor.map"
cloudsmap_end:

;;;;;;;;;;;;;;;;
; TILED Stuffs ;
;;;;;;;;;;;;;;;;

; tiled_testbg:
; .incbin "res/tiled_testset.pic"
; tiled_testbg_end:

; tiled_testpal:
; .incbin "res/tiled_testset.pal"
; tiled_testpal_end:

; tiled_testmap:	; MAPS that are used in the map engine don't require any length.
; .incbin "res/tiled_test.m16"

; tiled_testtiles:
; .incbin "res/tiled_test.t16"

; tiled_testprops:
; .incbin "res/tiled_test.b16"

; tldWalltilesetbg:
; .incbin "res/LevelConcepts/WallTileset.pic"
; tldWalltilesetbg_end:

; tldWalltilesetpal:
; .incbin "res/LevelConcepts/WallTileset.pal"
; tldWalltilesetpal_end:

; tldLevel1map:
; .incbin "res/LevelConcepts/tldLevel1.m16"
; tldLevel1prp:
; .incbin "res/LevelConcepts/tldLevel1.b16"
; tldLevel1tle:
; .incbin "res/LevelConcepts/tldLevel1.t16"

;;;;;;;;;
; Fonts ;
;;;;;;;;;

snesfont:
.incbin "res/pvsneslibfont.pic"

snespal:
.incbin "res/pvsneslibfont.pal"

.ends

.section ".rodata2" superfree

;;;;;;;;;;;
; Sprites ;
;;;;;;;;;;;
sheepspr:
.incbin "res/AngelAngle/sheep_sprites.pic"
sheepspr_end:

sheeppal:
.incbin "res/AngelAngle/sheep_sprites.pal"
sheeppal_end:

bulletspr:
.incbin "res/AngelAngle/bullet_sprites.pic"
bulletspr_end:

bulletbluepal:
.incbin "res/AngelAngle/bullet_sprites_blue.pal"
bulletbluepal_end:

soulspr:
.incbin "res/AngelAngle/soul_sprites.pic"
soulspr_end:

soulbluepal:
.incbin "res/AngelAngle/soul_sprites_blue.pal"
soulbluepal_end:

;;;;;;;;;;;;;;;
; Backgrounds ;
;;;;;;;;;;;;;;;
level1bg:
.incbin "res/LevelConcepts/concept1.pic"
level1bg_end:
level1pal:
.incbin "res/LevelConcepts/concept1.pal"
level1pal_end:
level1map:
.incbin "res/LevelConcepts/concept1.map"
level1map_end:

level2bg:
.incbin "res/LevelConcepts/concept2.pic"
level2bg_end:
level2pal:
.incbin "res/LevelConcepts/concept2.pal"
level2pal_end:
level2map:
.incbin "res/LevelConcepts/concept2.map"
level2map_end:

level3bg:
.incbin "res/LevelConcepts/concept3.pic"
level3bg_end:
level3pal:
.incbin "res/LevelConcepts/concept3.pal"
level3pal_end:
level3map:
.incbin "res/LevelConcepts/concept3.map"
level3map_end:

level4bg:
.incbin "res/LevelConcepts/concept4.pic"
level4bg_end:
level4pal:
.incbin "res/LevelConcepts/concept4.pal"
level4pal_end:
level4map:
.incbin "res/LevelConcepts/concept4.map"
level4map_end:

level5bg:
.incbin "res/LevelConcepts/concept6.pic"
level5bg_end:
level5pal:
.incbin "res/LevelConcepts/concept6.pal"
level5pal_end:
level5map:
.incbin "res/LevelConcepts/concept6.map"
level5map_end:

.ends