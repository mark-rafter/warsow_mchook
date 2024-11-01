#include "Common.h"

CUserInterface UI;

CButton *pRadarVar, *pSpycamVar, *pWinampVar, *pPlaylistVar, *pEspVar, *pAimbotVar, *pWallhackVar, *pQuadchamsVar, *pLightVar, *pAutoshootVar, *pSpyplayerVar, *pFovVar;
CButton *pBindVar, *pSchemeVar, *pHelpVar, *pQuitVar, *pSpamVar;

Key_t Keys[256];

char *szKeyList[] =
{
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	"ins", "del", "pgdn", "pgup", "home", "end", "mouse1", "mouse2", "mouse3"
};

int iKeyListSize = sizeof( szKeyList ) / sizeof( szKeyList[0] );

void fn_Bind_Toggle( CUserInterface *pThis, int iNumArgs, CParser::arg_t *szArgs )
{
	int i, j;

	if( iNumArgs <= 0 || iNumArgs >= 2 )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: /bind [key] toggle [cvar]" );
									//szArgs  [0]	 [1]   [2]
	}

	for( i = 0; i < UI_VARTABLE_SIZE; i++ )
	{
		if( !strcmp( pThis->UIVarTable[ i ].szCvar + 1, szArgs[2] ) ) // + 1 because szCvar has a '/' at the start.
		{
			for( j = K_F1; j < K_MOUSE3 + 1; j++ )
			{
				if( j == K_END + 1 )
					j = K_MOUSE1; // is there a better way of doing this? except another loop (which is worse)

				if( !strcmp( Keys[ j ].szKeyCode, szArgs[0] ) ) // this is the key
				{
					pThis->UIVarTable[ i ].pKey = &Keys[ j ];
					pThis->UIVarTable[ i ].iBindType = BINDACTION_TOGGLE;

					pThis->ProcessMessage( AppendStr( "[" S_COLOR_GREEN "toggle %s" S_COLOR_WHITE "] bound to " S_COLOR_ORANGE "%s", szArgs[2], szArgs[0] ) );
					return;
				}
			}
		}
	}
}

void fn_Bind_Set( CUserInterface *pThis, int iNumArgs, CParser::arg_t *szArgs )
{
	int i, j, k;
	char szValues[64] = { '\0' };

	if( iNumArgs <= 1 )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: /bind [key] set [cvar] [values]" );
									//szArgs  [0]  [1]   [2]    [3]
	}

	for( i = 0; i < UI_VARTABLE_SIZE; i++ )
	{
		if( !strcmp( pThis->UIVarTable[ i ].szCvar + 1, szArgs[2] ) )
		{
			for( j = K_F1; j < K_MOUSE3 + 1; j++ )
			{
				if( j == K_END + 1 )
					j = K_MOUSE1;

				if( !strcmp( Keys[ j ].szKeyCode, szArgs[0] ) )
				{
					pThis->UIVarTable[ i ].pKey = &Keys[ j ];
					pThis->UIVarTable[ i ].iBindType = BINDACTION_SET;
					
					pThis->UIVarTable[ i ].iNumBindArgs = iNumArgs - 1;

					delete [] pThis->UIVarTable[ i ].szBindArgs; // necessary? might aswell be on the safe side.

					pThis->UIVarTable[ i ].szBindArgs = new CParser::arg_t [iNumArgs - 1];

					for( k = 1; k < iNumArgs; k++ )
					{
						strcpy( pThis->UIVarTable[ i ].szBindArgs[k - 1], szArgs[k + 2] );
						strcat( szValues, AppendStr( "%s ", szArgs[k + 2] ) );
					}

					szValues[ strlen( szValues ) - 1 ] = '\0';

					pThis->ProcessMessage( AppendStr( "[" S_COLOR_GREEN "set %s %s" S_COLOR_WHITE "] bound to " S_COLOR_ORANGE "%s", szArgs[2], szValues, szArgs[0] ) );
					return;
				}
			}
		}
	}
}

