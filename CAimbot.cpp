#include "Common.h"

// Credits to noskill for lots of this.

CAimbot Aimbot; // offsets for the mouse yaw + pitch

WeapInfo_t WeapInfoTable[WEAP_TOTAL + WEAP_INSTAGUN] = 
{
	{ "No Weapon",			0,		0	 },	// WEAP_NONE

	{ "Gunblade",			3000,	5000 },	// WEAP_GUNBLADE (strong) (the gun)
	{ "Shockwave",			1000,	3000 },	// WEAP_SHOCKWAVE (strong)
	{ "Riotgun",			0,		3000 },	// WEAP_RIOTGUN (strong)
	{ "Grenade Launcher",	900,	2000 },	// WEAP_GRENADELAUNCHER (strong)
	{ "Rocket Launcher",	1000,	10000},	// WEAP_ROCKETLAUNCHER (strong)
	{ "Plasmagun",			1700,	5000 },	// WEAP_PLASMAGUN (strong)
	{ "Lasergun",			0,		590	 },	// WEAP_LASERGUN (strong)
	{ "Electrobolt",		0,		6024 },	// WEAP_ELECTROBOLT (strong)
	{ "Instagun",			0,		1024 },	// WEAP_INSTAGUN (strong)

	{ "Gunblade",			0,		64	 },	// WEAP_GUNBLADE (weak) (the blade)
	{ "Shockwave",			1000,	3000 },	// WEAP_SHOCKWAVE (weak)
	{ "Riotgun",			0,		3000 },	// WEAP_RIOTGUN (weak)
	{ "Grenade Launcher",	800,	2000 },	// WEAP_GRENADELAUNCHER (weak)
	{ "Rocket Launcher",	1000,	10000},	// WEAP_ROCKETLAUNCHER (weak)
	{ "Plasmagun",			1700,	5000 },	// WEAP_PLASMAGUN (weak)
	{ "Lasergun",			0,		590	 },	// WEAP_LASERGUN (weak)
	{ "Electrobolt",		6000,	5000 },	// WEAP_ELECTROBOLT (weak)
	{ "Instagun",			6000,	5000 }	// WEAP_INSTAGUN (weak)
}; // NOTE: Electrobolt has a weird config, where the strong fires a strong and a weak (so dist += 5000)
// NOTE2: I set riotgun to 3000, as at 8192 the spread is awful.

CAimbot::CAimbot( )
{
	this->m_bAiming = false;
	this->fOldSens = 0.0f;
}

CAimbot::~CAimbot( )
{
	this->m_bAiming = false;
}

void CAimbot::Aim( bool bHitbox )
{
	vec3_t fAngle, fDirection, fAxis[3], fEnemy;

	// Copy target origin and add ping/vel pred to 'fEnemy'.
	VectorMA( pAimbotTarget->GetVisibleOrigin( bHitbox ), pMe->GetPing( ) / 1000, pAimbotTarget->GetVelocity( ), fEnemy );

	// instagib messes with weap ranges and also means instant projectiles.
	if( !pCgs->serverRules.instagib && !this->CalcWeapon( fEnemy ) )
	{
		this->m_bAiming = false;
		return;
	}

	this->m_bAiming = true;

	AnglesToAxis( pCg->view.refdef.viewangles, fAxis );

	VecToAngles( fAxis[0], pCg->view.refdef.viewangles );
	pCg->view.refdef.viewangles[PITCH] *= -1;

	VectorSubtract( fEnemy, pCg->view.refdef.vieworg, fDirection );
	VecToAngles( fDirection, fAngle );

	fAngle[PITCH] *= -1;

	if		( fAngle[PITCH] > 180.0 )	fAngle[PITCH] -= 360.0;
	else if ( fAngle[PITCH] < -180.0 )	fAngle[PITCH] += 360.0;

	if		( fAngle[YAW] > 180.0 )		fAngle[YAW] -= 360.0;
	else if ( fAngle[YAW] < -180.0 )	fAngle[YAW] += 360.0;

	AnglesToAxis( fAngle, fAxis );

	fAngle[PITCH] -= pCg->view.refdef.viewangles[PITCH];

	if		( fAngle[PITCH] > 180.0 )	fAngle[PITCH] -= 360.0;
	else if ( fAngle[PITCH] < -180.0 )	fAngle[PITCH] += 360.0;

	fAngle[YAW] -= pCg->view.refdef.viewangles[YAW];

	if	    ( fAngle[YAW] > 180.0 )		fAngle[YAW] -= 360.0;
	else if ( fAngle[YAW] < -180.0 )	fAngle[YAW] += 360.0;

	fAngle[PITCH] *= -1;

	g_pfViewangles[YAW] += fAngle[YAW];
	g_pfViewangles[PITCH] += fAngle[PITCH];

	VecToAngles( fAxis[0], pCg->view.refdef.viewangles );
	pCg->view.refdef.viewangles[PITCH] *= -1;
}

