#include "Common.h"

#ifdef _DEBUG_DRAWBONES_
vec4_t fBones_Color[27] =
{
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 0.1f, 0.0f, 1.0f, 1.0f },
	{ 0.2f, 0.0f, 1.0f, 1.0f },
	{ 0.3f, 0.0f, 1.0f, 1.0f },
	{ 0.4f, 0.0f, 1.0f, 1.0f },
	{ 0.5f, 0.0f, 1.0f, 1.0f },
	{ 0.6f, 0.0f, 1.0f, 1.0f },
	{ 0.7f, 0.0f, 1.0f, 1.0f },
	{ 0.8f, 0.0f, 1.0f, 1.0f },
	{ 0.9f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 0.1f, 0.0f, 1.0f },
	{ 1.0f, 0.2f, 0.0f, 1.0f },
	{ 1.0f, 0.3f, 0.0f, 1.0f },
	{ 1.0f, 0.4f, 0.0f, 1.0f },
	{ 1.0f, 0.5f, 0.0f, 1.0f },
	{ 1.0f, 0.6f, 0.0f, 1.0f },
	{ 1.0f, 0.7f, 0.0f, 1.0f },
	{ 1.0f, 0.8f, 0.0f, 1.0f },
	{ 1.0f, 0.9f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.1f, 1.0f },
	{ 1.0f, 0.0f, 0.2f, 1.0f },
	{ 1.0f, 0.0f, 0.3f, 1.0f },
	{ 1.0f, 0.0f, 0.4f, 1.0f },
	{ 1.0f, 0.0f, 0.5f, 1.0f },
	{ 1.0f, 0.0f, 0.6f, 1.0f },
	{ 1.0f, 0.0f, 0.7f, 1.0f },
	{ 1.0f, 0.0f, 0.8f, 1.0f }
};
#endif // _DEBUG_DRAWBONES_

#define TABLE_GENERIC_SIZE	15
#define TABLE_BOBOT_SIZE	16
#define TABLE_PADPORK_SIZE	20

int iBoneTable_Generic[TABLE_GENERIC_SIZE] = { 4, 7, 13, 5, 6, 3, 2, 0, 1, 8, 14, 19, 23, 20, 24 };
int iBoneTable_Bobot[TABLE_BOBOT_SIZE] = { 8, 12, 20, 9, 10, 7, 6, 3, 5, 13, 21, 28, 33, 29, 34, 11 };
int iBoneTable_Padpork[TABLE_PADPORK_SIZE] = { 5, 4, 12, 19, 6, 7, 3, 2, 0, 1, 13, 20, 26, 30, 27, 31, 8, 9, 10, 11 };

int iHitboxTable_Diff[7] = { 0, +1, -1, +2, -2 }; // Visibility priority

CPlayer vPlayers[NUM_MAX_CLIENTS];
CPlayer *pAimbotTarget = ( CPlayer * )-1, *pMe = ( CPlayer * )-1, *pSpycamClient = ( CPlayer * )-1;

void UpdatePlayers( centity_t *pCent )
{
	int i = pCent->current.number;

	vPlayers[ i ].bDraw2D = false;

	if( pCent->serverFrame != pCg->frame.serverFrame || pCgs->clientInfo[ i - 1 ].name[0] < 1 )
	{
		if( pAimbotTarget != ( CPlayer * )-1 )
		{
			if( pAimbotTarget->GetClientNum( ) == i )
				pAimbotTarget = ( CPlayer * )-1;
		}

		if( pMe != ( CPlayer * )-1 )
		{
			if( pMe->GetClientNum( ) == i )
				pMe->~CPlayer( );
		}

		if( pSpycamClient != ( CPlayer * )-1 )
		{
			if( pSpycamClient->GetClientNum( ) == i )
				pSpycamClient->~CPlayer( );
		}

		vPlayers[ i ].~CPlayer( );
		return;
	}

	if( !vPlayers[ i ].bValid )
		vPlayers[ i ] = CPlayer( pCent );

	vPlayers[ i ].UpdatePlayerState( );
	vPlayers[ i ].UpdateTeam( );
	vPlayers[ i ].UpdateVectors( );

	vPlayers[ i ].bUpdated = true;
	
	if( vPlayers[ i ].GetClientNum( ) == pMe->GetClientNum( ) )
	{
		vPlayers[ i ].UpdatePing( ); // only need our ping for aimbot pred.
		return;
	}

	vPlayers[ i ].UpdateDistance( );

	Aimbot.FindTarget( &vPlayers[ i ] );

	if( vPlayers[ i ].IsDead( ) )
		return;

	vPlayers[ i ].bDraw2D = true;

	pSpycamClient = ( pAimbotTarget == ( CPlayer * )-1 || !pAimbotTarget->bUpdated ? &vPlayers[ i ] : pAimbotTarget );
}

