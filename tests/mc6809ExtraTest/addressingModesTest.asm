; Sources:
; MC6809 datasheet, Figure 19 - Programming Aid
; Programming the 6809, Zaks, Appendix F - Indirect Addressing Mode Postbytes


        ORG     $F000

MF000
; LDA
        LDA     #$00        ; 0000: 86 00          Immediate
        LDA     MF000       ; 0002: 96 00          Direct
        LDA     $00,X       ; 0004: A6 00          Indexed
        LDA     >MF000      ; 0006: B6 00 00       Extended

        LDA     ,X          ; Constant Indexed non-indirect 
        LDA     $1,X        ; Constant Indexed non-indirect, 5 bits
        LDA     $11,X       ; Constant Indexed non-indirect, 8 bits
        LDA     $1111,X     ; Constant Indexed non-indirect, 16 bits
        LDA     ,Y          ; Constant Indexed non-indirect 
        LDA     $1,Y        ; Constant Indexed non-indirect, 5 bits
        LDA     $11,Y       ; Constant Indexed non-indirect, 8 bits
        LDA     $1111,Y     ; Constant Indexed non-indirect, 16 bits
        LDA     ,Y          ; Constant Indexed non-indirect 
        LDA     $1,U        ; Constant Indexed non-indirect, 5 bits
        LDA     $11,U       ; Constant Indexed non-indirect, 8 bits
        LDA     $1111,U     ; Constant Indexed non-indirect, 16 bits
        LDA     ,U          ; Constant Indexed non-indirect 
        LDA     $1,S        ; Constant Indexed non-indirect, 5 bits
        LDA     $11,S       ; Constant Indexed non-indirect, 8 bits
        LDA     $1111,S     ; Constant Indexed non-indirect, 16 bits

        LDA     A,X         ; Accumulator Indexed indirect
        LDA     B,X         ; Accumulator Indexed indirect
        LDA     D,X         ; Accumulator Indexed indirect
        LDA     A,Y         ; Accumulator Indexed indirect
        LDA     B,Y         ; Accumulator Indexed indirect
        LDA     D,Y         ; Accumulator Indexed indirect
        LDA     A,U         ; Accumulator Indexed indirect
        LDA     B,U         ; Accumulator Indexed indirect
        LDA     D,U         ; Accumulator Indexed indirect
        LDA     A,S         ; Accumulator Indexed indirect
        LDA     B,S         ; Accumulator Indexed indirect
        LDA     D,S         ; Accumulator Indexed indirect

        LDA     ,X+         ; Auto increment indexed non-indirect
        LDA     ,X++        ; Auto increment indexed non-indirect
        LDA     ,-X         ; Auto decrement indexed non-indirect
        LDA     ,--X        ; Auto decrement indexed non-indirect
        LDA     ,Y+         ; Auto increment indexed non-indirect
        LDA     ,Y++        ; Auto increment indexed non-indirect
        LDA     ,-Y         ; Auto decrement indexed non-indirect
        LDA     ,--Y        ; Auto decrement indexed non-indirect
        LDA     ,U+         ; Auto increment indexed non-indirect
        LDA     ,U++        ; Auto increment indexed non-indirect
        LDA     ,-U         ; Auto decrement indexed non-indirect
        LDA     ,--U        ; Auto decrement indexed non-indirect
        LDA     ,S+         ; Auto increment indexed non-indirect
        LDA     ,S++        ; Auto increment indexed non-indirect
        LDA     ,-S         ; Auto decrement indexed non-indirect
        LDA     ,--S        ; Auto decrement indexed non-indirect

        LDA     -$1,PCR     ; Constant Offset from PC

        LDA     [,X]        ; Constant Offset from R, Indirect, no offset
        LDA     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LDA     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LDA     [,Y]        ; Constant Offset from R, Indirect, no offset
        LDA     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LDA     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LDA     [,U]        ; Constant Offset from R, Indirect, no offset
        LDA     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LDA     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LDA     [,S]        ; Constant Offset from R, Indirect, no offset
        LDA     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LDA     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LDA     [A,X]       ; Accumulator Indexed indirect
        LDA     [B,X]       ; Accumulator Indexed indirect
        LDA     [D,X]       ; Accumulator Indexed indirect
        LDA     [A,Y]       ; Accumulator Indexed indirect
        LDA     [B,Y]       ; Accumulator Indexed indirect
        LDA     [D,Y]       ; Accumulator Indexed indirect
        LDA     [A,U]       ; Accumulator Indexed indirect
        LDA     [B,U]       ; Accumulator Indexed indirect
        LDA     [D,U]       ; Accumulator Indexed indirect
        LDA     [A,S]       ; Accumulator Indexed indirect
        LDA     [B,S]       ; Accumulator Indexed indirect
        LDA     [D,S]       ; Accumulator Indexed indirect

        LDA     [,X++]      ; Auto increment indexed indirect
        LDA     [,--X]      ; Auto decrement indexed indirect
        LDA     [,Y++]      ; Auto increment indexed indirect
        LDA     [,--Y]      ; Auto decrement indexed indirect
        LDA     [,U++]      ; Auto increment indexed indirect
        LDA     [,--U]      ; Auto decrement indexed indirect
        LDA     [,S++]      ; Auto increment indexed indirect
        LDA     [,--S]      ; Auto decrement indexed indirect
        
        LDA     [-$1,PCR]   ; Constant Offset from PC 

