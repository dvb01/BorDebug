#ifndef BORDEBUG_H
#define BORDEBUG_H

#ifdef  __cplusplus
extern  "C"
{
#endif


//---------------------------------------------------------------------

/*
    General information

    This set of API's facilitates the reading of debug information
    in *.TDS, *.EXE, and *.DLL files, as produced by the Borland
    32 bit compilers.

*/

//---------------------------------------------------------------------


/*

    Some general explanations


    When the linker emits address information about  a symbol,
    it is done in a segment: offset format.  The segment is a
    logical segment index assigned by the linker and the offset
    is the  offset from the beginning of the  logical segment.
    The physical address is assigned by the operating system
    when the program is loaded.



    You can go from segment:offset address in the debug info
    to actual physical address during debugging in the
    following way:

    - find the base address of the loaded executable
    - find the segment and offset of the piece of debug
      info you are interested in.  This can be a procedure,
      thunk, data, etc.
    - crack the pe-header, and find out the base offset
      of the segment you are interested in
    - Now add the base address of the executable, the
      base offset of the segment, and the offset, and
      you will have the physical address where the item
      you are interested in is loaded at runtime.




    In the following API's, if an argument is a pointer,
    the caller is responsible for making sure that pointer
    is pointing to a valid memory address.
    These API's make no effort to check for valid pointers.


    When you get a type index, you can use BorDebugTypeFromIndex, and
    BorDebugTYPEXXXX API's to receive more information about the type.
    In certain cases it is useful to call BorDebugTypeIndexToString
    to get a string representing the name of the type.

    When you get a name index, you can use BorDebugNameIndexToName
    or BorDebugNameIndexToUnMangledName to get the actual name as
    a string.

    When you get a reg index, you can use BorDebugRegIndexToName to
    get the actual name of the register.

    When you get a browser offset, you can use the browser
    info API's to get more information.


*/


//---------------------------------------------------------------------


/*

    How to proceed reading the debug information:

    Step 1:
    Register the file. This should be a .tds, .exe, or .dll file
    that contains debug info. If the file registration fails, you
    might have a file with no debug info.

        BorDebugRegisterFile
        BorDebugUnregisterFile


    Step 2:
    Get the details about the subsections for each subsection.
    Then proceed depending on the subsection you are interested in.

        BorDebugSubSectionDirOffset
        BorDebugSubSectionCount
        BorDebugSubSection


    Step 3:
    Use the sstModule API's to get details of sstModule subsections. You
    find details here about source names, and segment ranges.

        BorDebugModule
        BorDebugModuleSegment


    Step 4:
    Go through the sstAlign symbols, which are the local block symbols.
    For each symbol you get a symbol kind, offset, and length, and you
    can use the symbol kind to determine the correct BorDebugSymbolXXXX
    API to use.

        BorDebugStartSymbols
        BorDebugNextSymbol
        BorDebugSymbolXXXX functions

    Step 5:
    Use the sstSrcModule API's to get all the details of sstSrcModule
    subSections.  Here you will find the source names, the address
    ranges of the code, and the line number information

        BorDebugSrcModule
        BorDebugSrcModuleRanges
        BorDebugSrcModuleSources
        BorDebugSrcModuleSourceRanges
        BorDebugSrcModuleLineNumbers

    Step 6:
    Get the details of the sstGlobalSym and/or sstGlobalPub subSections.
    After that you can go through the symbols, and for each symbol you
    get a symbol kind, offset, and length, and you can use the symbol
    kind to determine the correct BorDebugSymbolXXXX API to use.

        BorDebugGlobalSym
        BorDebugSymbolXXXX functions

    Step 7:
    To find out about types, get the details of the sstGlobalTypes section.
    You can access each type by its 0x1000-based index number.

        BorDebugGlobalTypes
        BorDebugTypeFromIndex
        BorDebugTypeFromOffset
        BorDebugTypeXXXX functions
        BorDebugTypeIndexToString

    Step 8:
    Find out how many names there are in the sstNames section.
    You can go from a 1-based name index to the actual name by
    using the name index converting functions.

        BorDebugNamesTotalNames
        BorDebugNameIndexToUnmangledName
        BorDebugNameIndexToName
        BorDebugRegIndexToName

*/


//---------------------------------------------------------------------


/*

    The different kind of subSection types these API's recognize

*/

enum
{
    BORDEBUG_SSTMODULE      = 0x00000120,
    BORDEBUG_SSTALIGNSYM    = 0x00000125,
    BORDEBUG_SSTSRCMODULE   = 0x00000127,
    BORDEBUG_SSTGLOBALSYM   = 0x00000129,
    BORDEBUG_SSTGLOBALPUB   = 0x0000012A,   // Treated as sstGlobalSym
    BORDEBUG_SSTGLOBALTYPES = 0x0000012B,
    BORDEBUG_SSTNAMES       = 0x00000130,
    BORDEBUG_SSTBROWSE      = 0x00000131,

    BORDEBUG_SSTINVALID     = 0xFFFFFFFF,
};


//---------------------------------------------------------------------


/*

    The different kinds of symbols

*/

enum
{
    BORDEBUG_S_COMPILE      = 0x00000001,
    BORDEBUG_S_REGISTER     = 0x00000002,
    BORDEBUG_S_CONST        = 0x00000003,
    BORDEBUG_S_UDT          = 0x00000004,
    BORDEBUG_S_SSEARCH      = 0x00000005,
    BORDEBUG_S_END          = 0x00000006,
    BORDEBUG_S_SKIP         = 0x00000007,
    BORDEBUG_S_CVRESERVE    = 0x00000008,
    BORDEBUG_S_OBJNAME      = 0x00000009,

    BORDEBUG_S_GPROCREF     = 0x00000020,
    BORDEBUG_S_GDATAREF     = 0x00000021,
    BORDEBUG_S_EDATA        = 0x00000022,
    BORDEBUG_S_EPROC        = 0x00000023,
    BORDEBUG_S_USES         = 0x00000024,
    BORDEBUG_S_NAMESPACE    = 0x00000025,
    BORDEBUG_S_USING        = 0x00000026,
    BORDEBUG_S_PCONSTANT    = 0x00000027,

    BORDEBUG_S_BPREL32      = 0x00000200,
    BORDEBUG_S_LDATA32      = 0x00000201,
    BORDEBUG_S_GDATA32      = 0x00000202,
    BORDEBUG_S_PUB32        = 0x00000203,
    BORDEBUG_S_LPROC32      = 0x00000204,
    BORDEBUG_S_GPROC32      = 0x00000205,
    BORDEBUG_S_THUNK32      = 0x00000206,
    BORDEBUG_S_BLOCK32      = 0x00000207,
    BORDEBUG_S_WITH32       = 0x00000208,
    BORDEBUG_S_LABEL32      = 0x00000209,
    BORDEBUG_S_CEXMODEL32   = 0x0000020A,
    BORDEBUG_S_VFTPATH32    = 0x0000020B,
    BORDEBUG_S_ENTRY32      = 0x00000210,
    BORDEBUG_S_OPTVAR32     = 0x00000211,
    BORDEBUG_S_PROCRET32    = 0x00000212,
    BORDEBUG_S_SAVREGS32    = 0x00000213,

    BORDEBUG_S_SLINK32      = 0x00000230,

    BORDEBUG_S_INVALID      = 0xFFFFFFFF,
};


//---------------------------------------------------------------------


/*

    The different kind of types

*/


enum    BORDEBUG_leaf000s
{
    BORDEBUG_LF_STARTYP     = 0x00000000,
    BORDEBUG_LF_MODIFIER    = 0x00000001,
    BORDEBUG_LF_POINTER     = 0x00000002,
    BORDEBUG_LF_ARRAY       = 0x00000003,
    BORDEBUG_LF_CLASS       = 0x00000004,
    BORDEBUG_LF_STRUCT      = 0x00000005,
    BORDEBUG_LF_UNION       = 0x00000006,
    BORDEBUG_LF_ENUM        = 0x00000007,
    BORDEBUG_LF_PROCEDURE   = 0x00000008,
    BORDEBUG_LF_MFUNCTION   = 0x00000009,
    BORDEBUG_LF_VTSHAPE     = 0x0000000A,
    BORDEBUG_LF_COBOL0      = 0x0000000B,
    BORDEBUG_LF_COBOL1      = 0x0000000C,
    BORDEBUG_LF_BARRAY      = 0x0000000D,
    BORDEBUG_LF_LABEL       = 0x0000000E,
    BORDEBUG_LF_NULL        = 0x0000000F,
    BORDEBUG_LF_NOTTRANS    = 0x00000010,
    BORDEBUG_LF_DIMARRAY    = 0x00000011,
    BORDEBUG_LF_VFTPATH     = 0x00000012,
    BORDEBUG_LF_SET         = 0x00000030,
    BORDEBUG_LF_SUBRANGE    = 0x00000031,
    BORDEBUG_LF_PARRAY      = 0x00000032,
    BORDEBUG_LF_PSTRING     = 0x00000033,
    BORDEBUG_LF_CLOSURE     = 0x00000034,
    BORDEBUG_LF_PROPERTY    = 0x00000035,
    BORDEBUG_LF_LSTRING     = 0x00000036,
    BORDEBUG_LF_VARIANT     = 0x00000037,
    BORDEBUG_LF_CLASSREF    = 0x00000038,
    BORDEBUG_LF_WSTRING     = 0x00000039,
    BORDEBUG_LF_UNKNOWN     = 0x000000EF,

    BORDEBUG_LF_INVALID_0   = 0xFFFFFFF0,
};

enum    BORDEBUG_leaf200s
{
    BORDEBUG_LF_SKIP        = 0x00000200,
    BORDEBUG_LF_ARGLIST     = 0x00000201,
    BORDEBUG_LF_DEFARG      = 0x00000202,
    BORDEBUG_LF_LIST        = 0x00000203,
    BORDEBUG_LF_FIELDLIST   = 0x00000204,
    BORDEBUG_LF_DERIVED     = 0x00000205,
    BORDEBUG_LF_BITFIELD    = 0x00000206,
    BORDEBUG_LF_METHODLIST  = 0x00000207,
    BORDEBUG_LF_DIMCONU     = 0x00000208,
    BORDEBUG_LF_DIMCONLU    = 0x00000209,
    BORDEBUG_LF_DIMVARU     = 0x0000020A,
    BORDEBUG_LF_DIMVARLU    = 0x0000020B,
    BORDEBUG_LF_REFSYM      = 0x0000020C,

    BORDEBUG_LF_INVALID_2   = 0xFFFFFFF2,
};

enum    BORDEBUG_leaf400s
{
    BORDEBUG_LF_BCLASS      = 0x00000400,
    BORDEBUG_LF_VBCLASS     = 0x00000401,
    BORDEBUG_LF_IVBCLASS    = 0x00000402,
    BORDEBUG_LF_ENUMERATE   = 0x00000403,
    BORDEBUG_LF_FRIENDFCN   = 0x00000404,
    BORDEBUG_LF_INDEX       = 0x00000405,
    BORDEBUG_LF_MEMBER      = 0x00000406,
    BORDEBUG_LF_STMEMBER    = 0x00000407,
    BORDEBUG_LF_METHOD      = 0x00000408,
    BORDEBUG_LF_NESTTYPE    = 0x00000409,
    BORDEBUG_LF_VFUNCTAB    = 0x0000040A,
    BORDEBUG_LF_FRIENDCLS   = 0x0000040B,

    BORDEBUG_LF_INVALID_4   = 0xFFFFFFF4,
};

enum    BORDEBUG_leaf800s
{
    BORDEBUG_LF_CHAR        = 0x00008000,
    BORDEBUG_LF_SHORT       = 0x00008001,
    BORDEBUG_LF_USHORT      = 0x00008002,
    BORDEBUG_LF_LONG        = 0x00008003,
    BORDEBUG_LF_ULONG       = 0x00008004,
    BORDEBUG_LF_REAL32      = 0x00008005,
    BORDEBUG_LF_REAL64      = 0x00008006,
    BORDEBUG_LF_REAL80      = 0x00008007,
    BORDEBUG_LF_REAL128     = 0x00008008,
    BORDEBUG_LF_QUADWORD    = 0x00008009,
    BORDEBUG_LF_UQUADWORD   = 0x0000800A,
    BORDEBUG_LF_REAL48      = 0x0000800B,

    BORDEBUG_LF_INVALID_8   = 0xFFFFFFF8,
};

enum    BORDEBUG_leaf900s
{
    BORDEBUG_LF_RAWBITS     = 0x00009000,
    BORDEBUG_LF_INVALID_9   = 0xFFFFFFF9,
};


//---------------------------------------------------------------------


/*

    BorDebugRegisterFile


    Register a file with debug info


    This should be a *.DLL, *.EXE, or *.TDS file.


    This function simply tries to open the file using "fileName".
    No attempts are made to look in the system PATH, or any other
    predefined PATH's.  If the file is not in the current directory,
    you have to supply a fully qualified file name.


    You should also make sure the filename has the proper extension,
    since that is how this routine figures out what kind of file
    you are trying to open.


    The sstNames subsection has no index table, it is basically a
    series of zero terminated names preceded by a length byte.

    To allow some flexibility here, there are 3 possibilities:

    - set skipNames to non-zero.  This means the sstNames section
      is not scanned, and no name index table is build, and no
      names are cached at all.  This also means that you can't
      go from a name index to a string representation.  You will
      always get a blank string back from the nameIndex conversion
      routines. This option saves space and time, at the expense
      of no names.

    - set skipNames to zero, and set cacheNames to zero.  This
      means that the sstNames section is scanned and an index
      table is build. The sstNames section is not cached though
      so each lookup of a name will cause a SEEK and a READ
      from the file, which is slow.  This option costs some
      time to build the index table, and takes up space for
      the index table, and name accesses are slow.  It does
      save space because the sstNames section is not cached.

    - set skipNames to zero, and set cacheNames to non-zero.
      The name index table is build and the whole sstNames
      section stays also in memory.  Name accesses are very
      fast, but this is at the expense of the memory the
      sstNames takes up.


    If you want you can cache your own names.  You can get the
    starting offset and size of the sstNames section from
    BorDebugSubSection.  The first 4 bytes hold the total
    number of names, and the rest of the sstNames section is
    composed of zero terminated names preceded by a length
    byte.  Based on this you can set up your own name cache.
    Remember that name indices are 1-based.


    fileName:   the name of the file with debug info
    skipNames:  if non-zero, don't do anything with the sstNames
    cacheNames: if non-zero, cache complete sstNames section
    failure:    will be set on failure


    Returns a "registerCookie" which can be used in all the
    API's described below.  On success, the return value will
    be non-zero. On failure, the return value will be zero,
    and the failure variable will be set to an error code:

    0:  everything is fine and dandy
    1:  extension not recognized
    2:  cannot open the file
    3:  no debug info in the file
    4:  failures while reading from the file
    5:  out of memory

*/

typedef void * BorDebugCookie;


BorDebugCookie  BorDebugRegisterFile(const char   * fileName,
                                     unsigned int   skipNames,
                                     unsigned int   cacheNames,
                                     unsigned int * failure);


/*

    BorDebugUnregisterFile


    Unregister a file that was previously registered.
    This will free up any memory used.

*/


void    BorDebugUnregisterFile(BorDebugCookie registerCookie);




//---------------------------------------------------------------------

/*

    General subsection API's:

*/


/*

    BorDebugSubSectionDirOffset


    File offset in bytes of the SubSection Dir

    return: starting offset of the subSection dir

*/

unsigned int    BorDebugSubSectionDirOffset(BorDebugCookie registerCookie);



/*

    BorDebugSubSectionCount


    How many subsections are there

    return:  number of subsections

*/

unsigned int    BorDebugSubSectionCount(BorDebugCookie registerCookie);



/*

    BorDebugSubSection


    Details of a subsection

    subSectionNo:   zero-based subsection number
    subSectionType: type of subsection as in BORDEBUG_SSTXXXX
    module:         the module the subsection belongs to
    offset:         file offset in bytes of subsection
    size:           size of subsection in bytes

*/

void    BorDebugSubSection(BorDebugCookie registerCookie,
                           unsigned int   subSectionNo,
                           unsigned int * subSectionType,
                           unsigned int * module,
                           unsigned int * offset,
                           unsigned int * size);



//---------------------------------------------------------------------

/*

    sstModule API's:

*/

/*

    BorDebugModule


    Details of a sstModule subsection

    offset:         file offset of subsection as received in BorDebugSubSection
                    assumes this subsection is of sstModule type
    overlay:        overlay number
    libIndex:       index into sstLibrary, which we don't use
    style:          debugging style
    name:           name index
    timeStamp:      time stamp of the module
    segmentCount:   number of segments

*/

void    BorDebugModule(BorDebugCookie registerCookie,
                       unsigned int   offset,
                       unsigned int * overlay,
                       unsigned int * libIndex,
                       unsigned int * style,
                       unsigned int * name,
                       unsigned int * timeStamp,
                       unsigned int * segmentCount);




/*

    BorDebugModuleSegment


    Details of a module segment

    moduleOffset:   file offset of subsection as received in BorDebugSubSection
                    assumes this subsection is of sstModule type
    segmentNo:      zero-based segment number
    segment:        linker segment
    offset:         File offset of segment in bytes
    size:           size of the segment in bytes
    flags:          flags of the segment
                    0 = data segment
                    1 = code segment

*/

void    BorDebugModuleSegment(BorDebugCookie registerCookie,
                              unsigned int   moduleOffset,
                              unsigned int   segmentNo,
                              unsigned int * segment,
                              unsigned int * offset,
                              unsigned int * size,
                              unsigned int * flags);



//---------------------------------------------------------------------

/*

    General Symbol subsection API's.

    Used for sstAlignSyms, sstGlobalSyms, and sstGlobalPubs

    The symbols in these Symbol sections are only accessible in
    serial mode.  To go from one symbol to the next requires reading
    the length of a symbol and forwarding the file pointer to beyond
    that symbol.

    The API's supplied here are the most basic way to access serial
    data.  You can build your own routines to get the total count
    of the symbols, and to access symbols by index.

*/


/*

    BorDebugStartSymbols


    Start iterating through the Symbols in the subsection

    subSectionType: type of subsection as received in BorDebugSubSection
                    assumes this subsection is sstAlign, sstGlobalSym,
                    or sstGlobalPub.
    offset:         file offset of subsection as received in BorDebugSubSection
                    assumes this subsection is sstAlign, sstGlobalSym,
                    or sstGlobalPub
    size:           size of subsection as received in BorDebugSubSection

*/

void    BorDebugStartSymbols(BorDebugCookie registerCookie,
                             unsigned int   subSectionType,
                             unsigned int   offset,
                             unsigned int   size);

/*

    BorDebugNextSymbol


    Next symbol in the Symbol subsection, 0, 0, 0 if at end

    kind:           what kind of symbol is it
    symOffset:      starting offset of the symbol,
                    which can be used in the symbol API's below
    symLen:         length of the symbol in bytes

*/

void    BorDebugNextSymbol(BorDebugCookie registerCookie,
                           unsigned int * kind,
                           unsigned int * symOffset,
                           unsigned int * symLen);


//---------------------------------------------------------------------

/*

    Browser API's

*/

/*

    BorDebugDumpBrowserInfo


    To be determined

*/

void    BorDebugDumpBrowserInfo(BorDebugCookie registerCookie,
                                unsigned int   browserOffset);

//---------------------------------------------------------------------

/*

    symbol API's for sstAlignSym, sstGlobalSym, and sstGlobalPub

*/


/*

    Details of BORDEBUG_S_COMPILE = 0x00000001


    BorDebugSymbolCOMPILE


    Compile flags

    This symbol communicates to Code View compile time information on a
    per module basis, such as the language and version number of the
    compiler, the ambient model for code and data, and the target
    processor.

    symOffset: file offset of the symbol in bytes
    machine:
            0x00    Intel 8080
            0x01    Intel 8086
            0x02    Intel 80286
            0x03    Intel 80386
            0x04    Intel 80486
            0x05    Intel 80586
            0x06    Intel 80686
    language:
            0       C
            1       C++
            2       FORTRAN
            3       Tasm
            4       Pascal
            5       Basic
            6       COBOL
            7-255   Reserved

    flags:
            PCodePresent    :0
            FloatPrecision  :1-2
                    0       fast floating point
                    1       ANSI compatible slow floating point
            FloatPackage    :3-4
                    0       Hardware processor (80x87)
                    1       Emulator
                    2       Altmath
                    3       Reserved
            AmbientData     :5-7
                    0       Near
                    1       Far
                    2       Huge
                    3-7     Reserved
            AmbientCode     :8-10
                    0       Near
                    1       Far
                    2       Huge
                    3-7     Reserved
            Mode32          :11     Compiled for 32 bit addresses
            chsign          :12     True is 'char' is a signed type
            Reserved        :13-31
    compilerName:   points to memory to receive null-terminated name
    maxNameCount:   size of compilerName array. The max name length
                    is 255 char's, so you need at least 256 char's

*/

void    BorDebugSymbolCOMPILE(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * machine,
                              unsigned int * language,
                              unsigned int * flags,
                              char         * compilerName,
                              unsigned int   maxNameCount);




/*

    Details of BORDEBUG_S_REGISTER = 0x00000002


    BorDebugSymbolREGISTER


    Register variable

    symOffset:      file offset of the symbol in bytes
    typeIndex:      type index of the symbol
    reg:            register index
    name:           name index
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolREGISTER(BorDebugCookie registerCookie,
                               unsigned int   symOffset,
                               unsigned int * typeIndex,
                               unsigned int * reg,
                               unsigned int * name,
                               unsigned int * browserOffset);

/*


    Details of BORDEBUG_S_CONST = 0x00000003


    BorDebugSymbolCONST


    Constants and enums. If enum, the type index refers to the
    containing enum

    symOffset:      file offset of the symbol in bytes
    typeIndex:      type index of the symbol
    name:           name index
    browserOffset:  offset of browser info
    value:          what is the constant value

*/

void    BorDebugSymbolCONST(BorDebugCookie registerCookie,
                            unsigned int   symOffset,
                            unsigned int * typeIndex,
                            unsigned int * name,
                            unsigned int * browserOffset,
                            unsigned int * value);

/*

    Details of BORDEBUG_S_UDT = 0x00000004


    BorDebugSymbolUDT


    User defined type

    symOffset:      file offset of the symbol in bytes
    typeIndex:      type index of the symbol
    properties:     zero bit on means it's a tag, otherwise a typedef
                    one  bit on means it's nested, otherwise not
    name:           name index
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolUDT(BorDebugCookie registerCookie,
                          unsigned int   symOffset,
                          unsigned int * typeIndex,
                          unsigned int * properties,
                          unsigned int * name,
                          unsigned int * browserOffset);

/*

    Details of BORDEBUG_S_SSEARCH = 0x00000005


    BorDebugSymbolSSEARCH


    Start search

    symOffset:        file offset of the symbol in bytes
    firstProcSegment: segment of first proc. symbol
    firstProcOffset:  offset from symOffset of first proc. symbol
    codeSymCount:     how many code symbols
    dataSymCount:     how many data symbols
    firstData:        offset from symOffset of first data symbol

*/

void    BorDebugSymbolSSEARCH(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * firstProcSegment,
                              unsigned int * firstProcOffset,
                              unsigned int * codeSymCount,
                              unsigned int * dataSymCount,
                              unsigned int * firstData);

/*

    Details of BORDEBUG_S_END = 0x00000006

    End of block, procedure, with, or thunk

    Nothing needed here

*/


/*

    Details of BORDEBUG_S_SKIP = 0x00000007

    Reserve space

    Not used

*/


/*

    Details of BORDEBUG_S_CVRESERVE = 0x00000008

    Code view internal use

    Not used

*/

/*

    Details of BORDEBUG_S_OBJNAME = 0x00000009


    BorDebugSymbolOBJNAME


    Name of object file

    symOffset:  file offset of the symbol in bytes
    signature:  optional signature (not used)
    name:       name index

*/

void    BorDebugSymbolOBJNAME(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * signature,
                              unsigned int * name);


/*

    Details of BORDEBUG_S_GPROCREF = 0x00000020


    BorDebugSymbolGPROCREF


    Global procedure reference

    symOffset:      file offset of the symbol in bytes
    refSymOffset:   file offset of symbol this one refers to
    typeIndex:      type index of symbol this one refers to
    name:           name index
    browserOffset:  offset of browser info
    codeSegment:    segment of the symbol
    codeOffset:     offset of the symbol
        If code segment == 0xFFFF, the symbol lives in
        another DLL or EXE, and codeOffset is the
        nameIndex of that DLL or EXE

*/

void    BorDebugSymbolGPROCREF(BorDebugCookie registerCookie,
                               unsigned int   symOffset,
                               unsigned int * refSymOffset,
                               unsigned int * typeIndex,
                               unsigned int * name,
                               unsigned int * browserOffset,
                               unsigned int * codeSegment,
                               unsigned int * codeOffset);


/*

    Details of BORDEBUG_S_GDATAREF = 0x00000021


    BorDebugSymbolGDATAREF


    Global data reference

    symOffset:      file offset of the symbol in bytes
    refSymOffset:   file offset of symbol this one refers to
    typeIndex:      type index of symbol this one refers to
    name:           name index
    browserOffset:  offset of browser info
    dataSegment:    segment of the symbol
    dataOffset:     offset of the symbol
        If datasegment == 0xFFFF, the symbol lives in
        another DLL or EXE, and dataOffset is the
        nameIndex of that DLL or EXE

*/

void    BorDebugSymbolGDATAREF(BorDebugCookie registerCookie,
                               unsigned int   symOffset,
                               unsigned int * refSymOffset,
                               unsigned int * typeIndex,
                               unsigned int * name,
                               unsigned int * browserOffset,
                               unsigned int * dataSegment,
                               unsigned int * dataOffset);

/*

    Details of BORDEBUG_S_EDATA = 0x00000022


    BorDebugSymbolEDATA


    External data

    symOffset:      file offset of the symbol in bytes
    typeIndex:      type index of the symbol
    name:           name index
    externIndex:    index of the corresponding external
    flags:          1 == TLS var.
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolEDATA(BorDebugCookie registerCookie,
                            unsigned int   symOffset,
                            unsigned int * typeIndex,
                            unsigned int * name,
                            unsigned int * externIndex,
                            unsigned int * flags,
                            unsigned int * browserOffset);

/*

    Details of BORDEBUG_S_EPROC = 0x00000023


    BorDebugSymbolEPROC


    External procedure

    symOffset:      file offset of the symbol in bytes
    typeIndex:      type index of the procedure
    name:           name index of the procedure
    externIndex:    index of the corresponding external
    flags:          not used yet
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolEPROC(BorDebugCookie registerCookie,
                            unsigned int   symOffset,
                            unsigned int * typeIndex,
                            unsigned int * name,
                            unsigned int * externIndex,
                            unsigned int * flags,
                            unsigned int * browserOffset);

/*

    Details of BORDEBUG_S_USES = 0x00000024


    BorDebugSymbolUSES


    Pascal uses

    symOffset:      file offset of the symbol in bytes
    nameCount:      max. number of names to return
    nameIndices:    allocated array for name indices
    returnvalue:    total number of uses names

    This API has to be called twice:
    First call with correct symOffset, set
    nameCount and nameIndices to 0, and the
    total number of uses names is returned.
    Next, make sure you allocate enough space for
    the array of name indices and call again with
    the symOffset, the number of name indices that
    can fit in the array and the pointer to the array.
    On return the array will be filled in.

*/
unsigned int    BorDebugSymbolUSES(BorDebugCookie registerCookie,
                                   unsigned int   symOffset,
                                   unsigned int   nameCount,
                                   unsigned int * nameIndices);

/*

    Details of BORDEBUG_S_NAMESPACE = 0x00000025


    BorDebugSymbolNAMESPACE


    Namespace

    symOffset:      file offset of the symbol in bytes
    name:           name index
    browserOffset:  offset of browser info
    usingCount:     max. number of using names to return
    usingIndices:   allocated array for using name indices
    returnvalue:    total number of using names

    This API has to be called twice:
    First call with correct symOffset, and set usingCount,
    name, browserOffset, and usingIndices to zero and
    the total number of using names is returned.
    Next, make sure you allocate enough space for the array
    of using name indices, set usingCount to the maximum
    number of indices you want, and point name and browserOffset
    to valid memory.  On return the array and other information
    will be filled in.

*/

unsigned int    BorDebugSymbolNAMESPACE(BorDebugCookie registerCookie,
                                        unsigned int   symOffset,
                                        unsigned int   usingCount,
                                        unsigned int * name,
                                        unsigned int * browserOffset,
                                        unsigned int * usingIndices);

/*

    Details of BORDEBUG_S_USING = 0x00000026


    BorDebugSymbolUSING


    Using namespace

    symOffset:      file offset of the symbol in bytes
    nameCount:      max. number of names to return
    nameIndices:    allocated array for name indices
    returnvalue:    total number of using names

    This API has to be called twice:
    First call with correct symOffset, set
    nameCount and nameIndices to 0, and the
    total number of using names is returned.
    Next, make sure you allocate enough space for
    the array of name indices and call again with
    the symOffset, the number of name indices that
    can fit in the array and the pointer to the array.
    On return the array will be filled in.

*/

unsigned int    BorDebugSymbolUSING(BorDebugCookie registerCookie,
                                    unsigned int   symOffset,
                                    unsigned int   nameCount,
                                    unsigned int * nameIndices);

/*

    Details of BORDEBUG_S_PCONSTANT = 0x00000027


    BorDebugSymbolPCONSTANT


    Pascal constant

    symOffset:      file offset of the symbol in bytes
    typeIndex:      type index of the constant
    name:           name index of the constant
    properties:     0x10 == exported
    browserOffset:  offset of browser info
    valueMaxLen:    total length of value array
    value:          array to hold values

    Value should point to an array of char's to hold the string
    representation of the value of the constant.  Make sure that
    valueMaxLen holds the max. number of char's possible in value,
    including any string zero terminators.  A good valueMaxLen to
    start with is 256.
    Returns the total length of the const. value.  If the returned
    value is more than the valueMaxLen provided, the value was
    truncated.  In that case you have to call in again with a bigger
    buffer.

*/

unsigned int    BorDebugSymbolPCONSTANT(BorDebugCookie  registerCookie,
                                        unsigned int    symOffset,
                                        unsigned int  * typeIndex,
                                        unsigned int  * name,
                                        unsigned int  * properties,
                                        unsigned int  * browserOffset,
                                        unsigned int    valueMaxLen,
                                        unsigned char * value);



/*

    Details of BORDEBUG_S_BPREL32 = 0x00000200


    BorDebugSymbolBPREL32


    BP relative stack entry

    symOffset:      file offset of the symbol in bytes
    offset:         how far removed from EBP
    typeIndex:      type index of the local
    name:           name index of the local
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolBPREL32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * offset,
                              unsigned int * typeIndex,
                              unsigned int * name,
                              unsigned int * browserOffset);



/*

    Details of BORDEBUG_S_LDATA32 = 0x00000201


    BorDebugSymbolLDATA32


    Local data

    symOffset:      file offset of the symbol in bytes
    offset:         offset of address of data
    segment:        segment of address of data
    flags:          0x1 == TLS data
    typeIndex:      type index of the symbol
    name:           name index of the symbol
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolLDATA32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * offset,
                              unsigned int * segment,
                              unsigned int * flags,
                              unsigned int * typeIndex,
                              unsigned int * name,
                              unsigned int * browserOffset);




/*

    Details of BORDEBUG_S_GDATA32 = 0x00000202


    BorDebugSymbolGDATA32


    Global data

    symOffset:      file offset of the symbol in bytes
    offset:         offset of address of data
    segment:        segment of address of data
    flags:          0x1 == TLS data
    typeIndex:      type index of the symbol
    name:           name index of the symbol
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolGDATA32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * offset,
                              unsigned int * segment,
                              unsigned int * flags,
                              unsigned int * typeIndex,
                              unsigned int * name,
                              unsigned int * browserOffset);



/*

    Details of BORDEBUG_S_PUB32 = 0x00000203


    BorDebugSymbolPUB32


    Public symbol

    symOffset:      file offset of the symbol in bytes
    offset:         offset of address of data
    segment:        segment of address of data
    flags:          0x1 == TLS data
    typeIndex:      type index of the symbol
    name:           name index of the symbol
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolPUB32(BorDebugCookie registerCookie,
                            unsigned int   symOffset,
                            unsigned int * offset,
                            unsigned int * segment,
                            unsigned int * flags,
                            unsigned int * typeIndex,
                            unsigned int * name,
                            unsigned int * browserOffset);



/*

    Details of BORDEBUG_S_LPROC32 = 0x00000204


    BorDebugSymbolLPROC32


    Local procedure

    symOffset:      file offset of the symbol in bytes
    parent:         address of parent of this procedure as
                    measured from the start of the subsection
    end:            address of the end of the debug info of
                    this procedure (and all its locals) as
                    measured from the start of the subsection
    next:           address of the debug info of the next
                    procedure as measured from the start of
                    the subsection
    codeLength:     length in bytes of the code in the procedure
    debugStart:     start of the debug info from "offset"
    debugEnd:       end of the debug info from "offset"
    offset:         starting address of the code of the procedure
    segment:        segment of address the code of the procedure
    flags:          looks like it's always zero for now
    typeIndex:      type index of the procedure
    name:           name index of the procedure
    browserOffset:  offset of browser info

*/

void    BorDebugSymbolLPROC32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * parent,
                              unsigned int * end,
                              unsigned int * next,
                              unsigned int * codeLength,
                              unsigned int * debugStart,
                              unsigned int * debugEnd,
                              unsigned int * offset,
                              unsigned int * segment,
                              unsigned int * flags,
                              unsigned int * typeIndex,
                              unsigned int * name,
                              unsigned int * browserOffset);



