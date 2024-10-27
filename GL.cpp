#include "Common.h"

void ( __stdcall *pglEnable )( unsigned int );
void ( __stdcall *pglBegin )( unsigned int );
void ( __stdcall *pglEnd )( void );
void ( __stdcall *pglColor4f )( float, float, float, float );
void ( __stdcall *pglColor4fv )( const float * );
void ( __stdcall *pglDisable )( unsigned int );
void ( __stdcall *pglVertex2f )( float, float );
void ( __stdcall *pglGetIntegerv )( unsigned int, int * );
void ( __stdcall *pglBlendFunc )( unsigned int, unsigned int );

unsigned int ( __stdcall *pglGenLists )( int );
void ( __stdcall *pglLineWidth )( float );
void ( __stdcall *pglGetBooleanv )( unsigned int, unsigned char * );
void ( __stdcall *pglPopAttrib )( void );
void ( __stdcall *pglCallLists )( int, unsigned int, const void * );
void ( __stdcall *pglListBase )( unsigned int );
void ( __stdcall *pglPushAttrib )( unsigned int );
void ( __stdcall *pglHint )( unsigned int, unsigned int );
void ( __stdcall *pglRasterPos2i )( int, int );
int ( __stdcall *pwglUseFontBitmapsA )( HDC, DWORD, DWORD, DWORD );
HDC ( __stdcall *pwglGetCurrentDC )( void );

//
void ( __stdcall *pglColor4ubv )( const unsigned char * );
void ( __stdcall *pglDrawElements )( unsigned int, int, unsigned int, const void * );
void ( __stdcall *pglDrawRangeElementsEXT )( unsigned int, int, unsigned int, unsigned int, unsigned int, const void * );
FARPROC ( __stdcall *pwglGetProcAddress )( LPCSTR );

unsigned char fOrigGlowColor[4];
HMODULE hOpengl32_Dll = 0;

void __stdcall glColor4ubv_Hook( const unsigned char *v )
{
	if( g_bShellEffect && !g_bSpycamRendering )
	{
		fOrigGlowColor[0] = v[0];
		fOrigGlowColor[1] = v[1];
		fOrigGlowColor[2] = 0;
		fOrigGlowColor[3] = v[3] / 2;
		return;
	}
	else if( g_bShellEffect && g_bSpycamRendering )
	{
		fOrigGlowColor[0] = 255;
		fOrigGlowColor[1] = 255;
		fOrigGlowColor[2] = 255;
		fOrigGlowColor[3] = 255;
		pglColor4ubv( fOrigGlowColor );
		return;
	}
	else if( !g_bShellEffect && g_bSpycamRendering )
	{
		fOrigGlowColor[0] = v[0] - ( v[0] / 3 );
		fOrigGlowColor[1] = v[1];
		fOrigGlowColor[2] = v[2];
		fOrigGlowColor[3] = v[3];
		pglColor4ubv( fOrigGlowColor );
		return;
	}

	pglColor4ubv( v );
}

unsigned char fNewGlowColor[4];

void __stdcall glDrawRangeElementsEXT_Hook( unsigned int mode, int count, unsigned int start, unsigned int end, unsigned int type, const void *indices )
{
	if( g_bShellEffect && UI.UIVarTable[ UI_QUADCHAMS_VAR ].iValue >= 1 && !g_bSpycamRendering )
	{
		fNewGlowColor[0] = ( unsigned char )128;
		fNewGlowColor[1] = ( unsigned char )128;
		fNewGlowColor[2] = ( unsigned char )255;
		fNewGlowColor[3] = ( unsigned char )200;

		pglDisable( GL_DEPTH_TEST );
		pglColor4ubv( fOrigGlowColor );
		pglDrawRangeElementsEXT( mode, count, start, end, type, indices );
		pglEnable( GL_DEPTH_TEST );
		pglColor4ubv( fNewGlowColor );
	}
	else if( g_bDrawingGLPlayer && UI.UIVarTable[ UI_WALLHACK_VAR ].iValue >= 1 && !g_bSpycamRendering )
	{
		if( g_iCounter < 5 )
		{
			pglDisable( GL_DEPTH_TEST );
			pglDrawRangeElementsEXT( mode, count, start, end, type, indices );
			pglEnable( GL_DEPTH_TEST );
		}
		g_iCounter++;
	}

	pglDrawRangeElementsEXT( mode, count, start, end, type, indices );
}