; LDB
        LDB     #$00        ; 0009: C6 00          Immediate
        LDB     MF000       ; 000B: D6 00          Direct
        LDB     $00,X       ; 000D: E6 00          Indexed
        LDB     >MF000      ; 000F: F6 00 00       Extended
        
        LDB     ,X          ; Constant Indexed non-indirect 
        LDB     $1,X        ; Constant Indexed non-indirect
        LDB     $11,X       ; Constant Indexed non-indirect
        LDB     $1111,X     ; Constant Indexed non-indirect
        LDB     ,Y          ; Constant Indexed non-indirect 
        LDB     $1,Y        ; Constant Indexed non-indirect, 5 bits
        LDB     $11,Y       ; Constant Indexed non-indirect, 8 bits
        LDB     $1111,Y     ; Constant Indexed non-indirect, 16 bits
        LDB     ,Y          ; Constant Indexed non-indirect 
        LDB     $1,U        ; Constant Indexed non-indirect, 5 bits
        LDB     $11,U       ; Constant Indexed non-indirect, 8 bits
        LDB     $1111,U     ; Constant Indexed non-indirect, 16 bits
        LDB     ,U          ; Constant Indexed non-indirect 
        LDB     $1,S        ; Constant Indexed non-indirect, 5 bits
        LDB     $11,S       ; Constant Indexed non-indirect, 8 bits
        LDB     $1111,S     ; Constant Indexed non-indirect, 16 bits

        LDB     A,X         ; Accumulator Indexed indirect
        LDB     B,X         ; Accumulator Indexed indirect
        LDB     D,X         ; Accumulator Indexed indirect
        LDB     A,Y         ; Accumulator Indexed indirect
        LDB     B,Y         ; Accumulator Indexed indirect
        LDB     D,Y         ; Accumulator Indexed indirect
        LDB     A,U         ; Accumulator Indexed indirect
        LDB     B,U         ; Accumulator Indexed indirect
        LDB     D,U         ; Accumulator Indexed indirect
        LDB     A,S         ; Accumulator Indexed indirect
        LDB     B,S         ; Accumulator Indexed indirect
        LDB     D,S         ; Accumulator Indexed indirect

        LDB     ,X+         ; Auto increment indexed non-indirect
        LDB     ,X++        ; Auto increment indexed non-indirect
        LDB     ,-X         ; Auto decrement indexed non-indirect
        LDB     ,--X        ; Auto decrement indexed non-indirect
        LDB     ,Y+         ; Auto increment indexed non-indirect
        LDB     ,Y++        ; Auto increment indexed non-indirect
        LDB     ,-Y         ; Auto decrement indexed non-indirect
        LDB     ,--Y        ; Auto decrement indexed non-indirect
        LDB     ,U+         ; Auto increment indexed non-indirect
        LDB     ,U++        ; Auto increment indexed non-indirect
        LDB     ,-U         ; Auto decrement indexed non-indirect
        LDB     ,--U        ; Auto decrement indexed non-indirect
        LDB     ,S+         ; Auto increment indexed non-indirect
        LDB     ,S++        ; Auto increment indexed non-indirect
        LDB     ,-S         ; Auto decrement indexed non-indirect
        LDB     ,--S        ; Auto decrement indexed non-indirect

        LDB     -$1,PCR     ; Constant Offset from PC

        LDB     [,X]        ; Constant Offset from R, Indirect, no offset
        LDB     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LDB     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LDB     [,Y]        ; Constant Offset from R, Indirect, no offset
        LDB     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LDB     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LDB     [,U]        ; Constant Offset from R, Indirect, no offset
        LDB     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LDB     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LDB     [,S]        ; Constant Offset from R, Indirect, no offset
        LDB     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LDB     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LDB     [A,X]       ; Accumulator Indexed indirect
        LDB     [B,X]       ; Accumulator Indexed indirect
        LDB     [D,X]       ; Accumulator Indexed indirect

        LDB     [,X++]      ; Auto increment indexed indirect
        LDB     [,--X]      ; Auto decrement indexed indirect
        LDB     [,Y++]      ; Auto increment indexed indirect
        LDB     [,--Y]      ; Auto decrement indexed indirect
        LDB     [,U++]      ; Auto increment indexed indirect
        LDB     [,--U]      ; Auto decrement indexed indirect
        LDB     [,S++]      ; Auto increment indexed indirect
        LDB     [,--S]      ; Auto decrement indexed indirect

        LDB     [-$1,PCR]   ; Constant Offset from PC 

; LDD
        LDD     #MF000      ; 0012: CC 00 00       Immediate
        LDD     MF000       ; 0015: DC 00          Direct
        LDD     $00,X       ; 0017: EC 00          Indexed
        LDD     >MF000      ; 0019: FC 00 00       Extended

        LDD     ,X          ; Constant Indexed non-indirect 
        LDD     $1,X        ; Constant Indexed non-indirect
        LDD     $11,X       ; Constant Indexed non-indirect
        LDD     $1111,X     ; Constant Indexed non-indirect
        LDD     ,Y          ; Constant Indexed non-indirect 
        LDD     $1,Y        ; Constant Indexed non-indirect, 5 bits
        LDD     $11,Y       ; Constant Indexed non-indirect, 8 bits
        LDD     $1111,Y     ; Constant Indexed non-indirect, 16 bits
        LDD     ,Y          ; Constant Indexed non-indirect 
        LDD     $1,U        ; Constant Indexed non-indirect, 5 bits
        LDD     $11,U       ; Constant Indexed non-indirect, 8 bits
        LDD     $1111,U     ; Constant Indexed non-indirect, 16 bits
        LDD     ,U          ; Constant Indexed non-indirect 
        LDD     $1,S        ; Constant Indexed non-indirect, 5 bits
        LDD     $11,S       ; Constant Indexed non-indirect, 8 bits
        LDD     $1111,S     ; Constant Indexed non-indirect, 16 bits

        LDD     A,X         ; Accumulator Indexed indirect
        LDD     B,X         ; Accumulator Indexed indirect
        LDD     D,X         ; Accumulator Indexed indirect        
        LDD     A,Y         ; Accumulator Indexed indirect
        LDD     B,Y         ; Accumulator Indexed indirect
        LDD     D,Y         ; Accumulator Indexed indirect        
        LDD     A,U         ; Accumulator Indexed indirect
        LDD     B,U         ; Accumulator Indexed indirect
        LDD     D,U         ; Accumulator Indexed indirect        
        LDD     A,S         ; Accumulator Indexed indirect
        LDD     B,S         ; Accumulator Indexed indirect
        LDD     D,S         ; Accumulator Indexed indirect        

        LDD     ,X+         ; Auto increment indexed non-indirect 
        LDD     ,X++        ; Auto increment indexed non-indirect 
        LDD     ,-X         ; Auto decrement indexed non-indirect 
        LDD     ,--X        ; Auto decrement indexed non-indirect 
        LDD     ,Y+         ; Auto increment indexed non-indirect 
        LDD     ,Y++        ; Auto increment indexed non-indirect 
        LDD     ,-Y         ; Auto decrement indexed non-indirect 
        LDD     ,--Y        ; Auto decrement indexed non-indirect 
        LDD     ,U+         ; Auto increment indexed non-indirect 
        LDD     ,U++        ; Auto increment indexed non-indirect 
        LDD     ,-U         ; Auto decrement indexed non-indirect 
        LDD     ,--U        ; Auto decrement indexed non-indirect 
        LDD     ,S+         ; Auto increment indexed non-indirect 
        LDD     ,S++        ; Auto increment indexed non-indirect 
        LDD     ,-S         ; Auto decrement indexed non-indirect 
        LDD     ,--S        ; Auto decrement indexed non-indirect 

        LDD     -$1,PCR     ; Constant Offset from PC

        LDD     [,X]        ; Constant Offset from R, Indirect, no offset
        LDD     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LDD     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LDD     [,Y]        ; Constant Offset from R, Indirect, no offset
        LDD     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LDD     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LDD     [,U]        ; Constant Offset from R, Indirect, no offset
        LDD     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LDD     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LDD     [,S]        ; Constant Offset from R, Indirect, no offset
        LDD     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LDD     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LDD     [A,X]       ; Accumulator Indexed indirect
        LDD     [B,X]       ; Accumulator Indexed indirect
        LDD     [D,X]       ; Accumulator Indexed indirect

        LDD     [,X++]      ; Auto increment indexed indirect
        LDD     [,--X]      ; Auto decrement indexed indirect      
        LDD     [,Y++]      ; Auto increment indexed indirect
        LDD     [,--Y]      ; Auto decrement indexed indirect
        LDD     [,U++]      ; Auto increment indexed indirect
        LDD     [,--U]      ; Auto decrement indexed indirect
        LDD     [,S++]      ; Auto increment indexed indirect
        LDD     [,--S]      ; Auto decrement indexed indirect

        LDD     [-$1,PCR]   ; Constant Offset from PC 

