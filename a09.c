/* A09, 6809 Assembler. 
   
   (C) Copyright 1993,1994 L.C. Benschop. 
   Parts (C) Copyright 2001-2023 H. Seib.
   This version of the program is distributed under the terms and conditions 
   of the GNU General Public License version 2. See file the COPYING for details.   
   THERE IS NO WARRANTY ON THIS PROGRAM. 
    
   Generates binary image file from the lowest to
   the highest address with actually assembled data.
   
   Machine dependencies:
                  char is 8 bits.
                  short is 16 bits.
                  integer arithmetic is twos complement.
   
   syntax:
     a09 [-{b|r|s|x|f}filename]|[-c] [-lfilename] [-ooption] [-dsym=value]* sourcefile.
                  
   Options
   -c            suppresses code output
   -bfilename    binary output file name (default name minus a09 suffix plus .bin)
   -rfilename    Flex9 RELASMB-compatible output file name (relocatable)
   -sfilename    s-record output file name (default its a binary file)
   -xfilename    intel hex output file name (-"-)
   -ffilename    Flex9 output file name (-"-)
   -lfilename    list file name (default no listing)
   -dsym[=value] define a symbol
   -oopt         defines an option
   -u            Set all undefined symbols as external (for RELASMB output)
   
   recognized pseudoops:
    ext        defines an external symbol (relocating mode only)
    extern     -"-
    public     defines a public symbol (effective in relocating mode only)
    macro      defines a macro
    exitm      terminates a macro expansion
    endm       ends a macro definition
    if
    ifn
    ifc
    ifnc
    else
    endif
    dup
    endd
    org
    equ
    set
    setdp      sets direct page in 6809 / 6309 mode
    text
    fcb
    fcw
    fdb
    fcc
    rmb
    reg
    end
    include
    title
    nam
    ttl
    sttl
    setpg
    pag
    spc
    rep
    rpt
    repeat
    opt        define an option (see below for possible options)
    err
    abs        force absolute mode
    common
    endcom
    def
    define
    enddef
    name
    symlen
    bin, binary includes a binary file at the current position
    pemt       emit patch data if option LPA is set

   recognized options:
     On | Off         Meaning
    ------------------------------------------------------------
    PAG | NOP*        Page Formatting
    CON | NOC*        Print skipped conditional code
    MAC*| NOM         Print macro calling line
    EXP | NOE*        Print macro expansion lines
    SYM*| NOS         Print symbol table
    MUL*| NMU         Print multiple object code lines
    LIS*| NOL         Print assembler listing
    LP1 | NO1*        Print Pass 1 Listing
    DAT*| NOD         Print date in listing
    NUM | NON*        Print line numbers
    INV | NOI*        Print invisible lines
    TSC | NOT*        Strict TSC Compatibility
    WAR*| NOW         Print warnings
    CLL*| NCL         Check line length
    LFN | NLF*        Print long file names
    LLL*| NLL         List library lines
    GAS | NOG*        Gnu AS source style compatibility
    REL*| NOR         Print Relocation table in Relocating mode
    M68*| H63|M00|M01 MC6809 / HD6309 / MC6800 / MC6801 mode
        | M02|M03|M08 MC6802 / MC6803 / MC6808 mode
        | M09         MC6809 mode
    TXT | NTX*        Print text table
    LPA | LNP*        Listing in f9dasm/dasmfw patch format
    DLM | NDL*        Define label on macro expansion
    RED | NRD*        
    FBG*| NFB         Fill gaps in binary output files
    UEX | NUE*        Undefined labels are treated as external
    * denotes default value

    
   v0.1   1993-11-03 Initial version.
    
   v0.2   1994-03-21 Fixed PC relative addressing bug
                     Added SET, SETDP, INCLUDE. IF/ELSE/ENDIF
                     No macros yet, and no separate linkable modules.

   v0.1.X 1999-12-20 added Intel hex option (SLB)

-- H.Seib Additions: --

   v0.2.S 2001-10-13 converted to readable format
   v0.3   2001-10-15 added transfer address processing
                     increased max. #symbols and other constants
                       (hey, this isn't CP/M anymore :-)
                     added a bit of intelligence to command line processing
                     loads file(s) into internal storage prior to processing
                       (this aids a lot in preventing recursive invocations)
                     added LIB,LIBRARY pseudoop (same as INCLUDE)
                     added OPT,OPTION pseudoop
   v0.4   2001-10-22 added -t switch to increase compatibility with the
                     TSC FLEX Assembler line, which uses a slightly more
                     rigid source file format
   v0.5   2001-10-23 added SETPG, SETLI, PAG, SPC, NAM(TTL,TITLE), STTL, REP(RPT,REPEAT)
   v0.6   2001-10-23 added EXITM, IFN, IFC, IFNC, DUP, ENDD
   v0.7   2001-10-29 added REG, ERR, TEXT
   v0.8   2001-10-30 converted command line handling to -O switch for options
                     and added a bunch of options
                     RZB pseudoop added
   v0.9   2001-11-05 clean-ups, increased TSC FLEX Assembler compatibility
   v0.10  2001-11-15 ASM and VERSION texts predefined
   v0.11  2001-11-27 increased TSC FLEX Assembler compatibility (labels made case sensitive)
   v0.12  2001-11-28 added some convenience mnemonics and simulated 6800 instructions to
                     increase TSC FLEX Assembler compatibility
   v1.02  2001-04-11 check for address range overlaps implemented
   v1.03  2002-09-24 macro parser had an error; corrected
   v1.04  2002-12-10 error message output adjusted for Visual C++ 6.0 IDE compatibility
                     added OPT LFN/NLF for that purpose
   v1.05  2002-12-23 crude form of backwards ORG added to binary output
   v1.06  2003-06-10 added OPT LLL/NLL to include/suppress listing of LIB lines
   v1.07  2003-06-23 outsymtable() only starts with a page feed if current line
                     is is NOT the first line on a page
   v1.08  2003-07-01 SKIP count for the IF(N) and IF(N)C statements added to
                     normal statements (i.e., outside macro definitions)
   v1.09  2005-06-02 some cleanups
                     added -r format for FLEX9 RELASMB compatible output
                     added ABS, GLOBAL, EXT, DEFINE, ENDDEF, COMMON, ENDCOM
                     REL|NOR option added
                     GAS|NOG option added (rudimentary)
                     M68|H63 option added
   v1.10  2005-06-03 made options available as numeric strings
                     added local label support
                     added SYMLEN
                     TXT|NTX option added
   v1.11  2005-06-06 IFD/IFND directives added
                     added special checks for "0,-[-]indexreg" and "0,indexreg+[+]"
                     when determining the postbyte for indexed addressing modes
   v1.12  2005-06-20 OPT CON|NOC correctly implemented
   v1.13             skipped for users that might have an uneasy feeling about
                     the number "13" >:-)
   v1.14  2005-08-01 text symbol replacement enhanced; it was a bit TOO general.
                     Now, &xxx is only treated as a text variable when xxx REALLY
                     is a text symbol; in all other cases, it's not preprocessed.
                     Let the assembler engine handle eventual errors.
                     Thanks to Peter from Australia for pointing this out!
   v1.15  2005-08-02 only 1 local label was allowed per address; now, as many as
                     possible can be at the same location.
                     Thanks to Peter from Australia for pointing this out!
   v1.16  2005-08-05 SETDP is illegal in relocating mode!
                     SETDP without any parameter DISABLES Direct mode entirely.
                     Cured an interesting bug... the following sequence:
                                ORG 1
                                LDA TEST
                         SHIT   NOP
                                ORG 1
                         TEST   RMB 1
                     caused a phase error; this was caused by the invalid
                     assumption in scanlabel() that forward references have
                     a higher memory location than the current address.
                     Thanks to Peter from Australia for pointing this out!
   v1.17  2005-08-08 made tests for above problem more rigorous; now ANY forward
                     reference to a label that's not yet defined is treated as
                     uncertain.
                     Removed a nasty bug that caused the following code to produce
                     incorrect values:
                              org $8000  (anything >= $8000 will do)
                           lbl rmb 8
                           SizeBad equ (*-lbl)/2
                     OPT NOL changed to OPT NO1 to allow implementing OPT LIS|NOL.
                     OPT LIS*|NOL added
                     Thanks to Peter from Australia for pointing these out!
   v1.18  2005-08-09 backward search for local labels now searches for "<= local
                     address" instead of "< local address"
                     Thanks to Peter from Australia for pointing this out!
                     Added INCD / DECD convenience mnemonics
                     (realized as ADDD/SUBD 1)
   v1.19  2005-08-10 Added a bunch of 6800-style mnemonics; if they conflict
                     with 6809 mnemonics ("INC A" for example), they are only
                     active in TSC mode
   v1.20  2005-08-16 changed special checks for
                       "0,-[-]indexreg" and "0,indexreg+[+]"
                     to include all known labels and changed the scope to
                       ",[-[-]]indexreg[+[+]]"
   v1.21  2006-02-21 Bug in "xxx  >0,reg" found by Margus Kliimask
   v1.22  2008-09-03 Addded BIN(ARY) pseudo-op to include binary files
   v1.23  2009-02-13 6800 code generation added
                     accept multiple input files (first one defines default
                       output and listing file names)
   v1.24  2009-02-13 made compilable with gcc
   v1.25  2009-03-05 6800 alternate mnemonics work better now
   v1.26  2009-03-14 assembling DOS format files in Loonix works better now
   v1.27  2010-01-20 LPA/NLP options added
   v1.28  2010-04-21 INCD/DECD produced invalid code
   v1.29  2015-06-19 M01/M02/M03/M08 options added
   v1.30  2015-06-22 PHASE/DEPHASE pseudo-ops added
   v1.31  2015-07-15 6301/6303 support added
   v1.32  2015-08-19 FCQ/FQB pseudo-ops added
   v1.33  2015-08-28 Macro problems (found by Bob Grieb) fixed
   v1.34  2015-09-09 More macro problems (found by Bob Grieb) fixed
   v1.35  2015-09-11 Corrected an oversight in the special checks for
                     "0,-[-]indexreg" and "0,indexreg+[+]"
   v1.36  2015-09-25 BSZ,ZMB pseudo-ops added (AS9-compatible alternatives
                       to RZB)
                     FILL <value>,<byte_count> pseudo-op added
   v1.37  2016-06-16 DLM|NDL* options added; see
                       https://github.com/Arakula/A09/issues/1
                     for details.
   v1.38  2018-12-06 corrected 6801/6301 errors in JSR/STD processing
                     as reported by M. Hepperle - thank you!
   v1.39  2019-02-28 Improved gcc / clang compatibility,
                     based on Steve Byan's pull request here:
                     https://github.com/Arakula/A09/pull/2
   v1.40  2020-03-01 Program Counter Relative addressing didn't work correctly
                     as reported by M. Richemont - thank you!
   v1.41  2020-07-08 Improved syntax checking
   v1.42  2020-08-10 added "-dFILCHR=fillchar" constant to set up a fill
                       character other than \0 for RMB and ORG fillers in
                       binary output; in source files, this can be written as
                         "FILCHR TEXT $FF"
                       for example. See
                         https://github.com/Arakula/A09/issues/6
                       for details.
                     Regression: TSC-format local labels didn't work. Thanks to
                       Ray Bellis for pointing that out. See
                         https://github.com/Arakula/A09/issues/7
                       for details.
   v1.43  2020-08-27 Nested macros didn't work correctly; thanks to
                       Ray Bellis for pointing that out. See
                         https://github.com/Arakula/A09/issues/8
                       for details.
                     Macro expansion didn't correctly expand texts; thanks to
                       Ray Bellis for pointing that out. Also Issue 8.
   v1.44  2020-08-29 Macros can override mnemonics now; a \ before the mnemonic
                       prohibits macro invocation and uses the original mnemonic.
                     TEXT pseudo-op interpreted trailing blanks wrong.
                     TEXT and macro parameter expansion does NOT include leading
                       and trailing string delimiters in the output any more;
                       this is RELASMB-compatible and allows more constructs.
                       As an indirect consequence, "&gotcha" now REPLACES &text
                       with the contents of a text named gotcha; if you want
                       the verbatim output, you need to write it as "\&gotcha".
                     bitdirect() and the ~ operator on constants didn't work correctly;
                       thanks to Ray Bellis for pointing that out. See
                         https://github.com/Arakula/A09/issues/9
                       for details.
   v1.45 2020-12-18 Motorola 68HC11 mode added
                    RED|NRD* option added
   v1.46 2021-01-15 FBG*|NFB option added
   v1.47 2021-02-22 LPA option cleanups
                    PEMT pseudo-opt added
   v1.48 2021-02-22 additional PEMT capabilities
                    ASLD,ASRD,CLRD,DECD,INCD,LSLD,LSRD convenience mnemonics added
                    to all 6800-based assemblers
   v1.49 2021-09-16 additional PEMT capabilities
                    added WRN pseudo-op to output user-defined warnings
   v1.50 2021-09-18 ";;" or "**" in LPA mode works like an inline version
                      of PEMT [l]comment without expansions
                    ";@" or "*@" in LPA mode at the start of a line works like
                      an inline version of PEMT insert without expansions
                    an appended "+" means "after"
   v1.51 2021-10-19 PEMT improvements
   v1.52 2022-04-22 increase Motorola as9 compatibility by NOT flagging
                      16-bit relative offsets as error but silently wrapping
                      around the 64K boundary if possible
                      Thanks to Michael Oehler for pointing that out.
                    "LDA B 1,X" didn't work, not even in TSC mode. See
                        https://github.com/Arakula/A09/issues/10
                      for details.
   v1.53 2022-04-25 fix https://github.com/Arakula/A09/issues/11
   v1.54 2022-04-25 6800 alternative mnemonics are not TSC-specific any more 
   v1.55 2022-05-16 RMB,RZB et al. now get printed with offset in listing
   v1.56 2022-07-18 FCW can now produce relocation table entries
   v1.57 2022-08-18 RMB,RZB,FILL now produce relocatable output
                    -u command line switch added for RELASMB compatibility
                    UEX|NUE* option added
                    Thanks to Dieter Deyke for RELASMB compatibility checks!
   v1.58 2022-10-17 improved 68HC11 BSET/BCLR/BRSET/BRCLR parameter handling
                    (no more need to use blanks as parameter separators)
   v1.59 2022-10-19 improved handling of hex, octal, and binary constants;
                    TEXT prepends the label with '&' now to keep text labels
                    separate from normal ones. This reduces the maximum length
                    of TEXT names to 31. Still enough, I think ...
                    Further RELASMB compatibility improvements
   v1.60 2023-07-04 previous version ruined the FILCHR option; corrected. See
                         https://github.com/Arakula/A09/issues/13
                       for details.
   v1.61 2023-11-30 PSH reg / PUL reg added for the 6800/6801/HC11 assemblers; see
                         https://github.com/Arakula/A09/issues/18
                       for details.
   v1.62 2025-01-30 improved line level cleanup and flag handling
*/

#include "a09.h"

/*****************************************************************************/
/* expandfn : expands a file name to full-blown path                         */
/*****************************************************************************/

char const *expandfn(char const *fn)
{
    #ifdef WIN32
    static char szBuf[_MAX_PATH];           /* allocate big buffer               */

    if (dwOptions & OPTION_LFN)             /* if long file names wanted         */
      return _fullpath(szBuf, fn, sizeof(szBuf));
    #endif

    return fn;                              /* can't do that yet                 */
}

/*****************************************************************************/
/* PageFeed : advances the list file                                         */
/*****************************************************************************/

void PageFeed()
{
    time_t tim;
    struct tm *ltm;

    time(&tim);
    ltm = localtime(&tim);

    fputc('\x0c', listfile);                /* print header                      */
    nCurPage++;                             /* advance to next page              */
    fprintf(listfile, "\n\n%-32.32s ", szTitle);
    if (dwOptions & OPTION_DAT)
      fprintf(listfile,
              "%04d-%02d-%02d ",
              ltm->tm_year + 1900,
              ltm->tm_mon + 1,
              ltm->tm_mday);
    fprintf(listfile,
            "A09 %d Assembler V" VERSION " Page %d\n",
            (dwOptions & OPTION_H09) ? 6309 :
                (dwOptions & OPTION_M00) ? 6800 :
                (dwOptions & OPTION_M01) ? 6801 :
                (dwOptions & OPTION_H01) ? 6301 :
                6809,
            nCurPage);
    fprintf(listfile, "%-.79s\n\n", szSubtitle);

    nCurLine = 5;                           /* remember current line             */
    nCurCol = 0;                            /* and reset current column          */
}

/*****************************************************************************/
/* putlist : puts something to the list file                                 */
/*****************************************************************************/

void putlist(const char *szFmt, ...)
{
    char szList[1024];                      /* buffer for 1k list output         */
    char *p;

    va_list al;
    va_start(al, szFmt);
    vsprintf(szList, szFmt, al);            /* generate formatted output buffer  */
    va_end(al);

    for (p = szList; *p; p++)               /* then walk through the buffer      */
    {
      fputc(*p, listfile);                  /* print out each character          */
      if (*p == '\n')                       /* if newline sent                   */
      {
        nCurLine++;
        nCurCol = 0;
        if ((nCurLine >= nLinesPerPage) &&  /* if beyond # lines per page        */
            (dwOptions & OPTION_PAG))       /* if pagination activated           */
          PageFeed();                       /* do a page feed                    */
      }
      else                                  /* if another character              */
      {
        nCurCol++;                          /* advance to next column            */
        if (dwOptions & OPTION_CLL)         /* if line length checked            */
        {
                                            /* check if word would go too far    */
          if ((nCurCol >= nColsPerLine * 3 / 4) && (*p == ' '))
          {
            int i;                          /* look whether more delimiters      */
            char c;
            for (i = nCurCol + 1; i < nColsPerLine; i++)
              {
              c = p[i - nCurCol];
              if ((c == '\t') || (c == ' ') || (!c))
                break;
              }
            if (i >= nColsPerLine)          /* if no more delimiters,            */
              nCurCol = nColsPerLine;       /* make sure to advance to new line  */
          }

          if (nCurCol >= nColsPerLine)      /* if it IS too far                  */
          {
            putlist("\n");                  /* recurse with a newline            */
            nCurCol = 0;
          }
        }
      }
    }
}

/*****************************************************************************/
/* findop : finds a mnemonic in table using binary search                    */
/*****************************************************************************/

struct oprecord * findop(char * nm)
{
    int lo,hi,i,s;

    lo = 0;
    hi = optablesize - 1;
    do
      {
      i = (lo + hi) / 2;
      s = strcmp(optable[i].name, nm);
      if (s < 0)
        lo = i + 1;
      else if (s > 0)
        hi = i - 1;
      else
        break;
      } while (hi >= lo);
    if (s)
      return NULL;
    return optable + i;
}  

/*****************************************************************************/
/* findlocal : finds a local symbol table record                             */
/*****************************************************************************/

struct symrecord * findlocal(struct symrecord *sym, char forward, int insert)
{
    static struct symrecord empty = {"", SYMCAT_EMPTYLOCAL, 0, {0}};
    int lo,hi,i,j,s;

    if ((!sym) ||                           /* if no main symbol for that        */
        ((!insert) &&                       /* or not inserting, but             */
         (sym->cat == SYMCAT_EMPTYLOCAL)))  /*    yet undefined label            */
      return sym;                           /* pass back main symbol             */

    lo = 0;                                 /* do binary search for the thing    */
    hi = lcltable.counter - 1;
    s = 1;
    i = 0;
    while (hi >= lo)
    {
      i = (lo + hi) / 2;                    /* binary search for current address */
      s = lcltable.rec[i].value - loccounter;
      if (s < 0)
        lo = i + 1;
      else if (s > 0)
        hi = i - 1;
      else                                  /* if found,                         */
        {                                   /* go to 1st of this value           */
        while ((i) && (lcltable.rec[i - 1].value == loccounter))
          i--;
        while ((i < lcltable.counter) &&    /* search for the NAME now           */
               (lcltable.rec[i].value == loccounter))
          {
          s = strcmp(sym->name, lcltable.rec[i].name);
          if (s <= 0)
            {
            if (s)
              i--;
            break;
            }
          i++;
          }
        if (s)                              /* if not found, re-set              */
          {
          if (i >= lcltable.counter)
            s = 1;
          else
            s = lcltable.rec[i].value - loccounter;
          }
        break;
        }
    }

    if (insert)                             /* if inserting,                     */
    {
      if (!s)                               /* if address is already in use      */
        {                                   /* but not the correct label         */
        if (strcmp(sym->name, lcltable.rec[i].name))
          error |= ERR_LABEL_MULT;          /* set error                         */
        return lcltable.rec + i;            /* return the local symbol           */
        }
      i = (s < 0 ? i + 1 : i);
      if (lcltable.counter == MAXLABELS)
        {
        printf("%s(%ld): error 25: out of local symbol storage\n",
               expandfn(curline->fn), curline->ln);
        if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
          putlist( "*** Error 25: out of local symbol storage\n");
        exit(4);
        }
      sym->cat = SYMCAT_LOCALLABEL;
      for (j = lcltable.counter; j > i; j--)
        lcltable.rec[j] = lcltable.rec[j - 1];
      lcltable.counter++;
      strcpy(lcltable.rec[i].name, sym->name);
      lcltable.rec[i].cat = SYMCAT_LOCALLABEL;
      lcltable.rec[i].value = loccounter;
      lcltable.rec[i].u.parent = NULL;
      return lcltable.rec + i;              /* pass back this symbol             */
    }

    if (forward)                            /* if forward search                 */
    {
      i = (s < 0 ? i - 1 : i);
      for (; i < lcltable.counter; i++)
      {
        if ((!strcmp(lcltable.rec[i].name, sym->name)) &&
            (lcltable.rec[i].value > loccounter))
          return lcltable.rec + i;
      }
    }
    else                                    /* if backward search                */
    {
      i = (s > 0 ? i + 1 : i);
      for (; i >= 0; i--)
        {
        if ((!strcmp(lcltable.rec[i].name, sym->name)) &&
            (lcltable.rec[i].value <= loccounter))
          return lcltable.rec + i;
        }
    }

    return &empty;                          /* if not found, return empty label  */
}

/*****************************************************************************/
/* findsym : finds symbol table record; inserts if not found                 */
/*           uses binary search, maintains sorted table                      */
/*****************************************************************************/

struct symrecord * findsym (char * nm, int insert)
{
    int lo,hi,i,j,s;
    char islocal = 0, forward = 0;
    char name[MAXIDLEN + 1] = "";
                                            /* copy name to internal buffer      */
    strncpy(name, (nm) ? nm : "", sizeof(name) - 1);
    for (i = 0; name[i]; i++)               /* determine whether local label     */
      if ((name[i] < '0') || (name[i] > '9'))
        break;
    if (i)                                  /* if starting with number           */
    {
      if (!name[i])                         /* if ONLY numeric                   */
        islocal = 1;                        /* this is a local label             */
      else                                  /* otherwise check direction         */
        {
        switch(toupper(name[i]))
          {
          case 'B' :                        /* backward reference ?              */
            islocal = 2;                    /* this is a local label reference   */
            forward = 0;
            break;
          case 'F' :                        /* forward reference ?               */
            islocal = 2;                    /* this is a local label reference   */
            forward = 1;
            break;
          }
        if (islocal && name[i + 1])         /* if followed by anything else      */
          islocal = 0;                      /* reset flag for local label        */
        else                                /* otherwise                         */
          name[i] = 0;                      /* remove the direction              */
        }
    }

    lo = 0;                                 /* do binary search for the thing    */
    hi = symtable.counter - 1;
    s = 1;
    i = 0;
    while (hi >= lo)
    {
      i = (lo + hi) / 2;
      s = strcmp(symtable.rec[i].name, name);
      if (s < 0)
        lo = i + 1;
      else if (s > 0)
        hi = i - 1;
      else
        break;
    }

    if (s)                                  /* if symbol not found               */
    {
      if (!insert)                          /* if inserting prohibited,          */
        return NULL;                        /* return without pointer            */

      i = (s < 0 ? i + 1 : i);
      if (symtable.counter == MAXLABELS)
        {
        printf("%s(%ld): error 23: out of symbol storage\n",
               expandfn(curline->fn), curline->ln);
        if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
          putlist( "*** Error 23: out of symbol storage\n");
        exit(4);
        }
      if (commonsym >= symtable.rec + i)
        commonsym++;
      for (j = 0; j < symtable.counter; j++)
        if (symtable.rec[j].u.parent >= symtable.rec + i)
          symtable.rec[j].u.parent++;
      for (j = symtable.counter; j > i; j--)
        symtable.rec[j] = symtable.rec[j-1];
      symtable.counter++;
      strcpy(symtable.rec[i].name, name);
      symtable.rec[i].cat = (islocal) ? SYMCAT_EMPTYLOCAL : SYMCAT_EMPTY;
      symtable.rec[i].value = 0;
      symtable.rec[i].u.flags = 0;
    }

    if (islocal)                            /* if searching for a local label    */
      return findlocal(symtable.rec + i,    /* search for the local label        */
                       forward, (islocal < 2));

    return symtable.rec + i;                /* return the found or inserted sym  */
}  

/*****************************************************************************/
/* findsymat : finds 1st symbol for a given address                          */
/*****************************************************************************/

char *findsymat(unsigned short addr)
{
    /* since the symbol table is sorted by name, this needs a sequential search  */
    int i;
    for (i = 0; i < symtable.counter; i++) 
      if (symtable.rec[i].cat != SYMCAT_EMPTY)
        {
        if (symtable.rec[i].cat == SYMCAT_TEXT)
          continue;
        if (symtable.rec[i].value == addr)
          return symtable.rec[i].name;
        }
    return NULL;
}

/*****************************************************************************/
/* isValidNameChar : returns whether char is a valid name char               */
/*****************************************************************************/