void __stdcall glDrawElements_Hook( unsigned int mode, int count, unsigned int type, const void *indices )
{
	if( g_bShellEffect && UI.UIVarTable[ UI_QUADCHAMS_VAR ].iValue >= 1 && !g_bSpycamRendering )
	{
		fNewGlowColor[0] = ( unsigned char )128;
		fNewGlowColor[1] = ( unsigned char )128;
		fNewGlowColor[2] = ( unsigned char )255;
		fNewGlowColor[3] = ( unsigned char )200;

		pglDisable( GL_DEPTH_TEST );
		pglColor4ubv( fOrigGlowColor );
		pglDrawElements( mode, count, type, indices );
		pglEnable( GL_DEPTH_TEST );
		pglColor4ubv( fNewGlowColor );
	}
	else if( g_bDrawingGLPlayer && UI.UIVarTable[ UI_WALLHACK_VAR ].iValue >= 1 && !g_bSpycamRendering )
	{
		if( g_iCounter < 5 )
		{
			pglDisable( GL_DEPTH_TEST );
			pglDrawElements( mode, count, type, indices );
			pglEnable( GL_DEPTH_TEST );
		}
		g_iCounter++;
	}

	pglDrawElements( mode, count, type, indices );
}

FARPROC __stdcall wglGetProcAddress_Hook( LPCSTR lpProcName )
{
	if( !lstrcmp( lpProcName, "wglGetProcAddress" ) )
	{
		return( ( FARPROC )&wglGetProcAddress_Hook );
	}
	else if( !strcmp( lpProcName, "glDrawRangeElements" ) )
	{
		pwglGetProcAddress( lpProcName );
		_asm mov [pglDrawRangeElementsEXT], eax;

		return( ( FARPROC )&glDrawRangeElementsEXT_Hook );
	}

	return( pwglGetProcAddress( lpProcName ) );
}

void Init_OpenGL( void )
{
	hOpengl32_Dll = LoadLibrary( "opengl32.dll" );

	// Game doesn't use these but we do, so we have to call GPA ourselves.
	pGetProcAddress( hOpengl32_Dll, "glGenLists" );		_asm mov [pglGenLists], eax;
	pGetProcAddress( hOpengl32_Dll, "glLineWidth" );	_asm mov [pglLineWidth], eax;
	pGetProcAddress( hOpengl32_Dll, "glGetBooleanv" );	_asm mov [pglGetBooleanv], eax;
	pGetProcAddress( hOpengl32_Dll, "glPopAttrib" );	_asm mov [pglPopAttrib], eax;
	pGetProcAddress( hOpengl32_Dll, "glCallLists" );	_asm mov [pglCallLists], eax;
	pGetProcAddress( hOpengl32_Dll, "glListBase" );		_asm mov [pglListBase], eax;
	pGetProcAddress( hOpengl32_Dll, "glPushAttrib" );	_asm mov [pglPushAttrib], eax;
	pGetProcAddress( hOpengl32_Dll, "glHint" );			_asm mov [pglHint], eax;
	pGetProcAddress( hOpengl32_Dll, "glRasterPos2i" );	_asm mov [pglRasterPos2i], eax;
	pGetProcAddress( hOpengl32_Dll, "wglUseFontBitmapsA" );	_asm mov [pwglUseFontBitmapsA], eax;
	pGetProcAddress( hOpengl32_Dll, "wglGetCurrentDC" );	_asm mov [pwglGetCurrentDC], eax;
}