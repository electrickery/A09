; Source: www.flexusergroup.com/flexusergroup/pdfs/asmb.pdf

        ORG     $f000
        
        SETDP   $D0
        
        PAG     
        
        RMB     $0010
        
        SPC     4
        
        FCB     $01, $23, $45, $67, $89, $AB, $CD, $EF 
        
        NAM     This is the title or TTL     
          
        FDB     $0123, $4567, $89AB, $CDEF
        
        STTL    This is the sub-title
        
        FCC     'Any text'
        
;ERRMSG  ERR     Custom error message
        
LABEL:  EQU     $00

        RPT     3

        NOP

SETL:	SET    $0123

        LIB    include.txt

SETL:	SET    $4567

RLIST2  REG     A,B,Y,U,DP

        OPT     PAG
;        PAG enable page formatting and numbering 
;        NOP* disable pagination 
;        CON print conditionally skipped code 
;        NOC* suppress conditional code printing 
;        MAC* print macro calling lines 
;        NOM suppress printing of macro calls 
;        EXP print macro expansion lines 
;        NOE* Suppress macro expansion printing


        END     

POSTEND:        NOP     ; This code should be ignored