; LDS
        LDS     #MF000      ; 001C: 10 CE 00 00    Immediate
        LDS     MF000       ; 0020: 10 DE 00       Direct
        LDS     $00,X       ; 0023: 10 EE 00       Indexed
        LDS     >MF000      ; 0026: 10 FE 00 00    Extended
        
        LDS     ,X          ; Constant Indexed non-indirect 
        LDS     $1,X        ; Constant Indexed non-indirect
        LDS     $11,X       ; Constant Indexed non-indirect
        LDS     $1111,X     ; Constant Indexed non-indirect
        LDS     ,Y          ; Constant Indexed non-indirect 
        LDS     $1,Y        ; Constant Indexed non-indirect, 5 bits
        LDS     $11,Y       ; Constant Indexed non-indirect, 8 bits
        LDS     $1111,Y     ; Constant Indexed non-indirect, 16 bits
        LDS     ,Y          ; Constant Indexed non-indirect 
        LDS     $1,U        ; Constant Indexed non-indirect, 5 bits
        LDS     $11,U       ; Constant Indexed non-indirect, 8 bits
        LDS     $1111,U     ; Constant Indexed non-indirect, 16 bits
        LDS     ,U          ; Constant Indexed non-indirect 
        LDS     $1,S        ; Constant Indexed non-indirect, 5 bits
        LDS     $11,S       ; Constant Indexed non-indirect, 8 bits
        LDS     $1111,S     ; Constant Indexed non-indirect, 16 bits        
        
        LDS     A,X         ; Accumulator Indexed indirect
        LDS     B,X         ; Accumulator Indexed indirect
        LDS     D,X         ; Accumulator Indexed indirect        
        LDS     A,Y         ; Accumulator Indexed indirect
        LDS     B,Y         ; Accumulator Indexed indirect
        LDS     D,Y         ; Accumulator Indexed indirect        
        LDS     A,U         ; Accumulator Indexed indirect
        LDS     B,U         ; Accumulator Indexed indirect
        LDS     D,U         ; Accumulator Indexed indirect        
        LDS     A,S         ; Accumulator Indexed indirect
        LDS     B,S         ; Accumulator Indexed indirect
        LDS     D,S         ; Accumulator Indexed indirect        
        
        LDS     ,X+         ; Auto increment indexed non-indirect 
        LDS     ,X++        ; Auto increment indexed non-indirect 
        LDS     ,-X         ; Auto decrement indexed non-indirect 
        LDS     ,--X        ; Auto decrement indexed non-indirect 
        LDS     ,Y+         ; Auto increment indexed non-indirect 
        LDS     ,Y++        ; Auto increment indexed non-indirect 
        LDS     ,-Y         ; Auto decrement indexed non-indirect 
        LDS     ,--Y        ; Auto decrement indexed non-indirect 
        LDS     ,U+         ; Auto increment indexed non-indirect 
        LDS     ,U++        ; Auto increment indexed non-indirect 
        LDS     ,-U         ; Auto decrement indexed non-indirect 
        LDS     ,--U        ; Auto decrement indexed non-indirect 
        LDS     ,S+         ; Auto increment indexed non-indirect 
        LDS     ,S++        ; Auto increment indexed non-indirect 
        LDS     ,-S         ; Auto decrement indexed non-indirect 
        LDS     ,--S        ; Auto decrement indexed non-indirect 

        LDS     -$1,PCR     ; Constant Offset from PC

        LDS     [,X]        ; Constant Offset from R, Indirect, no offset
        LDS     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LDS     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LDS     [,Y]        ; Constant Offset from R, Indirect, no offset
        LDS     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LDS     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LDS     [,U]        ; Constant Offset from R, Indirect, no offset
        LDS     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LDS     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LDS     [,S]        ; Constant Offset from R, Indirect, no offset
        LDS     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LDS     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LDS     [A,X]       ; Accumulator Indexed indirect
        LDS     [B,X]       ; Accumulator Indexed indirect
        LDS     [D,X]       ; Accumulator Indexed indirect

        LDS     [,X++]      ; Auto increment indexed indirect
        LDS     [,--X]      ; Auto decrement indexed indirect      
        LDS     [,Y++]      ; Auto increment indexed indirect
        LDS     [,--Y]      ; Auto decrement indexed indirect
        LDS     [,U++]      ; Auto increment indexed indirect
        LDS     [,--U]      ; Auto decrement indexed indirect
        LDS     [,S++]      ; Auto increment indexed indirect
        LDS     [,--S]      ; Auto decrement indexed indirect

        LDS     [-$1,PCR]   ; Constant Offset from PC 

