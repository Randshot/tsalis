#include <stdlib.h>
#include <curses.h>
#include <tsalis.h>

#define DEF_ORDER 16

sbool        g_running;
suint        g_frmStep;
const char * g_fileName = "def.salis";

sbool
t_init ( int argc, const char * argv [] )
{
        FILE * test;

        /* parse arguments */
        switch ( argc ) {
        case 1:
                test = fopen ( g_fileName, "r" );

                if ( test ) {
                        fclose  ( test );
                        sl_load ( g_fileName );
                } else {
                        s_init ( DEF_ORDER );
                }

                break;
        case 2:
                switch ( argv [ 1 ][ 0 ] ) {
                case 'n':
                        s_init ( atoi ( &argv [ 1 ][ 1 ] ));
                        break;
                case 'l':
                        g_fileName = &argv [ 1 ][ 1 ];
                        sl_load ( g_fileName );
                        break;
                default:
                        return SFALSE;
                }

                break;
        default:
                return SFALSE;
        }

        initscr  ();
        cbreak   ();
        noecho   ();
        curs_set ( 0 );
        keypad   ( stdscr, STRUE );

        start_color ();
        init_pair ( 1, COLOR_CYAN,   COLOR_BLACK ); /*    header */
        init_pair ( 2, COLOR_CYAN,    COLOR_BLUE ); /* free cell */
        init_pair ( 3, COLOR_BLUE,    COLOR_CYAN ); /* used cell */
        init_pair ( 4, COLOR_BLUE,   COLOR_GREEN ); /*  mb2 cell */
        init_pair ( 5, COLOR_BLUE,  COLOR_YELLOW ); /*  mb1 cell */
        init_pair ( 6, COLOR_BLUE, COLOR_MAGENTA ); /*   sp cell */
        init_pair ( 7, COLOR_BLUE,   COLOR_WHITE ); /*   ip cell */

        th_setVars ();

        g_wZoom   = 1;
        g_frmStep = 1;

        return STRUE;
}

void
t_exec ( void )
{
        do    { tp_print (); }
        while ( th_handle ( getch ()));
}

void
t_quit ( void )
{
        sl_save ( g_fileName );
        s_quit  ();
        endwin  ();
}

int
main ( int argc, const char * argv [] )
{
        if ( !t_init ( argc, argv )) {
                return 1;
        }

        t_exec ();
        t_quit ();

        return 0;
}