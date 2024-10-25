.include "hdr.asm"

.section ".rodata1" superfree

;;;;;;;;;;;
; Sprites ;
;;;;;;;;;;;
johnspr:
;.incbin "res/johnplaceholder.pic"
.incbin "res/johnplaceholder2.pic"
johnspr_end:

johnpal:
;.incbin "res/johnplaceholder.pal"
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

tileset:
.incbin "res/tileset.pic"
tileset_end:

tileset_pal:
.incbin "res/tileset.pal"

;;;;;;;;
; Maps ;
;;;;;;;;

map_testlvl:
.incbin "res/BG1.m16"

map_tilesetatt:
.incbin "res/test_lvl1.b16"

map_tilesetdef:
.incbin "res/test_lvl1.t16"

;;;;;;;;;
; Fonts ;
;;;;;;;;;

snesfont:
.incbin "res/pvsneslibfont.pic"

snespal:
.incbin "res/pvsneslibfont.pal"

.ends
