/*
   Copyright (C) 2002-2003 Victor Luchits

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 */
// cg_local.h -- local definitions for client game module

#include "../game/q_shared.h"
#include "../gameshared/gs_public.h"
#include "ref.h"

#include "cg_public.h"
//	bigmac
//#include "cg_syscalls.h"
//	/bigmac

#define MAX_PREDICTED_EDICTS 32

#define CG_OBITUARY_HUD	    1
#define CG_OBITUARY_CENTER  2
#define CG_OBITUARY_CONSOLE 4

#define ITEM_RESPAWN_TIME   1000

#define FLAG_TRAIL_DROP_DELAY 300
#define HEADICON_TIMEOUT 8000

#define INSTAGUN_DERECOIL_TIME 150

enum
{
	LOCALEFFECT_EV_PLAYER_TELEPORT_IN
	, LOCALEFFECT_EV_PLAYER_TELEPORT_OUT
	, LOCALEFFECT_VSAY_HEADICON
	, LOCALEFFECT_VSAY_HEADICON_TIMEOUT
	, LOCALEFFECT_TRAIL_LAST_DROP
	, LOCALEFFECT_ROCKETFIRE_LAST_DROP
	, MAX_LOCALEFFECTS = 128
};

typedef struct
{
	entity_state_t current;
	entity_state_t prev;        // will always be valid, but might just be a copy of current

	int serverFrame;            // if not current, this ent isn't in the frame
	unsigned int fly_stoptime;

	int respawnTime;
	int respawnIndicator;

	entity_t ent;                   // interpolated, to be added to render list
	unsigned int type;
	unsigned int renderfx;
	unsigned int effects;
	struct cgs_skeleton_s *skel;

	vec3_t velocity;

	gitem_t	*item;

	//effects
	vec3_t trailOrigin;         // for particle trails

	// local effects from events timers
	unsigned int localEffects[MAX_LOCALEFFECTS];

	vec3_t teleportedFrom;
	byte_vec4_t outlineColor;
	byte_vec4_t color;

	// for predicted entities
	unsigned int predictedCommand; // remove after we get a snap from server acking this command

	// used for client side animation of player models
	int lastAnims;
	int lastVelocities[4][4];
	qboolean jumpedLeft;
	vec3_t animVelocity;
} centity_t;

#include "cg_pmodels.h"

typedef struct cgs_media_handle_s
{
	char *name;
	void *data;
	struct cgs_media_handle_s *next;
} cgs_media_handle_t;

#define MAX_MEATY_GIBS	4

#define STAT_MINUS	10  // num frame for '-' stats digit

