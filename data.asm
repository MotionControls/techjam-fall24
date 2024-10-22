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
; coltestbg:
; .incbin "res/BgColTest.pic"
; coltestbg_end:

; coltestpal:
; .incbin "res/BgColTest.pal"
; coltestpal_end:

; coltestmap:
; .incbin "res/BgColTest.map"
; coltestmap_end:

.ends
