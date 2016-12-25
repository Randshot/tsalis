#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <tsalis.h>

#define INPUT_LENGTH 64

static void
flipPage ( void )
{
        g_page ++;
        g_page %= PAGE_COUNT;
}

static void
scrollPage ( int evnt )
{
        suint * vs = NULL;
        suint * hs = NULL;
        suint   vm = 0;
        suint   hm = 0;

        switch ( g_page ) {
        case PG_PROCS:
                vs = &g_vsProcs;
                hs = g_showGenes ? &g_gsProcs : &g_hsProcs;
                vm = sp_getCap ();
                hm = g_showGenes ? -1 : SPELM_COUNT;
                break;
        case PG_WORLD:
                vs = &g_vsWorld;
                vm = SPELM_COUNT;
                break;
        }

        switch ( evnt ) {
        case KEY_UP:
                if ( vs && *vs < vm ) {
                        ( *vs ) ++;
                }
                break;
        case KEY_LEFT:
                if ( hs && *hs ) {
                        ( *hs ) --;
                }
                break;
        case KEY_DOWN:
                if ( vs && *vs ) {
                        ( *vs ) --;
                }
                break;
        case KEY_RIGHT:
                if ( hs && *hs < hm ) {
                        ( *hs ) ++;
                }
                break;
        case '\b':
                if ( vs ) { *vs = 0; }
                if ( hs ) { *hs = 0; }
        }
}

static void
panWorld ( int evnt )
{
        if ( g_page != PG_WORLD ) {
                return;
        }

        th_setVars ();

        switch ( evnt ) {
        case 'w':
                g_wZoom *= 2;
                break;
        case 'a':
                g_wPos -= g_wZoom;
                break;
        case 's':
                g_wZoom /= 2;
                break;
        case 'd':
                g_wPos += g_wZoom;
                break;
        case 'W':
                g_wPos += g_wZoom * g_wLineSz;
                break;
        case 'S':
                g_wPos -= g_wZoom * g_wLineSz;
                break;
        case 'q':
                g_wPos = 0;
                break;
        case 'k':
                if ( !sp_isFree ( g_wProc )) {
                        g_wPos = sp_getProc ( g_wProc )-> b1p;
                }
                break;
        }

        switch ( evnt ) {
        case 'w':
                if ( g_wZoom > g_wMxZoom ) {
                        g_wZoom = g_wMxZoom;
                }
                break;
        case 's':
                if ( !g_wZoom ) {
                        g_wZoom = 1;
                }
                break;
        case 'a':
        case 'S':
                if ( !sm_isValid ( g_wPos )) {
                        g_wPos = 0;
                }
                break;
        case 'd':
        case 'W':
                if ( !sm_isValid ( g_wPos )) {
                        g_wPos = sm_getSize ();
                }
                break;
        }
}

static void
flipProc ( sbool up )
{
        g_wProc += sp_getCap ();
        g_wProc += ( up ? 1 : -1 );
        g_wProc %= sp_getCap ();
}

static void
setInst ( char * cm )
{
        suint  ad = atoi ( cm );
        char * sq = strchr ( cm, '_' );

        if ( !sq ) {
                return;
        }

        sq ++;

        while ( sm_isValid ( ad ) && *sq ) {
                sbyte vl;

                if ( sc_symbValid ( *sq )) {
                        vl = sc_symbToInst ( *sq );
                } else {
                        vl = 0;
                }

                sm_setInst ( ad, vl );

                ad ++;
                sq ++;
        }
}


static void
compile ( char * cm )
{
        suint  ad = atoi ( cm );
        char * sq = strchr ( cm, '_' );

        if ( !sq ) {
                return;
        }

        sq ++;

        sc_compile ( sq, ad );
}

static void
create ( char * cm )
{
        suint  mi;
        suint  sz;
        suint  ad = atoi ( cm );
        char * sq = strchr ( cm, '_' );

        if ( !sq ) {
                return;
        }

        sq ++;
        sz = atoi ( sq );

        if ( sz < sm_getMinBSize () || sz > sm_getMaxBSize ()) {
                return;
        }

        if ( !sm_isValid ( ad ) || !sm_isValid ( ad + sz - 1 )) {
                return;
        }

        for ( mi = 0; mi < sz; mi ++ ) {
                if ( sm_isUsed ( ad + mi )) {
                        return;
                }
        }

        sp_create ( ad, sz, 0, SFALSE );
}

