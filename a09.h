
/* @see https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32
   or http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system */
#if !defined(_WIN32) && !defined(_WIN64) && \
    (defined(__unix__) || defined(__unix) || \
     defined(__linux__) || \
    (defined(__APPLE__) && defined(__MACH__)))
#define UNIX 1                          /* UNIX specials                     */
#else
#define UNIX 0                          /* Windows-specific                  */
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#if UNIX
#define stricmp strcasecmp
#include <unistd.h>
#else
#include <malloc.h>
#endif

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/

#define VERSION      "1.62"
#define VERSNUM      "$013E"            /* can be queried as &VERSION        */
#define RMBDEFCHR    "$00"

#define MAXFILES     128
#define MAXLABELS    8192
#define MAXMACROS    1024
#define MAXTEXTS     1024
#define MAXRELOCS    32768
#define MAXIDLEN     32
#define MAXLISTBYTES 7
#define FNLEN        256
#define LINELEN      1024

/*****************************************************************************/
/* Line buffer definitions                                                   */
/*****************************************************************************/

struct linebuf
  {
  struct linebuf * next;                /* pointer to next line              */
  struct linebuf * prev;                /* pointer to previous line          */
  char *fn;                             /* pointer to original file name     */
  long ln;                              /* line number therein               */
  unsigned char lvl;                    /* line level                        */
  unsigned char rel;                    /* relocation mode                   */
  unsigned char flg;                    /* flags                             */
  char txt[1];                          /* text buffer                       */
  };

char *fnms[MAXFILES] = {0};             /* process up to N different files   */
short nfnms;                            /* # loaded files                    */

                                        /* flag definitions :                */
#define LINCAT_PEMTCMT      0x10        /* prepend comment char in listing   */
#define LINCAT_MACDEF       0x20        /* macro definition                  */
#define LINCAT_MACEXP       0x40        /* macro expansion                   */
#define LINCAT_MACINV       (0x20|0x40) /* macro invocation                  */
#define LINCAT_INVISIBLE    0x80        /* does not appear in listing        */

#define LINCAT_LVLMASK      0x1F        /* mask for line levels (0..31)      */

                                        /* Helpers for the above             */
#define LINE_IS_PEMTCMT(flg) ((flg & LINCAT_PEMTCMT) == LINCAT_PEMTCMT)
#define LINE_IS_MACDEF(flg) ((flg & LINCAT_MACINV) == LINCAT_MACDEF)
#define LINE_IS_MACEXP(flg) ((flg & LINCAT_MACINV) == LINCAT_MACEXP)
#define LINE_IS_MACINV(flg) ((flg & LINCAT_MACINV) == LINCAT_MACINV)
#define LINE_IS_INVISIBLE(flg) (flg & LINCAT_INVISIBLE)

struct linebuf *rootline = NULL;        /* pointer to 1st line of the file   */
struct linebuf *curline = NULL;         /* pointer to currently processed ln */

/*****************************************************************************/
/* Opcode definitions                                                        */
/*****************************************************************************/

struct oprecord
  {
  char * name;                          /* opcode mnemonic                   */
  unsigned short cat;                   /* opcode category                   */
  unsigned long code;                   /* category-dependent additional code*/
  };

                                        /* Instruction categories :          */
#define OPCAT_ONEBYTE        0x0000     /* one byte opcodes             NOP  */
#define OPCAT_TWOBYTE        0x0001     /* two byte opcodes             SWI2 */
#define OPCAT_THREEBYTE      0x0002     /* three byte opcodes            TAB */
#define OPCAT_FOURBYTE       0x0003     /* four byte opcodes             ABA */
#define OPCAT_IMMBYTE        0x0004     /* opcodes w. imm byte         ANDCC */
#define OPCAT_LEA            0x0005     /* load effective address       LEAX */
#define OPCAT_SBRANCH        0x0006     /* short branches               BGE  */
#define OPCAT_LBR2BYTE       0x0007     /* long branches 2 byte opc     LBGE */
#define OPCAT_LBR1BYTE       0x0008     /* long branches 2 byte opc     LBRA */
#define OPCAT_ARITH          0x0009     /* accumulator instr.           ADDA */
#define OPCAT_DBLREG1BYTE    0x000a     /* double reg instr 1 byte opc  LDX  */
#define OPCAT_DBLREG2BYTE    0x000b     /* double reg instr 2 byte opc  LDY  */
#define OPCAT_SINGLEADDR     0x000c     /* single address instrs        NEG  */
#define OPCAT_2REG           0x000d     /* 2 register instr         TFR,EXG  */
#define OPCAT_STACK          0x000e     /* stack instr             PSHx,PULx */
#define OPCAT_BITDIRECT      0x000f     /* direct bitmanipulation       AIM  */
#define OPCAT_BITTRANS       0x0010     /* direct bit transfer         BAND  */
#define OPCAT_BLOCKTRANS     0x0011     /* block transfer               TFM  */
#define OPCAT_IREG           0x0012     /* inter-register operations   ADCR  */
#define OPCAT_QUADREG1BYTE   0x0013     /* quad reg instr 1 byte opc    LDQ  */
#define OPCAT_2IMMBYTE       0x0014     /* 2byte opcode w. imm byte    BITMD */
#define OPCAT_2ARITH         0x0015     /* 2byte opcode accum. instr.   SUBE */
#define OPCAT_ACCARITH       0x0016     /* acc. instr. w.explicit acc   ADD  */
#define OPCAT_IDXEXT         0x0017     /* indexed/extended, 6800-style JMP  */
#define OPCAT_ACCADDR        0x0018     /* single address instrs, 6800  NEG  */
#define OPCAT_SETMASK        0x0019     /* set/clear with mask, 68HC11 BCLR  */
#define OPCAT_BRMASK         0x001a     /* branch with mask, 68HC11   BRCLR  */
#define OPCAT_OACCARITH      0x001b     /* acc. instr. w.optional acc   LDA  */
#define OPCAT_OSTACK         0x001c     /* 6800 stack instr        PSHx,PULx */
#define OPCAT_PSEUDO         0x003f     /* pseudo-ops                        */
#define OPCAT_6309           0x0040     /* valid for 6309 only!              */
#define OPCAT_NOIMM          0x0080     /* immediate not allowed!       STD  */
#define OPCAT_6301           0x0100     /* valid for 6301 only!              */
#define OPCAT_PAGE18         0x0200     /* operation with prefix 18 (68HC11) */
#define OPCAT_PAGE1A         0x0400     /* operation with prefix 1A (68HC11) */
                                        /* the various Pseudo-Ops            */
#define PSEUDO_RMB            0
#define PSEUDO_ELSE           1
#define PSEUDO_END            2
#define PSEUDO_ENDIF          3
#define PSEUDO_ENDM           4
#define PSEUDO_EQU            5
#define PSEUDO_EXT            6
#define PSEUDO_FCB            7
#define PSEUDO_FCC            8
#define PSEUDO_FCW            9
#define PSEUDO_IF            10
#define PSEUDO_MACRO         11
#define PSEUDO_ORG           12
#define PSEUDO_PUB           13
#define PSEUDO_SETDP         14
#define PSEUDO_SET           15
#define PSEUDO_INCLUDE       16
#define PSEUDO_OPT           17
#define PSEUDO_NAM           18
#define PSEUDO_STTL          19
#define PSEUDO_PAG           20
#define PSEUDO_SPC           21
#define PSEUDO_REP           22
#define PSEUDO_SETPG         23
#define PSEUDO_SETLI         24
#define PSEUDO_EXITM         25
#define PSEUDO_IFN           26
#define PSEUDO_IFC           27
#define PSEUDO_IFNC          28
#define PSEUDO_DUP           29
#define PSEUDO_ENDD          30
#define PSEUDO_REG           31
#define PSEUDO_ERR           32
#define PSEUDO_TEXT          33
#define PSEUDO_RZB           34
#define PSEUDO_ABS           35
#define PSEUDO_DEF           36
#define PSEUDO_ENDDEF        37
#define PSEUDO_COMMON        38
#define PSEUDO_ENDCOM        39
#define PSEUDO_NAME          40
#define PSEUDO_SYMLEN        41
#define PSEUDO_IFD           42
#define PSEUDO_IFND          43
#define PSEUDO_BINARY        44
#define PSEUDO_PHASE         45
#define PSEUDO_DEPHASE       46
#define PSEUDO_FCQ           47
#define PSEUDO_FILL          48
#define PSEUDO_PEMT          49
#define PSEUDO_WRN           50

#define PSEUDO_FCCH          51     // FCCH is a 'hi-bit' terminated string
//#define PSEUDO_FCCZ          52     // FCCZ is a 0x00 terminated string

