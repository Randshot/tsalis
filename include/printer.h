#ifndef TSALIS_PRINTER_H
#define TSALIS_PRINTER_H

#define PAGE_COUNT  4
#define DCOL_WIDTH  13
#define DATA_MARGIN 26

enum Pages
{
        PG_MEMORY,
        PG_PROCS,
        PG_EVOLVER,
        PG_WORLD
};

enum CellState
{
        CS_FREE,
        CS_ALLOC,
        CS_MB2,
        CS_MB1,
        CS_SELSP,
        CS_SELIP
};

extern const char PELEM_LIST [ SPELM_COUNT ][ 5 ];

extern suint g_page;

extern sbool g_showGenes;
extern suint g_vsProcs;
extern suint g_hsProcs;
extern suint g_gsProcs;
extern suint g_vsWorld;

extern suint g_wZoom;
extern suint g_wPos;
extern suint g_wProc;

extern suint g_wLineSz;
extern suint g_wCellNo;
extern suint g_wMxZoom;

void tp_print ( void );
void tp_pHelp ( void );

#endif