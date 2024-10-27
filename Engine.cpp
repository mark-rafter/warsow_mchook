#include "Common.h"

cgame_export_t *( *pGetCGameAPI )( cgame_import_t * );

void CG_DrawPlayerNames_Hook( struct mufont_s *font );
void ( *pCG_DrawPlayerNames )( struct mufont_s * );

void CG_Draw2D_Hook( void );
void ( *pCG_Draw2D )( void );

void CG_EntityEvent_Hook( void );
void ( *pCG_EntityEvent )( void );

void R_RenderMeshBuffer_Hook( void );
void ( *pR_RenderMeshBuffer )( void );

void CL_MouseMove_Hook( usercmd_t *cmd );
void ( *pCL_MouseMove )( usercmd_t * );

void Key_Event_Hook( int key, qboolean down );
void ( *pKey_Event )( int, qboolean );

LONG __stdcall MainWndProc_Hook( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LONG ( __stdcall *pMainWndProc )( HWND, UINT, WPARAM, LPARAM );

int ( *pCG_PointContents )( vec3_t );

cgame_import_t oImports;
cgame_export_t oExports;

cgame_export_t *pCGExports = 0;

bool g_bShellEffect = false, g_bDrawingGLPlayer = false, g_bUpdatePlayers = false;
int g_iCounter = 0;

// EXE
int *pCLS_ucmdHead = CLS_UCMDHEAD_ADDR;
usercmd_t *pCL_Cmds = CL_CMDS_ADDR;
float *g_pfViewangles = CL_VIEWANGLES_ADDR;
shader_t *pR_Shaders = R_SHADERS_ADDR;

// DLL
cg_static_t	*pCgs = 0;
cg_state_t	*pCg = 0;
centity_t	*pCg_Entities = 0;
pmodel_t	*pCg_EntPModels = 0;
scb_playertab_t *pSCB_Players = 0;

centity_t	*g_pCent = 0;

shader_t g_shaderCursor, g_shaderCrosshair, g_shaderArrowUp, g_shaderShell;
int g_iSizeSmallFont = 0;

cgame_export_t *GetCGameAPI_Hook( cgame_import_t *import )
{
	Detour_CGame( );

	pCG_PointContents = ( int ( * )( vec3_t ) )CG_POINTCONTENTS_ADDR;

	memcpy( &oImports, import, sizeof( oImports ) );

	import->R_AddEntityToScene = &R_AddEntityToScene_Hook;
	import->R_RenderScene = &R_RenderScene_Hook;
	import->SCR_DrawString = &SCR_DrawString_Hook;

	pCGExports = pGetCGameAPI( import );

	memcpy( &oExports, pCGExports, sizeof( oExports ) );

	pCGExports->Init = &Init_Hook;
	pCGExports->Shutdown = &Shutdown_Hook;
	pCGExports->RenderView = &RenderView_Hook;

	pCg = ( cg_state_t * )CG_ADDR;
	pCgs = ( cg_static_t * )CGS_ADDR;
	pCg_Entities = ( centity_t * )CG_ENTITIES_ADDR;
	pSCB_Players = ( scb_playertab_t * )SCB_PLAYERS_ADDR;
	pCg_EntPModels = ( pmodel_t * )CG_ENTPMODELS_ADDR;

    return( pCGExports );
}

void CG_DrawPlayerNames_Hook( struct mufont_s *font )
{
	if( !g_bUpdatePlayers )
		goto loc_End;

	for( int i = 0; i < NUM_MAX_CLIENTS; i++ )
		vPlayers[ i ].Draw_2D( );

	if( UI.UIVarTable[ UI_ESP_VAR ].iValue >= 1 )
		return;

loc_End:
	pCG_DrawPlayerNames( font );
}

void CG_Draw2D_Hook( void )
{
	if( pMe != ( CPlayer * )-1 && pMe->bValid )
	{
		Gui.Update( );
		UI.UpdateCvars( );
	}

	pCG_Draw2D( ); 
}

entity_state_t *EntityEvent_pEnt = 0;
int EntityEvent_iEV = 0, EntityEvent_iParm = 0;

void xCG_EntityEvent( void )
{
	if( EntityEvent_iEV == EV_PAIN )
	{
		switch( EntityEvent_iParm )
		{
			case PAIN_25:
				vPlayers[ EntityEvent_pEnt->number ].iHealth = 24; // 0 - 25
				break;

			case PAIN_50:
				vPlayers[ EntityEvent_pEnt->number ].iHealth = 49; // 25 - 50
				break;

			case PAIN_75:
				vPlayers[ EntityEvent_pEnt->number ].iHealth = 74; // 50 - 75
				break;

			case PAIN_100:
				vPlayers[ EntityEvent_pEnt->number ].iHealth = 99; // 75 - 100
				break;

			case PAIN_WARSHELL:
				break;

			default:
				_asm int 3; // we should never get this.
				break;
		}
	}
	else if( EntityEvent_iEV == EV_DIE )
	{
		vPlayers[ EntityEvent_pEnt->number ].iHealth = 0;
	}
	else if( EntityEvent_iEV == EV_PLAYER_TELEPORT_IN )
	{
		// when this counts as teleporting into the game, set health to 100;
	}
	else if( EntityEvent_iEV == EV_FALL && pCgs->serverRules.falldamage )
	{
		vPlayers[ EntityEvent_pEnt->number ].iHealth -= EntityEvent_iParm;
	}
}

__declspec( naked )void CG_EntityEvent_Hook( void )
{
	__asm
	{
		mov EntityEvent_iEV, eax;
		mov EntityEvent_pEnt, edx;
		
		mov eax, [esp+4]; // use EAX as temp register.
		mov EntityEvent_iParm, eax;

		mov eax, EntityEvent_iEV; // restore EAX for orig func.
				
		push [esp+4];
		call pCG_EntityEvent;
		add esp, 4;

		call xCG_EntityEvent;
		ret;
	}
}

// ri.meshlist->meshbuffer_opaque = 0x787AB8
// ri.meshlist->meshbuffer_translucent = 0x787AC4

meshbuffer_t *g_pMeshBuff = 0;

__declspec( naked )void R_RenderMeshBuffer_Hook( void )
{
	_asm pushad;
	_asm mov g_pMeshBuff, eax;

	if( strstr( pR_Shaders[g_pMeshBuff->shaderkey & 0xFFF].name, "/player" ) )
	{
		g_iCounter = 0;
		g_bDrawingGLPlayer = true;

		_asm popad;
		_asm call pR_RenderMeshBuffer;

		g_iCounter = 0;
		g_bDrawingGLPlayer = false;
		_asm ret;
	}
	else if( strstr( pR_Shaders[g_pMeshBuff->shaderkey & 0xFFF].name, "/warsh" ) )
	{
		g_bShellEffect = true;

		_asm popad;
		_asm call pR_RenderMeshBuffer;

		g_bShellEffect = false;
		_asm ret;
	}

	__asm
	{
		popad;
		call pR_RenderMeshBuffer;
		ret;
	}
}

int *p_iMX = CL_MOUSEMOVE_ARG_X_ADDR;
int *p_iMY = CL_MOUSEMOVE_ARG_Y_ADDR;

void CL_MouseMove_Hook( usercmd_t *cmd )
{
	static int iTmpMX, iTmpMY;

	if( Gui.GetActiveState( ) == 1 )
	{
		Gui.fLastMouse[0] = Gui.fMouse[0];
		Gui.fLastMouse[1] = Gui.fMouse[1];

		Gui.fMouse[0] += *p_iMX;
		Gui.fMouse[1] += *p_iMY;
        
		if( Gui.fMouse[0] <= 0 )
			Gui.fMouse[0] = 1;
		else if( Gui.fMouse[0] > pCgs->vidWidth )
			Gui.fMouse[0] = pCgs->vidWidth;

		if( Gui.fMouse[1] <= 0 )
			Gui.fMouse[1] = 1;
		else if( Gui.fMouse[1] > pCgs->vidHeight )
			Gui.fMouse[1] = pCgs->vidHeight;

		if( Gui.fLastMouse[0] != Gui.fMouse[0] || Gui.fLastMouse[1] != Gui.fMouse[1] )
			Gui.Update( );

		iTmpMX = *p_iMX;
		iTmpMY = *p_iMY;

		*p_iMX = *p_iMY = 0;
		pCL_MouseMove( cmd );

		*p_iMX = iTmpMX; // TODO: this necessary?
		*p_iMY = iTmpMY;
	}
	else
		pCL_MouseMove( cmd );
}

// sys_msg_time = 0x61FC00

void Key_Event_Hook( int key, qboolean down )
{
	Keys[ key ].bGlobalDown = ( down == qtrue );

	if( !Gui.HandleMsg( key, ( down == qtrue ) ) )
		return;

	if( Gui.GetActiveState( ) == 1 )
	{
		if( !isprint( key ) )
			Gui.SetKey( key, ( down == qtrue ) );
		return;
	}

	pKey_Event( key, down );
}

LONG __stdcall MainWndProc_Hook( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg == WM_CHAR )
	{
		if( Gui.GetActiveState( ) == 1 && wParam >= 0 && wParam <= 255 )
		{
			Gui.SetKey( wParam, true );
			return( 0 );
		}
	}

	return( pMainWndProc( hWnd, uMsg, wParam, lParam ) );
}