/*

    Details of BORDEBUG_S_GPROC32 = 0x00000205


    BorDebugSymbolGPROC32


    Global procedure


    symOffset:      file offset of the symbol in bytes
    parent:         address of parent of this procedure as
                    measured from the start of the subsection
    end:            address of the end of the debug info of
                    this procedure (and all its locals) as
                    measured from the start of the subsection
    next:           address of the debug info of the next
                    procedure as measured from the start of
                    the subsection
    codeLength:     length in bytes of the code in the procedure
    debugStart:     start of the debug info from "offset"
    debugEnd:       end of the debug info from "offset"
    offset:         starting address of the code of the procedure
    segment:        segment of address of the code of the procedure
    flags:          looks like it's always zero for now
    typeIndex:      type index of the procedure
    name:           name index of the procedure
    browserOffset:  offset of browser info
    linkName:       name of the procedure in the link phase
                    should point to maxLinkName char's of memory
    maxLinkName:    max. number of char's including zero terminator
                    that can be filled in at the linkName parameter

*/

unsigned int    BorDebugSymbolGPROC32(BorDebugCookie registerCookie,
                                      unsigned int   symOffset,
                                      unsigned int * parent,
                                      unsigned int * end,
                                      unsigned int * next,
                                      unsigned int * codeLength,
                                      unsigned int * debugStart,
                                      unsigned int * debugEnd,
                                      unsigned int * offset,
                                      unsigned int * segment,
                                      unsigned int * flags,
                                      unsigned int * typeIndex,
                                      unsigned int * name,
                                      unsigned int * browserOffset,
                                      char         * linkName,
                                      unsigned int   maxLinkName);



