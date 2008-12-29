// include file for the Enhanced GUI debugger frontend

#ifndef BX_WENH_DBG_H
#define BX_WENH_DBG_H

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

// User Customizable initial settings:

extern bx_bool SeeReg[8];

extern bx_bool SingleCPU;       // Display all SMP CPUs
extern bx_bool ShowIOWindows;   // Display the Input and Output Internal Debugger windows
extern bx_bool ShowButtons;     // Display the top-row Step/Continue pushbuttons
extern bx_bool SeeRegColors;    // Display registers with background color "groups"
extern bx_bool ignoreNxtT;      // Do not show "Next at t=" output lines
extern bx_bool ignSSDisasm;     // Do not show extra disassembly line at each break
extern int UprCase;             // 1 = convert all Asm, Register names, Register values to uppercase
extern int DumpInAsciiMode;     // bit 1 = show ASCII in dumps, bit 2 = show hex, value=0 is illegal

extern bx_bool isLittleEndian;
extern int DefaultAsmLines;      // default # of asm lines disassembled and "cached"
extern int bottommargin;         // ASM autoscroller tries to leave this many lines below
extern int topmargin;            // autoscroller tries to leave this many lines above
// Note: topmargin must be less than bottommargin

// How to use DockOrder: the Register list is window 1, ASM is window 2, MemDump is window 3
// Create the hex value with the digits in the order you want the windows to be.
// 0x312 would have MemDump on the left, Register in the middle, ASM on the right
extern short DockOrder;        // set the default List "docking" (Reg, ASM, Dump)

#define STACK_ENTRIES       50 // # of words/dwords/qwords to display in stack window
// Note: STACK_ENTRIES must be <= 512

// END of User Customizable settings

#include <math.h>

#define MAX_ASM         2048    // max entries in ASM window

// Note: all these BSS global variables are initialized to 0 automatically
extern bx_bool SA_valid;
extern Bit64u SelectedDataAddress;
extern Bit64u CurrentAsmLA;    // = EIP/RIP -- for highlighting in ASM window
extern Bit64u BottomAsmLA;     // beginning and end addrs on ASM window
extern Bit64u TopAsmLA;

extern Bit32u PrevStepNSize;  // cpu_loop control variables
//Bit32u RequestedLoops;      // number of cpu_loops the user is asking for
//bx_bool DoBreak;            // stop a local subthreaded cpu_loop
extern Bit64u PrevPtime;      // any number other than 0
extern Bit64u NewPtime;       // used to test whether sim has "updated"
extern unsigned TotCPUs;      // # of CPUs in a multi-CPU simulation
extern unsigned CurrentCPU;   // cpu that is being displayed

// AskTest stuff -- for the generic dialog box
struct ASKTEXT {
    char* title;
    char* prompt;
    char* reply;
};

extern struct ASKTEXT ask_str;

// window resizing/docking stuff
extern int OneCharWide;    // average width of a char in current font (pixels)
extern int Sizing;         // current "resizing/docking mode"
extern int Resize_HiX;     // horizontal limits of the current resize operation (pixels)
extern int Resize_LoX;
extern unsigned ListWidthPix[3]; // set initial proportions of Reg, Asm, Dump windows
extern unsigned LstTop;
extern int CurCenterList;
extern int SizeList;             // pre-docking list #
extern bx_bool DumpHasFocus;
extern Bit32s xClick;            // halfway through a mouseclick flag + location
extern Bit32s yClick;            // values are in Listview coordinates
extern Bit32u CurTimeStamp;      // last mousedown time
// BarClix holds the x-axis position (in pixels or logical units) of the two resizing bars,
// in parent coordinates (ie. any window that contains the lists)
extern unsigned short BarClix[2];

extern bx_bool AtBreak;        // Status indicators
extern bx_bool PrevAtBreak;
extern bx_bool CpuModeChange;
extern bx_bool StatusChange;