char isValidNameChar(char c, char cFirst)
{
    if (cFirst)                             /* first char of a name is special   */
    {
      if ((c >= 'A' && c <= 'Z') ||         /* A..Z is always valid              */
          (c >= 'a' && c <= 'z') ||
          (c >= '0' && c <= '9'))           /* local labels start numeric        */
        return 1;
      if ((cFirst == 1) &&                  /* if explicitly checking for labels */
          (dwOptions & OPTION_TSC))         /* only letters and numbers in TSC   */
        return 0;                           /* in 1st position                   */
      if (c == '.' || c == '_')             /* others allow . and _ as start char*/
        return 1;
      if ((dwOptions & OPTION_GAS) &&       /* Gnu as allows $, too              */
          (c == '$'))
        return 1;
      if (cFirst == 1)                      /* if explicitly checking for labels */
        return 0;                           /* anything else is invalid          */
    }

    if ((c >= '0' && c <= '9') ||           /* normally, labels may consist of   */
        (c >= 'A' && c <= 'Z') ||           /* the characters 0..9,A..Z,a..z,_   */
        (c >= 'a' && c <= 'z') ||
        (c == '_'))
      return 1;
    if ((dwOptions & OPTION_GAS) &&         /* Gnu as also allows $              */
        (c == '$'))
      return 1;
    if (!(dwOptions & OPTION_TSC) &&        /* non-TSC and non-GNU allows more:  */
        (c == '.' ||                        /* . $ _ are allowed in names, too   */
         c == '_' ||
         c == '$'))
      return 1;

    return 0;
}

/*****************************************************************************/
/* parsename : parses a name from a text string                              */
/*****************************************************************************/

void parsename(char *srcptr, char **nextptr)
{
    int i = 0;
    char c, cValid, cFirst = 2;

    while (1)
    {
      c = *srcptr++;
      if ((!(dwOptions & OPTION_TSC)) &&    /* TSC Assembler is case-sensitive   */
          (!(dwOptions & OPTION_GAS)) &&    /* GNU Assembler is case-sensitive   */
          (c >= 'a' && c <= 'z'))
        c -= ('a' - 'A');
      cValid = isValidNameChar(c, cFirst);  /* check for validity                */
      if (!cValid)                          /* if invalid character encountered  */
        break;                              /* stop here                         */

      if (i < maxidlen)
      {
        namebuf[i] = c;
        unamebuf[i] = toupper(c);
        i++;
      }
      cFirst = 0;
    }
    namebuf[i] = '\0';
    unamebuf[i] = '\0';
    if (nextptr)
      *nextptr = --srcptr;
}

/*****************************************************************************/
/* parsedecimal : parses a decimal number                                    */
/*****************************************************************************/

long parsedecimal(char *srcptr, char **nextptr)
{
    char c;
    long t = 0, mult = 1;
    int i = 0;

    c = *srcptr++;
    #if 0
    /* since decimals don't have a leading indicator, '-' should already have been
       processed by scanfactor() / scanexpr() */
    while (c == '-')
    {
      mult = -mult;
      c = *srcptr++;
    }
    #endif
    while (isdigit(c))
    {
      i++;
      t = t * 10 + c - '0';
      c = *srcptr++;
    }
    t *= mult;
    if (i == 0)
      error |= ERR_EXPR;
    if (nextptr)
      *nextptr = --srcptr;
    return t;
} 

/*****************************************************************************/
/* parsehex : parses a hex number                                            */
/*****************************************************************************/

long parsehex(char *srcptr, char **nextptr)
{
    char c;
    int i = 0;
    long t = 0, mult = 1;

    srcptr++;                               /* skip $                            */
    while (*srcptr == '-')
    {
      mult = -mult;
      srcptr++;
    }
    c = toupper(*srcptr++);
    while ((c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F'))
    {
      i++;
      if (c >= '0' && c <= '9')
        c -= '0';
      else
        c -= ('A' - 10);
      t = t * 16 + c;
      c = toupper(*srcptr++);
    }  
    t *= mult;
    if (i == 0)
      error |= ERR_EXPR;
    if (nextptr)
      *nextptr = --srcptr;
    return t;
}

/*****************************************************************************/
/* parsechar : parses a character                                            */
/*****************************************************************************/

long parsechar(char *srcptr, char **nextptr)
{
    long t = *++srcptr;                     /* get char skipping '               */
    if (nextptr && t)
    {
      srcptr++;
      if (*srcptr == '\'')
        srcptr++;
      *nextptr = srcptr;
    }
    return t;
}

/*****************************************************************************/
/* parsebin : parses a binary value                                          */
/*****************************************************************************/

long parsebin(char *srcptr, char **nextptr)
{
    char c;
    int i = 0;
    long t = 0, mult = 1;

    srcptr++;                               /* skip %                            */
    while (*srcptr == '-')
    {
      mult = -mult;
      srcptr++;
    }
    c = *srcptr++;
    while (c == '0' || c == '1')
    {
      i++;
      t = t * 2 + c - '0';
      c = *srcptr++;
    }
    t *= mult;
    if (i == 0)
      error |= ERR_EXPR;
    if (nextptr)
      *nextptr = --srcptr;
    return t;
}

/*****************************************************************************/
/* parseoct : parses an octal value                                          */
/*****************************************************************************/

long parseoct(char *srcptr, char **nextptr)
{
    char c;
    int i = 0;
    long t = 0, mult = 1;

    srcptr++;                               /* skip @                            */
    while (*srcptr == '-')
    {
      mult = -mult;
      srcptr++;
    }
    c = *srcptr++;
    while (c >= '0' && c <= '7')
    {
      i++;
      t = t * 8 + c - '0';
      c = *srcptr++;
    }
    t *= mult;
    if (i == 0)
      error |= ERR_EXPR;
    if (nextptr)
      *nextptr = --srcptr;
    return t;
}

/*****************************************************************************/
/* parsestring : parses a string into a buffer                               */
/*****************************************************************************/

char * parsestring(char *srcptr, char *dest, int nlen, char **nextptr)
{
    char *s = srcptr;
    char *d = dest;
    int nInString = 0;
    char c;

    if (*srcptr == '\'' || *srcptr == '\"')
    {
      nInString = 1;
      srcptr++;
    }

    while (*srcptr)
    {
      if (!nInString &&
          (*srcptr == ' ' || *srcptr == ','))
        break;
      else if (nInString && *s == *srcptr)
      {
        srcptr++;
        break;
      }
      c = *srcptr++;
      if (!nInString && c >= 'a' && c <= 'z')
        c -= 32;
      *d++ = c;
    }
    *d = '\0';

    if (nextptr)
      *nextptr = srcptr;
    return dest;
}

/*****************************************************************************/
/* parsespace : parses whitespace characters                                 */
/*****************************************************************************/

char parsespace(char *srcptr, char **nextptr)
{
    char c, lblk = 0;
    while ((c = *srcptr) == ' ' || c == '\t')
    {
      if (lblk < 127)
        lblk++;
      srcptr++;
    }
    *nextptr = srcptr;
    return lblk; /* return whether (and approx. how many) leading blanks skipped */
}

/*****************************************************************************/
/* settext : sets a text symbol                                              */
/*****************************************************************************/

int settext(char *namebuf, char *text)
{
    struct symrecord *lp = findsym(namebuf, 1);
    char iname[sizeof(lp->name)];

    if (!namebuf)
    {
      error |= ERR_LABEL_UNDEF;
      return -1;
    }
    /* to keep texts separate from the normal labels' way, prepend them with &   */
    if (*namebuf != '&')
    {
      sprintf(iname, "&%.*s", (int)(sizeof(iname) - 2), namebuf);
      namebuf = iname;
    }

    lp = findsym(namebuf, 1);
    if (!lp)
    {
      error |= ERR_LABEL_UNDEF;
      return -1;
    }
    if (lp->cat != SYMCAT_EMPTY &&
        lp->cat != SYMCAT_TEXT)
    {
      error |= ERR_LABEL_MULT;
      return -1;
    }

    if (lp->cat != SYMCAT_EMPTY)
      free(texts[lp->value]);
    else if (nTexts >= MAXTEXTS)
      {
      error |= ERR_MALLOC;
      return -1;
      }
    else
      {
      lp->cat = SYMCAT_TEXT;
      lp->value = nTexts++;
      }

    if (!text)                              /* if NULL input                     */
      text = "";                            /* assume empty string               */

    texts[lp->value] = (char *)malloc(strlen(text) + 1);
    if (texts[lp->value])
      strcpy(texts[lp->value], text);

    if (!strcmp(namebuf, "&FILCHR"))        /* special for FILCHR : set internal */
    {                                     /* byte                              */
      switch (text[0])
      {
        case '$' :
          rmbfillchr = (char)parsehex(text, NULL);
          break;
        case '%' :
          rmbfillchr = (char)parsebin(text, NULL);
          break;
        case '@' :
          rmbfillchr = (char)parseoct(text, NULL);
          break;
        case '\'' :
          rmbfillchr = (char)parsechar(text, NULL);
          break;
        default :
          error |= ERR_EXPR;
          break;
      }
    }

    return lp->value;
}

/*****************************************************************************/
/* outsymtable : prints the symbol table                                     */
/*****************************************************************************/

void outsymtable()
{
    int i,j = 0;

    if (dwOptions & OPTION_PAG)             /* if pagination active,             */
    {
      if (nCurLine > 5)                     /* if not on 1st line,               */
        PageFeed();                         /* shift to next page                */
    }
    else
      putlist("\n");

    putlist("%sSYMBOL TABLE", (dwOptions & OPTION_LPA) ? "* " : "");
    for (i = 0; i < symtable.counter; i++) 
    {
      if (symtable.rec[i].cat != SYMCAT_EMPTY)
      {
                                            /* suppress listing of predef texts  */
        if ((symtable.rec[i].cat == SYMCAT_TEXT)
    #if 1
    /* not really necessary. OPT TXT can be used to output these. */
            && (symtable.rec[i].value < nPredefinedTexts)
    #endif
            )
          continue;
                                            /* if local label                    */
        if (symtable.rec[i].cat == SYMCAT_LOCALLABEL)
        {
          int k;                            /* walk local label list             */
          for (k = 0; k < lcltable.counter; k++)
            if (!strcmp(lcltable.rec[k].name, symtable.rec[i].name))
            {
              if (j % 4 == 0)
                putlist("\n%s", (dwOptions & OPTION_LPA) ? "* " : "");
              putlist( " %9s %02d %04X", lcltable.rec[k].name,
                                         lcltable.rec[k].cat,
                                         lcltable.rec[k].value); 
              j++;
            }
        }
        else                                /* if normal label                   */
        {
          if (j % 4 == 0)
            putlist("\n%s", (dwOptions & OPTION_LPA) ? "* " : "");
          putlist( " %9s %02d %04X", symtable.rec[i].name,
                                     symtable.rec[i].cat,
                                     symtable.rec[i].value); 
          j++;
        }
      }
    }
    putlist("\n%s%d SYMBOLS\n", (dwOptions & OPTION_LPA) ? "* " : "", j);
} 

/*****************************************************************************/
/* outreltable : prints the relocation table                                 */
/*****************************************************************************/

void outreltable()
{
    int i,j = 0;

    if (dwOptions & OPTION_PAG)             /* if pagination active,             */
    {
      if (nCurLine > 5)                     /* if not on 1st line,               */
        PageFeed();                         /* shift to next page                */
    }
    else
      putlist("\n");

    putlist("%sRELOCATION TABLE", (dwOptions & OPTION_LPA) ? "* " : "");
    for (i = 0; i < relcounter; i++) 
    {
      char name[10];
      sprintf(name, "%c%-.8s",
              (reltable[i].exprcat & EXPRCAT_NEGATIVE) ? '-' :
                  (reltable[i].exprcat & EXPRCAT_ONEBYTE) ? '\'' :
                  ' ',
              reltable[i].sym->name);
      if (j % 4 == 0)
        putlist("\n%s", (dwOptions & OPTION_LPA) ? "* " : "");
      putlist( " %9s %02d %04X",
               name,
               reltable[i].sym->cat,
               reltable[i].addr);
      j++;
    }
    putlist("\n%s%d RELOCATIONS\n", (dwOptions & OPTION_LPA) ? "* " : "", j);
} 

/*****************************************************************************/
/* outtexttable : prints the text table                                      */
/*****************************************************************************/

void outtexttable()
{
    int i,j = 0;

    if (dwOptions & OPTION_PAG)             /* if pagination active,             */
    {
      if (nCurLine > 5)                     /* if not on 1st line,               */
        PageFeed();                         /* shift to next page                */
    }
    else
      putlist("\n");

    putlist("%sTEXT TABLE", (dwOptions & OPTION_LPA) ? "* " : "");
    for (i = 0; i < symtable.counter; i++) 
    {
      if (symtable.rec[i].cat == SYMCAT_TEXT)
      {
                                            /* suppress listing of predef texts  */
        if (symtable.rec[i].value < nPredefinedTexts)
          continue;
        putlist("\n%s %9s %s", (dwOptions & OPTION_LPA) ? "* " : "",
                symtable.rec[i].name, texts[symtable.rec[i].value]); 
        j++;
      }
    }
    putlist("\n%s%d TEXTS\n", (dwOptions & OPTION_LPA) ? "* " : "", j);
} 

/*****************************************************************************/
/* findreg : finds a register per name                                       */
/*****************************************************************************/

struct regrecord * findreg(char *nm)
{
    int i;
    for (i = 0; regtable[i].name != NULL; i++)
    {
      if (strcmp(regtable[i].name,nm) == 0)
        return regtable + i;
    }
    return 0;                   
}

/*****************************************************************************/
/* findreg63 : finds a register per name for HD63 operations                 */
/*****************************************************************************/

struct regrecord * findreg63(char *nm)
{
    int i;
    for (i = 0; i < (sizeof(regtable63) / sizeof(regtable63[0])); i++)
    {
      if (strcmp(regtable63[i].name,nm) == 0)
        return regtable63 + i;
    }
    return 0;                   
}

/*****************************************************************************/
/* findbitreg : finds a bit transfer register per name (6309 only)           */
/*****************************************************************************/

struct regrecord * findbitreg(char *nm)
{
    int i;

    for (i = 0; i < bitregtablesize; i++)
    {
      if (strcmp(bitregtable[i].name,nm) == 0)
        return bitregtable + i;
    }
    return 0;                   
}

/*****************************************************************************/
/* strupr : converts a string to uppercase (crude)                           */
/*****************************************************************************/

char *strupr(char *name)
{
    int i;
    if (!name)
      return name;
    for (i = 0; name[i]; i++)
      if ((name[i] >= 'a') && (name[i] <= 'z'))
        name[i] -= ('a' - 'A');
    return name;
}

/*****************************************************************************/
/* addreloc : adds a relocation record to the list                           */
/*****************************************************************************/

void addreloc(struct relocrecord *p)
{
    struct relocrecord rel = {0};           /* internal copy                     */

    if (p)                                  /* if there's a record,              */
      rel = *p;                             /* copy to internal                  */

    if ((!relocatable) ||                   /* if generating unrelocatable binary*/
        (!rel.sym) ||                       /* or no symbol                      */
        (pass == 1))                        /* or in pass 1                      */
      return;                               /* do nothing here                   */

    switch (rel.sym->cat)                   /* check symbol category             */
    {
      case SYMCAT_PUBLIC :                  /* public label ?                    */
        if (rel.sym->u.flags & SYMFLAG_ABSOLUTE)
          return;
        /* fall thru on purpose for now ... */
      case SYMCAT_LABEL :                   /* normal label ?                    */
      case SYMCAT_EXTERN :                  /* external symbol ?                 */
        break;                              /* these are allowed                 */
      case SYMCAT_COMMONDATA :              /* Common data ?                     */
        rel.sym = rel.sym->u.parent;        /* switch to COMMON block            */
        break;                              /* and allow it.                     */
      default :                             /* anything else                     */
        return;                             /* isn't                             */
    }

    if (relcounter >= MAXRELOCS)            /* if no more space                  */
    {                                     /* should NEVER happen... but then...*/
      error |= ERR_MALLOC;                  /* set mem alloc err                 */
      return;                               /* and get out of here               */
    }

    reltable[relcounter++] = rel;           /* add relocation record             */

    switch (p->sym->cat)                    /* do specials...                    */
    {
      case SYMCAT_PUBLIC :                  /* public label ?                    */
      case SYMCAT_LABEL :                   /* normal label ?                    */
      case SYMCAT_COMMONDATA :              /* common data ?                     */
        curline->rel =                      /* remember there's a relocation     */
          (p->exprcat & EXPRCAT_NEGATIVE) ? '-' : '+';
        break;
      case SYMCAT_EXTERN :                  /* external symbol ?                 */
        curline->rel =
          (p->exprcat & EXPRCAT_NEGATIVE) ? 'x' : 'X';
        break;
    }
}

/*****************************************************************************/
/* scanname : scans a name from the input buffer                             */
/*****************************************************************************/

void scanname()
{
    parsename(srcptr, &srcptr);             /* parse with global pointer         */
}

/*****************************************************************************/
/* skipspace : skips whitespace characters                                   */
/*****************************************************************************/

char skipspace()
{
    return parsespace(srcptr, &srcptr);
} 

/*****************************************************************************/
/* scandecimal : scans a decimal number                                      */
/*****************************************************************************/

long scandecimal()
{
    return parsedecimal(srcptr, &srcptr);   /* parse using global pointer        */
} 

/*****************************************************************************/
/* scanhex : scans hex number                                                */
/*****************************************************************************/

long scanhex()
{
    return parsehex(srcptr, &srcptr);       /* parse using global pointer        */
}

/*****************************************************************************/
/* scanchar : scan a character                                               */
/*****************************************************************************/

long scanchar()
{
    return parsechar(srcptr, &srcptr);      /* parse using global pointer        */
}

/*****************************************************************************/
/* scanbin : scans a binary value                                            */
/*****************************************************************************/

long scanbin()
{
return parsebin(srcptr, &srcptr);       /* parse using global pointer        */
}

/*****************************************************************************/
/* scanoct : scans an octal value                                            */
/*****************************************************************************/

long scanoct()
{
    return parseoct(srcptr, &srcptr);       /* parse using global pointer        */
}

/*****************************************************************************/
/* scanstring : scans a string into a buffer                                 */
/*****************************************************************************/

char * scanstring(char *dest, int nlen)
{                                       /* parse using global pointer        */
    return parsestring(srcptr, dest, nlen, &srcptr);
}


/*****************************************************************************/
/* scanlabel : scans a label                                                 */
/*****************************************************************************/

unsigned short scanlabel(struct relocrecord *pp)
{
    struct symrecord * p;

    scanname();
    p = findsym(namebuf, 1);
    if (p->cat == SYMCAT_EMPTY)
    {
      p->cat = SYMCAT_UNRESOLVED;
      p->value = 0;
      p->u.flags |= SYMFLAG_FORWARD;
    }

    if (p->cat == SYMCAT_MACRO ||
        p->cat == SYMCAT_PARMNAME ||
        p->cat == SYMCAT_TEXT)
      error |= ERR_EXPR;
    exprcat = p->cat & (EXPRCAT_PUBLIC | EXPRCAT_EXTADDR | EXPRCAT_INTADDR);
    if (exprcat == (EXPRCAT_EXTADDR | EXPRCAT_INTADDR) ||
        exprcat == (EXPRCAT_PUBLIC | EXPRCAT_INTADDR))
      unknown = 1;

    #if 1
    /* anything that's not yet defined is uncertain in pass 2! */
    if ((p->u.flags & (SYMFLAG_FORWARD | SYMFLAG_PASSED)) == SYMFLAG_FORWARD)
      certain = 0;
    #else
    if (((exprcat == EXPRCAT_INTADDR ||
          exprcat == EXPRCAT_PUBLIC) && 
    //    (unsigned short)(p->value) > (unsigned short)loccounter) ||
    //    (p->u.flags & SYMFLAG_FORWARD)) ||
        ((p->u.flags & (SYMFLAG_FORWARD | SYMFLAG_PASSED)) == SYMFLAG_FORWARD)) ||
        exprcat == EXPRCAT_EXTADDR)
      certain = 0;
    #endif
    if ((!absmode) &&                       /* if in relocating mode             */
        (exprcat))                          /* and this is not a constant        */
      certain = 0;                          /* this is NOT certain               */
    if (exprcat == EXPRCAT_PUBLIC ||
        exprcat == (EXPRCAT_EXTADDR | EXPRCAT_INTADDR) ||
        exprcat == (EXPRCAT_PUBLIC | EXPRCAT_INTADDR))
      exprcat = EXPRCAT_INTADDR;
    if (pp)
    {
      pp->exprcat = exprcat;
      pp->sym = p;
    }
    return p->value;
}
 
/*****************************************************************************/
/* isfactorstart : returns whether passed character possibly starts a factor */
/*****************************************************************************/

int isfactorstart(char c)
{
    if (isalpha(c))
      return 1;
    else if (isdigit(c))
      return 1;
    else switch (c)
    {
      case '*' :
      case '$' :
      case '%' :
      case '@' :
      case '\'' :
      case '(' :
      case '-' :
      case '+' :
      case '!' :
      case '~' :
        return 1;
    }
    return 0;
}

long scanexpr(int, struct relocrecord *);

/*****************************************************************************/
/* scanfactor : scans an expression factor                                   */
/*****************************************************************************/

long scanfactor(struct relocrecord *p)
{
    char c;
    long t;

    if (!(dwOptions & OPTION_TSC))
      skipspace();
    c = *srcptr;
    if (isValidNameChar(c, 1) &&
        !isdigit(c))                        /* digit is either constant or loclbl*/
      return (unsigned short)scanlabel(p);
    else if (isdigit(c))
    {
      char *locptr = srcptr;                /* watch out for local labels        */
      char caft;
      while ((*locptr >= '0') &&            /* advance to next nonnumeric        */
             (*locptr <= '9'))
        locptr++;
      caft = toupper(*locptr);              /* get next character in uppercase   */
      if ((caft == 'B') || (caft == 'F'))   /* if this might be a local reference*/
        return (short)scanlabel(p);         /* look it up.                       */
      switch (c)
      {
        case '0' :                          /* GNU AS bin/oct/hex?               */
          if (dwOptions & OPTION_GAS)       /* if GNU AS extensions,             */
          {
            if (srcptr[1] == 'b')           /* if binary value,                  */
            {
              srcptr++;                     /* advance behind 0                  */
              return scanbin();             /* and treat rest as binary value    */
            }
            else if (srcptr[1] == 'x')      /* if hex value,                     */
            {
              srcptr++;                     /* advance behind 0                  */
              return scanhex();             /* and treat rest as hex value       */
            }
            return scanoct();               /* otherwise treat as octal          */
          }
          /* else fall thru on purpose */
        default :                           /* decimal in any case ?             */
          return scandecimal();
      }
    }
    else switch (c)
    {
      case '*' :
        srcptr++;
        exprcat |= EXPRCAT_INTADDR;
        return loccounter + phase;
      case '$' :
        return scanhex();
      case '%' :
        return scanbin();
      case '@' :
        return scanoct();
      case '\'' :
        return scanchar();
      case '(' :
        srcptr++;
        t = scanexpr(0, p);
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (*srcptr == ')')
          srcptr++;
        else
          error |= ERR_EXPR;
        return t; 
      case '-' :
        srcptr++;
        t = scanfactor(p);
        exprcat ^= EXPRCAT_NEGATIVE;
        if (p)
          p->exprcat ^= EXPRCAT_NEGATIVE;
        return -t;
      case '+' :
        srcptr++;
        return scanfactor(p);
      case '!' :
        srcptr++;
        exprcat |= EXPRCAT_FIXED;
        return !scanfactor(p);
      case '~' :
        srcptr++;
        exprcat |= EXPRCAT_FIXED;
        return ~scanfactor(p);           
    }
    error |= ERR_EXPR;
    return 0;
}

/*****************************************************************************/
/* some nice macros                                                          */
/*****************************************************************************/

/*define EXITEVAL { srcptr--; return t; } */
#define EXITEVAL { srcptr--; parsing = 0; break; }

#define RESOLVECAT if((oldcat & 15) == 0) oldcat = 0;               \
           if ((exprcat & 15) == 0)exprcat = 0;                     \
           if ((exprcat == EXPRCAT_INTADDR &&                       \
                oldcat == (EXPRCAT_INTADDR | EXPRCAT_NEGATIVE)) ||  \
               (exprcat == (EXPRCAT_INTADDR | EXPRCAT_NEGATIVE)&&   \
               oldcat == EXPRCAT_INTADDR))                          \
           {                                                        \
             exprcat = 0;                                           \
             oldcat = 0;                                            \
           }                                                        \
           exprcat |= oldcat;
/* resolve such cases as constant added to address or difference between
   two addresses in same module */           

/*****************************************************************************/
/* scanexpr : scan expression                                                */
/*****************************************************************************/

long scanexpr(int level, struct relocrecord *pp) /* This is what you call _recursive_ descent!!!*/
{
    long t, u;
    char oldcat,c,parsing=1;
    struct relocrecord ip = {0}, p = {0};

    exprcat = 0;
    if (level == 10)
      return scanfactor(pp);
    t = scanexpr(level + 1, &ip);
    /* ip.exprcat = exprcat; */
    while (parsing)
    {
      p.sym = NULL;
      if (!(dwOptions & OPTION_TSC))
        skipspace();
      c = *srcptr++; 
      switch(c)
      {
        case '*':
          oldcat = exprcat;
          t *= scanexpr(10, &p);
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;
        case '/':
          oldcat = exprcat;
          u = scanexpr(10, &p);
          if (u)
            t /= u;
          else
            error |= ERR_EXPR;
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;
        case '%':
          oldcat = exprcat;
          u = scanexpr(10, &p);
          if (u)
            t %= u;
          else
            error |= ERR_EXPR;
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;
        case '+':
          if (level == 9)
            EXITEVAL
          oldcat = exprcat;
          t += scanexpr(9, &p);
          RESOLVECAT
          break;
        case '-':
          if (level == 9)
            EXITEVAL              
          oldcat = exprcat;
          t -= scanexpr(9, &p);
          exprcat ^= EXPRCAT_NEGATIVE;
          RESOLVECAT
          break;
        case '<':
          if (*srcptr == '<')
            {
            if (level >= 8)
              EXITEVAL
            srcptr++;
            oldcat = exprcat;
            t <<= scanexpr(8, &p);
            exprcat |= oldcat | EXPRCAT_FIXED;
            }
          else if (*srcptr == '=')
            {
            if (level >= 7)
              EXITEVAL
            srcptr++;
            oldcat = exprcat;
            t = t <= scanexpr(7, &p);
            exprcat |= oldcat | EXPRCAT_FIXED;
            }
          else
            {
            if (level >= 7)
              EXITEVAL
            oldcat = exprcat;
            t = t < scanexpr(7, &p);
            exprcat |= oldcat | EXPRCAT_FIXED;
            }
          break;
        case '>':
          if (*srcptr == '>')
            {
            if (level >= 8)
              EXITEVAL
            srcptr++;
            oldcat = exprcat;
            t >>= scanexpr(8, &p);
            exprcat |= oldcat | EXPRCAT_FIXED;
            }
          else if (*srcptr == '=')
            {
            if (level>=7)
              EXITEVAL
            srcptr++;
            oldcat = exprcat;
            t = t >= scanexpr(7, &p);
            exprcat |= oldcat | EXPRCAT_FIXED;
            }
          else
            {
            if (level >= 7)
              EXITEVAL
            oldcat = exprcat;
            t = t > scanexpr(7, &p);
            exprcat |= oldcat | EXPRCAT_FIXED;
            }
          break;
        case '!':
          if (level >= 6 || *srcptr != '=')
            EXITEVAL
          srcptr++;
          oldcat = exprcat;
          t = t != scanexpr(6, &p);
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;             
        case '=':
          if (level >= 6)
            EXITEVAL
          if (*srcptr == '=')
            srcptr++;
          oldcat = exprcat;
          t = (t == scanexpr(6, &p));
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;
        case '&':
          if (level >= 5)
            EXITEVAL
          oldcat = exprcat;
          t &= scanexpr(5, &p);
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;
        case '^':
          if (level >= 4)
            EXITEVAL
          oldcat = exprcat;
          t ^= scanexpr(4, &p);
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;
        case '|':
          if (level >= 3)
            EXITEVAL
          oldcat = exprcat;
          t |= scanexpr(3, &p);
          exprcat |= oldcat | EXPRCAT_FIXED;
          break;
        default:
          EXITEVAL
      }
      p.exprcat = exprcat;
      if (p.sym)
      {
        if (ip.sym)                         /* if 2 symbols, cancel 'em          */
        {
          /* a simple safeguard against cancelling local vs. external symbols 
             or operations between 2 external symbols.
             This means that an external symbol has to be the last one in an
             expression, or the others have to be paired so that they cancel
             each other's effect AND their subexpression has to be parenthesized.*/
          char cat1 = ip.sym->cat;
          char cat2 = p.sym->cat;
          if (cat1 == SYMCAT_EXTERN || cat2 == SYMCAT_EXTERN)
            error |= ERR_EXPR;
          else
            ip.sym = NULL;                  /* this might be TOO crude...        */
        }
        else                                /* if new symbol                     */
          ip = p;                           /* use this one                      */
      }
    }

    *pp = ip;
    return t;
}

/*****************************************************************************/
/* isindexreg : returns whether on an index register                         */
/*****************************************************************************/

int isindexreg()
{
    switch (toupper(*srcptr))
    {
      case 'X':
      case 'Y':
      case 'U':
      case 'S':
        return 1;
      case 'W' :
        if (dwOptions & OPTION_H09)
          return 1;
        break;
    }  
    return 0;
}

/*****************************************************************************/
/* scanindexreg : scans an index register                                    */
/*****************************************************************************/

int scanindexreg()
{
    switch (toupper(*srcptr))
    {
      case 'X':
        return 1;
      case 'Y':
        postbyte |= 0x20;
        return 1;
      case 'U':
        postbyte |= 0x40;
        return 1;
      case 'S':
        postbyte |= 0x60;
        return 1;
    }  
    return 0;
}

/*****************************************************************************/
/* set3 : sets mode to at least ADRMODE_POST                                 */
/*****************************************************************************/

void set3()
{
    if (mode < ADRMODE_POST)
      mode = ADRMODE_POST;
}

/*****************************************************************************/
/* scanspecial : scans for increments                                        */
/*****************************************************************************/

void scanspecial()
{
    set3();
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr == '-')
    {
      srcptr++;
      if (*srcptr == '-')
        {
        srcptr++;
        postbyte = 0x83;
        }
      else
        postbyte = 0x82;

      if ((dwOptions & OPTION_H09) &&       /* special for ,--W and [,--W]       */
          (postbyte == 0x83) &&
          (toupper(*srcptr) == 'W'))
        {
        postbyte = (mode == ADRMODE_IND) ? 0x90 : 0x8f;
        srcptr++;
        }
      else if (!scanindexreg())
        error |= ERR_ILLEGAL_ADDR;
      else
        srcptr++; 
    }
    else
    {
      postbyte = 0x80;
      if ((dwOptions & OPTION_H09) &&       /* special for ,W, [,W], ,W++, [,W++]*/
          (toupper(*srcptr) == 'W'))
      {
        srcptr++;                           /* advance behind W                  */
        if (*srcptr == '+')
        {
          srcptr++;
          if (*srcptr == '+')               /* ,W++ and [,W++]                   */
          {
            postbyte = (mode == ADRMODE_IND) ? 0xB0 : 0xAF;
            srcptr++;
          }
          else
            error |= ERR_ILLEGAL_ADDR;
        }
        else                                /* ,W and [,W]                       */
          postbyte = (mode == ADRMODE_IND) ? 0x90 : 0x8F;
      }
      else                                  /* normal index register addressing  */
      {
        if (!scanindexreg())
          error |= ERR_ILLEGAL_ADDR;
        else
          srcptr++;
        if (*srcptr == '+')
        {
          srcptr++;
          if (*srcptr == '+')
            {
            srcptr++;
            postbyte += 1;
            }  
        }
        else
          postbyte += 4;
      }
    }    
}

/*****************************************************************************/
/* scanindexed :                                                             */
/*****************************************************************************/

void scanindexed()
{
    set3();
    postbyte = 0;
    if ((dwOptions & OPTION_H09) &&         /* special for W as index register   */
        (toupper(*srcptr) == 'W'))
    {
      srcptr++;
      postbyte = (mode == ADRMODE_IND) ? 0xb0 : 0xaf;
      opsize = 3;
    }
    else if (scanindexreg())
    {
      srcptr++;
      if (opsize == 0)
      {
        if (unknown || !certain)
          opsize = 3;
        else if (operand >= -16 && operand < 16 && mode == ADRMODE_POST)
          opsize = 1;
        else if (operand >= -128 && operand < 128)
          opsize = 2;
        else
          opsize = 3;
      }
      switch (opsize)
      {
        case 1:
          postbyte += (operand & 31);
          opsize = 0;
          break;
        case 2:
          postbyte += 0x88;
          break;
        case 3:
          postbyte += 0x89;
          break;
      }                 
    }
    else
    { /*pc relative*/
      if (toupper(*srcptr) != 'P')
        error |= ERR_ILLEGAL_ADDR;
      else
      {
        srcptr++;
        if (toupper(*srcptr) != 'C')
          error |= ERR_ILLEGAL_ADDR;
        else
          {
          srcptr++;
          if (toupper(*srcptr) == 'R')
            srcptr++;
          } 
      }
      mode++;
      if (!absmode && !certain)
      {
        /* This may not be enough. */
        if (mode == ADRMODE_PCR &&
            exprcat == EXPRCAT_INTADDR &&
            !unknown)
          certain = 1;
      }
      postbyte += 0x8c;
      if (opsize == 1)
        opsize = 2;    
    }
}

/*****************************************************************************/
/* scanoperands :                                                            */
/*****************************************************************************/

void dodefault(struct relocrecord *pp) {
    operand = scanexpr(0, pp);
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr == ',')
    {
      srcptr++;
      if (!(dwOptions & OPTION_TSC))
        skipspace();
    #if 1
      /* The TSC 6809 assembler, according to its documentation,
         allows constructs like "0,-[-]reg" or "0,reg+[+]", i.e., it treats
         "zero offset" like "no offset". Duplicated here. */
      if ((operand == 0) &&             /* special for "0,-[-]indexreg       */
          (!unknown) && (certain) &&    /*         and "0,indexreg[+[+]]     */
          (opsize < 2) &&               /* but NOT for ">0,indexreg"!        */
          (*srcptr == '-' ||
           (isindexreg() && srcptr[1] == '+')))
        scanspecial();
      else
    #endif
        scanindexed();
    }
    else
    {
      if (opsize == 0)
      {
        if (unknown || !certain || dpsetting == -1 ||
          (unsigned short)(operand - dpsetting * 256) >= 256)
          opsize = 3;
        else
          opsize = 2;
      }  
      if (opsize == 1)
        opsize = 2;         
      if (mode == ADRMODE_IND)
      {
        postbyte = 0x8f;
        opsize = 3;
      }
      else
        mode = opsize - 1;
    }
}