void CUserInterface::fnUI_Radar( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_RADAR_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/radar [value]" );
		pThis->ProcessMessage( "Info: Opens the radar window" );
		pThis->ProcessMessage( AppendStr( "Radar Cvar Currently: %s", pThis->UIVarTable[ UI_RADAR_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/radar [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_RADAR_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_RADAR_VAR ].szValue, ( pThis->UIVarTable[ UI_RADAR_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( S_COLOR_YELLOW "Radar " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_RADAR_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Spycam( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_SPYCAM_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/spycam [value]" );
		pThis->ProcessMessage( "Info: Sets the spycam size, 1 is default." );
		pThis->ProcessMessage( AppendStr( "Spycam Cvar Currently: %s", pThis->UIVarTable[ UI_SPYCAM_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/spycam [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_SPYCAM_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_SPYCAM_VAR ].szValue, ( pThis->UIVarTable[ UI_SPYCAM_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Spycam " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_SPYCAM_VAR ].szValue ) );

	Gui.SpycamWnd.SetDimensions
	( 
		Gui.SpycamWnd.GetDimension( 0 ),
		Gui.SpycamWnd.GetDimension( 1 ),
		( pCg->view.refdef.width / 3 ) * pThis->UIVarTable[ UI_SPYCAM_VAR ].iValue,
		( ( pCg->view.refdef.height / 3 ) * pThis->UIVarTable[ UI_SPYCAM_VAR ].iValue ) + FONT_SIZE - 1
	);

	Gui.SpycamWnd.GetObjectPtr( OBJ_TITLEBAR )->SetDimensions
	( 
		Gui.SpycamWnd.GetObjectPtr( OBJ_TITLEBAR )->GetDimension( 0 ),
		Gui.SpycamWnd.GetObjectPtr( OBJ_TITLEBAR )->GetDimension( 1 ),
		( pCg->view.refdef.width / 3 ) * pThis->UIVarTable[ UI_SPYCAM_VAR ].iValue,
		Gui.SpycamWnd.GetObjectPtr( OBJ_TITLEBAR )->GetDimension( 3 )
	);

	Gui.SpycamWnd.GetObjectPtr( OBJ_MINIMIZE )->SetDimensions
	( 
		( pCg->view.refdef.width / 3 ) * pThis->UIVarTable[ UI_SPYCAM_VAR ].iValue - 12,
		Gui.SpycamWnd.GetObjectPtr( OBJ_MINIMIZE )->GetDimension( 1 ),
		Gui.SpycamWnd.GetObjectPtr( OBJ_MINIMIZE )->GetDimension( 2 ),
		Gui.SpycamWnd.GetObjectPtr( OBJ_MINIMIZE )->GetDimension( 3 )
	);

	delete [] szArgs;
}

void CUserInterface::fnUI_Winamp( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0, i = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_WINAMP_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/winamp [window name]" );
		pThis->ProcessMessage( "Info: changes the winamp window handle" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	char *szWindowName = new char [128];

	memset( szWindowName, '\0', sizeof( szWindowName ) );

	for( i = 0; i < iNumArgs; i++ )
		strcat( szWindowName, AppendStr( "%s ", szArgs[ i ] ) );

	szWindowName[ strlen( szWindowName ) - 1 ] = '\0';

	char *szInitMessage = new char [128];

	memset( szInitMessage, '\0', sizeof( szInitMessage ) );

	if( Winamp.Init( szWindowName, szInitMessage ) )
	{
		strcpy( pThis->UIVarTable[ UI_WINAMP_VAR ].szValue, szWindowName );
		pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Winamp " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_WINAMP_VAR ].szValue ) );
	}
	else
	{
		pThis->ProcessMessage( szInitMessage );
	}

	delete [] szInitMessage;
	delete [] szWindowName;
	delete [] szArgs;
}

void CUserInterface::fnUI_Playlist( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_PLAYLIST_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/playlist [value]" );
		pThis->ProcessMessage( "Info: Opens the winamp playlist window" );
		pThis->ProcessMessage( AppendStr( "Playlist Cvar Currently: %s", pThis->UIVarTable[ UI_PLAYLIST_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/playlist [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	if( !Winamp.bInited && atoi( szArgs[0] ) > 0 )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": Cannot open Playlist as Winamp is inactive" );
	}
	else
	{
		pThis->UIVarTable[ UI_PLAYLIST_VAR ].iValue = atoi( szArgs[0] );
		strcpy( pThis->UIVarTable[ UI_PLAYLIST_VAR ].szValue, ( pThis->UIVarTable[ UI_PLAYLIST_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
		pThis->ProcessMessage( AppendStr( S_COLOR_YELLOW "Playlist " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_PLAYLIST_VAR ].szValue ) );
	}

	delete [] szArgs;
}

void CUserInterface::fnUI_Esp( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_ESP_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/esp [value]" );
		pThis->ProcessMessage( "Info: Enables name ESP" );
		pThis->ProcessMessage( AppendStr( "Esp Cvar Currently: %s", pThis->UIVarTable[ UI_ESP_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/esp [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_ESP_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_ESP_VAR ].szValue, ( pThis->UIVarTable[ UI_ESP_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Esp " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_ESP_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Aimbot( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_AIMBOT_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/aimbot [value]" );
		pThis->ProcessMessage( "Info: Sets the aimbot style" );
		pThis->ProcessMessage( AppendStr( "Aimbot Cvar Currently: %s", pThis->UIVarTable[ UI_AIMBOT_VAR ].szValue ) );
		pThis->ProcessMessage(  "Available values: 0 = off, 1 = hitbox, 2 = bone" );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/aimbot [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_AIMBOT_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_AIMBOT_VAR ].szValue, ( pThis->UIVarTable[ UI_AIMBOT_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Aimbot " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_AIMBOT_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Wallhack( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_WALLHACK_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/wallhack [value]" );
		pThis->ProcessMessage( "Info: Enables the wallhack" );
		pThis->ProcessMessage( AppendStr( "Wallhack Cvar Currently: %s", pThis->UIVarTable[ UI_WALLHACK_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/wallhack [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_WALLHACK_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_WALLHACK_VAR ].szValue, ( pThis->UIVarTable[ UI_WALLHACK_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Wallhack " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_WALLHACK_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Quadchams( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_QUADCHAMS_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/quadchams [value]" );
		pThis->ProcessMessage( "Info: Enables the quadchams" );
		pThis->ProcessMessage( AppendStr( "Quadchams Cvar Currently: %s", pThis->UIVarTable[ UI_QUADCHAMS_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/quadchams [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_QUADCHAMS_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_QUADCHAMS_VAR ].szValue, ( pThis->UIVarTable[ UI_QUADCHAMS_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Quadchams " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_QUADCHAMS_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Light( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_LIGHT_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/light [value]" );
		pThis->ProcessMessage( "Info: Draws light around enemy players" );
		pThis->ProcessMessage( AppendStr( "Light Cvar Currently: %s", pThis->UIVarTable[ UI_LIGHT_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/light [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_LIGHT_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_LIGHT_VAR ].szValue, ( pThis->UIVarTable[ UI_LIGHT_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Light " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_LIGHT_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Autoshoot( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_AUTOSHOOT_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/autoshoot [value]" );
		pThis->ProcessMessage( "Info: Enables the autoshoot" );
		pThis->ProcessMessage( AppendStr( "Autoshoot Cvar Currently: %s", pThis->UIVarTable[ UI_AUTOSHOOT_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/autoshoot [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_AUTOSHOOT_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_AUTOSHOOT_VAR ].szValue, ( pThis->UIVarTable[ UI_AUTOSHOOT_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Autoshoot " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_AUTOSHOOT_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Spyplayer( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0, i = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	char *szPlayerList;
	int iPlayerListLen;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_SPYPLAYER_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/spyplayer [enemy id]" );
		pThis->ProcessMessage( "Info: Changes the player to spy on, 0 is default" );
		pThis->ProcessMessage( AppendStr( "Spyplayer Cvar Currently: %s", pThis->UIVarTable[ UI_SPYPLAYER_VAR ].szValue ) );
		pThis->ProcessMessage( "List of Enemy Ids (name / id):" );

		szPlayerList = new char [ NUM_MAX_CLIENTS * pCg->frame.numEntities ]; // even if they have a 3 digit player id, +12 is safe.

		szPlayerList[0] = '\0';

		for( ; i < NUM_MAX_CLIENTS; i++ )
		{
			if( vPlayers[ i ].bValid && vPlayers[ i ].IsEnemy( ) )
				strcat( szPlayerList, AppendStr( "[%s" S_COLOR_WHITE " / %i] ", vPlayers[ i ].GetName( ), vPlayers[ i ].GetClientNum( ) ) );
		}

		iPlayerListLen = ( int )strlen( szPlayerList );
		szPlayerList[ iPlayerListLen ] = '\0';

		pThis->ProcessMessage( ( iPlayerListLen > 1 ?  szPlayerList : "None" ) );

		delete [] szPlayerList;

		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/spyplayer [enemy id]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 

	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	if( !vPlayers[ atoi( szArgs[0] ) ].bValid )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid player id" );
		pThis->ProcessMessage( "Type /spyplayer for an enemy id list" );
		goto loc_del;
	}
	else if( !vPlayers[ atoi( szArgs[0] ) ].IsEnemy( ) )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": player id must be an enemy" );
		pThis->ProcessMessage( "Type /spyplayer for an enemy id list" );
		goto loc_del;
	}

	pThis->UIVarTable[ UI_SPYPLAYER_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_SPYPLAYER_VAR ].szValue, ( pThis->UIVarTable[ UI_SPYPLAYER_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Spyplayer " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_SPYPLAYER_VAR ].szValue ) );

loc_del:
	delete [] szArgs;
}

void CUserInterface::fnUI_Fov( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_FOV_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/fov [value]" );
		pThis->ProcessMessage( "Info: Sets the aimbot fov value" );
		pThis->ProcessMessage( AppendStr( "Fov Cvar Currently: %s", pThis->UIVarTable[ UI_FOV_VAR ].szValue ) );
		return;
	case 1:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/fov [value]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	pThis->UIVarTable[ UI_FOV_VAR ].iValue = atoi( szArgs[0] );
	strcpy( pThis->UIVarTable[ UI_FOV_VAR ].szValue, ( pThis->UIVarTable[ UI_FOV_VAR ].iValue < 0 ? "0" : szArgs[0] ) );
	pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Fov " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_FOV_VAR ].szValue ) );

	delete [] szArgs;
}

void CUserInterface::fnUI_Bind( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0, i = 0, iActionTableSize = 0;
	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_BIND_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/bind [key] [action]" );
		pThis->ProcessMessage( "Info: Binds an action to a button" );
		pThis->ProcessMessage( "Available Keys:" );
		pThis->ProcessMessage( "" S_COLOR_YELLOW "F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12" );
		pThis->ProcessMessage( "" S_COLOR_YELLOW "ins, del, pgdn, pgup, home, end, mouse1, mouse2, mouse3" ); 
		// TODO: loop through szKeyLists FFS!!! this is crude
		return;
	case 1:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
        pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/bind [key] [action]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	for( ; i < iKeyListSize; i++ )
	{
		if( !strcmp( szKeyList[ i ], szArgs[0] ) )
			break;
	}

	if( i >= iKeyListSize )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": Unrecognised key (type /bind for a key list)" );
		goto loc_del;
	}

	iActionTableSize = sizeof( pThis->BindActionTable ) / sizeof( pThis->BindActionTable[0] );

	for( i = 0; i < iActionTableSize; i++ )
	{
		if( !strcmp( pThis->BindActionTable[ i ].szAction, szArgs[1] ) )
		{
			// iNumArgs - 2 because we don't care about key toggle or key set, we already know.
			pThis->BindActionTable[ i ].fnCallBack( pThis, iNumArgs - 2, szArgs );
			goto loc_del;
		}
	}

	if( i >= iActionTableSize )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": Unrecognised action" );
		pThis->ProcessMessage( "Available Actions:" );
		pThis->ProcessMessage( "" S_COLOR_YELLOW "toggle [cvar], set [cvar] [values]" );
	}

loc_del:
	delete [] szArgs;
}

void CUserInterface::fnUI_Scheme( char *szBuf, CUserInterface *pThis )
{
	int iNumArgs = 0, i = 0;
	char szSchemeName[64] = { '\0' };

	CParser::arg_t *szArgs;
	char *szBuffer = 0;

	szBuffer = CParser::GetArgBuffer( szBuf, ( int )strlen( pThis->UIVarTable[ UI_SCHEME_VAR ].szCvar ), &iNumArgs );

	switch( iNumArgs )
	{
	case 0:
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/scheme [new scheme]" );
		pThis->ProcessMessage( "Info: changes the GUI colour scheme" );
		pThis->ProcessMessage( AppendStr( "Scheme Currently: %s", Gui.GetScheme( )->GetSchemeName( ) ) );
		pThis->ProcessMessage( "Available Schemes:" );

		for( ; i < SCHEME_LEN; i++ )
			pThis->ProcessMessage( AppendStr( S_COLOR_GREEN "%s", Gui.GetScheme( i )->GetSchemeName( ) ) );
		return;
	case 2:
		break;
	default:
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": invalid number of arguments" );
		pThis->ProcessMessage( "Usage: " S_COLOR_YELLOW "/scheme [new scheme]" );
		return;
	}

	szArgs = new CParser::arg_t [ iNumArgs ]; 
	CParser::GetArgs( szBuffer, iNumArgs, szArgs );

	for( i = 0; i < iNumArgs; i++ )
		strcat( szSchemeName, AppendStr( "%s ", szArgs[ i ] ) );

	szSchemeName[ strlen( szSchemeName ) - 1 ] = '\0';

	if( !strcmp( Gui.GetScheme( )->GetSchemeName( ), szSchemeName ) )
		goto loc_del; // Already using this scheme.

	for( i = 0; i < SCHEME_LEN; i++ )
	{
		if( !strcmp( szSchemeName, Gui.GetScheme( i )->GetSchemeName( ) ) )
		{
			Gui.SetScheme( Gui.GetScheme( i ) );
			pThis->UIVarTable[ UI_SCHEME_VAR ].iValue = i; // this necessary?
			strcpy( pThis->UIVarTable[ UI_SCHEME_VAR ].szValue, szSchemeName );
			pThis->ProcessMessage( AppendStr( "" S_COLOR_YELLOW "Scheme " S_COLOR_WHITE "Set To: %s", pThis->UIVarTable[ UI_SCHEME_VAR ].szValue ) );
			goto loc_del;
		}
	}

	if( i >= SCHEME_LEN )
		pThis->ProcessMessage( AppendStr( S_COLOR_RED "Error" S_COLOR_WHITE ": unrecognised scheme name: %s", szSchemeName ) );

loc_del:
	delete [] szArgs;
}

void CUserInterface::fnUI_Help( char *szBuf, CUserInterface *pThis )
{
	int i = 0;
	char *szCmds;
	char *szCvars;

	szCmds = new char [256];

	strcpy( szCmds, "Cmds:" );

	for( i = UI_BIND_VAR; i < UI_VARTABLE_SIZE; i++ )
		strcat( szCmds, AppendStr( S_COLOR_YELLOW " "S_COLOR_YELLOW"%s", pThis->UIVarTable[ i ].szCvar ) );

	pThis->ProcessMessage( szCmds );

	szCvars = new char [256];

	strcpy( szCvars, "Cvars:" );

	for( i = UI_RADAR_VAR; i < UI_BIND_VAR; i++ )
		strcat( szCvars, AppendStr( " "S_COLOR_YELLOW"%s", pThis->UIVarTable[ i ].szCvar ) );

	pThis->ProcessMessage( szCvars );

	delete [] szCmds;
	delete [] szCvars;
}

void CUserInterface::fnUI_Quit( char *szBuf, CUserInterface *pThis )
{
	oImports.Cmd_ExecuteText( EXEC_NOW, "quit" );
}

void CUserInterface::fnUI_Spam( char *szBuf, CUserInterface *pThis )
{
	if( !Winamp.bInited )
	{
		pThis->ProcessMessage( S_COLOR_RED "Error" S_COLOR_WHITE ": Winamp not activated" );
		return;
	}

	oImports.Cmd_ExecuteText( EXEC_NOW, "say " S_COLOR_RED "***************************************************" );
	oImports.Cmd_ExecuteText( EXEC_NOW, "say " S_COLOR_CYAN "Winamp " S_COLOR_WHITE "- " S_COLOR_ORANGE "Currently Playing" S_COLOR_WHITE ":" );
	oImports.Cmd_ExecuteText( EXEC_NOW, AppendStr( "say " S_COLOR_YELLOW "%s", Winamp.GetTrackTitle( ) ) );
	oImports.Cmd_ExecuteText( EXEC_NOW, "say " S_COLOR_RED "***************************************************" );
}

// esto es una función grande :|
CUserInterface::CUserInterface( void )
{
	// err.. loop through szKeyCodes?
	Keys[ K_F1 ].szKeyCode = "F1";
	Keys[ K_F2 ].szKeyCode = "F2";
	Keys[ K_F3 ].szKeyCode = "F3";
	Keys[ K_F4 ].szKeyCode = "F4";
	Keys[ K_F5 ].szKeyCode = "F5";
	Keys[ K_F6 ].szKeyCode = "F6";
	Keys[ K_F7 ].szKeyCode = "F7";
	Keys[ K_F8 ].szKeyCode = "F8";
	Keys[ K_F9 ].szKeyCode = "F9";
	Keys[ K_F10 ].szKeyCode = "F10";
	Keys[ K_F11 ].szKeyCode = "F11";
	Keys[ K_F12 ].szKeyCode = "F12";
	Keys[ K_INS ].szKeyCode = "ins";
	Keys[ K_DEL ].szKeyCode = "del";
	Keys[ K_PGDN ].szKeyCode = "pgdn";
	Keys[ K_PGUP ].szKeyCode = "pgup";
	Keys[ K_HOME ].szKeyCode = "home";
	Keys[ K_END ].szKeyCode = "end";
	Keys[ K_MOUSE1 ].szKeyCode = "mouse1";
	Keys[ K_MOUSE2 ].szKeyCode = "mouse2";
	Keys[ K_MOUSE3 ].szKeyCode = "mouse3";

	// Buttons are for the GUI drop down menu when typing in console.

	pEspVar = new CButton;
	pRadarVar  = new CButton;
	pAimbotVar = new CButton;
	pSpycamVar = new CButton;
	pWallhackVar = new CButton;
	pQuadchamsVar = new CButton;
	pLightVar = new CButton;
	pAutoshootVar = new CButton;
	pSpyplayerVar = new CButton;
	pWinampVar = new CButton;
	pPlaylistVar = new CButton;
	pFovVar = new CButton;

	pBindVar = new CButton; 
	pSchemeVar = new CButton; 
	pHelpVar = new CButton;
	pQuitVar = new CButton;
	pSpamVar = new CButton;

	this->UIVarTable[ UI_RADAR_VAR ].fnCallBack = &fnUI_Radar;
	this->UIVarTable[ UI_RADAR_VAR ].szCvar = "/radar";
	this->UIVarTable[ UI_RADAR_VAR ].pVarButton = pRadarVar;
	this->UIVarTable[ UI_RADAR_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_RADAR_VAR ].szValue, "0" );
	this->UIVarTable[ UI_RADAR_VAR ].pWindow = &Gui.RadarWnd;

	this->UIVarTable[ UI_SPYCAM_VAR ].fnCallBack = &fnUI_Spycam;
	this->UIVarTable[ UI_SPYCAM_VAR ].szCvar = "/spycam";
	this->UIVarTable[ UI_SPYCAM_VAR ].pVarButton = pSpycamVar;
	this->UIVarTable[ UI_SPYCAM_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_SPYCAM_VAR ].szValue, "0" );
	this->UIVarTable[ UI_SPYCAM_VAR ].pWindow = &Gui.SpycamWnd;

	this->UIVarTable[ UI_WINAMP_VAR ].fnCallBack = &fnUI_Winamp;
	this->UIVarTable[ UI_WINAMP_VAR ].szCvar = "/winamp";
	this->UIVarTable[ UI_WINAMP_VAR ].pVarButton = pWinampVar;
	this->UIVarTable[ UI_WINAMP_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_WINAMP_VAR ].szValue, "0" );
	this->UIVarTable[ UI_WINAMP_VAR ].pWindow = &Gui.WinampWnd;

	this->UIVarTable[ UI_PLAYLIST_VAR ].fnCallBack = &fnUI_Playlist;
	this->UIVarTable[ UI_PLAYLIST_VAR ].szCvar = "/playlist";
	this->UIVarTable[ UI_PLAYLIST_VAR ].pVarButton = pPlaylistVar;
	this->UIVarTable[ UI_PLAYLIST_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_PLAYLIST_VAR ].szValue, "0" );
	this->UIVarTable[ UI_PLAYLIST_VAR ].pWindow = &Gui.WinampPLWnd;

	this->UIVarTable[ UI_ESP_VAR ].fnCallBack = &fnUI_Esp;
	this->UIVarTable[ UI_ESP_VAR ].szCvar = "/esp";
	this->UIVarTable[ UI_ESP_VAR ].pVarButton = pEspVar;
	this->UIVarTable[ UI_ESP_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_ESP_VAR ].szValue, "0" );

	this->UIVarTable[ UI_AIMBOT_VAR ].fnCallBack = &fnUI_Aimbot;
	this->UIVarTable[ UI_AIMBOT_VAR ].szCvar = "/aimbot";
	this->UIVarTable[ UI_AIMBOT_VAR ].pVarButton = pAimbotVar;
	this->UIVarTable[ UI_AIMBOT_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_AIMBOT_VAR ].szValue, "0" );

	this->UIVarTable[ UI_WALLHACK_VAR ].fnCallBack = &fnUI_Wallhack;
	this->UIVarTable[ UI_WALLHACK_VAR ].szCvar = "/wallhack";
	this->UIVarTable[ UI_WALLHACK_VAR ].pVarButton = pWallhackVar;
	this->UIVarTable[ UI_WALLHACK_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_WALLHACK_VAR ].szValue, "0" );

	this->UIVarTable[ UI_QUADCHAMS_VAR ].fnCallBack = &fnUI_Quadchams;
	this->UIVarTable[ UI_QUADCHAMS_VAR ].szCvar = "/quadchams";
	this->UIVarTable[ UI_QUADCHAMS_VAR ].pVarButton = pQuadchamsVar;
	this->UIVarTable[ UI_QUADCHAMS_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_QUADCHAMS_VAR ].szValue, "0" );

	this->UIVarTable[ UI_LIGHT_VAR ].fnCallBack = &fnUI_Light;
	this->UIVarTable[ UI_LIGHT_VAR ].szCvar = "/light";
	this->UIVarTable[ UI_LIGHT_VAR ].pVarButton = pLightVar;
	this->UIVarTable[ UI_LIGHT_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_LIGHT_VAR ].szValue, "0" );

	this->UIVarTable[ UI_AUTOSHOOT_VAR ].fnCallBack = &fnUI_Autoshoot;
	this->UIVarTable[ UI_AUTOSHOOT_VAR ].szCvar = "/autoshoot";
	this->UIVarTable[ UI_AUTOSHOOT_VAR ].pVarButton = pAutoshootVar;
	this->UIVarTable[ UI_AUTOSHOOT_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_AUTOSHOOT_VAR ].szValue, "0" );

	this->UIVarTable[ UI_SPYPLAYER_VAR ].fnCallBack = &fnUI_Spyplayer;
	this->UIVarTable[ UI_SPYPLAYER_VAR ].szCvar = "/spyplayer";
	this->UIVarTable[ UI_SPYPLAYER_VAR ].pVarButton = pSpyplayerVar;
	this->UIVarTable[ UI_SPYPLAYER_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_SPYPLAYER_VAR ].szValue, "0" );

	this->UIVarTable[ UI_FOV_VAR ].fnCallBack = &fnUI_Fov;
	this->UIVarTable[ UI_FOV_VAR ].szCvar = "/fov";
	this->UIVarTable[ UI_FOV_VAR ].pVarButton = pFovVar;
	this->UIVarTable[ UI_FOV_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_FOV_VAR ].szValue, "0" );

	this->UIVarTable[ UI_BIND_VAR ].fnCallBack = &fnUI_Bind;
	this->UIVarTable[ UI_BIND_VAR ].szCvar = "/bind";
	this->UIVarTable[ UI_BIND_VAR ].pVarButton = pBindVar;

	this->UIVarTable[ UI_SCHEME_VAR ].fnCallBack = &fnUI_Scheme;
	this->UIVarTable[ UI_SCHEME_VAR ].szCvar = "/scheme";
	this->UIVarTable[ UI_SCHEME_VAR ].pVarButton = pSchemeVar;

	this->UIVarTable[ UI_HELP_VAR ].fnCallBack = &fnUI_Help;
	this->UIVarTable[ UI_HELP_VAR ].szCvar = "/help";
	this->UIVarTable[ UI_HELP_VAR ].pVarButton = pHelpVar;
	this->UIVarTable[ UI_HELP_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_HELP_VAR ].szValue, "0" );

	this->UIVarTable[ UI_QUIT_VAR ].fnCallBack = &fnUI_Quit;
	this->UIVarTable[ UI_QUIT_VAR ].szCvar = "/quit";
	this->UIVarTable[ UI_QUIT_VAR ].pVarButton = pQuitVar;
	this->UIVarTable[ UI_QUIT_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_QUIT_VAR ].szValue, "0" );

	this->UIVarTable[ UI_SPAM_VAR ].fnCallBack = &fnUI_Spam;
	this->UIVarTable[ UI_SPAM_VAR ].szCvar = "/spam";
	this->UIVarTable[ UI_SPAM_VAR ].pVarButton = pSpamVar;
	this->UIVarTable[ UI_SPAM_VAR ].iValue = 0;
	strcpy( this->UIVarTable[ UI_SPAM_VAR ].szValue, "0" );

	this->BindActionTable[ BINDACTION_TOGGLE ].szAction = "toggle";
	this->BindActionTable[ BINDACTION_TOGGLE ].iNumArgs = 1;
	this->BindActionTable[ BINDACTION_TOGGLE ].fnCallBack = &fn_Bind_Toggle;

	this->BindActionTable[ BINDACTION_SET ].szAction = "set";
	this->BindActionTable[ BINDACTION_SET ].iNumArgs = 2;
	this->BindActionTable[ BINDACTION_SET ].fnCallBack = &fn_Bind_Set;
}

CUserInterface::~CUserInterface( void )
{
	for( int i = 0; i < UI_VARTABLE_SIZE; i++ )
		delete [] UIVarTable[ i ].szBindArgs;

	delete pEspVar;
	delete pRadarVar;
	delete pWinampVar;
	delete pPlaylistVar;
	delete pAimbotVar;
	delete pSpycamVar;
	delete pWallhackVar;
	delete pQuadchamsVar;
	delete pLightVar;
	delete pAutoshootVar;
	delete pSpyplayerVar;
	delete pFovVar;

	delete pBindVar;
	delete pSchemeVar;
	delete pHelpVar;
	delete pQuitVar;
	delete pSpamVar;
}

inline void CUserInterface::ProcessMessage( char *szMessage )
{
	m_pMemo->PushTruncLine( szMessage );
}

inline void CUserInterface::SetMemo( CMemo *pNewMemo )
{
	this->m_pMemo = pNewMemo;
}

void CUserInterface::UpdateCvars( void )
{
	int i, j;

	for( i = 0; i < UI_VARTABLE_SIZE; i++ )
	{
		if( this->UIVarTable[ i ].pKey && this->UIVarTable[ i ].pKey->bGlobalDown )
		{
			switch( this->UIVarTable[ i ].iBindType )
			{
			case BINDACTION_TOGGLE:
				strcpy( this->UIVarTable[ i ].szValue, ( !this->UIVarTable[ i ].iValue ? "1" : "0" ) );
				this->UIVarTable[ i ].iValue = atoi( this->UIVarTable[ i ].szValue );
				break;

			case BINDACTION_SET:
				for( j = 0; j < this->UIVarTable[ i ].iNumBindArgs; j++ )
					strcat( this->UIVarTable[ i ].szValue, AppendStr( "%s ", this->UIVarTable[ i ].szBindArgs[ j ] ) );

				this->UIVarTable[ i ].szValue[ strlen( this->UIVarTable[ i ].szValue ) - 1 ] = '\0';
				this->UIVarTable[ i ].iValue = atoi( this->UIVarTable[ i ].szValue );
				break;
			default:
				break;
			}

			this->UIVarTable[ i ].pKey->bGlobalDown = false;
		}
	}
}