/*

    Details of BORDEBUG_S_THUNK32 = 0x00000206


    BorDebugSymbolTHUNK32


    Thunk


    symOffset:      file offset of the symbol in bytes
    parent:         address of parent of this thunk as
                    measured from the start of the subsection
    end:            address of the end of this thunk as
                    measured from the start of the subsection
    next:           address of next thunk as measured
                    from the start of the subsection
    offset:         starting address of the thunk
    segment:        segment of address of the thunk
    codeLength:     length in bytes of the code in the thunk
    ordinal:        0 == no type
                    1 == adjustor, delta = delta
                    2 == virtual call, delta == displacement
    name:           name index
    delta:          see notes under ordinal above

*/

void    BorDebugSymbolTHUNK32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * parent,
                              unsigned int * end,
                              unsigned int * next,
                              unsigned int * offset,
                              unsigned int * segment,
                              unsigned int * codeLength,
                              unsigned int * ordinal,
                              unsigned int * name,
                              unsigned int * delta);



/*

    Details of BORDEBUG_S_BLOCK32 = 0x00000207


    BorDebugSymbolBLOCK32


    Block

    symOffset:      file offset of the symbol in bytes
    parent:         address of parent of this block as
                    measured from the start of the subsection
    end:            address of the end of this block as
                    measured from the start of the subsection
    codeLength:     length in bytes of the code of the block
    offset:         offset of the code address of the block
    segment:        segment of the code address of the block
    name:           name index

*/

void    BorDebugSymbolBLOCK32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * parent,
                              unsigned int * end,
                              unsigned int * codeLength,
                              unsigned int * offset,
                              unsigned int * segment,
                              unsigned int * name);




/*

    Details of BORDEBUG_S_WITH32 = 0x00000208


    BorDebugSymbolWITH32


    With

    symOffset:      file offset of the symbol in bytes
    parent:         address of parent of with as
                    measured from the start of the subsection
    codeLength:     length in bytes of the code of the with block
    offset:         offset of address of start of the code of with
    segment:        segment of address of start of the code of with
    flags:          0 : nameIndex is true nameIndex
                    1 : nameIndex is offset in sstAlignSym (obsolete)
    typeIndex:      type index of the symbol
    name:           name index of the symbol
    varOffset:      offset within record if the with variable is
                    a field in a record

*/