; LDU
        LDU     #MF000      ; 002A: CE 00 00       Immediate
        LDU     MF000       ; 002D: DE 00          Direct
        LDU     $00,X       ; 002F: EE 00          Indexed
        LDU     >MF000      ; 0031: FE 00 00       Extended

        LDU     ,X          ; Constant Indexed non-indirect 
        LDU     $1,X        ; Constant Indexed non-indirect
        LDU     $11,X       ; Constant Indexed non-indirect
        LDU     $1111,X     ; Constant Indexed non-indirect
        LDU     ,Y          ; Constant Indexed non-indirect 
        LDU     $1,Y        ; Constant Indexed non-indirect, 5 bits
        LDU     $11,Y       ; Constant Indexed non-indirect, 8 bits
        LDU     $1111,Y     ; Constant Indexed non-indirect, 16 bits
        LDU     ,Y          ; Constant Indexed non-indirect 
        LDU     $1,U        ; Constant Indexed non-indirect, 5 bits
        LDU     $11,U       ; Constant Indexed non-indirect, 8 bits
        LDU     $1111,U     ; Constant Indexed non-indirect, 16 bits
        LDU     ,U          ; Constant Indexed non-indirect 
        LDU     $1,S        ; Constant Indexed non-indirect, 5 bits
        LDU     $11,S       ; Constant Indexed non-indirect, 8 bits
        LDU     $1111,S     ; Constant Indexed non-indirect, 16 bits

        LDU     A,X         ; Accumulator Indexed indirect
        LDU     B,X         ; Accumulator Indexed indirect
        LDU     D,X         ; Accumulator Indexed indirect        
        LDU     A,Y         ; Accumulator Indexed indirect
        LDU     B,Y         ; Accumulator Indexed indirect
        LDU     D,Y         ; Accumulator Indexed indirect        
        LDU     A,U         ; Accumulator Indexed indirect
        LDU     B,U         ; Accumulator Indexed indirect
        LDU     D,U         ; Accumulator Indexed indirect        
        LDU     A,S         ; Accumulator Indexed indirect
        LDU     B,S         ; Accumulator Indexed indirect
        LDU     D,S         ; Accumulator Indexed indirect        

        LDU     ,X+         ; Auto increment indexed non-indirect 
        LDU     ,X++        ; Auto increment indexed non-indirect 
        LDU     ,-X         ; Auto decrement indexed non-indirect 
        LDU     ,--X        ; Auto decrement indexed non-indirect 
        LDU     ,Y+         ; Auto increment indexed non-indirect 
        LDU     ,Y++        ; Auto increment indexed non-indirect 
        LDU     ,-Y         ; Auto decrement indexed non-indirect 
        LDU     ,--Y        ; Auto decrement indexed non-indirect 
        LDU     ,U+         ; Auto increment indexed non-indirect 
        LDU     ,U++        ; Auto increment indexed non-indirect 
        LDU     ,-U         ; Auto decrement indexed non-indirect 
        LDU     ,--U        ; Auto decrement indexed non-indirect 
        LDU     ,S+         ; Auto increment indexed non-indirect 
        LDU     ,S++        ; Auto increment indexed non-indirect 
        LDU     ,-S         ; Auto decrement indexed non-indirect 
        LDU     ,--S        ; Auto decrement indexed non-indirect 

        LDU     -$1,PCR     ; Constant Offset from PC

        LDU     [,X]        ; Constant Offset from R, Indirect, no offset
        LDU     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LDU     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LDU     [,Y]        ; Constant Offset from R, Indirect, no offset
        LDU     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LDU     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LDU     [,U]        ; Constant Offset from R, Indirect, no offset
        LDU     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LDU     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LDU     [,S]        ; Constant Offset from R, Indirect, no offset
        LDU     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LDU     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LDU     [A,X]       ; Accumulator Indexed indirect
        LDU     [B,X]       ; Accumulator Indexed indirect
        LDU     [D,X]       ; Accumulator Indexed indirect

        LDU     [,X++]      ; Auto increment indexed indirect
        LDU     [,--X]      ; Auto decrement indexed indirect      
        LDU     [,Y++]      ; Auto increment indexed indirect
        LDU     [,--Y]      ; Auto decrement indexed indirect
        LDU     [,U++]      ; Auto increment indexed indirect
        LDU     [,--U]      ; Auto decrement indexed indirect
        LDU     [,S++]      ; Auto increment indexed indirect
        LDU     [,--S]      ; Auto decrement indexed indirect

        LDU     [-$1,PCR]   ; Constant Offset from PC 