void accoffset(char c, unsigned char accpost, unsigned char h63, struct relocrecord *pp) {
    char *oldsrcptr = srcptr;
    srcptr++;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr != ',')
      { srcptr = oldsrcptr; c = *srcptr; dodefault(pp); }
    else
    {
      if ((h63) && (!(dwOptions & OPTION_H09)))
        error |= ERR_ILLEGAL_ADDR;
      postbyte = accpost;
      srcptr++;
      if (!scanindexreg())
        { srcptr = oldsrcptr; c = *srcptr; dodefault(pp); }
      else
      {
        srcptr++;
        set3();
      }
    }
}

void scanoperands09(struct relocrecord *pp, int endc)
{
    char c, *oldsrcptr;
    unsigned char accpost, h63 = 0;

    unknown = 0;
    opsize = 0;
    certain = 1;
    skipspace();
    c = *srcptr;
    mode = ADRMODE_IMM;
    if (c == '[')
    {
      c = *++srcptr;
      mode = ADRMODE_IND;
    }
    switch (toupper(c))
    {
      case 'D':
        accpost = 0x8b;
/*      accoffset:                // label
        oldsrcptr = srcptr;
        srcptr++;
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (*srcptr != ',')
          { srcptr = oldsrcptr; c = *srcptr; dodefault(pp); }
        else
        {
          if ((h63) && (!(dwOptions & OPTION_H09)))
            error |= ERR_ILLEGAL_ADDR;
          postbyte = accpost;
          srcptr++;
          if (!scanindexreg())
            { srcptr = oldsrcptr; c = *srcptr; dodefault(pp); }
          else
          {
            srcptr++;
            set3();
          }
        }   */
        accoffset(c, accpost, h63, pp);
        break;    
      case 'A':
        accpost = 0x86;
//        goto accoffset;         // goto
        accoffset(c, accpost, h63, pp);
        break;
      case 'B':
        accpost = 0x85;
//        goto accoffset;         // goto
        accoffset(c, accpost, h63, pp);
        break;
      case 'E':
        accpost = 0x87;
        h63 = 1;
//        goto accoffset;         // goto
        accoffset(c, accpost, h63, pp);
        break;
      case 'F':
        accpost = 0x8a;
        h63 = 1;
//        goto accoffset;         // goto
        accoffset(c, accpost, h63, pp);
        break;
      case 'W' :
        accpost = 0x8e;
        h63 = 1;
//        goto accoffset;         // goto
        accoffset(c, accpost, h63, pp);
        break;
      case ',':
        srcptr++;
        scanspecial();
        break; 
      case '#':
        if (mode == ADRMODE_IND)
          error |= ERR_ILLEGAL_ADDR;
        else
          mode = ADRMODE_IMM;
        srcptr++;
        operand = scanexpr(0, pp);
        break;
      case '<':
        srcptr++;
        if (*srcptr == '<')
        {
          srcptr++;
          opsize = 1;
        }
        else
          opsize = 2;
        dodefault(pp);
        break;
      case '>':
        srcptr++;
        opsize = 3;
        /* fall thru on purpose */
      default:
        dodefault(pp);

    } // end switch

    if (mode >= ADRMODE_IND)
    {
      if (!(dwOptions & OPTION_TSC))
        skipspace();
      postbyte |= 0x10;
      if (*srcptr != ']')
        error |= ERR_ILLEGAL_ADDR;
      else
        srcptr++;    
    }
    if (pass == 2 && unknown)
      error |= ERR_LABEL_UNDEF; 
}

void scanoperands00(struct relocrecord *pp, int endc)
{
    char c, *s = srcptr;

    unknown = 0;
    opsize = 0;
    certain = 1;
    operand = 0;
    skipspace();
    c = *srcptr;
    mode = ADRMODE_IMM;
    switch (toupper(c))
    {
      case 'X' :                            /* "X"?                              */
        scanname();
        if (!strcmp(unamebuf, "X"))         /* if it's "X" alone,                */
          goto XWithout;                    /* assume it means "0,X"             */
        srcptr = s;                         /* else restore current offset       */
        goto dodefault;                     /* and treat as label starting with X*/
      case ',':                             /* ","?                              */
      Indexed :     // label for GOTO!
        srcptr++;                           /* must be followed by "X"           */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        scanname();
        if (strcmp(unamebuf, "X"))          /* if it's NOT "X" alone,            */
        {
          error |= ERR_ILLEGAL_ADDR;
          break;
        }
      XWithout :
        if ((unsigned)operand < 256)
          mode = ADRMODE_IDX;
        else
          error |= ERR_ILLEGAL_ADDR;
        break; 
      case '#':
        srcptr++;
        operand = scanexpr(0, pp);
        break;
      case '<':
        srcptr++;
        if (*srcptr == '<')
          {
          srcptr++;
          opsize = 1;
          }
        else
          opsize = 2;
        goto dodefault;    
      case '>':
        srcptr++;
        opsize = 3;
        /* fall thru on purpose */
      default:
      dodefault:
        operand = scanexpr(0, pp);
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (*srcptr == ',')
          goto Indexed;         //    GOTO !
        else
        {
          if (opsize == 0)
          {
            if (unknown || !certain || 
              (unsigned short)(operand) >= 256)
              opsize = 3;
            else
              opsize = 2;
          }  
          if (opsize == 1)
            opsize = 2;         
          mode = opsize - 1;
        }
    }

    if (pass == 2 && unknown)
      error |= ERR_LABEL_UNDEF; 
}

void scanoperands11(struct relocrecord *pp, int endc)
{
    char c, *s = srcptr, *next;

    unknown = 0;
    opsize = 0;
    certain = 1;
    operand = 0;
    skipspace();
    c = *srcptr;
    mode = ADRMODE_IMM;

    switch (toupper(c))
    {
      case 'X' :                            /* "X"?                              */
      case 'Y' :                            /* "Y"?                              */
        scanname();
        if (!unamebuf[1])                   /* if it's "X" or "Y" alone,         */
          goto IdxWithout;                  /* assume it means "0,X" / "0,Y"     */
        srcptr = s;                         /* else restore current offset       */
        goto dodefault;                     /* and treat as label                */
      case ',':                             /* ","?                              */
      Indexed :   // label for goto
        srcptr++;                           /* must be followed by "X" or "Y"    */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        scanname();
        if (strcmp(unamebuf, "X") &&        /* if it's NOT "X" alone             */
            strcmp(unamebuf, "Y"))          /* or "Y" alone                      */
        {
          error |= ERR_ILLEGAL_ADDR;
          break;
        }
      IdxWithout :                     // label
        if ((unsigned)operand < 256)
        {
          if (unamebuf[0] == 'X')           /* page byte processing for X        */
          {
            if (codeptr)
            {
              if (codebuf[0] == 0x18)
                codebuf[0] = 0x1a;
            }
          }
          else if (unamebuf[0] == 'Y')      /* page byte processing for Y        */
          {
            if (!codeptr)
            {
              int i;
              for (i = codeptr++; i > 0; i--)
                codebuf[i] = codebuf[i - 1];
              codebuf[0] = 0x18;
            }
            else if (codebuf[0] == 0x1a)
              codebuf[0] = 0xcd;
          }
          mode = ADRMODE_IDX;
        }
        else
          error |= ERR_ILLEGAL_ADDR;
        break; 
      case '#':
        srcptr++;
        operand = scanexpr(0, pp);
        break;
      case '<':
        srcptr++;
        if (*srcptr == '<')
          {
          srcptr++;
          opsize = 1;
          }
        else
          opsize = 2;
        goto dodefault;    // goto
      case '>':
        srcptr++;
        opsize = 3;
        /* fall thru on purpose */
      default:
      dodefault:           // label for goto
        operand = scanexpr(0, pp);
        if (!(dwOptions & OPTION_TSC))
          skipspace();
    #if 1
        if (*srcptr == ',')
        {
          if (!endc)
            goto Indexed;    // goto
          next = srcptr + 1;                /* if endc, parse ahead ...          */
          if (!(dwOptions & OPTION_TSC))
            parsespace(next, &next);
          parsename(next, &next);
          if (!strcmp(unamebuf, "X") ||     /* if it's "X" or "Y",               */
              !strcmp(unamebuf, "Y"))       /* or "Y" alone                      */
          {
            srcptr = next;
            goto IdxWithout;                /* process index argument            */
          }
          /* otherwise end parsing here */
        }
    #else
        if (*srcptr == ',')
        {
          if (!endc)
            goto Indexed;        // goto
        }
        else
    #endif
        {
          if (opsize == 0)
          {
            if (unknown || !certain || 
              (unsigned short)(operand) >= 256)
              opsize = 3;
            else
              opsize = 2;
          }  
          if (opsize == 1)
            opsize = 2;         
          mode = opsize - 1;
        }
    }

    if (pass == 2 && unknown)
      error |= ERR_LABEL_UNDEF; 
}

/*****************************************************************************/
/* writerelhdr : writes a FLEX Relocatable Format header                     */
/*****************************************************************************/

void writerelhdr(char wcommon)
{
    int i;

    #if 0
    struct                                  /* Header Layout                     */
    {
      unsigned char Signature;              /* Signature (always $03)            */
      unsigned char flags1;                 /* Flags 1                           */
      unsigned short datasize;              /* size of binary data               */
      unsigned char unknown1[4];            /* unknown data                      */
      unsigned short extsize;               /* size of External table            */
      unsigned short startaddr;             /* program start address             */
      unsigned char unknown2[2];            /* unknown data                      */
      unsigned short globalsize;            /* size of global table              */
      unsigned char unknown3[2];            /* unknown data                      */
      unsigned short namesize;              /* length of module name             */
      unsigned char flags2;                 /* Flags 2                           */
      unsigned char unknown4[3];            /* unknown data; filler?             */
    } hdr;
    memset(&hdr, 0, sizeof(hdr));
    #endif

    fputc(0x03, objfile);                   /* write signature                   */

    if (wcommon)                            /* if writing common data,           */
      fputc(0x18, objfile);                 /* Flags1 = $18                      */
    else if (!absmode)                      /* if writing Relative data,         */
      fputc(0x10, objfile);                 /* Flags1 = $10                      */
    else                                    /* if writing Absolute data,         */
      fputc(0x12, objfile);                 /* Flags1 = $12                      */

    if (wcommon)                            /* if writing common data            */
      {                                     /* write size of data                */
      fputc((unsigned char)(commonsym->value >> 8), objfile);
      fputc((unsigned char)(commonsym->value & 0xFF), objfile);
      }
    else                                    /* otherwise                         */
      {                                     /* write size of binary data         */
    /* this is wrong, as the current block doesn't necessarily start at 0 in abs mode! */
    /* TODO: replace this with reldatasize once it's calculated correctly */
    #if 1
      fputc((unsigned char)(reldatasize >> 8), objfile);
      fputc((unsigned char)(reldatasize & 0xFF), objfile);
    #else
      fputc((unsigned char)(loccounter >> 8), objfile);
      fputc((unsigned char)(loccounter & 0xFF), objfile);
    #endif
      }

    fputc(0, objfile);                      /* unknown data                      */
    fputc(0, objfile);
    fputc(0, objfile);
    fputc(0, objfile);

    if (wcommon)                            /* if writing common data            */
      {                                     /* external table is empty           */
      fputc(0, objfile);
      fputc(0, objfile);
      }
    else
      {
      int extrel = 0;                       /* # external relocation records     */

      for (i = 0; i < relcounter; i++)      /* calc # external symbols           */
        {
        char cat = reltable[i].sym->cat;
        if ((cat == SYMCAT_EXTERN) ||
            (cat == SYMCAT_COMMON))
          extrel++;
        }
                                            /* then calculate table size         */
      extrel = (extrel * 8) + (relcounter * 3);
                                            /* and write it out                  */
      fputc((unsigned char)(extrel >> 8), objfile);
      fputc((unsigned char)(extrel & 0xFF), objfile);
      }

    if (wcommon ||                          /* if writing common data            */
        (!tfradrset))                       /* or no transfer address given      */
      {                                     /* start address is empty            */
      fputc(0, objfile);
      fputc(0, objfile);
      }
    else                                    /* write transfer address            */
      {
      fputc((unsigned char)(tfradr >> 8), objfile);
      fputc((unsigned char)(tfradr & 0xFF), objfile);
      }

    fputc(0, objfile);                      /* unknown data                      */
    fputc(0, objfile);

    if (wcommon)
      {                                     /* always 1 Global - the LABEL       */
      fputc(0, objfile);
      fputc(12, objfile);
      }
    else                                    /* calculate & write out global size */
      {
      int globals = 0;
      for (i = 0; i < symtable.counter; i++)
        if (symtable.rec[i].cat == SYMCAT_PUBLIC)
          globals++;
      globals *= 12;
      fputc((unsigned char)(globals >> 8), objfile);
      fputc((unsigned char)(globals & 0xFF), objfile);
      }

    fputc(0, objfile);                      /* unknown data                      */
    fputc(0, objfile);

    if (wcommon)
      {                                     /* no name size yet ...              */
      size_t len;
      char name[9] = "";
      sprintf(name, "%-.8s", commonsym->name);
      strupr(name);
      len = strlen(name);
      if (len)                              /* if there,                         */
        len++;                              /* append a $04                      */
      fputc((unsigned char)(len >> 8), objfile);
      fputc((unsigned char)(len & 0xFF), objfile);
      }
    else                                    /* write out module name size        */
      {
      size_t len = strlen(modulename);
      if (len)                              /* if there,                         */
        len++;                              /* append a $04                      */
      fputc((unsigned char)(len >> 8), objfile);
      fputc((unsigned char)(len & 0xFF), objfile);
      }

    if ((!wcommon) && (tfradrset))          /* if transfer address set           */
      fputc(0x80, objfile);                 /* write $80 flag                    */
    else
      fputc(0, objfile);

    fputc(0, objfile);                      /* unknown data                      */
    fputc(0, objfile);
    fputc(0, objfile);
}

/*****************************************************************************/
/* writerelcommon : writes out all common blocks                             */
/*****************************************************************************/

void writerelcommon()
{
    int i, j;
    char name[9];
                                            /* work through symbol list          */
    for (i = 0; i < symtable.counter; i++)
    {                                     /* if that is a common block         */
      if (symtable.rec[i].cat == SYMCAT_COMMON)
      {
        commonsym = symtable.rec + i;       /* write it out                      */
        writerelhdr(1);
                                            /* then write the global definition  */
        sprintf(name, "%-8.8s", symtable.rec[i].name);
        // strupr(name);
        fwrite(name, 1, 8, objfile);
        fputc(1, objfile);                  /* unknown data                      */
        fputc((unsigned char)(symtable.rec[i].value >> 8), objfile);
        fputc((unsigned char)(symtable.rec[i].value & 0xFF), objfile);
        fputc(0x13, objfile);               /* unknown flag                      */

                                            /* then write the Common name        */
        sprintf(name, "%-.8s", symtable.rec[i].name);
        // strupr(name);
        for (j = 0; name[j]; j++)
          fputc(name[j], objfile);
        fputc(0x04, objfile);

        j = (int)ftell(objfile);            /* fill last sector with zeroes      */
        while (j % 252)
        {
          fputc(0, objfile);
          j++;
        }
      }
    }
}

/*****************************************************************************/
/* writerelext : writes out a FLEX Relocatable External table                */
/*****************************************************************************/

void writerelext()
{
    int i;
    char name[9];
    unsigned char flags;

    for (i = 0; i < relcounter; i++)        /* write out the external data       */
    {
      char cat = reltable[i].sym->cat;
      fputc((unsigned char)(reltable[i].addr >> 8), objfile);
      fputc((unsigned char)(reltable[i].addr & 0xFF), objfile);

      flags = 0x00;                         /* reset flags                       */
      if (reltable[i].exprcat & EXPRCAT_EXTOFF)
        flags |= 0x10;                      /* add offset flag if necessary      */
      if (reltable[i].exprcat & EXPRCAT_NEGATIVE)
        flags |= 0x20;                      /* add subtraction flag if necessary */
      if (reltable[i].exprcat & EXPRCAT_ONEBYTE)
        flags |= 0x40;                      /* add single-byte flag if necessary */
      if ((cat == SYMCAT_EXTERN) ||
          (cat == SYMCAT_COMMON))
        flags |= 0x80;                      /* add External flag where necessary */
      fputc(flags, objfile);                /* write the flag bytes              */
      if (flags & 0x80)                     /* write external symbol if needed   */
      {
        sprintf(name, "%-8.8s", reltable[i].sym->name);
        // strupr(name);
        fwrite(name, 1, 8, objfile);
      }
    }
}

/*****************************************************************************/
/* writerelglobal : writes out FLEX Relocatable Global table                 */
/*****************************************************************************/

void writerelglobal()
{
    int i;
    char name[9];

    for (i = 0; i < symtable.counter; i++)  /* write out the global data         */
    {
      if (symtable.rec[i].cat == SYMCAT_PUBLIC)
      {
        int flag2 = 0x02;                   /* start with GLOBAL flag            */
                                            /* add ABSOLUTE if necessary         */
        if (symtable.rec[i].u.flags & SYMFLAG_ABSOLUTE)
          flag2 |= 0x10;

        sprintf(name, "%-8.8s", symtable.rec[i].name);
        // strupr(name);
        fwrite(name, 1, 8, objfile);

        fputc(0, objfile);                  /* unknown data                      */

        fputc((unsigned char)(symtable.rec[i].value >> 8), objfile);
        fputc((unsigned char)(symtable.rec[i].value & 0xFF), objfile);

        fputc(flag2, objfile);              /* unknown flag                      */
      }
    }
}