void    BorDebugSymbolWITH32(BorDebugCookie registerCookie,
                             unsigned int   symOffset,
                             unsigned int * parent,
                             unsigned int * codeLength,
                             unsigned int * offset,
                             unsigned int * segment,
                             unsigned int * flags,
                             unsigned int * typeIndex,
                             unsigned int * name,
                             unsigned int * varOffset);



/*

    Details of BORDEBUG_S_LABEL32 = 0x00000209


    BorDebugSymbolLABEL32


    Label

    symOffset:      file offset of the symbol in bytes
    offset:         offset of address of start of label
    segment:        segment of address of start of label
    nearFar:        0 : near, 4 : far
    name:           name index

*/

void    BorDebugSymbolLABEL32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * offset,
                              unsigned int * segment,
                              unsigned int * nearFar,
                              unsigned int * name);



/*

    Details of BORDEBUG_S_CEXMODEL32 = 0x0000020A

    Change execution model

    Not used

*/

/*

    Details of BORDEBUG_S_VFTPATH32 = 0x0000020B

    Virtual function table path

    Not used

*/

/*

    Details of BORDEBUG_S_ENTRY32 = 0x00000210


    BorDebugSymbolENTRY32


    Entry point of application

    symOffset:  file offset of the symbol in bytes
    offset:     offset of address of function
    segment:    segment of address of function

*/

void    BorDebugSymbolENTRY32(BorDebugCookie registerCookie,
                              unsigned int   symOffset,
                              unsigned int * offset,
                              unsigned int * segment);




/*

    Details of BORDEBUG_S_OPTVAR32 = 0x00000211


    BorDebugSymbolOPTVAR32


    Optimized variable

    This API has to be called twice:
    The first time call with the correct symOffset
    and leave maxEntries 0 and startEntries, lengthEntries,
    and regNameEntries NULL, and the returned value is
    the number of entries for the optimized variable.
    The second time, allocate memory for startEntries,
    lengthEntries, and regNameEntries, and set
    maxEntries to the max. number of entries
    expected.  On return the three arrays will be
    filled in.

    symOffset:      file offset of the symbol in bytes
    maxEntries:     max. number of entries wanted
    startEntries:   starting offsets of the entries
    lengthEntries:  length in bytes of the entries
    regNameEntries: register name indices of the entries
                    use RegIndexToName to get the names

*/

unsigned int    BorDebugSymbolOPTVAR32(BorDebugCookie registerCookie,
                                       unsigned int   symOffset,
                                       unsigned int   maxEntries,
                                       unsigned int * startEntries,
                                       unsigned int * lengthEntries,
                                       unsigned int * regNameEntries);



/*

    Details of BORDEBUG_S_PROCRET32 = 0x00000212


    BorDebugSymbolPROCRET32


    Procedure return point

    symOffset:  file offset of the symbol in bytes
    offset:     starting address of function epilogue
    length:     length of function epilogue

*/

void    BorDebugSymbolPROCRET32(BorDebugCookie registerCookie,
                                unsigned int   symOffset,
                                unsigned int * offset,
                                unsigned int * length);




/*

    Details of BORDEBUG_S_SAVREGS32 = 0x00000213


    BorDebugSymbolSAVREGS32


    Callee saved registers

    symOffset:  file offset of the symbol in bytes
    mask:       Saved registers:
                EBX == 0x0001
                EDI == 0x0002
                ESI == 0x0004
                EBP == 0x0008
    offset:     EBP relative stack offset

*/

void    BorDebugSymbolSAVREGS32(BorDebugCookie registerCookie,
                                unsigned int   symOffset,
                                unsigned int * mask,
                                unsigned int * offset);



/*

    Details of BORDEBUG_S_SLINK32 = 0x00000230


    BorDebugSymbolSLINK32


    Pascal static link symbol. The offset in the parent stack
    frame of a variable that is accessed in a local procedure

    symOffset:      file offset of the symbol in bytes
    return:         the offset of the variable in the parent stack frame

*/

unsigned int    BorDebugSymbolSLINK32(BorDebugCookie registerCookie,
                                      unsigned int   symOffset);



//---------------------------------------------------------------------

/*

    sstSrcModule API's

*/


/*

    Here is some example source code about how to access the
    SrcModule API's.



        // Get the counts
        BorDebugSrcModule(cookie, offset, &rangeNum, &sourceNum);

        // Get the range details
        rangeSegs = malloc(rangeNum * sizeof(unsigned int));
        rangeStarts = malloc(rangeNum * sizeof(unsigned int));
        rangeEnds = malloc(rangeNum * sizeof(unsigned int));

        BorDebugSrcModuleRanges(cookie, offset, rangeSegs, rangeStarts, rangeEnds);

        // Print out the segments, starting offsets and ending offsets of the ranges.


        // free them all
        free(rangeSegs);
        free(rangeStarts);
        free(rangeEnds);

        // Get the source details
        sourceOffsets = malloc(sourceNum * sizeof(unsigned int));
        sourceNames = malloc(sourceNum * sizeof(unsigned int));
        sourceRangeCounts = malloc(sourceNum * sizeof(unsigned int));

        BorDebugSrcModuleSources(cookie,
                                 offset,
                                 sourceOffsets,
                                 sourceNames,
                                 sourceRangeCounts);

        // Print out the offsets of the source file records in the
        // file and the names of the sources


        // Free some

        free(sourceNames);
        free(sourceOffsets);


        // For each range in each source

        for (sourceIndex = 0; sourceIndex < sourceNum; sourceIndex++)
        {
            // Get the range details for each source
            rangeSegs = malloc(sourceRangeCounts[sourceIndex] * sizeof(unsigned int));
            rangeStarts = malloc(sourceRangeCounts[sourceIndex] * sizeof(unsigned int));
            rangeEnds = malloc(sourceRangeCounts[sourceIndex] * sizeof(unsigned int));
            rangeLineCounts = malloc(sourceRangeCounts[sourceIndex] * sizeof(unsigned int));

            BorDebugSrcModuleSourceRanges(cookie,
                                          offset,
                                          sourceIndex,
                                          rangeSegs,
                                          rangeStarts,
                                          rangdEnds,
                                          rangeLineCounts);

            // Do something with the range details


            // For each range, get the line number details
            for (rangeIndex = 0; rangeIndex < sourceRangeCounts[sourceIndex]; rangeIndex++)
            {
                lineNums = malloc(rangeLineCounts[rangeIndex] * sizeof(unsigned int));
                lineOffs = malloc(rangeLineCounts[rangeIndex] * sizeof(unsigned int));

                BorDebugSrcModuleLineNumbers(cookie,
                                             offset,
                                             sourceIndex,
                                             rangeIndex,
                                             lineNums,
                                             lineOffs);

                // Do something with the line number details

                // Free
                free(lineNums);
                free(lineOffs);
            }

            free(rangeSegs);
            free(rangeStarts);
            free(rangeEnds);
            free(rangeLineCounts);
        }

        free(sourceRangeCounts);


*/

/*

    BorDebugSrcModule


    Details of a sstSrcModule subsection

    offset:         file offset of subsection as received in BorDebugSubSection
                    assumes this subsection is of sstSrcModule type
    rangeCount:     number of code ranges
                    access them with BorDebugSrcModuleRanges
    sourceCount:    number of source files
                    access them with BorDebugSrcModuleSources


*/

void    BorDebugSrcModule(BorDebugCookie registerCookie,
                          unsigned int   offset,
                          unsigned int * rangeCount,
                          unsigned int * sourceCount);



/*

    BorDebugSrcModuleRanges


    Details of the code ranges in the sstSrcModule

    The total number of code ranges is known from BorDebugSrcModule.
    When you call this API, make sure to allocate enough memory for the
    three arrays, each should be of size number of code ranges *
    sizeof(unsigned int).

    offset:             file offset of subsection as received in BorDebugSubSection
                        assumes this subsection is of sstSrcModule type
    segments:           points to array of segments, one for each code range
    segmentStarts:      points to array of segment starts, one for each code range
    segmentEnds:        points to array of segment ends, one for each code range

*/

void    BorDebugSrcModuleRanges(BorDebugCookie registerCookie,
                                unsigned int   offset,
                                unsigned int * segments,
                                unsigned int * segmentStarts,
                                unsigned int * segmentEnds);




/*

    BorDebugSrcModuleSources


    Details of source files in the sstSrcModule

    The total number of source files is known from
    BorDebugSrcModuleSource. When you call this API, make
    sure to allocate enough memory for the three arrays, each
    should be of size number of source files * sizeof(unsigned int)

    offset:         file offset of subsection as received in BorDebugSubSection
                    assumes this subsection is of sstSrcModule type
    sourceOffsets:  points to array of file offsets of source file records
    names:          points to array of name indices of the sources
    rangeCounts:    points to array of number of code ranges in the sources
                    access them with BorDebugSrcModuleSourceRanges

*/

void    BorDebugSrcModuleSources(BorDebugCookie registerCookie,
                                 unsigned int   offset,
                                 unsigned int * sourceOffsets,
                                 unsigned int * names,
                                 unsigned int * rangeCounts);




/*

    BorDebugSrcModuleSourceRanges


    Details of code ranges in a source file in sstSrcModule


    The total number of code ranges is known from
    BorDebugSrcModuleSource. When you call this API, make
    sure to allocate enough memory for the four arrays, each
    should be of size number of code ranges * sizeof(unsigned int)

    Source refers to a 0-based source file index.  The total number of
    source files is known from BorDebugSrcModuleSource, and this API
    allows you to iterate through those source files.

    offset:             file offset of subsection as received in BorDebugSubSection
                        assumes this subsection is of sstSrcModule type
    source:             0-based the source being questioned
    segments:           points to array of segments
    segmentStarts:      points to array of segment starts
    segmentEnds:        points to array of segment ends
    lineNumberCounts:   points to array of line number / address pair counts
                        access them with BorDebugSrcModuleLineNumbers

*/

void    BorDebugSrcModuleSourceRanges(BorDebugCookie registerCookie,
                                      unsigned int   offset,
                                      unsigned int   source,
                                      unsigned int * segments,
                                      unsigned int * segmentStarts,
                                      unsigned int * segmentEnds,
                                      unsigned int * lineNumberCounts);


/*

    BorDebugSrcModuleLineNumbers


    The total number of line number / address pairs is known from
    BorDebugSrcModuleSourceRanges.
    When you call this API, make sure to allocate enough memory for the
    two arrays, each should be of size number of pairs * sizeof(unsigned int).


    Details of line number / address pairs of a range in a source

    offset:         file offset of subsection as received in BorDebugSubSection
                    assumes this subsection is of sstSrcModule type
    source:         the source being questioned
    range:          the range being questioned
    lineNumbers:    line number
    lineOffsets:    address of line

*/

void    BorDebugSrcModuleLineNumbers(BorDebugCookie registerCookie,
                                     unsigned int   offset,
                                     unsigned int   source,
                                     unsigned int   range,
                                     unsigned int * lineNumber,
                                     unsigned int * lineOffset);


//---------------------------------------------------------------------

/*

    sstGlobalSym and sstGlobalPub API's

*/

/*

    BorDebugGlobalSym


    Get the general details of a sstGlobalSym or a sstGlobalPub

    offset:             file offset of subsection as received in
                        BorDebugSubSection assumes this subsection is
                        of sstGlobalSym or sstGlobalPub type.

    symHashFunction:    name index of symbol hash function
    addrHashFunction:   name index of address hash function
    symTableBytes:      total bytes in the symbol table
    symHashTableBytes:  total bytes in the symbol hash table
    addrHashTableBytes: total bytes in the address hash table
    totalUDTs:          total number of user defined types
    totalOtherSyms:     total number of other symbols
    totalSymbols:       total number of symbols
    totalNameSpaces:    total number of namespaces

*/


void    BorDebugGlobalSym(BorDebugCookie registerCookie,
                          unsigned int   offset,
                          unsigned int * symHashFunction,
                          unsigned int * addrHashFunction,
                          unsigned int * symTableBytes,
                          unsigned int * symHashTableBytes,
                          unsigned int * addrHashTableBytes,
                          unsigned int * totalUDTs,
                          unsigned int * totalOtherSyms,
                          unsigned int * totalSymbols,
                          unsigned int * totalNameSpaces);




//---------------------------------------------------------------------

/*

    sstGlobalTypes API's

*/

