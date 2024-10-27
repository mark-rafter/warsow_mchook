#ifndef _GL_HEADER
#define _GL_HEADER

extern void Init_OpenGL( void );
extern HMODULE hOpengl32_Dll;

extern FARPROC ( __stdcall *pGetProcAddress )( HMODULE, LPCSTR );

extern void __stdcall glColor4ubv_Hook( const unsigned char *v ); 
extern void __stdcall glDrawElements_Hook( unsigned int mode, int count, unsigned int type, const void *indices );
extern void __stdcall glDrawRangeElementsEXT_Hook( unsigned int mode, int count, unsigned int start, unsigned int end, unsigned int type, const void *indices );
extern FARPROC __stdcall wglGetProcAddress_Hook( LPCSTR lpszProc );

extern void ( __stdcall *pglEnable )( unsigned int );
extern void ( __stdcall *pglBegin )( unsigned int );
extern void ( __stdcall *pglEnd )( void );
extern void ( __stdcall *pglColor4f )( float, float, float, float );
extern void ( __stdcall *pglColor4fv )( const float * );
extern void ( __stdcall *pglDisable )( unsigned int );
extern void ( __stdcall *pglVertex2f )( float, float );
extern void ( __stdcall *pglGetIntegerv )( unsigned int, int * );
extern void ( __stdcall *pglBlendFunc )( unsigned int, unsigned int );

extern unsigned int ( __stdcall *pglGenLists )( int );
extern void ( __stdcall *pglLineWidth )( float );
extern void ( __stdcall *pglGetBooleanv )( unsigned int, unsigned char * );
extern void ( __stdcall *pglPopAttrib )( void );
extern void ( __stdcall *pglCallLists )( int, unsigned int, const void * );
extern void ( __stdcall *pglListBase )( unsigned int );
extern void ( __stdcall *pglPushAttrib )( unsigned int );
extern void ( __stdcall *pglHint )( unsigned int, unsigned int );
extern void ( __stdcall *pglRasterPos2i )( int, int );
extern int ( __stdcall *pwglUseFontBitmapsA )( HDC, DWORD, DWORD, DWORD );
extern HDC ( __stdcall *pwglGetCurrentDC )( void );

//
extern void ( __stdcall *pglColor4ubv )( const unsigned char * );
extern void ( __stdcall *pglDrawElements )( unsigned int, int, unsigned int, const void * );
extern void ( __stdcall *pglDrawRangeElementsEXT )( unsigned int, int, unsigned int, unsigned int, unsigned int, const void * );
extern FARPROC ( __stdcall *pwglGetProcAddress )( LPCSTR );

#endif // _GL_HEADER