#include <curses.h>
#include <tsalis.h>

const char PELEM_LIST [ SPELM_COUNT ][ 5 ] =
{
        "b1p",
        "b1s",
        "b2p",
        "b2s",
        "ip",
        "sp",
        "r1x",
        "r2x",
        "r3x",
        "r4x",
        "s1",
        "s2",
        "s3",
        "s4",
        "s5",
        "s6",
        "s7",
        "s8"
};

suint g_page;

sbool g_showGenes;
suint g_vsProcs;
suint g_hsProcs;
suint g_gsProcs;
suint g_vsWorld;

suint g_wZoom;
suint g_wPos;
suint g_wProc;

suint g_wLineSz;
suint g_wCellNo;
suint g_wMxZoom;

static int g_colLmt;

static void
pStr ( int ln, int cl, const char * str )
{
        int px = ( cl * DCOL_WIDTH ) + 1;
        int ps = px - 2;

        if ( g_colLmt < px ) {
                return;
        }

        mvprintw ( ln, px, cl ? "%10s" : "%-10s", str );

        if ( cl ) {
                mvaddch ( ln, ps, '|');
        }
}

static void
pVal ( int ln, int cl, suint val )
{
        int px = ( cl * DCOL_WIDTH ) + 1;
        int ps = px - 2;

        if ( g_colLmt < px ) {
                return;
        }

        if ( val == SNULL ) {
                mvprintw ( ln, px, cl ? "%10s" : "%-10s", "NULL" );
        } else {
                mvprintw ( ln, px, cl ? "%10u" : "%-10u", val );
        }

        if ( cl ) {
                mvaddch ( ln, ps, '|');
        }
}

static void
pMemory ( void )
{
        attron ( COLOR_PAIR ( 1 ));
        pStr ( 7, 0, "MEMORY" );
        standend ();

        pStr (  8, 0,   "order" );
        pStr (  9, 0,    "size" );
        pStr ( 10, 0,    "used" );
        pStr ( 11, 0, "min bsz" );
        pStr ( 12, 0, "max bsz" );

        pVal (  8, 1,    sm_getOrder ());
        pVal (  9, 1,     sm_getSize ());
        pVal ( 10, 1,     sm_getUsed ());
        pVal ( 11, 1, sm_getMinBSize ());
        pVal ( 12, 1, sm_getMaxBSize ());
}

static void
pGenome ( suint pidx, suint ln )
{
        SProc * pr = sp_getProc ( pidx );
        suint   mi = g_gsProcs;
        suint   wc = 15;

        move ( ln, wc );

        while (( mi < pr -> b1s ) && ( wc < ( suint ) COLS )) {
                suint ma = pr -> b1p + mi;

                if ( ma == pr -> sp ) {
                        attron ( COLOR_PAIR ( 6 ));
                } else if ( ma == pr -> ip ) {
                        attron ( COLOR_PAIR ( 7 ));
                } else {
                        attron ( COLOR_PAIR ( 5 ));
                }

                addch ( sc_instToSymb ( sm_getInst ( ma )));

                mi ++;
                wc ++;
        }

        if (( pr -> b1s ) < g_gsProcs ) {
                mi = g_gsProcs - ( pr -> b1s );
        } else {
                mi = 0;
        }

        while (( mi < pr -> b2s ) && ( wc < ( suint ) COLS )) {
                suint ma = pr -> b2p + mi;

                if ( ma == pr -> sp ) {
                        attron ( COLOR_PAIR ( 6 ));
                } else if ( ma == pr -> ip ) {
                        attron ( COLOR_PAIR ( 7 ));
                } else {
                        attron ( COLOR_PAIR ( 4 ));
                }

                addch ( sc_instToSymb ( sm_getInst ( ma )));

                mi ++;
                wc ++;
        }
}

static void
pProcGenes ( void )
{
        suint li;
        suint pi;

        attron ( COLOR_PAIR ( 1 ));
        pStr ( 17, 0, "pidx" );
        standend ();

        pStr ( 18, 0, "---" );

        li = 19;
        pi = g_vsProcs;

        while ( 1 ) {
                if ( li >= LINES - 1u )  { break; }
                if ( pi >= sp_getCap ()) { break; }

                if ( !sp_isFree ( pi )) {
                        attron ( COLOR_PAIR ( 1 ));
                }

                if ( pi == g_wProc ) {
                        attron ( A_BOLD );
                }

                pVal ( li, 0, pi );
                printw ( " | " );

                if ( !sp_isFree ( pi )) {
                        pGenome ( pi, li );
                }

                standend ();

                li ++;
                pi ++;
        }
}