/*

    BorDebugGlobalTypes


    Get the general details of the sstGlobalTypes section

    signature:      signature at the start
    totalTypes:     total number of types
                    You can access each 0x1000 based type index
                    with the type index API's

*/

void    BorDebugGlobalTypes(BorDebugCookie registerCookie,
                            unsigned int * signature,
                            unsigned int * totalTypes);


//---------------------------------------------------------------------

/*

    Type index API's

*/

/*

    IMPORTANT:

    All type indices are 0x1000 based
    None of the API's checks for type index validity.
    If you access types 0-based, you will get either
    crashes or bogus results.

    Types are build up from leaves (BORDEBUG_LF_XXX records)

    There are several types of leaves:
    - Types directly referenced in symbols (0x00000001 - 0x000000EE)
      See enum leaf000s, defined at the top of the file
    - Types referenced by other types (0x00000200 - 0x000002EE)
      See enum leaf200s, defined at the top of the file
    - Internal leaves to build more complex types (0x00000400 - 0x000004EE)
      See enum leaf400s, defined at the top of the file

    There are also numeric leaves (0x00008000 - 0x000080EE), but when
    you use these API's you will probably never see them.

    Finally there are padding leaves (0xF0 - 0xFF), although when you
    use these API's you shouldn't see any.

    Each type starts at a leaf (type kind).  This type kind will give
    some information, and will often refer to other types by their
    0x1000 based type index numbers.  You follow these underlying types
    by calling BorDebugTypeFromIndex with the type index, which will give
    you the offset and the kind, after which you can call the relevant
    BorDebugTypeXXXX API.
    To understand the complete type, keep following the type indices
    till there are no more references to underlying types.

    The BorDebugTypeXXXX API's don't check if the type starting at the
    offset you provide is indeed of the expected type.  Mysterious
    results and random crashes will happen if you don't match the
    correct BorDebugTypeXXXX with the corresponding BORDEBUG_LF_XXXX value.

*/

//---------------------------------------------------------------------


/*

    BorDebugTypeFromIndex


    Get the general info for a type from its index


    This is the random access way of reading the types.  All you need
    is a 0x1000 based type index, and you'll find out what type it is
    and where it starts.


    BorDebugTypeFromIndex will give you the starting point of a
    certain type with index "typeIndex", which should be 0x1000
    based.  Each type points to a primary type kind.  Based on
    the type kind you get back from BorDebugTypeFromIndex, call
    the appropriate BorDebugTypeXXXX routine supplying the
    typeOffset you also got from BorDebugTypeFromIndex.


    typeIndex:  0x1000 based type index
    typeOffset: file offset in bytes of the primary type
    length:     length in bytes of the primary type
    typeKind:   BORDEBUG_LF_XXX kind of the primary type

*/

void    BorDebugTypeFromIndex(BorDebugCookie registerCookie,
                              unsigned int   typeIndex,
                              unsigned int * typeOffset,
                              unsigned int * length,
                              unsigned int * typeKind);





/*

    BorDebugTypeFromOffset


    Get the general info for a type from its offset


    In some cases a program needs to get the basic information of
    a type based on its starting offset.  This is true for instance
    for .obj and .lib files where the types are ordered sequentially
    with no type index table.


    BorDebugTypeFromOffset will give you the length and the type
    kind (BORDEBUG_LF_XXXX) of the type starting at a particular offset.
    You can use the offset and the type kind to call the appropriate
    BorDebugTypeXXXX API.


    typeOffset: file offset in bytes of the type
    length:     length in bytes of the type
    typeKind:   BORDEBUG_LF_XXX kind of the type

*/

void    BorDebugTypeFromOffset(BorDebugCookie registerCookie,
                               unsigned int   typeOffset,
                               unsigned int * length,
                               unsigned int * typeKind);


//---------------------------------------------------------------------

/*

    Type API's

*/

/*

    Details of BORDEBUG_LF_STARTYP    = 0x00000000

    Not a valid type

*/

/*

    Details of BORDEBUG_LF_MODIFIER   = 0x00000001


    BorDebugTypeMODIFIER


    Specifies if a type is const and/or volatile.

    typeOffset: file offset in bytes of the type
    attributes: 0x1 == const, 0x2 == volatile
    typeIndex:  type index of modified type

*/

void    BorDebugTypeMODIFIER(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * attributes,
                             unsigned int * typeIndex);



/*

    Details of BORDEBUG_LF_POINTER    = 0x00000002


    BorDebugTypePOINTER


    Describes pointers, including references, pointers to
    member data and pointers to member functions.


    typeOffset:     file offset in bytes of the type
    attributes:
                    type modifier of pointer in bits 0 - 4
                    0 = near
                    1 = far
                    2 = huge
                    3 = based on segment
                    4 = based on value
                    5 = based on segment of value
                    6 = based on address of symbol
                    7 = based on segment of symbol
                    8 = based on type
                    9 = based on self
                    a = near32
                    b = far32

                    mode of pointer in bits 5 - 7
                    0 pointer
                    1 reference
                    2 pointer to data member
                    3 pointer to function

                    if bit 8 is on, this is a flat32 pointer
                    if bit 9 is on, this is a volatile pointer
                    if bit 10 is on, this is a const pointer

    typeIndex:      type index of pointed to type

    value1:         If the pointer mode is 2 or 3, this will
                    be filled with the class type index
                    If the pointer type is 3, this will be
                    filled with the segment
                    If the pointer type is 8, this will be
                    filled with a type index
    value2:         If the pointer mode is 2 or 3, this will
                    be filled with the member pointer type:
                     31 = determined by class type
                     32 = points to single inheritance class
                     33 = points to multiple inheritance class
                     34 = points to class with virtual bases
                    If the pointer type is 8, this will be
                    filled with a name index

*/

void    BorDebugTypePOINTER(BorDebugCookie registerCookie,
                            unsigned int   typeOffset,
                            unsigned int * attributes,
                            unsigned int * typeIndex,
                            unsigned int * value1,
                            unsigned int * value2);


/*

    Details of BORDEBUG_LF_ARRAY      = 0x00000003


    BorDebugTypeARRAY


    Describes one dimensional arrays.


    typeOffset:     file offset in bytes of the type
    elementType:    type index of elements
    indexType:      type index of indexer
    name:           name index of the array name
    size:           size of the array in bytes
    elements:       number of elements

*/

void    BorDebugTypeARRAY(BorDebugCookie registerCookie,
                          unsigned int   typeOffset,
                          unsigned int * elementType,
                          unsigned int * indexType,
                          unsigned int * name,
                          unsigned int * size,
                          unsigned int * elements);


/*

    Details of BORDEBUG_LF_CLASS      = 0x00000004
    Details of BORDEBUG_LF_STRUCT     = 0x00000005


    BorDebugTypeCLASS


    Describes classes and structs respectively.


    typeOffset:         file offset in bytes of the type
    fieldCount:         number of fields
                        this includes all bases, members, methods, and friends
    fieldList:          type index of list of fields
    property:           bitfield of properties
        if bit 0 is on, class has a constructor
        if bit 1 is on, class has overloaded operators
        if bit 2 is on, class is a nested class
        if bit 3 is on, class contains nested classes
        if bit 4 is on, class has overloaded assignment
        if bit 5 is on, class has casting methods
        if bit 6 is on, class is a forward (incomplete) reference
        if bit 7 is on, class has a destructor
    containingClass:    type index of containing class, if any
    derivationList:     type index of list of deriving classes
    vtable:             type index of vtable layout
    name:               name index of class
    classSize:          size of class

*/

void    BorDebugTypeCLASS(BorDebugCookie registerCookie,
                          unsigned int   typeOffset,
                          unsigned int * fieldCount,
                          unsigned int * fieldList,
                          unsigned int * property,
                          unsigned int * containingClass,
                          unsigned int * derivationList,
                          unsigned int * vtable,
                          unsigned int * name,
                          unsigned int * classSize);


/*

    Details of BORDEBUG_LF_UNION      = 0x00000006


    BorDebugTypeUNION


    Describes unions.


    typeOffset:         file offset in bytes of the type
    fieldCount:         number of fields
    fieldList:          type index of list of fields
    property:           bitfield of properties (not all are applicable)
        if bit 0 is on, union has a constructor
        if bit 1 is on, union has overloaded operators
        if bit 2 is on, union is a nested union
        if bit 3 is on, union contains nested classes
        if bit 4 is on, union has overloaded assignment
        if bit 5 is on, union has casting methods
        if bit 6 is on, union is a forward (incomplete) reference
        if bit 7 is on, union has a destructor
    containingClass:    type index of containing class, if any
    name:               name index of class
    classSize:          size of union

*/

void    BorDebugTypeUNION(BorDebugCookie registerCookie,
                          unsigned int   typeOffset,
                          unsigned int * fieldCount,
                          unsigned int * fieldList,
                          unsigned int * property,
                          unsigned int * containingClass,
                          unsigned int * name,
                          unsigned int * classSize);


/*

    Details of BORDEBUG_LF_ENUM       = 0x00000007


    BorDebugTypeENUM


    Describes enums.


    typeOffset:         file offset in bytes of the type
    memberCount:        number of members in the enum
    underType:          type index of the underlying type of the enum
    memberList:         type index of list of members
    containingClass:    type index of containing class, if any
    name:               name index of the enum

*/

void    BorDebugTypeENUM(BorDebugCookie registerCookie,
                         unsigned int   typeOffset,
                         unsigned int * memberCount,
                         unsigned int * underType,
                         unsigned int * memberList,
                         unsigned int * containingClass,
                         unsigned int * name);


/*

    Details of BORDEBUG_LF_PROCEDURE  = 0x00000008


    BorDebugTypePROCEDURE


    Describes global functions and procedures.


    typeOffset:        file offset in bytes of the type
    returnType:        type index of return value
    callingConvention: calling convention in lower 4 bits:
        0  near C
        1  far  C
        2  near Pascal
        3  far  Pascal
        4  near Fastcall
        5  far  Fastcall
        6  PCode
        7  near Stdcall
        8  far  Stdcall
        9  near Syscall
        10 far  Syscall
        11 this call
        12 near Fastcall (same as 4)
        13      MS compatible fastcall

        0x80 bit set : fast this
        0x40 bit set : variable args
    argCount:        number of arguments
    argList:         type index of list of arguments

*/

void    BorDebugTypePROCEDURE(BorDebugCookie registerCookie,
                              unsigned int   typeOffset,
                              unsigned int * returnType,
                              unsigned int * callingConvention,
                              unsigned int * argCount,
                              unsigned int * argList);


/*

    Details of BORDEBUG_LF_MFUNCTION  = 0x00000009


    BorDebugTypeMFUNCTION


    Describes member functions


    typeOffset:         file offset in bytes of the type
    returnType:         type index of return value
    classType:          type index of the containing class
    thisType:           type index of this parameter.
                                if thisType == NULL, function is static
    callingConvention:  calling convention in lower 4 bits:
        0  near C
        1  far  C
        2  near Pascal
        3  far  Pascal
        4  near Fastcall
        5  far  Fastcall
        6  PCode
        7  near Stdcall
        8  far  Stdcall
        9  near Syscall
        10 far  Syscall
        11 this call
        12 near Fastcall (same as 4)
        13      MS compatible fastcall

        0x80 bit set : fast this
        0x40 bit set : variable args
    argCount:           number of arguments
    argList:            type index of list of arguments
    thisAdjust:         offset of function inside class

*/

void    BorDebugTypeMFUNCTION(BorDebugCookie registerCookie,
                              unsigned int   typeOffset,
                              unsigned int * returnType,
                              unsigned int * classType,
                              unsigned int * thisType,
                              unsigned int * callingConvention,
                              unsigned int * argCount,
                              unsigned int * argList,
                              unsigned int * thisAdjust);


/*

    Details of BORDEBUG_LF_VTSHAPE    = 0x0000000A


    BorDebugTypeVTSHAPE


    Describes virtual function tables.  A class that has a
    vtable will have a VFUNCTAB as one of its members, and
    that VFUNCTAB member should be of a POINTER type, and
    the underlying type of that POINTER type will be a
    VTSHAPE type.


    This API should be called twice:
    The first time, use typeOffset, and set maxCount and
    descriptorArray to zero, and the count of descriptor
    entries is returned.
    The second time, use typeOffset, set MaxCount to the
    number of entries desired, and point descriptorArray
    to enough memory for all descriptors wanted.

    typeOffset:      file offset in bytes of the type
    maxCount:        max. number of descriptors wanted
    descriptorArray: points to memory for descriptors

    descriptor:
        0               Near
        1               Far
        2               Thin
        3               Address point displacement to outermost class.
                        This is at entry [-1] from tables address.
        4               Far pointer to metaclass descriptor.
                        This is at entry [-2] from table address.
        5               Near32
        6               Far32
*/