static void
kill ( void )
{
        if ( sp_getFirst () != SNULL ) {
                sp_kill ();
        }
}

static void
pScroll ( char * cm )
{
        suint pidx = atoi ( cm );

        if ( pidx > sp_getCap ()) {
                pidx = sp_getCap ();
        }

        g_vsProcs = pidx;
}

static void
selProc ( char * cm )
{
        suint pidx = atoi ( cm );

        if ( pidx < sp_getCap ()) {
                g_wProc = pidx;
        }
}


static void
setERate ( char * cm )
{
        switch ( cm [ 0 ] ) {
        case 'w':
                se_setWRate ( atoi ( &cm [ 1 ] ));
                break;
        case 'c':
                se_setCRate ( atoi ( &cm [ 1 ] ));
                break;
        case 'p':
                se_setPRate ( atoi ( &cm [ 1 ] ));
                break;
        }
}

static void
setBSize ( char * cm )
{
        switch ( cm [ 0 ] ) {
        case 'm':
                sm_setMinBSize ( atoi ( &cm [ 1 ] ));
                break;
        case 'x':
                sm_setMaxBSize ( atoi ( &cm [ 1 ] ));
                break;
        }
}

static void
saveSim ( char * cm )
{
        sl_save ( cm );
}

static void
runConsole ( void )
{
        char cm [ INPUT_LENGTH ] = { 0 };

        echo ();
        mvprintw ( LINES - 1, 1, "$ " );
        curs_set ( STRUE );
        getnstr ( cm, INPUT_LENGTH - 1 );
        curs_set ( SFALSE );
        noecho ();

        switch ( cm [ 0 ] ) {
        case 'i':
                setInst ( &cm [ 1 ] );
                break;
        case 'c':
                compile ( &cm [ 1 ] );
                break;
        case 'n':
                create ( &cm [ 1 ] );
                break;
        case 'k':
                kill ();
                break;
        case 's':
                pScroll ( &cm [ 1 ] );
                break;
        case 'p':
                selProc ( &cm [ 1 ] );
                break;
        case 'e':
                setERate ( &cm [ 1 ] );
                break;
        case 'b':
                setBSize ( &cm [ 1 ] );
                break;
        case 'S':
                saveSim ( &cm [ 1 ] );
                break;
        }
}


void
th_setVars ( void )
{
        g_wLineSz = COLS - DATA_MARGIN;
        g_wCellNo = LINES * g_wLineSz;
        g_wMxZoom = 1;

        while (( g_wCellNo * g_wMxZoom ) < sm_getSize ()) {
                g_wMxZoom *= 2;
        }
}

sbool
th_handle ( int evnt )
{
        sbool act = STRUE;

        switch ( evnt ) {
        case '\n':
                flipPage ();
                break;
        case KEY_UP:
        case KEY_LEFT:
        case KEY_DOWN:
        case KEY_RIGHT:
        case '\b':
                scrollPage ( evnt );
                break;
        case 'g':
                if ( g_page == PG_PROCS ) {
                        g_showGenes = !g_showGenes;
                }
                break;
        case 'w':
        case 'a':
        case 's':
        case 'd':
        case 'W':
        case 'S':
        case 'q':
        case 'k':
                panWorld ( evnt );
                break;
        case 'o':
                flipProc ( SFALSE );
                break;
        case 'p':
                flipProc ( STRUE );
                break;
        case 'f':
                if ( sp_getCount ()) {
                        g_vsProcs = sp_getFirst ();
                        g_wProc   = sp_getFirst ();
                }
                break;
        case 'l':
                if ( sp_getCount ()) {
                        g_vsProcs = sp_getLast ();
                        g_wProc   = sp_getLast ();
                }
                break;
        case 'h':
                tp_pHelp ();
                break;
        case 'z':
                if ( g_frmStep > 1 ) {
                        g_frmStep /= 2;
                }
                break;
        case 'x':
                g_frmStep *= 2;
                break;
        case KEY_F ( 1 ):
                runConsole ();
                break;
        case '1':
                s_cycle ();
                break;
        case ' ':
                g_running = !g_running;
                nodelay ( stdscr, g_running );
                break;
        case ERR:
                act = SFALSE;
                break;
        case KEY_END:
                return SFALSE;
        }

        if ( g_running ) {
                suint ci = g_frmStep;

                while ( ci ) {
                        s_cycle ();
                        ci --;
                }

                if ( act ) {
                        while ( getch () != ERR );
                }
        }

        return STRUE;
}