#include "Common.h"

void DrawSpyCam( void );

bool g_bSpycamRendering = false;

refdef_t SpyCam_rd;

/*
Export Hooks
*/

void Init_Hook( unsigned int playerNum, int vidWidth, int vidHeight, qboolean demoplaying, qboolean pure, qboolean tv, unsigned int baseServerTime, unsigned int snapFrameTime, int protocol )
{
	oExports.Init( playerNum, vidWidth, vidHeight, demoplaying, pure, tv, baseServerTime, snapFrameTime, protocol );
	
	g_shaderCursor = *( oImports.R_RegisterPic( "gfx/ui/cursor" ) );
	g_shaderCrosshair = *( oImports.R_RegisterPic( "gfx/hud/crosshair3" ) );
	g_shaderArrowUp = *( oImports.R_RegisterPic( "gfx/ui/arrow_up" ) );
	g_shaderShell = *( oImports.R_RegisterPic( "powerups/warshell" ) );
	g_iSizeSmallFont = ( int )oImports.SCR_strHeight( pCgs->fontSystemSmall );

	strcpy( UI.UIVarTable[ UI_SCHEME_VAR ].szValue, Gui.GetScheme( )->GetSchemeName( ) );

	Gui.Init( );
}

void Shutdown_Hook( void )
{
	int i = 0;

	if( Aimbot.fOldSens )
		oImports.Cvar_SetValue( "sensitivity", Aimbot.fOldSens );

	Gui.SetActiveState( 0 );

	oExports.Shutdown( );

	g_bUpdatePlayers = false;

	for( i = 0; i < NUM_MAX_CLIENTS; i++ )
		vPlayers[ i ].~CPlayer( );

	pAimbotTarget = pSpycamClient = pMe = ( CPlayer * )-1;
}

void RenderView_Hook( float frameTime, float realFrameTime, int realTime, unsigned int serverTime, float stereo_separation )
{
	int i = 0;

	if( !pCg->frame.valid ) // frame not valid, destroy all player data
	{
		for( i = 0; i < NUM_MAX_CLIENTS; i++ )
			vPlayers[ i ].~CPlayer( );

		pAimbotTarget = pSpycamClient = pMe = ( CPlayer * )-1;
		g_bUpdatePlayers = false;
	}
	else // frame valid, set everyone to 'not updated' at start of frame.
	{
		for( i = 0; i < NUM_MAX_CLIENTS; i++ )
			vPlayers[ i ].bUpdated = false;
	}

	oExports.RenderView( frameTime, realFrameTime, realTime, serverTime, stereo_separation );
}

/*
Import Hooks
*/

void R_AddEntityToScene_Hook( entity_t *ent )
{
	static centity_t *pCent = 0;
	centity_t *pTmp = 0;
	int i = 0;

	pCent = ( centity_t * )( ( int )ent - ( int )&pTmp->ent );

	if( pCent < pCg_Entities || pCent->current.number >= NUM_MAX_CLIENTS )
		goto loc_End; // not a player ent, skip the rest.

	g_bUpdatePlayers = true; // true until proven false.
	
	if( pMe == ( CPlayer * )-1 || !pMe->bValid ) // not filled the local player, do this first.
	{
		i = pCg_Entities[ pCgs->playerNum + 1 ].current.number;

		vPlayers[ i ].bDraw2D = false;

		if( pCg_Entities[ i ].serverFrame != pCg->frame.serverFrame || pCgs->clientInfo[ i - 1 ].name[0] < 1 )
		{
			vPlayers[ i ].~CPlayer( );
			g_bUpdatePlayers = false;
			goto loc_End; // we're not valid, skip rest of func.
		}
		else
		{
			vPlayers[ i ] = CPlayer( &pCg_Entities[ i ] );

			vPlayers[ i ].UpdatePlayerState( );
			vPlayers[ i ].UpdateTeam( );
			vPlayers[ i ].UpdateVectors( );
			vPlayers[ i ].UpdateDistance( );
			vPlayers[ i ].bUpdated = true;

			pMe = &vPlayers[ i ]; // local player is filled, okay to fill the rest now.
		}
	}

	if( !g_bUpdatePlayers )
		goto loc_End; // non-valid frame, skippidy.

	UpdatePlayers( pCent );

	i = pCent->current.number;

	if( vPlayers[ i ].bUpdated && vPlayers[ i ].IsEnemy( ) )
	{
#ifdef _DEBUG_DRAWBONES_
		pCent->ent.customSkin = NULL; // so the bone 'dots' are more visible
#endif // _DEBUG_DRAWBONES_

		if( UI.UIVarTable[ UI_QUADCHAMS_VAR ].iValue >= 1 )
		{
			ent->outlineHeight = 0;
			pCent->effects |= EF_SHELL;
		}

		if( UI.UIVarTable[ UI_LIGHT_VAR ].iValue >= 1 && vPlayers[ i ].IsVisible( ) )
			vPlayers[ i ].DrawLight( );
	}

loc_End:
	oImports.R_AddEntityToScene( ent );
}