CPlayer::CPlayer( )
{
	this->bValid = false;
	this->bUpdated = false;
	this->m_pClientEnt = ( centity_t * )-1;
	this->m_iClientNum = -1;
	this->m_bMe = false;
	this->m_bDead = true;
	this->m_iNumAllBones = 0;
	this->m_iVisibleBone = 0;
	this->m_iVisibleHitbox = 0;
	this->m_iHistoryIndex = 0;
	this->m_iPing = 0;
	this->iHealth = 0;

	memset( this->m_fHitboxOrigin, 0, sizeof( m_fHitboxOrigin ) );
	memset( this->m_fBoneOrigin, 0, sizeof( m_fBoneOrigin ) );
	memset( this->m_fLerpAngles, 0, sizeof( m_fLerpAngles ) );
	memset( this->m_fLerpOrigin, 0, sizeof( m_fLerpOrigin ) );
	memset( this->m_fVelocity, 0, sizeof( m_fVelocity ) );
	memset( this->m_fOriginHistory, 0, sizeof( m_fOriginHistory ) );
	memset( this->m_iTimeHistory, 0, sizeof( m_iTimeHistory ) );
	memset( this->m_fColor, 0, sizeof( m_fColor ) );
}

CPlayer::CPlayer( centity_t *pClientEnt )
{
	this->bValid = true;
	this->bUpdated = false;
	this->m_pClientEnt = pClientEnt;
	this->m_iClientNum = m_pClientEnt->current.number;
	this->m_bMe = ( m_iClientNum - 1 == pCgs->playerNum );
	this->m_bDead = true;
	this->m_iNumAllBones = 0;
	this->m_iVisibleBone = 0;
	this->m_iVisibleHitbox = 0;
	this->m_iHistoryIndex = 0;
	this->m_iPing = 0;
	this->iHealth = 0;
	
	memset( this->m_fHitboxOrigin, 0, sizeof( m_fHitboxOrigin ) );
	memset( this->m_fBoneOrigin, 0, sizeof( m_fBoneOrigin ) );
	memset( this->m_fLerpAngles, 0, sizeof( m_fLerpAngles ) );
	memset( this->m_fLerpOrigin, 0, sizeof( m_fLerpOrigin ) );
	memset( this->m_fVelocity, 0, sizeof( m_fVelocity ) );
	memset( this->m_fOriginHistory, 0, sizeof( m_fOriginHistory ) );
	memset( this->m_iTimeHistory, 0, sizeof( m_iTimeHistory ) );
	memset( this->m_fColor, 0, sizeof( m_fColor ) );

	m_fColor[3] = 1.0f;

	if( m_bMe )
	{
		this->UpdatePing( );
		this->m_bVisible = true;
		pMe = this;
	}
	else if( m_bEnemy )
	{
		VectorSet( m_fHitboxMins[BOX_STAND], -16, -16, -24 ); // SDK values
		VectorSet( m_fHitboxMaxs[BOX_STAND], +16, +16, +40 ); // ..

		VectorSet( m_fHitboxMins[BOX_CROUCH], -16, -16, -24 ); // ..
		VectorSet( m_fHitboxMaxs[BOX_CROUCH], +16, +16, +16 ); // ..
	}
}

CPlayer::~CPlayer( )
{
	this->bValid = false;
	this->bUpdated = false;
	this->bDraw2D = false;
	this->m_pClientEnt = ( centity_t * )-1;
	this->m_iClientNum = -1;
	this->m_bMe = false;
	this->m_bDead = true;
	this->iHealth = 0;
}

inline centity_t *CPlayer::GetClientEnt( void )
{
	return( m_pClientEnt );
}

inline int CPlayer::GetClientNum( void )
{
	return( m_iClientNum );
}

inline float CPlayer::GetDistance( void )
{
	return( m_fDistance );
}