struct oprecord optable09[]=
  {
  { "ABA",     OPCAT_FOURBYTE,    0x3404abe0 },  // PSHB ADDA,S+
  { "ABS",     OPCAT_PSEUDO,      PSEUDO_ABS },
  { "ABX",     OPCAT_ONEBYTE,     0x3a },
  { "ABY",     OPCAT_TWOBYTE,     0x31a5 },
  { "ADC",     OPCAT_ACCARITH,    0x89 },
  { "ADCA",    OPCAT_ARITH,       0x89 },
  { "ADCB",    OPCAT_ARITH,       0xc9 },
  { "ADCD",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1089 },
  { "ADCR",    OPCAT_6309 |
               OPCAT_IREG,        0x1031 },
  { "ADD",     OPCAT_ACCARITH,    0x8b },
  { "ADDA",    OPCAT_ARITH,       0x8b },
  { "ADDB",    OPCAT_ARITH,       0xcb },
  { "ADDD",    OPCAT_DBLREG1BYTE, 0xc3 },
  { "ADDE",    OPCAT_6309 |
               OPCAT_2ARITH,      0x118b },
  { "ADDF",    OPCAT_6309 |
               OPCAT_2ARITH,      0x11cb },
  { "ADDR",    OPCAT_6309 |
               OPCAT_IREG,        0x1030 },
  { "ADDW",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x108b },
  { "AIM",     OPCAT_6309 |
               OPCAT_BITDIRECT,   0x02 }, 
  { "AND",     OPCAT_ACCARITH,    0x84 },
  { "ANDA",    OPCAT_ARITH,       0x84 },
  { "ANDB",    OPCAT_ARITH,       0xc4 },
  { "ANDCC",   OPCAT_IMMBYTE,     0x1c },
  { "ANDD",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1084 },
  { "ANDR",    OPCAT_6309 |
               OPCAT_IREG,        0x1034 },
  { "ASL",     OPCAT_SINGLEADDR,  0x08 },
  { "ASLA",    OPCAT_ONEBYTE,     0x48 },
  { "ASLB",    OPCAT_ONEBYTE,     0x58 },
  { "ASLD",    OPCAT_TWOBYTE,     0x5849 },  // ASLB ROLA
  { "ASLD63",  OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1048 },
  { "ASR",     OPCAT_SINGLEADDR,  0x07 },
  { "ASRA",    OPCAT_ONEBYTE,     0x47 },
  { "ASRB",    OPCAT_ONEBYTE,     0x57 },
  { "ASRD",    OPCAT_TWOBYTE,     0x4756 },  // ASRA RORB
  { "ASRD63",  OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1047 },
  { "BAND",    OPCAT_6309 |
               OPCAT_BITTRANS,    0x1130 }, 
  { "BCC",     OPCAT_SBRANCH,     0x24 },
  { "BCS",     OPCAT_SBRANCH,     0x25 },
  { "BEC",     OPCAT_SBRANCH,     0x24 },
  { "BEOR",    OPCAT_6309 |
               OPCAT_BITTRANS,    0x1134 }, 
  { "BEQ",     OPCAT_SBRANCH,     0x27 },
  { "BES",     OPCAT_SBRANCH,     0x25 },
  { "BGE",     OPCAT_SBRANCH,     0x2c },
  { "BGT",     OPCAT_SBRANCH,     0x2e },
  { "BHI",     OPCAT_SBRANCH,     0x22 },
  { "BHS",     OPCAT_SBRANCH,     0x24 },
  { "BIAND",   OPCAT_6309 |
               OPCAT_BITTRANS,    0x1131 }, 
  { "BIEOR",   OPCAT_6309 |
               OPCAT_BITTRANS,    0x1135 }, 
  { "BIN",     OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BINARY",  OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BIOR",    OPCAT_6309 |
               OPCAT_BITTRANS,    0x1133 }, 
  { "BIT",     OPCAT_ACCARITH,    0x85 },
  { "BITA",    OPCAT_ARITH,       0x85 },
  { "BITB",    OPCAT_ARITH,       0xc5 },
  { "BITD",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1085 },
  { "BITMD",   OPCAT_6309 |
               OPCAT_2IMMBYTE,    0x113c },
  { "BLE",     OPCAT_SBRANCH,     0x2f },
  { "BLO",     OPCAT_SBRANCH,     0x25 },
  { "BLS",     OPCAT_SBRANCH,     0x23 },
  { "BLT",     OPCAT_SBRANCH,     0x2d },
  { "BMI",     OPCAT_SBRANCH,     0x2b },
  { "BNE",     OPCAT_SBRANCH,     0x26 },
  { "BOR",     OPCAT_6309 |
               OPCAT_BITTRANS,    0x1132 }, 
  { "BPL",     OPCAT_SBRANCH,     0x2a },
  { "BRA",     OPCAT_SBRANCH,     0x20 },
  { "BRN",     OPCAT_SBRANCH,     0x21 },
  { "BSR",     OPCAT_SBRANCH,     0x8d },
  { "BSZ",     OPCAT_PSEUDO,      PSEUDO_RZB },  // AS9 style
  { "BVC",     OPCAT_SBRANCH,     0x28 },
  { "BVS",     OPCAT_SBRANCH,     0x29 },
  { "CBA",     OPCAT_FOURBYTE,    0x3404a1e0 },
  { "CLC",     OPCAT_TWOBYTE,     0x1cfe },
  { "CLF",     OPCAT_TWOBYTE,     0x1cbf },
  { "CLI",     OPCAT_TWOBYTE,     0x1cef },
  { "CLIF",    OPCAT_TWOBYTE,     0x1caf },
  { "CLR",     OPCAT_SINGLEADDR,  0x0f },
  { "CLRA",    OPCAT_ONEBYTE,     0x4f },
  { "CLRB",    OPCAT_ONEBYTE,     0x5f },
  { "CLRD",    OPCAT_TWOBYTE,     0x4f5f },  // CLRA CLRB
  { "CLRD63",  OPCAT_6309 |
               OPCAT_TWOBYTE,     0x104f },
  { "CLRE",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x114f },
  { "CLRF",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x115f },
  { "CLRW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x105f },
  { "CLV",     OPCAT_TWOBYTE,     0x1cfd },
  { "CLZ",     OPCAT_TWOBYTE,     0x1cfb },
  { "CMP",     OPCAT_ACCARITH,    0x81 },
  { "CMPA",    OPCAT_ARITH,       0x81 },
  { "CMPB",    OPCAT_ARITH,       0xc1 },
  { "CMPD",    OPCAT_DBLREG2BYTE, 0x1083 },
  { "CMPE",    OPCAT_6309 |
               OPCAT_2ARITH,      0x1181 },
  { "CMPF",    OPCAT_6309 |
               OPCAT_2ARITH,      0x11c1 },
  { "CMPR",    OPCAT_6309 |
               OPCAT_IREG,        0x1037 },
  { "CMPS",    OPCAT_DBLREG2BYTE, 0x118c },
  { "CMPU",    OPCAT_DBLREG2BYTE, 0x1183 },
  { "CMPW",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1081 },
  { "CMPX",    OPCAT_DBLREG1BYTE, 0x8c },
  { "CMPY",    OPCAT_DBLREG2BYTE, 0x108c },
  { "COM",     OPCAT_SINGLEADDR,  0x03 },
  { "COMA",    OPCAT_ONEBYTE,     0x43 },
  { "COMB",    OPCAT_ONEBYTE,     0x53 },
  { "COMD",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1043 },
  { "COME",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1143 },
  { "COMF",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1153 },
  { "COMW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1053 },
  { "COMMON",  OPCAT_PSEUDO,      PSEUDO_COMMON },
  { "CPD",     OPCAT_DBLREG2BYTE, 0x1083 },
  { "CPX",     OPCAT_DBLREG1BYTE, 0x8c },
  { "CPY",     OPCAT_DBLREG2BYTE, 0x108c },
  { "CWAI",    OPCAT_IMMBYTE,     0x3c },
  { "DAA",     OPCAT_ONEBYTE,     0x19 },
  { "DEC",     OPCAT_SINGLEADDR,  0x0a },
  { "DECA",    OPCAT_ONEBYTE,     0x4a },
  { "DECB",    OPCAT_ONEBYTE,     0x5a },
  { "DECD",    OPCAT_THREEBYTE,   0x830001 },  // SUBD #1
  { "DECD63",  OPCAT_6309 |
               OPCAT_TWOBYTE,     0x104a },
  { "DECE",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x114a },
  { "DECF",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x115a },
  { "DECW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x105a },
  { "DEF",     OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEFINE",  OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEPHASE", OPCAT_PSEUDO,      PSEUDO_DEPHASE },
  { "DES",     OPCAT_TWOBYTE,     0x327f },
  { "DEU",     OPCAT_TWOBYTE,     0x335f },
  { "DEX",     OPCAT_TWOBYTE,     0x301f },
  { "DEY",     OPCAT_TWOBYTE,     0x313f },
  { "DIVD",    OPCAT_6309 |
               OPCAT_2ARITH,      0x118d },
  { "DIVQ",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x118e },
  { "DUP",     OPCAT_PSEUDO,      PSEUDO_DUP },
  { "EIM",     OPCAT_6309 |
               OPCAT_BITDIRECT,   0x05 }, 
  { "ELSE",    OPCAT_PSEUDO,      PSEUDO_ELSE },
  { "END",     OPCAT_PSEUDO,      PSEUDO_END },
  { "ENDCOM",  OPCAT_PSEUDO,      PSEUDO_ENDCOM },
  { "ENDD",    OPCAT_PSEUDO,      PSEUDO_ENDD },
  { "ENDDEF",  OPCAT_PSEUDO,      PSEUDO_ENDDEF },
  { "ENDIF",   OPCAT_PSEUDO,      PSEUDO_ENDIF },
  { "ENDM",    OPCAT_PSEUDO,      PSEUDO_ENDM },
  { "EOR",     OPCAT_ACCARITH,    0x88 },
  { "EORA",    OPCAT_ARITH,       0x88 },
  { "EORB",    OPCAT_ARITH,       0xc8 },
  { "EORD",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1088 },
  { "EORR",    OPCAT_6309 |
               OPCAT_IREG,        0x1036 },
  { "EQU",     OPCAT_PSEUDO,      PSEUDO_EQU },
  { "ERR",     OPCAT_PSEUDO,      PSEUDO_ERR },
  { "EXG",     OPCAT_2REG,        0x1e },
  { "EXITM",   OPCAT_PSEUDO,      PSEUDO_EXITM },
  { "EXT",     OPCAT_PSEUDO,      PSEUDO_EXT },
  { "EXTERN",  OPCAT_PSEUDO,      PSEUDO_EXT },
  { "FCB",     OPCAT_PSEUDO,      PSEUDO_FCB },
  { "FCC",     OPCAT_PSEUDO,      PSEUDO_FCC },
  { "FCCH",    OPCAT_PSEUDO,      PSEUDO_FCCH },
  { "FCQ",     OPCAT_6309 |
               OPCAT_PSEUDO,      PSEUDO_FCQ },
  { "FCW",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FDB",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FILL",    OPCAT_PSEUDO,      PSEUDO_FILL },
  { "FQB",     OPCAT_6309 |
               OPCAT_PSEUDO,      PSEUDO_FCQ },
  { "GLOBAL",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "IF",      OPCAT_PSEUDO,      PSEUDO_IF },
  { "IFC",     OPCAT_PSEUDO,      PSEUDO_IFC },
  { "IFD",     OPCAT_PSEUDO,      PSEUDO_IFD },
  { "IFN",     OPCAT_PSEUDO,      PSEUDO_IFN },
  { "IFNC",    OPCAT_PSEUDO,      PSEUDO_IFNC },
  { "IFND",    OPCAT_PSEUDO,      PSEUDO_IFND },
  { "INC",     OPCAT_SINGLEADDR,  0x0c },
  { "INCA",    OPCAT_ONEBYTE,     0x4c },
  { "INCB",    OPCAT_ONEBYTE,     0x5c },
  { "INCD",    OPCAT_THREEBYTE,   0xc30001 },  // ADDD 1
  { "INCD63",  OPCAT_6309 |
               OPCAT_TWOBYTE,     0x104c },
  { "INCE",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x114c },
  { "INCF",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x115c },
  { "INCLUDE", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "INCW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x105c },
  { "INS",     OPCAT_TWOBYTE,     0x3261 },
  { "INU",     OPCAT_TWOBYTE,     0x3341 },
  { "INX",     OPCAT_TWOBYTE,     0x3001 },
  { "INY",     OPCAT_TWOBYTE,     0x3121 },
  { "JMP",     OPCAT_SINGLEADDR,  0x0e },
  { "JSR",     OPCAT_DBLREG1BYTE, 0x8d },
  { "LBCC",    OPCAT_LBR2BYTE,    0x1024 },
  { "LBCS",    OPCAT_LBR2BYTE,    0x1025 },
  { "LBEC",    OPCAT_LBR2BYTE,    0x1024 },
  { "LBEQ",    OPCAT_LBR2BYTE,    0x1027 },
  { "LBES",    OPCAT_LBR2BYTE,    0x1025 },
  { "LBGE",    OPCAT_LBR2BYTE,    0x102c },
  { "LBGT",    OPCAT_LBR2BYTE,    0x102e },
  { "LBHI",    OPCAT_LBR2BYTE,    0x1022 },
  { "LBHS",    OPCAT_LBR2BYTE,    0x1024 },
  { "LBLE",    OPCAT_LBR2BYTE,    0x102f },
  { "LBLO",    OPCAT_LBR2BYTE,    0x1025 },
  { "LBLS",    OPCAT_LBR2BYTE,    0x1023 },
  { "LBLT",    OPCAT_LBR2BYTE,    0x102d },
  { "LBMI",    OPCAT_LBR2BYTE,    0x102b },
  { "LBNE",    OPCAT_LBR2BYTE,    0x1026 },
  { "LBPL",    OPCAT_LBR2BYTE,    0x102a },
  { "LBRA",    OPCAT_LBR1BYTE,    0x16 },
  { "LBRN",    OPCAT_LBR2BYTE,    0x1021 },
  { "LBSR",    OPCAT_LBR1BYTE,    0x17 },
  { "LBVC",    OPCAT_LBR2BYTE,    0x1028 },
  { "LBVS",    OPCAT_LBR2BYTE,    0x1029 },
  { "LD",      OPCAT_ACCARITH,    0x86 },
  { "LDA",     OPCAT_ACCARITH,    0x86 },
  { "LDAA",    OPCAT_ARITH,       0x86 },
  { "LDAB",    OPCAT_ARITH,       0xc6 },
  { "LDAD",    OPCAT_DBLREG1BYTE, 0xcc },
  { "LDB",     OPCAT_ARITH,       0xc6 },
  { "LDBT",    OPCAT_6309 |
               OPCAT_BITTRANS,    0x1136 }, 
  { "LDD",     OPCAT_DBLREG1BYTE, 0xcc },
  { "LDE",     OPCAT_6309 |
               OPCAT_2ARITH,      0x1186 },
  { "LDF",     OPCAT_6309 |
               OPCAT_2ARITH,      0x11c6 },
  { "LDMD",    OPCAT_6309 |
               OPCAT_2IMMBYTE,    0x113d },
  { "LDQ",     OPCAT_6309 |
               OPCAT_QUADREG1BYTE,0x10cc },
  { "LDS",     OPCAT_DBLREG2BYTE, 0x10ce },
  { "LDU",     OPCAT_DBLREG1BYTE, 0xce },
  { "LDW",     OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1086 },
  { "LDX",     OPCAT_DBLREG1BYTE, 0x8e },
  { "LDY",     OPCAT_DBLREG2BYTE, 0x108e },
  { "LEAS",    OPCAT_LEA,         0x32 },
  { "LEAU",    OPCAT_LEA,         0x33 },
  { "LEAX",    OPCAT_LEA,         0x30 },
  { "LEAY",    OPCAT_LEA,         0x31 },
  { "LIB",     OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LIBRARY", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LSL",     OPCAT_SINGLEADDR,  0x08 },
  { "LSLA",    OPCAT_ONEBYTE,     0x48 },
  { "LSLB",    OPCAT_ONEBYTE,     0x58 },
  { "LSLD",    OPCAT_TWOBYTE,     0x5849 },  // LSLB ROLA
  { "LSLD63",  OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1048 },
  { "LSR",     OPCAT_SINGLEADDR,  0x04 },
  { "LSRA",    OPCAT_ONEBYTE,     0x44 },
  { "LSRB",    OPCAT_ONEBYTE,     0x54 },
  { "LSRD",    OPCAT_TWOBYTE,     0x4456 },  // LSRA RORB
  { "LSRD63",  OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1044 },
  { "LSRW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1054 },
  { "MACRO",   OPCAT_PSEUDO,      PSEUDO_MACRO },
  { "MUL",     OPCAT_ONEBYTE,     0x3d },
  { "MULD",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x118f },
  { "NAM",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "NAME",    OPCAT_PSEUDO,      PSEUDO_NAME },
  { "NEG",     OPCAT_SINGLEADDR,  0x00 },
  { "NEGA",    OPCAT_ONEBYTE,     0x40 },
  { "NEGB",    OPCAT_ONEBYTE,     0x50 },
  { "NEGD",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1040 },
  { "NOP",     OPCAT_ONEBYTE,     0x12 },
  { "OIM",     OPCAT_6309 |
               OPCAT_BITDIRECT,   0x01 }, 
  { "OPT",     OPCAT_PSEUDO,      PSEUDO_OPT },
  { "OPTION",  OPCAT_PSEUDO,      PSEUDO_OPT },
  { "ORA",     OPCAT_ARITH,       0x8a },
  { "ORAA",    OPCAT_ARITH,       0x8a },
  { "ORAB",    OPCAT_ARITH,       0xca },
  { "ORB",     OPCAT_ARITH,       0xca },
  { "ORCC",    OPCAT_IMMBYTE,     0x1a },
  { "ORD",     OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x108a },
  { "ORG",     OPCAT_PSEUDO,      PSEUDO_ORG },
  { "ORR",     OPCAT_6309 |
               OPCAT_IREG,        0x1035 },
  { "PAG",     OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PAGE",    OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PEMT",    OPCAT_PSEUDO,      PSEUDO_PEMT },
  { "PHASE",   OPCAT_PSEUDO,      PSEUDO_PHASE },
  { "PSH",     OPCAT_STACK,       0x34 },
  { "PSHA",    OPCAT_TWOBYTE,     0x3402 },
  { "PSHB",    OPCAT_TWOBYTE,     0x3404 },
  { "PSHD",    OPCAT_TWOBYTE,     0x3406 },
  { "PSHS",    OPCAT_STACK,       0x34 },
  { "PSHSW",   OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1038 },
  { "PSHU",    OPCAT_STACK,       0x36 },
  { "PSHUW",   OPCAT_6309 |
               OPCAT_TWOBYTE,     0x103a },
  { "PSHX",    OPCAT_TWOBYTE,     0x3410 },
  { "PSHY",    OPCAT_TWOBYTE,     0x3420 },
  { "PUB",     OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUBLIC",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUL",     OPCAT_STACK,       0x35 },
  { "PULA",    OPCAT_TWOBYTE,     0x3502 },
  { "PULB",    OPCAT_TWOBYTE,     0x3504 },
  { "PULD",    OPCAT_TWOBYTE,     0x3506 },
  { "PULS",    OPCAT_STACK,       0x35 },
  { "PULSW",   OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1039 },
  { "PULU",    OPCAT_STACK,       0x37 },
  { "PULUW",   OPCAT_6309 |
               OPCAT_TWOBYTE,     0x103b },
  { "PULX",    OPCAT_TWOBYTE,     0x3510 },
  { "PULY",    OPCAT_TWOBYTE,     0x3520 },
  { "REG",     OPCAT_PSEUDO,      PSEUDO_REG },
  { "REP",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "REPEAT",  OPCAT_PSEUDO,      PSEUDO_REP },
  { "RESET",   OPCAT_ONEBYTE,     0x3e },
  { "RMB",     OPCAT_PSEUDO,      PSEUDO_RMB },
  { "ROL",     OPCAT_SINGLEADDR,  0x09 },
  { "ROLA",    OPCAT_ONEBYTE,     0x49 },
  { "ROLB",    OPCAT_ONEBYTE,     0x59 },
  { "ROLD",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1049 },
  { "ROLW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1059 },
  { "ROR",     OPCAT_SINGLEADDR,  0x06 },
  { "RORA",    OPCAT_ONEBYTE,     0x46 },
  { "RORB",    OPCAT_ONEBYTE,     0x56 },
  { "RORD",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1046 },
  { "RORW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x1056 },
  { "RPT",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "RTI",     OPCAT_ONEBYTE,     0x3b },
  { "RTS",     OPCAT_ONEBYTE,     0x39 },
  { "RZB",     OPCAT_PSEUDO,      PSEUDO_RZB },
  { "SBA",     OPCAT_FOURBYTE,    0x3404a0e0 },
  { "SBC",     OPCAT_ACCARITH,    0x82 },
  { "SBCA",    OPCAT_ARITH,       0x82 },
  { "SBCB",    OPCAT_ARITH,       0xc2 },
  { "SBCD",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1082 },
  { "SBCR",    OPCAT_6309 |
               OPCAT_IREG,        0x1033 },
  { "SEC",     OPCAT_TWOBYTE,     0x1a01 },
  { "SEF",     OPCAT_TWOBYTE,     0x1a40 },
  { "SEI",     OPCAT_TWOBYTE,     0x1a10 },
  { "SEIF",    OPCAT_TWOBYTE,     0x1a50 },
  { "SET",     OPCAT_PSEUDO,      PSEUDO_SET },
  { "SETDP",   OPCAT_PSEUDO,      PSEUDO_SETDP },
  { "SETLI",   OPCAT_PSEUDO,      PSEUDO_SETLI },
  { "SETPG",   OPCAT_PSEUDO,      PSEUDO_SETPG },
  { "SEV",     OPCAT_TWOBYTE,     0x1a02 },
  { "SEX",     OPCAT_ONEBYTE,     0x1d },
  { "SEXW",    OPCAT_6309 |
               OPCAT_ONEBYTE,     0x14 },
  { "SEZ",     OPCAT_TWOBYTE,     0x1a04 },
  { "SPC",     OPCAT_PSEUDO,      PSEUDO_SPC },
  { "STA",     OPCAT_NOIMM |
               OPCAT_ARITH,       0x87 },
  { "STAA",    OPCAT_NOIMM |
               OPCAT_ARITH,       0x87 },
  { "STAB",    OPCAT_NOIMM |
               OPCAT_ARITH,       0xc7 },
  { "STAD",    OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcd },
  { "STB",     OPCAT_NOIMM |
               OPCAT_ARITH,       0xc7 },
  { "STBT",    OPCAT_6309 |
               OPCAT_BITTRANS,    0x1137 }, 
  { "STD",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcd },
  { "STE",     OPCAT_NOIMM |
               OPCAT_6309 |
               OPCAT_2ARITH,      0x1187 },
  { "STF",     OPCAT_NOIMM |
               OPCAT_6309 |
               OPCAT_2ARITH,      0x11c7 },
  { "STQ",     OPCAT_NOIMM |
               OPCAT_6309 |
               OPCAT_QUADREG1BYTE,0x10cd },
  { "STS",     OPCAT_NOIMM |
               OPCAT_DBLREG2BYTE, 0x10cf },
  { "STTL",    OPCAT_PSEUDO,      PSEUDO_STTL },
  { "STU",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcf },
  { "STW",     OPCAT_NOIMM |
               OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1087 },
  { "STX",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0x8f },
  { "STY",     OPCAT_NOIMM |
               OPCAT_DBLREG2BYTE, 0x108f },
  { "SUB",     OPCAT_ACCARITH,    0x80 },
  { "SUBA",    OPCAT_ARITH,       0x80 },
  { "SUBB",    OPCAT_ARITH,       0xc0 },
  { "SUBD",    OPCAT_DBLREG1BYTE, 0x83 },
  { "SUBE",    OPCAT_6309 |
               OPCAT_2ARITH,      0x1180 },
  { "SUBF",    OPCAT_6309 |
               OPCAT_2ARITH,      0x11c0 },
  { "SUBW",    OPCAT_6309 |
               OPCAT_DBLREG2BYTE, 0x1080 },
  { "SUBR",    OPCAT_6309 |
               OPCAT_IREG,        0x1032 },
  { "SWI",     OPCAT_ONEBYTE,     0x3f },
  { "SWI2",    OPCAT_TWOBYTE,     0x103f },
  { "SWI3",    OPCAT_TWOBYTE,     0x113f },
  { "SYMLEN",  OPCAT_PSEUDO,      PSEUDO_SYMLEN },
  { "SYNC",    OPCAT_ONEBYTE,     0x13 },
  { "TAB",     OPCAT_THREEBYTE,   0x1f894d },
  { "TAP",     OPCAT_TWOBYTE,     0x1f8a },
  { "TBA",     OPCAT_THREEBYTE,   0x1f984d },
  { "TEXT",    OPCAT_PSEUDO,      PSEUDO_TEXT },
  { "TFM",     OPCAT_6309 |
               OPCAT_BLOCKTRANS,  0x1138 }, 
  { "TFR",     OPCAT_2REG,        0x1f },
  { "TIM",     OPCAT_6309 |
               OPCAT_BITDIRECT,   0x0b }, 
  { "TITLE",   OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TPA",     OPCAT_TWOBYTE,     0x1fa8 },
  { "TST",     OPCAT_SINGLEADDR,  0x0d },
  { "TSTA",    OPCAT_ONEBYTE,     0x4d },
  { "TSTB",    OPCAT_ONEBYTE,     0x5d },
  { "TSTD",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x104d },
  { "TSTE",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x114d },
  { "TSTF",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x115d },
  { "TSTW",    OPCAT_6309 |
               OPCAT_TWOBYTE,     0x105d },
  { "TSX",     OPCAT_TWOBYTE,     0x1f41 },
  { "TSY",     OPCAT_FOURBYTE,    0x34403520 },  /* PSHS S/PULS Y */
  { "TTL",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TXS",     OPCAT_TWOBYTE,     0x1f14 },
  { "TYS",     OPCAT_FOURBYTE,    0x34203540 },  /* PSHS Y/PULS S */
  { "WAI",     OPCAT_TWOBYTE,     0x3cff },
  { "WRN",     OPCAT_PSEUDO,      PSEUDO_WRN },
  { "ZMB",     OPCAT_PSEUDO,      PSEUDO_RZB },
};

struct oprecord optable00[]=
  {
  { "ABA",     OPCAT_ONEBYTE,     0x1b },
  { "ABS",     OPCAT_PSEUDO,      PSEUDO_ABS },
  { "ADC",     OPCAT_ACCARITH,    0x89 },
  { "ADCA",    OPCAT_ARITH,       0x89 },
  { "ADCB",    OPCAT_ARITH,       0xc9 },
  { "ADD",     OPCAT_ACCARITH,    0x8b },
  { "ADDA",    OPCAT_ARITH,       0x8b },
  { "ADDB",    OPCAT_ARITH,       0xcb },
  { "AND",     OPCAT_ACCARITH,    0x84 },
  { "ANDA",    OPCAT_ARITH,       0x84 },
  { "ANDB",    OPCAT_ARITH,       0xc4 },
  { "ASL",     OPCAT_ACCADDR,     0x08 },
  { "ASLA",    OPCAT_ONEBYTE,     0x48 },
  { "ASLB",    OPCAT_ONEBYTE,     0x58 },
  { "ASLD",    OPCAT_TWOBYTE,     0x5849 },  // ASLB ROLA
  { "ASR",     OPCAT_ACCADDR,     0x07 },
  { "ASRA",    OPCAT_ONEBYTE,     0x47 },
  { "ASRB",    OPCAT_ONEBYTE,     0x57 },
  { "ASRD",    OPCAT_TWOBYTE,     0x4756 },  // ASRA RORB
  { "BCC",     OPCAT_SBRANCH,     0x24 },
  { "BCS",     OPCAT_SBRANCH,     0x25 },
  { "BEC",     OPCAT_SBRANCH,     0x24 },
  { "BEQ",     OPCAT_SBRANCH,     0x27 },
  { "BES",     OPCAT_SBRANCH,     0x25 },
  { "BGE",     OPCAT_SBRANCH,     0x2c },
  { "BGT",     OPCAT_SBRANCH,     0x2e },
  { "BHI",     OPCAT_SBRANCH,     0x22 },
  { "BHS",     OPCAT_SBRANCH,     0x24 },
  { "BIN",     OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BINARY",  OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BIT",     OPCAT_ACCARITH,    0x85 },
  { "BITA",    OPCAT_ARITH,       0x85 },
  { "BITB",    OPCAT_ARITH,       0xc5 },
  { "BLE",     OPCAT_SBRANCH,     0x2f },
  { "BLO",     OPCAT_SBRANCH,     0x25 },
  { "BLS",     OPCAT_SBRANCH,     0x23 },
  { "BLT",     OPCAT_SBRANCH,     0x2d },
  { "BMI",     OPCAT_SBRANCH,     0x2b },
  { "BNE",     OPCAT_SBRANCH,     0x26 },
  { "BPL",     OPCAT_SBRANCH,     0x2a },
  { "BRA",     OPCAT_SBRANCH,     0x20 },
  { "BSR",     OPCAT_SBRANCH,     0x8d },
  { "BSZ",     OPCAT_PSEUDO,      PSEUDO_RZB },  // AS9 style
  { "BVC",     OPCAT_SBRANCH,     0x28 },
  { "BVS",     OPCAT_SBRANCH,     0x29 },
  { "CBA",     OPCAT_ONEBYTE,     0x11 },
  { "CLC",     OPCAT_ONEBYTE,     0x0c },
  { "CLI",     OPCAT_ONEBYTE,     0x0e },
  { "CLR",     OPCAT_ACCADDR,     0x0f },
  { "CLRA",    OPCAT_ONEBYTE,     0x4f },
  { "CLRB",    OPCAT_ONEBYTE,     0x5f },
  { "CLRD",    OPCAT_TWOBYTE,     0x4f5f },  // CLRA CLRB
  { "CLV",     OPCAT_ONEBYTE,     0x0a },
  { "CMP",     OPCAT_ACCARITH,    0x81 },
  { "CMPA",    OPCAT_ARITH,       0x81 },
  { "CMPB",    OPCAT_ARITH,       0xc1 },
  { "COM",     OPCAT_ACCADDR,     0x03 },
  { "COMA",    OPCAT_ONEBYTE,     0x43 },
  { "COMB",    OPCAT_ONEBYTE,     0x53 },
  { "CPX",     OPCAT_DBLREG1BYTE, 0x8c },
  { "DAA",     OPCAT_ONEBYTE,     0x19 },
  { "DEC",     OPCAT_ACCADDR,     0x0a },
  { "DECA",    OPCAT_ONEBYTE,     0x4a },
  { "DECB",    OPCAT_ONEBYTE,     0x5a },
  { "DECD",    OPCAT_FOURBYTE,    0xc0018200 },  // SUBB#1 SBCA#0
  { "DEF",     OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEFINE",  OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEPHASE", OPCAT_PSEUDO,      PSEUDO_DEPHASE },
  { "DES",     OPCAT_ONEBYTE,     0x34 },
  { "DEX",     OPCAT_ONEBYTE,     0x09 },
  { "DUP",     OPCAT_PSEUDO,      PSEUDO_DUP },
  { "ELSE",    OPCAT_PSEUDO,      PSEUDO_ELSE },
  { "END",     OPCAT_PSEUDO,      PSEUDO_END },
  { "ENDCOM",  OPCAT_PSEUDO,      PSEUDO_ENDCOM },
  { "ENDD",    OPCAT_PSEUDO,      PSEUDO_ENDD },
  { "ENDDEF",  OPCAT_PSEUDO,      PSEUDO_ENDDEF },
  { "ENDIF",   OPCAT_PSEUDO,      PSEUDO_ENDIF },
  { "ENDM",    OPCAT_PSEUDO,      PSEUDO_ENDM },
  { "EOR",     OPCAT_ACCARITH,    0x88 },
  { "EORA",    OPCAT_ARITH,       0x88 },
  { "EORB",    OPCAT_ARITH,       0xc8 },
  { "EQU",     OPCAT_PSEUDO,      PSEUDO_EQU },
  { "ERR",     OPCAT_PSEUDO,      PSEUDO_ERR },
  { "EXITM",   OPCAT_PSEUDO,      PSEUDO_EXITM },
  { "EXT",     OPCAT_PSEUDO,      PSEUDO_EXT },
  { "EXTERN",  OPCAT_PSEUDO,      PSEUDO_EXT },
  { "FCB",     OPCAT_PSEUDO,      PSEUDO_FCB },
  { "FCC",     OPCAT_PSEUDO,      PSEUDO_FCC },
  { "FCW",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FDB",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FILL",    OPCAT_PSEUDO,      PSEUDO_FILL },
  { "GLOBAL",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "IF",      OPCAT_PSEUDO,      PSEUDO_IF },
  { "IFC",     OPCAT_PSEUDO,      PSEUDO_IFC },
  { "IFD",     OPCAT_PSEUDO,      PSEUDO_IFD },
  { "IFN",     OPCAT_PSEUDO,      PSEUDO_IFN },
  { "IFNC",    OPCAT_PSEUDO,      PSEUDO_IFNC },
  { "IFND",    OPCAT_PSEUDO,      PSEUDO_IFND },
  { "INC",     OPCAT_ACCADDR,     0x0c },
  { "INCA",    OPCAT_ONEBYTE,     0x4c },
  { "INCB",    OPCAT_ONEBYTE,     0x5c },
  { "INCD",    OPCAT_FOURBYTE,    0xcb018900 },  // ADDB#1 ADCA#0
  { "INCLUDE", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "INS",     OPCAT_ONEBYTE,     0x31 },
  { "INX",     OPCAT_ONEBYTE,     0x08 },
  { "JMP",     OPCAT_IDXEXT,      0x4e },
  { "JSR",     OPCAT_IDXEXT,      0x8d },
  { "LDA",     OPCAT_ACCARITH,    0x86 },
  { "LDAA",    OPCAT_ARITH,       0x86 },
  { "LDAB",    OPCAT_ARITH,       0xc6 },
  { "LDB",     OPCAT_ARITH,       0xc6 },
  { "LDS",     OPCAT_DBLREG1BYTE, 0x8e },
  { "LDX",     OPCAT_DBLREG1BYTE, 0xce },
  { "LIB",     OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LIBRARY", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LSL",     OPCAT_ACCADDR,     0x08 },
  { "LSLA",    OPCAT_ONEBYTE,     0x48 },
  { "LSLB",    OPCAT_ONEBYTE,     0x58 },
  { "LSLD",    OPCAT_TWOBYTE,     0x5849 },  // LSLB ROLA
  { "LSR",     OPCAT_ACCADDR,     0x04 },
  { "LSRA",    OPCAT_ONEBYTE,     0x44 },
  { "LSRB",    OPCAT_ONEBYTE,     0x54 },
  { "LSRD",    OPCAT_TWOBYTE,     0x4456 },  // LSRA RORB
  { "MACRO",   OPCAT_PSEUDO,      PSEUDO_MACRO },
  { "NAM",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "NAME",    OPCAT_PSEUDO,      PSEUDO_NAME },
  { "NEG",     OPCAT_ACCADDR,     0x00 },
  { "NEGA",    OPCAT_ONEBYTE,     0x40 },
  { "NEGB",    OPCAT_ONEBYTE,     0x50 },
  { "NOP",     OPCAT_ONEBYTE,     0x01 },
  { "OPT",     OPCAT_PSEUDO,      PSEUDO_OPT },
  { "OPTION",  OPCAT_PSEUDO,      PSEUDO_OPT },
  { "ORA",     OPCAT_ARITH,       0x8a },
  { "ORAA",    OPCAT_ARITH,       0x8a },
  { "ORAB",    OPCAT_ARITH,       0xca },
  { "ORB",     OPCAT_ARITH,       0xca },
  { "ORG",     OPCAT_PSEUDO,      PSEUDO_ORG },
  { "PAG",     OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PAGE",    OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PEMT",    OPCAT_PSEUDO,      PSEUDO_PEMT },
  { "PHASE",   OPCAT_PSEUDO,      PSEUDO_PHASE },
  { "PSH",     OPCAT_OSTACK,      0x36 },
  { "PSHA",    OPCAT_ONEBYTE,     0x36 },
  { "PSHB",    OPCAT_ONEBYTE,     0x37 },
  { "PUB",     OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUBLIC",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUL",     OPCAT_OSTACK,      0x30 },
  { "PULA",    OPCAT_ONEBYTE,     0x32 },
  { "PULB",    OPCAT_ONEBYTE,     0x33 },
//{ "REG",     OPCAT_PSEUDO,      PSEUDO_REG },    // not needed
  { "REP",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "REPEAT",  OPCAT_PSEUDO,      PSEUDO_REP },
  { "RMB",     OPCAT_PSEUDO,      PSEUDO_RMB },
  { "ROL",     OPCAT_ACCADDR,     0x09 },
  { "ROLA",    OPCAT_ONEBYTE,     0x49 },
  { "ROLB",    OPCAT_ONEBYTE,     0x59 },
  { "ROR",     OPCAT_ACCADDR,     0x06 },
  { "RORA",    OPCAT_ONEBYTE,     0x46 },
  { "RORB",    OPCAT_ONEBYTE,     0x56 },
  { "RPT",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "RTI",     OPCAT_ONEBYTE,     0x3b },
  { "RTS",     OPCAT_ONEBYTE,     0x39 },
  { "RZB",     OPCAT_PSEUDO,      PSEUDO_RZB },
  { "SBA",     OPCAT_ONEBYTE,     0x10 },
  { "SBC",     OPCAT_ACCARITH,    0x82 },
  { "SBCA",    OPCAT_ARITH,       0x82 },
  { "SBCB",    OPCAT_ARITH,       0xc2 },
  { "SEC",     OPCAT_ONEBYTE,     0x0d },
  { "SEI",     OPCAT_ONEBYTE,     0x0f },
  { "SET",     OPCAT_PSEUDO,      PSEUDO_SET },
//{ "SETDP",   OPCAT_PSEUDO,      PSEUDO_SETDP },
  { "SETLI",   OPCAT_PSEUDO,      PSEUDO_SETLI },
  { "SETPG",   OPCAT_PSEUDO,      PSEUDO_SETPG },
  { "SEV",     OPCAT_ONEBYTE,     0x0b },
  { "SPC",     OPCAT_PSEUDO,      PSEUDO_SPC },
  { "STA",     OPCAT_NOIMM |
               OPCAT_ACCARITH,    0x87 },
  { "STAA",    OPCAT_NOIMM |
               OPCAT_ARITH,       0x87 },
  { "STAB",    OPCAT_NOIMM |
               OPCAT_ARITH,       0xc7 },
  { "STS",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0x8f },
  { "STTL",    OPCAT_PSEUDO,      PSEUDO_STTL },
  { "STX",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcf },
  { "SUB",     OPCAT_ACCARITH,    0x80 },
  { "SUBA",    OPCAT_ARITH,       0x80 },
  { "SUBB",    OPCAT_ARITH,       0xc0 },
  { "SWI",     OPCAT_ONEBYTE,     0x3f },
  { "SYMLEN",  OPCAT_PSEUDO,      PSEUDO_SYMLEN },
  { "TAB",     OPCAT_ONEBYTE,     0x16 },
  { "TAP",     OPCAT_ONEBYTE,     0x06 },
  { "TBA",     OPCAT_ONEBYTE,     0x17 },
  { "TEXT",    OPCAT_PSEUDO,      PSEUDO_TEXT },
  { "TITLE",   OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TPA",     OPCAT_ONEBYTE,     0x07 },
  { "TST",     OPCAT_ACCADDR,     0x0d },
  { "TSTA",    OPCAT_ONEBYTE,     0x4d },
  { "TSTB",    OPCAT_ONEBYTE,     0x5d },
  { "TSX",     OPCAT_ONEBYTE,     0x30 },
  { "TTL",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TXS",     OPCAT_ONEBYTE,     0x35 },
  { "WAI",     OPCAT_ONEBYTE,     0x3e },
  { "WRN",     OPCAT_PSEUDO,      PSEUDO_WRN },
  { "ZMB",     OPCAT_PSEUDO,      PSEUDO_RZB },
  };

struct oprecord optable01[]=
  {
  { "ABA",     OPCAT_ONEBYTE,     0x1b },
  { "ABS",     OPCAT_PSEUDO,      PSEUDO_ABS },
  { "ABX",     OPCAT_ONEBYTE,     0x3a },
  { "ADC",     OPCAT_ACCARITH,    0x89 },
  { "ADCA",    OPCAT_ARITH,       0x89 },
  { "ADCB",    OPCAT_ARITH,       0xc9 },
  { "ADD",     OPCAT_ACCARITH,    0x8b },
  { "ADDA",    OPCAT_ARITH,       0x8b },
  { "ADDB",    OPCAT_ARITH,       0xcb },
  { "ADDD",    OPCAT_DBLREG1BYTE, 0xc3 },
  { "AIM",     OPCAT_6301 |
               OPCAT_BITDIRECT,   0x01 }, 
  { "AND",     OPCAT_ACCARITH,    0x84 },
  { "ANDA",    OPCAT_ARITH,       0x84 },
  { "ANDB",    OPCAT_ARITH,       0xc4 },
  { "ASL",     OPCAT_ACCADDR,     0x08 },
  { "ASLA",    OPCAT_ONEBYTE,     0x48 },
  { "ASLB",    OPCAT_ONEBYTE,     0x58 },
  { "ASLD",    OPCAT_ONEBYTE,     0x05 },
  { "ASR",     OPCAT_ACCADDR,     0x07 },
  { "ASRA",    OPCAT_ONEBYTE,     0x47 },
  { "ASRB",    OPCAT_ONEBYTE,     0x57 },
  { "ASRD",    OPCAT_TWOBYTE,     0x4756 },  // ASRA RORB
  { "BCC",     OPCAT_SBRANCH,     0x24 },
  { "BCS",     OPCAT_SBRANCH,     0x25 },
  { "BEC",     OPCAT_SBRANCH,     0x24 },
  { "BEQ",     OPCAT_SBRANCH,     0x27 },
  { "BES",     OPCAT_SBRANCH,     0x25 },
  { "BGE",     OPCAT_SBRANCH,     0x2c },
  { "BGT",     OPCAT_SBRANCH,     0x2e },
  { "BHI",     OPCAT_SBRANCH,     0x22 },
  { "BHS",     OPCAT_SBRANCH,     0x24 },
  { "BIN",     OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BINARY",  OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BIT",     OPCAT_ACCARITH,    0x85 },
  { "BITA",    OPCAT_ARITH,       0x85 },
  { "BITB",    OPCAT_ARITH,       0xc5 },
  { "BLE",     OPCAT_SBRANCH,     0x2f },
  { "BLO",     OPCAT_SBRANCH,     0x25 },
  { "BLS",     OPCAT_SBRANCH,     0x23 },
  { "BLT",     OPCAT_SBRANCH,     0x2d },
  { "BMI",     OPCAT_SBRANCH,     0x2b },
  { "BNE",     OPCAT_SBRANCH,     0x26 },
  { "BPL",     OPCAT_SBRANCH,     0x2a },
  { "BRA",     OPCAT_SBRANCH,     0x20 },
  { "BRN",     OPCAT_SBRANCH,     0x21 },
  { "BSR",     OPCAT_SBRANCH,     0x8d },
  { "BSZ",     OPCAT_PSEUDO,      PSEUDO_RZB },  // AS9 style
  { "BVC",     OPCAT_SBRANCH,     0x28 },
  { "BVS",     OPCAT_SBRANCH,     0x29 },
  { "CBA",     OPCAT_ONEBYTE,     0x11 },
  { "CLC",     OPCAT_ONEBYTE,     0x0c },
  { "CLI",     OPCAT_ONEBYTE,     0x0e },
  { "CLR",     OPCAT_ACCADDR,     0x0f },
  { "CLRA",    OPCAT_ONEBYTE,     0x4f },
  { "CLRB",    OPCAT_ONEBYTE,     0x5f },
  { "CLRD",    OPCAT_TWOBYTE,     0x4f5f },  // CLRA CLRB
  { "CLV",     OPCAT_ONEBYTE,     0x0a },
  { "CMP",     OPCAT_ACCARITH,    0x81 },
  { "CMPA",    OPCAT_ARITH,       0x81 },
  { "CMPB",    OPCAT_ARITH,       0xc1 },
  { "COM",     OPCAT_ACCADDR,     0x03 },
  { "COMA",    OPCAT_ONEBYTE,     0x43 },
  { "COMB",    OPCAT_ONEBYTE,     0x53 },
  { "CPX",     OPCAT_DBLREG1BYTE, 0x8c },
  { "DAA",     OPCAT_ONEBYTE,     0x19 },
  { "DEC",     OPCAT_ACCADDR,     0x0a },
  { "DECA",    OPCAT_ONEBYTE,     0x4a },
  { "DECB",    OPCAT_ONEBYTE,     0x5a },
  { "DECD",    OPCAT_FOURBYTE,    0xc0018200 },  // SUBB#1 SBCA#0
  { "DEF",     OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEFINE",  OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEPHASE", OPCAT_PSEUDO,      PSEUDO_DEPHASE },
  { "DES",     OPCAT_ONEBYTE,     0x34 },
  { "DEX",     OPCAT_ONEBYTE,     0x09 },
  { "DUP",     OPCAT_PSEUDO,      PSEUDO_DUP },
  { "EIM",     OPCAT_6301 |
               OPCAT_BITDIRECT,   0x05 }, 
  { "ELSE",    OPCAT_PSEUDO,      PSEUDO_ELSE },
  { "END",     OPCAT_PSEUDO,      PSEUDO_END },
  { "ENDCOM",  OPCAT_PSEUDO,      PSEUDO_ENDCOM },
  { "ENDD",    OPCAT_PSEUDO,      PSEUDO_ENDD },
  { "ENDDEF",  OPCAT_PSEUDO,      PSEUDO_ENDDEF },
  { "ENDIF",   OPCAT_PSEUDO,      PSEUDO_ENDIF },
  { "ENDM",    OPCAT_PSEUDO,      PSEUDO_ENDM },
  { "EOR",     OPCAT_ACCARITH,    0x88 },
  { "EORA",    OPCAT_ARITH,       0x88 },
  { "EORB",    OPCAT_ARITH,       0xc8 },
  { "EQU",     OPCAT_PSEUDO,      PSEUDO_EQU },
  { "ERR",     OPCAT_PSEUDO,      PSEUDO_ERR },
  { "EXITM",   OPCAT_PSEUDO,      PSEUDO_EXITM },
  { "EXT",     OPCAT_PSEUDO,      PSEUDO_EXT },
  { "EXTERN",  OPCAT_PSEUDO,      PSEUDO_EXT },
  { "FCB",     OPCAT_PSEUDO,      PSEUDO_FCB },
  { "FCC",     OPCAT_PSEUDO,      PSEUDO_FCC },
  { "FCW",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FDB",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FILL",    OPCAT_PSEUDO,      PSEUDO_FILL },
  { "GLOBAL",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "IF",      OPCAT_PSEUDO,      PSEUDO_IF },
  { "IFC",     OPCAT_PSEUDO,      PSEUDO_IFC },
  { "IFD",     OPCAT_PSEUDO,      PSEUDO_IFD },
  { "IFN",     OPCAT_PSEUDO,      PSEUDO_IFN },
  { "IFNC",    OPCAT_PSEUDO,      PSEUDO_IFNC },
  { "IFND",    OPCAT_PSEUDO,      PSEUDO_IFND },
  { "INC",     OPCAT_ACCADDR,     0x0c },
  { "INCA",    OPCAT_ONEBYTE,     0x4c },
  { "INCB",    OPCAT_ONEBYTE,     0x5c },
  { "INCD",    OPCAT_FOURBYTE,    0xcb018900 },  // ADDB#1 ADCA#0
  { "INCLUDE", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "INS",     OPCAT_ONEBYTE,     0x31 },
  { "INX",     OPCAT_ONEBYTE,     0x08 },
  { "JMP",     OPCAT_IDXEXT,      0x4e },
  { "JSR",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0x8d },
  { "LDA",     OPCAT_ACCARITH,    0x86 },
  { "LDAA",    OPCAT_ARITH,       0x86 },
  { "LDAB",    OPCAT_ARITH,       0xc6 },
  { "LDB",     OPCAT_ARITH,       0xc6 },
  { "LDD",     OPCAT_DBLREG1BYTE, 0xcc },
  { "LDS",     OPCAT_DBLREG1BYTE, 0x8e },
  { "LDX",     OPCAT_DBLREG1BYTE, 0xce },
  { "LIB",     OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LIBRARY", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LSL",     OPCAT_ACCADDR,     0x08 },
  { "LSLA",    OPCAT_ONEBYTE,     0x48 },
  { "LSLB",    OPCAT_ONEBYTE,     0x58 },
  { "LSLD",    OPCAT_TWOBYTE,     0x05 },
  { "LSR",     OPCAT_ACCADDR,     0x04 },
  { "LSRA",    OPCAT_ONEBYTE,     0x44 },
  { "LSRB",    OPCAT_ONEBYTE,     0x54 },
  { "LSRD",    OPCAT_ONEBYTE,     0x04 },
  { "MACRO",   OPCAT_PSEUDO,      PSEUDO_MACRO },
  { "MUL",     OPCAT_ONEBYTE,     0x3d },
  { "NAM",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "NAME",    OPCAT_PSEUDO,      PSEUDO_NAME },
  { "NEG",     OPCAT_ACCADDR,     0x00 },
  { "NEGA",    OPCAT_ONEBYTE,     0x40 },
  { "NEGB",    OPCAT_ONEBYTE,     0x50 },
  { "NOP",     OPCAT_ONEBYTE,     0x01 },
  { "OIM",     OPCAT_6301 |
               OPCAT_BITDIRECT,   0x02 }, 
  { "OPT",     OPCAT_PSEUDO,      PSEUDO_OPT },
  { "OPTION",  OPCAT_PSEUDO,      PSEUDO_OPT },
  { "ORA",     OPCAT_ARITH,       0x8a },
  { "ORAA",    OPCAT_ARITH,       0x8a },
  { "ORAB",    OPCAT_ARITH,       0xca },
  { "ORB",     OPCAT_ARITH,       0xca },
  { "ORG",     OPCAT_PSEUDO,      PSEUDO_ORG },
  { "PAG",     OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PAGE",    OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PEMT",    OPCAT_PSEUDO,      PSEUDO_PEMT },
  { "PHASE",   OPCAT_PSEUDO,      PSEUDO_PHASE },
  { "PSH",     OPCAT_OSTACK,      0x34 },
  { "PSHA",    OPCAT_ONEBYTE,     0x36 },
  { "PSHB",    OPCAT_ONEBYTE,     0x37 },
  { "PSHX",    OPCAT_ONEBYTE,     0x3c },
  { "PUB",     OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUBLIC",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUL",     OPCAT_OSTACK,      0x30 },
  { "PULA",    OPCAT_ONEBYTE,     0x32 },
  { "PULB",    OPCAT_ONEBYTE,     0x33 },
  { "PULX",    OPCAT_ONEBYTE,     0x38 },
//{ "REG",     OPCAT_PSEUDO,      PSEUDO_REG },    // not needed
  { "REP",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "REPEAT",  OPCAT_PSEUDO,      PSEUDO_REP },
  { "RMB",     OPCAT_PSEUDO,      PSEUDO_RMB },
  { "ROL",     OPCAT_ACCADDR,     0x09 },
  { "ROLA",    OPCAT_ONEBYTE,     0x49 },
  { "ROLB",    OPCAT_ONEBYTE,     0x59 },
  { "ROR",     OPCAT_ACCADDR,     0x06 },
  { "RORA",    OPCAT_ONEBYTE,     0x46 },
  { "RORB",    OPCAT_ONEBYTE,     0x56 },
  { "RPT",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "RTI",     OPCAT_ONEBYTE,     0x3b },
  { "RTS",     OPCAT_ONEBYTE,     0x39 },
  { "RZB",     OPCAT_PSEUDO,      PSEUDO_RZB },
  { "SBA",     OPCAT_ONEBYTE,     0x10 },
  { "SBC",     OPCAT_ACCARITH,    0x82 },
  { "SBCA",    OPCAT_ARITH,       0x82 },
  { "SBCB",    OPCAT_ARITH,       0xc2 },
  { "SEC",     OPCAT_ONEBYTE,     0x0d },
  { "SEI",     OPCAT_ONEBYTE,     0x0f },
  { "SET",     OPCAT_PSEUDO,      PSEUDO_SET },
//{ "SETDP",   OPCAT_PSEUDO,      PSEUDO_SETDP },
  { "SETLI",   OPCAT_PSEUDO,      PSEUDO_SETLI },
  { "SETPG",   OPCAT_PSEUDO,      PSEUDO_SETPG },
  { "SEV",     OPCAT_ONEBYTE,     0x0b },
  { "SLP",     OPCAT_6301 |
               OPCAT_ONEBYTE,     0x1a },
  { "SPC",     OPCAT_PSEUDO,      PSEUDO_SPC },
  { "STA",     OPCAT_NOIMM |
               OPCAT_ACCARITH,    0x87 },
  { "STAA",    OPCAT_NOIMM |
               OPCAT_ARITH,       0x87 },
  { "STAB",    OPCAT_NOIMM |
               OPCAT_ARITH,       0xc7 },
  { "STD",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcd },
  { "STS",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0x8f },
  { "STTL",    OPCAT_PSEUDO,      PSEUDO_STTL },
  { "STX",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcf },
  { "SUB",     OPCAT_ACCARITH,    0x80 },
  { "SUBA",    OPCAT_ARITH,       0x80 },
  { "SUBB",    OPCAT_ARITH,       0xc0 },
  { "SUBD",    OPCAT_DBLREG1BYTE, 0x83 },
  { "SWI",     OPCAT_ONEBYTE,     0x3f },
  { "SYMLEN",  OPCAT_PSEUDO,      PSEUDO_SYMLEN },
  { "TAB",     OPCAT_ONEBYTE,     0x16 },
  { "TAP",     OPCAT_ONEBYTE,     0x06 },
  { "TBA",     OPCAT_ONEBYTE,     0x17 },
  { "TEXT",    OPCAT_PSEUDO,      PSEUDO_TEXT },
  { "TIM",     OPCAT_6301 |
               OPCAT_BITDIRECT,   0x0b }, 
  { "TITLE",   OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TPA",     OPCAT_ONEBYTE,     0x07 },
  { "TST",     OPCAT_ACCADDR,     0x0d },
  { "TSTA",    OPCAT_ONEBYTE,     0x4d },
  { "TSTB",    OPCAT_ONEBYTE,     0x5d },
  { "TSX",     OPCAT_ONEBYTE,     0x30 },
  { "TTL",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TXS",     OPCAT_ONEBYTE,     0x35 },
  { "WAI",     OPCAT_ONEBYTE,     0x3e },
  { "WRN",     OPCAT_PSEUDO,      PSEUDO_WRN },
  { "XGDX",    OPCAT_6301 |
               OPCAT_ONEBYTE,     0x18 },
  { "ZMB",     OPCAT_PSEUDO,      PSEUDO_RZB },
  };

struct oprecord optable11[]=
  {
  { "ABA",     OPCAT_ONEBYTE,     0x1b },
  { "ABS",     OPCAT_PSEUDO,      PSEUDO_ABS },
  { "ABX",     OPCAT_ONEBYTE,     0x3a },
  { "ABY",     OPCAT_TWOBYTE,     0x183a },
  { "ADC",     OPCAT_ACCARITH,    0x89 },
  { "ADCA",    OPCAT_ARITH,       0x89 },
  { "ADCB",    OPCAT_ARITH,       0xc9 },
  { "ADD",     OPCAT_ACCARITH,    0x8b },
  { "ADDA",    OPCAT_ARITH,       0x8b },
  { "ADDB",    OPCAT_ARITH,       0xcb },
  { "ADDD",    OPCAT_DBLREG1BYTE, 0xc3 },
  { "AND",     OPCAT_ACCARITH,    0x84 },
  { "ANDA",    OPCAT_ARITH,       0x84 },
  { "ANDB",    OPCAT_ARITH,       0xc4 },
  { "ASL",     OPCAT_ACCADDR,     0x08 },
  { "ASLA",    OPCAT_ONEBYTE,     0x48 },
  { "ASLB",    OPCAT_ONEBYTE,     0x58 },
  { "ASLD",    OPCAT_ONEBYTE,     0x05 },
  { "ASR",     OPCAT_ACCADDR,     0x07 },
  { "ASRA",    OPCAT_ONEBYTE,     0x47 },
  { "ASRB",    OPCAT_ONEBYTE,     0x57 },
  { "ASRD",    OPCAT_TWOBYTE,     0x4756 },  // ASRA RORB
  { "BCC",     OPCAT_SBRANCH,     0x24 },
  { "BCLR",    OPCAT_SETMASK,     0x15 },
  { "BCS",     OPCAT_SBRANCH,     0x25 },
  { "BEC",     OPCAT_SBRANCH,     0x24 },
  { "BEQ",     OPCAT_SBRANCH,     0x27 },
  { "BES",     OPCAT_SBRANCH,     0x25 },
  { "BGE",     OPCAT_SBRANCH,     0x2c },
  { "BGT",     OPCAT_SBRANCH,     0x2e },
  { "BHI",     OPCAT_SBRANCH,     0x22 },
  { "BHS",     OPCAT_SBRANCH,     0x24 },
  { "BIN",     OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BINARY",  OPCAT_PSEUDO,      PSEUDO_BINARY },
  { "BIT",     OPCAT_ACCARITH,    0x85 },
  { "BITA",    OPCAT_ARITH,       0x85 },
  { "BITB",    OPCAT_ARITH,       0xc5 },
  { "BLE",     OPCAT_SBRANCH,     0x2f },
  { "BLO",     OPCAT_SBRANCH,     0x25 },
  { "BLS",     OPCAT_SBRANCH,     0x23 },
  { "BLT",     OPCAT_SBRANCH,     0x2d },
  { "BMI",     OPCAT_SBRANCH,     0x2b },
  { "BNE",     OPCAT_SBRANCH,     0x26 },
  { "BPL",     OPCAT_SBRANCH,     0x2a },
  { "BRA",     OPCAT_SBRANCH,     0x20 },
  { "BRA",     OPCAT_SBRANCH,     0x20 },
  { "BRCLR",   OPCAT_BRMASK,      0x13 },
  { "BRN",     OPCAT_SBRANCH,     0x21 },
  { "BRSET",   OPCAT_BRMASK,      0x12 },
  { "BSET",    OPCAT_SETMASK,     0x14 },
  { "BSR",     OPCAT_SBRANCH,     0x8d },
  { "BSZ",     OPCAT_PSEUDO,      PSEUDO_RZB },  // AS9 style
  { "BVC",     OPCAT_SBRANCH,     0x28 },
  { "BVS",     OPCAT_SBRANCH,     0x29 },
  { "CBA",     OPCAT_ONEBYTE,     0x11 },
  { "CLC",     OPCAT_ONEBYTE,     0x0c },
  { "CLI",     OPCAT_ONEBYTE,     0x0e },
  { "CLR",     OPCAT_ACCADDR,     0x0f },
  { "CLRA",    OPCAT_ONEBYTE,     0x4f },
  { "CLRB",    OPCAT_ONEBYTE,     0x5f },
  { "CLRD",    OPCAT_TWOBYTE,     0x4f5f },  // CLRA CLRB
  { "CLV",     OPCAT_ONEBYTE,     0x0a },
  { "CMP",     OPCAT_ACCARITH,    0x81 },
  { "CMPA",    OPCAT_ARITH,       0x81 },
  { "CMPB",    OPCAT_ARITH,       0xc1 },
  { "COM",     OPCAT_ACCADDR,     0x03 },
  { "COMA",    OPCAT_ONEBYTE,     0x43 },
  { "COMB",    OPCAT_ONEBYTE,     0x53 },
  { "CPD",     OPCAT_PAGE1A |
               OPCAT_DBLREG1BYTE, 0x83 },
  { "CPX",     OPCAT_DBLREG1BYTE, 0x8c },
  { "CPY",     OPCAT_PAGE18 |
               OPCAT_DBLREG1BYTE, 0x8c },
  { "DAA",     OPCAT_ONEBYTE,     0x19 },
  { "DEC",     OPCAT_ACCADDR,     0x0a },
  { "DECA",    OPCAT_ONEBYTE,     0x4a },
  { "DECB",    OPCAT_ONEBYTE,     0x5a },
  { "DECD",    OPCAT_FOURBYTE,    0xc0018200 },  // SUBB#1 SBCA#0
  { "DEF",     OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEFINE",  OPCAT_PSEUDO,      PSEUDO_DEF },
  { "DEPHASE", OPCAT_PSEUDO,      PSEUDO_DEPHASE },
  { "DES",     OPCAT_ONEBYTE,     0x34 },
  { "DEX",     OPCAT_ONEBYTE,     0x09 },
  { "DEY",     OPCAT_TWOBYTE,     0x1809 },
  { "DUP",     OPCAT_PSEUDO,      PSEUDO_DUP },
  { "ELSE",    OPCAT_PSEUDO,      PSEUDO_ELSE },
  { "END",     OPCAT_PSEUDO,      PSEUDO_END },
  { "ENDCOM",  OPCAT_PSEUDO,      PSEUDO_ENDCOM },
  { "ENDD",    OPCAT_PSEUDO,      PSEUDO_ENDD },
  { "ENDDEF",  OPCAT_PSEUDO,      PSEUDO_ENDDEF },
  { "ENDIF",   OPCAT_PSEUDO,      PSEUDO_ENDIF },
  { "ENDM",    OPCAT_PSEUDO,      PSEUDO_ENDM },
  { "EOR",     OPCAT_ACCARITH,    0x88 },
  { "EORA",    OPCAT_ARITH,       0x88 },
  { "EORB",    OPCAT_ARITH,       0xc8 },
  { "EQU",     OPCAT_PSEUDO,      PSEUDO_EQU },
  { "ERR",     OPCAT_PSEUDO,      PSEUDO_ERR },
  { "EXITM",   OPCAT_PSEUDO,      PSEUDO_EXITM },
  { "EXT",     OPCAT_PSEUDO,      PSEUDO_EXT },
  { "EXTERN",  OPCAT_PSEUDO,      PSEUDO_EXT },
  { "FCB",     OPCAT_PSEUDO,      PSEUDO_FCB },
  { "FCC",     OPCAT_PSEUDO,      PSEUDO_FCC },
  { "FCW",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FDB",     OPCAT_PSEUDO,      PSEUDO_FCW },
  { "FDIV",    OPCAT_ONEBYTE,     0x03 },
  { "FILL",    OPCAT_PSEUDO,      PSEUDO_FILL },
  { "GLOBAL",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "IDIV",    OPCAT_ONEBYTE,     0x02 },
  { "IF",      OPCAT_PSEUDO,      PSEUDO_IF },
  { "IFC",     OPCAT_PSEUDO,      PSEUDO_IFC },
  { "IFD",     OPCAT_PSEUDO,      PSEUDO_IFD },
  { "IFN",     OPCAT_PSEUDO,      PSEUDO_IFN },
  { "IFNC",    OPCAT_PSEUDO,      PSEUDO_IFNC },
  { "IFND",    OPCAT_PSEUDO,      PSEUDO_IFND },
  { "INC",     OPCAT_ACCADDR,     0x0c },
  { "INCA",    OPCAT_ONEBYTE,     0x4c },
  { "INCB",    OPCAT_ONEBYTE,     0x5c },
  { "INCD",    OPCAT_FOURBYTE,    0xcb018900 },  // ADDB#1 ADCA#0
  { "INCLUDE", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "INS",     OPCAT_ONEBYTE,     0x31 },
  { "INX",     OPCAT_ONEBYTE,     0x08 },
  { "INY",     OPCAT_TWOBYTE,     0x1808 },
  { "JMP",     OPCAT_IDXEXT,      0x4e },
  { "JSR",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0x8d },
  { "LDA",     OPCAT_ACCARITH,    0x86 },
  { "LDAA",    OPCAT_ARITH,       0x86 },
  { "LDAB",    OPCAT_ARITH,       0xc6 },
  { "LDB",     OPCAT_ARITH,       0xc6 },
  { "LDD",     OPCAT_DBLREG1BYTE, 0xcc },
  { "LDS",     OPCAT_DBLREG1BYTE, 0x8e },
  { "LDX",     OPCAT_DBLREG1BYTE, 0xce },
  { "LDY",     OPCAT_PAGE18 |
               OPCAT_DBLREG1BYTE, 0xce },
  { "LIB",     OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LIBRARY", OPCAT_PSEUDO,      PSEUDO_INCLUDE },
  { "LSL",     OPCAT_ACCADDR,     0x08 },
  { "LSLA",    OPCAT_ONEBYTE,     0x48 },
  { "LSLB",    OPCAT_ONEBYTE,     0x58 },
  { "LSLD",    OPCAT_ONEBYTE,     0x05 },
  { "LSR",     OPCAT_ACCADDR,     0x04 },
  { "LSRA",    OPCAT_ONEBYTE,     0x44 },
  { "LSRB",    OPCAT_ONEBYTE,     0x54 },
  { "LSRD",    OPCAT_ONEBYTE,     0x04 },
  { "MACRO",   OPCAT_PSEUDO,      PSEUDO_MACRO },
  { "MUL",     OPCAT_ONEBYTE,     0x3d },
  { "NAM",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "NAME",    OPCAT_PSEUDO,      PSEUDO_NAME },
  { "NEG",     OPCAT_ACCADDR,     0x00 },
  { "NEGA",    OPCAT_ONEBYTE,     0x40 },
  { "NEGB",    OPCAT_ONEBYTE,     0x50 },
  { "NOP",     OPCAT_ONEBYTE,     0x01 },
  { "OPT",     OPCAT_PSEUDO,      PSEUDO_OPT },
  { "OPTION",  OPCAT_PSEUDO,      PSEUDO_OPT },
  { "ORA",     OPCAT_ARITH,       0x8a },
  { "ORAA",    OPCAT_ARITH,       0x8a },
  { "ORAB",    OPCAT_ARITH,       0xca },
  { "ORB",     OPCAT_ARITH,       0xca },
  { "ORG",     OPCAT_PSEUDO,      PSEUDO_ORG },
  { "PAG",     OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PAGE",    OPCAT_PSEUDO,      PSEUDO_PAG },
  { "PEMT",    OPCAT_PSEUDO,      PSEUDO_PEMT },
  { "PHASE",   OPCAT_PSEUDO,      PSEUDO_PHASE },
  { "PSH",     OPCAT_OSTACK,      0x34 },
  { "PSHA",    OPCAT_ONEBYTE,     0x36 },
  { "PSHB",    OPCAT_ONEBYTE,     0x37 },
  { "PSHX",    OPCAT_ONEBYTE,     0x3c },
  { "PSHY",    OPCAT_TWOBYTE,     0x183c },
  { "PUB",     OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUBLIC",  OPCAT_PSEUDO,      PSEUDO_PUB },
  { "PUL",     OPCAT_OSTACK,      0x30 },
  { "PULA",    OPCAT_ONEBYTE,     0x32 },
  { "PULB",    OPCAT_ONEBYTE,     0x33 },
  { "PULX",    OPCAT_ONEBYTE,     0x38 },
  { "PULY",    OPCAT_TWOBYTE,     0x1838 },
//{ "REG",     OPCAT_PSEUDO,      PSEUDO_REG },    // not needed
  { "REP",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "REPEAT",  OPCAT_PSEUDO,      PSEUDO_REP },
  { "RMB",     OPCAT_PSEUDO,      PSEUDO_RMB },
  { "ROL",     OPCAT_ACCADDR,     0x09 },
  { "ROLA",    OPCAT_ONEBYTE,     0x49 },
  { "ROLB",    OPCAT_ONEBYTE,     0x59 },
  { "ROR",     OPCAT_ACCADDR,     0x06 },
  { "RORA",    OPCAT_ONEBYTE,     0x46 },
  { "RORB",    OPCAT_ONEBYTE,     0x56 },
  { "RPT",     OPCAT_PSEUDO,      PSEUDO_REP },
  { "RTI",     OPCAT_ONEBYTE,     0x3b },
  { "RTS",     OPCAT_ONEBYTE,     0x39 },
  { "RZB",     OPCAT_PSEUDO,      PSEUDO_RZB },
  { "SBA",     OPCAT_ONEBYTE,     0x10 },
  { "SBC",     OPCAT_ACCARITH,    0x82 },
  { "SBCA",    OPCAT_ARITH,       0x82 },
  { "SBCB",    OPCAT_ARITH,       0xc2 },
  { "SEC",     OPCAT_ONEBYTE,     0x0d },
  { "SEI",     OPCAT_ONEBYTE,     0x0f },
  { "SET",     OPCAT_PSEUDO,      PSEUDO_SET },
  { "SETLI",   OPCAT_PSEUDO,      PSEUDO_SETLI },
  { "SETPG",   OPCAT_PSEUDO,      PSEUDO_SETPG },
  { "SEV",     OPCAT_ONEBYTE,     0x0b },
  { "SPC",     OPCAT_PSEUDO,      PSEUDO_SPC },
  { "STA",     OPCAT_NOIMM |
               OPCAT_ACCARITH,    0x87 },
  { "STAA",    OPCAT_NOIMM |
               OPCAT_ARITH,       0x87 },
  { "STAB",    OPCAT_NOIMM |
               OPCAT_ARITH,       0xc7 },
  { "STD",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcd },
  { "STOP",    OPCAT_ONEBYTE,     0xcf },
  { "STS",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0x8f },
  { "STTL",    OPCAT_PSEUDO,      PSEUDO_STTL },
  { "STX",     OPCAT_NOIMM |
               OPCAT_DBLREG1BYTE, 0xcf },
  { "STY",     OPCAT_NOIMM |
               OPCAT_PAGE18 |
               OPCAT_DBLREG1BYTE, 0xcf },
  { "SUB",     OPCAT_ACCARITH,    0x80 },
  { "SUBA",    OPCAT_ARITH,       0x80 },
  { "SUBB",    OPCAT_ARITH,       0xc0 },
  { "SUBD",    OPCAT_DBLREG1BYTE, 0x83 },
  { "SWI",     OPCAT_ONEBYTE,     0x3f },
  { "SYMLEN",  OPCAT_PSEUDO,      PSEUDO_SYMLEN },
  { "TAB",     OPCAT_ONEBYTE,     0x16 },
  { "TAP",     OPCAT_ONEBYTE,     0x06 },
  { "TBA",     OPCAT_ONEBYTE,     0x17 },
  { "TEST",    OPCAT_ONEBYTE,     0x00 },
  { "TEXT",    OPCAT_PSEUDO,      PSEUDO_TEXT },
  { "TITLE",   OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TPA",     OPCAT_ONEBYTE,     0x07 },
  { "TST",     OPCAT_ACCADDR,     0x0d },
  { "TSTA",    OPCAT_ONEBYTE,     0x4d },
  { "TSTB",    OPCAT_ONEBYTE,     0x5d },
  { "TSX",     OPCAT_ONEBYTE,     0x30 },
  { "TSY",     OPCAT_TWOBYTE,     0x1830 },
  { "TTL",     OPCAT_PSEUDO,      PSEUDO_NAM },
  { "TXS",     OPCAT_ONEBYTE,     0x35 },
  { "TYS",     OPCAT_TWOBYTE,     0x1835 },
  { "WAI",     OPCAT_ONEBYTE,     0x3e },
  { "WRN",     OPCAT_PSEUDO,      PSEUDO_WRN },
  { "XGDX",    OPCAT_ONEBYTE,     0x8f },
  { "XGDY",    OPCAT_TWOBYTE,     0x188f },
  { "ZMB",     OPCAT_PSEUDO,      PSEUDO_RZB },
  };

/* expression categories...
   all zeros is ordinary constant.
   bit 1 indicates address within module.
   bit 2 indicates external address.
   bit 4 indicates this can't be relocated if it's an address.
   bit 5 indicates address (if any) is negative.
   bit 6 indicates a single-byte external (if set with bit 2)
   bit 7 indicates the external address is an offset
*/ 

#define EXPRCAT_INTADDR       0x02
#define EXPRCAT_EXTADDR       0x04
#define EXPRCAT_PUBLIC        0x08
#define EXPRCAT_FIXED         0x10
#define EXPRCAT_NEGATIVE      0x20
#define EXPRCAT_ONEBYTE       0x40
#define EXPRCAT_EXTOFF        0x80

/*****************************************************************************/
/* Symbol definitions                                                        */
/*****************************************************************************/

struct symrecord
  {
  char name[MAXIDLEN + 1];              /* symbol name                       */
  char cat;                             /* symbol category                   */
  unsigned short value;                 /* symbol value                      */
  union
    {
    struct symrecord *parent;           /* parent symbol (for COMMON)        */
    long flags;                         /* forward reference flag (otherwise)*/
    } u;
  };

struct symtable
  {
  long counter;                         /* # entries in table                */
  struct symrecord rec[MAXLABELS];      /* symbol records (fixed size)       */
  };
                                        /* symbol categories :               */
#define SYMCAT_CONSTANT       0x00      /* constant value (from equ)         */
#define SYMCAT_VARIABLE       0x01      /* variable value (from set)         */
#define SYMCAT_LABEL          0x02      /* address within module (label)     */
#define SYMCAT_VARADDR        0x03      /* variable containing address       */
#define SYMCAT_EXTERN         0x04      /* address in other module (extern)  */
#define SYMCAT_VAREXTERN      0x05      /* variable containing extern addr.  */
#define SYMCAT_UNRESOLVED     0x06      /* unresolved address                */
#define SYMCAT_VARUNRESOLVED  0x07      /* var. containing unresolved addr   */
#define SYMCAT_PUBLIC         0x08      /* public label                      */
#define SYMCAT_MACRO          0x09      /* macro definition                  */
#define SYMCAT_PUBLICUNDEF    0x0A      /* public label (yet undefined)      */
#define SYMCAT_PARMNAME       0x0B      /* parameter name                    */
#define SYMCAT_EMPTY          0x0D      /* empty                             */
#define SYMCAT_REG            0x0E      /* REG directive                     */
#define SYMCAT_TEXT           0x0F      /* /Dsymbol or TEXT label            */
#define SYMCAT_COMMONDATA     0x12      /* COMMON Data                       */
#define SYMCAT_COMMON         0x14      /* COMMON block                      */
#define SYMCAT_LOCALLABEL     0x22      /* local label                       */
#define SYMCAT_EMPTYLOCAL     0x26      /* empty local label                 */
                                        /* symbol flags:                     */
#define SYMFLAG_FORWARD       0x01      /* forward reference                 */
#define SYMFLAG_PASSED        0x02      /* passed forward reference          */
#define SYMFLAG_ABSOLUTE      0x04      /* absolute public label             */

struct symtable symtable = {0};         /* symbol table                      */
struct symtable lcltable = {0};         /* local symbol table (fixed size)   */
  
/*****************************************************************************/
/* regrecord structure definition                                            */
/*****************************************************************************/

struct regrecord
  {
  char *name;                           /* name of the register              */
  unsigned char tfr, psh;               /* bit value for tfr and psh/pul     */
  };

/*****************************************************************************/
/* regtable : table of all registers                                         */
/*****************************************************************************/

struct regrecord regtable09[]=
  {
  { "D",   0x00, 0x06 },
  { "X",   0x01, 0x10 },
  { "Y",   0x02, 0x20 },
  { "U",   0x03, 0x40 },
  { "S",   0x04, 0x40 },
  { "PC",  0x05, 0x80 },
  { "A",   0x08, 0x02 },
  { "B",   0x09, 0x04 },
  { "CC",  0x0a, 0x01 },
  { "CCR", 0x0a, 0x01 },
  { "DP",  0x0b, 0x08 },
  { "DPR", 0x0b, 0x08 },
  { 0,     0,    0    }
  };

struct regrecord regtable63[]=          /* same for HD6309                   */
  {
  { "D",   0x00, 0x06 },
  { "X",   0x01, 0x10 },
  { "Y",   0x02, 0x20 },
  { "U",   0x03, 0x40 },
  { "S",   0x04, 0x40 },
  { "PC",  0x05, 0x80 },
  { "W",   0x06, 0x00 },
  { "V",   0x07, 0x00 },
  { "A",   0x08, 0x02 },
  { "B",   0x09, 0x04 },
  { "CC",  0x0a, 0x01 },
  { "CCR", 0x0a, 0x01 },
  { "DP",  0x0b, 0x08 },
  { "DPR", 0x0b, 0x08 },
  { "0",   0x0c, 0x00 },
  { "E",   0x0e, 0x00 },
  { "F",   0x0f, 0x00 },
  { 0,     0,    0    }
  };

struct regrecord regtable00[]=
  {
  { "X",   0x01, 0x00 },
  { "S",   0x04, 0x00 },
  { "PC",  0x05, 0x00 },
  { "A",   0x08, 0x02 },
  { "B",   0x09, 0x03 },
  { "CC",  0x0a, 0x00 },
  { "CCR", 0x0a, 0x00 },
  { 0,     0,    0    }
  };

struct regrecord regtable01[]=
  {
  { "X",   0x01, 0x08 },
  { "S",   0x04, 0x00 },
  { "PC",  0x05, 0x00 },
  { "A",   0x08, 0x02 },
  { "B",   0x09, 0x03 },
  { "CC",  0x0a, 0x00 },
  { "CCR", 0x0a, 0x00 },
  { 0,     0,    0    }
  };

struct regrecord regtable11[]=
  {
  { "X",   0x01, 0x08 },
  { "Y",   0x02, 0x88 },
  { "S",   0x04, 0x00 },
  { "PC",  0x05, 0x00 },
  { "A",   0x08, 0x02 },
  { "B",   0x09, 0x03 },
  { "CC",  0x0a, 0x00 },
  { "CCR", 0x0a, 0x00 },
  { 0,     0,    0    }
  };

/*****************************************************************************/
/* bitregtable : table of all bit transfer registers                         */
/*****************************************************************************/

struct regrecord bitregtable09[] =
  {
  { "CC",  0x00, 0x00 },
  { "A",   0x01, 0x01 },
  { "B",   0x02, 0x02 },
  };

struct regrecord bitregtable00[] =
  {
  { "CC",  0x00, 0x00 },
  { "A",   0x01, 0x01 },
  { "B",   0x02, 0x02 },
  };

/*****************************************************************************/
/* relocrecord structure definition                                          */
/*****************************************************************************/

struct relocrecord
  {
  unsigned short addr;                  /* address that needs relocation     */
  char exprcat;                         /* expression category               */
  struct symrecord *sym;                /* symbol for relocation             */
  };

long relcounter = 0;                    /* # currently defined relocations   */
struct relocrecord reltable[MAXRELOCS]; /* relocation table (fixed size)     */
long relhdrfoff;                        /* FLEX Relocatable Global Hdr Offset*/
long reldataorg = -2;                   /* org for rel data block in abs mode*/
long reldatasize = 0;                   /* size of rel data block            */
long relabsfoff = -1;                   /* current abs block offset          */

/*****************************************************************************/
/* operandrecord structure definition                                        */
/*****************************************************************************/

struct operandrecord
  {
  struct relocrecord p;                 /* possible relocation record        */
  };

/*****************************************************************************/
/* Options definitions                                                       */
/*****************************************************************************/

#define OPTION_M09    0x00000001L       /* MC6809 mode                       */
#define OPTION_H09    0x00000002L       /* HD6309 mode                       */
#define OPTION_M00    0x00000004L       /* MC6800 mode                       */
#define OPTION_M01    0x00000008L       /* MC6801 mode                       */
#define OPTION_H01    0x00000010L       /* HD6301 mode                       */
#define OPTION_PAG    0x00000020L       /* page formatting ON                */
#define OPTION_CON    0x00000040L       /* print cond. skipped code          */
#define OPTION_MAC    0x00000080L       /* print macro calling line (default)*/
#define OPTION_EXP    0x00000100L       /* print macro expansion lines       */
#define OPTION_SYM    0x00000200L       /* print symbol table (default)      */
#define OPTION_MUL    0x00000400L       /* print multiple oc lines (default) */
#define OPTION_LP1    0x00000800L       /* print pass 1 listing              */
#define OPTION_DAT    0x00001000L       /* print date in listing (default)   */
#define OPTION_NUM    0x00002000L       /* print line numbers                */
#define OPTION_INV    0x00004000L       /* print invisible lines             */
#define OPTION_TSC    0x00008000L       /* TSC-compatible source format      */
#define OPTION_WAR    0x00010000L       /* print warnings                    */
#define OPTION_CLL    0x00020000L       /* check line length (default)       */
#define OPTION_LFN    0x00040000L       /* print long file names             */
#define OPTION_LLL    0x00080000L       /* list library lines                */
#define OPTION_GAS    0x00100000L       /* Gnu AS compatibility              */
#define OPTION_REL    0x00200000L       /* print relocation table            */
#define OPTION_TXT    0x00400000L       /* print text table                  */
#define OPTION_LIS    0x00800000L       /* print assembler output listing    */
#define OPTION_LPA    0x01000000L       /* listing in f9dasm patch format    */
#define OPTION_DLM    0x02000000L       /* define label on macro expansion   */
#define OPTION_H11    0x04000000L       /* 68HC11 mode                       */
#define OPTION_RED    0x08000000L       /* redefine label if code label, too */
#define OPTION_FBG    0x10000000L       /* fill binary gaps                  */
#define OPTION_UEX    0x20000000L       /* undefined is treated as external  */

struct
  {
  char *Name;
  unsigned long dwAdd;
  unsigned long dwRem;
  } Options[] =
  {/*Name          Add      Remove */
  { "PAG",  OPTION_PAG,          0 },
  { "NOP",           0, OPTION_PAG },
  { "CON",  OPTION_CON,          0 },
  { "NOC",           0, OPTION_CON },
  { "MAC",  OPTION_MAC,          0 },
  { "NOM",           0, OPTION_MAC },
  { "EXP",  OPTION_EXP,          0 },
  { "NOE",           0, OPTION_EXP },
  { "SYM",  OPTION_SYM,          0 },
  { "NOS",           0, OPTION_SYM },
  { "MUL",  OPTION_MUL,          0 },
  { "NMU",           0, OPTION_MUL },
  { "LP1",  OPTION_LP1,          0 },
  { "NO1",           0, OPTION_LP1 },
  { "DAT",  OPTION_DAT,          0 },
  { "NOD",           0, OPTION_DAT },
  { "NUM",  OPTION_NUM,          0 },
  { "NON",           0, OPTION_NUM },
  { "INV",  OPTION_INV,          0 },
  { "NOI",           0, OPTION_INV },
  { "TSC",  OPTION_TSC,          0 },
  { "NOT",           0, OPTION_TSC },
  { "WAR",  OPTION_WAR,          0 },
  { "NOW",           0, OPTION_WAR },
  { "CLL",  OPTION_CLL,          0 },
  { "NCL",           0, OPTION_CLL },
  { "LFN",  OPTION_LFN,          0 },
  { "NLF",           0, OPTION_LFN },
  { "LLL",  OPTION_LLL,          0 },
  { "NLL",           0, OPTION_LLL },
  { "GAS",  OPTION_GAS,          0 },
  { "NOG",           0, OPTION_GAS },
  { "REL",  OPTION_REL,          0 },
  { "NOR",           0, OPTION_REL },
  { "H63",  OPTION_H09, OPTION_H01 | OPTION_M09 | OPTION_M00 | OPTION_M01 | OPTION_H11 },
  { "H09",  OPTION_H09, OPTION_H01 | OPTION_M09 | OPTION_M00 | OPTION_M01 | OPTION_H11 },
  { "M68",  OPTION_M09, OPTION_H01 | OPTION_H09 | OPTION_M00 | OPTION_M01 | OPTION_H11 },
  { "M09",  OPTION_M09, OPTION_H01 | OPTION_H09 | OPTION_M00 | OPTION_M01 | OPTION_H11 },
  { "M00",  OPTION_M00, OPTION_H01 | OPTION_H09 | OPTION_M09 | OPTION_M01 | OPTION_H11 },
  { "M02",  OPTION_M00, OPTION_H01 | OPTION_H09 | OPTION_M09 | OPTION_M01 | OPTION_H11 },
  { "M08",  OPTION_M00, OPTION_H01 | OPTION_H09 | OPTION_M09 | OPTION_M01 | OPTION_H11 },
  { "M01",  OPTION_M01, OPTION_H01 | OPTION_H09 | OPTION_M09 | OPTION_M00 | OPTION_H11 },
  { "M03",  OPTION_M01, OPTION_H01 | OPTION_H09 | OPTION_M09 | OPTION_M00 | OPTION_H11 },
  { "H01",  OPTION_H01, OPTION_H09 | OPTION_M09 | OPTION_M00 | OPTION_M01 | OPTION_H11 },
  { "H03",  OPTION_H01, OPTION_H09 | OPTION_M09 | OPTION_M00 | OPTION_M01 | OPTION_H11 },
  { "H11",  OPTION_H11, OPTION_H09 | OPTION_M09 | OPTION_M00 | OPTION_M01 | OPTION_H01 },
  { "TXT",  OPTION_TXT,          0 },
  { "NTX",           0, OPTION_TXT },
  { "LIS",  OPTION_LIS,          0 },
  { "NOL",           0, OPTION_LIS },
  { "LPA",  OPTION_LPA | OPTION_EXP | OPTION_LLL,  // LPA enforces EXP / LLL
                        OPTION_NUM | OPTION_CLL }, // LPA inhibits NUM / CLL
  { "NLP",           0, OPTION_LPA },
  { "DLM",  OPTION_DLM,          0 },
  { "NDL",           0, OPTION_DLM },
  { "RED",  OPTION_RED,          0 },
  { "NRD",           0, OPTION_RED },
  { "FBG",  OPTION_FBG,          0 },
  { "NFB",           0, OPTION_FBG },
  { "UEX",  OPTION_UEX,          0 },
  { "NUE",           0, OPTION_UEX },
  };

unsigned long dwOptions =               /* options flags, init to default:   */
    OPTION_M09 |                        /* MC6809 mode                       */
    OPTION_MAC |                        /* print macro calling line          */
    OPTION_SYM |                        /* print symbol table                */
    OPTION_MUL |                        /* print multiple oc lines           */
    OPTION_DAT |                        /* print date in listing             */
    OPTION_WAR |                        /* print warnings                    */
    OPTION_CLL |                        /* check line length                 */
    OPTION_LLL |                        /* list library lines                */
    OPTION_REL |                        /* print relocation table            */
    OPTION_FBG |                        /* fill gaps in binary output        */
    OPTION_LIS;                         /* print assembler output listing    */

/*****************************************************************************/
/* arrays of error/warning messages                                          */
/*****************************************************************************/
                                        /* defined error flags               */
#define ERR_OK            0x0000        /* all is well                       */
#define ERR_EXPR          0x0001        /* Error in expression               */
#define ERR_ILLEGAL_ADDR  0x0002        /* Illegal adressing mode            */
#define ERR_LABEL_UNDEF   0x0004        /* Undefined label                   */
#define ERR_LABEL_MULT    0x0008        /* Label multiply defined            */
#define ERR_RANGE         0x0010        /* Relative branch out of range      */
#define ERR_LABEL_MISSING 0x0020        /* Missing label                     */
#define ERR_OPTION_UNK    0x0040        /* Option unknown                    */
#define ERR_MALLOC        0x0080        /* Out of memory                     */
#define ERR_NESTING       0x0100        /* Nesting not allowed               */
#define ERR_RELOCATING    0x0200        /* Statement not valid for reloc mode*/
#define ERR_ERRTXT        0x4000        /* ERR text output                   */
#define ERR_ILLEGAL_MNEM  0x8000        /* Illegal mnemonic                  */

char *errormsg[]=
  {
  "Error in expression",                /* 1     ERR_EXPR                    */
  "Illegal addressing mode",            /* 2     ERR_ILLEGAL_ADDR            */
  "Undefined label",                    /* 4     ERR_LABEL_UNDEF             */
  "Multiple definitions of label",      /* 8     ERR_LABEL_MULT              */
  "Relative branch out of range",       /* 16    ERR_RANGE                   */
  "Missing label",                      /* 32    ERR_LABEL_MISSING           */
  "Unknown option specified",           /* 64    ERR_OPTION_UNK              */
  "Out of memory",                      /* 128   ERR_MALLOC                  */
  "Nesting not allowed",                /* 256   ERR_NESTING                 */
  "Illegal for current relocation mode",/* 512   ERR_RELOCATING              */
  "",                                   /* 1024                              */
  "",                                   /* 2048                              */
  "",                                   /* 4096                              */
  "",                                   /* 8192                              */
  NULL,                                 /* 16384 ERR_ERRTXT (ERR output)     */
  "Illegal mnemonic"                    /* 32768 ERR_ILLEGAL_MNEM            */
  };

                                        /* defined warning flags             */
#define WRN_OK            0x0000        /* All OK                            */
#define WRN_OPT           0x0001        /* Long branch in short range        */
#define WRN_SYM           0x0002        /* Symbolic text undefined           */
#define WRN_AREA          0x0004        /* Area already in use               */
#define WRN_AMBIG         0x0008        /* Ambiguous 6800 opcode             */
#define WRN_ADR_TRUNC     0x0010        /* Forced address truncated          */
#define WRN_IMM_TRUNC     0x0020        /* Immediate value truncated         */
#define WRN_ILLF_IGN      0x0040        /* Illogical forcing ignored         */
#define WRN_REL_WRAP      0x0080        /* Relative address wraparound       */
#define WRN_WRNTXT        0x4000        /* WRN text output                   */

char *warningmsg[] =
  {
  "Long branch within short branch "    /* 1     WRN_OPT                     */
      "range could be optimized",
  "Symbolic text undefined",            /* 2     WRN_SYM                     */
  "Area already in use",                /* 4     WRN_AREA                    */
  "Ambiguous 6800 alternate notation",  /* 8     WRN_AMBIG                   */
  "Forced address truncated",           /* 16    WRN_ADR_TRUNC               */
  "Immediate value truncated",          /* 32    WRN_IMM_TRUNC               */
  "Illogical forcing ignored",          /* 64    WRN_ILLF_IGN                */
  "Relative addressing wraparound",     /* 128   WRN_REL_WRAP                */
  "",                                   /* 256                               */
  "",                                   /* 512                               */
  "",                                   /* 1024                              */
  "",                                   /* 2048                              */
  "",                                   /* 4096                              */
  "",                                   /* 8192                              */
  NULL,                                 /* 16384 WRN_WRNTXT (WRN output)     */
  ""                                    /* 32768                             */
  };

/*****************************************************************************/
/* Listing Definitions                                                       */
/*****************************************************************************/

#define LIST_OFF  0x00                  /* listing is generally off          */
#define LIST_ON   0x01                  /* listing is generally on           */

char listing = LIST_OFF;                /* listing flag                      */

/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/

FILE *listfile = NULL;                  /* list file                         */
FILE *objfile = NULL;                   /* object file                       */
char listname[FNLEN + 1];               /* list file name                    */
char objname[FNLEN + 1];                /* object file name                  */
char srcname[FNLEN + 1];                /* source file name                  */

                                        /* assembler mode specifics:         */
struct oprecord *optable = optable09;   /* used op table                     */
                                        /* size of this table                */
int optablesize = sizeof(optable09) / sizeof(optable09[0]);
struct regrecord *regtable = regtable09;/* used register table               */
                                        /* used bit register table           */
struct regrecord *bitregtable = bitregtable09;
int bitregtablesize = sizeof(bitregtable09) / sizeof(bitregtable09[0]);
void (*scanoperands)(struct relocrecord *, int);

char pass;                              /* Assembler pass = 1 or 2           */
char relocatable = 0;                   /* relocatable object flag           */
char absmode = 1;                       /* absolute mode                     */
long global = 0;                        /* all labels global flag            */
long common = 0;                        /* common definition flag            */
char rmbfillchr = 0;                    /* RMB fill character for binaries   */
struct symrecord * commonsym = NULL;    /* current common main symbol        */
char g_termflg;                         /* termination flag                  */
char generating;                        /* code generation flag              */
unsigned short loccounter,oldlc;        /* Location counter                  */
int phase;                              /* phase (offfset to ORG)            */

char inpline[LINELEN];                  /* Current input line (not expanded) */
char srcline[LINELEN];                  /* Current source line               */
char * srcptr;                          /* Pointer to line being parsed      */

char unknown;          /* flag to indicate value unknown */
char certain;          /* flag to indicate value is certain at pass 1*/
long error;            /* flags indicating errors in current line. */
long errors;           /* number of errors in current pass */
long warning;          /* flags indicating warnings in current line */
long warnings;         /* number of warnings in current pass */
long nTotErrors;                        /* total # of errors                 */
long nTotWarnings;                      /* total # warnings                  */
char exprcat;          /* category of expression being parsed, eg. 
                          label or constant, this is important when
                          generating relocatable object code. */

int maxidlen = MAXIDLEN;                /* maximum ID length                 */

char modulename[MAXIDLEN + 1] = "";     /* module name buffer                */
char namebuf[MAXIDLEN + 1];             /* name buffer for parsing           */
char unamebuf[MAXIDLEN + 1];            /* name buffer in uppercase          */

char mode;                              /* adressing mode:                   */
#define ADRMODE_IMM  0                  /* 0 = immediate                     */
#define ADRMODE_DIR  1                  /* 1 = direct                        */
#define ADRMODE_EXT  2                  /* 2 = extended                      */
#define ADRMODE_IDX  3                  /* 3 = indexed                       */
#define ADRMODE_POST 4                  /* 4 = postbyte                      */
#define ADRMODE_PCR  5                  /* 5 = PC relative (with postbyte)   */
#define ADRMODE_IND  6                  /* 6 = indirect                      */
#define ADRMODE_PIN  7                  /* 7 = PC relative & indirect        */

char opsize;                            /* desired operand size :            */
                                        /* 0=dunno,1=5, 2=8, 3=16            */
long operand;
unsigned char postbyte;

long dpsetting = 0;                     /* Direct Page Default = 0           */

unsigned char codebuf[256];
int codeptr;                            /* byte offset within instruction    */
int suppress;                           /* 0=no suppress                     */
                                        /* 1=until ENDIF                     */
                                        /* 2=until ELSE                      */
                                        /* 3=until ENDM                      */
char condline = 0;                      /* flag whether on conditional line  */
int ifcount;                            /* count of nested IFs within        */
                                        /* suppressed text                   */
int printovr = 0;                       /* print override flags:             */
#define PRINTOV_PADDR   1               /* print address in listing          */
#define PRINTOV_PLABEL  2               /* print label address in listing    */
#define PRINTOV_PDP     4               /* print direct page in listing      */
#define PRINTOV_PTFR    8               /* print transfer address in listing */
#define PRINTOV_PPHA   16               /* print phase in listing            */

#define OUT_NONE  -1                    /* no output                         */
#define OUT_BIN   0                     /* binary output                     */
#define OUT_SREC  1                     /* Motorola S-Records                */
#define OUT_IHEX  2                     /* Intel Hex Records                 */
#define OUT_FLEX  3                     /* Flex9 ASMB-compatible output      */
#define OUT_GAS   4                     /* GNU relocation output             */
#define OUT_REL   5                     /* Flex9 RELASMB output              */
int outmode = OUT_BIN;                  /* default to binary output          */

int hexmaxcount = 16;                   /* max. # bytes per S09 line         */
int ihexmaxcount = 32;                  /* max. # bytes per Intel hex line   */
int iflexmaxcount = 255;                /* max. # bytes per Flex09 record    */

unsigned short hexaddr;
int hexcount;
unsigned char hexbuffer[256];
unsigned int chksum;

int nRepNext = 0;                       /* # repetitions for REP pseudo-op   */
int nSkipCount = 0;                     /* # lines to skip                   */

unsigned short tfradr = 0;
int tfradrset = 0;

int nCurLine = 0;                       /* current output line on page       */
int nCurCol = 0;                        /* current output column on line     */
int nCurPage = 0;                       /* current page #                    */
int nLinesPerPage = 66;                 /* # lines on a page                 */
int nColsPerLine = 80;                  /* # columns per line                */
char szTitle[128] = "";                 /* title for listings                */
char szSubtitle[128] = "";              /* subtitle for listings             */

char szBuf1[LINELEN];                   /* general-purpose buffers for parse */
char szBuf2[LINELEN];

struct linebuf *macros[MAXMACROS];      /* pointers to the macros            */
int nMacros = 0;                        /* # parsed macros                   */
int inMacro = 0;                        /* flag whether in macro definition  */
int lvlMacro = 0;                       /* current macro expansion level     */

char *texts[MAXTEXTS];                  /* pointers to the texts             */
int nPredefinedTexts = 0;               /* # predefined texts                */
int nTexts = 0;                         /* # currently defined texts         */

unsigned char bUsedBytes[8192] = {0};   /* 1 bit per byte of the address spc */

/*****************************************************************************/
/* Necessary forward declarations                                            */
/*****************************************************************************/

struct linebuf *readfile(char *name, unsigned char lvl, struct linebuf *after);
struct linebuf *readbinary(char *name, unsigned char lvl, struct linebuf *after, struct symrecord *lp);

/*****************************************************************************/
/* allocline : allocates a line of text                                      */
/*****************************************************************************/

struct linebuf * allocline
    (
    struct linebuf *prev,
    char *fn,
    int line,
    unsigned char lvl,
    char *text
    )
{
    struct linebuf *pNew = (struct linebuf *)
        malloc(sizeof(struct linebuf) + strlen(text));
    if (!pNew)
      return NULL;
    pNew->next = (prev) ? prev->next : NULL;
    pNew->prev = prev;
    if (prev)
      prev->next = pNew;
    if (pNew->next)
      pNew->next->prev = pNew;
    pNew->lvl = lvl;
    pNew->flg = 0;
    pNew->fn = fn;
    pNew->ln = line;
    pNew->rel = ' ';
    strcpy(pNew->txt, text);
    return pNew;
}