static void
pProcList ( void )
{
        suint ei;
        suint cl;
        suint li;
        suint pi;

        attron ( COLOR_PAIR ( 1 ));
        pStr ( 17, 0, "pidx" );

        cl = 1;
        ei = g_hsProcs;

        while ( ei < SPELM_COUNT ) {
                pStr ( 17, cl, PELEM_LIST [ ei ] );
                cl ++;
                ei ++;
        }

        standend ();

        pStr ( 18, 0, "---" );

        li = 19;
        pi = g_vsProcs;

        while ( 1 ) {
                if ( li >= LINES - 1u )  { break; }
                if ( pi >= sp_getCap ()) { break; }

                if ( !sp_isFree ( pi )) {
                        attron ( COLOR_PAIR ( 1 ));
                }

                if ( pi == g_wProc ) {
                        attron ( A_BOLD );
                }

                pVal ( li, 0, pi );

                cl = 1;
                ei = g_hsProcs;

                while ( ei < SPELM_COUNT ) {
                        pVal ( li, cl, sp_getPData ( pi ) [ ei ] );
                        cl ++;
                        ei ++;
                }

                standend ();

                li ++;
                pi ++;
        }
}

static void
pProcs ( void )
{
        attron ( COLOR_PAIR ( 1 ));
        pStr ( 7, 0, "PROCESS" );
        standend ();

        pStr (  8, 0, "count" );
        pStr (  9, 0,   "cap" );
        pStr ( 10, 0, "first" );
        pStr ( 11, 0,  "last" );

        pVal (  8, 1, sp_getCount ());
        pVal (  9, 1,   sp_getCap ());
        pVal ( 10, 1, sp_getFirst ());
        pVal ( 11, 1,  sp_getLast ());

        attron ( COLOR_PAIR ( 1 ));
        pStr ( 13, 0, "LIST" );
        standend ();

        pStr ( 14, 0, "vscroll" );
        pStr ( 15, 0, g_showGenes ? "gscroll" : "hscroll" );

        pVal ( 14, 1, g_vsProcs );
        pVal ( 15, 1, g_showGenes ? g_gsProcs : g_hsProcs );

        if ( g_showGenes ) {
                pProcGenes ();
        } else {
                pProcList ();
        }
}

static void
pEvolver ( void )
{
        attron ( COLOR_PAIR ( 1 ));
        pStr ( 7, 0, "EVOLVER" );
        standend ();

        pStr (  8, 0, "we rate" );
        pStr (  9, 0, "we next" );
        pStr ( 10, 0, "cr rate" );
        pStr ( 11, 0, "cr next" );
        pStr ( 12, 0, "pm rate" );
        pStr ( 13, 0, "pm next" );

        pVal (  8, 1, se_getWRate ());
        pVal (  9, 1, se_getWNext ());
        pVal ( 10, 1, se_getCRate ());
        pVal ( 11, 1, se_getCNext ());
        pVal ( 12, 1, se_getPRate ());
        pVal ( 13, 1, se_getPNext ());
}

static void
incState ( sbyte * cs, sbyte tv )
{
        if ( tv > *cs ) {
                *cs = tv;
        }
}