/*****************************************************************************/
/* writerelmodname : writes out FLEX Relocatable Module Name                 */
/*****************************************************************************/

void writerelmodname()
{
    int i;

    if (!modulename[0])
      return;
    // strupr(modulename);
    for (i = 0; modulename[i]; i++)
      fputc(modulename[i], objfile);
    fputc(0x04, objfile);
}

/*****************************************************************************/
/* flushhex : write Motorola s-records                                       */
/*****************************************************************************/

void flushhex()
{
    int i;

    if (hexcount)
    {
      if (objfile)
      {
        fprintf(objfile, "S1%02X%04X", (hexcount + 3) & 0xff, hexaddr & 0xffff);
        for (i = 0; i < hexcount; i++)
          fprintf(objfile, "%02X", hexbuffer[i]);
        chksum += (hexaddr & 0xff) + ((hexaddr >> 8) & 0xff) + hexcount + 3;
        fprintf(objfile, "%02X\n", 0xff - (chksum & 0xff));
      }
      hexaddr += hexcount;
      hexcount = 0;
      chksum = 0;
    }
}

/*****************************************************************************/
/* flushihex : write Intel hex record                                        */
/*****************************************************************************/

void flushihex()
{
    int i;
    unsigned char  *j;

    if (hexcount)
    {
      if (objfile)
      {
        j = &hexbuffer[0];
        fprintf(objfile, ":%02X%04X00", hexcount, hexaddr & 0xffff);
        chksum = hexcount + ((hexaddr >> 8) & 0xff) + (hexaddr & 0xff);
        for (i = 0; i < hexcount; i++, j++)
        {
          chksum += *j;
          fprintf(objfile, "%02X", *j);
        }
        fprintf(objfile, "%02X\n", (-(signed)chksum) & 0xff);
      }
      hexaddr += hexcount;
      hexcount = 0;
      chksum = 0;
    }
}

/*****************************************************************************/
/* flushflex : write FLEX binary record                                      */
/*****************************************************************************/

void flushflex()
{
    int i;
    unsigned char *j;

    if (hexcount)
    {
      j = &hexbuffer[0];
      if (objfile)
      {
        fputc(0x02, objfile);               /* start of record indicator         */
        fputc((hexaddr >> 8) & 0xff,        /* load address high part            */
              objfile);
        fputc(hexaddr & 0xff, objfile);     /* load address low part             */
        fputc(hexcount & 0xff, objfile);    /* # following data bytes            */
        for (i = 0; i < hexcount; i++, j++) /* then put all data bytes           */
          fputc(*j, objfile);
      }
      hexaddr += hexcount;                  /* set new address                   */
      hexcount = 0;                         /* reset counter                     */
    }
}

/*****************************************************************************/
/* outhex : add a byte to motorola s-record output                           */
/*****************************************************************************/

void outhex (unsigned char x) 
{
    if (hexcount == hexmaxcount)
      flushhex();
    hexbuffer[hexcount++] = x;
    chksum += x;
}

/*****************************************************************************/
/* outihex : add a byte to intel hex output                                  */
/*****************************************************************************/

void outihex (unsigned char x) 
{
    if (hexcount == ihexmaxcount)
      flushihex();
    hexbuffer[hexcount++] = x;
    chksum += x;
}

/*****************************************************************************/
/* outflex : adds a byte to FLEX output                                      */
/*****************************************************************************/

void outflex(unsigned char x)
{
    if (hexcount == iflexmaxcount)          /* if buffer full                    */
      flushflex();                          /* flush it                          */
    hexbuffer[hexcount++] = x;              /* then put byte into buffer         */
}

/*****************************************************************************/
/* outbyte : writes one byte to the output in the selected format            */
/*****************************************************************************/

void outbyte(unsigned char uc, int off)
{
    int nByte = (loccounter + off) / 8;
    unsigned char nBitMask = (unsigned char) (1 << ((loccounter + off) % 8));

    if (bUsedBytes[nByte] & nBitMask)       /* if address already used           */
      warning |= WRN_AREA;                  /* set warning code                  */
    else                                    /* otherwise                         */
      bUsedBytes[nByte] |= nBitMask;        /* mark it as used                   */

    if ((outmode == OUT_REL) &&             /* if in REL output mode             */
        (absmode))                          /* and producing an absolute module  */
    {
      if (reldataorg + 1 != loccounter + off)
      {
        long curoff = ftell(objfile);
        if (relabsfoff >= 0)
        {
          long blockcnt = curoff - relabsfoff - 4;
          fseek(objfile, relabsfoff, SEEK_SET);
          fputc((unsigned char)(blockcnt >> 8), objfile);
          fputc((unsigned char)(blockcnt & 0xFF), objfile);
          fseek(objfile, curoff, SEEK_SET);
        }
        relabsfoff = curoff;
        fputc(0, objfile);
        fputc(1, objfile);
        fputc((unsigned char)((loccounter + off) >> 8), objfile);
        fputc((unsigned char)((loccounter + off) & 0xFF), objfile);
        reldatasize += 4;
      }
      reldataorg = loccounter + off;
    }

    switch (outmode)
    {
      case OUT_BIN :                        /* binary file                       */
      case OUT_REL :                        /* FLEX Relocatable                  */
        fputc(uc, objfile);
        reldatasize++;
        break;
      case OUT_SREC :                       /* Motorola S-records                */
        outhex(uc);
        break;
      case OUT_IHEX :                       /* Intel Hex                         */
        outihex(uc);
        break;
      case OUT_FLEX :                       /* FLEX                              */
        outflex(uc);
        break;
    }
}

/*****************************************************************************/
/* outbuffer : writes the output to a file in the selected format            */
/*****************************************************************************/

void outbuffer()
{
    int i;
    for (i = 0; i < codeptr; i++)
      outbyte(codebuf[i], i);
}

/*****************************************************************************/
/* report : reports an error                                                 */
/*****************************************************************************/

void report()
{
    int i;

    for (i = 0; i < 16; i++)
    {
      if (error & 1)
      {
        printf("%s(%ld) : error %d: %s in \"%s\"\n", 
               expandfn(curline->fn), curline->ln, i + 1,
               errormsg[i], curline->txt);
        if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
          putlist( "*** Error %d: %s\n", i + 1, errormsg[i]);
        errors++;
      }
      error >>= 1;
    }

    if ((dwOptions & OPTION_TSC))           /* suppress warning 1 in TSC mode    */
      warning &= ~WRN_OPT;

    if (!(dwOptions & OPTION_WAR))          /* reset warnings if not wanted      */
      warning = WRN_OK;

    for (i = 0; i < 16; i++)
    {
      if (warning & 1)
      {
        printf("%s(%ld) : warning %d: %s in \"%s\"\n", 
               expandfn(curline->fn), curline->ln, i + 1, warningmsg[i], curline->txt);
        if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
          putlist( "*** warning %d: %s\n", i + 1, warningmsg[i]);
        warnings++;
      }
      warning >>= 1;
    }
} 

/*****************************************************************************/
/* pemt_expr : recursively processes a PEMT expression enclosed in @(...)    */
/*****************************************************************************/

int pemt_expr
    (
    const char *szBuf1,                 /* buffer we're working on           */
    int atat,                           /* @ position in szBuf1              */
    char *szOutBuf,                     /* output buffer address             */
    int *newpos                         /* position after processing         */
    )
{
    char lBsl = 0;
    char szInt[LINELEN];
    int i, lvl = 1, ol = 0, odec = 0;
    const char *fmts[] =
    {
      "%04lX",
      "%ld",
      "%04lX"
    };

    for (i = atat + 2; szBuf1[i]; i++)      /* walk through the string           */
    {
      char c = szBuf1[i];
      if (c == '*' && !lBsl)                /* if on a plain *, replace with addr*/
      {
        /* inside @(...), * is expanded to $addr, as this is the format used
           in scanexpr()! */
        int odec2 = 0;
        const char *fmts2[] =
        {
          "$%04lX",
          "%ld",
          "$%04lX"
        };
        if (szBuf1[i + 1] == ',' &&         /* *,d means "output addr as dec"    */
            (szBuf1[i + 2] == 'd' ||        /* *,x means output addr as hex"     */
             szBuf1[i + 2] == 'x'))
        {
          odec2 = 1 + (szBuf1[i + 2] == 'x');
          i += 2;
        }
        ol += sprintf(szInt + ol, fmts2[odec2], oldlc);
      }
      else if (c == '@' && szBuf1[i + 1] == '(' && !lBsl)
      {                                   /* if on @(, recurse                 */
        ol += pemt_expr(szBuf1, i, szInt + ol, &i);
      }
      else if (c == '(')                    /* if on (, increase level           */
      {
        if (!lBsl)
          lvl++;
        szInt[ol++] = c;
        szInt[ol] = '\0';
      }
      else if (c == ')')                    /* if on ), stop this.               */
      {
        if (!lBsl)
          lvl--;
        if (!lvl)
          {
          /* ),d as end means "output as dec", ),x means "output as hex"  */
          if (szBuf1[i + 1] == ',' && 
              (szBuf1[i + 2] == 'd' || szBuf1[i + 2] == 'x'))
          {
            odec = 1 + (szBuf1[i + 2] == 'x');
            i += 2;
          }
          break;
          }
        szInt[ol++] = c;
        szInt[ol] = '\0';
      }
      else if (c == '\\' && !lBsl)
        ;
      else
      {
        szInt[ol++] = c;
        szInt[ol] = '\0';
      }

      lBsl = !lBsl && (c == '\\');          /* remember whether on \             */
    }

    *newpos = i;                            /* tell caller where we ended        */
    if (i > atat + 2)                       /* if something to evaluate          */
    {                                     /* evaluate it                       */
      char *osp = srcptr;
      long l;
      struct relocrecord pp = {0};
      srcptr = szInt;
      l = scanexpr(0, &pp);
      if (pass == 2 && unknown)             /* unresolved items are not allowed  */
        error |= ERR_LABEL_UNDEF;
      ol = sprintf(szOutBuf, fmts[odec], l);
      srcptr = osp;
    }

    return ol;
}

/*****************************************************************************/
/* pemt_putlist : lists the (rest of the) line with PEMT expressions         */
/*****************************************************************************/

void pemt_putlist(const char *szLine, int off)
{
    /* Specials: 
       - '*' means "current address", unless prefixed with '\'
         always output as hex value without leading $, as this is
         for dasmfw / f9dasm, which expect hex without $.
         For more complex evaluations, use @(...).
       - '@(...)' means "evaluate the expression with scanexpr() and
         use the result as a numerical value", unless prefixed with '\'
    */
    char lBsl = 0;
    for ( ; szLine[off]; off++)
    {
      char c = szLine[off];
      if (szLine[off] == '*' && !lBsl)
        putlist("%04X", oldlc);
      else if (c == '@' && szLine[off + 1] == '(' && !lBsl)
      {
        char szExpr[33];
        pemt_expr(szLine, off, szExpr, &off);
        putlist("%s", szExpr);
      }
      else if (c == '\\' && !lBsl)          /* \ is an escape char, so ...       */
        ;                                   /* skip the first one.               */
        /* NB: to output a \, two are needed. To output \*, \\\* must be written */
      else
        putlist("%c", c);
      lBsl = !lBsl && (c == '\\');
    }
    putlist("\n");
}

/*****************************************************************************/
/* outlist : lists the code bytes for an instruction                         */
/*****************************************************************************/

void outlist(struct oprecord *op, struct symrecord *lpLabel)
{
    int i;

    if (LINE_IS_INVISIBLE(curline->flg) &&  /* don't list invisible lines        */
        !(dwOptions & OPTION_INV))
      return;

    if (LINE_IS_MACINV(curline->flg) &&     /* don't list macro invocations if   */
        !(dwOptions & OPTION_MAC))          /* not explicitly requested          */
      return;

    if (LINE_IS_MACEXP(curline->flg) &&     /* don't list macro expansions if    */
        !(dwOptions & OPTION_EXP))          /* not explicitly requested          */
      return;

    if ((curline->lvl & LINCAT_LVLMASK) &&  /* if level 1..nn                    */
        !(dwOptions & OPTION_LLL))          /* and this is not to be listed      */
      return;

    if ((suppress || nSkipCount) &&         /* if this is conditionally skipped  */
        !(dwOptions & OPTION_CON))          /* and this is not to be listed      */
      return;
    if ((condline) &&                       /* if this is a condition line       */
        !(dwOptions & OPTION_CON))          /* and this is not to be listed      */
      return;

    if (dwOptions & OPTION_NUM)             /* if number output                  */
      putlist("%4d ", curline->ln);         /* print out the line number         */

    if (!absmode &&                         /* if in relocating assembler mode   */
        !(dwOptions & OPTION_LPA))
      putlist("%c", curline->rel);          /* output relocation information     */

    if (dwOptions & OPTION_LPA)             /* if in patch mode                  */
    {
      const char *blnkfil = "      ";
      if (LINE_IS_PEMTCMT(curline->flg))    /* if PEMT comment flag set          */
      {
        putlist("*");                       /* prefix line with *                */
        blnkfil++;
      }
      if ((op) && (op->cat == OPCAT_PSEUDO))
      {
        switch (op->code)
        {
          case PSEUDO_SETDP :
            if (dpsetting >= 0)
              putlist("setdp %04X %02X\n", loccounter, dpsetting);
            break;
          case PSEUDO_ORG :
    #if 0
            putlist("insert %04X \\        ORG     $%04X\n", loccounter, loccounter);
    #endif
            break;
          case PSEUDO_RZB :
          case PSEUDO_FILL :
          case PSEUDO_FCB :
          case PSEUDO_FCC :
            putlist("data %04X", oldlc);
            if (codeptr > 1)
              putlist("-%04X", oldlc + codeptr - 1);
            /* we got a little conceptual problem here... if RZB or FILL reserve
            more than sizeof(codebuf) bytes, this is not correctly reflected. */
            putlist("\n");
            break;
          case PSEUDO_FCW :
            putlist("word %04x", oldlc);
            if (codeptr > 2)
              putlist("-%04X", oldlc + codeptr - 1);
            putlist("\n");
            break;
          case PSEUDO_FCQ :
            putlist("dword %04x", oldlc);
            if (codeptr > 2)
              putlist("-%04X", oldlc + codeptr - 1);
            putlist("\n");
            break;
          case PSEUDO_PEMT :
            pemt_putlist(szBuf1, 0);
            break;
        }
      }
      if (codeptr > 0)                      /* if there are code bytes           */
      {
        char *name = lpLabel ? lpLabel->name : findsymat(oldlc);
        if (name)
          putlist("label %04X %s\n", oldlc, name);
        putlist("patch ");                  /* write "patch"                     */
      }
      else if (*curline->txt)
        putlist(blnkfil);
    #if 0
      else
        putlist("comment %04X", oldlc);
    #endif
    }
    else if ((warning & WRN_OPT) &&         /* excessive branch, TSC style       */
        (dwOptions & OPTION_TSC) &&         
        (dwOptions & OPTION_WAR))
      putlist(">");
    else if (LINE_IS_MACDEF(curline->flg))  /* if in macro definition            */
      putlist("#");                         /* prefix line with #                */
    else if (LINE_IS_MACEXP(curline->flg))  /* if in macro expansion             */
      putlist("+");                         /* prefix line with +                */
    else if (LINE_IS_MACINV(curline->flg))  /* if in macro invocation            */
      putlist("&");                         /* prefix line with &                */
    else if (LINE_IS_INVISIBLE(curline->flg))
      putlist("-");
    else if (LINE_IS_PEMTCMT(curline->flg))
      putlist("-");
    else if (*curline->txt)                 /* otherwise                         */
      putlist(" ");                         /* prefix line with blank            */

    if (codeptr > 0 || printovr & PRINTOV_PADDR)
      putlist("%04X ", (unsigned short)(oldlc + ((dwOptions & OPTION_LPA) ? 0 : phase)));
    else if (printovr & PRINTOV_PLABEL)
      putlist("%04X ", (unsigned short)lpLabel->value);
    else if (printovr & PRINTOV_PDP && dpsetting >= 0)
      putlist("%02X   ", (unsigned short)dpsetting);
    else if (printovr & PRINTOV_PTFR)
      putlist("%04X ", tfradr);
    else if (printovr & PRINTOV_PPHA)
      putlist("%04X ", (unsigned short)phase);
    else if (*curline->txt)
      putlist("     ");
    else
    {
      putlist( "\n");
      return;
    }

    for (i = 0; i < codeptr && i < MAXLISTBYTES; i++)
    {
      if (dwOptions & OPTION_LPA)
        putlist("%02X ", codebuf[i]);
      else
        putlist("%02X", codebuf[i]);
    }
    for (; i <= MAXLISTBYTES; i++)
    {
      if (dwOptions & OPTION_LPA)
        putlist("   ");
      else
        putlist("  ");
    }

    if ((dwOptions & OPTION_LPA) &&
        (*curline->txt))
      putlist("* ");

    if (strcmp(curline->txt, srcline) &&    /* if text inserted                  */
        (dwOptions & OPTION_EXP))           /* and expansion activated           */
      putlist("%s", curline->txt);          /* just print out the source line    */
    else                                    /* otherwise                         */
      putlist("%s", srcline);               /* print possibly expanded line      */

    putlist("\n");                          /* send newline                      */

    /* allow to create [l]comments by using a double comment character;
       commentchar+@ on start of line creates an insert instead of a comment */
    if ((dwOptions & OPTION_LPA) &&         /* patch output - check for comments */
        (*curline->txt))
    {
    #if 1
      /* work on the already expanded text */
      const char *srcptr = srcline;
    #else
      /* work on the original text */
      const char *srcptr = curline->txt;
    #endif
      char c, iscchr, incomment = 0, nonblnk = 0, add_after = 0;
      int fnd = -1, bef = 0;
      for (i = 0; srcptr[i]; i++)
      {
        c = srcptr[i];
        iscchr = ((c == '*') ||
                  ((dwOptions & OPTION_GAS) && (c == '|')) ||
                  (c == ';'));
        if (!incomment &&
            iscchr &&
            (c == srcptr[i + 1] || 
             (srcptr[i + 1] == '@' && !nonblnk)))
        {
          c = srcptr[++i];
          if (srcptr[i + 1] == '+')
            {
            ++i;
            add_after = 1;
            }
          fnd = ++i;
          if (isspace(srcptr[fnd]))
            fnd++;
          break;
        }
        else if (isspace(c))
        {
          if (!nonblnk)
            bef++;
        }
        else
        {
          if (iscchr)
            incomment = 1;
          nonblnk = 1;
        }
      }
      if (fnd > 0 && (srcptr[fnd] || !(nonblnk || bef > 2)))
      {
        if (nonblnk || bef > 2)
          putlist("lcomment %s%04X ", add_after ? "after " : "", oldlc);
        else if (c == '@')
          putlist("insert %s%04X ", add_after ? "after " : "", oldlc);
        else
          putlist("comment %s%04X ", add_after ? "after " : "", oldlc);
        if (isspace(srcptr[fnd]))
          putlist("\\");
        pemt_putlist(srcptr, fnd);
      }
    }

    if (codeptr > MAXLISTBYTES &&           /* if there are additional bytes,    */
        (dwOptions & (OPTION_LPA | OPTION_MUL)))
    {                                     /* print them.                       */
      if (dwOptions & OPTION_LPA)           /* if in patch mode                  */
        putlist("patch");                   /* write "patch"                     */
      for (i = MAXLISTBYTES; i < codeptr; i++)
      {
        if (!(i % MAXLISTBYTES))
        {
          if (i != MAXLISTBYTES)
          {
            putlist("\n");
            if (dwOptions & OPTION_LPA &&   /* if in patch mode                  */
                i < codeptr)                /* and bytes follow                  */
              putlist("patch");             /* write "patch"                     */
          }
          if (dwOptions & OPTION_NUM)       /* if number output                  */
            putlist("     ");
          if (!absmode)
            putlist(" ");
          putlist(" %04X ",
                  (unsigned short)(oldlc +
                                   ((dwOptions & OPTION_LPA) ? 0 : phase) +
                                   i));
        }
        if (dwOptions & OPTION_LPA)         /* if in patch mode                  */
          putlist("%02X ", codebuf[i]);
        else
          putlist("%02X", codebuf[i]);
      }
      putlist("\n");
    }

    if (strcmp(curline->txt, srcline) &&    /* if text inserted                  */
        (dwOptions & OPTION_EXP))           /* and expansion activated           */
    {
      if (dwOptions & OPTION_NUM)
        putlist("%4d ", curline->ln);
      if (!absmode)
        putlist(" ");
      putlist("+                   ( %s )\n", srcline);
    }
}

/*****************************************************************************/
/* setlabel : sets a label                                                   */
/*****************************************************************************/

void setlabel(struct symrecord * lp)
{
    if (lp)
    {
      if (lp->cat == SYMCAT_PUBLICUNDEF)
      {
        lp->cat = SYMCAT_PUBLIC;
        lp->value = (unsigned short)(loccounter + phase);
      }
      else if (lp->cat == SYMCAT_EMPTYLOCAL)
      {
        lp->cat = SYMCAT_LOCALLABEL;
      }
      else if (lp->cat == SYMCAT_LOCALLABEL)
        ;
      else if (lp->cat != SYMCAT_EMPTY && lp->cat != SYMCAT_UNRESOLVED)
      {
        if (dwOptions & OPTION_RED)
          {
          /* if existing constant value equals the current location, let it through */
          if (lp->cat == SYMCAT_CONSTANT &&
              lp->value == (unsigned short)(loccounter + phase))
            lp->cat = (global) ? SYMCAT_PUBLIC : SYMCAT_LABEL;
          }
        if ((lp->cat != SYMCAT_LABEL && lp->cat != SYMCAT_PUBLIC) ||
            lp->value != (unsigned short)(loccounter + phase))
          error |= ERR_LABEL_MULT;
      }
      else
      {
        lp->cat = (global) ? SYMCAT_PUBLIC : SYMCAT_LABEL;
        lp->value = (unsigned short)(loccounter + phase);
      } 
    } 
}

/*****************************************************************************/
/* putbyte : adds a byte to the instruction code buffer                      */
/*****************************************************************************/

void putbyte(unsigned char b)
{
    if (codeptr < sizeof(codebuf))
      codebuf[codeptr] = b;
    codeptr++;
}

/*****************************************************************************/
/* putword : adds a word to the instruction code buffer                      */
/*****************************************************************************/

void putword(unsigned short w)
{
    putbyte((unsigned char)(w >> 8));
    putbyte((unsigned char)(w & 0xff));
}

/*****************************************************************************/
/* putdword : adds a doubleword to the instruction code buffer               */
/*****************************************************************************/

void putdword(unsigned long d)
{
    putbyte((unsigned char)((d >> 24) & 0xff));
    putbyte((unsigned char)((d >> 16) & 0xff));
    putbyte((unsigned char)((d >>  8) & 0xff));
    putbyte((unsigned char)(d & 0xff));
}

/*****************************************************************************/
/* doaddress : assemble the right addressing bytes for an instruction        */
/*****************************************************************************/

void doaddress(struct relocrecord *p)
{
    int offs;
    int addrelocation = 0;
          
    if (p)                                  /* create relocation record          */
      p->addr = (unsigned short)(loccounter + codeptr);

    switch (mode)
    {
      case ADRMODE_IMM :
        if (opsize == 2)
        {
          putbyte((unsigned char)operand);
          addrelocation = 1;
          p->exprcat |= EXPRCAT_ONEBYTE;
        }
        else if (opsize == 5)               /* LDQ special                       */
          putdword(operand);
        else
        {
          putword((unsigned short)operand);
          addrelocation = 1;
        }
        break; 
      case ADRMODE_DIR :
        putbyte((unsigned char)operand);
        break;
      case ADRMODE_EXT :
        if (((codebuf[0] == 0x7e) ||        /* special for JMP                   */
             (codebuf[0] == 0xbd)) &&       /* and JSR                           */
            pass > 1)
        {
          int nDiff = (int)(short)operand - (int)(short)(loccounter + phase) - 3;
          if (((nDiff & 0xff80) == 0x0000) ||
               ((nDiff & 0xff80) == 0xff80))
               warning |= (certain) ? WRN_OPT : 0;
        }
        putword((unsigned short)operand);
        addrelocation = 1;
        break;
      case ADRMODE_IDX :
        putbyte((unsigned char)operand);
        break;
      case ADRMODE_POST :
      case ADRMODE_IND :
        putbyte(postbyte);
        switch (opsize)
        {
          case 2:
            putbyte((unsigned char)operand);
            break;
          case 3:
            putword((unsigned short)operand); 
            addrelocation = 1;
        }
        break;
      case ADRMODE_PCR :
      case ADRMODE_PIN :
        offs = (unsigned short)operand - (loccounter + phase) - codeptr - 2;
        if (offs < -128 || offs >= 128 || opsize == 3 || unknown || !certain)
        {
          // all unknowns with opsize 0 (w/o < or > prefix) are treated as 16-bit
          if (opsize == 0 || opsize == 3)  
          {
            offs--;
            opsize = 3;
            postbyte++;
            p->addr++;
          }
          if (!unknown)
          {
            // for Motorola as9 compatibility - overflows are captured
            // and wrapped around if possible
            if (opsize == 3)
            {
              if (offs < -32768)
              {
                offs += 0x10000;
                warning |= WRN_REL_WRAP;
              }
              else if (offs > 32767)
              {
                offs -= 0x10000;
                warning |= WRN_REL_WRAP;
              }
            }
            if ((opsize == 2 && (offs < -128 || offs >= 128)) ||
                (opsize == 3 && (offs < -32768 || offs >= 32768)))
              error |= ERR_RANGE;
          }
        }
        putbyte(postbyte);
        if (opsize == 3)
        {
          putword((unsigned short)offs); 
          if (mode == ADRMODE_PIN ||
              p->exprcat & EXPRCAT_EXTADDR)
            addrelocation = 1;
        }
        else
          putbyte((unsigned char)offs);   
    }     

    if (addrelocation)
      addreloc(p);
}