inline float *CPlayer::GetLerpAngles( void )
{
	return( m_fLerpAngles );
}

inline float *CPlayer::GetLerpOrigin( void )
{
	return( m_fLerpOrigin );
}

inline char *CPlayer::GetName( void )
{
	return( pCgs->clientInfo[ m_iClientNum - 1 ].name );
}

inline int CPlayer::GetPing( void )
{
	return( m_iPing );
}

inline int CPlayer::GetTeam( void )
{
	return( m_pClientEnt->current.team );
}

inline float *CPlayer::GetVelocity( void )
{
	return( m_fVelocity );
}

inline float *CPlayer::GetVisibleOrigin( bool bHitbox )
{
	return( bHitbox ? m_fHitboxOrigin[ m_iVisibleHitbox ] : m_fBoneOrigin[ m_iVisibleBone ] );
}

inline bool CPlayer::IsEnemy( void )
{
	return( m_bEnemy );
}

inline bool CPlayer::IsVisible( void )
{
	return( m_bVisible );
}

inline bool CPlayer::IsDead( void )
{
	return( m_bDead );
}

void CPlayer::UpdateBones( void )
{
	int i = 0;

	if( strstr( m_pClientEnt->skel->model->name, "viciious" ) || strstr( m_pClientEnt->skel->model->name, "monada" ) || strstr( m_pClientEnt->skel->model->name, "silverclaw" ) )
	{
		m_vBoneTable.clear( );

		for( i = 0; i < TABLE_GENERIC_SIZE; i++ )
			m_vBoneTable.push_back( iBoneTable_Generic[ i ] );
	}
	else if( strstr( m_pClientEnt->skel->model->name, "bobot" ) )
	{
		m_vBoneTable.clear( );

		for( i = 0; i < TABLE_BOBOT_SIZE; i++ )
			m_vBoneTable.push_back( iBoneTable_Bobot[ i ] );
	}
	else if( strstr( m_pClientEnt->skel->model->name, "padpork" ) )
	{
		m_vBoneTable.clear( );

		for( i = 0; i < TABLE_PADPORK_SIZE; i++ )
			m_vBoneTable.push_back( iBoneTable_Padpork[ i ] );
	}
	else
	{
#ifdef _DEBUG_ADDLOG_
		AddLog( "ERROR: CPlayer::UpdateBones( ): Skeleton not recognised (%s)", m_pClientEnt->skel->model->name );
#endif // _DEBUG_ADDLOG_
		return;
	}

	m_iNumAllBones = m_pClientEnt->skel->numBones;
}

inline void CPlayer::UpdateDistance( void )
{
	this->m_fDistance = DistanceFast( pCg->view.refdef.vieworg, m_fLerpOrigin );
}

