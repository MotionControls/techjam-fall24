.include "hdr.asm"

.section ".rodata1" superfree

;;;;;;;;;;;
; Sprites ;
;;;;;;;;;;;
johnspr:
.incbin "res/johnplaceholder.pic"
johnspr_end:

johnpal:
.incbin "res/johnplaceholder.pal"
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

.ends