void R_RenderScene_Hook( refdef_t *fd )
{
	static bool bGuiInited = false;

	SpyCam_rd = *fd;

	if( !g_bUpdatePlayers )
	{
		oImports.R_RenderScene( fd );
		return;
	}

	// Aimbot stuff

	if( pAimbotTarget != ( CPlayer * )-1 && pAimbotTarget->bUpdated )
	{
		// would rather do these checks here instead of calling Aimbot.Aim every render when it's just returning.
		if( UI.UIVarTable[ UI_AIMBOT_VAR ].iValue == 1 ) // hitbox
			Aimbot.Aim( true );
		else if( UI.UIVarTable[ UI_AIMBOT_VAR ].iValue == 2 ) // bone
			Aimbot.Aim( false );
		else
			Aimbot.UnAim( );
	}
	else
		Aimbot.UnAim( );
	
	if( Aimbot.IsAiming( ) )
	{
		if( Aimbot.fOldSens )
			oImports.Cvar_SetValue( "sensitivity", 0.0f );
		else
		{
			Aimbot.fOldSens = oImports.Cvar_Value( "sensitivity" );
			oImports.Cvar_SetValue( "sensitivity", 0.0f );
		}
	}
	else if( Aimbot.fOldSens )
	{
		oImports.Cvar_SetValue( "sensitivity", Aimbot.fOldSens );
		Aimbot.fOldSens = 0.0f;
	}

	if( UI.UIVarTable[ UI_AUTOSHOOT_VAR ].iValue >= 1 )
		Aimbot.AutoShoot( &pCL_Cmds[ *pCLS_ucmdHead & CMD_MASK ] );

	// Spycam stuff

    if( UI.UIVarTable[ UI_SPYPLAYER_VAR ].iValue >= 1 && vPlayers[ UI.UIVarTable[ UI_SPYPLAYER_VAR ].iValue ].bUpdated )
		pSpycamClient = &vPlayers[ UI.UIVarTable[ UI_SPYPLAYER_VAR ].iValue ];

	if( Gui.GetActiveState( ) )
	{
		SpyCam_rd.width = ( fd->width / 3 ) * UI.UIVarTable[ UI_SPYCAM_VAR ].iValue;
		SpyCam_rd.height = ( fd->height / 3 ) * UI.UIVarTable[ UI_SPYCAM_VAR ].iValue;

		if( !bGuiInited )
		{
			bGuiInited = true;
			Gui.PushWindow( &Gui.RadarWnd, fd->width - 250, 0, 250, 250, "Radar", true );
			Gui.PushWindow( &Gui.SpycamWnd, fd->width - ( fd->width / 3 ) - 5, 260, ( fd->width / 3 ), ( fd->height / 3 ) + ( FONT_SIZE - 1 ), "Spycam", true, false );
		}	

		SpyCam_rd.x = Gui.SpycamWnd.GetDimension( 0 );
		SpyCam_rd.y = Gui.SpycamWnd.GetDimension( 1 ) + ( FONT_SIZE - 1 );
	}

	oImports.R_RenderScene( fd );

	if( pSpycamClient != ( CPlayer * )-1 && pSpycamClient->bUpdated && Gui.GetActiveState( ) 
	&& Gui.SpycamWnd.GetMaximized( ) && UI.UIVarTable[ UI_SPYCAM_VAR ].iValue >= 1 )
		DrawSpyCam( );
}

void SCR_DrawString_Hook( int x, int y, int align, const char *str, struct mufont_s *font, vec4_t color )
{
	if( strstr( str, "Load" ) && str[0] == 'L' )
	{
		oImports.SCR_DrawString( x, y, align, S_COLOR_YELLOW "Warsow mcHook (for " S_COLOR_RED "0.42" S_COLOR_YELLOW ") by " S_COLOR_ORANGE "bigmac", font, color );
		oImports.SCR_DrawString( x, y + ( int )oImports.SCR_strHeight( font ) + 1, align, str, font, color );
		return;
	}

	oImports.SCR_DrawString( x, y, align, str, font, color );
}

/*
End of Imports
*/

void DrawSpyCam( void )
{
	vec3_t fAdditional = { 0.0f, 0.0f, 25.0f };

	VectorCopy( pSpycamClient->GetLerpAngles( ), SpyCam_rd.viewangles );
	VectorCopy( pSpycamClient->GetLerpOrigin( ), SpyCam_rd.vieworg );

	SpyCam_rd.vieworg[2] += 32.0f; // TODO: if crouching, += 12.0f

	oImports.R_ClearScene( );

	pMe->GetClientEnt( )->ent.renderfx &= ~RF_VIEWERMODEL;
	pMe->GetClientEnt( )->ent.customShader = &g_shaderShell;
	pMe->GetClientEnt( )->ent.renderfx |= ( RF_FULLBRIGHT | RF_NOSHADOW );

	// TODO: just draw the local player's hitbox instead of his player model.
	oImports.R_AddEntityToScene( &pMe->GetClientEnt( )->ent );

	g_bSpycamRendering = true;
	oImports.R_RenderScene( &SpyCam_rd );
	g_bSpycamRendering = false;
}

bool WorldToScreen( vec3_t fWorldCoord, vec2_t fScreenCoord )
{
	vec3_t fLocalOrigin = { 0.0f, 0.0f, 0.0f }; 

	VectorSubtract( fWorldCoord, pCg->view.refdef.vieworg, fLocalOrigin ); 

	if( DotProduct( fLocalOrigin, pCg->view.axis[FORWARD] ) < 0 ) 
		return( false ); 

	oImports.R_TransformVectorToScreen( &pCg->view.refdef, fWorldCoord, fScreenCoord );

	if( ( fScreenCoord[0] < 0 || fScreenCoord[0] > pCgs->vidWidth ) 
	||  ( fScreenCoord[1] < 0 || fScreenCoord[1] > pCgs->vidHeight ) )
		return( false );

	return( true );
}