#include "Common.h"

#define GET_PAGE( x ) ( ( void * )( ( ( unsigned long )x ) & 0xFFFFF000 ) )

// Creds noskill.

void *DetourFunc( void *pOrig, void *pDest, int iLen )
{
	unsigned char *cTramp;
	DWORD dwTmp = 0;

	if( iLen < 5 )
		iLen = 6; // default

	cTramp = ( unsigned char * )malloc( iLen + 5 );

	memcpy( cTramp, pOrig, iLen );

	cTramp[ iLen ] = 0xE9;

	__asm
	{
		mov eax, dword ptr [pOrig] 
		add eax, dword ptr [iLen] 
		mov ecx, dword ptr [iLen] 
		mov edx, dword ptr [cTramp] 
		lea ecx, [edx+ecx+5] 
		sub eax, ecx 
		add edx, dword ptr [iLen] 
		mov dword ptr [edx+1], eax 
	}

	VirtualProtect( GET_PAGE( pOrig ), iLen, PAGE_EXECUTE_READWRITE, &dwTmp );

	*( ( unsigned char * ) pOrig ) = 0xE9;

	__asm
	{
		mov eax, dword ptr [pOrig] 
		add eax, 5 
		mov ecx, dword ptr [pDest] 
		sub ecx, eax 
		mov edx, dword ptr [pOrig] 
		mov dword ptr [edx+1], ecx 
	}

	return( cTramp );
}

void Detour_CGame( void )
{
	DetourFunc( CG_DRAWPLAYERNAMES_ADDR, ( PBYTE )&CG_DrawPlayerNames_Hook, CG_DRAWPLAYERNAMES_LEN );
	_asm mov [pCG_DrawPlayerNames], eax;

	DetourFunc( CG_DRAW2D_ADDR, ( PBYTE )&CG_Draw2D_Hook, CG_DRAW2D_LEN );
	_asm mov [pCG_Draw2D], eax;

	DetourFunc( CG_ENTITYEVENT_ADDR, ( PBYTE )&CG_EntityEvent_Hook, CG_ENTITYEVENT_LEN );
	_asm mov [pCG_EntityEvent], eax;
}

void Detour_Warsow( void )
{
	DetourFunc( R_RENDERMESHBUFFER_ADDR, ( PBYTE )&R_RenderMeshBuffer_Hook, R_RENDERMESHBUFFER_LEN );
	_asm mov [pR_RenderMeshBuffer], eax;

	DetourFunc( CL_MOUSEMOVE_ADDR, ( PBYTE )&CL_MouseMove_Hook, CL_MOUSEMOVE_LEN );
	_asm mov [pCL_MouseMove], eax;

	DetourFunc( KEY_EVENT_ADDR, ( PBYTE )&Key_Event_Hook, KEY_EVENT_LEN );
	_asm mov [pKey_Event], eax;

	DetourFunc( MAINWNDPROC_ADDR, ( PBYTE )&MainWndProc_Hook, MAINWNDPROC_LEN );
	_asm mov [pMainWndProc], eax;
}