/*****************************************************************************/
/* onebyte : saves integer as one instruction byte                           */
/*****************************************************************************/

void onebyte(int co)
{
    putbyte((unsigned char)co);
}

/*****************************************************************************/
/* twobyte : saves integer as two instruction bytes                          */
/*****************************************************************************/

void twobyte(int co)
{
    putword((unsigned short)co);
}

/*****************************************************************************/
/* threebyte : saves long integer as three instruction bytes                 */
/*****************************************************************************/

void threebyte(unsigned long co)
{
    putbyte((unsigned char)((co >> 16) & 0xff));
    putbyte((unsigned char)((co >> 8) & 0xff));
    putbyte((unsigned char)(co & 0xff));
}

/*****************************************************************************/
/* fourbyte : saves long integer as four instruction bytes                   */
/*****************************************************************************/

void fourbyte(unsigned long co)
{
    putbyte((unsigned char)((co >> 24) & 0xff));
    putbyte((unsigned char)((co >> 16) & 0xff));
    putbyte((unsigned char)((co >> 8) & 0xff));
    putbyte((unsigned char)(co & 0xff));
}

/*****************************************************************************/
/* oneimm : saves one immediate value                                        */
/*****************************************************************************/

void oneimm(int co)
{
    struct relocrecord p = {0};

    scanoperands(&p, 0);
    if (mode >= ADRMODE_POST)
      error |= ERR_ILLEGAL_ADDR;
    putbyte((unsigned char)co);

    /* addreloc(0, 2, p); */                /* no relocation for immediate op's  */

    putbyte((unsigned char)operand);
}

/*****************************************************************************/
/* lea :                                                                     */
/*****************************************************************************/

void lea(int co)
{
    struct relocrecord p = {0};

    onebyte((unsigned char)co);
    scanoperands(&p, 0);
    if (mode == ADRMODE_IMM)
      error |= ERR_ILLEGAL_ADDR;
    if (mode < ADRMODE_POST)
    {
      opsize = 3;
      postbyte = 0x8f;
      mode = ADRMODE_POST;
    }
    doaddress(&p);
}

/*****************************************************************************/
/* sbranch : processes a short branch                                        */
/*****************************************************************************/

void sbranch(int co)
{
    struct relocrecord p = {0};
    int offs;

    scanoperands(&p, 0);
    if (mode != ADRMODE_DIR && mode != ADRMODE_EXT)
      error |= ERR_ILLEGAL_ADDR;
    offs = (unsigned short)operand - (loccounter + phase) - 2;
    if (!unknown && (offs < -128 || offs >= 128))
      error |= ERR_RANGE;
    if (pass == 2 && unknown)
      error |= ERR_LABEL_UNDEF;
    putbyte((unsigned char)co);
    putbyte((unsigned char)offs);
}

/*****************************************************************************/
/* lbra : does a long branch                                                 */
/*****************************************************************************/

void lbra(int co)
{
    struct relocrecord p = {0};
    int nDiff;

    scanoperands(&p, 0);
    if (mode != ADRMODE_DIR && mode != ADRMODE_EXT)
      error |= ERR_ILLEGAL_ADDR;
    putbyte((unsigned char)co);

    nDiff = operand - (loccounter + phase) - 3;

    if (p.exprcat & EXPRCAT_EXTADDR)
    {
      p.exprcat |= EXPRCAT_EXTOFF;
      doaddress(&p);
    }
    else
    {
      putword((unsigned short)nDiff);
      if (((nDiff & 0xff80) == 0x0000) ||
          ((nDiff & 0xff80) == 0xff80))
        warning |= (certain) ? WRN_OPT : 0;
    }
}

/*****************************************************************************/
/* lbranch : does a long branch                                              */
/*****************************************************************************/

void lbranch(int co)
{
    struct relocrecord p = {0};
    int nDiff;

    scanoperands(&p, 0);
    if (mode != ADRMODE_DIR && mode != ADRMODE_EXT)
      error |= ERR_ILLEGAL_ADDR;
    putword((unsigned short)co);
    nDiff = operand - (loccounter + phase) - 4;

    if (p.exprcat & EXPRCAT_EXTADDR)
    {
      p.exprcat |= EXPRCAT_EXTOFF;
      doaddress(&p);
    }
    else
    {
      putword((unsigned short)nDiff);
      if (((nDiff & 0xff80) == 0x0000) ||
          ((nDiff & 0xff80) == 0xff80))
        warning |= (certain) ? WRN_OPT : 0;
    }
}

/*****************************************************************************/
/* arith : process arithmetic operation                                      */
/*****************************************************************************/

void arith(int co, char noimm)
{
    struct relocrecord p = {0};

    scanoperands(&p, 0);
    switch (mode)
    {
      case ADRMODE_IMM :
        if (noimm)
          error |= ERR_ILLEGAL_ADDR;
        opsize = 2;
        putbyte((unsigned char)co);
        break;
      case ADRMODE_DIR :
        putbyte((unsigned char)(co + 0x010));
        break;
      case ADRMODE_EXT :
        putbyte((unsigned char)(co + 0x030));
        break;
      default:
        putbyte((unsigned char)(co + 0x020));
    }
    doaddress(&p);
}

/*****************************************************************************/
/* accarith : process arithmetic operation with explicit accumulator         */
/*****************************************************************************/

void accarith(int co, char noimm, char ignore)
{
    char *s = srcptr;                       /* remember current offset           */
    char expacc = 1;                        /* flag whether explicit acc.        */
    char ambig = 0;

    /* this is supposed to catch 6800 legacy expressions of the form
         LDA A operands (i.e., a 6809 LDA mnemonic)
         LDA B operands (i.e., a 6809 LDB mnemonic)
       but it should NOT catch
         LDA A,X
         LDA B,X
       which are valid 6809 operations! */

    skipspace();                            /* skip space                        */
    scanname();                             /* get following name                */

    if (strcmp(unamebuf, "A") &&            /* has to be followed by A or B      */
        strcmp(unamebuf, "B"))
      expacc = 0;

    #if 1
    if (expacc && !skipspace())
      expacc = 0;
    if (expacc && *srcptr == ',' && !(dwOptions & OPTION_TSC))
      ambig = 1;
    #else
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr++ != ',')                   /* and a comma                       */
      expacc = 0;
    #endif

    if (!expacc)                            /* if NOT followed by "A " or "B "   */
    {
    #if 1
      srcptr = s;                           /* go back to parm start             */
    #else
      if (ignore)                           /* if ignoring that,                 */
        srcptr = s;                         /* go back to parm start             */
      else                                  /* otherwise                         */
        error |= ERR_EXPR;                  /* flag as an error                  */
    #endif
    }
    else
    {
      if (unamebuf[0] == 'B')               /* eventually transform to acc.B     */
        co |= 0x40;
      if (ambig)
        warning |= WRN_AMBIG;
    }
    arith(co, noimm);                       /* then process as arithmetic        */
}

/*****************************************************************************/
/* idxextarith : indexed / extended arithmetic, 6800 style                   */
/*****************************************************************************/

void idxextarith(int co)
{
    struct relocrecord p = {0};

    scanoperands(&p, 0);
    switch (mode)
    {
      case ADRMODE_IMM :
        error |= ERR_ILLEGAL_ADDR;
        opsize = 3;
        putbyte((unsigned char)co);
        break;
      case ADRMODE_DIR :
        mode = ADRMODE_EXT;                 /* implicitly convert to extended    */
        /* fall thru to... */
      case ADRMODE_EXT :
        putbyte((unsigned char)(co + 0x030));
        break;
      default:
        putbyte((unsigned char)(co + 0x020));
        break;
    }
    doaddress(&p);
}

/*****************************************************************************/
/* darith : process direct arithmetic                                        */
/*****************************************************************************/

void darith(int co, char noimm)
{
    struct relocrecord p = {0};

    scanoperands(&p, 0);
    switch (mode)
    {
      case ADRMODE_IMM :
        if (noimm)
          error |= ERR_ILLEGAL_ADDR;
        opsize = 3;
        putbyte((unsigned char)co);
        break;
      case ADRMODE_DIR :
        putbyte((unsigned char)(co + 0x010));
        break;
      case ADRMODE_EXT :
        putbyte((unsigned char)(co + 0x030));
        break;
      default:
        putbyte((unsigned char)(co + 0x020));
        if (dwOptions & OPTION_H11)         /* specials for 68HC11 page bytes    */
        {
          if ((codebuf[0] == 0x18) &&
              (co == 0x8c ||                /* CPX                               */
               co == 0xce ||                /* LDX                               */
               co == 0xcf))                 /* STX                               */
            codebuf[0] = 0xcd;
        }
        break;
   }
   doaddress(&p);
}

void darith18(int co, char noimm)
{
    codebuf[codeptr++] = 0x18;
    darith(co, noimm);
    if (codebuf[0] == 0xcd)
      codebuf[0] = 0x18;
}

void darith1a(int co, char noimm)
{
    codebuf[codeptr++] = 0x1a;
    darith(co, noimm);
}

/*****************************************************************************/
/* d2arith : process direct word arithmetic                                  */
/*****************************************************************************/

void d2arith(int co, char noimm)
{
    struct relocrecord p = {0};

    scanoperands(&p, 0);
    switch (mode)
    {
      case ADRMODE_IMM :
        if (noimm)
          error |= ERR_ILLEGAL_ADDR;
        opsize = 3;
        putword((unsigned short)co);
        break;
      case ADRMODE_DIR :
        putword((unsigned short)(co + 0x010));
        break;
      case ADRMODE_EXT :
        putword((unsigned short)(co + 0x030));
        break;
      default:
        putword((unsigned short)(co + 0x020));
   }
   doaddress(&p);
}

/*****************************************************************************/
/* qarith : process direct doubleword arithmetic                             */
/*****************************************************************************/

void qarith(int co, char noimm)
{
    struct relocrecord p = {0};

    scanoperands(&p, 0);
    switch (mode)
    {
      case ADRMODE_IMM :
        if (noimm)
          error |= ERR_ILLEGAL_ADDR;
        opsize = 5;
        putbyte((unsigned char)0xcd);       /* this can ONLY be LDQ!             */
        break;
      case ADRMODE_DIR :
        putword((unsigned short)(co + 0x010));
        break;
      case ADRMODE_EXT :
        putword((unsigned short)(co + 0x030));
        break;
      default:
        putword((unsigned short)(co + 0x020));
        break;
    }
    doaddress(&p);
}

/*****************************************************************************/
/* oneaddr :                                                                 */
/*****************************************************************************/

void oneaddr(int co)
{
    struct relocrecord p = {0};
    char *s;

    skipspace();
    #if 1
    if (co != 0x0e)                         /* check for 6800 convenience things */
    #else
    if ((dwOptions & OPTION_TSC) &&         /* if TSC mode, check for 6800       */
        (co != 0x0e))                       /* convenience things                */
    #endif
    {
      s = srcptr;
      scanname();                           /* look whether followed by A or B   */
      if (((!strcmp(unamebuf, "A")) ||
           (!strcmp(unamebuf, "B"))) &&
          (*srcptr != ','))                 /* and NO comma                      */
        {                                   /* if so, replace by 6809 mnemonic   */
        warning |= WRN_AMBIG;               /* ... but not without a warning     */
        onebyte(co | ((unamebuf[0] == 'A') ? 0x40 : 0x50));
        return;
        }
      srcptr = s;
    }

    scanoperands(&p, 0);
    switch (mode)
    {
      case ADRMODE_IMM :
        error |= ERR_ILLEGAL_ADDR;
        break;
      case ADRMODE_DIR :
        if ((dwOptions & OPTION_M00) &&     /* on MC6800, a DIRect JMP is not OK */
            (co == 0x0e))
          error |= ERR_ILLEGAL_ADDR;
        else
          putbyte((unsigned char)co);
        break;
      case ADRMODE_EXT :
        putbyte((unsigned char)(co + 0x70));
        break;
      default:
        putbyte((unsigned char)(co + 0x60));
        break;
    }

    if (p.exprcat & EXPRCAT_EXTADDR &&
        mode == ADRMODE_PCR)
      p.exprcat |= EXPRCAT_EXTOFF;

    doaddress(&p);
}

/*****************************************************************************/
/* accaddr :                                                                 */
/*****************************************************************************/

void accaddr(int co)
{
    struct relocrecord p = {0};
    char *s;

    skipspace();
    #if 1
                                            /* always check for 6800             */
    #else
    if (dwOptions & OPTION_TSC)             /* if TSC mode, check for 6800       */
    #endif
    {
      s = srcptr;
      scanname();                           /* look whether followed by A or B   */
      if (((!strcmp(unamebuf, "A")) ||
           (!strcmp(unamebuf, "B"))) &&
          (*srcptr != ','))                 /* and NO comma                      */
        {                                   /* if so, replace                    */
        onebyte(co | ((unamebuf[0] == 'A') ? 0x40 : 0x50));
        return;
        }
      srcptr = s;
    }

    scanoperands(&p, 0);
    switch (mode)
    {
      case ADRMODE_IMM :
        error |= ERR_ILLEGAL_ADDR;
        break;
      case ADRMODE_DIR :
        mode = ADRMODE_EXT;                 /* silently convert to extended      */
        /* fall through to ... */
      case ADRMODE_EXT :
        putbyte((unsigned char)(co + 0x70));
        break;
      default:
        putbyte((unsigned char)(co + 0x60));
        break;
    }
    doaddress(&p);
}

/*****************************************************************************/
/* tfrexg :                                                                  */
/*****************************************************************************/

void tfrexg(int co)
{
    struct regrecord * p;

    putbyte((unsigned char)co);
    skipspace();
    scanname();

    if (dwOptions & OPTION_H09)
      p = findreg63(unamebuf);
    else
      p = findreg(unamebuf);
    if (!p)
      error |= ERR_ILLEGAL_ADDR;
    else
      postbyte = (p->tfr) << 4;
    skipspace();
    if (*srcptr == ',')
      srcptr++;
    else
      error |= ERR_ILLEGAL_ADDR;
    skipspace();
    scanname();
    if ((p = findreg(unamebuf)) == 0)
      error |= ERR_ILLEGAL_ADDR;
    else
      postbyte |= p->tfr;
    putbyte(postbyte);
}

/*****************************************************************************/
/* pshpul : operates on PSH / PUL mnemonics                                  */
/*****************************************************************************/

void pshpul(int co, int mode)
{
    struct regrecord *p;
    struct relocrecord sp = {0};
    postbyte = 0;

    skipspace();

    if (mode == 1)                          /* 6800/6801/6811 mode:              */
    {
      scanname(); 
      if ((p = findreg(unamebuf)) == 0)
        error |= ERR_ILLEGAL_ADDR;
      else
      {
        postbyte |= p->psh;
        if (!postbyte)
          error |= ERR_ILLEGAL_ADDR;
      }
      co |= postbyte & 0x7f;
      if (postbyte & 0x80)
        putbyte((unsigned char)0x18);
    }

    putbyte((unsigned char)co);

    if (mode == 0)                          /* 6809/6309 mode:                   */
    {
      if (*srcptr == '#')
      {
        srcptr++;
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        postbyte = (unsigned char)scanexpr(0, &sp);
      }
      else do
      {
        if (*srcptr == ',')
          srcptr++;
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        scanname(); 
        if ((p = findreg(unamebuf)) == 0)
          error |= ERR_ILLEGAL_ADDR;
        else
          postbyte |= p->psh; 
        if (!(dwOptions & OPTION_TSC))
          skipspace();
      } while (*srcptr == ',');
      putbyte(postbyte);
    }
}

/*****************************************************************************/
/* bitdirect :                                                               */
/*****************************************************************************/

void bitdirect(int co)
{
    struct relocrecord p = {0};
    unsigned short dir;

    skipspace();
    if (*srcptr++ != '#')
      error |= ERR_EXPR;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    dir = (unsigned short)scanexpr(0, &p);
    /* check for overflow - is this necessary? Don't know ... */
    if (dir > 0xff && dir < 0xff00)
      warning |= WRN_IMM_TRUNC;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr++ != ',')
      error |= ERR_EXPR;
    scanoperands(&p, 0);

    /* HD6301/6303 can handle Immediate and Indexed addressing, whereas
       HD6309 can handle Immediate, Extended and Indexed addressing */
    switch (mode)
    {
      case ADRMODE_DIR :
        if (dwOptions & OPTION_H01)
          putbyte((unsigned char)(co + 0x70));
        else
          putbyte((unsigned char)co);
        break;
      case ADRMODE_EXT :
        if (dwOptions & OPTION_H01)
          error |= ERR_ILLEGAL_ADDR;
        else
          putbyte((unsigned char)(co + 0x70));
        break;
      case ADRMODE_POST:
      case ADRMODE_IDX:
        putbyte((unsigned char)(co + 0x60));
        break;
      default :
        error |= ERR_ILLEGAL_ADDR;
        break;
    }
    putbyte((unsigned char)dir);
    doaddress(&p);
}

/*****************************************************************************/
/* bittrans :                                                                */
/*****************************************************************************/

void bittrans(int co)
{
    struct regrecord *p;
    struct relocrecord rp = {0};
    long t;

    putword((unsigned short)co);

    skipspace();
    scanname();
    if ((p = findbitreg(unamebuf)) == 0)
      error |= ERR_ILLEGAL_ADDR;
    else
      postbyte = (p->tfr) << 6;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr == ',')
      srcptr++;
    else
     error |= ERR_ILLEGAL_ADDR;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    t = scanfactor(&rp);
    if (t & 0xfff8)
      error |= ERR_ILLEGAL_ADDR;
    else
      postbyte |= (t << 3);
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr == ',')
      srcptr++;
    else
     error |= ERR_ILLEGAL_ADDR;
    t = scanfactor(&rp);
    if (t & 0xfff8)
      error |= ERR_ILLEGAL_ADDR;
    else
      postbyte |= t;
    putbyte((unsigned char)postbyte);
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr == ',')
      srcptr++;
    else
     error |= ERR_ILLEGAL_ADDR;
    scanoperands(&rp, 0);
    switch (mode)
    {
      case ADRMODE_DIR :
        putbyte((unsigned char)operand);
        break;
      default:
        error |= ERR_ILLEGAL_ADDR;
    }
}

/*****************************************************************************/
/* blocktrans :                                                              */
/*****************************************************************************/

void blocktrans(int co)
{
    char reg1,reg2;
    char mode[3] = "";
    static char regnames[] = "DXYUS";
    static char *modes[] =
    {
      "++",
      "--",
      "+",
      ",+"
    };
    int i;

    skipspace();
    reg1 = toupper(*srcptr);
    for (i = 0; regnames[i]; i++)
      if (reg1 == regnames[i])
        break;
    if (!regnames[i])
      error |= ERR_ILLEGAL_ADDR;
    else
      reg1 = i;
    mode[0] = *++srcptr;
    if ((mode[0] != '+') && (mode[0] != '-'))
    {
      if (!(dwOptions & OPTION_TSC))
        skipspace();
      mode[0] = *srcptr;
    }
    else
      srcptr++;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr != ',') 
      error |= ERR_ILLEGAL_ADDR;
    srcptr++;
    reg2 = toupper(*srcptr);
    for (i = 0; regnames[i]; i++)
      if (reg2 == regnames[i])
        break;
    if (!regnames[i])
      error |= ERR_ILLEGAL_ADDR;
    else
      reg2 = i;
    mode[1] = *++srcptr;
    if ((mode[1] != '+') && (mode[1] != '-'))
    {
      if (!(dwOptions & OPTION_TSC))
        skipspace();
      mode[1] = *srcptr;
    }
    else
      srcptr++;
    if ((mode[1] == ';') || (mode[1] == '*') ||
        (mode[1] == ' ') || (mode[1] == '\t'))
      mode[1] = '\0';
    for (i = 0; i < (sizeof(modes) / sizeof(modes[0])); i++)
      if (!strcmp(mode, modes[i]))
        break;
    if (i >= (sizeof(modes) / sizeof(modes[0])))
      error |= ERR_ILLEGAL_ADDR;
    else
      co |= i;

    putword((unsigned short)co);
    putbyte((unsigned char)((reg1 << 4) | reg2));
}

/*****************************************************************************/
/* setmask :                                                                 */
/*****************************************************************************/

void setmask(int co)
{
    struct relocrecord p = {0};
    long mask = -1;

    /* expected:
       dir [#]mask               |  dir,[#]mask
       [[off],]indexreg [#]mask  |  [[off],]indexreg,[#]mask */

    scanoperands(&p, 1);                    /* get 1st part (dir/index)          */

    if (*srcptr == ',' ||                   /* either , or ' ' is valid          */
        *srcptr == ' ')
      srcptr++;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr == '#')
      srcptr++;
    mask = scanexpr(0, &p);
    if (mask < 0 || mask > 0xff)
      error |= ERR_EXPR;

    switch (mode)
    {
      case ADRMODE_IMM :
      case ADRMODE_EXT :
        error |= ERR_ILLEGAL_ADDR;
        break;
      case ADRMODE_DIR :
        putbyte((unsigned char)(co));
        break;
      default:
        putbyte((unsigned char)(co + 0x08));
        break;
    }
    doaddress(&p);
    putbyte((unsigned char)mask);
}

/*****************************************************************************/
/* brmask :                                                                  */
/*****************************************************************************/

void brmask(int co)
{
    struct relocrecord p = {0}, pj = {0};
    long mask = -1;
    int offs;

    /* expected:
       dir [#]mask target  |  dir,[#]mask,target
       [[off],]indexreg [#]mask target  |  [[off],]indexreg,[#]mask,target */

    scanoperands(&p, 1);

    if (*srcptr == ',' ||                   /* either , or ' ' is valid          */
        *srcptr == ' ')
      srcptr++;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    if (*srcptr == '#')
      srcptr++;
    mask = scanexpr(0, &p);
    if (mask < 0 || mask > 0xff)
      error |= ERR_EXPR;

    if (*srcptr == ',' ||                   /* either , or ' ' is valid          */
        *srcptr == ' ')
      srcptr++;
    if (!(dwOptions & OPTION_TSC))
      skipspace();
    {
        char s_mode = mode;
        char s_opsize = opsize;
        long s_operand = operand;
        char s_unknown = unknown;
        char s_certain = certain;
        unsigned char s_postbyte = postbyte;
        scanoperands(&pj, 0);
        if (mode != ADRMODE_DIR && mode != ADRMODE_EXT)
          error |= ERR_ILLEGAL_ADDR;
        offs = (unsigned short)operand - (loccounter + phase) - 4;
        if (codeptr) offs--;
        if (!unknown && (offs < -128 || offs >= 128))
          error |= ERR_RANGE;
        if (pass == 2 && unknown)
          error |= ERR_LABEL_UNDEF;
        mode = s_mode;
        opsize = s_opsize;
        operand = s_operand;
        postbyte = s_postbyte;
        unknown = s_unknown;
        certain = s_certain;
    }

    switch (mode)
    {
      case ADRMODE_IMM :
      case ADRMODE_EXT :
        error |= ERR_ILLEGAL_ADDR;
        break;
      case ADRMODE_DIR :
        putbyte((unsigned char)(co));
        break;
      default:
        putbyte((unsigned char)(co + 0x0c));
        break;
    }

    doaddress(&p);
    putbyte((unsigned char)mask);
    putbyte((unsigned char)offs);
}


/*****************************************************************************/
/* expandline : un-tabify current line                                       */
/*****************************************************************************/

void expandline() 
{
    int i, j = 0, k, j1;

    for (i = 0; i < LINELEN && j < LINELEN; i++)
    {
      if (inpline[i] == '\n')
      {
        srcline[j] = 0;
        break;
      }
      else if (inpline[i] == '\t')
      {
        j1 = j;
        for (k = 0; k < 8 - j1 % 8 && j < LINELEN; k++)
          srcline[j++] = ' ';
      }
      else if (inpline[i] == '\r')
      {
        continue;
      }
      else
        srcline[j++] = inpline[i];     
   }
   srcline[LINELEN - 1] = 0;
}

/*****************************************************************************/
/* expandtext : expands all texts in a line                                  */
/*****************************************************************************/

void expandtext()
{
    char *p;
    int i, j = 0;

    for (p = curline->txt; (*p) && (j < LINELEN); )
    {
      if (*p == '\\' && p[1] == '&')        /* when encountering \&,             */
      {
        srcline[j++] = *(++p);              /* swallow the \,                    */
        p++;                                /* keep and skip over the &          */
      }
      else if (*p == '&' && (p[1] < '0' || p[1] > '9'))
      {
        struct symrecord *lp;
        char iname[sizeof(namebuf)];
        srcptr = p + 1;
        scanname();
        sprintf(iname, "&%.*s", (int)(sizeof(iname) - 2), namebuf);
        lp = findsym(iname, 0);
        if (lp && *namebuf &&               /* if symbol IS a text constant,     */
            (lp->cat == SYMCAT_TEXT))
        {                                 /* insert its content                */
          char *from = texts[lp->value];
          for (i = 0; j < LINELEN && from[i]; i++)
            srcline[j++] = from[i];
          p = srcptr;
        }
        else                                /* otherwise                         */
          srcline[j++] = *p++;              /* simply use the '&' and go on.     */
      }
      else
        srcline[j++] = *p++;
    }
    srcline[j >= LINELEN ? LINELEN - 1 : j] = '\0';
}

/*****************************************************************************/
/* readfile : reads in a file and recurses through includes                  */
/*****************************************************************************/

struct linebuf *readfile(char *name, unsigned char lvl, struct linebuf *after)
{
    FILE *srcfile;
    struct linebuf *pNew;
    int lineno = 0;
    int i;
    int nfnidx = -1;

    for (i = 0; i < nfnms; i++)             /* prohibit recursion                */
      if (!strcmp(name, fnms[i]))
      {
        nfnidx = i;
        break;
      }
    if (nfnidx < 0)
    {
      if (nfnms >= (sizeof(fnms) / sizeof(fnms[0])))
        {
        printf("%s(0) : error 21: nesting level too deep\n", name);
        if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
          putlist( "*** Error 21: nesting level too deep\n");
        exit(4);
        }
      nfnidx = nfnms++;
    }