typedef struct
{
	// sounds
	cgs_media_handle_t *sfxChat;

	// timers
	cgs_media_handle_t *sfxTimerBipBip;
	cgs_media_handle_t *sfxTimerPloink;

	cgs_media_handle_t *sfxRic[3];

	cgs_media_handle_t *sfxWeaponUp;
	cgs_media_handle_t *sfxWeaponUpNoAmmo;  //[end]

	//--------------------------------------

	cgs_media_handle_t *sfxWeaponHit[4];
	cgs_media_handle_t *sfxWeaponKill;
	cgs_media_handle_t *sfxWeaponHitTeam;

	cgs_media_handle_t *sfxItemRespawn;
	cgs_media_handle_t *sfxItemQuadRespawn;
	cgs_media_handle_t *sfxItemWarShellRespawn;
	cgs_media_handle_t *sfxTeleportIn;
	cgs_media_handle_t *sfxTeleportOut;
	cgs_media_handle_t *sfxShellHit;

	// Gunblade sounds :
	cgs_media_handle_t *sfxGunbladeWeakShot[3];
	cgs_media_handle_t *sfxGunbladeStrongShot;
	cgs_media_handle_t *sfxBladeFleshHit[3];
	cgs_media_handle_t *sfxBladeWallHit[2];
	cgs_media_handle_t *sfxGunbladeStrongHit[3];

	// Riotgun sounds :
	cgs_media_handle_t *sfxRiotgunWeakShot;
	cgs_media_handle_t *sfxRiotgunStrongShot;
	cgs_media_handle_t *sfxRiotgunWeakHit;
	cgs_media_handle_t *sfxRiotgunStrongHit;

	// Grenade launcher sounds :
	cgs_media_handle_t *sfxGrenadeLauncherWeakShot;
	cgs_media_handle_t *sfxGrenadeLauncherStrongShot;
	cgs_media_handle_t *sfxGrenadeWeakBounce[2];
	cgs_media_handle_t *sfxGrenadeStrongBounce[2];
	cgs_media_handle_t *sfxGrenadeWeakExplosion;
	cgs_media_handle_t *sfxGrenadeStrongExplosion;

	// Rocket launcher sounds :
	cgs_media_handle_t *sfxRocketLauncherWeakShot;
	cgs_media_handle_t *sfxRocketLauncherStrongShot;
	cgs_media_handle_t *sfxRocketLauncherWeakHit;
	cgs_media_handle_t *sfxRocketLauncherStrongHit;

	// Plasmagun sounds
	cgs_media_handle_t *sfxPlasmagunWeakShot;
	cgs_media_handle_t *sfxPlasmagunStrongShot[3];
	cgs_media_handle_t *sfxPlasmaWeakHit;
	cgs_media_handle_t *sfxPlasmaStrongHit;

	// Electrobolt sounds
	cgs_media_handle_t *sfxElectroboltWeakShot;
	cgs_media_handle_t *sfxElectroboltStrongShot;

	// Instagun sounds
	cgs_media_handle_t *sfxInstagunWeakShot;
	cgs_media_handle_t *sfxInstagunStrongShot;

	// Lasergun sounds
	cgs_media_handle_t *sfxLasergunStrongShot;
	cgs_media_handle_t *sfxLasergunWeakShot;

	// VSAY sounds
	cgs_media_handle_t *sfxVSaySounds[VSAY_TOTAL];

	//no wsw

	// models
	//	cgs_media_handle_t		*modTeleportEffect;
	cgs_media_handle_t *modDash;

	// wsw : jal : gib models
	cgs_media_handle_t *modMeatyGibs[MAX_MEATY_GIBS];

	//wsw weapon sfx
	cgs_media_handle_t *modRocketExplosion;
	//	cgs_media_handle_t		*modGrenadeExplosion;
	cgs_media_handle_t *modPlasmaExplosion;
	//	cgs_media_handle_t		*modBoltExplosion;
	//	cgs_media_handle_t		*modInstaExplosion;

	cgs_media_handle_t *modBulletExplode;
	cgs_media_handle_t *modBladeWallHit;
	cgs_media_handle_t *modBladeWallExplo;

	cgs_media_handle_t *modElectroBoltWallHit;
	cgs_media_handle_t *modInstagunWallHit;

	//no wsw

	//splitmodels[start] //eject brass-debris
	cgs_media_handle_t *modEjectBrassMachinegun;
	cgs_media_handle_t *modEjectBrassShotgun;   //[end]

	cgs_media_handle_t *shaderParticle;
	cgs_media_handle_t *shaderGrenadeExplosion;
	cgs_media_handle_t *shaderRocketExplosion;
	cgs_media_handle_t *shaderRocketExplosionRing;
	cgs_media_handle_t *shaderBulletExplosion;
	cgs_media_handle_t *shaderPowerupQuad;
	cgs_media_handle_t *shaderQuadWeapon;
	cgs_media_handle_t *shaderShellEffect;
	cgs_media_handle_t *shaderRaceGhostEffect;
	cgs_media_handle_t *shaderWaterBubble;
	//	cgs_media_handle_t		*shaderTeleportEffect;
	cgs_media_handle_t *shaderSmokePuff;

	cgs_media_handle_t *shaderSmokePuff1;
	cgs_media_handle_t *shaderSmokePuff2;
	cgs_media_handle_t *shaderSmokePuff3;
	cgs_media_handle_t *shaderSmokePuff1dark;
	cgs_media_handle_t *shaderSmokePuff2dark;
	cgs_media_handle_t *shaderSmokePuff3dark;

	cgs_media_handle_t *shaderStrongRocketFireTrailPuff;
	cgs_media_handle_t *shaderWeakRocketFireTrailPuff;
	cgs_media_handle_t *shaderGrenadeTrailSmokePuff;
	cgs_media_handle_t *shaderBloodTrailPuff;
	cgs_media_handle_t *shaderBloodTrailLiquidPuff;
	cgs_media_handle_t *shaderBloodImpactPuff;
	cgs_media_handle_t *shaderTeleporterSmokePuff;
	cgs_media_handle_t *shaderBulletMark;
	cgs_media_handle_t *shaderExplosionMark;
	cgs_media_handle_t *shaderEnergyMark;
	cgs_media_handle_t *shaderLaser;
	cgs_media_handle_t *shaderNet;
	cgs_media_handle_t *shaderBackTile;
	cgs_media_handle_t *shaderSelect;
	cgs_media_handle_t *shaderChatBalloon;

	//wsw
	//----------------------------------------------

	cgs_media_handle_t *shaderAdditiveParticleShine;

	//wsw weapon sfx
	cgs_media_handle_t *shaderPlasmaMark;
	cgs_media_handle_t *shaderElectroBeam;
	cgs_media_handle_t *shaderInstaBeam;
	cgs_media_handle_t *shaderLaserGunBeam;
	cgs_media_handle_t *shaderElectroboltMark;
	cgs_media_handle_t *shaderInstagunMark;

	//wsw
	cgs_media_handle_t *shaderPlayerShadow;
	cgs_media_handle_t *shaderFlagFlare;

	// hud icons
	cgs_media_handle_t *shaderWeaponIcon[WEAP_TOTAL];
	cgs_media_handle_t *shaderNoGunWeaponIcon[WEAP_TOTAL];

	cgs_media_handle_t *shaderKeyIconOn[KEYICON_TOTAL];
	cgs_media_handle_t *shaderKeyIconOff[KEYICON_TOTAL];

	//no wsw

	cgs_media_handle_t *sbNums[11];
	cgs_media_handle_t *shaderCrosshair[NUM_CROSSHAIRS];

	// VSAY icons
	cgs_media_handle_t *shaderVSayIcon[VSAY_TOTAL];
} cgs_media_t;

