/*
 *  Copyright � 2006-2012 SplinterGU (Fenix/Bennugd)
 *  Copyright � 2002-2006 Fenix Team (Fenix)
 *  Copyright � 1999-2002 Jos� Luis Cebri�n Pag�e (Fenix)
 *
 *  This file is part of Bennu - Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

/*
 * INCLUDES
 */

#ifdef _WIN32
#define  _WIN32_WINNT 0x0500
#include <windows.h>
#endif

#ifdef __SWITCH__
#include <switch.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bgdi.h"
#include "bgdrtm.h"
#include "xstrings.h"
#include "dirs.h"
#if defined(TARGET_IOS)
#include <SDL.h>
#elif defined(TARGET_WII)
#include <SDL.h>
#include <fat.h>
#elif defined(TARGET_PSP)
#include "psp.h"
#endif

/* ---------------------------------------------------------------------- */

static char * dcb_exts[] = { ".dcb", ".dat", ".bin", NULL };

static int standalone  = 0;  /* 1 only if this is an standalone interpreter   */
static int embedded    = 0;  /* 1 only if this is a stub with an embedded DCB */

/* ---------------------------------------------------------------------- */

/*
 *  FUNCTION : main
 *
 *  Main function
 *
 *  PARAMS:
 *      INT n: ERROR LEVEL to return to OS
 *
 *  RETURN VALUE:
 *      No value
 *
 */