;LDX
        LDX     #MF000      ; 0034: 8E 00 00       Immediate
        LDX     MF000       ; 0037: 9E 00          Direct
        LDX     $00,X       ; 0039: AE 00          Indexed
        LDX     >MF000      ; 003B: BE 00 00       Extended

        LDX     ,X          ; Constant Indexed non-indirect 
        LDX     $1,X        ; Constant Indexed non-indirect
        LDX     $11,X       ; Constant Indexed non-indirect
        LDX     $1111,X     ; Constant Indexed non-indirect
        LDX     ,Y          ; Constant Indexed non-indirect 
        LDX     $1,Y        ; Constant Indexed non-indirect, 5 bits
        LDX     $11,Y       ; Constant Indexed non-indirect, 8 bits
        LDX     $1111,Y     ; Constant Indexed non-indirect, 16 bits
        LDX     ,Y          ; Constant Indexed non-indirect 
        LDX     $1,U        ; Constant Indexed non-indirect, 5 bits
        LDX     $11,U       ; Constant Indexed non-indirect, 8 bits
        LDX     $1111,U     ; Constant Indexed non-indirect, 16 bits
        LDX     ,U          ; Constant Indexed non-indirect 
        LDX     $1,S        ; Constant Indexed non-indirect, 5 bits
        LDX     $11,S       ; Constant Indexed non-indirect, 8 bits
        LDX     $1111,S     ; Constant Indexed non-indirect, 16 bits

        LDX     A,X         ; Accumulator Indexed indirect
        LDX     B,X         ; Accumulator Indexed indirect
        LDX     D,X         ; Accumulator Indexed indirect        
        LDX     A,Y         ; Accumulator Indexed indirect
        LDX     B,Y         ; Accumulator Indexed indirect
        LDX     D,Y         ; Accumulator Indexed indirect        
        LDX     A,U         ; Accumulator Indexed indirect
        LDX     B,U         ; Accumulator Indexed indirect
        LDX     D,U         ; Accumulator Indexed indirect        
        LDX     A,S         ; Accumulator Indexed indirect
        LDX     B,S         ; Accumulator Indexed indirect
        LDX     D,S         ; Accumulator Indexed indirect        

        LDX     ,X+         ; Auto increment indexed non-indirect 
        LDX     ,X++        ; Auto increment indexed non-indirect 
        LDX     ,-X         ; Auto decrement indexed non-indirect 
        LDX     ,--X        ; Auto decrement indexed non-indirect 
        LDX     ,Y+         ; Auto increment indexed non-indirect 
        LDX     ,Y++        ; Auto increment indexed non-indirect 
        LDX     ,-Y         ; Auto decrement indexed non-indirect 
        LDX     ,--Y        ; Auto decrement indexed non-indirect 
        LDX     ,U+         ; Auto increment indexed non-indirect 
        LDX     ,U++        ; Auto increment indexed non-indirect 
        LDX     ,-U         ; Auto decrement indexed non-indirect 
        LDX     ,--U        ; Auto decrement indexed non-indirect 
        LDX     ,S+         ; Auto increment indexed non-indirect 
        LDX     ,S++        ; Auto increment indexed non-indirect 
        LDX     ,-S         ; Auto decrement indexed non-indirect 
        LDX     ,--S        ; Auto decrement indexed non-indirect 

        LDX     -$1,PCR     ; Constant Offset from PC

        LDX     [,X]        ; Constant Offset from R, Indirect, no offset
        LDX     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LDX     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LDX     [,Y]        ; Constant Offset from R, Indirect, no offset
        LDX     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LDX     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LDX     [,U]        ; Constant Offset from R, Indirect, no offset
        LDX     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LDX     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LDX     [,S]        ; Constant Offset from R, Indirect, no offset
        LDX     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LDX     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LDX     [A,X]       ; Accumulator Indexed indirect
        LDX     [B,X]       ; Accumulator Indexed indirect
        LDX     [D,X]       ; Accumulator Indexed indirect

        LDX     [,X++]      ; Auto increment indexed indirect
        LDX     [,--X]      ; Auto decrement indexed indirect      
        LDX     [,Y++]      ; Auto increment indexed indirect
        LDX     [,--Y]      ; Auto decrement indexed indirect
        LDX     [,U++]      ; Auto increment indexed indirect
        LDX     [,--U]      ; Auto decrement indexed indirect
        LDX     [,S++]      ; Auto increment indexed indirect
        LDX     [,--S]      ; Auto decrement indexed indirect

        LDX     [-$1,PCR]   ; Constant Offset from PC 

; LDY
        LDY     #MF000      ; 003E: 10 8E 00 00    Immediate
        LDY     MF000       ; 0042: 10 9E 00       Direct
        LDY     $00,X       ; 0045: 10 AE 00       Indexed
        LDY     >MF000      ; 0048: 10 BE 00 00    Extended

        LDY     ,X          ; Constant Indexed non-indirect 
        LDY     $1,X        ; Constant Indexed non-indirect
        LDY     $11,X       ; Constant Indexed non-indirect
        LDY     $1111,X     ; Constant Indexed non-indirect
        LDY     ,Y          ; Constant Indexed non-indirect 
        LDY     $1,Y        ; Constant Indexed non-indirect, 5 bits
        LDY     $11,Y       ; Constant Indexed non-indirect, 8 bits
        LDY     $1111,Y     ; Constant Indexed non-indirect, 16 bits
        LDY     ,Y          ; Constant Indexed non-indirect 
        LDY     $1,U        ; Constant Indexed non-indirect, 5 bits
        LDY     $11,U       ; Constant Indexed non-indirect, 8 bits
        LDY     $1111,U     ; Constant Indexed non-indirect, 16 bits
        LDY     ,U          ; Constant Indexed non-indirect 
        LDY     $1,S        ; Constant Indexed non-indirect, 5 bits
        LDY     $11,S       ; Constant Indexed non-indirect, 8 bits
        LDY     $1111,S     ; Constant Indexed non-indirect, 16 bits

        LDY     A,X         ; Accumulator Indexed indirect
        LDY     B,X         ; Accumulator Indexed indirect
        LDY     D,X         ; Accumulator Indexed indirect        
        LDY     A,Y         ; Accumulator Indexed indirect
        LDY     B,Y         ; Accumulator Indexed indirect
        LDY     D,Y         ; Accumulator Indexed indirect        
        LDY     A,U         ; Accumulator Indexed indirect
        LDY     B,U         ; Accumulator Indexed indirect
        LDY     D,U         ; Accumulator Indexed indirect        
        LDY     A,S         ; Accumulator Indexed indirect
        LDY     B,S         ; Accumulator Indexed indirect
        LDY     D,S         ; Accumulator Indexed indirect        

        LDY     ,X+         ; Auto increment indexed non-indirect 
        LDY     ,X++        ; Auto increment indexed non-indirect 
        LDY     ,-X         ; Auto decrement indexed non-indirect 
        LDY     ,--X        ; Auto decrement indexed non-indirect 
        LDY     ,Y+         ; Auto increment indexed non-indirect 
        LDY     ,Y++        ; Auto increment indexed non-indirect 
        LDY     ,-Y         ; Auto decrement indexed non-indirect 
        LDY     ,--Y        ; Auto decrement indexed non-indirect 
        LDY     ,U+         ; Auto increment indexed non-indirect 
        LDY     ,U++        ; Auto increment indexed non-indirect 
        LDY     ,-U         ; Auto decrement indexed non-indirect 
        LDY     ,--U        ; Auto decrement indexed non-indirect 
        LDY     ,S+         ; Auto increment indexed non-indirect 
        LDY     ,S++        ; Auto increment indexed non-indirect 
        LDY     ,-S         ; Auto decrement indexed non-indirect 
        LDY     ,--S        ; Auto decrement indexed non-indirect 

        LDY     -$1,PCR     ; Constant Offset from PC

        LDY     [,X]        ; Constant Offset from R, Indirect, no offset
        LDY     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LDY     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LDY     [,Y]        ; Constant Offset from R, Indirect, no offset
        LDY     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LDY     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LDY     [,U]        ; Constant Offset from R, Indirect, no offset
        LDY     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LDY     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LDY     [,S]        ; Constant Offset from R, Indirect, no offset
        LDY     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LDY     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LDY     [A,X]       ; Accumulator Indexed indirect
        LDY     [B,X]       ; Accumulator Indexed indirect
        LDY     [D,X]       ; Accumulator Indexed indirect

        LDY     [,X++]      ; Auto increment indexed indirect
        LDY     [,--X]      ; Auto decrement indexed indirect      
        LDY     [,Y++]      ; Auto increment indexed indirect
        LDY     [,--Y]      ; Auto decrement indexed indirect
        LDY     [,U++]      ; Auto increment indexed indirect
        LDY     [,--U]      ; Auto decrement indexed indirect
        LDY     [,S++]      ; Auto increment indexed indirect
        LDY     [,--S]      ; Auto decrement indexed indirect

        LDY     [-$1,PCR]   ; Constant Offset from PC 