    if ((srcfile = fopen(name, "r")) == 0)
    {
      printf("%s(0) : error 17: cannot open source file\n", name);
      if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
        putlist( "*** Error 17: cannot open source file\n");
      exit(4);
    }
    if (!fnms[nfnidx])                      /* if not yet done,                  */
      fnms[nfnidx] = strdup(name);          /* remember the file name            */
    while (fgets(inpline, LINELEN, srcfile))
    {
      expandline();
      pNew = allocline(after, fnms[nfnidx], ++lineno, lvl, srcline);
      if (!pNew)
      {
        printf("%s(%d) : error 22: memory allocation error\n", name, lineno);
        if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
          putlist( "*** Error 22: memory allocation error\n");
        exit(4);
      }
      if (!after)                           /* if 1st line                       */
        rootline = pNew;                    /* remember it as root               */
      after = pNew;                         /* insert behind the new line        */
    }
    fclose(srcfile);                        /* then close the file               */
    return after;                           /* pass back last line inserted      */
}

/*****************************************************************************/
/* readbinary: reads in a binary file and converts to fcw / fcb lines        */
/*****************************************************************************/

struct linebuf *readbinary
    (
    char *name,
    unsigned char lvl,
    struct linebuf *after,
    struct symrecord *lp
    )
{
    FILE *srcfile;
    struct linebuf *pNew;
    int lineno = 0;
    int i;
    int nfnidx = -1;
    unsigned char binlin[16];
    int binlen;
    int fcbstart;

    for (i = 0; i < nfnms; i++)             /* prohibit recursion                */
      if (!strcmp(name, fnms[i]))
      {
        nfnidx = i;
        break;
      }
    if (nfnidx < 0)
    {
      if (nfnms >= (sizeof(fnms) / sizeof(fnms[0])))
      {
        printf("%s(0) : error 21: nesting level too deep\n", name);
        if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
          putlist( "*** Error 21: nesting level too deep\n");
        exit(4);
      }
      nfnidx = nfnms++;
      }

    if ((srcfile = fopen(name, "rb")) == 0)
    {
      printf("%s(0) : error 17: cannot open source file\n", name);
      if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
        putlist( "*** Error 17: cannot open source file\n");
      exit(4);
    }
    if (!fnms[nfnidx])                      /* if not yet done,                  */
      fnms[nfnidx] = strdup(name);          /* remember the file name            */
    while ((binlen = (int)fread(binlin, 1, sizeof(binlin), srcfile)) > 0)
    {
      sprintf(inpline, "%s", (lineno || (!lp)) ? "" : lp->name);
      i = 0;
      if ((binlen & (~1)) && (binlen > 7))
      {
        sprintf(inpline + strlen(inpline), "\tFCW\t" /* , (binlen & 1) ? 'B' : 'W' */);
        for (; i < (binlen & ~1); i += 2)
          sprintf(inpline + strlen(inpline), "%s$%02X%02X", (i) ? "," : "",
                  binlin[i], binlin[i + 1]);
        expandline();
        pNew = allocline(after, fnms[nfnidx], ++lineno, lvl, srcline);
        if (!pNew)
        {
          printf("%s(%d) : error 22: memory allocation error\n", name, lineno);
          if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
            putlist( "*** Error 22: memory allocation error\n");
          exit(4);
        }
        if (!after)                         /* if 1st line                       */
          rootline = pNew;                  /* remember it as root               */
        after = pNew;                       /* insert behind the new line        */
        inpline[0] = '\0';                  /* reset input line                  */
      }
      if ((binlen & 1) || (binlen <= 7))
      {
        fcbstart = i;
        sprintf(inpline + strlen(inpline), "\tFCB\t");
        for (; i < binlen; i++)
          sprintf(inpline + strlen(inpline), "%s$%02X", (i > fcbstart) ? "," : "",
                  binlin[i]);

        expandline();
        pNew = allocline(after, fnms[nfnidx], ++lineno, lvl, srcline);
        if (!pNew)
        {
          printf("%s(%d) : error 22: memory allocation error\n", name, lineno);
          if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
            putlist( "*** Error 22: memory allocation error\n");
          exit(4);
        }
        if (!after)                         /* if 1st line                       */
          rootline = pNew;                  /* remember it as root               */
        after = pNew;                       /* insert behind the new line        */
      }
    }
    fclose(srcfile);                        /* then close the file               */
    return after;                           /* pass back last line inserted      */
}

/*****************************************************************************/
/* setoptiontexts : sets up the option text variables                        */
/*****************************************************************************/

void setoptiontexts()
{
    int i;
                                            /* walk option list                  */
    for (i = 0; i < (sizeof(Options) / sizeof(Options[0])); i++)
    {
      if ((dwOptions & Options[i].dwAdd) ||
          (!(dwOptions & ~Options[i].dwRem)))
        settext(Options[i].Name, "1");
      else
        settext(Options[i].Name, "0");
    }
}

/*****************************************************************************/
/* setoption : processes an option string                                    */
/*****************************************************************************/

int setoption ( char *szOpt )
{
    char iopt[4];
    int i;

    for (i = 0; szOpt[i] && i < sizeof(iopt); i++)
      iopt[i] = toupper(szOpt[i]);
    if (i >= sizeof(iopt))
      i--;
    iopt[i] = '\0';
                                            /* search option list                */
    for (i = 0; i < (sizeof(Options) / sizeof(Options[0])); i++)
    {
      if (!strcmp(iopt, Options[i].Name))   /* if option found                   */
      {
        dwOptions |= Options[i].dwAdd;      /* add flags                         */
        dwOptions &= ~Options[i].dwRem;     /* and remove flags                  */

        switch (Options[i].dwAdd)           /* afterprocessing for specials:     */
        {
          case OPTION_M09 :                 /* switch to MC6809 processor        */
            optable = optable09;
            optablesize = sizeof(optable09) / sizeof(optable09[0]);
            regtable = regtable09;
            bitregtable = bitregtable09;
            bitregtablesize = sizeof(bitregtable09) / sizeof(bitregtable09[0]);
            scanoperands = scanoperands09;
            break;
          case OPTION_H09 :                 /* switch to HD6309 processor        */
            optable = optable09;
            optablesize = sizeof(optable09) / sizeof(optable09[0]);
            regtable = regtable63;
            bitregtable = bitregtable09;
            bitregtablesize = sizeof(bitregtable09) / sizeof(bitregtable09[0]);
            scanoperands = scanoperands09;
            break;
          case OPTION_M00 :                 /* switch to MC6800 processor        */
            optable = optable00;
            optablesize = sizeof(optable00) / sizeof(optable00[0]);
            regtable = regtable00;
            bitregtable = bitregtable00;
            bitregtablesize = sizeof(bitregtable00) / sizeof(bitregtable00[0]);
            scanoperands = scanoperands00;
            break;
          case OPTION_M01 :                 /* switch to MC6801 processor        */
            optable = optable01;
            optablesize = sizeof(optable01) / sizeof(optable01[0]);
            regtable = regtable01;
            bitregtable = bitregtable00;
            bitregtablesize = sizeof(bitregtable00) / sizeof(bitregtable00[0]);
            scanoperands = scanoperands00;
            break;
          case OPTION_H01 :                 /* switch to HD6301 processor        */
            optable = optable01;
            optablesize = sizeof(optable01) / sizeof(optable01[0]);
            regtable = regtable01;
            bitregtable = bitregtable00;
            bitregtablesize = sizeof(bitregtable00) / sizeof(bitregtable00[0]);
            scanoperands = scanoperands00;
            break;
          case OPTION_H11 :                 /* switch to 68HC11 processor        */
            optable = optable11;
            optablesize = sizeof(optable11) / sizeof(optable11[0]);
            regtable = regtable11;
            bitregtable = bitregtable00;
            bitregtablesize = sizeof(bitregtable00) / sizeof(bitregtable00[0]);
            scanoperands = scanoperands11;
            break;
        }

        setoptiontexts();
        return 0;                           /* then return OK                    */
      }
    }

    return 1;                               /* unknown option                    */
}

/*****************************************************************************/
/* pseudoop : processes all known pseudo-ops                                 */
/*****************************************************************************/

void pseudoop(int co, struct symrecord * lp)
{
    int i, j;
    char c, fillc;
    struct relocrecord p = {0};

    if (common &&                           /* if in COMMON definition mode      */
        (co != PSEUDO_ENDCOM) &&            /* and this is neither ENDCOM        */
        (co != PSEUDO_RMB))                 /* nor RMB                           */
      error |= ERR_EXPR;                    /* this is an error.                 */

    switch (co)
    {
      case PSEUDO_ABS :                     /* ABS                               */
        absmode = 1;                        /* reset mode to absolute            */
        break;
      case PSEUDO_DEF :                     /* DEFINE                            */
        global++;                           /* all labels from now on are global */
        break;
      case PSEUDO_ENDDEF :                  /* ENDDEF                            */
        if (global)                         /* all labels from now on are local  */
          global--;
        else
          error |= ERR_EXPR;                /* must be paired                    */
        break;
      case PSEUDO_COMMON :                  /* label COMMON                      */
        if (absmode)                        /* if in absolute assembler mode     */
          error |= ERR_RELOCATING;
        if (!lp)
          error |= ERR_LABEL_MISSING;
        else
          lp->cat = SYMCAT_COMMON;
        common++;                           /* go into common mode               */
        commonsym = lp;                     /* remember common symbol            */
        break;
      case PSEUDO_ENDCOM :                  /* ENDCOM                            */
        if (absmode)                        /* if in absolute assembler mode     */
          error |= ERR_RELOCATING;
        if (common)                         /* terminate common mode             */
          common--;
        else
          error |= ERR_EXPR;                /* must be paired                    */
        break;
      case PSEUDO_FILL :                    /* [label] FILL <value>,<byte_count> */
        operand = scanexpr(0, &p);
        if (unknown)
          error |= ERR_LABEL_UNDEF;
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (*srcptr == ',')                 /* skip count?                       */
          {
          fillc = (char)operand;
          srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          goto DoFill;              // goto
          }
        else
          error |= ERR_EXPR;
        break;
      case PSEUDO_RMB :                     /* [label] RMB <absolute expression> */
      case PSEUDO_RZB :                     /* [label] RZB <absolute expression> */
        fillc = (co == PSEUDO_RZB) ? 0 : rmbfillchr;
      DoFill:                        // label
        operand = scanexpr(0, &p);
        if (unknown)
          error |= ERR_LABEL_UNDEF;

        if (common)                         /* if in common mode                 */
          {
          if ((lp->cat != SYMCAT_EMPTY) &&
              (lp->cat != SYMCAT_UNRESOLVED) &&
              (lp->cat != SYMCAT_COMMONDATA))
            error |= ERR_LABEL_MULT;
          if (lp->cat != SYMCAT_COMMONDATA) /* if not yet done,                  */
            {
            lp->cat = SYMCAT_COMMONDATA;    /* set tymbol type                   */
            lp->u.parent = commonsym;       /* remember COMMON symbol            */
            lp->value = commonsym->value;   /* remember offset from COMMON symbol*/
            commonsym->value +=             /* append # bytes to reserve to -"-  */
                (unsigned short)operand;
            }
          break;
          }

        printovr |= PRINTOV_PADDR;          /* print address in listing          */
        setlabel(lp);
        if (generating && pass == 2)
        {
          if (co != PSEUDO_RMB ||
              outmode == OUT_BIN ||
              outmode == OUT_REL)
          {
            if (co != PSEUDO_RMB)           /* if filling with a value,          */
            {                             /* fill codebuf first                */
              if (!inMacro)
                generating = 1;
              for (i = 0; i < operand && i < sizeof(codebuf); i++)
                putbyte(fillc);
              operand -= i;
            }
            for (i = 0; i < operand; i++)   /* if too much, output rest directly */
              outbyte(fillc, i);
          }
          else switch (outmode)
          {
            case OUT_SREC :                 /* Motorola S51-09 ?                 */
              flushhex();
              break;
            case OUT_IHEX :                 /* Intel Hex ?                       */
              flushihex();  
              break;
            case OUT_FLEX :                 /* FLEX binary ?                     */
              flushflex();
              break;
          }
        }   
        loccounter += (unsigned short)operand;
        hexaddr = loccounter;
        break;  
      case PSEUDO_EQU :                     /* label EQU x                       */
        nRepNext = 0;                       /* reset eventual repeat             */
        if (dwOptions & OPTION_LPA)         /* if listing in patch format,       */
          curline->flg |= LINCAT_PEMTCMT;   /* prefix with comment               */
        operand = scanexpr(0, &p);
        if (!lp)
          error |= ERR_LABEL_MISSING;
        else
        {
          if (lp->cat == SYMCAT_EMPTY ||
              lp->cat == SYMCAT_UNRESOLVED ||
              (lp->value == (unsigned short)operand &&
               pass == 2))
          {
            if (lp->cat != SYMCAT_PUBLIC)
            {   
              if (exprcat == EXPRCAT_INTADDR)
                lp->cat = SYMCAT_LABEL;
              else
                lp->cat = SYMCAT_CONSTANT;
            }
            lp->value = (unsigned short)operand;
            printovr |= PRINTOV_PLABEL;
          }
          else if (lp->cat == SYMCAT_PUBLICUNDEF ||
                   (lp->cat == SYMCAT_PUBLIC &&
                    lp->value == (unsigned short)operand))
          {
            lp->cat = SYMCAT_PUBLIC;
            if (!(exprcat & (EXPRCAT_INTADDR | EXPRCAT_EXTADDR)))
              lp->u.flags |= SYMFLAG_ABSOLUTE;
            lp->value = (unsigned short)operand;
            /* Doesn't look like it's necessary ...
            p.addr = (unsigned short)operand;
            addreloc(&p);
            */
          }
          else
            error |= ERR_LABEL_MULT;
        }
        break;
      case PSEUDO_PUB :                     /* PUBLIC a[,b[,c...]]               */
      case PSEUDO_EXT :                     /* EXTERN a[,b[,c...]]               */
        nRepNext = 0;                       /* reset eventual repeat             */
        skipspace();
        while (isalnum(*srcptr))
        {
          scanname();                       /* parse option                      */
          lp = findsym(namebuf, 1);         /* look up the symbol                */
          switch (co)
          {
            case PSEUDO_PUB :               /* PUBLIC a[,b[,c...]]               */
              if (lp->cat == SYMCAT_EMPTY ||
                  lp->cat == SYMCAT_UNRESOLVED)
                lp->cat = SYMCAT_PUBLICUNDEF;
              else if (lp->cat == SYMCAT_LABEL)
                lp->cat = SYMCAT_PUBLIC;
              else if (lp->cat == SYMCAT_PUBLICUNDEF)
                error |= ERR_LABEL_UNDEF;
              else if (lp->cat != SYMCAT_PUBLIC)
                error |= ERR_LABEL_MULT;
              break;
            case PSEUDO_EXT :               /* EXTERN a[,b[,c...]]               */
              if (absmode)                  /* if not in relocating asm mode,    */
                error |= ERR_RELOCATING;    /* set error                         */
              if (lp->cat == SYMCAT_EMPTY ||
                  lp->cat == SYMCAT_UNRESOLVED)
                lp->cat = SYMCAT_EXTERN;
              else if (lp->cat != SYMCAT_EXTERN)
                error |= ERR_LABEL_MULT;
              break;
          }
          if (*srcptr == ',')
          {
            srcptr++;
            if (!(dwOptions & OPTION_TSC))
              skipspace();
          }
        }
        break;
      case PSEUDO_FCB :                     /* [label] FCB expr[,expr...]        */
        setlabel(lp);
        if (!inMacro)
          generating = 1;
        do
        {
          if (*srcptr == ',')
            srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          if (*srcptr == '\"')
          {
            srcptr++;
            while (*srcptr != '\"' && *srcptr)
              putbyte(*srcptr++);
            if (*srcptr == '\"')
              srcptr++; 
          }
          else
          {
            long ex = scanexpr(0, &p);
            p.addr = (unsigned short)(loccounter + codeptr);
            putbyte((unsigned char)ex);
            p.exprcat |= EXPRCAT_ONEBYTE;
            addreloc(&p);
            if (unknown && pass == 2)
              error |= ERR_LABEL_UNDEF;
          }
          if (!(dwOptions & OPTION_TSC))
            skipspace();
        } while (*srcptr == ',');
        break; 
      case PSEUDO_FCC :                     /* [label] FCC expr[,expr...]        */
      case PSEUDO_FCCH :                     /* [label] FCC expr[,expr...]        */
        setlabel(lp);
        if (!inMacro)
          generating = 1;
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (!(dwOptions & OPTION_TSC))      /* if standard                       */
        {                                 /* accept ONE sequence with an       */
          c = *srcptr++;                    /* arbitrary delimiter character     */
          while (*srcptr != c && *srcptr)
          {
            if (co == PSEUDO_FCCH && *(srcptr+1) == c) {
                putbyte((*srcptr++) | 0x80);
            } else {
                putbyte(*srcptr++);
            }
          }
          if (*srcptr == c)
            srcptr++;
        }
        else                                /* if TSC extended format            */
        {                                 /* accept MORE sequences             */
          do
          {
            if (*srcptr == ',')
              srcptr++;
            if (!(dwOptions & OPTION_TSC))
              skipspace();
            c = *srcptr;
            if ((c == '$') || isalnum(c))
            {
              long ex = scanexpr(0, &p);
              p.addr = (unsigned short)(loccounter + codeptr);
              putbyte((unsigned char)ex);
              if (unknown && pass == 2)
                error |= ERR_LABEL_UNDEF;
            }
            else
            {
              srcptr++;
              while (*srcptr != c && *srcptr)
                putbyte(*srcptr++);
              if (*srcptr == c)
                srcptr++;
            }
            if (!(dwOptions & OPTION_TSC))
              skipspace();
          } while (*srcptr == ',');
        }
        break;
      case PSEUDO_FCW :                     /* [label] FCW,FDB  expr[,expr...]   */
        setlabel(lp);
        if (!inMacro)
          generating = 1;
        do
        {
          unsigned short us;

          if (*srcptr == ',')
            srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          us = (unsigned short)scanexpr(0, &p);
          p.addr = (unsigned short)(loccounter + codeptr);
          putword(us);
          addreloc(&p);
          if (unknown && pass == 2)
            error |= ERR_LABEL_UNDEF; 
          if (!(dwOptions & OPTION_TSC))
            skipspace();     
        } while (*srcptr == ',');
        break;
      case PSEUDO_FCQ :                     /* [label] FCQ,FQB  expr[,expr...]   */
        setlabel(lp);
        if (!inMacro)
          generating = 1;
        do
        {
          if (*srcptr == ',')
            srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          putdword(scanexpr(0, &p));
          if (unknown && pass == 2)
            error |= ERR_LABEL_UNDEF; 
          if (!(dwOptions & OPTION_TSC))
            skipspace();     
        } while (*srcptr == ',');
        break;
      case PSEUDO_ELSE :                    /* ELSE                              */
        if (inMacro)                        /* don't process if in MACRO def.    */
          break;
        nRepNext = 0;                       /* reset eventual repeat             */
        suppress = 1;
        condline = 1;                       /* this is a conditional line        */
        break;
      case PSEUDO_ENDIF :                   /* ENDIF                             */
        if (inMacro)                        /* don't process if in MACRO def.    */
          break;
        nRepNext = 0;                       /* reset eventual repeat             */
        condline = 1;                       /* this is a conditional line        */
        break;
      case PSEUDO_IF :                      /* IF  <expression>[,<skip>]         */
      case PSEUDO_IFN :                     /* IFN <expression>[,<skip>]         */
        if (inMacro)                        /* don't process if in MACRO def.    */
          break;
        condline = 1;                       /* this is a conditional line        */
        nRepNext = 0;                       /* reset eventual repeat             */
        operand = scanexpr(0, &p);
        if (unknown)
          error |= ERR_LABEL_UNDEF;
        if (co == PSEUDO_IFN)               /* if IFN                            */
          operand = !operand;               /* reverse operand                   */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (*srcptr == ',')                 /* skip count?                       */
          {
          srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          nSkipCount = scanexpr(0, &p);
          if (!operand)
            nSkipCount = 0;
          }
        else if (!operand)
          suppress = 2;
        break;
      case PSEUDO_IFC :                     /* IFC  <string1>,<string2>[,<skip>] */
      case PSEUDO_IFNC :                    /* IFNC <string1>,<string2>[,<skip>] */
        if (inMacro)                        /* don't process if in MACRO def.    */
          break;
        condline = 1;                       /* this is a conditional line        */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        scanstring(szBuf1, sizeof(szBuf1));
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (*srcptr != ',')                 /* if not on comma                   */
        {
          *szBuf2 = '\0';                   /* reset 2nd string                  */
          error |= ERR_EXPR;                /* set error                         */
        }
        else
        {
          srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          scanstring(szBuf2, sizeof(szBuf2));
          if (!(dwOptions & OPTION_TSC))
            skipspace();
        }
        operand = !strcmp(szBuf1, szBuf2);
        if (co == PSEUDO_IFNC)
          operand = !operand;
        if (*srcptr == ',')                 /* if skip count                     */
        {
          srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          nSkipCount = scanexpr(0, &p);
          if (!operand)
            nSkipCount = 0;
        }
        else if (!operand)
          suppress = 2;
        break;

      case PSEUDO_IFD :                     /* IFD <symbol>[,skipcount]          */
      case PSEUDO_IFND :                    /* IFND <symbol>[,skipcount]         */
        /* ATTENTION: it is easy to produce phasing errors with these 2,         */
        /*            since symbols are NOT reset when starting pass 2!          */
        if (inMacro)                        /* don't process if in MACRO def.    */
          break;
        skipspace();
        scanname();                         /* parse symbol name                 */
        lp = findsym(namebuf, 0);           /* look up the symbol                */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (*srcptr == ',')                 /* if skip count                     */
        {
          srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          nSkipCount = scanexpr(0, &p);
          if (!lp != (co == PSEUDO_IFND))
            nSkipCount = 0;
        }
                                            /* if no skip count and NOT matched  */
        else if (!lp != (co == PSEUDO_IFND))
          suppress = 2;                     /* suppress until ELSE or ENDIF      */
        condline = 1;                       /* this is a conditional line        */
        break;

      case PSEUDO_ORG :                     /* ORG <expression>                  */
        nRepNext = 0;                       /* reset eventual repeat             */
        operand = scanexpr(0, &p);
        if (unknown)
          error |= ERR_LABEL_UNDEF;
        if (relocatable &&                  /* if in relocating assembler mode   */
            (!absmode))
        {
          error |= ERR_RELOCATING;          /* set error                         */
          break;                            /* and ignore                        */
        }
        if (generating && pass == 2)
        {
          switch (outmode)
          {
            case OUT_BIN :                  /* binary output file                */
              j = (int)(unsigned short)operand - (int)loccounter;
              if (j > 0)                    /* if forward gap                    */
              {                           /* get current pos and file end      */
                long pos = ftell(objfile);
                long avbytes = fseek(objfile, 0, SEEK_END) - pos;
                fseek(objfile, pos, SEEK_SET);
                for (i = 0; i < j; i++)     /* seek forward that many bytes      */
                {
                  if (i < avbytes)
                    fseek(objfile, 1, SEEK_CUR);
                  else if (dwOptions & OPTION_FBG)
                    fputc(rmbfillchr, objfile);
                }
              }
              else                          /* if backward gap                   */
              {
                j = -j;
                if (dwOptions & OPTION_FBG) /* and we're filling gaps,           */
                  for (i = 0; i < j; i++)   /* seek back that many bytes         */
                    fseek(objfile, -1, SEEK_CUR);
              }
              break;
            case OUT_SREC :                 /* motorola s51-09                   */
              flushhex();  
              break;
            case OUT_IHEX :                 /* intel hex format                  */
              flushihex();  
              break;
            case OUT_FLEX :                 /* FLEX binary                       */
              flushflex();
              break;
            case OUT_REL :                  /* RELASMB output                    */
              break;
          }
        }
        loccounter = (unsigned short)operand;
        hexaddr = loccounter;
        break;
      case PSEUDO_SETDP :                   /* SETDP [<abs page value>]          */
        nRepNext = 0;                       /* reset eventual repeat             */
        skipspace();
        if ((!*srcptr) || (*srcptr == '*') || (*srcptr == ';'))
          operand = -1;
        else
          operand = scanexpr(0, &p);
        if (unknown)
          error |= ERR_LABEL_UNDEF;
        if (!(operand & 255))
          operand = (unsigned short)operand >> 8;
        if ((unsigned)operand > 255)
          operand = -1;
        if (absmode)
        {
          dpsetting = operand;
          printovr |= PRINTOV_PDP;
        }
        else
          error |= ERR_RELOCATING;
        break;
      case PSEUDO_SET :                     /* label SET <non external expr>     */
        nRepNext = 0;                       /* reset eventual repeat             */
        operand = scanexpr(0, &p);
        if (!lp)
          error |= ERR_LABEL_MISSING;
        else
        {
          if (lp->cat & SYMCAT_VARIABLE ||
              lp->cat == SYMCAT_UNRESOLVED)
          {
            if (exprcat == EXPRCAT_INTADDR)
              lp->cat = SYMCAT_VARADDR;
            else
              lp->cat = SYMCAT_VARIABLE;
            lp->value = (unsigned short)operand;
            printovr |= PRINTOV_PLABEL;
          }
          else
            error |= ERR_LABEL_MULT;
        }
        break;
      case PSEUDO_END :                     /* END [loadaddr]                    */
        nRepNext = 0;                       /* reset possible repeat             */
        skipspace();                        /* skip blanks                       */
        if (!(curline->lvl & LINCAT_LVLMASK))  /* only in outermost level!       */
        {
          if (isfactorstart(*srcptr))       /* if possible transfer address      */
          {
            tfradr = (unsigned short)       /* get transfer address              */
                scanexpr(0, &p);
            if (error)                      /* if error in here                  */
              tfradr = 0;                   /* reset to zero                     */
            else                            /* otherwise                         */
            {
              tfradrset = 1;                /* remember transfer addr. is set    */
              printovr |= PRINTOV_PTFR;
            }
          }
        }
                                            /* terminate current level           */
        g_termflg = (curline->lvl & LINCAT_LVLMASK);  /* (and any sublevels)     */
        break;     
      case PSEUDO_INCLUDE :                 /* INCLUDE <filename>                */
        nRepNext = 0;                       /* reset eventual repeat             */
        if (inMacro ||                      /* if in macro definition            */
            (curline->flg & LINCAT_MACEXP)) /* or macro expansion                */
          error |= ERR_EXPR;                /* this is an error.                 */
        else if (pass == 1)                 /* otherwise expand if in pass 1     */
        {
          char fname[FNLEN + 1];
          int instring = 0;
          char *osrc = srcptr;
                                            /* if impossible                     */
          if ((curline->lvl & LINCAT_LVLMASK) == LINCAT_LVLMASK)
          {
    #if 0
            allocline(curline, "NULL", -1, 0x0f,
                      "Error including file - nesting level too deep");
    #endif
            error |= ERR_MALLOC;            /* set OUT OF MEMORY error           */
            break;
          }
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          if (*srcptr == '\"')
          {
            srcptr++;
            instring = 1;
          }
          for (i = 0; i < FNLEN; i++)
          {
            if (*srcptr == 0 ||
                (!instring && *srcptr == ' ') ||
                *srcptr == '"')
              break;
            fname[i] = *srcptr++;
          }
          fname[i] = 0;
          curline->flg |= LINCAT_INVISIBLE; /* preclude listing of INCLUDE line  */
                                            
          readfile(fname,                   /* append include after current line */
                   (unsigned char)((curline->lvl & LINCAT_LVLMASK) + 1),
                   curline);
          expandtext();                     /* re-expand current line            */
          srcptr = osrc;
        }
        break; 
      case PSEUDO_OPT :                     /* OPT,OPTION  option[,option...]    */
        nRepNext = 0;                       /* reset eventual repeat             */
        skipspace();
        while (isalnum(*srcptr))
        {
          scanname();                       /* parse option                      */
          if (setoption(unamebuf))
            error |= ERR_OPTION_UNK;
          if (*srcptr == ',')
          {
            srcptr++;
            if (!(dwOptions & OPTION_TSC))
              skipspace();
          }
        }
        break;
      case PSEUDO_NAM :                     /* NAM,TTL <text>                    */
      case PSEUDO_STTL :                    /* STTL <text>                       */
        nRepNext = 0;                       /* reset eventual repeat             */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (isalnum(*srcptr))
        {
          char *tgt = (co == PSEUDO_NAM) ? szTitle : szSubtitle;
          char *lnblnk = tgt;
          int nBytes = 0;
          while (*srcptr && nBytes < (sizeof(szTitle) - 1))
          {
            if (*srcptr != ' ')
              lnblnk = tgt;
            *tgt++ = *srcptr++;
            nBytes++;
          }
          lnblnk[1] = '\0';                 /* terminate after last nonblank     */
          while (*srcptr)                   /* skip rest if too long             */
            srcptr++;
        }
        break;
      case PSEUDO_PAG :                     /* PAG [<abs expression>]            */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (isfactorstart(*srcptr))         /* if possible new page number       */
        {
          int nPage = scanexpr(0, &p);      /* get new page #                    */
          if (!error && listfile)           /* if valid and writing listfile     */
            nCurPage = nPage - 1;
          else
            break;
        }
        curline->flg |= LINCAT_INVISIBLE;
        if ((listing & LIST_ON) && listfile &&
            (dwOptions & OPTION_PAG) &&
            (!(curline->lvl & LINCAT_LVLMASK) || (dwOptions & OPTION_LLL)) &&
            (pass > 1 || (dwOptions & OPTION_LP1)))
          PageFeed();
        break;
      case PSEUDO_SPC :                     /* SPC <n[,keep]>                    */
      {
        int nSpc = 1, nKeep = 0;
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (isfactorstart(*srcptr))         /* if possible new page number       */
        {
          nSpc = scanexpr(0, &p);           /* get # space lines                 */
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          if (*srcptr == ',')               /* if followed by ,                  */
          {
            srcptr++;
            if (!(dwOptions & OPTION_TSC))
              skipspace();
            nKeep = scanexpr(0, &p);        /* get # keep lines                  */
          }
          if (error)
            break;
        }
        curline->flg |= LINCAT_INVISIBLE;
        if (listing & LIST_ON)
        {
          if (nSpc > 0)                     /* if spaces needed                  */
          {
            if ((dwOptions & OPTION_PAG) &&
                (nCurLine + nSpc + nKeep >= nLinesPerPage))
              PageFeed();
            else for (; nSpc; nSpc--)
              putlist("\n");
          }
          else if (nKeep &&
              (dwOptions & OPTION_PAG) &&
              (nCurLine + nKeep >= nLinesPerPage))
            PageFeed();
        }
      }
        break;
      case PSEUDO_REP :                     /* REP n                             */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        nRepNext = scanexpr(0, &p);         /* get # repetitions                 */
        curline->flg |= LINCAT_INVISIBLE;
        break;
      case PSEUDO_SETPG :                   /* SETPG pagelen                     */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        nLinesPerPage = scanexpr(0, &p);    /* get # lines per page              */
        if (nLinesPerPage < 10)             /* adjust to boundary values         */
          nLinesPerPage = 10;
        else if (nLinesPerPage > 1000)
          nLinesPerPage = 1000;
        curline->flg |= LINCAT_INVISIBLE;
        break;
      case PSEUDO_SETLI :                   /* SETLI linelen                     */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        nColsPerLine = scanexpr(0, &p);     /* get # columns per line            */
        if (nColsPerLine < 40)              /* adjust to boundary values         */
          nColsPerLine = 40;
        else if (nColsPerLine > 2000)
          nColsPerLine = 2000;
        curline->flg |= LINCAT_INVISIBLE;
        break;
      case PSEUDO_SYMLEN :                  /* SYMLEN symbollength               */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        maxidlen = scanexpr(0, &p);         /* get # significant ID places       */
        if (maxidlen < 6)                   /* adjust to boundary values         */
          maxidlen = 6;
        else if ((outmode == OUT_REL) &&
                 (maxidlen > 8))
          maxidlen = 8;
        else if (maxidlen > MAXIDLEN)
          maxidlen = MAXIDLEN;
        curline->flg |= LINCAT_INVISIBLE;
        break;
      case PSEUDO_MACRO :                   /* label MACRO                       */
        printf("Warning: MACRO not supported\n");
        if (!lp) {                           /* a macro NEEDS a label!            */
          error |= ERR_LABEL_MISSING;
          break;
        }
        if (lp->cat == SYMCAT_EMPTY)
        {
          if (nMacros < MAXMACROS)          /* if space for another macro defin. */
          {
            lp->cat = SYMCAT_MACRO;         /* remember it's a macro             */
            macros[nMacros] = curline;      /* remember pointer to start line    */
            lp->value = nMacros++;          /* and remember the macro            */
          }
          else
            error |= ERR_MALLOC;
        }
        else if (lp->cat != SYMCAT_MACRO || macros[lp->value] != curline)
          error |= ERR_LABEL_MULT;
        inMacro++;
        curline->flg |= LINCAT_MACDEF;
        if (inMacro > 1)
          error |= ERR_NESTING;
        break;
      case PSEUDO_ENDM :                    /* ENDM                              */
        if (inMacro) {
          inMacro--;
        }
        else
        {
          if (lvlMacro)
            lvlMacro--;
          if (!(curline->flg & LINCAT_MACEXP))
            error |= ERR_EXPR;
        }
        break;
      case PSEUDO_EXITM :                   /* EXITM                             */
        if (inMacro)
          ;
        else
        {
          if (lvlMacro)
            lvlMacro--;
          if (!(curline->flg & LINCAT_MACEXP))
            error |= ERR_EXPR;
        }
        break;
      case PSEUDO_REG :                     /* label REG <register list>         */
        if (!lp)                            /* label is mandatory!               */
          error |= ERR_LABEL_MISSING;
        {
        struct regrecord *p;
        postbyte = 0;
        do
        {
          if (*srcptr == ',')
            srcptr++;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
          scanname(); 
          if ((p = findreg(unamebuf)) == 0)
            error |= ERR_ILLEGAL_ADDR;
          else
            postbyte |= p->psh; 
          if (!(dwOptions & OPTION_TSC))
            skipspace();
        } while (*srcptr == ',');
        if (lp->cat == SYMCAT_EMPTY)
        {
          lp->cat = SYMCAT_REG;
          lp->value = postbyte;
        }
        else if (lp->cat != SYMCAT_REG ||
                 lp->value != postbyte)
          error |= ERR_LABEL_MULT;
        }
        break;
      case PSEUDO_ERR :                     /* ERR text?                         */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (pass != 1)                      /* ignore in pass 1                  */
        {
          errormsg[14] = srcptr;
          error |= ERR_ERRTXT;
        }
        break;
      case PSEUDO_WRN :                     /* WRN text?                         */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (pass != 1)                      /* ignore in pass 1                  */
        {
          warningmsg[14] = srcptr;
          warning |= WRN_WRNTXT;
        }
        break;
      case PSEUDO_TEXT :                    /* TEXT text ?                       */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (!lp)                            /* label is mandatory!               */
          error |= ERR_LABEL_MISSING;
        else if (lp->cat != SYMCAT_EMPTY && /* and must be text, if there        */
                 lp->cat != SYMCAT_TEXT)
          error |= ERR_LABEL_MULT;
        else                                /* if all OK, (re)define text        */
        {
          scanstring(szBuf1, sizeof(szBuf1));
          settext(lp->name, szBuf1);
        }
        break;
      case PSEUDO_NAME :                    /* NAME <modulename> ?               */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        scanname();
        if (!namebuf[0])                    /* name must be given                */
          error |= ERR_EXPR;
    #if 0
        // gcc 9.1.0 flags this as "output may be truncated" (well, duh!), so use sprintf instead
        strncpy(modulename, namebuf, maxidlen);
        modulename[maxidlen] = '\0';
    #else
        sprintf(modulename, "%.*s", maxidlen, namebuf);
    #endif
        break;
      case PSEUDO_BINARY :                  /* BIN[ARY] <filename> ?             */
        nRepNext = 0;                       /* reset eventual repeat             */
        if (pass == 1)                      /* expand if in pass 1               */
        {
          char fname[FNLEN + 1];
          int instring = 0;
          char *osrc = srcptr;

          if (!(dwOptions & OPTION_TSC))
            skipspace();
          if (*srcptr == '\"')
          {
            srcptr++;
            instring = 1;
          }
          for (i = 0; i < FNLEN; i++)
          {
            if (*srcptr == 0 ||
                (!instring && *srcptr == ' ') ||
                *srcptr == '"')
              break;
            fname[i] = *srcptr++;
          }
          fname[i] = 0;
          curline->flg |= LINCAT_INVISIBLE; /* preclude listing of BINARY line   */
                                            
          readbinary(fname,                 /* append binary after current line  */
                   (unsigned char)((curline->lvl & LINCAT_LVLMASK) + 1),
                   curline, lp);
          expandtext();                     /* re-expand current line            */
          srcptr = osrc;
        }
        break; 
      case PSEUDO_PHASE :                   /* PHASE address ?                   */
        nRepNext = 0;                       /* reset eventual repeat             */
        operand = scanexpr(0, &p);
        if (unknown)
          error |= ERR_LABEL_UNDEF;
        if (relocatable &&                  /* if in relocating assembler mode   */
            (!absmode))
        {
          error |= ERR_RELOCATING;          /* set error                         */
          break;                            /* and ignore                        */
        }
        /* ... now act on operand! */
        phase = (int)operand - (int)loccounter;
        printovr |= PRINTOV_PPHA;
        break;
      case PSEUDO_DEPHASE :                 /* DEPHASE ?                         */
        /* end last PHASE */
        phase = 0;
        printovr |= PRINTOV_PPHA;
        break;
      case PSEUDO_PEMT :                    /* PEMT <text>                       */
        nRepNext = 0;                       /* reset eventual repeat             */
        skipspace();
        if (isalnum(*srcptr))
        {
          char *tgt = szBuf1;
          char *lnblnk = tgt;
          int nBytes = 0;
          while (*srcptr && nBytes < (sizeof(szBuf1) - 1))
          {
            if (*srcptr != ' ')
              lnblnk = tgt;
            *tgt++ = *srcptr++;
            nBytes++;
          }
          lnblnk[1] = '\0';                 /* terminate after last nonblank     */
          while (*srcptr)                   /* skip rest if too long             */
            srcptr++;
          if (!(dwOptions & OPTION_LPA))
            curline->flg |= LINCAT_INVISIBLE;
        }
        break;
    }
}