void CPlayer::UpdateVectors( void )
{
	int i;
	vec3_t fForward, fRight, fUp, fPlayerAngles;

	for( i = 0; i < 3; i++ )
	{
		this->m_fLerpOrigin[ i ] = m_pClientEnt->prev.origin[ i ] + pCg->lerpfrac * ( m_pClientEnt->current.origin[ i ] - m_pClientEnt->prev.origin[ i ] );
		this->m_fLerpAngles[ i ] = LerpAngle( m_pClientEnt->prev.angles[ i ], m_pClientEnt->current.angles[ i ], pCg->lerpfrac );
	}

	if( m_bDead || !m_bEnemy )
	{
		m_bVisible = false; // don't care if team mates are visible or not
		return;
	}

	trace_t *pTrace = new trace_t;
	Stance_t HitboxStance = BOX_STAND; // default

	switch( UI.UIVarTable[ UI_AIMBOT_VAR ].iValue )
	{
		case 1: // hitbox aiming
			if( pCg_EntPModels[ m_iClientNum ].anim.current[LOWER] == LEGS_CRWALK 
			|| pCg_EntPModels[ m_iClientNum ].anim.current[LOWER] == LEGS_IDLECR )
			{
				HitboxStance = BOX_CROUCH;
			}

			for( i = 0; i < 7; i++ )
			{
				// TODO: NOTE: if hitboxes/origin change in future, use VectorAvg.
				VectorSet( m_fHitboxOrigin[i], 
					m_fLerpOrigin[0], 
					m_fLerpOrigin[1], 
					m_fLerpOrigin[2] + ( ( m_fHitboxMins[HitboxStance][2] + m_fHitboxMaxs[HitboxStance][2] ) / 2 ) + ( ( ( m_fHitboxMins[HitboxStance][2] - m_fHitboxMins[HitboxStance][2] ) / 6 ) * iHitboxTable_Diff[i] ) ); 

				// Update visibility.
				oImports.CM_BoxTrace( pTrace, pCg->view.refdef.vieworg, m_fHitboxOrigin[i], vec3_origin, vec3_origin, NULL, MASK_SHOT|MASK_OPAQUE );
				this->m_bVisible = ( pTrace->fraction == 1.0f || pTrace->ent == m_iClientNum );

				if( m_bVisible )
				{
					m_iVisibleHitbox = i;
					break; // no point looping through less accurate hitbox poses.
				}
			}
			break;

		case 2: // bone aiming		
			if( m_pClientEnt->skel->numBones != m_iNumAllBones ) // player has changed their model
				this->UpdateBones( );

			// My quaternion maths is shit, but this method works (please tell me the proper way of doing this, if there is)
			VectorSet( fPlayerAngles, 0, m_fLerpAngles[1], 0 );
			AngleVectors( fPlayerAngles, fForward, fRight, fUp );

			fForward[2] = -fForward[2];

			for( i = 0; i < ( int )m_vBoneTable.size( ); i++ ) // loop through the bones, more importance -> less importance.
			{
				VectorCopy( m_fLerpOrigin, m_fBoneOrigin[ m_vBoneTable[ i ] ] );

				VectorMA( 
					m_fBoneOrigin[ m_vBoneTable[ i ] ], 
					m_pClientEnt->ent.boneposes[ m_vBoneTable[ i ] ].origin[0], 
					fForward, 
					m_fBoneOrigin[ m_vBoneTable[ i ] ] );

				VectorMA( 
					m_fBoneOrigin[ m_vBoneTable[ i ] ], 
					-m_pClientEnt->ent.boneposes[ m_vBoneTable[ i ] ].origin[1], 
					fRight, 
					m_fBoneOrigin[ m_vBoneTable[ i ] ] );

				VectorMA( 
					m_fBoneOrigin[ m_vBoneTable[ i ] ], 
					m_pClientEnt->ent.boneposes[ m_vBoneTable[ i ] ].origin[2], 
					fUp, 
					m_fBoneOrigin[ m_vBoneTable[ i ] ] );

				// Update visibility.
				oImports.CM_BoxTrace( pTrace, pCg->view.refdef.vieworg, m_fBoneOrigin[ m_vBoneTable[ i ] ], vec3_origin, vec3_origin, NULL, MASK_SHOT|MASK_OPAQUE );
				this->m_bVisible = ( pTrace->fraction == 1.0f || pTrace->ent == m_iClientNum );

				if( m_bVisible )
				{
					m_iVisibleBone = m_vBoneTable[ i ];
					break; // no point looping through less useful bones.
				}
			}
			break;
		default: // aimbot off
			oImports.CM_BoxTrace( pTrace, pCg->view.refdef.vieworg, m_fLerpOrigin, vec3_origin, vec3_origin, NULL, MASK_SHOT|MASK_OPAQUE );
			this->m_bVisible = ( pTrace->fraction == 1.0f || pTrace->ent == m_iClientNum );
			break;
	}

	delete pTrace;
}

inline void CPlayer::UpdatePing( void )
{
	m_iPing = pSCB_Players[ m_iClientNum - 1 ].ping;
}

void CPlayer::UpdatePrediction( void )
{
	float fScale = 0.0f;
	vec3_t fLerped = { 0.0f, 0.0f, 0.0f }, fFrameDiff = { 0.0f, 0.0f, 0.0f };
	int iCurFrame = 0, iNextFrame = 0, iTargetTime = 0;

	// muchos créditos noskill :)
	
	VectorCopy( m_fLerpOrigin, m_fOriginHistory[ m_iHistoryIndex ] );

	m_iTimeHistory[ m_iHistoryIndex ] = oImports.Milliseconds( );

	iTargetTime = m_iTimeHistory[ m_iHistoryIndex ] - 100;

	iCurFrame = iNextFrame = m_iHistoryIndex;

	do
	{
		if( iTargetTime >= m_iTimeHistory[ iCurFrame ] )
			break;

		iNextFrame = iCurFrame;

		iCurFrame--;

		if( iCurFrame == -1 )
			iCurFrame = NUM_HISTORY_FRAMES - 1;
	} while( iCurFrame != m_iHistoryIndex );

	fScale = ( iTargetTime - m_iTimeHistory[ iCurFrame ] ) / ( m_iTimeHistory[ iNextFrame ] - m_iTimeHistory[ iCurFrame ] );

	VectorSubtract( m_fOriginHistory[ iNextFrame ], m_fOriginHistory[ iCurFrame ], fFrameDiff );

	VectorMA( m_fOriginHistory[ iCurFrame ], fScale, fFrameDiff, fLerped );

	VectorSubtract( m_fLerpOrigin, fLerped, m_fVelocity );

	m_iHistoryIndex++;

	if( m_iHistoryIndex == NUM_HISTORY_FRAMES )
		m_iHistoryIndex = 0;
}

