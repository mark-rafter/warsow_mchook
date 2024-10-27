/****************************************/
/*	Warsow mcHook (for version 0.42)	*/
/*										*/
/*	Please read the readme.txt!!!		*/
/****************************************/

/*
If you want to use this code/find it helpful, please give me credit in your readme/whatever. 

If you have trouble using any of this code, send me a PM on http://forum.gamedeception.net 
( username: BiGmAc ) or catch me on IRC ( username: bigmac ):

server: irc.rizon.net
channel: #game-deception.
*/

#include "Common.h"

HWND ( __stdcall *pCreateWindowExA )( DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID );
FARPROC ( __stdcall *pGetProcAddress )( HMODULE, LPCSTR );

void *InterceptDllCall( char *szDllName, char *szFunctionName, DWORD pNewFunction );

FARPROC __stdcall GetProcAddress_Hook( HMODULE hModule, LPCSTR lpProcName )
{
	if( HIWORD( lpProcName ) )
	{
		if( !strcmp( lpProcName, "glBegin" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglBegin], eax;

			return( ( FARPROC )pglBegin );
		}
		else if( !strcmp( lpProcName, "glEnable" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglEnable], eax;

			return( ( FARPROC )pglEnable );
		}
		else if( !strcmp( lpProcName, "glEnd" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglEnd], eax;

			return( ( FARPROC )pglEnd );
		}
		else if( !strcmp( lpProcName, "glColor4f" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglColor4f], eax;

			return( ( FARPROC )pglColor4f );
		}
		else if( !strcmp( lpProcName, "glColor4fv" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglColor4fv], eax;

			return( ( FARPROC )pglColor4fv );
		}
		else if( !strcmp( lpProcName, "glDisable" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglDisable], eax;

			return( ( FARPROC )pglDisable );
		}
		else if( !strcmp( lpProcName, "glVertex2f" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglVertex2f], eax;

			return( ( FARPROC )pglVertex2f );
		}
		else if( !strcmp( lpProcName, "glGetIntegerv" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglGetIntegerv], eax;

			return( ( FARPROC )pglGetIntegerv );
		}
		else if( !strcmp( lpProcName, "glBlendFunc" ) )
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglBlendFunc], eax;

			return( ( FARPROC )pglBlendFunc );
		}
		else if( !strcmp( lpProcName, "glColor4ubv" ) ) // Hooked
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglColor4ubv], eax;

			return( ( FARPROC )&glColor4ubv_Hook );
		}
		else if( !strcmp( lpProcName, "glDrawElements" ) ) // Hooked
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pglDrawElements], eax;

			return( ( FARPROC )&glDrawElements_Hook );
		}
		else if( !strcmp( lpProcName, "wglGetProcAddress" ) ) // Hooked
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pwglGetProcAddress], eax;

			return( ( FARPROC )&wglGetProcAddress_Hook );
		}
		else if( !strcmp( lpProcName, "GetCGameAPI" ) ) // Hooked
		{
			pGetProcAddress( hModule, lpProcName );
			_asm mov [pGetCGameAPI], eax;

			return( ( FARPROC )&GetCGameAPI_Hook );
		}
		else if( !strcmp( lpProcName, "GetProcAddress" ) ) // Hooked
		{
			return( ( FARPROC )&GetProcAddress_Hook );
		}
	}

	return( pGetProcAddress( hModule, lpProcName ) );
}

HWND __stdcall CreateWindowExA_Hook( DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam )
{
	HWND hRetval = pCreateWindowExA( dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );

	if( lpWindowName && strstr( lpWindowName, "Warsow" ) )
	{ // Stuff that needs to be Inited only once, in here.
		Init_OpenGL( );
		Detour_Warsow( );
	}

	return( hRetval );
}

bool __stdcall DllMain( HMODULE hModule, DWORD dwReason, LPVOID lpReserved )
{
	if( dwReason == DLL_PROCESS_ATTACH )
	{
		InterceptDllCall( "kernel32.dll", "GetProcAddress", ( DWORD )&GetProcAddress_Hook );
		_asm mov [pGetProcAddress], eax;

		InterceptDllCall( "user32.dll", "CreateWindowExA", ( DWORD )&CreateWindowExA_Hook );
		_asm mov [pCreateWindowExA], eax;
	}
	else if ( dwReason == DLL_PROCESS_DETACH )
	{
		FreeLibrary( hOpengl32_Dll );
	}

	return( true );
}

#define MakePtr( type, ptr, addValue )( type )( ( DWORD )( ptr ) + ( DWORD )( addValue ) )

void *InterceptDllCall( char *szDllName, char *szFunctionName, DWORD pNewFunction )
{
	HMODULE hModule = GetModuleHandle( 0 );

	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk;

	DWORD dwOldProtect, dwOldProtect2;

	void *pOldFunction;

	if( !( pOldFunction = GetProcAddress( GetModuleHandle( szDllName ), szFunctionName ) ) )
		return( NULL );

	pDosHeader = ( PIMAGE_DOS_HEADER )hModule;

	if( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
		return( NULL );

	pNTHeader = MakePtr( PIMAGE_NT_HEADERS, pDosHeader,pDosHeader->e_lfanew );

	if( pNTHeader->Signature != IMAGE_NT_SIGNATURE || ( pImportDesc = MakePtr( PIMAGE_IMPORT_DESCRIPTOR, pDosHeader, pNTHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress ) ) == ( PIMAGE_IMPORT_DESCRIPTOR )pNTHeader )
		return( NULL );

	while( pImportDesc->Name )
	{
		char *szModuleName = MakePtr( char *, pDosHeader, pImportDesc->Name );
		if( !stricmp( szModuleName, szDllName ) )
			break;
		pImportDesc++;
	}
	if( pImportDesc->Name == NULL )
		return( NULL );

	pThunk = MakePtr( PIMAGE_THUNK_DATA, pDosHeader, pImportDesc->FirstThunk );
	while( pThunk->u1.Function )
	{
		if( pThunk->u1.Function == ( DWORD )pOldFunction )
		{
			VirtualProtect( ( void * )&pThunk->u1.Function, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &dwOldProtect );
			pThunk->u1.Function = ( DWORD )pNewFunction;
			VirtualProtect( ( void * )&pThunk->u1.Function, sizeof( DWORD ), dwOldProtect, &dwOldProtect2 );
			return( pOldFunction );
		}
		pThunk++;
	}
	return( NULL ); 
}