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
    
        AIM    #$3F,4,U         ; Direct DP-page
        AIM    #$7F,<$0123      ; Indexed
        AIM    #$10,$20,U       ; Extended
        
        ANDR    D,X             ; Immed.
        ANDR    Y,U             ; Immed.
        ANDR    S,PC            ; Immed.
        ANDR    A,B             ; Immed.
        ANDR    CC,DP           ; Immed.
        ANDR    X,D             ; Immed.

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

        EIM    #$3F,4,U         ; Direct DP-page
        EIM    #$7F,<$0123      ; Indexed
        EIM    #$10,$20,U       ; Extended

        EORD    #$0123          ; Immed.
        EORD    $01             ; Direct DP-page
        EORD    [$0123]         ; Indexed
        EORD    >$0123          ; Extended
        
        EORR    D,X             ; Immed.
        EORR    Y,U             ; Immed.
        EORR    S,PC            ; Immed.
        EORR    A,B             ; Immed.
        EORR    CC,DP           ; Immed.
        EORR    X,D             ; Immed.
        
        EXG     D,X             ; Immed.
        EXG     Y,U             ; Immed.
        EXG     S,PC            ; Immed.
        EXG     A,B             ; Immed.
        EXG     CC,DP           ; Immed.
        EXG     X,D             ; Immed.

        INCD                    ; Inherent
        INCE                    ; Inherent
        INCF                    ; Inherent
        INCW                    ; Inherent
        
        LDD    [,X+]            ; Post-Inc Indirect
        LDD    [,Y+]            ; Post-Inc Indirect
        LDY    [,X+]            ; Post-Inc Indirect
        LDY    [,Y+]            ; Post-Inc Indirect
        LDD    [,W++]           ; Post-Inc Indirect
        LDD    [,X++]           ; Post-Inc Indirect
        LDD    [,Y++]           ; Post-Inc Indirect
        LDY    [,X++]           ; Post-Inc Indirect
        LDY    [,Y++]           ; Post-Inc Indirect
        LDD    [,-X]            ; Post-Inc Indirect
        LDD    [,-Y]            ; Post-Inc Indirect
        LDY    [,-X]            ; Post-Inc Indirect
        LDY    [,-Y]            ; Post-Inc Indirect
        LDD    [,--W]           ; Post-Inc Indirect
        LDD    [,--X]           ; Post-Inc Indirect
        LDD    [,--Y]           ; Post-Inc Indirect
        LDY    [,--X]           ; Post-Inc Indirect
        LDY    [,--Y]           ; Post-Inc Indirect
        
        LDE     #$0123          ; Immed.
        LDE     $01             ; Direct DP-page
        LDE     [$0123]         ; Indexed
        LDE     >$0123          ; Extended
        
        LDF     #$0123          ; Immed.
        LDF     $01             ; Direct DP-page
        LDF     [$0123]         ; Indexed
        LDF     >$0123          ; Extended
        
        LDQ     #$01234567          ; Immed.
        LDQ     $01             ; Direct DP-page
        LDQ     [$0123]         ; Indexed
        LDQ     >$0123          ; Extended
        
        
        LDW     [,X+]           ; Post-Inc Indirect
        LDW     [,Y+]           ; Post-Inc Indirect
        LDW     [,W++]          ; Post-Inc Indirect
        LDW     [,X++]          ; Post-Inc Indirect
        LDW     [,Y++]          ; Post-Inc Indirect
        LDW     [,-X]           ; Post-Inc Indirect
        LDW     [,-Y]           ; Post-Inc Indirect
        LDW     [,--W]          ; Post-Inc Indirect
        LDW     [,--X]          ; Post-Inc Indirect
        LDW     [,--Y]          ; Post-Inc Indirect
        
        LDW     #$0123          ; Immed.
        LDW     $01             ; Direct DP-page
        LDW     [$0123]         ; Indexed
        LDW     >$0123          ; Extended
        
        LDMD    #$0123          ; Immed.
        LDMD    $01             ; Direct DP-page   ????
;        LDMD   [$0123]         ; Indexed
        LDMD    >$0123          ; Extended         ????
        
        LDQ     #$0123          ; Immed.
        LDQ     $01             ; Direct DP-page
        LDQ     [$0123]         ; Indexed
        LDQ     >$0123          ; Extended
        
        LSLD                    ; Inherent
        
        LSRD                    ; Inherent

        LSRW                    ; Inherent
        
        MULD    #$0123          ; Immed.
        MULD    $01             ; Direct DP-page
        MULD    [$0123]         ; Indexed
        MULD    >$0123          ; Extended

        NEGD                    ; Inherent
        
        ; 
        OIM    #$3F,4,U         ; Direct DP-page
        OIM    #$7F,<$0123      ; Indexed
        OIM    #$10,$20,U       ; Extended

        
        ORD     #$0123          ; Immed.
        ORD     $01             ; Direct DP-page
        ORD     [$0123]         ; Indexed
        ORD     >$0123          ; Extended
        
        ORR     D,X             ; Immed.
        ORR     Y,U             ; Immed.
        ORR     S,PC            ; Immed.
        ORR     A,B             ; Immed.
        ORR     CC,DP           ; Immed.
        ORR     X,D             ; Immed.
        

        PSHSW                   ; Inherent

        PSHUW                   ; Inherent
        
        PULSW                   ; Inherent
        
        PULUW                   ; Inherent

        ROLD    #$0123          ; Inherent

        ROLW    #$0123          ; Inherent

        RORD    #$0123          ; Inherent

        RORW    #$0123          ; Inherent

        SBCD    #$0123          ; Immed.
        SBCD    $01             ; Direct DP-page
        SBCD    [$0123]         ; Indexed
        SBCD    >$0123          ; Extended
        
        SBCR    D,X             ; Immed.
        SBCR    Y,U             ; Immed.
        SBCR    S,PC            ; Immed.
        SBCR    A,B             ; Immed.
        SBCR    CC,DP           ; Immed.
        SBCR    X,D             ; Immed.
        
        SEXW                    ; Inherent
        
        STBT    A,4,5,$3F        ; Direct DP-page
        
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

        SUBR    D,X             ; Immed.
        SUBR    Y,U             ; Immed.
        SUBR    S,PC            ; Immed.
        SUBR    A,B             ; Immed.
        SUBR    CC,DP           ; Immed.
        SUBR    X,D             ; Immed.

        TFM     X+,Y+           ; Immed.
        TFM     U-,S-           ; Immed.
        TFM     D+,X            ; Immed.
        TFM     Y,U+            ; Immed.

        TFR     D,X             ; Immed.
        TFR     Y,U             ; Immed.
        TFR     S,PC            ; Immed.
        TFR     A,B             ; Immed.
        TFR     CC,DP           ; Immed.
        TFR     X,D             ; Immed.

        TIM    #$3F,4,U         ; Direct DP-page
        TIM    #$7F,<$0123      ; Indexed
        TIM    #$10,$20,U       ; Extended


        TSTD                    ; Inherent
        TSTE                    ; Inherent
        TSTF                    ; Inherent
        TSTW                    ; Inherent
        
        ; operands: destination register, source bit, destination bit, source address.
        BAND    A,0,4,$23       ; Immed.   

        BIAND   A,0,4,$23       ; Immed.

        BOR     A,0,4,$23       ; Immed.

        BIOR    A,0,4,$23       ; Immed.

        BEOR    A,0,4,$23       ; Immed.

        BIEOR   A,0,4,$23       ; Immed.

        LDBT    A,5,1,$40       ; Immed.

        STBT    A,5,1,$40       ; Immed.