static void
pCell ( suint y, suint x, suint addr )
{
        SProc * pr = NULL;
        sbool   pf = SFALSE;
        sbyte   cs = CS_FREE;
        suint   ci = 0;
        suint   in = 0;
        sbyte   il = ' ';

        if ( sp_getCount ()) {
                pr = sp_getProc ( g_wProc );
                pf = sp_isFree ( g_wProc );
        }

        for (; ci < g_wZoom; ci ++ ) {
                suint sadr = addr + ci;

                if ( !sm_isValid ( sadr )) {
                        break;
                }

                if ( pr ) {
                        if ( !pf && sadr == pr -> ip ) {
                                cs = CS_SELIP;
                                break;
                        } else if ( !pf && sadr == pr -> sp ) {
                                incState ( &cs, CS_SELSP );
                        } else if ( !pf && sp_isOnMB1 ( g_wProc, sadr )) {
                                incState ( &cs, CS_MB1 );
                        } else if ( !pf && sp_isOnMB2 ( g_wProc, sadr )) {
                                incState ( &cs, CS_MB2 );
                        } else if ( sm_isUsed ( sadr )) {
                                incState ( &cs, CS_ALLOC );
                        }
                }
        }

        for (ci = 0; ci < g_wZoom; ci++ ) {
                suint sadr = addr + ci;

                if ( !sm_isValid ( sadr )) {
                        break;
                }

                in += sm_getInst ( sadr );
        }

        if ( g_wZoom == 1 ) {
                il = sc_instToSymb ( in );
        } else {
                in /= g_wZoom;

                if ( in > (( SINST_COUNT / 3 ) * 2 )) {
                        il = 240;
                } else if ( in > ( SINST_COUNT / 3 )) {
                        il = '=';
                } else if ( in ) {
                        il = '-';
                }
        }

        attron ( COLOR_PAIR ( 2 + cs ));
        mvaddch ( y, x, il );
        standend ();
}

static void
pWorld ( void )
{
        suint ms = sm_getSize ();
        suint ar = g_wCellNo * g_wZoom;
        suint we = g_wPos + ar;
        int   y;
        int   x;

        if ( we > ms ) { we = ms; }

        attron ( COLOR_PAIR ( 1 ));
        pStr ( 7, 0, "WORLD VIEW" );
        standend ();

        pStr (  8, 0,      "beg" );
        pStr (  9, 0,      "end" );
        pStr ( 10, 0,     "zoom" );
        pStr ( 11, 0, "sel proc" );

        pVal (  8, 1,  g_wPos );
        pVal (  9, 1,      we );
        pVal ( 10, 1, g_wZoom );
        pVal ( 11, 1, g_wProc );

        if ( sp_getCount ()) {
                suint li;
                suint ei;

                attron ( COLOR_PAIR ( 1 ));
                pStr ( 13, 0, "SELECTED PROC" );
                standend ();

                pStr ( 14, 0, "vscroll" );
                pVal ( 14, 1, g_vsWorld );

                attron ( COLOR_PAIR ( 1 ));
                pStr ( 16, 0, "field" );
                pStr ( 16, 1, "val" );
                standend ();

                pStr ( 17, 0, "---" );

                li = 18;
                ei = g_vsWorld;

                while ( 1 ) {
                        if ( li >= LINES - 1u )  { break; }
                        if ( ei >= SPELM_COUNT ) { break; }

                        pStr ( li, 0, PELEM_LIST [ ei ] );
                        pVal ( li, 1, sp_getPData ( g_wProc ) [ ei ] );

                        li ++;
                        ei ++;
                }
        }

        for ( y = 0; y < LINES; y ++ ) {
                for ( x = 0; x < ( int ) g_wLineSz; x ++ ) {
                        suint wp = x + DATA_MARGIN;
                        suint lr = y * g_wLineSz;
                        suint of = x + lr;
                        suint zo = of * g_wZoom;
                        suint ca = zo + g_wPos;

                        if ( sm_isValid ( ca )) {
                                pCell ( y, wp, ca );
                        } else {
                                return;
                        }
                }
        }
}

void
tp_print ( void )
{
        clear ();

        g_colLmt = COLS - DCOL_WIDTH;

        attron ( COLOR_PAIR ( 1 ));
        pStr ( 1, 0, "SALIS" );
        standend ();

        pStr ( 2, 0,  "updates" );
        pStr ( 3, 0,    "epoch" );
        pStr ( 4, 0,    "state" );
        pStr ( 5, 0, "cycls/pr" );

        pVal ( 2, 1, s_getCycles ());
        pVal ( 3, 1,  s_getEpoch ());

        pStr ( 4, 1, g_running ? "running" : "paused" );
        pVal ( 5, 1, g_frmStep );

        switch ( g_page ) {
        case PG_MEMORY:
                pMemory ();
                break;
        case PG_PROCS:
                pProcs ();
                break;
        case PG_EVOLVER:
                pEvolver ();
                break;
        case PG_WORLD:
                pWorld ();
                break;
        }

        refresh ();
}

void
tp_pHelp ( void )
{}