typedef struct bonenode_s
{
	int bonenum;
	int numbonechildren;
	struct bonenode_s **bonechildren;
} bonenode_t;

typedef struct cg_tagmask_s
{
	char tagname[64];
	char bonename[64];
	int bonenum;
	struct cg_tagmask_s *next;
	vec3_t offset;
} cg_tagmask_t;

typedef struct
{
	char name[MAX_QPATH];
	int flags;
	int parent;
	struct bonenode_s *node;
} cgs_bone_t;

typedef struct cgs_skeleton_s
{
	struct model_s *model;

	int numBones;
	cgs_bone_t *bones;

	int numFrames;
	bonepose_t **bonePoses;

	struct cgs_skeleton_s *next;

	// store the tagmasks as part of the skeleton (they are only used by player models, tho)
	struct cg_tagmask_s *tagmasks;

	struct bonenode_s *bonetree;
} cgs_skeleton_t;

// bigmac
//#include "cg_boneposes.h"
// /bigmac

typedef struct cg_sexedSfx_s
{
	char *name;
	struct sfx_s *sfx;
	struct cg_sexedSfx_s *next;
} cg_sexedSfx_t;

typedef struct
{
	char name[MAX_QPATH];
	char cinfo[MAX_QPATH];
	int hand;
	byte_vec4_t color;
	int movestyle;

	struct	shader_s *icon;
} cg_clientInfo_t;

#define MAX_ANGLES_KICKS 3

typedef struct
{
	unsigned int timestamp;
	unsigned int kicktime;
	float v_roll, v_pitch;

} cg_kickangles_t;

#define MAX_COLORBLENDS 3

typedef struct
{
	unsigned int timestamp;
	unsigned int blendtime;
	float blend[4];

} cg_viewblend_t;

typedef struct
{
	char gametype[32];
	qboolean has_challengers;
	int maxteams;
	qboolean instagib;
	qboolean falldamage;
} cg_serverrules_t;

#define PREDICTED_STEP_TIME 150 // wsw : jal : stairs smoothing predicted time

// view types
enum
{
	VIEWDEF_CAMERA,
	VIEWDEF_PLAYERVIEW,

	VIEWDEF_MAXTYPES
};

typedef struct
{
	int type;
	int POVent;
	qboolean thirdperson;
	qboolean playerPrediction;
	qboolean drawWeapon;
	qboolean draw2D;
	refdef_t refdef;
	float fracDistFOV;
	vec3_t axis[3];
} cg_viewdef_t;

// bigmac
//#include "cg_democams.h"
// /bigmac

// bigmac
// inserted from cg_pmodels.h

enum
{

	VWEAP_STANDBY = 1,
	VWEAP_ATTACK_WEAK,
	VWEAP_ATTACK_STRONG,
	VWEAP_WEAPDOWN,
	VWEAP_WEAPONUP,

	VWEAP_MAXANIMS
};

typedef struct weaponinfo_s
{
	char name[MAX_QPATH];
	qboolean inuse;

	struct	model_s	*model[WEAPMODEL_PARTS]; //one weapon consists of several models

	int firstframe[VWEAP_MAXANIMS];         //animation script
	int lastframe[VWEAP_MAXANIMS];
	int loopingframes[VWEAP_MAXANIMS];
	unsigned int frametime[VWEAP_MAXANIMS];

	float rotationscale;

	orientation_t tag_projectionsource;
	orientation_t tag_handposition;
	byte_vec4_t outlineColor;

} weaponinfo_t;
// /bigmac