void CPlayer::UpdateTeam( void )
{
	if( m_bMe )
	{
		this->m_bEnemy = false;
		goto jmp_UpdateColor;
	}

	if( pMe->GetTeam( ) == TEAM_SPECTATOR || m_pClientEnt->current.team == TEAM_SPECTATOR )
	{
		this->m_bEnemy = false;
		goto jmp_UpdateColor;
	}

	if( pMe->GetTeam( ) == TEAM_PLAYERS ) // Deathmatch
	{
		this->m_bEnemy = true;
		goto jmp_UpdateColor;
	}

	if( pMe->GetTeam( ) == m_pClientEnt->current.team )
	{
		this->m_bEnemy = false;
		goto jmp_UpdateColor;
	}

	// anything else = enemy;
	this->m_bEnemy = true;

jmp_UpdateColor:
	m_fColor[0] = m_bEnemy;
	m_fColor[1] = 0;
	m_fColor[2] = !m_bEnemy;
}

inline void CPlayer::UpdatePlayerState( void )
{
	m_bDead = ( m_pClientEnt->current.type == ET_CORPSE || !m_pClientEnt->current.modelindex );

	if( this->iHealth <= 0 && !m_bDead )
		this->iHealth = 100;
}

inline void CPlayer::DrawLight( void )
{
	oImports.R_AddLightToScene( m_fLerpOrigin, 1000, 1.0f, 1.0f, 1.0f, NULL );
}