/*****************************************************************************/
/* macskip : skips a range of macro lines                                    */
/*****************************************************************************/

struct linebuf *macskip(struct linebuf *pmac, int nSkips)
{
    if (nSkips < 0)                         /* we need to go to the line BEFORE  */
      nSkips--;                             /* the one we need!                  */
    while (nSkips)
    {
      if (nSkips < 0)
      {
        if (!pmac->prev)
          break;
        pmac = pmac->prev;
        nSkips++;
      }
      else
      {
        if (!pmac->next)
          break;
        pmac = pmac->next;
        nSkips--;
      }
    }
    return pmac;
}

/*****************************************************************************/
/* expandmacro : expands a macro definition below the current line           */
/*****************************************************************************/

void expandmacro(struct symrecord *lp, struct symrecord *lpmac)
{
    struct oprecord *op;
    char szMacInv[LINELEN];                 /* macro invocation line             */
    char szLine[LINELEN];                   /* current macro line                */
    char *szMacParm[10];
    char *s, *d;                            /* source / destination work pointers*/
    char *srcsave = srcptr;
    struct linebuf *cursave = curline;
    int nMacParms = 1;
    int nInString = 0;
                                            /* current macro line                */
    struct linebuf *pmac = macros[lpmac->value]->next;
    struct linebuf *pcur = curline;         /* current expanded macro line       */
    struct linebuf *pdup = NULL;            /* DUP start line                    */
    int nDup = 0;                           /* # repetitions for DUP             */
    int terminate = 0;                      /* terminate macro expansion if set  */
    int skipit = 0;                         /* skip this line                    */
    int suppress[64] = {0};                 /* internal suppression (max.64 lvl) */
    int ifcount = 0;                        /* internal if counter               */
    int curlvl = curline->lvl & LINCAT_LVLMASK;
    struct relocrecord p = {0};

    #if 0
    if ((listing & LIST_ON) &&              /* if listing pass 1                 */
        (dwOptions & OPTION_LIS) &&
        (dwOptions & OPTION_LP1) &&
        (dwOptions & OPTION_MAC))
      outlist(NULL, NULL);                  /* show macro invocation BEFORE      */
                                            /* processing the expansions         */
    #endif

    skipspace();                            /* skip spaces before macro args     */

    if (!lp)                                /* if no macro label                 */
      szMacParm[0] = "";                    /* set &0 to blank                   */
    else                                    /* otherwise                         */
      szMacParm[0] = lp->name;              /* set &0 to line label              */
                                            /* initialize all parameters to ""   */
    for (nMacParms = 1; nMacParms < 10; nMacParms++)
      szMacParm[nMacParms] = "";
    nMacParms = 1;                          /* reset # parsed parms to 1         */

    strcpy(szMacInv, srcptr);               /* copy the current line for mangling*/
    srcptr = szMacInv;                      /* set pointer to internal buffer    */

    while (*srcptr)                         /* while there are parameters        */
    {
      if (*srcptr == ',')                   /* if parameter delimiter            */
      {
        *srcptr++ = '\0';                   /* delimit & advance behind it       */
        if (!(dwOptions & OPTION_TSC))
          skipspace();
        if (++nMacParms >= 10)
          break;
      }
      else                                  /* otherwise walk thru parameter     */
      {
        szMacParm[nMacParms] = srcptr;      /* store parameter start pointer     */
        if ((*srcptr == '\'') ||            /* if delimited string               */
            (*srcptr == '\"'))
        {
          nInString = 1;                    /* remember we're in delimited strg  */
          srcptr++;
        }
        while (*srcptr)                     /* walk to end of parameter          */
        {
          if (!nInString &&
              ((*srcptr == ' ') || (*srcptr == ',')))
            break;
          else if (nInString &&
                  *srcptr == *szMacParm[nMacParms])
          {
            szMacParm[nMacParms]++;
            *srcptr++ = '\0';
            break;
          }
          else
            srcptr++;
        }
        if (*srcptr == ' ')                 /* ' ' terminates in TSC             */
        {
          if (dwOptions & OPTION_TSC)
            {
            *srcptr++ = '\0';
            break;
            }
          else
            skipspace();
        }
        else if (*srcptr == ',')            /* advance to next                   */
        {
          *srcptr++ = '\0';
          if (++nMacParms >= 10)
            break;
          if (!(dwOptions & OPTION_TSC))
            skipspace();
        }
      }
    }

    /*---------------------------------------------------------------------------*/
    /* OK, got macro arguments &0...&9 now                                       */
    /*---------------------------------------------------------------------------*/

    while (pmac)                            /* walk through the macro lines      */
    {
      srcptr = s = pmac->txt;
      d = szLine;
      op = NULL;
      skipit = 0;

      while (*s)                            /* first, expand the line            */
      {
        if (*s == '\\' && s[1] == '&')
        {
          s++;
          *d++ = *s++;
        }
        else if (*s == '&')
        {
          if (s[1] >= '0' && s[1] <= '9')
          {
            char *from = szMacParm[s[1] - '0'];
            int i = 0, j = (int)(d - szLine);
            for (; j < LINELEN && from[i]; i++, j++)
              *d++ = from[i];
            s += 2;
          }
          else
          {
            struct symrecord *lp;
            char iname[sizeof(namebuf)];
            char *nc;
            parsename(s + 1, &nc);
            sprintf(iname, "&%.*s", (int)(sizeof(iname) - 2), namebuf);
            lp = findsym(iname, 0);
            if (lp && *namebuf &&           /* if symbol IS a text constant,     */
                (lp->cat == SYMCAT_TEXT))
            {                             /* insert its content                */
              int i = 0, j = (int)(d - szLine);
              char *from = texts[lp->value];
              for (; j < LINELEN && from[i]; i++, j++)
                *d++ = from[i];
              s = nc;
            }
            else
              *d++ = *s++;
          }
        }
        else
          *d++ = *s++;
      }
      *d = '\0';

      srcptr = szLine;                      /* then, look whether code or macro  */
      if (isalnum(*srcptr))
      {
        scanname();
        lp = findsym(namebuf, 1);
        if (*srcptr == ':')
          srcptr++;
      } 
      skipspace();
      if (isalnum(*srcptr))                 /* then parse opcode                 */
      {
        scanname(); 
        op = findop(unamebuf);
      }
      skipspace();                          /* and skip to eventual parameter    */

      if (op && op->cat == OPCAT_PSEUDO)    /* if pseudo-op                      */
      {
        switch (op->code)                   /* examine for macro expansion       */
        {
          case PSEUDO_ENDM :                /* ENDM ?                            */
            terminate = 1;                  /* terminate macro expansion         */
            break;
          case PSEUDO_EXITM :               /* EXITM ?                           */
            if (!suppress[ifcount])         /* if not suppressed                 */
              terminate = 1;                /* terminate macro expansion         */
            break;
          case PSEUDO_DUP :                 /* DUP ?                             */
            if (pdup != NULL)               /* nesting not allowed               */
              error |= ERR_NESTING;
            else
            {
              nDup = scanexpr(0, &p);       /* scan the expression               */
              pdup = pmac;                  /* remember DUP start line           */
              if (nDup < 1 || nDup > 255)   /* if invalid # repetitions          */
                {
                error |= ERR_EXPR;          /* set error here                    */
                nDup = 1;
                }
              else
                skipit = 1;                 /* skip this line                    */
            }
            break;
          case PSEUDO_ENDD :                /* ENDD ?                            */
            if (!pdup)                      /* if no DUP defined                 */
              error |= ERR_EXPR;            /* can't do that here.               */
            else
            {
              nDup--;                       /* decrement # duplications          */
              if (!nDup)                    /* if done,                          */
                pdup = NULL;                /* reset dup start                   */
              else                          /* otherwise                         */
                pmac = pdup;                /* reset current line to DUP op      */
              skipit = 1;                   /* this line isn't really there      */
            }
            break;
          case PSEUDO_IF :                  /* IF                                */
          case PSEUDO_IFN :                 /* IFN                               */
            ifcount++;                      /* increment # ifs                   */
                                            /* take suppression from higher level*/
            suppress[ifcount] = suppress[ifcount - 1];
            if (suppress[ifcount])          /* if already suppressed             */
              suppress[ifcount]++;          /* inrease # suppressions            */
            else                            /* otherwise evaluate expression     */
              {
              operand = scanexpr(0, &p);
              if (unknown)
                error |= ERR_LABEL_UNDEF;
              if (op->code == PSEUDO_IFN)   /* if IFN                            */
                operand = !operand;         /* invert operand                    */
              if (!operand)                 /* if evaulation to zer0             */
                suppress[ifcount]++;        /* suppress until ELSE or ENDIF      */
              if (!(dwOptions & OPTION_TSC))
                skipspace();
              if (*srcptr == ',')           /* if skip count passed              */
                {
                int nSkips;
                srcptr++;
                if (!(dwOptions & OPTION_TSC))
                  skipspace();
                nSkips = scanexpr(0, &p);   /* scan skip count                   */
                ifcount--;                  /* this needs no ENDIF               */
                if (nSkips < -255 || nSkips > 255 || nSkips == 0)
                  {
                  error |= ERR_EXPR;
                  break;
                  }
                if (operand)                /* if evaluation is true             */
                  pmac = macskip(pmac, nSkips); /* then skip the amount of lines */
                }
              }
            skipit = 1;                     /* don't add this line!              */
            break;
          case PSEUDO_IFC :                 /* IFC                               */
          case PSEUDO_IFNC :                /* IFNC                              */
            ifcount++;                      /* increment # ifs                   */
                                            /* take suppression from higher level*/
            suppress[ifcount] = suppress[ifcount - 1];
            if (suppress[ifcount])          /* if already suppressed             */
              suppress[ifcount]++;          /* inrease # suppressions            */
            else                            /* otherwise evaluate expression     */
            {
              if (!(dwOptions & OPTION_TSC))
                skipspace();
              scanstring(szBuf1, sizeof(szBuf1));
              if (!(dwOptions & OPTION_TSC))
                skipspace();
              if (*srcptr != ',')           /* if not on comma                   */
              {
                *szBuf2 = '\0';             /* reset 2nd string                  */
                error |= ERR_EXPR;          /* set error                         */
              }
              else
              {
                srcptr++;
                if (!(dwOptions & OPTION_TSC))
                  skipspace();
                scanstring(szBuf2, sizeof(szBuf2));
              }
              operand = !strcmp(szBuf1, szBuf2);
              if (op->code == PSEUDO_IFNC)
                operand = !operand;
              if (!operand)                 /* if evaulation to zer0             */
                suppress[ifcount]++;        /* suppress until ELSE or ENDIF      */
              if (!(dwOptions & OPTION_TSC))
                skipspace();
              if (*srcptr == ',')           /* if skip count passed              */
              {
                int nSkips;
                srcptr++;
                if (!(dwOptions & OPTION_TSC))
                  skipspace();
                nSkips = scanexpr(0, &p);   /* scan skip count                   */
                ifcount--;                  /* this needs no ENDIF               */
                if (nSkips < -255 || nSkips > 255 || nSkips == 0)
                  {
                  error |= ERR_EXPR;
                  break;
                  }
                if (operand)                /* if evaluation is true             */
                  pmac = macskip(pmac, nSkips); /* then skip the amount of lines */
              }
            }
            if (!error)
              skipit = 1;                   /* don't add this line!              */
            break;
          case PSEUDO_ELSE :                /* ELSE                              */
            if (!suppress[ifcount])         /* if IF not suppressed              */
              suppress[ifcount]++;          /* suppress ELSE clause              */
            else                            /* otherwise                         */
              suppress[ifcount]--;          /* decrement suppression             */
            skipit = 1;                     /* don't add this line!              */
            break;
          case PSEUDO_ENDIF :               /* ENDIF                             */
            if (ifcount)
              ifcount--;
            if (suppress[ifcount])
              suppress[ifcount]--;
            skipit = 1;                     /* don't add this line!              */
            break;
        }
      }

      if (!skipit && !suppress[ifcount])    /* if not skipping this one          */
      {                                   /* add line to source                */
        pcur = allocline(pcur, curline->fn, curline->ln,
                         (unsigned char)(LINCAT_MACEXP | curlvl),
                         szLine);
        if (!pcur)
        {
          error |= ERR_MALLOC;
          break;
        }
        else
        {
          curline = pcur;
          error = ERR_OK;
          warning = WRN_OK;
          expandtext();
        }
      }

      if (terminate)                        /* if macro termination needed       */
        break;                              /* terminate here                    */

      pmac = pmac->next;
    }

    curline = cursave;
    expandtext();
    srcptr = srcsave;                       /* restore source pointer            */
    codeptr = 0;
}

/*****************************************************************************/
/* processline : processes a source line                                     */
/*****************************************************************************/

