#ifndef _COMMON_HEADER
#define _COMMON_HEADER

#pragma warning( disable : 4101 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4996 )
#pragma warning( disable : 4566 )

#define WIN32_LEAN_AND_MEAN

//#define _DEBUG_

#ifdef _DEBUG_
//#define _DEBUG_DRAWBONES_
//#define _DEBUG_DRAWBBOXES_
//#define _DEBUG_ADDLOG_
//static int g_bInterrupt;
//#define _INT3 if( g_bInterrupt ) _asm int 3;
#endif // _DEBUG_

#include <windows.h>
#include <vector>

#define GL_DEPTH_TEST					0x0B71
#define GL_TEXTURE_2D					0x0DE1
#define GL_PERSPECTIVE_CORRECTION_HINT	0x0C50
#define GL_NICEST						0x1102
#define GL_LIST_BIT						0x20000
#define GL_UNSIGNED_BYTE				0x1401
#define GL_LINES						0x0001
#define GL_LINE_LOOP					0x0002
#define GL_TRIANGLES					0x0004
#define GL_QUADS						0x0007
#define GL_BLEND						0x0BE2
#define GL_MODULATE						0x2100
#define GL_SRC_ALPHA					0x0302
#define GL_ONE_MINUS_SRC_ALPHA			0x0303

#define NUM_MAX_CLIENTS 32

#include "Eng/cgame/cg_local.h"
#include "Eng/ref_gl/r_local.h"
#include "Eng/client/keys.h"
#include "Engine.h"
#include "Detour.h"
#include "Syscalls.h"
#include "CPlayer.h"
#include "CAimbot.h"
#include "CStringParser.h"
#include "CFont.h"
#include "CWinamp.h"
#include "CGui.h"
#include "CUserInterface.h"
#include "GL.h"

static char *AppendStr( char *szOldText, ... ); // this gets used in CConfig.h

#include "CConfig.h"

#ifdef _DEBUG_ADDLOG_
static void AddLog ( const char *fmt, ... )
{
	va_list		va_alist;
	FILE		*fp;

	struct 	tm * curtime;
	time_t	current_time;

	char logbuf[256];

	time( &current_time );
	curtime = localtime( &current_time );

	sprintf( logbuf, "%02d:%02d:%02d ", curtime->tm_hour, curtime->tm_min, curtime->tm_sec );

	va_start( va_alist, fmt );
	_vsnprintf( logbuf + strlen( logbuf ), sizeof( logbuf ) - strlen( logbuf ), fmt, va_alist );
	va_end( va_alist );

	if ( ( fp = fopen ( "zz_mcHook_LOG.txt", "ab" ) ) != NULL )
	{
		fprintf ( fp, "%s\n", logbuf );
		fclose ( fp );
	}
}
#endif // _DEBUG_ADDLOG_

static char *AppendStr( char *szOldText, ... )
{
	va_list	pArgs;
	static int  iIndex;
	static char	szNewText[3][1024];

	( iIndex < 2 ? iIndex++ : iIndex = 0 );

	va_start( pArgs, szOldText );
	_vsnprintf( szNewText[iIndex], sizeof( szNewText[iIndex] ), szOldText, pArgs );
	va_end( pArgs );

	return( szNewText[iIndex] );
}

#endif // _COMMON_HEADER