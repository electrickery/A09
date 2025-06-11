   ; Source: https://www.jaapsch.net/psion/mcmnemal.htm
   
        OPT     H03 
   
Z10     EQU     $10
Z3210   EQU     $3210

M8      EQU     $23
M16     EQU     $0123

D45     EQU     $45
   
        ORG $FE00

        ABA       
        ABX       

        ADCA #Z10    
        ADCA M8   
        ADCA D45,X  
        ADCA M16   
        ADCB #Z10      
        ADCB M8   
        ADCB D45,X  
        ADCB M16   
   
        ADDA #Z10      
        ADDA M8   
        ADDA D45,X  
        ADDA M16   
        ADDB #Z10      
        ADDB M8   
        ADDB D45,X  
        ADDB M16   
        ADDD #Z10  
        ADDD M8   
        ADDD D45,X  
        ADDD M16   

        AIM #Z10,M8  
        AIM #Z10,D45,X 

        ANDA #Z10    
        ANDA M8   
        ANDA D45,X  
        ANDA M16   
        ANDB #Z10    
        ANDB M8   
        ANDB D45,X  
        ANDB M16   

        ASL D45,X   
        ASL M16    
        ASLA      
        ASLB      
        ASLD      

        ASR D45,X   
        ASR M16    
        ASRA      
        ASRB      

        BITA #Z10    
        BITA M8   
        BITA D45,X  
        BITA M16   
        BITB #Z10    
        BITB M8   
        BITB D45,X  
        BITB M16   
   
R8:
        BRA R8
        BRN R8
        BHI R8
        BLS R8
        BCC R8
        BCS R8
        BNE R8
        BEQ R8
        BVC R8
        BVS R8
        BPL R8
        BMI R8
        BGE R8
        BLT R8
        BGT R8
        BLE R8

        BSR R8

        CBA       
        CMPA #Z10    
        CMPA M8   
        CMPA D45,X  
        CMPA M16   
        CMPB #Z10    
        CMPB M8   
        CMPB D45,X  
        CMPB M16   
        CPX #Z3210    
        CPX M8    
        CPX D45,X   
        CPX M16    

        CLC       
        CLI       
        CLV       

        CLR D45,X   
        CLR M16    
        CLRA      
        CLRB      

        COM D45,X   
        COM M16    
        COMA      
        COMB      

        DAA       

        DEC D45,X   
        DEC M16    
        DECA      
        DECB      
        DES       
        DEX       

        EIM #Z10,M8  
        EIM #Z10,D45,X 

        EORA #Z10    
        EORA M8   
        EORA D45,X  
        EORA M16   
        EORB #Z10    
        EORB M8   
        EORB D45,X  
        EORB M16   

        INC D45,X   
        INC M16    
        INCA      
        INCB      
        INS       
        INX       

        JMP D45,X   
        JMP M16    

        JSR M8    
        JSR D45,X   
        JSR M16    

        LDAA #Z10    
        LDAA M8   
        LDAA D45,X  
        LDAA M16   
        LDAB #Z10    
        LDAB M8   
        LDAB D45,X  
        LDAB M16   
        LDD #Z3210  
        LDD M8    
        LDD D45,X   
        LDD M16    
        LDS #Z3210  
        LDS M8    
        LDS D45,X   
        LDS M16    
        LDX #Z3210  
        LDX M8    
        LDX D45,X   
        LDX M16    

        LSR D45,X   
        LSR M16    
        LSRA      
        LSRB      
        LSRD      

        MUL       

        NEG D45,X   
        NEG M16    
        NEGA      
        NEGB      

        NOP       

        OIM #Z10,M8  
        OIM #Z10,D45,X 

        ORAA #Z10    
        ORAA M8   
        ORAA D45,X  
        ORAA M16   
        ORAB #Z10    
        ORAB M8   
        ORAB D45,X  
        ORAB M16   

        PSHA      
        PSHB      
        PSHX      

        PULA      
        PULB      
        PULX      

        ROL D45,X   
        ROL M16    
        ROLA      
        ROLB      

        ROR D45,X   
        ROR M16    
        RORA      
        RORB      

        RTI       
        RTS       

        SBA       

        SBCA #Z10    
        SBCA M8   
        SBCA D45,X  
        SBCA M16   
        SBCB #Z10    
        SBCB M8   
        SBCB D45,X  
        SBCB M16   

        SEC       
        SEI       
        SEV       

        SLP       

        STAA M8   
        STAA D45,X  
        STAA M16   
        STAB M8   
        STAB D45,X  
        STAB M16   
        STD M8    
        STD D45,X   
        STD M16    
        STS M8    
        STS D45,X   
        STS M16    
        STX M8    
        STX D45,X   
        STX M16    

        SUBA #Z10    
        SUBA M8   
        SUBA D45,X  
        SUBA M16   
        SUBB #Z10    
        SUBB M8   
        SUBB D45,X  
        SUBB M16   
        SUBD #Z3210  
        SUBD M8   
        SUBD D45,X  
        SUBD M16   

        SWI       

        TAB       
        TAP       
        TBA       
        TPA       
        TSX       
        TXS       

        TIM #Z10,M8  
        TIM #Z10,D45,X 

        ;    TRAP      

        TST D45,X   
        TST M16    
        TSTA      
        TSTB      

        WAI       
        XGDX      

        END