unsigned int    BorDebugTypeVTSHAPE(BorDebugCookie  registerCookie,
                                    unsigned int    typeOffset,
                                    unsigned int    maxCount,
                                    unsigned char * descriptorArray);


/*

    Details of BORDEBUG_LF_COBOL0     = 0x0000000B

    Not used

*/


/*

    Details of BORDEBUG_LF_COBOL1     = 0x0000000C

    Not used

*/


/*

    Details of BORDEBUG_LF_BARRAY     = 0x0000000D

    Not used

*/



/*

    Details of BORDEBUG_LF_LABEL      = 0x0000000E


    BorDebugTypeLABEL


    Describes assembler labels


    typeOffset: file offset in bytes of the type
    return:     0 : near label, 4 : far label

*/

unsigned int    BorDebugTypeLABEL(BorDebugCookie registerCookie,
                                  unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_NULL       = 0x0000000F

    Not used

*/


/*

    Details of BORDEBUG_LF_NOTTRANS   = 0x00000010

    Not used

*/


/*

    Details of BORDEBUG_LF_DIMARRAY   = 0x00000011

    Not used

*/



/*

    Details of BORDEBUG_LF_VFTPATH    = 0x00000012

    Not used

*/


/*

    Details of BORDEBUG_LF_SET        = 0x00000030


    BorDebugTypeSET


    Describes Pascal type sets.


    typeOffset: file offset in bytes of the type
    elemType:   type index of elements, could be a subrange type
    name:       name index of set name
    lowByte:    first value in set
    length:     number of elements

*/

void    BorDebugTypeSET(BorDebugCookie registerCookie,
                        unsigned int   typeOffset,
                        unsigned int * elemType,
                        unsigned int * name,
                        unsigned int * lowByte,
                        unsigned int * length);


/*

    Details of BORDEBUG_LF_SUBRANGE   = 0x00000031


    BorDebugTypeSUBRANGE


    Describes Pascal type subranges.


    typeOffset: file offset in bytes of the type
    baseType:   type index of elements
    name:       name index of subrange name
    loBound:    low bound
    hiBound:    high bound
    size:       size

*/

void    BorDebugTypeSUBRANGE(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * baseType,
                             unsigned int * name,
                             unsigned int * loBound,
                             unsigned int * hiBound,
                             unsigned int * size);


/*

    Details of BORDEBUG_LF_PARRAY     = 0x00000032


    BorDebugTypePARRAY


    Describes Pascal style arrays. It is similar to C style
    arrays, except the indexType could be a subrange.


    typeOffset:  file offset in bytes of the type
    elementType: type index of elements
    indexType:   type index of indexer, could be a subrange type
    name:        name index of the array name
    size:        size of the array in bytes
    elements:    number of elements

*/

void    BorDebugTypePARRAY(BorDebugCookie registerCookie,
                           unsigned int   typeOffset,
                           unsigned int * elementType,
                           unsigned int * indexType,
                           unsigned int * name,
                           unsigned int * size,
                           unsigned int * elements);


/*

    Details of BORDEBUG_LF_PSTRING    = 0x00000033


    BorDebugTypePSTRING


    Describes Pascal short strings.


    typeOffset: file offset in bytes of the type
    elemType:   type index of elements
    indexType:  type index of index
    name:       name index of pstring name

*/

void    BorDebugTypePSTRING(BorDebugCookie registerCookie,
                            unsigned int   typeOffset,
                            unsigned int * elemType,
                            unsigned int * indexType,
                            unsigned int * name);


/*

    Details of BORDEBUG_LF_CLOSURE    = 0x00000034


    BorDebugTypeCLOSURE


    Describes closure's.


    typeOffset:        file offset in bytes of the type
    returnType:        type index of return value
    callingConvention: calling convention in lower 4 bits:
        0  near C
        1  far  C
        2  near Pascal
        3  far  Pascal
        4  near Fastcall
        5  far  Fastcall
        6  PCode
        7  near Stdcall
        8  far  Stdcall
        9  near Syscall
        10 far  Syscall
        11 this call
        12 near Fastcall (same as 4)
        13      MS compatible fastcall

        0x80 bit set : fast this
        0x40 bit set : variable args
    argCount:           number of arguments
    argList:            type index of list of arguments

*/

void    BorDebugTypeCLOSURE(BorDebugCookie registerCookie,
                            unsigned int   typeOffset,
                            unsigned int * returnType,
                            unsigned int * callingConvention,
                            unsigned int * argCount,
                            unsigned int * argList);


/*

    Details of BORDEBUG_LF_PROPERTY   = 0x00000035


    BorDebugTypePROPERTY


    Describes properties.


    typeOffset: file offset in bytes of the type
    propType:   type index of the property
    flags:      property flags
        PROPF_DEFAULT       = 1
        PROPF_READ_IS_NAME  = 2 (read slot is name index)
        PROPF_WRITE_IS_NAME = 4 (write slot is name index)
    arrayIndex: type index of BORDEBUG_LF_ARGLIST of array properties
    propIndex:  index number for indexed properties
    readSlot:   offset for fields, name index for methods
    writeSlot:  offset for fields, name index for methods

*/

void    BorDebugTypePROPERTY(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * propType,
                             unsigned int * flags,
                             unsigned int * arrayIndex,
                             unsigned int * propIndex,
                             unsigned int * readSlot,
                             unsigned int * writeSlot);


/*

    Details of BORDEBUG_LF_LSTRING    = 0x00000036


    BorDebugTypeLSTRING


    Describes Pascal long strings.


    typeOffset: file offset in bytes of the type
    return:     name index of string

*/

unsigned int    BorDebugTypeLSTRING(BorDebugCookie registerCookie,
                                    unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_VARIANT    = 0x00000037


    BorDebugTypeVARIANT


    Describes variants.


    typeOffset: file offset in bytes of the type
    return:     name index of variant

*/

unsigned int    BorDebugTypeVARIANT(BorDebugCookie registerCookie,
                                    unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_CLASSREF   = 0x00000038


    BorDebugTypeCLASSREF


    Describes Pascal class references.


    typeOffset: file offset in bytes of the type
    refType:    type index of referred class
    vtShape:    type index of vtable shape

*/

void    BorDebugTypeCLASSREF(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * refType,
                             unsigned int * vtShape);


/*

    Details of BORDEBUG_LF_WSTRING    = 0x00000039


    BorDebugTypeWSTRING


    Describes Pascal wide strings.


    typeOffset: file offset in bytes of the type
    return:     name index of string

*/

unsigned int    BorDebugTypeWSTRING(BorDebugCookie registerCookie,
                                    unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_SKIP      = 0x00000200

    Not used

*/


/*

    Details of BORDEBUG_LF_ARGLIST    = 0x00000201


    BorDebugTypeARGLIST


    Describes argument lists of functions and methods.
    The type API's for methods, closures and functions
    will return a type index for the argument list, and
    this type index will belong to an ARGLIST type.


    This API has to be called twice:
    The first time, fill in typeOffset and set maxTypes
    and typeIndexArray to zero.  The number of arguments is
    returned.
    The second time, fill in typeOffset, set maxTypes to
    the max. number of type indices you want, and point
    typeIndexArray to enough valid memory.  On return,
    the typeIndexArray will be filled with type indices
    for all the arguments.

    typeOffset:     file offset in bytes of the type
    maxTypes:       max. number of argument type indices wanted
    typeIndexArray: points to memory to receive argument type indices

*/

unsigned int    BorDebugTypeARGLIST(BorDebugCookie registerCookie,
                                    unsigned int   typeOffset,
                                    unsigned int   maxTypes,
                                    unsigned int * typeIndexArray);


/*

    Details of BORDEBUG_LF_DEFARG     = 0x00000202

    Not used.

*/




/*

    Details of BORDEBUG_LF_LIST      = 0x00000203

    Not used

*/



/*

    Details of BORDEBUG_LF_FIELDLIST  = 0x00000204


    BorDebugTypeFIELDLIST


    Describes the members of a class, struct, union, or enum.
    This list includes base classes and vtable descriptors.


    A FIELDLIST can only be accessed serially, due to the fact
    that the length of each field is not stored, so each field
    has to be extracted before we can skip to the next one.

    First, BorDebugTypeStartFIELDLIST has to be called to set
    up the serial traversing of the fields.
    Next, keep calling BorDebugTypeNextFIELDLIST till both the
    kind and the offset fields receive zero.

    It is possible that the field list is split up into multiple
    parts.  If this is the case, each FIELDLIST will have a
    BORDEBUG_LF_INDEX as its last member.  To continue going
    through the fields, get the type index out of the
    BORDEBUG_LF_INDEX by using BorDebugTypeINDEX, and start
    a new FIELDLIST, e.g.:

    // start the field list
    BorDebugTypeStartFIELDLIST(cookie, offset);
    while (1)
    {
        // next field
        BorDebugTypeNextFIELDLIST(cookie, &kind, &offset);

        // are we done
        if  (kind == 0 && offset == 0)
            break;

        // is this a continuation
        if  (kind == BORDEBUG_LF_INDEX)
        {
            // get the continuation index and its offset
            contIndex = BorDebugTypeINDEX(cookie, offset);
            BorDebugTypeFromIndex(cookie, contIndex, &offset, &len, &kind);

            // continue in the next field list
            BorDebugTypeStartFIELDLIST(cookie, offset);
            continue;
        }

        // process the type
    }


    To find out more information about each member type, call
    into the relevant BorDebugTypeXXXX API, based on the kind.


    typeOffset: file offset in bytes of the field list type

    kind:       receives the kind of the next field
    offset:     receives the offset of the next field
*/

void    BorDebugTypeStartFIELDLIST(BorDebugCookie registerCookie,
                                   unsigned int   typeOffset);


void    BorDebugTypeNextFIELDLIST(BorDebugCookie registerCookie,
                                  unsigned int * kind,
                                  unsigned int * offset);



/*

    Details of BORDEBUG_LF_DERIVED    = 0x00000205


    BorDebugTypeDERIVED


    Describes all the classes that are directly derived from a
    class.  The class type will have a type index for its derivation
    list, which should have BORDEBUG_LF_DERIVED type kind.


    This API has to be called twice:
    The first time, use the correct typeOffset, and set maxTypes and
    derivedTypes to zero, and the total number of derived types is
    returned.
    The second time, set maxTypes to the number of types wanted, and
    point derivedTypes to enough allocated memory to hold all the
    type indices.  On return the type array is filled in.

    typeOffset:     file offset in bytes of the type
    maxTypes:       max. number of type indices wanted
    derivedTypes:   allocated array for type indices

    returns the total number of derived types.

*/

unsigned int    BorDebugTypeDERIVED(BorDebugCookie registerCookie,
                                    unsigned int   typeOffset,
                                    unsigned int   maxTypes,
                                    unsigned int * derivedTypes);



/*

    Details of BORDEBUG_LF_BITFIELD   = 0x00000206


    BorDebugTypeBITFIELD


    Describes bitfields.


    typeOffset: file offset in bytes of the type
    length:     length in bits
    position:   starting position (zero based) in object
    typeIndex:  type index of variable

*/

void    BorDebugTypeBITFIELD(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * length,
                             unsigned int * position,
                             unsigned int * typeIndex);


/*

    Details of BORDEBUG_LF_METHODLIST = 0x00000207


    BorDebugTypeMETHODLIST


    Describes functions and procedures.  For non-overloaded
    functions, the methodlist will contain one entry, and for
    overloaded functions there will be multiple entries.


    This API has to be called twice
    The first time, fill in typeOffset, and
    set the other fields to zero.  The total
    number of methods is returned. The second
    time, set maxMethods to the max. number of
    methods wanted, and point the 4 arrays to
    enough memory to hold the details of all the
    methods wanted.

    For each method, you can get more information from the
    type index of the method.  Use BorDebugTypeFromIndex to get
    the offset and the type kind, and call the relevant
    BorDebugTypeXXXX function to get more information
    about the type

    typeOffset:   file offset in bytes of the type
    maxMethods:   the max. number of methods wanted
    typeArray:    filled in with type indices
    attribArray:  filled in with attributes
        access:  2 bits for access
                 0 no access protection
                 1 private access
                 2 protected access
                 3 public access
        mprop:   3 bits for properties
                 0 vanilla
                 1 virtual
                 2 static
                 3 friend
                 4 introducing virtual
                 5 pure virtual
                 6 pure introducing virtual
                 7 reserved
    browserArray: filled in with browser offsets
    vtabOffArray: filled in with vtable offsets, if any
        if mprop == 4 || mprop == 6, vtable offset is filled in
        otherwise it's zero

*/

unsigned int    BorDebugTypeMETHODLIST(BorDebugCookie registerCookie,
                                       unsigned int   typeOffset,
                                       unsigned int   maxMethods,
                                       unsigned int * typeArray,
                                       unsigned int * attribArray,
                                       unsigned int * browserArray,
                                       unsigned int * vtabOffArray);


/*

    Details of BORDEBUG_LF_DIMCONU    = 0x00000208

    Not used

*/


/*

    Details of BORDEBUG_LF_DIMCONLU   = 0x00000209

    Not used

*/


/*

    Details of BORDEBUG_LF_DIMVARU    = 0x0000020A

    Not used

*/


/*

    Details of BORDEBUG_LF_DIMVARLU   = 0x0000020B

    Not used

*/


/*

    Details of BORDEBUG_LF_REFSYM     = 0x0000020C

    Not used

*/


/*

    Details of BORDEBUG_LF_BCLASS     = 0x00000400


    BorDebugTypeBCLASS


    Describes non virtual base classes.


    typeOffset: file offset in bytes of the type
    baseType:   type index of base class
    attrib:     attributes
        access:  bits 0 - 1 for access
                 0 no access protection
                 1 private access
                 2 protected access
                 3 public access
        mprop:   bit 2 - 4 for properties
                 0 vanilla
                 1 virtual
                 2 static
                 3 friend
                 4 introducing virtual
                 5 pure virtual
                 6 pure introducing virtual
                 7 reserved
    offset:     offset in derived class

*/

void    BorDebugTypeBCLASS(BorDebugCookie registerCookie,
                           unsigned int   typeOffset,
                           unsigned int * baseType,
                           unsigned int * attrib,
                           unsigned int * offset);



/*

    Details of BORDEBUG_LF_VBCLASS    = 0x00000401


    BorDebugTypeVBCLASS


    Describes the virtual base of a class, with the
    restriction that the trail from the class to the
    virtual base is composed of zero or more virtual
    base classes.  If any of the base classes is non
    virtual, the type would be BORDEBUG_LF_IVBCLASS
    (0x00000402).


    typeOffset: file offset in bytes of the type
    vbType:     type index of virtual base class
    vbptype:    type index of virtual base pointer
    attrib:     attributes
        access:  bits 0 - 1 for access
                 0 no access protection
                 1 private access
                 2 protected access
                 3 public access
        mprop:   bit 2 - 4 for properties
                 0 vanilla
                 1 virtual
                 2 static
                 3 friend
                 4 introducing virtual
                 5 pure virtual
                 6 pure introducing virtual
                 7 reserved
    vbpOffset:   offset of virtual base pointer in derived class
    offset:      offset in derived class, relative to class + vbpOffset

*/

void    BorDebugTypeVBCLASS(BorDebugCookie registerCookie,
                            unsigned int   typeOffset,
                            unsigned int * vbType,
                            unsigned int * vbptype,
                            unsigned int * attrib,
                            unsigned int * vbpOffset,
                            unsigned int * offset);


/*

    Details of BORDEBUG_LF_IVBCLASS   = 0x00000402


    BorDebugTypeIVBCLASS


    Describes the indirect virtual base of a class, with
    the restriction that the trail from the class to the
    virtual base has at least one non-virtual base.  If
    all of the base classes are virtual, the type would
    be BORDEBUG_LF_VBCLASS (0x00000401).


    typeOffset: file offset in bytes of the type
    vbType:     type index of virtual base class
    vbpType:    type index of virtual base pointer
    attrib:     attributes
        access:  bits 0 - 1 for access
                 0 no access protection
                 1 private access
                 2 protected access
                 3 public access
        mprop:   bit 2 - 4 for properties
                 0 vanilla
                 1 virtual
                 2 static
                 3 friend
                 4 introducing virtual
                 5 pure virtual
                 6 pure introducing virtual
                 7 reserved
    vbpOffset:   offset of virtual base pointer in derived class
    offset:      offset in derived class, relative to class + vbpOffset

*/

void    BorDebugTypeIVBCLASS(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * vbType,
                             unsigned int * vbpType,
                             unsigned int * attrib,
                             unsigned int * vbpOffset,
                             unsigned int * offset);


/*

    Details of BORDEBUG_LF_ENUMERATE  = 0x00000403


    BorDebugTypeENUMERATE


    Describes the member (enumerate) of an enum.  The enum type will
    have a type index for its fieldlist, and that fieldlist will
    contain members of the enumerate type.


    typeOffset:    file offset in bytes of the type
    attrib:        not used right now
    name:          name index of enum member
    browserOffset: browser offset of enum member
    value:         value of enum member

*/

void    BorDebugTypeENUMERATE(BorDebugCookie registerCookie,
                              unsigned int   typeOffset,
                              unsigned int * attrib,
                              unsigned int * name,
                              unsigned int * browserOffset,
                              unsigned int * value);


/*

    Details of BORDEBUG_LF_FRIENDFCN  = 0x00000404


    BorDebugTypeFRIENDFCN


    Describes a friend function.


    typeOffset: file offset in bytes of the type
    typeIndex:  type index of the friend function
    name:       name index of friend function

*/

void    BorDebugTypeFRIENDFCN(BorDebugCookie registerCookie,
                              unsigned int   typeOffset,
                              unsigned int * typeIndex,
                              unsigned int * name);


/*

    Details of BORDEBUG_LF_INDEX      = 0x00000405


    BorDebugTypeINDEX


    Describes a continuation type index.  This is used for instance
    in the FIELDLIST type, if the list of fields is too long and
    has to be split into multiple FIELDLIST's.  Each continuation
    type points to the next FIELDLIST.


    typeOffset: file offset in bytes of the type
    return:     continuation type index

*/

unsigned int    BorDebugTypeINDEX(BorDebugCookie registerCookie,
                                  unsigned int   typeOffset);



/*

    Details of BORDEBUG_LF_MEMBER     = 0x00000406


    BorDebugTypeMEMBER


    Describes non-static data members of classes, structs,
    and unions.


    typeOffset:    file offset in bytes of the type
    typeIndex:     type index of member
    attrib:        attributes of member
        access:  bits 0 - 1 for access
                 0 no access protection
                 1 private access
                 2 protected access
                 3 public access
        mprop:   bit 2 - 4 for properties
                 0 vanilla
                 1 virtual
                 2 static
                 3 friend
                 4 introducing virtual
                 5 pure virtual
                 6 pure introducing virtual
                 7 reserved
    name:          name index of member
    offset:        offset of member inside structure
    browserOffset: browser Offset of member

*/

void    BorDebugTypeMEMBER(BorDebugCookie registerCookie,
                           unsigned int   typeOffset,
                           unsigned int * typeIndex,
                           unsigned int * attrib,
                           unsigned int * name,
                           unsigned int * offset,
                           unsigned int * browserOffset);


/*

    Details of BORDEBUG_LF_STMEMBER   = 0x00000407


    BorDebugTypeSTMEMBER


    Describes static data members of classes, structs, and
    unions.  To find the symbol belonging to a static data
    member, find its name, and qualify it with the name of
    its containing class, as in class::staticMember, and
    search for a symbol with this name and the correct
    type index.


    typeOffset:     file offset in bytes of the type
    typeIndex:      type index of static member
    attrib:         attributes of static member
        access:  bits 0 - 1 for access
                 0 no access protection
                 1 private access
                 2 protected access
                 3 public access
        mprop:   bit 2 - 4 for properties
                 0 vanilla
                 1 virtual
                 2 static
                 3 friend
                 4 introducing virtual
                 5 pure virtual
                 6 pure introducing virtual
                 7 reserved
    name:           name index of static member
    browserOffset:  browser Offset of static member

*/

void    BorDebugTypeSTMEMBER(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * typeIndex,
                             unsigned int * attrib,
                             unsigned int * name,
                             unsigned int * browserOffset);


/*

    Details of BORDEBUG_LF_METHOD     = 0x00000408


    BorDebugTypeMETHOD


    Describes the member functions of a class.  The type
    index of the methodlist points to a METHODLIST,
    which contains one or more types of functions.


    typeOffset: file offset in bytes of the type
    count:      number of (overloaded) functions
    methodList: type index of method list
    name:       name index of method

*/

void    BorDebugTypeMETHOD(BorDebugCookie registerCookie,
                           unsigned int   typeOffset,
                           unsigned int * count,
                           unsigned int * methodList,
                           unsigned int * name);



/*

    Details of BORDEBUG_LF_NESTTYPE   = 0x00000409


    BorDebugTypeNESTTYPE


    Describes nested types inside classes, structs, unions, and
    enums.  The containing class will have a member that is of
    type NESTTYPE, and its type index will point to the record
    of the nested type.


    typeOffset:    file offset in bytes of the type
    typeIndex:     type index of nested type
    name:          name index of nested type
    browserOffset: browser offset of nested type

*/

void    BorDebugTypeNESTTYPE(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * typeIndex,
                             unsigned int * name,
                             unsigned int * browserOffset);


/*

    Details of BORDEBUG_LF_VFUNCTAB   = 0x0000040A


    BorDebugTypeVFUNCTAB


    Describes the virtual table pointers in a class.  A class
    can have a member of type VFUNCTAB, which will be a POINTER
    type, and the pointed to type will be a VTSHAPE type.
    It is usually emitted as the first member of a class.


    typeOffset: file offset in bytes of the type
    typeIndex:  type index of vtable pointer
    offset:     offset of vtable pointer in derived class

*/

void    BorDebugTypeVFUNCTAB(BorDebugCookie registerCookie,
                             unsigned int   typeOffset,
                             unsigned int * typeIndex,
                             unsigned int * offset);


/*

    Details of BORDEBUG_LF_FRIENDCLS  = 0x0000040B


    BorDebugTypeFRIENDCLS


    Describes a friend class.  A class can have a member that
    is of type FRIENDCLS, and its type index points to the
    actual type record of the friend class.


    typeOffset: file offset in bytes of the type
    return:     type index of the friend class

*/

unsigned int    BorDebugTypeFRIENDCLS(BorDebugCookie registerCookie,
                                      unsigned int   typeOffset);



/*

    Details of BORDEBUG_LF_CHAR       = 0x00008000


    BorDebugTypeCHAR


    Describes char's and unsigned char's.
    In this implementation you will not encounter this type.


    typeOffset: file offset in bytes of the type
    return:     value of the char

*/

char    BorDebugTypeCHAR(BorDebugCookie registerCookie,
                         unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_SHORT      = 0x00008001


    BorDebugTypeSHORT


    Describes short's.
    In this implementation you will not encounter this type.


    typeOffset: file offset in bytes of the type
    return:     value of the short

*/

short   BorDebugTypeSHORT(BorDebugCookie registerCookie,
                          unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_USHORT     = 0x00008002


    BorDebugTypeUSHORT


    Describes unsigned short's.
    In this implementation you will not encounter this type.


    typeOffset: file offset in bytes of the type
    return:     value of the unsigned short

*/

unsigned short  BorDebugTypeUSHORT(BorDebugCookie registerCookie,
                                   unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_LONG       = 0x00008003


    BorDebugTypeLONG


    Describes long's.
    In this implementation you will not encounter this type.

    typeOffset: file offset in bytes of the type
    return:     value of the long

*/

long    BorDebugTypeLONG(BorDebugCookie registerCookie,
                         unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_ULONG      = 0x00008004


    BorDebugTypeULONG


    Describes unsigned long's.
    In this implementation you will not encounter this type.


    typeOffset: file offset in bytes of the type
    return:     value of the unsigned long

*/

unsigned long   BorDebugTypeULONG(BorDebugCookie registerCookie,
                                  unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_REAL32     = 0x00008005


    BorDebugTypeREAL32


    Describes 32 bit float's.


    typeOffset: file offset in bytes of the type
    return:     value of the real32

*/

float   BorDebugTypeREAL32(BorDebugCookie registerCookie,
                           unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_REAL64     = 0x00008006


    BorDebugTypeREAL64


    Describes 64 bit float's (double's).


    typeOffset: file offset in bytes of the type
    return:     value of the real64

*/

double  BorDebugTypeREAL64(BorDebugCookie registerCookie,
                           unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_REAL80     = 0x00008007


    BorDebugTypeREAL80


    Describes 80 bit float's (long double's).


    typeOffset: file offset in bytes of the type
    return:     value of the real80

*/

long double     BorDebugTypeREAL80(BorDebugCookie registerCookie,
                                   unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_REAL128    = 0x00008008

    Not used

*/


/*

    Details of BORDEBUG_LF_QUADWORD   = 0x00008009


    BorDebugTypeQUADWORD


    Describes 64 bit values.


    typeOffset: file offset in bytes of the type
    return:     value of quad word

*/

__int64 BorDebugTypeQUADWORD(BorDebugCookie registerCookie,
                             unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_UQUADWORD  = 0x0000800A


    BorDebugTypeUQUADWORD


    Describes unsigned 64 bit values.


    typeOffset: file offset in bytes of the type
    return:     value of uquad word

*/

unsigned __int64 BorDebugTypeUQUADWORD(BorDebugCookie registerCookie,
                                       unsigned int   typeOffset);


/*

    Details of BORDEBUG_LF_REAL48     = 0x0000800B


    BorDebugTypeREAL48


    Describes 48 bit Pascal type float's


    typeOffset: file offset in bytes of the type
    return:     value of the real48

*/

double  BorDebugTypeREAL48(BorDebugCookie registerCookie,
                           unsigned int   typeOffset);


//---------------------------------------------------------------------

/*

    sstNames API's

    Use the name index API's to retrieve the names.
    Remember:  the names are 1 - based.

*/


/*

    BorDebugNamesTotalNames

    Return the total number of names in the sstNames subsection

    return:     the total number of names
*/


unsigned int    BorDebugNamesTotalNames(BorDebugCookie registerCookie);



//---------------------------------------------------------------------

/*

    Index API's

*/


/*

    BorDebugNameIndexToUnmangedName


    Go from an index to the name as it appears in the
    debug info, and always unmangle the name if it
    is mangled.
    Buf is assumed to point to a buffer of at least
    bufLen characters.
    A buffer of 1024 char's is enough for most names

    name:   name index
    buf:    points to array of char's
    bufLen: size of buf array

*/


void    BorDebugNameIndexToUnmangledName(BorDebugCookie registerCookie,
                                         unsigned int   name,
                                         char         * buf,
                                         unsigned int   bufLen);

/*

    BorDebugNameIndexToName


    Go from an index to the name as it appears in the
    debug info.  This might be a mangled name.
    Buf is assumed to point to a buffer of at least
    bufLen characters.
    A buffer of 260 char's is enough for all names

    name:   name index
    buf:    points to array of char's
    bufLen: size of buf array

*/

void    BorDebugNameIndexToName(BorDebugCookie registerCookie,
                                unsigned int   name,
                                char         * buf,
                                unsigned int   bufLen);

/*

    BorDebugRegIndexToName


    Go from an index to the name of a register.
    Buf is assumed to point to a buffer of at least
    bufLen characters.
    Ten characters should be enough for register names

    reg:   name index of register
    buf:    points to array of char's
    bufLen: size of buf array

*/

void    BorDebugRegIndexToName(BorDebugCookie registerCookie,
                               unsigned int   reg,
                               char         * buf,
                               unsigned int   bufLen);


/*

    BorDebugTypeIndexToString


    This API is usefull in two cases:
    - If the type index is less than 0x1000, the type is a basic
      (built-in) type, and you can get the string representation
      of this type.
    - If the high bit of type index is turned on, meaning type
      index is negative if cast to an integer, this means that
      the type is an external type.  The index number of this
      external type is simply the type index multiplied by -1.
      This function will return a string representation in the
      form : EI[XXX], where XXX is the external type index.

    For user defined types, where the type index is in the range
    of 0x1000 - 0x7FFFFFFF, this function will simply return
    the type as a string.


    Go from a type index to a string representation
    Buf is assumed to point to a buffer of at least
    bufLen characters.
    A buffer of 260 char's is enough for all type names

    typeIndex: type index
    buf:       points to array of char's
    bufLen:    size of buf array

*/

void    BorDebugTypeIndexToString(BorDebugCookie registerCookie,
                                  unsigned int   typeIndex,
                                  char         * buf,
                                  unsigned int   bufLen);



//---------------------------------------------------------------------

/*

    Unmangling entry point

    BorDebugUmKind  BorDebugUnmangle(char         * src,
                                     char         * dest,
                                     unsigned int   maxlen,
                                     char         * qualP,
                                     char         * baseP,
                                     int            doArgs)


    This is the main entry-point for the unmangler code.  To use it, pass
    the following arguments:

       src      the source buffer, NULL terminated, which contains
                the mangled name.  If this pointer is NULL, unmangle()
                will return UM_NOT_MANGLED.

       dest     the destination buffer.  If this pointer is NULL,
                unmangle() will return UM_ERROR.

       maxlen   the maximum number of bytes which should be output
                to the destination buffer.  Remember to account for
                the NULL that will be output at the end of the mangled
                name.

                It is impossible to know beforehand exactly how long a
                mangled name should be, but due to restrictions in the
                length of linker names, imposed by the OMF format, a
                buffer of at least 2K bytes or longer should always be
                sufficient.

                If the size of the buffer is insufficient, unmangle()
                will return with the flag UM_BUFOVRFLW set in the return
                code.  Any other flags set in the return code will
                reflect whatever information unmangle() was able to
                determine before the overflow occurred.

       qualP    if non-NULL, this argument should point to the address
                of a buffer large enough to contain the qualifier part
                of the unmangled name.  For example, if the unmangled
                name is "foo::bar::baz", then the qualifier would be
                "foo::bar".

                Thus, this buffer should always be at least as large as
                the destination buffer, in order to ensure that memory
                overwrites never occur.

       baseP    if non-NULL, this argument should point to the address
                of a buffer large enough to contain the basename part
                of the unmangled name.  For example, if the unmangled
                name is "foo::bar::baz", then the basename would be
                "baz".  See the documentation of "qualP" for further
                notes on the required length of this buffer.

       doArgs   if this argument is non-0 (aka TRUE), it means that
                when unmangling a function name, its arguments should
                also be unmangled as part of the output name.
                Otherwise, only the name will be unmangled, and not
                the arguments.

    The return code of this function contains a series of flags, some of
    which are mutually exclusive, and some of which represent the status
    of the unmangled name.  These flags are:

        General:
        --------

        0x80000000  BORDEBUG_UM_NOT_MANGLED

            If the return value equals this flag, then
            it is the only flag which will be set, all
            other values being irrelevant.



        The kind of symbol (mutually exclusive):
        ----------------------------------------

        0x00000000  BORDEBUG_UM_UNKNOWN

            Symbol of unknown type

        0x00000001  BORDEBUG_UM_FUNCTION

            Global function, or member function

        0x00000002  BORDEBUG_UM_CONSTRUCTOR

            Class constructor function

        0x00000003  BORDEBUG_UM_DESTRUCTOR

            Class destructor function

        0x00000004  BORDEBUG_UM_OPERATOR

            Global operator, or member operator

        0x00000005  BORDEBUG_UM_CONVERSION

            Member conversion operator

        0x00000006  BORDEBUG_UM_DATA

            Class static data member

        0x00000007  BORDEBUG_UM_THUNK

            (16-bit only, no longer used)

        0x00000008  BORDEBUG_UM_TPDSC

            Type descriptor object (RTTI)

        0x00000009  BORDEBUG_UM_VTABLE

            Class virtual table

        0x0000000a  BORDEBUG_UM_VRDF_THUNK

            Virtual table thunk (special)

        0x000000ff  BORDEBUG_UM_KINDMASK

            This mask can be used to exclude all other
            flags from the return type, except the
            symbol kind.


        Modifiers (not mutually exclusive):
        -----------------------------------

        0x00000100  BORDEBUG_UM_QUALIFIED

            A member symbol, either of a class or of a namespace

        0x00000200  BORDEBUG_UM_TEMPLATE

            A template specialization symbol


        Modifiers (mutually exclusive):
        -------------------------------

        0x00000400  BORDEBUG_UM_VIRDEF_FLAG

            Virdef flag (special)

        0x00000800  BORDEBUG_UM_FRIEND_LIST

            Friend list (special)

        0x00001000  BORDEBUG_UM_CTCH_HNDL_TBL

            Catch handler table (exception handling)

        0x00002000  BORDEBUG_UM_OBJ_DEST_TBL

            Object destructor table (exception handling)

        0x00004000  BORDEBUG_UM_THROW_LIST

            Throw list (exception handling)

        0x00008000  BORDEBUG_UM_EXC_CTXT_TBL

            Exception context table (exception handling)

        0x00010000  BORDEBUG_UM_LINKER_PROC

            Special linker procedure (#pragma package)

        0x0001fc00  BORDEBUG_UM_SPECMASK

            Special flags mask.  Use this to extract only
            these special, mutually exclusive, flags.

        0x00ffff00  BORDEBUG_UM_MODMASK

            This mask can be used to access any of the
            symbol modifiers, whether mutually exclusive
            or not.


        Error flags (not mutually exclusive):
        -------------------------------------

        0x01000000  BORDEBUG_UM_BUFOVRFLW

            The output buffer has been overflowed

        0x02000000  BORDEBUG_UM_HASHTRUNC

            The input name was truncated by a hash code

        0x04000000  BORDEBUG_UM_ERROR

            Some other error has occurred

        0x7f000000  BORDEBUG_UM_ERRMASK

            Use this mask to examine only the error flags



    It is recommended to use the main output buffer.  The "qualP" and
    "baseP" arguments are there mostly for backwards compatibility.

    Note on exceptional conditions: Sometimes a mangled name does not
    have the correct format.  This can happen if garbage code is passed
    in, or a mangled name from a different, or older product, is used.
    In this case, you will notice a number enclosed in curly-braces at
    the point in the name where the fault was detected.

    For example, a false name like "@foo@$z" will generate an error like
    "foo::{853}...", because "$z" does not represent a valid special
    function name.  In this case, the number 853 represents the line in
    UM.C where the error was found.

    If you are debugging a problem with unmangling in a case where
    examining the mangled name under the debugger is not convenient, you
    can tell the unmangler to output the mangled form of the name in the
    output buffer by setting the environment variable SHOW_TROUBLED_NAME
    to any textual value.  In that case, the output buffer for the
    example above would contain the string "foo::{853: @foo@$z}".


*/


typedef enum
{
        /* The kind of symbol. */

        BORDEBUG_UM_UNKNOWN       = 0x00000000,

        BORDEBUG_UM_FUNCTION      = 0x00000001,
        BORDEBUG_UM_CONSTRUCTOR   = 0x00000002,
        BORDEBUG_UM_DESTRUCTOR    = 0x00000003,
        BORDEBUG_UM_OPERATOR      = 0x00000004,
        BORDEBUG_UM_CONVERSION    = 0x00000005,

        BORDEBUG_UM_DATA          = 0x00000006,
        BORDEBUG_UM_THUNK         = 0x00000007,
        BORDEBUG_UM_TPDSC         = 0x00000008,
        BORDEBUG_UM_VTABLE        = 0x00000009,
        BORDEBUG_UM_VRDF_THUNK    = 0x0000000a,

        BORDEBUG_UM_KINDMASK      = 0x000000ff,

        /* Modifier (is it a member, template?). */

        BORDEBUG_UM_QUALIFIED     = 0x00000100,
        BORDEBUG_UM_TEMPLATE      = 0x00000200,

        BORDEBUG_UM_VIRDEF_FLAG   = 0x00000400,
        BORDEBUG_UM_FRIEND_LIST   = 0x00000800,
        BORDEBUG_UM_CTCH_HNDL_TBL = 0x00001000,
        BORDEBUG_UM_OBJ_DEST_TBL  = 0x00002000,
        BORDEBUG_UM_THROW_LIST    = 0x00004000,
        BORDEBUG_UM_EXC_CTXT_TBL  = 0x00008000,
        BORDEBUG_UM_LINKER_PROC   = 0x00010000,
        BORDEBUG_UM_SPECMASK      = 0x0001fc00,

        BORDEBUG_UM_MODMASK       = 0x00ffff00,

        /* Some kind of error occurred. */

        BORDEBUG_UM_BUFOVRFLW     = 0x01000000,
        BORDEBUG_UM_HASHTRUNC     = 0x02000000,
        BORDEBUG_UM_ERROR         = 0x04000000,

        BORDEBUG_UM_ERRMASK       = 0x7f000000,

        /* This symbol is not a mangled name. */

        BORDEBUG_UM_NOT_MANGLED   = 0x80000000,
}
        BorDebugUmKind;


BorDebugUmKind  BorDebugUnmangle(char   *       src,
                                 char   *       dest,
                                 unsigned       maxlen,
                                 char   *       qualP,
                                 char   *       baseP,
                                 int            doArgs);


//---------------------------------------------------------------------


#ifdef  __cplusplus
}   // extern "C"
#endif

#endif  // BORDEBUG_H

//---------------------------------------------------------------------