void CAimbot::AutoShoot( usercmd_t *pCmd )
{
	if( UI.UIVarTable[ UI_AIMBOT_VAR ].iValue >= 1 )
	{
		if( m_bAiming /*&& !( pCg->frame.playerState.stats[STAT_POINTED_TEAMPLAYER] & 0x1F )*/ )
			pCmd->buttons |= BUTTON_ATTACK;
	}
	else // TODO: own trace ( Set g_iPointedPlayer in CPlayer filling? )
	{
		if( pCg->pointedNum && !( pCg->frame.playerState.stats[STAT_POINTED_TEAMPLAYER] & 0x1F ) )
			pCmd->buttons |= BUTTON_ATTACK;
	}
}

inline bool CAimbot::IsAiming( void )
{
	return( m_bAiming );
}

inline void CAimbot::UnAim( void )
{
	this->m_bAiming = false;
}

bool CAimbot::CheckTargetState( CPlayer *pPossibleTarget )
{
	// CHECK IN THIS ORDER!!

	if( pAimbotTarget == ( CPlayer * )-1 )
		return( true );

	if( !pAimbotTarget->bUpdated )
		return( true );

    if( !pPossibleTarget->bUpdated )
		return( false );

	if( !pPossibleTarget->IsEnemy( ) )
		return( false );

	if( pPossibleTarget->IsDead( ) )
		return( false );

	if( !pPossibleTarget->IsVisible( ) )
		return( false );

	if( pAimbotTarget->IsDead( ) )
		return( true );

	// From the above checks, both pAimbotTarget target and pPossibleTarget are good to aim at.
	// So now we have at least 2 targets on screen. The checks below help us decide which we'll aim at.

	if( !pCgs->serverRules.instagib && ( pCg->frame.playerState.stats[ STAT_GAMETYPE ] == GAMETYPE_DM || pCg->frame.playerState.stats[ STAT_GAMETYPE ] == GAMETYPE_TDM ) )
	{
		if( pPossibleTarget->iHealth >= pAimbotTarget->iHealth )
			return( false );
	}

	if( VectorLength( pPossibleTarget->GetVelocity( ) ) < VectorLength( pAimbotTarget->GetVelocity( ) ) - ( VectorLength( pAimbotTarget->GetVelocity( ) ) / 5 ) )
		return( true ); // they are at least 20% slower than our current target

	if( pPossibleTarget->GetClientEnt( )->current.effects & EF_QUAD )
		return( true ); // they have quad damage

	if( pPossibleTarget->GetClientEnt( )->current.effects & EF_ENEMY_FLAG )
		return( true ); // they have our flag

	if( pPossibleTarget->GetDistance( ) > pAimbotTarget->GetDistance( ) - ( pAimbotTarget->GetDistance( ) / 5 ) )
		return( false ); // they are not more than 20% nearer to us than our current target

	return( true );
}

bool CAimbot::PlayerInFov( CPlayer *pPossibleTarget, int iFovValue )
{
	if( !iFovValue || iFovValue >= 90 )
		return( true );

	vec2_t fScreen = { 0 };
	int iCenter[2] = { pCgs->vidWidth / 2, pCgs->vidHeight / 2 };
	int iFovRel[2] = { pCgs->vidWidth / ( 90 / iFovValue ), pCgs->vidHeight / ( 90 / iFovValue ) };

	oImports.R_TransformVectorToScreen( &pCg->view.refdef, pPossibleTarget->GetLerpOrigin( ), fScreen );

	if( ( fScreen[0] < iCenter[0] - iFovRel[0] || fScreen[0] > iCenter[0] + iFovRel[0] ) 
	||  ( fScreen[1] < iCenter[1] - iFovRel[1] || fScreen[1] > iCenter[1] + iFovRel[1] ) )
		return( false );

	return( true );
}