// this is not exactly "static" but still...
typedef struct
{
	unsigned int playerNum;

	// shaders
	struct shader_s	*shaderWhite;
	struct shader_s	*shaderLevelshot;
	struct shader_s	*shaderMiniMap;

	// fonts
	struct mufont_s	*fontSystemSmall;
	struct mufont_s	*fontSystemMedium;
	struct mufont_s	*fontSystemBig;

	cgs_media_t media;

	qboolean precacheDone;

	int vidWidth, vidHeight;

	qboolean demoPlaying;
	qboolean pure;
	qboolean tv, tvRequested;
	int gameProtocol;
	unsigned int baseServerTime;
	unsigned int snapFrameTime;

	char *demoAudioStream;

	//
	// locally derived information from server state
	//
	char configStrings[MAX_CONFIGSTRINGS][MAX_CONFIGSTRING_CHARS];

	char weaponModels[WEAP_TOTAL][MAX_QPATH];
	int numWeaponModels;
	weaponinfo_t *weaponInfos[WEAP_TOTAL];    // indexed list of weapon model infos
	orientation_t weaponItemTag;

	cg_clientInfo_t	clientInfo[MAX_CLIENTS];

	struct model_s *modelDraw[MAX_MODELS];
	struct model_s *inlineModelDraw[MAX_MODELS];

	struct pmodelinfo_s *pModelsIndex[MAX_MODELS];
	struct pmodelinfo_s *basePModelInfo; //fall back replacements
	struct skinfile_s *baseSkin;

	// force models
	struct pmodelinfo_s *teamModelInfo[GS_MAX_TEAMS];
	struct skinfile_s *teamCustomSkin[GS_MAX_TEAMS]; // user defined
	int teamColor[GS_MAX_TEAMS];

	struct sfx_s *soundPrecache[MAX_SOUNDS];
	struct shader_s	*imagePrecache[MAX_IMAGES];
	struct skinfile_s *skinPrecache[MAX_SKINFILES];

	cg_serverrules_t serverRules;

} cg_static_t;

typedef struct
{
	unsigned int time;
	float delay;

	unsigned int realTime;
	float frameTime;
	float realFrameTime;
	int frameCount;

	frame_t	frame, oldFrame;
	qboolean frameSequenceRunning;
	qboolean oldAreabits;
	qboolean portalInView;

	float predictedOrigins[CMD_BACKUP][3];              // for debug comparing against server

	float predictedStep;                // for stair up smoothing
	unsigned int predictedStepTime;

	unsigned int predictedTime;
	unsigned int topPredictTimeStamp;
	vec3_t predictedVelocity;
	vec3_t predictedOrigin;             // generated by CG_PredictMovement
	vec3_t predictedAngles;
	vec3_t predictionError;

	vec3_t autorotateAxis[3];

	float lerpfrac;                     // between oldframe and frame

	int effects;

	vec3_t lightingOrigin;

	qboolean showScoreboard;            // demos and multipov

	unsigned int multiviewPOVnum;       // for multipov chasing, takes effect on next snap

	int pointedNum;

	qboolean changing_weapon;           // for demos / chasecam to fix noammo sounds
	int latched_weapon;                 // weapon we think we are changing to
	int last_weapon;                    // weapon we had before current weapon

	//
	// all cyclic walking effects
	//
	float xyspeed;

	float oldBobTime;
	int bobCycle;                   // odd cycles are right foot going forward
	float bobFracSin;               // sin(bobfrac*M_PI)

	//
	// kick angles and color blend effects
	//

	cg_kickangles_t	kickangles[MAX_ANGLES_KICKS];
	cg_viewblend_t colorblends[MAX_COLORBLENDS];
	int damage_given;
	int damage_received;

	//
	// transient data from server
	//
	char checkname[MAX_QPATH];
	char loadingstring[MAX_QPATH];
	const char *matchmessage;
	char *teaminfo;
	char *motd;
	unsigned int motd_time;

	// statusbar program
	struct cg_layoutnode_s *statusBar;

	player_state_t interpolatedPlayerState;     // current in use, interpolated, player state
	cg_viewdef_t view;
} cg_state_t;

// bigmac (added from cg_scoreboard.c)

typedef enum
{
	SCBTAB_PLAYERFFA,
	SCBTAB_PLAYERRACE,
	SCBTAB_PLAYERDUEL,
	SCBTAB_PLAYERTDM,
	SCBTAB_PLAYERCTF,
	SCBTAB_SPECTATOR,
	SCBTAB_CHALLENGER,
	SCBTAB_CONNECTING,
	MAX_SCBTABS
} scbtype_t;

typedef struct
{
	scbtype_t type;
	int playernum;
	int score;
	int ping;
	int kills;
	int teamkills;
	int deaths;
	int suicides;
	int team;
	int ready;
	int dead;       // wsw : imp : added to parse
	int playerclass;        // wsw : jal : only used in ca classed gametype
	qboolean coach;
	qboolean waiting;

	unsigned int race_time;
} scb_playertab_t;

// /bigmac