; LEAS
        LEAS    ,X          ; Constant Indexed non-indirect 
        LEAS    $1,X        ; Constant Indexed non-indirect
        LEAS    $11,X       ; Constant Indexed non-indirect
        LEAS    $1111,X     ; Constant Indexed non-indirect
        LEAS    ,Y          ; Constant Indexed non-indirect 
        LEAS    $1,Y        ; Constant Indexed non-indirect
        LEAS    $11,Y       ; Constant Indexed non-indirect
        LEAS    $1111,Y     ; Constant Indexed non-indirect
        LEAS    ,U          ; Constant Indexed non-indirect 
        LEAS    $1,U        ; Constant Indexed non-indirect
        LEAS    $11,U       ; Constant Indexed non-indirect
        LEAS    $1111,U     ; Constant Indexed non-indirect
        LEAS    ,S          ; Constant Indexed non-indirect 
        LEAS    $1,S        ; Constant Indexed non-indirect
        LEAS    $11,S       ; Constant Indexed non-indirect
        LEAS    $1111,S     ; Constant Indexed non-indirect
        
        LEAS    A,X         ; Accumulator Indexed indirect
        LEAS    B,X         ; Accumulator Indexed indirect
        LEAS    D,X         ; Accumulator Indexed indirect
        LEAS    A,Y         ; Accumulator Indexed indirect
        LEAS    B,Y         ; Accumulator Indexed indirect
        LEAS    D,Y         ; Accumulator Indexed indirect
        LEAS    A,U         ; Accumulator Indexed indirect
        LEAS    B,U         ; Accumulator Indexed indirect
        LEAS    D,U         ; Accumulator Indexed indirect
        LEAS    A,S         ; Accumulator Indexed indirect
        LEAS    B,S         ; Accumulator Indexed indirect
        LEAS    D,S         ; Accumulator Indexed indirect
        
        LEAS    ,X+         ; Auto increment indexed non-indirect 
        LEAS    ,X++        ; Auto increment indexed non-indirect 
        LEAS    ,-X         ; Auto decrement indexed non-indirect 
        LEAS    ,--X        ; Auto decrement indexed non-indirect 
        LEAS    ,Y+         ; Auto increment indexed non-indirect 
        LEAS    ,Y++        ; Auto increment indexed non-indirect 
        LEAS    ,-Y         ; Auto decrement indexed non-indirect 
        LEAS    ,--Y        ; Auto decrement indexed non-indirect 
        LEAS    ,U+         ; Auto increment indexed non-indirect 
        LEAS    ,U++        ; Auto increment indexed non-indirect 
        LEAS    ,-U         ; Auto decrement indexed non-indirect 
        LEAS    ,--U        ; Auto decrement indexed non-indirect 
        LEAS    ,S+         ; Auto increment indexed non-indirect 
        LEAS    ,S++        ; Auto increment indexed non-indirect 
        LEAS    ,-S         ; Auto decrement indexed non-indirect 
        LEAS    ,--S        ; Auto decrement indexed non-indirect 
        
        LEAS     -$1,PCR     ; Constant Offset from PC

        LEAS     [,X]        ; Constant Offset from R, Indirect, no offset
        LEAS     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LEAS     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LEAS     [,Y]        ; Constant Offset from R, Indirect, no offset
        LEAS     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LEAS     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LEAS     [,U]        ; Constant Offset from R, Indirect, no offset
        LEAS     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LEAS     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LEAS     [,S]        ; Constant Offset from R, Indirect, no offset
        LEAS     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LEAS     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LEAS     [A,X]       ; Accumulator Indexed indirect
        LEAS     [B,X]       ; Accumulator Indexed indirect
        LEAS     [D,X]       ; Accumulator Indexed indirect

        LEAS    [,X++]      ; Auto increment indexed indirect
        LEAS    [,--X]      ; Auto decrement indexed indirect      
        LEAS    [,Y++]      ; Auto increment indexed indirect
        LEAS    [,--Y]      ; Auto decrement indexed indirect
        LEAS    [,U++]      ; Auto increment indexed indirect
        LEAS    [,--U]      ; Auto decrement indexed indirect
        LEAS    [,S++]      ; Auto increment indexed indirect
        LEAS    [,--S]      ; Auto decrement indexed indirect

        LEAS     [-$1,PCR]   ; Constant Offset from PC 
        
