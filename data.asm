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

;;;;;;;;;
; Fonts ;
;;;;;;;;;

snesfont:
.incbin "res/pvsneslibfont.pic"

snespal:
.incbin "res/pvsneslibfont.pal"

.ends
