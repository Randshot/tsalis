#ifndef TSALIS_H
#define TSALIS_H

#include <salis.h>
#include <printer.h>
#include <handler.h>

extern sbool        g_running;
extern suint        g_frmStep;
extern const char * g_fileName;

sbool t_init ( int argc, const char * argv [] );
void  t_exec ( void );
void  t_quit ( void );

#endif