void CAimbot::FindTarget( CPlayer *pPossibleTarget )
{
	if( UI.UIVarTable[ UI_AIMBOT_VAR ].iValue < 1 || !pMe->bValid || pMe->IsDead( ) || pCg->frame.playerState.stats[ STAT_GAMETYPE ] == GAMETYPE_RACE )
	{
		pAimbotTarget = ( CPlayer * )-1;
		return;
	}

	if( pPossibleTarget->IsEnemy( ) )
		pPossibleTarget->UpdatePrediction( );

	if( CheckTargetState( pPossibleTarget ) )
		pAimbotTarget = pPossibleTarget;

	if( pAimbotTarget != ( CPlayer * )-1 || pAimbotTarget->bUpdated )
	{
		if( !pAimbotTarget->bUpdated
		|| pAimbotTarget->IsDead( )
		|| !pAimbotTarget->IsVisible( )
		|| !pAimbotTarget->IsEnemy( ) 
		|| !PlayerInFov( pAimbotTarget, UI.UIVarTable[ UI_FOV_VAR ].iValue ) )
			pAimbotTarget = ( CPlayer * )-1;
	}
}

/************************************************************************/
/*							Prediction                                  */
/************************************************************************/

// muchos créditos al maqueta del desarrollo del software para esto ;)

void CAimbot::PredictProjectileShot( float fProjectileSpeed, vec3_t fTargetOrigin, vec3_t fTargetVelocity )
{
	vec3_t	fTargetPredOrigin;
	vec3_t	fTargetMoveDir;
	float	fTargetSpeed;
	float	fPredictionTime;
	float	fDistance;
	int		iContents;

	if( fProjectileSpeed <= 0.0f )
		return;

	fTargetSpeed = VectorNormalize2( fTargetVelocity, fTargetMoveDir );

	fDistance = pAimbotTarget->GetDistance( );

	fPredictionTime = fDistance / fProjectileSpeed;

	VectorMA( fTargetOrigin, fPredictionTime * fTargetSpeed, fTargetMoveDir, fTargetPredOrigin );

	iContents = pCG_PointContents( fTargetPredOrigin );

	if( iContents & CONTENTS_SOLID && !( iContents & CONTENTS_PLAYERCLIP ) )
	{
		VectorMA( fTargetOrigin, ( fPredictionTime * 0.5f ) * fTargetSpeed, fTargetMoveDir, fTargetPredOrigin );

		iContents = pCG_PointContents( fTargetPredOrigin );

		if( iContents & CONTENTS_SOLID && !( iContents & CONTENTS_PLAYERCLIP ) )
			return;
	}

	trace_t *pTrace = new trace_t;

	oImports.CM_BoxTrace( pTrace, pCg->view.refdef.vieworg, fTargetPredOrigin, vec3_origin, vec3_origin, NULL, MASK_SHOT|MASK_OPAQUE );

	if( pTrace->fraction == 1.0f || pTrace->ent == pAimbotTarget->GetClientNum( ) )
		VectorCopy( fTargetPredOrigin, fTargetOrigin );

	delete pTrace;
}