// bigmac
/*
extern cg_static_t cgs;
extern cg_state_t cg;

#define ISVIEWERENTITY( entNum )  ( ( cg.view.POVent > 0 ) && ( cg.view.POVent == entNum ) && ( cg.view.type == VIEWDEF_PLAYERVIEW ) )

extern centity_t cg_entities[MAX_EDICTS];

//
// cg_ents.c
//
extern cvar_t *cg_gun;
extern cvar_t *cg_timedemo;
extern cvar_t *cg_damage_kick;

void CG_NewFrameSnap( frame_t *frame, frame_t *lerpframe );
struct cmodel_s *CG_CModelForEntity( int entNum );
void CG_AddEntities( void );
void CG_GlobalSound( const sound_t *sound );
void CG_GetEntitySpatilization( int entNum, vec3_t origin, vec3_t velocity );
void CG_FirePlayerStateEvents( void );  // wsw : jal
void CG_LerpEntities( void );           // wsw : jal
void CG_LerpGenericEnt( centity_t *cent );

void CG_SetOutlineColor( byte_vec4_t outlineColor, vec4_t itemcolor );
void CG_AddColoredOutLineEffect( entity_t *ent, int effects, qbyte r, qbyte g, qbyte b, qbyte a );
void CG_AddCentityOutLineEffect( centity_t *cent );

void CG_AddFlagModelOnTag( centity_t *cent, int flag_team, char *tagname );
void CG_StartKickAnglesEffect( vec3_t source, float knockback, float radius, int time );
void CG_StartColorBlendEffect( float r, float g, float b, float a, int time );

//
// cg_draw.c
//
int CG_HorizontalAlignForWidth( const int x, int align, int width );
int CG_VerticalAlignForHeight( const int y, int align, int height );

void CG_DrawHUDField( int x, int y, int align, float *color, int size, int width, int value );
void CG_DrawHUDModel( int x, int y, int align, int w, int h, struct model_s *model, struct shader_s *shader, float yawspeed );
void CG_DrawHUDRect( int x, int y, int align, int w, int h, int val, int maxval, vec4_t color, struct shader_s *shader );

//
// cg_media.c
//
void CG_RegisterMediaSounds( void );
void CG_RegisterMediaModels( void );
void CG_RegisterMediaShaders( void );
void CG_RegisterLevelShot( void );
void CG_RegisterLevelMinimap( void );

struct sfx_s *CG_MediaSfx( cgs_media_handle_t *mediasfx );
struct model_s *CG_MediaModel( cgs_media_handle_t *mediamodel );
struct shader_s *CG_MediaShader( cgs_media_handle_t *mediashader );

//
// cg_players.c
//
extern cvar_t *model;
extern cvar_t *skin;
extern cvar_t *hand;

void CG_LoadClientInfo( cg_clientInfo_t *ci, char *s, int client );
void CG_UpdateSexedSoundsRegistration( pmodelinfo_t *pmodelinfo );
void CG_SexedSound( int entnum, int entchannel, char *name, float fvol );
struct sfx_s *CG_RegisterSexedSound( int entnum, char *name );

//
// cg_predict.c
//
extern cvar_t *cg_predict;
extern cvar_t *cg_showMiss;

qboolean CG_PredictionActive( void );
void CG_PredictedEvent( int entNum, int ev, int parm );
void CG_PredictMovement( void );
void CG_CheckPredictionError( void );
void CG_BuildSolidList( void );
void CG_Trace( trace_t *t, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int ignore, int contentmask );
int CG_PointContents( vec3_t point );
void CG_Predict_TouchTriggers( pmove_t *pm );

//
// cg_screen.c
//
typedef struct
{
	int x, y, width, height;
} vrect_t;

extern vrect_t scr_vrect;

extern cvar_t *cg_scoreboardFont;
extern cvar_t *cg_showFPS;
extern cvar_t *cg_showAwards;
extern cvar_t *cg_showZoomEffect;
extern cvar_t *cg_showCaptureAreas;

void CG_ScreenInit( void );
void CG_ScreenShutdown( void );
void CG_Draw2D( void );
void CG_CalcVrect( void );
void CG_TileClear( void );
void CG_DrawLoading( void );
void CG_CenterPrint( char *str );
void CG_CenterPrintToUpper( char *str );

void CG_EscapeKey( void );
void CG_LoadStatusBar( void );

void CG_LoadingString( char *str );
void CG_LoadingFilename( char *str );

void CG_DrawCrosshair( int x, int y, int align );
void CG_DrawKeyState( int x, int y, int w, int h, int align, char *key );

int CG_ParseValue( const char **s );

void CG_DrawClock( int x, int y, int align, struct mufont_s *font, vec4_t color );
void CG_DrawPointed( int x, int y, int align, struct mufont_s *font, vec4_t color );
void CG_DrawHUDNumeric( int x, int y, int align, float *color, int charwidth, int charheight, int value );
void CG_DrawTeamInfo( int x, int y, int align, struct mufont_s *font, vec4_t color );
void CG_DrawNet( int x, int y, int w, int h, int align, vec4_t color );

//
// cg_hud.c
//
extern cvar_t *cg_showminimap;
extern cvar_t *cg_placebo;

void CG_SC_Obituary( void );
void CG_SC_Award( void );
void Cmd_CG_PrintHudHelp_f( void );
void CG_ExecuteLayoutProgram( struct cg_layoutnode_s *rootnode );

//
// cg_damage_indicator.c
//

enum
{
	DAMAGE_INDICATOR_DIR_LEFT,
	DAMAGE_INDICATOR_DIR_RIGHT,
	DAMAGE_INDICATOR_DIR_FRONT,
	DAMAGE_INDICATOR_DIR_BEHIND,
	DAMAGE_INDICATOR_DIR_MAX
};

extern int damage_indicator_dir_values[DAMAGE_INDICATOR_DIR_MAX];
extern int damage_indicator_health;

extern cvar_t *cg_damage_indicator_fade;
extern cvar_t *cg_damage_indicator_dir_fade;
extern cvar_t *cg_damage_indicator;
extern cvar_t *cg_damage_indicator_dir;

void CG_UpdateDamageIndicator( entity_state_t *ent );
void CG_UpdateDamageIndicatorValues( void );
int CG_GetDamageIndicatorDir( int type );
int CG_GetDamageIndicatorHealth( void );
float CG_GetDamageIndicatorDirAlpha( int type );
float CG_GetDamageIndicatorAlpha( void );
void CG_ResetDamageIndicator( void );

//
// cg_scoreboard
//
void CG_DrawScoreboard( void );
void CG_ToggleScores_f( void );
void CG_ScoresOn_f( void );
void CG_ScoresOff_f( void );
qboolean CG_ExecuteScoreboardTemplateLayout( char *s );
void SCR_UpdateScoreboardMessage( const char *string );
void SCR_UpdatePlayerStatsMessage( const char *string );

//
// cg_main.c
//
extern cvar_t *developer;
extern cvar_t *cg_showClamp;

// wsw
extern cvar_t *cg_showObituaries;
extern cvar_t *cg_volume_hitsound;    // hit sound volume
extern cvar_t *cg_autoaction_demo;
extern cvar_t *cg_autoaction_screenshot;
extern cvar_t *cg_autoaction_stats;
extern cvar_t *cg_autoaction_spectator;
extern cvar_t *cg_simpleItems; // simple items
extern cvar_t *cg_simpleItemsSize; // simple items
extern cvar_t *cg_volume_players; // players sound volume
extern cvar_t *cg_volume_effects; // world sound volume
extern cvar_t *cg_volume_announcer; // announcer sounds volume
extern cvar_t *cg_volume_voicechats; //vsays volume
extern cvar_t *cg_rocketTrail;
extern cvar_t *cg_rocketFireTrail;
extern cvar_t *cg_grenadeTrail;
extern cvar_t *cg_bloodTrail;
extern cvar_t *cg_showBloodTrail;
extern cvar_t *cg_rocketTrailAlpha;
extern cvar_t *cg_grenadeTrailAlpha;
extern cvar_t *cg_bloodTrailAlpha;

extern cvar_t *cg_cartoonRockets;
extern cvar_t *cg_dashEffect;
extern cvar_t *cg_fallEffect;

extern cvar_t *cg_explosionsRing;
extern cvar_t *cg_gibs;
extern cvar_t *cg_outlineModels;
extern cvar_t *cg_outlineWorld;
extern cvar_t *cg_outlineItemsBlack;
extern cvar_t *cg_outlinePlayersBlack;

extern cvar_t *cg_drawEntityBoxes;
extern cvar_t *cg_fov;
extern cvar_t *cg_oldMovement;
extern cvar_t *cg_zoomSens;
extern cvar_t *cg_particles;
extern cvar_t *cg_showhelp;
extern cvar_t *cg_predictLaserBeam;
extern cvar_t *cg_voiceChats;
#ifdef CGAMEGETLIGHTORIGIN
extern cvar_t *cg_shadows;
#endif
extern cvar_t *cg_showSelfShadow;
extern cvar_t *cg_showPlayerTrails;
extern cvar_t *cg_laserBeamSubdivisions;
extern cvar_t *cg_raceGhosts;
extern cvar_t *cg_raceGhostsAlpha;
extern cvar_t *cg_chatBeep;
extern cvar_t *cg_chatFilter;
extern cvar_t *cg_chatFilterTV;

//force models
extern cvar_t *cg_teamPLAYERSmodel;
extern cvar_t *cg_teamALPHAmodel;
extern cvar_t *cg_teamBETAmodel;
extern cvar_t *cg_teamGAMMAmodel;
extern cvar_t *cg_teamDELTAmodel;

extern cvar_t *cg_teamPLAYERSskin;
extern cvar_t *cg_teamALPHAskin;
extern cvar_t *cg_teamBETAskin;
extern cvar_t *cg_teamGAMMAskin;
extern cvar_t *cg_teamDELTAskin;

extern cvar_t *cg_teamPLAYERScolor;
extern cvar_t *cg_teamALPHAcolor;
extern cvar_t *cg_teamBETAcolor;
extern cvar_t *cg_teamGAMMAcolor;
extern cvar_t *cg_teamDELTAcolor;

extern cvar_t *cg_forceMyTeamAlpha;
extern cvar_t *cg_forceTeamPlayersTeamBeta;

extern cvar_t *cg_teamColoredBeams;

#define CG_Malloc( size ) trap_MemAlloc( size, __FILE__, __LINE__ )
#define CG_Free( data ) trap_MemFree( data, __FILE__, __LINE__ )

int CG_API( void );
void CG_Init( unsigned int playerNum, int vidWidth, int vidHeight, qboolean demoplaying, qboolean pure, qboolean tv, unsigned int baseServerTime, unsigned int snapFrameTime, int protocol );
void CG_Shutdown( void );
void CG_ValidateItemDef( int tag, char *name );
void CG_Printf( const char *format, ... );
void CG_Error( const char *format, ... );
void CG_SoftRestart( void );
char *_CG_CopyString( const char *in, const char *filename, int fileline );
#define CG_CopyString( in ) _CG_CopyString( in, __FILE__, __LINE__ )

void CG_RegisterCGameCommands( void );
void CG_UnregisterCGameCommands( void );
void CG_RegisterDemoCommands( void );
void CG_UnregisterDemoCommands( void );
void CG_UpdateServerSettings( void );

void CG_GetCaptureAreaInfo( int areano, int *stateout, int *teamout, int *cteamout, float *percentageout );

//
// cg_svcmds.c
//
void CG_Cmd_DemoGet_f( void );
void CG_ConfigString( int i, char *s );
void CG_GameCommand( char *command );

//
// cg_teams.c
//
void CG_RegisterTeamColor( int team );
void CG_RegisterForceModels( void );
void CG_SetSceneTeamColors( void );
pmodelinfo_t *CG_PModelForCentity( centity_t *cent );
struct skinfile_s *CG_SkinForCentity( centity_t *cent );
void CG_SetPlayerColor( centity_t *cent );
vec_t *CG_TeamColor( int team, vec4_t color );
vec_t *CG_TeamBeamColor( int team, int ownerent, vec4_t color );

//
// cg_view.c
//
extern cvar_t *cg_thirdPerson;
extern cvar_t *cg_thirdPersonAngle;
extern cvar_t *cg_thirdPersonRange;
float CG_SetSensitivityScale( const float sens );
void CG_RenderView( float frameTime, float realFrameTime, int realTime, unsigned int serverTime, float stereo_separation );
void CG_AddKickAngles( vec3_t viewangles );
void CG_ChasePrev( void );
void CG_ChaseNext( void );

//
// cg_lents.c
//
void CG_ClearLocalEntities( void );
void CG_AddLocalEntities( void );

void CG_AddLaser( vec3_t start, vec3_t end, float radius, int colors, struct shader_s *shader );
void CG_BulletExplosion( vec3_t origin, vec3_t dir );
void CG_BubbleTrail( vec3_t start, vec3_t end, int dist );
void CG_Explosion1( vec3_t pos );
void CG_Explosion2( vec3_t pos );
void CG_NewRocketTrail( centity_t *cent );
void CG_RocketFireTrail( centity_t *cent );
void CG_NewGrenadeTrail( centity_t *cent );
void CG_NewBloodTrail( centity_t *cent );
void CG_BloodDamageEffect( vec3_t origin, vec3_t dir, int damage );
void CG_NewElectroBeamPuff( centity_t *cent, vec3_t origin, vec3_t dir );
void CG_FlagTrail( vec3_t origin, vec3_t start, vec3_t end, float r, float g, float b );
void CG_GreenLaser( vec3_t start, vec3_t end );
void CG_SmallPileOfGibs( vec3_t origin, int count, vec3_t velocity );
void CG_EjectBrass( vec3_t origin, int count, struct model_s *model );
void CG_PlasmaExplosion( vec3_t pos, vec3_t dir, int fire_mode, float radius );
void CG_GrenadeExplosionMode( vec3_t pos, vec3_t dir, int fire_mode, float radius );
void CG_RocketExplosionMode( vec3_t pos, vec3_t dir, int fire_mode, float radius );
void CG_ElectroTrail2( vec3_t start, vec3_t end, qboolean show_impact, vec4_t color );
void CG_BoltExplosionMode( vec3_t pos, vec3_t dir, int fire_mode );
void CG_InstaExplosionMode( vec3_t pos, vec3_t dir, int fire_mode );
void CG_BladeImpact( vec3_t pos, vec3_t dir );
void CG_GunBladeBlastImpact( vec3_t pos, vec3_t dir, float radius );
void CG_PModel_SpawnTeleportEffect( centity_t *cent );

void CG_Dash( entity_state_t *state );
void CG_SpawnTracer( vec3_t origin, vec3_t dir, vec3_t dir_per1, vec3_t dir_per2 );
void CG_Explosion_Puff_2( vec3_t pos, vec3_t vel, int radius );
void CG_DustCircle( vec3_t pos, vec3_t dir, float radius, int count );
void CG_FallEffect( vec3_t pos, int owner );
void CG_WalljumpEffect( entity_state_t *state, vec3_t dir );

//
// cg_decals.c
//
extern cvar_t *cg_addDecals;

void CG_ClearDecals( void );
void CG_SpawnDecal( vec3_t origin, vec3_t dir, float orient, float radius,
                    float r, float g, float b, float a, float die, float fadetime, qboolean fadealpha, struct shader_s *shader );
void CG_AddDecals( void );

//
// cg_polys.c	-	wsw	: jal
//
extern cvar_t *cg_ebbeam_width;
extern cvar_t *cg_ebbeam_alpha;
extern cvar_t *cg_ebbeam_time;
extern cvar_t *cg_instabeam_width;
extern cvar_t *cg_instabeam_alpha;
extern cvar_t *cg_instabeam_time;

void CG_ClearPolys( void );
void CG_AddPolys( void );
void CG_QuickPolyBeam( vec3_t start, vec3_t end, int width, struct shader_s *shader );
void CG_LaserGunPolyBeam( vec3_t start, vec3_t end, vec4_t color );
void CG_ElectroPolyBeam( vec3_t start, vec3_t end, qboolean show_impact, vec4_t color );
void CG_InstaPolyBeam( vec3_t start, vec3_t end, qboolean show_impact, vec4_t color );
void CG_PLink( vec3_t start, vec3_t end, vec4_t color, int flags );

//
// cg_effects.c
//
void CG_ClearEffects( void );

void CG_AddLightToScene( vec3_t org, float intensity, float r, float g, float b, struct shader_s *shader );
void CG_AddDlights( void );
#ifdef CGAMEGETLIGHTORIGIN
void CG_AllocShadeBox( int entNum, const vec3_t origin, const vec3_t mins, const vec3_t maxs, struct shader_s *shader );
void CG_AddShadeBoxes( void );
#endif
void CG_RunLightStyles( void );
void CG_SetLightStyle( int i );
void CG_AddLightStyles( void );

void CG_AddParticles( void );
void CG_ParticleEffect( vec3_t org, vec3_t dir, float r, float g, float b, int count );
void CG_ParticleEffect2( vec3_t org, vec3_t dir, float r, float g, float b, int count );
void CG_BlasterTrail( vec3_t start, vec3_t end );
void CG_FlyEffect( centity_t *ent, vec3_t origin );
void CG_AddLinearTrail( centity_t *cent, float lifetime );
void CG_ElectroIonsTrail( vec3_t start, vec3_t end );

// wsw
void CG_ElectroWeakTrail( vec3_t start, vec3_t end, vec4_t color );
void CG_ImpactPuffParticles( vec3_t org, vec3_t dir, int count, float scale, float r, float g, float b, float a, struct shader_s *shader );

//
// cg_test.c - debug only
//
#ifndef WSW_RELEASE
void CG_DrawTestLine( vec3_t start, vec3_t end );
void CG_DrawTestBox( vec3_t origin, vec3_t mins, vec3_t maxs, vec3_t angles );
void CG_AddTest( void );
#endif

//
//	cg_vweap.c - client weapon
//
void CG_AddViewWeapon( void );
void CG_CalcViewWeapon( void );
void CG_WeapLast_f( void );
void CG_WeapPrev_f( void );
void CG_WeapNext_f( void );
void CG_Cmd_Use_f( void );
void CG_WeaponAutoswitch( int weapon );
void CG_NoAmmoWeaponChange( void );
void CG_CheckWeaponState( void );

//
// cg_events.c
//
extern cvar_t *cg_footSteps;
extern cvar_t *cg_damage_blend;
extern cvar_t *cg_pickup_flash;
extern cvar_t *cg_weaponAutoswitch;

void CG_EntityEvent( entity_state_t *ent, int ev, int parm, qboolean predicted );
void CG_ReleaseAnnouncerEvents( void );
void CG_ClearAnnouncerEvents( void );

//=================================================
*/