extern bx_bool In64Mode;       // CPU modes
extern bx_bool In32Mode;
extern unsigned CpuMode;
extern Bit32u InPaging;        // Storage for the top bit of CR0, unmodified

extern bx_bool doOneTimeInit;  // Internal flags
extern bx_bool ResizeColmns;   // address/value column autosize flag
extern bx_bool FWflag;         // friendly warning has been shown to user once already
extern int StackSized;         // autosize flag for stack window

extern char *PrevStack;        // buffer for testing changes in stack values
extern Bit64u PStackLA;        // to calculate alignment between prev and current stack
extern bx_bool StackEntChg[STACK_ENTRIES];     // flag for "change detected" on each stack line
extern bx_bool StkInvOnce;     // sometimes need to specially invalidate the stack window

// List Window indexes = DockOrder window numbers -1
#define REG_WND     0
#define ASM_WND     1
#define DUMP_WND    2

// DViewModes
#define VIEW_MEMDUMP    0
#define VIEW_GDT        1
#define VIEW_IDT        2
#define VIEW_PAGING     3
#define VIEW_STACK      4
#define VIEW_BREAK      5
#define VIEW_PTREE      6

extern bx_bool DumpInitted;    // has the MemDump window ever been filled with data?
extern int DumpAlign;
extern int PrevDAD;            // saves "previous DumpAlign value" (forces column autosize)
extern char *DataDump;
extern Bit64u DumpStart;       // current emulated address (lin or phys) of DataDump
extern bx_bool doDumpRefresh;
extern int DViewMode;
extern bx_bool LinearDump;     // FALSE = memdump uses physical addressing

extern char *tmpcb;                // 512b is allocated in bigbuf
extern char *AsmData;              // 5K for binary disassembly data
extern char *CurStack;             // Stack workspace (400b usually)
extern char *AsciiHex;             // Unsigned char to printable hex xlat table

#define outbufSIZE      40960
extern char bigbuf[outbufSIZE];        // 40K preallocated storage for all char buffers (see DoAllInit)
extern char *DbgAppendPtr;
extern char *OutWindow;                // buffer for the Output window
#define OutWinSIZE  10240
extern int OutWinCnt;              // available size of OutWindow buffer
extern int PO_Tdelay;              // delay before displaying partial output lines

extern int AsmLineCount;           // # of disassembled asm lines loaded
extern int AsmPgSize;
extern int ListLineRatio;          // number of vertical pixels in a ListView Item
extern int ListVerticalPix; // number of vertical pixels in each List
extern Bit64u AsmLA[MAX_ASM];             // linear address of each disassembled ASM line

// Command stuff
extern int CommandHistoryIdx;
#define CmdHistorySize 64
extern char *CmdHistory[CmdHistorySize];   // 64 command History storage (fixed 80b each)
extern int CmdHInsert;                     // index of next history entry to store