void CPlayer::Draw_2D( void )
{
	if( !this->bDraw2D )
		return;

#ifdef _DEBUG_DRAWBONES_
	static bool bShowBones;

	if( GetAsyncKeyState( VK_F9 ) & 1 )
		bShowBones = !bShowBones;

	for( int j = 0; j < m_pClientEnt->skel->numBones; j++ )
	{
		vec3_t f3dOrig = { 0.0f, 0.0f, 0.0f };
		vec2_t f2dOrig = { 0.0f, 0.0f };
		vec3_t fo, ri, up, playerAngles;

		playerAngles[0] = 0;
		playerAngles[1] = m_fLerpAngles[1];
		playerAngles[2] = 0;

		AngleVectors( playerAngles, fo, ri, up );

		fo[2] = -fo[2];

		VectorCopy( m_fLerpOrigin, f3dOrig );

		VectorMA( f3dOrig, m_pClientEnt->ent.boneposes[ j ].origin[0], fo, f3dOrig );
		VectorMA( f3dOrig, -m_pClientEnt->ent.boneposes[ j ].origin[1], ri, f3dOrig );
		VectorMA( f3dOrig, m_pClientEnt->ent.boneposes[ j ].origin[2], up, f3dOrig );

		if( WorldToScreen( f3dOrig, f2dOrig ) )
		{
			oImports.R_DrawStretchPic
			( 
				f2dOrig[0],
				pCgs->vidHeight - f2dOrig[1],
				2.0, 2.0,
				0, 0, 1, 1,
				fBones_Color[j], 
				pCgs->shaderWhite 
			);

			if( bShowBones )
			{
				oImports.SCR_DrawString
				( 
					f2dOrig[0],
					pCgs->vidHeight - f2dOrig[1],
					ALIGN_LEFT_TOP,
					AppendStr( "%i", j ),
					pCgs->fontSystemSmall,
					fBones_Color[j]
				);
			}

			oImports.SCR_DrawString
			( 
				2, 
				( int )( g_iSizeSmallFont * j ) + 3, 
				ALIGN_LEFT_TOP,
				AppendStr( "[%i] %s", j, m_pClientEnt->skel->bones[ j ].name ),
				pCgs->fontSystemSmall,
				fBones_Color[j]
			);
		}
	}
#endif // _DEBUG_DRAWBONES_

#ifdef _DEBUG_DRAWBBOXES_
	vec3_t f3dOrig[8] = { 0 };
	vec2_t f2dOrig[8] = { 0 };
	vec3_t f3dHitboxAims = { 0 }, fHitboxMins, fHitboxMaxs;
	vec2_t f2dHitboxAims = { 0 };
	int i = 0;
	bool bVisible = false;

	if( pCg_EntPModels[ m_iClientNum ].anim.current[LOWER] == LEGS_CRWALK 
	|| pCg_EntPModels[ m_iClientNum ].anim.current[LOWER] == LEGS_IDLECR )
	{
		VectorCopy( playerbox_crouch_mins, fHitboxMins );
		VectorCopy( playerbox_crouch_maxs, fHitboxMaxs );
	}
	else
	{
		VectorCopy( playerbox_stand_mins, fHitboxMins );
		VectorCopy( playerbox_stand_maxs, fHitboxMaxs );
	}

	BuildBoxPoints( f3dOrig, m_fLerpOrigin, fHitboxMins, fHitboxMaxs );

	for( i = 0; i < 8; i++ )
	{
		if( !WorldToScreen( f3dOrig[i], f2dOrig[i] ) )
			bVisible = false;
	}

	for( i = 0; i < 7; i++ )
	{
		VectorSet( f3dHitboxAims, 
				m_fLerpOrigin[0], 
				m_fLerpOrigin[1], 
				m_fLerpOrigin[2] + ( ( fHitboxMins[2] + fHitboxMaxs[2] ) / 2 ) + ( ( ( fHitboxMaxs[2] - fHitboxMins[2] ) / 6 ) * iHitboxTable_Diff[i] ) ); 

		if( WorldToScreen( f3dHitboxAims, f2dHitboxAims ) )
		{
			oImports.SCR_DrawString
			(
				f2dHitboxAims[0],
				pCgs->vidHeight - f2dHitboxAims[1],
				ALIGN_CENTER_MIDDLE,
				AppendStr( "%i", iHitboxTable_Diff[i] ),
				pCgs->fontSystemSmall,
				colorYellow
			);
		}
	}

	//if( bVisible )
	//{
		pglEnable( GL_BLEND );
		pglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		pglDisable( GL_TEXTURE_2D );

		pglColor4fv( colorRed );

		pglBegin( GL_LINE_LOOP );
		pglVertex2f( f2dOrig[0][0], pCgs->vidHeight - f2dOrig[0][1] );
		pglVertex2f( f2dOrig[1][0], pCgs->vidHeight - f2dOrig[1][1] );
		pglVertex2f( f2dOrig[2][0], pCgs->vidHeight - f2dOrig[2][1] );
		pglVertex2f( f2dOrig[3][0], pCgs->vidHeight - f2dOrig[3][1] );
		pglEnd( );

		pglBegin( GL_LINE_LOOP );
		pglVertex2f( f2dOrig[4][0], pCgs->vidHeight - f2dOrig[4][1] );
		pglVertex2f( f2dOrig[5][0], pCgs->vidHeight - f2dOrig[5][1] );
		pglVertex2f( f2dOrig[6][0], pCgs->vidHeight - f2dOrig[6][1] );
		pglVertex2f( f2dOrig[7][0], pCgs->vidHeight - f2dOrig[7][1] );
		pglEnd( );

		pglBegin( GL_LINES );
		pglVertex2f( f2dOrig[0][0], pCgs->vidHeight - f2dOrig[0][1] );
		pglVertex2f( f2dOrig[6][0], pCgs->vidHeight - f2dOrig[6][1] );

		pglVertex2f( f2dOrig[1][0], pCgs->vidHeight - f2dOrig[1][1] );
		pglVertex2f( f2dOrig[5][0], pCgs->vidHeight - f2dOrig[5][1] );

		pglVertex2f( f2dOrig[2][0], pCgs->vidHeight - f2dOrig[2][1] );
		pglVertex2f( f2dOrig[4][0], pCgs->vidHeight - f2dOrig[4][1] );

		pglVertex2f( f2dOrig[3][0], pCgs->vidHeight - f2dOrig[3][1] );
		pglVertex2f( f2dOrig[7][0], pCgs->vidHeight - f2dOrig[7][1] );
		pglEnd( );

		pglEnable( GL_TEXTURE_2D );
	//}

#endif // _DEBUG_DRAWBBOXES_

	/* Radar */
	if( Gui.GetActiveState( ) && Gui.RadarWnd.GetMaximized( ) && UI.UIVarTable[ UI_RADAR_VAR ].iValue >= 1 )
	{
		vec3_t fLookDir, fPlayerDir;
		float fAngle, fDistance;
		float *fRadDim = Gui.RadarWnd.GetDimensions( );

		VectorSubtract( pMe->GetLerpOrigin( ), m_fLerpOrigin, fPlayerDir );

		fPlayerDir[2] = 0;

		fDistance = VectorNormalize( fPlayerDir );

		if( fDistance > 2100 )	// this value is just from trial and error
			return;

		fDistance /= 2350;		// ^^^
		fDistance *= fRadDim[2] / 2;

		AngleVectors( pCg->view.refdef.viewangles, fLookDir, NULL, NULL );

		fLookDir[2] = 0;

		VectorNormalize( fLookDir );

		fAngle = atan2( fLookDir[0], fLookDir[1] ) - atan2( fPlayerDir[0], fPlayerDir[1] );

		oImports.R_DrawStretchPic
		( 
			fRadDim[0] + ( fRadDim[2] / 2 ) + sin( fAngle ) * fDistance,
			fRadDim[1] + ( fRadDim[3] / 2 ) + cos( fAngle ) * fDistance, 
			4.0, 4.0,
			0, 0, 1, 1,
			m_fColor, 
			pCgs->shaderWhite 
		);
	}

	/* ESP */
	if( m_iClientNum != pCg->pointedNum && UI.UIVarTable[ UI_ESP_VAR ].iValue >= 1 && this->m_bEnemy )
	{
		vec3_t f3dHeadOrig = { 0 };
		vec2_t f2dHeadOrig = { 0 };
		vec4_t fColor = { 0 };

		Vector4Copy( m_fColor, fColor );

		VectorSet( f3dHeadOrig, m_fLerpOrigin[0], m_fLerpOrigin[1], m_fLerpOrigin[2] + 40 + 16 );

		if( WorldToScreen( f3dHeadOrig, f2dHeadOrig ) )
		{
			fColor[3] = 1.2f - ( m_fDistance / 10 ) * ( 1.0f / 255 );

			if( fColor[3] < 0.1f )
				fColor[3] = 0.1f;

			oImports.SCR_DrawString
			(
				f2dHeadOrig[0],
				( pCgs->vidHeight - f2dHeadOrig[1] ) - g_iSizeSmallFont,
				ALIGN_CENTER_MIDDLE,
				this->GetName( ),
				pCgs->fontSystemSmall,
				fColor
			);
		}
	}

	/* FOV */
	if( UI.UIVarTable[ UI_FOV_VAR ].iValue >= 1 && UI.UIVarTable[ UI_AIMBOT_VAR ].iValue >= 1 )
	{
		int iCenter[2] = { pCgs->vidWidth / 2, pCgs->vidHeight / 2 };
		int iFovRel[2] = { pCgs->vidWidth / ( 90 / UI.UIVarTable[ UI_FOV_VAR ].iValue ), pCgs->vidHeight / ( 90 / UI.UIVarTable[ UI_FOV_VAR ].iValue ) };

		pglDisable( GL_TEXTURE_2D );

		pglColor4fv( colorRed );

		pglBegin( GL_LINE_LOOP );
		pglVertex2f( iCenter[0] - iFovRel[0], pCgs->vidHeight - ( iCenter[1] - iFovRel[1] ) );
		pglVertex2f( iCenter[0] + iFovRel[0], pCgs->vidHeight - ( iCenter[1] - iFovRel[1] ) );
		pglVertex2f( iCenter[0] + iFovRel[0], pCgs->vidHeight - ( iCenter[1] + iFovRel[1] ) );
		pglVertex2f( iCenter[0] - iFovRel[0], pCgs->vidHeight - ( iCenter[1] + iFovRel[1] ) );
		pglEnd( );

		pglEnable( GL_TEXTURE_2D );
	}

	this->bDraw2D = false;
}