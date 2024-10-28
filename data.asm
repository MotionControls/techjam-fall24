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
iconsbg:
.incbin "res/bg_icons.pic"
iconsbg_end:

iconspal:
.incbin "res/bg_icons.pal"
iconspal_end:

iconsmap:
.incbin "res/bg_icons.map"
iconsmap_end:

;;;;;;;;;;;;;;;;
; TILED Stuffs ;
;;;;;;;;;;;;;;;;

tiled_testbg:
.incbin "res/tiled_testset.pic"
tiled_testbg_end:

tiled_testpal:
.incbin "res/tiled_testset.pal"
tiled_testpal_end:

tiled_testmap:	; MAPS that are used in the map engine don't require any length.
.incbin "res/tiled_test.m16"

tiled_testtiles:
.incbin "res/tiled_test.t16"

tiled_testprops:
.incbin "res/tiled_test.b16"

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

.ends