// register storage -- names and values
#define RAX_Rnum    0
#define RBX_Rnum    1
#define RCX_Rnum    2
#define RDX_Rnum    3
#define RSI_Rnum    4
#define RDI_Rnum    5
#define RBP_Rnum    6
#define RSP_Rnum    7
#define RIP_Rnum    8
#define R8_Rnum     9
#define R9_Rnum     10
#define R10_Rnum    11
#define R11_Rnum    12
#define R12_Rnum    13
#define R13_Rnum    14
#define R14_Rnum    15
#define R15_Rnum    16
#define EFL_Rnum    17      // it makes the most sense if EFLAGS is between RXX and EXX
#define EAX_Rnum    18      // because it "belongs with" both sets of GP registers
#define EBX_Rnum    19
#define ECX_Rnum    20
#define EDX_Rnum    21
#define ESI_Rnum    22
#define EDI_Rnum    23
#define EBP_Rnum    24
#define ESP_Rnum    25
#define EIP_Rnum    26
#define CS_Rnum     27
#define DS_Rnum     28
#define ES_Rnum     29
#define SS_Rnum     30
#define FS_Rnum     31
#define GS_Rnum     32
#define GDTRnum     33
#define IDTRnum     34
#define LDTRnum     35
#define TRRnum      36
#define CR0_Rnum    37
#define CR3_Rnum    38
#define CR4_Rnum    39
#define EFER_Rnum   40      // currently used to mark the last entry in rV[]
#define ST0_Rnum    41
#define ST1_Rnum    42
#define ST2_Rnum    43
#define ST3_Rnum    44
#define ST4_Rnum    45
#define ST5_Rnum    46
#define ST6_Rnum    47
#define ST7_Rnum    48
#define XMM0_Rnum   49
#define XMM1_Rnum   50
#define XMM2_Rnum   51
#define XMM3_Rnum   52
#define XMM4_Rnum   53
#define XMM5_Rnum   54
#define XMM6_Rnum   55
#define XMM7_Rnum   56
#define XMM8_Rnum   57
#define XMM9_Rnum   58
#define XMMA_Rnum   59
#define XMMB_Rnum   60
#define XMMC_Rnum   61
#define XMMD_Rnum   62
#define XMME_Rnum   63
#define XMMF_Rnum   64
#define DR0_Rnum    65
#define DR1_Rnum    66
#define DR2_Rnum    67
#define DR3_Rnum    68
#define DR6_Rnum    69
#define DR7_Rnum    70
// #define TR0_Rnum 71  -- put Test Registers in here when they are supported
#define TOT_REG_NUM 72
// extra "register numbers" for registers that have more than one part
#define GDTR_Lim    72
#define IDTR_Lim    73
#define ST0_exp     74
#define ST1_exp     75
#define ST2_exp     76
#define ST3_exp     77
#define ST4_exp     78
#define ST5_exp     79
#define ST6_exp     80
#define ST7_exp     81
#define XMM0_hi     82
#define XMM1_hi     82
#define XMM2_hi     83
#define XMM3_hi     84
#define XMM4_hi     85
#define XMM5_hi     86
#define XMM6_hi     87
#define XMM7_hi     88
#define XMM8_hi     89
#define XMM9_hi     90
#define XMMA_hi     91
#define XMMB_hi     92
#define XMMC_hi     93
#define XMMD_hi     94
#define XMME_hi     95
#define XMMF_hi     96
#define EXTRA_REGS  28

extern char* RDispName[EFER_Rnum + 1];
extern bx_param_num_c *RegObject[BX_MAX_SMP_THREADS_SUPPORTED][TOT_REG_NUM + EXTRA_REGS];
extern Bit64u rV[EFER_Rnum + 1];   // current values of registers
extern Bit64u PV[EFER_Rnum + 1];   // previous values of registers
extern Bit32s GDT_Len;             // "limits" (= bytesize-1) for GDT and IDT
extern Bit32s IDT_Len;
extern Bit8u RegColor[TOT_REG_NUM];    // specifies foreground and background color of registers
// Text color is red if the upper bit is set. Background is set according to ColorList.
extern int RitemToRnum[TOT_REG_NUM];   // mapping from Reg List Item# to register number

extern Bit64u ladrmin; // bochs linear addressing access variables
extern Bit64u ladrmax;
extern Bit64u l_p_offset;

// do the linear breakpoint list as 2 arrays, rather than a structure -- much easier to search!
extern bx_address BrkLAddr[BX_DBG_MAX_LIN_BPOINTS];
extern unsigned BrkIdx[BX_DBG_MAX_LIN_BPOINTS];
extern int BreakCount;

// Breakpoint Dump Window stuff
extern unsigned short BrkpIDMap[256];
extern unsigned short WWP_BaseEntry;
extern unsigned short RWP_BaseEntry;
extern unsigned short EndLinEntry;
extern unsigned short EndPhyEntry;
extern unsigned short WWPSnapCount;
extern unsigned short RWPSnapCount;
extern bx_phy_address WWP_Snapshot[16];
extern bx_phy_address RWP_Snapshot[16];
extern char UCtable[256];

#endif