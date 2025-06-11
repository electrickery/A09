; Source: HD63B09EP Technical Reference Guide
;         THE 6309 BOOK, Chris Burke

        OPT     h63

        ORG     $E000
        
        ADCD    #$0123          ; Immed.
        ADCD    $01             ; Direct DP-page
        ADCD    [$0123]         ; Indexed
        ADCD    >$0123          ; Extended

        ADDE    #$0123          ; Immed.
        ADDE    $01             ; Direct DP-page
        ADDE    [$0123]         ; Indexed
        ADDE    >$0123          ; Extended
        
        ADDF    #$0123          ; Immed.
        ADDF    $01             ; Direct DP-page
        ADDF    [$0123]         ; Indexed
        ADDF    >$0123          ; Extended
        
        ADDW    #$0123          ; Immed.
        ADDW    $01             ; Direct DP-page
        ADDW    [$0123]         ; Indexed
        ADDW    >$0123          ; Extended
    
;        AIM    $01              ; Direct DP-page
;        AIM    [$0123]          ; Indexed
;        AIM    >$0123           ; Extended

        ASLD                    ; Inherent

        ASRD                    ; Inherent
        
        CLRD                    ; Inherent
        CLRE                    ; Inherent
        CLRF                    ; Inherent
        CLRW                    ; Inherent

        CMPE    #$0123          ; Immed.
        CMPE    $01             ; Direct DP-page
        CMPE    [$0123]         ; Indexed
        CMPE    >$0123          ; Extended

        CMPF    #$0123          ; Immed.
        CMPF    $01             ; Direct DP-page
        CMPF    [$0123]         ; Indexed
        CMPF    >$0123          ; Extended

        CMPW    #$0123          ; Immed.
        CMPW    $01             ; Direct DP-page
        CMPW    [$0123]         ; Indexed
        CMPW    >$0123          ; Extended

        COMD                    ; Inherent
        COME                    ; Inherent
        COMF                    ; Inherent
        COMW                    ; Inherent
        
        DECD                    ; Inherent
        DECE                    ; Inherent
        DECF                    ; Inherent
        DECW                    ; Inherent
       
        DIVD    #$0123          ; Immed.
        DIVD    $01             ; Direct DP-page
        DIVD    [$0123]         ; Indexed
        DIVD    >$0123          ; Extended

        DIVQ    #$0123          ; Immed.
        DIVQ    $01             ; Direct DP-page
        DIVQ    [$0123]         ; Indexed
        DIVQ    >$0123          ; Extended

;        EIM     $01,4 U             ; Direct DP-page
;        EIM     [$0123]         ; Indexed
;        EIM     >$0123          ; Extended

        EORD    #$0123          ; Immed.
        EORD    $01             ; Direct DP-page
        EORD    [$0123]         ; Indexed
        EORD    >$0123          ; Extended

        INCD                    ; Inherent
        INCE                    ; Inherent
        INCF                    ; Inherent
        INCW                    ; Inherent
        
        LDE     #$0123          ; Immed.
        LDE     $01             ; Direct DP-page
        LDE     [$0123]         ; Indexed
        LDE     >$0123          ; Extended
        
        LDF     #$0123          ; Immed.
        LDF     $01             ; Direct DP-page
        LDF     [$0123]         ; Indexed
        LDF     >$0123          ; Extended
        
        LDQ     #$0123          ; Immed.
        LDQ     $01             ; Direct DP-page
        LDQ     [$0123]         ; Indexed
        LDQ     >$0123          ; Extended
        
        LDW     #$0123          ; Immed.
        LDW     $01             ; Direct DP-page
        LDW     [$0123]         ; Indexed
        LDW     >$0123          ; Extended
        
        LDMD    #$0123          ; Immed.
        LDMD    $01             ; Direct DP-page   ????
;        LDMD   [$0123]         ; Indexed
        LDMD    >$0123          ; Extended         ????

        LSRD                    ; Inherent

        LSRW                    ; Inherent
        
        MULD    #$0123          ; Immed.
        MULD    $01             ; Direct DP-page
        MULD    [$0123]         ; Indexed
        MULD    >$0123          ; Extended

        NEGD                    ; Inherent
        
;        OIM     $01             ; Direct DP-page
;        OIM     [$0123]         ; Indexed
;        OIM     >$0123          ; Extended
        
        ORD     #$0123          ; Immed.
        ORD     $01             ; Direct DP-page
        ORD     [$0123]         ; Indexed
        ORD     >$0123          ; Extended

        PSHSW   #$0123          ; Immed.

        PSHUW   #$0123          ; Immed.
        
        PULSW   #$0123          ; Immed.
        
        PULUW   #$0123          ; Immed.

        ROLD    #$0123          ; Immed.

        ROLW    #$0123          ; Immed.

        RORD    #$0123          ; Immed.

        RORW    #$0123          ; Immed.

        SBCD    #$0123          ; Immed.
        SBCD    $01             ; Direct DP-page
        SBCD    [$0123]         ; Indexed
        SBCD    >$0123          ; Extended
        
        SEXW                    ; Inherent
        
        STE     $01             ; Direct DP-page
        STE     [$0123]         ; Indexed
        STE     >$0123          ; Extended
        
        STF     $01             ; Direct DP-page
        STF     [$0123]         ; Indexed
        STF     >$0123          ; Extended
        
        STQ     $01             ; Direct DP-page
        STQ     [$0123]         ; Indexed
        STQ     >$0123          ; Extended
        
        STS     $01             ; Direct DP-page
        STS     [$0123]         ; Indexed
        STS     >$0123          ; Extended
        
        STW     $01             ; Direct DP-page
        STW     [$0123]         ; Indexed
        STW     >$0123          ; Extended

        SUBE    #$0123          ; Immed.
        SUBE    $01             ; Direct DP-page
        SUBE    [$0123]         ; Indexed
        SUBE    >$0123          ; Extended

        SUBF    #$0123          ; Immed.
        SUBF    $01             ; Direct DP-page
        SUBF    [$0123]         ; Indexed
        SUBF    >$0123          ; Extended

;        SUBW    #$0123          ; Immed.
;        SUBW    $01             ; Direct DP-page
;        SUBW    [$0123]         ; Indexed
;        SUBW    >$0123          ; Extended

        TIM     #$3F,4,U
;        TIM     >$23,4,U
;        TIM     $01             ; Direct DP-page
;        TIM     [$0123]         ; Indexed
;        TIM     >$0123          ; Extended

        TSTD                    ; Inherent
        TSTE                    ; Inherent
        TSTF                    ; Inherent
        TSTW                    ; Inherent
        
        ; operands: destination register, source bit, destination bit, source address.
        BAND    A, 0, 4, $23    ; Immed.   
        
        BIAND   A, 0, 4, $23    ; Immed.
        
        BOR     A, 0, 4, $23    ; Immed.
        
        BIOR    A, 0, 4, $23    ; Immed.
        
        BEOR    A, 0, 4, $23    ; Immed.
        
        BIEOR    A, 0, 4, $23   ; Immed.
        
;        LDBT    #$23          ; Immed.
        
;        STBT    $23          ; Immed.