void processline()
{
    struct symrecord *lp, *lpLabel = NULL, *lpmac = NULL;
    struct oprecord *op = NULL;
    int co;
    unsigned short cat;
    char c;
    char noimm;
    unsigned short page;
    char nomac = 0;

    #if 0
    srcptr = curline->txt;
    #else
    srcptr = srcline;
    #endif

    oldlc = loccounter;
    unknown = 0;
    certain = 1;
    lp = 0;
    codeptr = 0;
    condline = 0;
    printovr = 0;

    if (inMacro)
      curline->flg |= LINCAT_MACDEF;

    if (isValidNameChar(*srcptr, 1))        /* look for label on line start      */
    {
      scanname();
      lp = findsym(namebuf, 1);
      if (*srcptr == ':')                   /* skip : following label            */
        srcptr++;

      if (lp)
      {
        if ((lp->cat != SYMCAT_COMMONDATA) &&
            (lp->u.flags & SYMFLAG_FORWARD))
          lp->u.flags |= SYMFLAG_PASSED;
        if ((absmode) &&
            (lp->cat == SYMCAT_PUBLICUNDEF ||
             lp->cat == SYMCAT_PUBLIC))
          lp->u.flags |= SYMFLAG_ABSOLUTE;
      }

      lpLabel = lp;
    } 
    skipspace();

    if (*srcptr == '\\')                    /* if macro explicitly prohibited    */
    {
      srcptr++;
      nomac = 1;
    }

    if (isValidNameChar(*srcptr, 1))        /* mnemonic or macro name            */
    {
      scanname();
      if (!nomac)                           /* if macro allowed                  */
        lpmac = findsym(namebuf, 0);        /* look whether opcode is a macro    */
      if (lpmac && lpmac->cat == SYMCAT_MACRO)
      {
        if (pass == 1 &&                    /* if in pass 1 and not in macro def */
            !LINE_IS_MACDEF(curline->flg))
          {                                 /* if on top level,                  */
          if (!LINE_IS_MACEXP(curline->flg))
            curline->flg |= LINCAT_MACINV;  /* mark as macro invocation          */
          if (dwOptions & OPTION_DLM)
            setlabel(lp);
          if (++lvlMacro <= LINCAT_LVLMASK)
            expandmacro(lp, lpmac);         /* expand macro below current line   */
          else                              /* unless nesting level is too deep  */
            error |= ERR_NESTING;
          }
      }
      else                                  /* no macro, so check mnemonics      */
      {
        if ((dwOptions & OPTION_H09) &&     /* adjust some mnemonics             */
            ((!strcmp(unamebuf, "ASLD")) || /* that are available in the 6309    */
            (!strcmp(unamebuf, "ASRD")) ||  /* but are implemented as (slower)   */
            (!strcmp(unamebuf, "LSLD")) ||  /* convenience instructions on the   */
            (!strcmp(unamebuf, "LSRD")) ||  /* 6809                              */
            (!strcmp(unamebuf, "DECD")) ||
            (!strcmp(unamebuf, "INCD")) ||
            (!strcmp(unamebuf, "CLRD"))))
          strcat(unamebuf, "63");
        op = findop(unamebuf);
        if (op)
        {
          if ((dwOptions & OPTION_TSC))     /* if TSC compatible, skip space NOW */
            skipspace();                    /* since it's not allowed inside arg */
          if (op->cat != OPCAT_PSEUDO)
          {
            setlabel(lp);
            if (!inMacro)
              generating = 1;
          }
          co = op->code;
          cat = op->cat;
                                            /* only pseudo-ops in common mode!   */
          if (common && (cat != OPCAT_PSEUDO))
            error |= ERR_EXPR;

          noimm = cat & OPCAT_NOIMM;        /* isolate "no immediate possible"   */
          cat &= ~OPCAT_NOIMM;
          if (dwOptions & OPTION_H11)       /* if in 86HC11 mode,                */
          {                               /* isolate pageXX flag               */
            page = cat & (OPCAT_PAGE18 | OPCAT_PAGE1A);
            cat &= ~(OPCAT_PAGE18 | OPCAT_PAGE1A);
          }
          if (dwOptions & OPTION_H09)       /* if in HD6309 mode,                */
            cat &= ~OPCAT_6309;             /* mask out the 6309 flag (=allow)   */
          if (dwOptions & OPTION_H01)       /* if in HD6301 mode,                */
            cat &= ~OPCAT_6301;             /* mask out the 6301 flag (=allow)   */
          switch (cat)
          {
            case OPCAT_ONEBYTE :
              onebyte(co);
              break;
            case OPCAT_TWOBYTE :
              twobyte(co);
              break;
            case OPCAT_THREEBYTE :
              threebyte(co);
              break;
            case OPCAT_FOURBYTE :
              fourbyte(co);
              break;
            case OPCAT_2IMMBYTE :           /* 6309 only                         */
              putbyte((unsigned char)(co >> 8));
              /* fall thru on purpose! */
            case OPCAT_IMMBYTE :
              oneimm(co);
              break;
            case OPCAT_LEA :
              lea(co);
              break;
            case OPCAT_SBRANCH :
              sbranch(co);
              break;
            case OPCAT_LBR2BYTE :
              lbranch(co);
              break;
            case OPCAT_LBR1BYTE :
              lbra(co);
              break;
            case OPCAT_2ARITH :             /* 6309 only                         */
              putbyte((unsigned char)(co >> 8));
              /* fall thru on purpose! */
            case OPCAT_ARITH :
              arith(co, noimm);
              break;
            case OPCAT_ACCARITH :           /* 6800-style arith                  */
              accarith(co, noimm, 0);
              break;
            case OPCAT_OACCARITH :          /* potential 6800-style arith        */
              accarith(co, noimm, 1);
              break;
            case OPCAT_DBLREG1BYTE :
              if ((dwOptions & OPTION_H11) && (page & OPCAT_PAGE18))
                darith18(co, noimm);
              else if ((dwOptions & OPTION_H11) && (page & OPCAT_PAGE1A))
                darith1a(co, noimm);
              else
                darith(co, noimm);
              break;
            case OPCAT_DBLREG2BYTE :
              d2arith(co, noimm);
              break;
            case OPCAT_SINGLEADDR :
              oneaddr(co);
              break;
            case OPCAT_IREG :                 /* 6309 only                         */
              putbyte((unsigned char)(co >> 8));
              /* fall thru on purpose! */
            case OPCAT_2REG :
              tfrexg(co);
              break;
            case OPCAT_STACK :
              pshpul(co, 0);
              break;
            case OPCAT_BITDIRECT :          /* 6301/6309 only                    */
              bitdirect(co);
              break;
            case OPCAT_BITTRANS :           /* 6309 only                         */
              bittrans(co);
              break;
            case OPCAT_BLOCKTRANS :         /* 6309 only                         */
              blocktrans(co);
              break;
            case OPCAT_QUADREG1BYTE :
              qarith(co, noimm);
              break;
            case OPCAT_IDXEXT :
              idxextarith(co);
              break;
            case OPCAT_ACCADDR :
              accaddr(co);
              break;
            case OPCAT_SETMASK :            /* 68HC11 only                       */
              setmask(co);
              break;
            case OPCAT_BRMASK :             /* 68HC11 only                       */
              brmask(co);
              break;
            case OPCAT_OSTACK :
              pshpul(co, 1);
              break;
            case OPCAT_PSEUDO :
              pseudoop(co, lp);
              break;
            default :
              error |= ERR_ILLEGAL_MNEM;
              break;
          }
          c = *srcptr;                      /* get current character             */
          if (((dwOptions & OPTION_TSC) && (c == '*')) ||
              ((dwOptions & OPTION_GAS) && (c == '|')) ||
              (c == ';'))
            c = '\0';
          if (c != ' ' &&
              *(srcptr - 1) != ' ' &&
              c != '\0')
            error |= ERR_ILLEGAL_ADDR; 
        }
        else
          error |= ERR_ILLEGAL_MNEM;
      }
    }
    else
    {
      nRepNext = 0;                         /* reset possible repeat if no code  */
      setlabel(lp);
      c = *srcptr;                          /* now look where we are at ...      */
      if ((c == '*') ||
          ((dwOptions & OPTION_GAS) && (c == '|')) ||
          (c == ';'))
        c = '\0';
      if (c)                                /* if NOT empty or comment,          */
        error |= ERR_EXPR;                  /* mark as invalid expression        */
      }

    if (inMacro)                            /* if in macro definition            */
    {
      codeptr = 0;                          /* ignore the code                   */
      error &= (ERR_MALLOC | ERR_NESTING);  /* ignore most errors                */
      warning &= WRN_SYM;                   /* ignore most warnings              */
    }

    if (pass == 2)
    {
      outbuffer();
      if ((listing & LIST_ON) &&
          (dwOptions & OPTION_LIS))
        outlist(op, lpLabel);
    }
    else if ((listing & LIST_ON) &&
             (dwOptions & OPTION_LIS) &&
             (dwOptions & OPTION_LP1))
    {
      if (curline->flg & LINCAT_MACEXP ||   /* prevent 2nd listing of macro      */
          !curline->next ||                 /* since this is done in expansion   */
          !(curline->next->flg & LINCAT_MACEXP))
        outlist(op, lpLabel);
    }

    if (error || warning)
      report();
    loccounter += codeptr;
}

/*****************************************************************************/
/* suppressline : suppresses a line                                          */
/*****************************************************************************/

void suppressline()
{
    struct oprecord * op = NULL;

    srcptr = srcline;
    oldlc = loccounter;
    codeptr = 0;
    condline = 0;
    if (nSkipCount > 0)
    {
      nSkipCount--;
      condline = 1;                         /* this is STILL conditional         */
    }
    else
    {
      if (isalnum(*srcptr))
      {
        scanname();
        if (*srcptr == ':')
          srcptr++;
      }
      skipspace();
      scanname();
      op = findop(unamebuf);
      if (op && op->cat == OPCAT_PSEUDO)    /* examine pseudo-ops in detail      */
      {
        if ((op->code == PSEUDO_IF) ||      /* IF variants                       */
            (op->code == PSEUDO_IFN) ||
            (op->code == PSEUDO_IFC) ||
            (op->code == PSEUDO_IFNC) ||
            (op->code == PSEUDO_IFD) ||
            (op->code == PSEUDO_IFND))
        {
          ifcount++;
          condline = 1;                     /* this is a conditional line        */
        }
        else if (op->code == PSEUDO_ENDIF)  /* ENDIF                             */
        {
          if (ifcount > 0)
            ifcount--;
          else if (suppress == 1 || suppress == 2)
            suppress = 0;
          condline = 1;                     /* this is a conditional line        */
        }
        else if (op->code == PSEUDO_ELSE)   /* ELSE                              */
        {
          if (ifcount == 0 && suppress == 2)
            suppress = 0;
          condline = 1;                     /* this is a conditional line        */
        }
      }  
    }

    if (((pass == 2) || (dwOptions & OPTION_LP1)) &&
        (listing & LIST_ON) &&
        (dwOptions & OPTION_LIS))
      outlist(op, NULL);
}

/*****************************************************************************/
/* usage : prints out correct usage                                          */
/*****************************************************************************/

void usage(char *nm)
{
    printf("Usage: %s [-option*] srcname*\n",
           nm ? nm : "a09");
    printf("Available options are:\n");
    printf("-B[objname] ........ output to binary file (default)\n");
    printf("-F[objname] ........ output to FLEX binary file\n");
    /* printf("-G[objname] ........ output Gnu .o format file\n"); */
    printf("-R[objname] ........ output to FLEX relocatable object file\n");
    printf("-S[objname] ........ output to Motorola S51-09 file\n");
    printf("-X[objname] ........ output to Intel Hex file\n");
    printf("-L[listname] ....... create listing file \n");
    printf("-C ................. suppress code output\n");
    printf("-Dsymbol[=value] ... predefines a symbol\n");
    printf("                     for TSC 6809 Assembler compatibility,\n");
    printf("                     you should only use symbols A through C\n");
    printf("-Ooption ........... sets an option (as in OPT pseudoop)\n");
    printf("-W ................. suppress warnings\n");
    printf("srcname ............ source file name(s)\n");

    exit(2);
}

/*****************************************************************************/
/* getoptions : retrieves the options from the passed argument array         */
/*****************************************************************************/

void getoptions (int argc, char* argv[])
{
    int i, j;
    char *ld;

    for (i = 1; i < argc; i++)
    {
    #if !UNIX
      /* code for DOS / Windows / OS2 */
      if ((argv[i][0] == '-') ||
          (argv[i][0] == '/'))
    #else
      /* code for UNIX derivates */
      if (argv[i][0] == '-')
    #endif
      {
        for (j = 1; j < (int)strlen(argv[i]); j++)
        {
          switch (tolower(argv[i][j]))
          {
            case 'c' :                      /* suppress code output              */
              outmode = OUT_NONE;
              break;
            case 'u' :                      /* undefined is treated as external  */
              dwOptions |= OPTION_UEX;
              break;
            case 'b' :                      /* define binary output file         */
            case 's' :                      /* define Motorola output file       */
            case 'x' :                      /* define Intel Hex output file      */
            case 'f' :                      /* define FLEX output file           */
            case 'r' :                      /* define FLEX relocatable output f. */
        /*  case 'g' : */                   /* define GNU output file            */
              strcpy(objname,               /* copy in the name                  */
                      argv[i] + j + 1);
              switch (tolower(argv[i][j]))  /* then set output mode              */
              {
                case 'b' :
                  outmode = OUT_BIN;
                  break;
                case 's' :
                  outmode = OUT_SREC;
                  break;
                case 'x' :
                  outmode = OUT_IHEX;
                  break;
                case 'f' :
                  outmode = OUT_FLEX;
                  break;
    /*
                case 'g' :
                  outmode = OUT_GAS;
                  relocatable = 1;
                  dwOptions |= OPTION_REL;
                  break;
    */
                case 'r' :
                  outmode = OUT_REL;
                  maxidlen = 8;             /* only 8 significant ID chars!      */
                  relocatable = 1;
                  absmode = 0;
                  break;
              }
              j = strlen(argv[i]);          /* advance behind copied name        */
              break;
            case 'l' :                      /* define listing file               */
              strcpy(listname,              /* copy in the name                  */
                      argv[i] + j + 1);
              j = strlen(argv[i]);          /* advance behind copied name        */
              listing = LIST_ON;            /* remember we're listing            */
              break;
            case 'd' :                      /* define a symbol ?                 */
              srcptr = argv[i] + j + 1;     /* parse in the name                 */
              scanname();
              if (!namebuf[0])              /* if no name there                  */
                usage(argv[0]);             /* show usage and get out            */
              if (*srcptr == '=')           /* if followed by value              */
                srcptr++;                   /* advance behind it                 */
              else                          /* otherwise go to ""                */
                srcptr = argv[i] + strlen(argv[i]);
              settext(namebuf, srcptr);     /* define the symbol                 */
              j = strlen(argv[i]);          /* advance behind copied name        */
              break;
            case 'o' :                      /* option                            */
              if (setoption(argv[i] + j + 1))
                usage(argv[0]);
              j = strlen(argv[i]);          /* advance behind option             */
              break;
          }
        }
        for (j = i; j < argc; j++)          /* remove all consumed arguments     */
          argv[j] = argv[j + 1];
        argc--;                             /* reduce # arguments                */
        i--;                                /* and restart with next one         */
      }
    }

    if (argc < 2)                           /* if not at least one filename left */
      usage(argv[0]);                       /* complain & terminate              */

    strcpy(srcname, argv[1]);               /* copy it in.                       */

    if (!objname[0])                        /* if no object name defined         */
    {
      strcpy(objname, srcname);             /* copy in the source name           */
      ld = strrchr(objname, '.');           /* look whether there's a dot in it  */
      if (!ld)                              /* if not                            */
        ld = objname + strlen(objname);     /* append extension                  */
      switch (outmode)                      /* which output mode?                */
      {
        case OUT_BIN :                      /* binary ?                          */
    #if !UNIX
          strcpy(ld, ".bin");               /* DOS / Windows / OS2               */
    #else
          strcpy(ld, ".b");                 /* UNIX                              */
    #endif
          break;
        case OUT_SREC :                     /* Motorola S51-09 ?                 */
          strcpy(ld, ".s09");
          break;
        case OUT_IHEX :                     /* Intel Hex ?                       */
          strcpy(ld, ".hex");
          break;
        case OUT_FLEX :                     /* FLEX binary ?                     */
          strcpy(ld, ".bin");
          break;
        case OUT_GAS :                      /* GNU relocatable object ?          */
          strcpy(ld, ".o");
          break;
        case OUT_REL :                      /* Flex9 relocatable object ?        */
          strcpy(ld, ".rel");
          break;
      }
    }

    if ((listing & LIST_ON) && !listname[0])/* if no list file specified         */
    {
      strcpy(listname, srcname);            /* copy in the source name           */
      ld = strrchr(listname, '.');          /* look whether there's a dot in it  */
      if (!ld)                              /* if not                            */
        ld = listname + strlen(listname);   /* append extension                  */
      strcpy(ld, ".lst");                   /* .lst                              */
    }
}

/*****************************************************************************/
/* processfile : processes the input file                                    */
/*****************************************************************************/

void processfile(struct linebuf *pline)
{
    struct linebuf *plast = pline;

    while (pline)
    {
      if (g_termflg < 0 ||                  /* skip all lines until going up     */
          (pline->lvl & LINCAT_LVLMASK) < g_termflg)
      {
        curline = pline;
        error = ERR_OK;
        warning = WRN_OK;
        expandtext();                       /* expand text symbols               */
        srcptr = srcline;
        if (suppress || nSkipCount)
          suppressline();
        else
        {
          if (nRepNext)
          {
            for (; nRepNext > 0; nRepNext--)
            {
              processline();
              error = ERR_OK;
              warning = WRN_OK;
            }
            nRepNext = 0;
          }
          else
          {
            processline();
            error = ERR_OK;
            warning = WRN_OK;
          }
        }
      }
      plast = pline;
      pline = pline->next;
    }

    if (suppress)
    {
      printf("%s(%ld) : error 18: improperly nested IF statements\n",
             expandfn(plast->fn), plast->ln);
      if (((dwOptions & OPTION_LP1) || pass == 2) &&
          (listing & LIST_ON))
        putlist( "*** Error 18: improperly nested IF statements\n");
      errors++;
      suppress = 0;
    }

    if (common)
    {
      printf("%s(%ld) : error 24: improperly nested COMMON statements\n",
             expandfn(plast->fn), plast->ln);
      if (((dwOptions & OPTION_LP1) || pass == 2) &&
          (listing & LIST_ON))
        putlist( "*** Error 24: improperly nested COMMON statements\n");
      errors++;
      common = 0;
    }
}

/*****************************************************************************/
/* main : the main function                                                  */
/*****************************************************************************/

int main (int argc, char *argv[])
{
    int i;
    struct linebuf *pLastLine = NULL;

    scanoperands = scanoperands09;

    settext("ASM", "A09");                  /* initialize predefined texts       */
    settext("VERSION", VERSNUM);
    settext("PASS", "1");
    settext("FILCHR", RMBDEFCHR);
    setoptiontexts();
    nPredefinedTexts = nTexts;

    getoptions(argc, argv);
    pass = 1;
    loccounter = 0;
    phase = 0;
    error = 0;
    warning = 0;
    nTotErrors = errors = 0;
    nTotWarnings = warnings = 0;
    generating = 0;
    common = 0;
    g_termflg = -1;
    if (!absmode)                           /* in relocating mode                */
      dpsetting = -1;                       /* there IS no Direct Page           */
    if (outmode != OUT_REL)                 /* Undef->Ext only for RELASMB output*/
      dwOptions &= ~OPTION_UEX;

    printf("A09 Assembler V" VERSION "\n");

    if ((listing & LIST_ON) &&
        ((listfile = fopen(listname, "w")) == 0))
    {
      printf("%s(0) : error 19: Cannot open list file %s\n", srcname, listname);
      exit(4);
    }

    for (i = 1; argv[i]; i++)               /* read in all source files          */
      pLastLine = readfile(argv[i], 0, pLastLine);
    if (!rootline)                          /* if no lines in there              */
    {
      printf("%s(0) : error 23: no source lines in file\n", srcname);
      if (((dwOptions & OPTION_LP1) || pass == 2) &&
          (listing & LIST_ON))
        putlist( "*** Error 23: no source lines in file\n");
      exit(4);
    }

                                            /* Pass 1 - parse all symbols        */
    if ((listing & LIST_ON) && (dwOptions & OPTION_LP1))
      putlist( "*** Pass 1 ***\n\n");

    processfile(rootline);
    if (errors)
    {
      printf("%ld error(s) in pass 1\n",errors);
      if ((listing & LIST_ON) && (dwOptions & OPTION_LP1))
        putlist("%s%ld error(s) in pass 1.\n",
                (dwOptions & OPTION_LPA) ? "* " : "", errors);
      nTotErrors = errors;
    }
    if (warnings)
    {
      printf("%ld warning(s) in pass 1\n", warnings);
      if ((listing & LIST_ON) && (dwOptions & OPTION_LP1))
        putlist("%s%ld warning(s) in pass 1.\n",
                (dwOptions & OPTION_LPA) ? "* " : "",
                warnings);
      nTotWarnings = warnings;
    }

                                            /* Pass 2 - generate output          */
    pass = 2;
    settext("PASS", "2");
    loccounter = 0;
    phase = 0;
    errors = 0;
    warnings = 0;
    generating = 0;
    reldataorg = -2;
    reldatasize = 0;
    g_termflg = -1;
    memset(bUsedBytes, 0, sizeof(bUsedBytes));
    for (i = 0; i < symtable.counter; i++)   /* reset all PASSED flags            */
      if (symtable.rec[i].cat != SYMCAT_COMMONDATA)
        symtable.rec[i].u.flags &= ~SYMFLAG_PASSED;
    if (dwOptions & OPTION_UEX)
    {                                     /* make all undefined symbols extern */
      for (i = 0; i < symtable.counter; i++)
        if (symtable.rec[i].cat == SYMCAT_UNRESOLVED)
          symtable.rec[i].cat = SYMCAT_EXTERN;
    }

    if (listing & LIST_ON)
    {
      if ((dwOptions & OPTION_LP1))
      {
        if (dwOptions & OPTION_PAG)
          PageFeed();
        else
          putlist("\n");
        putlist( "*** Pass 2 ***\n\n");
      }
      else if (nTotErrors || nTotWarnings)
        putlist("%s%ld error(s), %ld warning(s) unlisted in pass 1\n",
                (dwOptions & OPTION_LPA) ? "* " : "",
                nTotErrors, nTotWarnings);
    }

    if ((outmode >= OUT_BIN) &&
        ((objfile = fopen(objname,
                         ((outmode != OUT_SREC) && (outmode != OUT_IHEX)) ? "wb" : "w"))
                         == 0))
    {
      printf("%s(0) : error 20: cannot write object file %s\n", srcname, objname);
      if (((dwOptions & OPTION_LP1) || pass == 2) && (listing & LIST_ON))
        putlist( "*** Error 20: cannot write object file %s\n", objname);
      exit(4);
    }

    if (outmode == OUT_REL)                 /* if writing FLEX Relocatable       */
    {
      writerelcommon();                     /* write out common blocks           */
      relhdrfoff = ftell(objfile);
      writerelhdr(0);                       /* write out initial header          */
      relabsfoff = -1;
    }

    processfile(rootline);

    if (errors)
    {
      printf("%ld error(s) in pass 2\n", errors);
      nTotErrors += errors;
    }
    if (warnings)
    {
      printf("%ld warning(s) in pass 2\n", warnings);
      nTotWarnings += warnings;
    }

    if (listing & LIST_ON)
    {
      if (errors || warnings)
        putlist("\n");
      if (errors)
        putlist("%s%ld error(s) in pass 2.\n",
                (dwOptions & OPTION_LPA) ? "* " : "", errors);
      if (warnings)
        putlist("%s%ld warning(s) in pass 2.\n",
                (dwOptions & OPTION_LPA) ? "* " : "", warnings);
      if (dwOptions & OPTION_SYM)
        outsymtable();
      if ((relocatable) && (dwOptions & OPTION_REL))
        outreltable();
      if ((dwOptions & OPTION_TXT) &&
          (nPredefinedTexts < nTexts))
        outtexttable();

      putlist( "\n%s%ld error(s), %ld warning(s)\n",
              (dwOptions & OPTION_LPA) ? "* " : "", nTotErrors, nTotWarnings);
      fclose(listfile);
    }
    else
      printf("Last assembled address: %04X\n", loccounter - 1);

    switch (outmode)                        /* look whether object cleanup needed*/
    {
      case OUT_SREC :                       /* Motorola S51-09                   */
        flushhex();
        chksum = (tfradr & 0xff) + ((tfradr >> 8) & 0xff) + 3;
        fprintf(objfile, "S903%04X%02X\n", tfradr, 0xff - (chksum & 0xff));
        break;
      case OUT_IHEX :                       /* Intel Hex                         */
        flushihex();
    #if 0
        /* put transfer address in here... unfortunately, the official
           Intel documentation doesn't allow this mechanism */
        chksum = ((tfradr >> 8) & 0xff) + (tfradr & 0xff) + 1;
        fprintf(objfile, ":00%04X01%02X\n", tfradr, (-(signed)chksum) & 0xff);
    #else
        fprintf(objfile, ":00000001FF\n");
    #endif
        break;
      case OUT_FLEX :                       /* FLEX binary                       */
        flushflex();
        if (tfradrset)                      /* if transfer address set           */
        {                                 /* write out transfer address block  */
          fputc(0x16, objfile);
          fputc((tfradr >> 8) & 0xff, objfile);
          fputc(tfradr & 0xff, objfile);
        }
        break;
      case OUT_REL :                        /* FLEX Relocatable                  */

        if (absmode)                        /* if producing an absolute module   */
        {                                 /* correct the last object record    */
          long curoff = ftell(objfile);
          if (relabsfoff >= 0)
          {
            long blockcnt = curoff - relabsfoff - 4;
            fseek(objfile, relabsfoff, SEEK_SET);
            fputc((unsigned char)(blockcnt >> 8), objfile);
            fputc((unsigned char)(blockcnt & 0xFF), objfile);
            fseek(objfile, curoff, SEEK_SET);
          }
          /* relabsfoff = curoff; */
        }

        writerelext();                      /* write out External table          */
        writerelglobal();                   /* write out Global table            */
        writerelmodname();                  /* write out Module Name             */

        i = (int)ftell(objfile);            /* fill last sector with zeroes      */
        while (i % 252)
        {
          fputc(0, objfile);
          i++;
        }
                                            /* reposition on header              */
        fseek(objfile, relhdrfoff, SEEK_SET);
        writerelhdr(0);                     /* rewrite completed header          */
        break;
    }

    if (objfile)
      fclose(objfile);

    if (errors && outmode >= OUT_BIN)
      unlink(objname);

    return (errors) ? 1 : 0;
}