int main( int argc, char *argv[] ) {

#ifdef __SWITCH__
//    consoleInit(NULL);
    romfsInit();
    chdir("romfs:/");
#endif

#ifdef __SWITCH__
    argc = 2;
    argv[0] = "bgdi";
    argv[1] = "game.dcb";
#endif

    char * filename = NULL, dcbname[ __MAX_PATH ], *ptr, *arg0 = NULL;
    int i, j, ret = -1;
    file * fp = NULL;
    dcb_signature dcb_signature = { 0 };

    /* disable stdout buffering */
    setvbuf( stdout, NULL, _IONBF, BUFSIZ );

#ifdef _WIN32
    if ( strlen( argv[0] ) < 4 || strncmpi( &argv[0][strlen( argv[0] ) - 4], ".exe", 4 ) ) {
        arg0 = malloc( strlen( argv[0] ) + 5 );
        sprintf( arg0, "%s.exe", argv[0] );
    } else {
#endif
        arg0 = strdup( argv[0] );
#ifdef _WIN32
    }
#endif

    ptr = arg0 + strlen( arg0 );
    while ( ptr > arg0 && ptr[-1] != '\\' && ptr[-1] != '/' ) ptr-- ;

    appexename = strdup( ptr );

    /* get executable full pathname  */
    fp = NULL;
#ifdef __SWITCH__
    appexefullpath = strdup( argv[0] );
#else
    appexefullpath = getfullpath( arg0 );
#endif

    if ( ( !strchr( arg0, '\\' ) && !strchr( arg0, '/' ) ) ) {
        struct stat st;
        if ( stat( appexefullpath, &st ) || !S_ISREG( st.st_mode ) ) {
            char *p = whereis( appexename );
            if ( p ) {
                char * tmp = calloc( 1, strlen( p ) + strlen( appexename ) + 2 );
                free( appexefullpath );
                sprintf( tmp, "%s/%s", p, appexename );
                free( p );
                appexefullpath = getfullpath( tmp );
                free( tmp );
            }
        }
    }

    /* get pathname of executable */
    ptr = strstr( appexefullpath, appexename );
    appexepath = calloc( 1, ptr - appexefullpath + 1 );
    strncpy( appexepath, appexefullpath, ptr - appexefullpath );

    standalone = ( strncmpi( appexename, "bgdi", 4 ) == 0 ) ;

    /* add binary path */
    file_addp( appexepath );

    if ( !standalone ) {
        /* Hand-made interpreter: search for DCB at EOF */
        fp = file_open( appexefullpath, "rb0" );
        if ( fp ) {
            file_seek( fp, -( int )sizeof( dcb_signature ), SEEK_END );
            file_read( fp, &dcb_signature, sizeof( dcb_signature ) );

        }

        filename = appexefullpath;
    }

    if ( standalone ) {
        /* Calling BGDI.EXE so we must get all command line params */

        for ( i = 1 ; i < argc ; i++ ) {
            if ( argv[i][0] == '-' ) {
                j = 1 ;
                while ( argv[i][j] ) {
                    if ( argv[i][j] == 'd' ) debug++;
                    if ( argv[i][j] == 'i' ) {
                        if ( argv[i][j+1] == 0 ) {
                            if ( i == argc - 1 ) {
                                fprintf( stderr, "Directory missing" "\n" ) ;
                                exit( 0 );
                            }
                            file_addp( argv[i+1] );
                            i++ ;
                            break ;
                        }
                        file_addp( &argv[i][j + 1] ) ;
                        break ;
                    }
                    j++ ;
                }
            } else {
                if ( !filename ) {
                    filename = argv[i] ;
                    if ( i < argc - 1 ) memmove( &argv[i], &argv[i+1], sizeof( char * ) * ( argc - i - 1 ) ) ;
                    argc-- ;
                    i-- ;
                }
            }
        }

        if ( !filename ) {
            printf( BGDI_VERSION "\n"
                    "Bennu Game Development Interpreter\n\n"
                     "\n" );

       
            return -1 ;
        }
    }

    /* Initialization (modules needed before dcb_load) */

    string_init() ;
    init_c_type() ;

    /* Init application title for windowed modes */

    strcpy( dcbname, filename ) ;

    ptr = filename + strlen( filename );
    while ( ptr > filename && ptr[-1] != '\\' && ptr[-1] != '/' ) ptr-- ;

    appname = strdup( ptr ) ;
    if ( strlen( appname ) > 3 ) {
        char ** dcbext = dcb_exts, *ext = &appname[ strlen( appname ) - 4 ];
#ifdef _WIN32
        if ( !strncmpi( ext, ".exe", 4 ) ) {
            *ext = '\0';
        }
        else
#endif
        while ( dcbext && *dcbext ) {
            if ( !strncmpi( ext, *dcbext, 4 ) ) {
                *ext = '\0';
                break;
            }
            dcbext++;
        }
    }

    if ( !embedded ) {
        /* First try to load directly (we expect myfile.dcb) */
        if ( !dcb_load( dcbname ) ) {
            char ** dcbext = dcb_exts;
            int dcbloaded = 0;

            while ( dcbext && *dcbext ) {
                strcpy( dcbname, appname ) ;
                strcat( dcbname, *dcbext ) ;
                if (( dcbloaded = dcb_load( dcbname ) ) ) break;
                dcbext++;
            }

            if ( !dcbloaded ) {
                fprintf( stderr, "Error in dcb" "\n", filename, DCB_VERSION >> 8 ) ;
                return -1 ;
            }
        }
    } else {
        dcb_load_from( fp, ( const char * ) dcbname, dcb_signature.dcb_offset );
    }

    /* If the dcb is not in debug mode */

    if ( dcb.data.NSourceFiles == 0 ) debug = 0;

    /* Initialization (modules needed after dcb_load) */

    sysproc_init() ;

#ifdef _WIN32
    HWND hWnd = GetConsoleWindow();
    DWORD dwProcessId;
    GetWindowThreadProcessId( hWnd, &dwProcessId );
    if ( dwProcessId == GetCurrentProcessId() ) ShowWindow( hWnd, SW_HIDE );
#endif

//#ifdef __SWITCH__
//    consoleExit(NULL);
//#endif

    argv[0] = filename;
    bgdrtm_entry( argc, argv );

    if ( mainproc ) {
        ( void ) instance_new( mainproc, NULL ) ;
        ret = instance_go_all() ;
    }

    bgdrtm_exit(ret);

    free( arg0              );

    free( appexename        );
    free( appexepath        );
    free( appexefullpath    );
    free( appname           );

#ifdef __SWITCH__
    romfsExit();
#endif

    return ret;
}