// this gets called on all non-instagib servers.
bool CAimbot::CalcWeapon( vec3_t fTargetOrigin )
{
	int			iWeapon = 0, iDistance = pAimbotTarget->GetDistance( );
	WeapInfo_t	*pWeapon;
	vec3_t		fForward, fRight, fUp, fScaledVel;
	float		fAdditional = 0.0f;
	
	iWeapon = pMe->GetClientEnt( )->current.weapon;

	if( !this->ChangeWeapon( iWeapon, pAimbotTarget->GetDistance( ) ) ) 
		return( false ); // they are too far away for any of our current weapons

	pWeapon = ( pCg->frame.playerState.weaponlist[ iWeapon - 1 ][1] > 0 ? &WeapInfoTable[ iWeapon ] : &WeapInfoTable[ iWeapon + WEAP_INSTAGUN ] );

	if( pWeapon->fProjSpeed > 0.0f )
	{
		vec3_t fPossibleTargetOrigin;

		VectorScale( pAimbotTarget->GetVelocity( ), ( pAimbotTarget->GetDistance( ) / 200 ), fScaledVel );

		this->PredictProjectileShot( pWeapon->fProjSpeed, fTargetOrigin, fScaledVel );

		AngleVectors( pAimbotTarget->GetLerpAngles( ), fForward, fRight, fUp );

		// TODO?: fine tune these values (ping prediction / additional / distance / etc.)
		/*switch( iWeapon )
		{
		case WEAP_GRENADELAUNCHER:
			fAdditional = 45.0f - ( ( pCg->view.refdef.vieworg[2] + playerbox_stand_viewheight ) - ( fTargetOrigin[2] ) ) * 0.5;
			fAdditional += ( pAimbotTarget->GetDistance( ) / 100 );

			VectorMA( fTargetOrigin, fAdditional, fUp, fPossibleTargetOrigin );
			break;
		case WEAP_ROCKETLAUNCHER:
			fAdditional = -35.0f + ( ( pCg->view.refdef.vieworg[2] + playerbox_stand_viewheight ) - ( fTargetOrigin[2] ) ) * 0.8;
			VectorMA( fTargetOrigin, fAdditional, fUp, fPossibleTargetOrigin );
			break;
		case WEAP_ELECTROBOLT:
			// fAdditional should be some formula involving the gun speed and player vel.
			VectorMA( fTargetOrigin, fAdditional, fRight, fTargetOrigin );
			break;

		default:
			return( true );
		}*/

		//trace_t *pTrace = new trace_t;

		//oImports.CM_BoxTrace( pTrace, pCg->view.refdef.vieworg, fPossibleTargetOrigin, vec3_origin, vec3_origin, NULL, MASK_SHOT|MASK_OPAQUE );

		//if( pTrace->fraction == 1.0f || pTrace->ent == pAimbotTarget->GetClientNum( ) )
		//	VectorCopy( fPossibleTargetOrigin, fTargetOrigin );

		//delete pTrace;
	}

	return( true );
}

int iWeap_Table[ WEAP_TOTAL-3 ] =
{ // this order is specific to the best guns first
	WEAP_LASERGUN, WEAP_ELECTROBOLT, WEAP_RIOTGUN, WEAP_PLASMAGUN, WEAP_SHOCKWAVE,
	WEAP_INSTAGUN, WEAP_GUNBLADE // always have gunblade..
};

bool CAimbot::ChangeWeapon( int iCurWeapon, float fDistance )
{
	for( int i = 0; i < WEAP_TOTAL-3; i++ )
	{
		if( !pCg->frame.playerState.weaponlist[ iWeap_Table[i] - 1 ][0] )
			continue; // don't have weap

		if( !pCg->frame.playerState.weaponlist[ iWeap_Table[i] - 1 ][2] && !pCg->frame.playerState.weaponlist[ iWeap_Table[i] - 1 ][1] )
			continue; // no ammo

		if( WeapInfoTable[ iWeap_Table[i] ].iTimeout < fDistance && WeapInfoTable[ iWeap_Table[i] + WEAP_INSTAGUN ].iTimeout < fDistance )
			continue; // can't reach enemy

		if( iWeap_Table[i] == iCurWeapon ) // we have this weapon in hand and it has ammo, can reach, so stop checks.
			return( true );

		pCg->last_weapon = ( pCg->latched_weapon != WEAP_NONE ? pCg->latched_weapon : pCg->frame.playerState.stats[STAT_WEAPON_ITEM] );
		pCg->latched_weapon = iWeap_Table[i];

		oImports.Cmd_ExecuteText( EXEC_NOW, AppendStr( "cmd use %s", WeapInfoTable[ iWeap_Table[i] ].szName ) );

		return( true );
	}

	return( false );
}