; LEAU
        LEAU   ,X          ; Constant Indexed non-indirect 
        LEAU   $1,X        ; Constant Indexed non-indirect
        LEAU   $11,X       ; Constant Indexed non-indirect
        LEAU   $1111,X     ; Constant Indexed non-indirect
        LEAU   ,Y          ; Constant Indexed non-indirect 
        LEAU   $1,Y        ; Constant Indexed non-indirect
        LEAU   $11,Y       ; Constant Indexed non-indirect
        LEAU   $1111,Y     ; Constant Indexed non-indirect
        LEAU   ,U          ; Constant Indexed non-indirect 
        LEAU   $1,U        ; Constant Indexed non-indirect
        LEAU   $11,U       ; Constant Indexed non-indirect
        LEAU   $1111,U     ; Constant Indexed non-indirect
        LEAU   ,S          ; Constant Indexed non-indirect 
        LEAU   $1,S        ; Constant Indexed non-indirect
        LEAU   $11,S       ; Constant Indexed non-indirect
        LEAU   $1111,S     ; Constant Indexed non-indirect
        
        LEAU   A,X         ; Accumulator Indexed indirect
        LEAU   B,X         ; Accumulator Indexed indirect
        LEAU   D,X         ; Accumulator Indexed indirect
        LEAU   A,Y         ; Accumulator Indexed indirect
        LEAU   B,Y         ; Accumulator Indexed indirect
        LEAU   D,Y         ; Accumulator Indexed indirect
        LEAU   A,U         ; Accumulator Indexed indirect
        LEAU   B,U         ; Accumulator Indexed indirect
        LEAU   D,U         ; Accumulator Indexed indirect
        LEAU   A,S         ; Accumulator Indexed indirect
        LEAU   B,S         ; Accumulator Indexed indirect
        LEAU   D,S         ; Accumulator Indexed indirect
        
        LEAU    ,X+         ; Auto increment indexed non-indirect 
        LEAU    ,X++        ; Auto increment indexed non-indirect 
        LEAU    ,-X         ; Auto decrement indexed non-indirect 
        LEAU    ,--X        ; Auto decrement indexed non-indirect 
        LEAU    ,Y+         ; Auto increment indexed non-indirect 
        LEAU    ,Y++        ; Auto increment indexed non-indirect 
        LEAU    ,-Y         ; Auto decrement indexed non-indirect 
        LEAU    ,--Y        ; Auto decrement indexed non-indirect 
        LEAU    ,U+         ; Auto increment indexed non-indirect 
        LEAU    ,U++        ; Auto increment indexed non-indirect 
        LEAU    ,-U         ; Auto decrement indexed non-indirect 
        LEAU    ,--U        ; Auto decrement indexed non-indirect 
        LEAU    ,S+         ; Auto increment indexed non-indirect 
        LEAU    ,S++        ; Auto increment indexed non-indirect 
        LEAU    ,-S         ; Auto decrement indexed non-indirect 
        LEAU    ,--S        ; Auto decrement indexed non-indirect 
        
        LEAU    -$1,PCR     ; Constant Offset from PC

        LEAU    [,X]        ; Constant Offset from R, Indirect, no offset
        LEAU    [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LEAU    [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LEAU    [,Y]        ; Constant Offset from R, Indirect, no offset
        LEAU    [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LEAU    [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LEAU    [,U]        ; Constant Offset from R, Indirect, no offset
        LEAU    [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LEAU    [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LEAU    [,S]        ; Constant Offset from R, Indirect, no offset
        LEAU    [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LEAU    [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LEAU    [A,X]       ; Accumulator Indexed indirect
        LEAU    [B,X]       ; Accumulator Indexed indirect
        LEAU    [D,X]       ; Accumulator Indexed indirect

        LEAU    [,X++]      ; Auto increment indexed indirect
        LEAU    [,--X]      ; Auto decrement indexed indirect      
        LEAU    [,Y++]      ; Auto increment indexed indirect
        LEAU    [,--Y]      ; Auto decrement indexed indirect
        LEAU    [,U++]      ; Auto increment indexed indirect
        LEAU    [,--U]      ; Auto decrement indexed indirect
        LEAU    [,S++]      ; Auto increment indexed indirect
        LEAU    [,--S]      ; Auto decrement indexed indirect

        LEAU    [-$1,PCR]   ; Constant Offset from PC 
        
; LEAX
        LEAX    ,X          ; Constant Indexed non-indirect 
        LEAX    $1,X        ; Constant Indexed non-indirect
        LEAX    $11,X       ; Constant Indexed non-indirect
        LEAX    $1111,X     ; Constant Indexed non-indirect
        LEAX    ,Y          ; Constant Indexed non-indirect 
        LEAX    $1,Y        ; Constant Indexed non-indirect
        LEAX    $11,Y       ; Constant Indexed non-indirect
        LEAX    $1111,Y     ; Constant Indexed non-indirect
        LEAX    ,U          ; Constant Indexed non-indirect 
        LEAX    $1,U        ; Constant Indexed non-indirect
        LEAX    $11,U       ; Constant Indexed non-indirect
        LEAX    $1111,U     ; Constant Indexed non-indirect
        LEAX    ,S          ; Constant Indexed non-indirect 
        LEAX    $1,S        ; Constant Indexed non-indirect
        LEAX    $11,S       ; Constant Indexed non-indirect
        LEAX    $1111,S     ; Constant Indexed non-indirect
        
        LEAX    A,X         ; Accumulator Indexed indirect
        LEAX    B,X         ; Accumulator Indexed indirect
        LEAX    D,X         ; Accumulator Indexed indirect
        LEAX    A,Y         ; Accumulator Indexed indirect
        LEAX    B,Y         ; Accumulator Indexed indirect
        LEAX    D,Y         ; Accumulator Indexed indirect
        LEAX    A,U         ; Accumulator Indexed indirect
        LEAX    B,U         ; Accumulator Indexed indirect
        LEAX    D,U         ; Accumulator Indexed indirect
        LEAX    A,S         ; Accumulator Indexed indirect
        LEAX    B,S         ; Accumulator Indexed indirect
        LEAX    D,S         ; Accumulator Indexed indirect
        
        LEAX    ,X+         ; Auto increment indexed non-indirect 
        LEAX    ,X++        ; Auto increment indexed non-indirect 
        LEAX    ,-X         ; Auto decrement indexed non-indirect 
        LEAX    ,--X        ; Auto decrement indexed non-indirect 
        LEAX    ,Y+         ; Auto increment indexed non-indirect 
        LEAX    ,Y++        ; Auto increment indexed non-indirect 
        LEAX    ,-Y         ; Auto decrement indexed non-indirect 
        LEAX    ,--Y        ; Auto decrement indexed non-indirect 
        LEAX    ,U+         ; Auto increment indexed non-indirect 
        LEAX    ,U++        ; Auto increment indexed non-indirect 
        LEAX    ,-U         ; Auto decrement indexed non-indirect 
        LEAX    ,--U        ; Auto decrement indexed non-indirect 
        LEAX    ,S+         ; Auto increment indexed non-indirect 
        LEAX    ,S++        ; Auto increment indexed non-indirect 
        LEAX    ,-S         ; Auto decrement indexed non-indirect 
        LEAX    ,--S        ; Auto decrement indexed non-indirect 
        
        LEAX     -$1,PCR     ; Constant Offset from PC
        
        LEAX     [,X]        ; Constant Offset from R, Indirect, no offset
        LEAX     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LEAX     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LEAX     [,Y]        ; Constant Offset from R, Indirect, no offset
        LEAX     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LEAX     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LEAX     [,U]        ; Constant Offset from R, Indirect, no offset
        LEAX     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LEAX     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LEAX     [,S]        ; Constant Offset from R, Indirect, no offset
        LEAX     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LEAX     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits

        LEAX     [A,X]       ; Accumulator Indexed indirect
        LEAX     [B,X]       ; Accumulator Indexed indirect
        LEAX     [D,X]       ; Accumulator Indexed indirect

        LEAX    [,X++]      ; Auto increment indexed indirect
        LEAX    [,--X]      ; Auto decrement indexed indirect      
        LEAX    [,Y++]      ; Auto increment indexed indirect
        LEAX    [,--Y]      ; Auto decrement indexed indirect
        LEAX    [,U++]      ; Auto increment indexed indirect
        LEAX    [,--U]      ; Auto decrement indexed indirect
        LEAX    [,S++]      ; Auto increment indexed indirect
        LEAX    [,--S]      ; Auto decrement indexed indirect

        LEAX     [-$1,PCR]   ; Constant Offset from PC 

; LEAY
        LEAY    ,X          ; Constant Indexed non-indirect 
        LEAY    $1,X        ; Constant Indexed non-indirect
        LEAY    $11,X       ; Constant Indexed non-indirect
        LEAY    $1111,X     ; Constant Indexed non-indirect
        LEAY    ,Y          ; Constant Indexed non-indirect 
        LEAY    $1,Y        ; Constant Indexed non-indirect
        LEAY    $11,Y       ; Constant Indexed non-indirect
        LEAY    $1111,Y     ; Constant Indexed non-indirect
        LEAY    ,U          ; Constant Indexed non-indirect 
        LEAY    $1,U        ; Constant Indexed non-indirect
        LEAY    $11,U       ; Constant Indexed non-indirect
        LEAY    $1111,U     ; Constant Indexed non-indirect
        LEAY    ,S          ; Constant Indexed non-indirect 
        LEAY    $1,S        ; Constant Indexed non-indirect
        LEAY    $11,S       ; Constant Indexed non-indirect
        LEAY    $1111,S     ; Constant Indexed non-indirect
        
        LEAY    A,X         ; Accumulator Indexed indirect
        LEAY    B,X         ; Accumulator Indexed indirect
        LEAY    D,X         ; Accumulator Indexed indirect
        LEAY    A,Y         ; Accumulator Indexed indirect
        LEAY    B,Y         ; Accumulator Indexed indirect
        LEAY    D,Y         ; Accumulator Indexed indirect
        LEAY    A,U         ; Accumulator Indexed indirect
        LEAY    B,U         ; Accumulator Indexed indirect
        LEAY    D,U         ; Accumulator Indexed indirect
        LEAY    A,S         ; Accumulator Indexed indirect
        LEAY    B,S         ; Accumulator Indexed indirect
        LEAY    D,S         ; Accumulator Indexed indirect
        
        LEAY    ,X+         ; Auto increment indexed non-indirect 
        LEAY    ,X++        ; Auto increment indexed non-indirect 
        LEAY    ,-X         ; Auto decrement indexed non-indirect 
        LEAY    ,--X        ; Auto decrement indexed non-indirect 
        LEAY    ,Y+         ; Auto increment indexed non-indirect 
        LEAY    ,Y++        ; Auto increment indexed non-indirect 
        LEAY    ,-Y         ; Auto decrement indexed non-indirect 
        LEAY    ,--Y        ; Auto decrement indexed non-indirect 
        LEAY    ,U+         ; Auto increment indexed non-indirect 
        LEAY    ,U++        ; Auto increment indexed non-indirect 
        LEAY    ,-U         ; Auto decrement indexed non-indirect 
        LEAY    ,--U        ; Auto decrement indexed non-indirect 
        LEAY    ,S+         ; Auto increment indexed non-indirect 
        LEAY    ,S++        ; Auto increment indexed non-indirect 
        LEAY    ,-S         ; Auto decrement indexed non-indirect 
        LEAY    ,--S        ; Auto decrement indexed non-indirect 
        
        LEAY     -$1,PCR     ; Constant Offset from PC

        LEAY     [,X]        ; Constant Offset from R, Indirect, no offset
        LEAY     [$1,X]      ; Constant Offset from R, Indirect, 8 bits 
        LEAY     [$1111,X]   ; Constant Offset from R, Indirect, 16 bits
        LEAY     [,Y]        ; Constant Offset from R, Indirect, no offset
        LEAY     [$1,Y]      ; Constant Offset from R, Indirect, 8 bits 
        LEAY     [$1111,Y]   ; Constant Offset from R, Indirect, 16 bits
        LEAY     [,U]        ; Constant Offset from R, Indirect, no offset
        LEAY     [$1,U]      ; Constant Offset from R, Indirect, 8 bits 
        LEAY     [$1111,U]   ; Constant Offset from R, Indirect, 16 bits
        LEAY     [,S]        ; Constant Offset from R, Indirect, no offset
        LEAY     [$1,S]      ; Constant Offset from R, Indirect, 8 bits 
        LEAY     [$1111,S]   ; Constant Offset from R, Indirect, 16 bits
        
        LEAY     [A,X]       ; Accumulator Indexed indirect
        LEAY     [B,X]       ; Accumulator Indexed indirect
        LEAY     [D,X]       ; Accumulator Indexed indirect

        LEAY    [,X++]      ; Auto increment indexed indirect
        LEAY    [,--X]      ; Auto decrement indexed indirect      
        LEAY    [,Y++]      ; Auto increment indexed indirect
        LEAY    [,--Y]      ; Auto decrement indexed indirect
        LEAY    [,U++]      ; Auto increment indexed indirect
        LEAY    [,--U]      ; Auto decrement indexed indirect
        LEAY    [,S++]      ; Auto increment indexed indirect
        LEAY    [,--S]      ; Auto decrement indexed indirect

        LEAY     [-$1,PCR]   ; Constant Offset from PC 
        

