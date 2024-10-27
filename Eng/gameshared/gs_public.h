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

#ifndef __GS_PUBLIC_H
#define __GS_PUBLIC_H

//	bigmac
//#include "gs_qrespath.h"
//	/bigmac

// shared callbacks

#define GS_MODULE_GAME	    1
#define GS_MODULE_CGAME	    2

extern int gs_module;

// bigmac
/*
void ( *GS_Printf )( const char *format, ... );
void ( *GS_Error )( const char *format, ... );
void *( *GS_Malloc )( size_t size );
void ( *GS_Free )( void *data );
void ( *GS_Trace )( trace_t *t, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int ignore, int contentmask );
int ( *GS_PointContents )( vec3_t point );
void ( *GS_PredictedEvent )( int entNum, int ev, int parm );
void ( *GS_PMoveTouchTriggers )( pmove_t *pm );
#ifndef WSW_RELEASE
int ( *GS_FS_FOpenFile )( const char *filename, int *filenum, int mode );
int ( *GS_FS_Read )( void *buffer, size_t len, int file );
int ( *GS_FS_Write )( const void *buffer, size_t len, int file );
void ( *GS_FS_FCloseFile )( int file );
#endif
*/
// /bigmac

//===============================================================
//		WARSOW player AAboxes sizes

extern vec3_t playerbox_stand_mins;
extern vec3_t playerbox_stand_maxs;
extern int playerbox_stand_viewheight;

extern vec3_t playerbox_crouch_mins;
extern vec3_t playerbox_crouch_maxs;
extern int playerbox_crouch_viewheight;

extern vec3_t playerbox_gib_mins;
extern vec3_t playerbox_gib_maxs;
extern int playerbox_gib_viewheight;

// item box
extern vec3_t item_box_mins;
extern vec3_t item_box_maxs;

//==================================================================

// box slide movement code (not used for player)
#define MAX_SLIDEMOVE_CLIP_PLANES   16

#define SLIDEMOVE_PLANEINTERACT_EPSILON	0.05
#define SLIDEMOVEFLAG_PLANE_TOUCHED 16
#define SLIDEMOVEFLAG_WALL_BLOCKED  8
#define SLIDEMOVEFLAG_TRAPPED	    4
#define SLIDEMOVEFLAG_BLOCKED	    2   // it was blocked at some point, doesn't mean it didn't slide along the blocking object
#define SLIDEMOVEFLAG_MOVED	    1

typedef struct
{
	vec3_t velocity;
	vec3_t origin;
	vec3_t mins, maxs;
	float remainingTime;

	vec3_t gravityDir;
	float slideBounce;
	int groundEntity;

	int passent, contentmask;

	int numClipPlanes;
	vec3_t clipPlaneNormals[MAX_SLIDEMOVE_CLIP_PLANES];

	int numtouch;
	int touchents[MAXTOUCH];
} move_t;

int GS_SlideMove( move_t *move );
void GS_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce );
void GS_SnapVelocity( vec3_t velocity );
qboolean GS_SnapPosition( vec3_t origin, vec3_t mins, vec3_t maxs, int passent, int contentmask );
qboolean GS_SnapInitialPosition( vec3_t origin, vec3_t mins, vec3_t maxs, int passent, int contentmask );


//==============================================================
//
//PLAYER MOVEMENT CODE
//
//Common between server and client so prediction matches
//
//==============================================================

#define STEPSIZE 18
enum {
	GS_DEFAULTBUNNY,
	GS_CLASSICBUNNY,

	GS_MAXBUNNIES
};

void Pmove( pmove_t *pmove );

//===============================================================

//==================
//SPLITMODELS
//==================

// The parts must be listed in draw order
enum
{
	LOWER = 0,
	UPPER,
	HEAD,

	PMODEL_PARTS
};

// -Torso DEATH frames and Legs DEATH frames must be the same.

// ANIMATIONS

enum
{
	ANIM_NONE = 0
	, BOTH_DEATH1       //Death animation
	, BOTH_DEAD1        //corpse on the ground
	, BOTH_DEATH2       //Death animation
	, BOTH_DEAD2        //corpse on the ground
	, BOTH_DEATH3       //Death animation
	, BOTH_DEAD3        //corpse on the ground
	, TORSO_TAUNT       //gesture
	, TORSO_ATTACK1     //Attack
	, TORSO_ATTACK2     //Attack2 (gunblade knifing)
	, TORSO_WEAP_DOWN   //put out current weapon
	, TORSO_WEAP_UP     //put in new weapon
	, TORSO_STAND       //Stand
	, TORSO_STAND2      //Stand2 (gaunlet)
	, LEGS_CRWALK       //Crouched Walk
	, LEGS_WALKFWD      //WalkFordward (few frames of this will show when moving by inertia)
	, LEGS_RUNFWD       //RunFordward
	, LEGS_RUNBACK      //RunBackward
	, LEGS_SWIMFWD      //Swim Fordward
	, LEGS_JUMP1        //Left leg Jump
	, LEGS_JUMP1ST      //Left leg land
	, LEGS_JUMP3        //Stand & Backwards jump.
	, LEGS_JUMP3ST      //Stand & Backwards land.
	, LEGS_IDLE         //Stand
	, LEGS_IDLECR       //Crouched Stand
	, LEGS_JUMP2        //Right leg Jump
	, LEGS_JUMP2ST      //Right leg land
	, LEGS_SWIM         //Stand & Backwards Swim
	, LEGS_WALKBACK     //WalkBackward
	, LEGS_WALKLEFT     //WalkLeft
	, LEGS_WALKRIGHT    //WalkRight
	, LEGS_RUNLEFT      //RunLeft
	, LEGS_RUNRIGHT     //RunRight
	, TORSO_RUN         //Run (also used for jump)
	, TORSO_SWIM        //Swim
	, TORSO_DROPHOLD    //Drop hold is only used at handgrenade activation.
	, TORSO_DROP        //Drop: true DROP animation
	, TORSO_PAIN1       //Pain1
	, TORSO_PAIN2       //Pain2
	, TORSO_PAIN3       //Pain3
	, LEGS_WALLJUMP     //wall-jump movement
	, LEGS_WALLJUMP_LEFT //wall-jump to left
	, LEGS_WALLJUMP_RIGHT //wall-jump to right
	, LEGS_WALLJUMP_BACK //wall-jump backwards
	, LEGS_DASH         //dash to left
	, LEGS_DASH_LEFT    //dash to left
	, LEGS_DASH_RIGHT   //dash to right
	, LEGS_DASH_BACK    //dash backwards

	, PMODEL_MAX_ANIMS
};

enum
{
	WEAPON,
	EXPANSION,
	BARREL,
	FLASH,
	HAND,
	HANDPOSITION,

	WEAPMODEL_PARTS
};

// gender stuff
enum
{
	GENDER_MALE,
	GENDER_FEMALE,
	GENDER_NEUTRAL
};

//===============================================================

// gs_items - shared items definitions

//==================
//	ITEM TAGS
//==================
// max weapons allowed by the net protocol are 128
typedef enum
{
	WEAP_NONE = 0,
	WEAP_GUNBLADE,
	WEAP_SHOCKWAVE,
	WEAP_RIOTGUN,
	WEAP_GRENADELAUNCHER,
	WEAP_ROCKETLAUNCHER,
	WEAP_PLASMAGUN,
	WEAP_LASERGUN,
	WEAP_ELECTROBOLT,
	WEAP_INSTAGUN,

	WEAP_TOTAL
} weapon_tag_t;

typedef enum
{
	AMMO_NONE = 0,
	AMMO_CELLS = WEAP_TOTAL,
	AMMO_WAVES,
	AMMO_SHELLS,
	AMMO_GRENADES,
	AMMO_ROCKETS,
	AMMO_PLASMA,
	AMMO_LASERS,
	AMMO_BOLTS,
	AMMO_INSTAS,

	AMMO_WEAK_GUNBLADE, //this is the blade
	AMMO_WEAK_WAVES,
	AMMO_WEAK_SHELLS,
	AMMO_WEAK_GRENADES,
	AMMO_WEAK_ROCKETS,
	AMMO_WEAK_PLASMA,
	AMMO_WEAK_LASERS,
	AMMO_WEAK_BOLTS,
	AMMO_WEAK_INSTAS,

	AMMO_TOTAL

} ammo_tag_t;

typedef enum
{
	ARMOR_NONE = 0,
	ARMOR_GA = AMMO_TOTAL,
	ARMOR_YA,
	ARMOR_RA,
	ARMOR_SHARD,

	ARMOR_TOTAL

} armor_tag_t;

typedef enum
{
	HEALTH_NONE = 0,
	HEALTH_SMALL = ARMOR_TOTAL,
	HEALTH_MEDIUM,
	HEALTH_LARGE,
	HEALTH_MEGA,

	HEALTH_TOTAL

} health_tag_t;

typedef enum
{
	POWERUP_NONE = 0,
	POWERUP_QUAD = HEALTH_TOTAL,
	POWERUP_SHELL,

	POWERUP_TOTAL

} powerup_tag_t;

typedef enum
{
	AMMO_PACK = POWERUP_TOTAL,
	FLAG_ALPHA,
	FLAG_BETA,
	FLAG_GAMMA,
	FLAG_DELTA,

	CAPTURE_AREA_INDICATOR_A,
	CAPTURE_AREA_INDICATOR_B,
	CAPTURE_AREA_INDICATOR_C,
	CAPTURE_AREA_INDICATOR_D,

	ITEMS_TOTAL
} otheritems_tag_t;

#define GS_MAX_ITEM_TAGS ITEMS_TOTAL

#define MAX_ITEM_MODELS 2

// gitem_t->flags
#define	ITFLAG_PICKABLE		1
#define	ITFLAG_USABLE		2
#define ITFLAG_DROPABLE		4
#define ITFLAG_STAY_COOP	8

// gitem_t->type
// define as bitflags values so they can be masked
typedef enum
{
	IT_WEAPON = 1
	, IT_AMMO = 2
	, IT_ARMOR = 4
	, IT_POWERUP = 8
	, IT_FLAG = 16
	, IT_HEALTH = 64
} itemtype_t;

typedef struct gitem_s
{
	//header
	char *classname;        // spawning name
	int tag;
	itemtype_t type;
	int flags;              // actions the item does in the game

	//media
	char *world_model[MAX_ITEM_MODELS];
	char *icon;
	char *simpleitem;       // Kurim : we use different images for representing simpleitems
	char *pickup_sound;
	int effects;
	int renderfx;


	char *pickup_name;      // for printing on pickup
	char *short_name;       // for printing on messages
	char *color;            // for printing on messages

	int quantity;           // for ammo how much, for weapons how much is used per shot
	// special
	int ammo_tag;           // uses this ammo, for weapons
	int weakammo_tag;

	void *info;             // miscelanea info goes pointed in here

	char *q3classname;

	// space separeted string of stuff to precache that's not mentioned above
	char *precache_models;
	char *precache_sounds;
	char *precache_images;
} gitem_t;

extern gitem_t itemdefs[];

gitem_t	*GS_FindItemByTag( int tag );
gitem_t	*GS_FindItemByClassname( char *classname );
gitem_t	*GS_FindItemByName( char *name );

//===============================================================

//===================
//	GAMETYPES
//===================

enum
{
	MATCH_STATE_NONE,
	MATCH_STATE_WARMUP,
	MATCH_STATE_COUNTDOWN,
	MATCH_STATE_PLAYTIME,
	MATCH_STATE_POSTMATCH,
	MATCH_STATE_WAITEXIT
};

enum
{
	GAMETYPE_DM,
	GAMETYPE_DUEL,
	GAMETYPE_TDM,
	GAMETYPE_CTF,
	GAMETYPE_RACE,
	GAMETYPE_CA,
#ifdef DUEL_ARENA
	GAMETYPE_DA,
#endif

	GAMETYPE_TOTAL
};

enum
{
	TEAM_SPECTATOR,
	TEAM_PLAYERS,
	TEAM_ALPHA,
	TEAM_BETA,
	TEAM_GAMMA,
	TEAM_DELTA,

	GS_MAX_TEAMS
};

#define DEFAULT_TEAMALPHA_COLOR			"255 65 35"
#define DEFAULT_TEAMBETA_COLOR			"150 62 255"
#define DEFAULT_TEAMGAMMA_COLOR			"0 255 0"
#define DEFAULT_TEAMDELTA_COLOR			"255 220 0"

// gs_gametypes.c
int GS_Gametype_FindByName( char *name );
int GS_Gametype_FindByShortName( char *name );
char *GS_Gametype_StatusBar( int gametype );
char *GS_Gametype_Name( int gametype );
char *GS_Gametype_ShortName( int gametype );
qboolean GS_Gametype_IsTeamBased( int gametype );
int GS_Gametype_SpawnableItemMask( int gametype );
int GS_Gametype_RespawnableItemMask( int gametype );
int GS_Gametype_DropableItemMask( int gametype );
int GS_GameType_MaxWallJumps( int gametype );

// teams
char *GS_TeamName( int team );
char *GS_TeamSkinName( int team );
int GS_Teams_TeamFromName( char *teamname );

//===============================================================

typedef enum
{
	MATCHMESSAGE_NONE
	, MATCHMESSAGE_CHALLENGERS_QUEUE
	, MATCHMESSAGE_ENTER_CHALLENGERS_QUEUE
	, MATCHMESSAGE_SPECTATOR_MODES
	, MATCHMESSAGE_GET_READY
} matchmessage_t;

//gs_misc.c
void GS_PrintfModule( int module, const char *format, ... );
void GS_Obituary( void *victim, int gender, void *attacker, int mod, char *message, char *message2 );
void GS_TouchPushTrigger( int entNum, pmove_state_t *pmove, entity_state_t *pusher );
int GS_WaterLevel( entity_state_t *state, vec3_t mins, vec3_t maxs );
void GS_BBoxForEntityState( entity_state_t *state, vec3_t mins, vec3_t maxs );
int GS_UpdateBaseAnims( entity_state_t *state, vec3_t velocity );
const char *GS_MatchMessageString( matchmessage_t mm );

//===============================================================

// award types
enum
{
	ACCURACY_AWARD = 1, // X consecutive hits with electrobolt
	DIRECTROCKET_AWARD, // X rockets fired and touching an enemy
	DIRECTGRENADE_AWARD, // X grenades fired and touching an enemy
	RL_MIDAIR_AWARD,    // 1 kill the enemy with a rocket while he's on the air
	GL_MIDAIR_AWARD,    // 1 kill the enemy with a grenade while he's on the air
	MULTIKILL_AWARD,    // killing X enemies with less than Y ms between frags
	SPREE_AWARD,        // killing X ennemies without dying
	COMBO_RL_TO_EB_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_RL_TO_LG_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_RL_TO_RG_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_RL_TO_GL_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_RL_TO_RL_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_GL_TO_EB_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_GL_TO_LG_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_GL_TO_RG_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_GL_TO_RL_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_GL_TO_GL_AWARD, // hitting an enemy in air with 2 different weapons
	COMBO_LB_TO_EB_AWARD, // hitting an enemy in air with 2 different weapons
	MH_CTRL_AWARD,      // duel only - same player picks up X time MH consecutively
	RA_CTRL_AWARD,      // duel only - same player picks up X time RA consecutively
	RACERECORD_AWARD,   // race only - current server record broken
	FLAG_CAPTURE_AWARD,      // ctf only
	FLAG_RECOVERY_AWARD,      // ctf only
	CAPTURE_AREA_AWARD, // itdm only - when a player picks captures an area
	ALL_CAPTURE_AREAS_AWARD, // itdm only - when a team has all capture areas
	FLAG_CARRIER_KILL_AWARD, // ctf only
	FLAG_CARRIER_DEFEND_AWARD, // ctf only
	FLAG_BASE_DEFEND_AWARD,  // ctf only
	TOTAL_AWARD
};

//===============================================================

#define DEFAULT_BULLET_SPREAD		300
#define CURVELASERBEAM_SUBDIVISIONS	5

#define NUM_CROSSHAIRS	13

//===============================================================

enum
{
	STAT_GAMETYPE = 0
	, STAT_INSTAGIB
	, STAT_HEALTH
	, STAT_AMMO
	, STAT_AMMO_ITEM
	, STAT_WEAK_AMMO
	, STAT_ARMOR
	, STAT_ARMOR_ITEM
	, STAT_SELECTED_ITEM
	, STAT_PICKUP_ITEM
	, STAT_WEAPON_ITEM
	, STAT_POWERUP_TIMES

	, STAT_LAYOUTS
	, STAT_FRAGS
	, STAT_TEAM
	, STAT_CHASING

	, STAT_POINTED_PLAYER
	, STAT_POINTED_TEAMPLAYER

	, STAT_TEAM_ALPHA_SCORE
	, STAT_TEAM_BETA_SCORE
	, STAT_TEAM_DELTA_SCORE
	, STAT_TEAM_GAMMA_SCORE

	// not gameplay specific as used in ctf
	, STAT_REDARMOR_TIMER
	, STAT_MEGAHEALTH_TIMER

	// different meanings in different gametypes
	, STAT_GAMETYPE_SPECIFIC1
	, STAT_GAMETYPE_SPECIFIC2
	, STAT_GAMETYPE_SPECIFIC3

	, STAT_REALTEAM

	, MAX_STATS = PS_MAX_STATS //32
};

#define STAT_RACE_TIME		STAT_GAMETYPE_SPECIFIC1
#define STAT_RACE_PLAYERBESTTIME    STAT_GAMETYPE_SPECIFIC2
#define STAT_RACE_MATCHBESTTIME	    STAT_GAMETYPE_SPECIFIC3


//#define STAT_CTF_FLAG_TIMER			STAT_GAMETYPE_SPECIFIC1
//#define STAT_CTF_FLAG_STATES		STAT_GAMETYPE_SPECIFIC2

// Modified by KoFFiE -> bitwise OR status with timer
// Currently only 2 teams are supported, 'alpha' and 'beta'
#define STAT_CTF_FLAG_ALPHA	    STAT_GAMETYPE_SPECIFIC1
#define STAT_CTF_FLAG_BETA	    STAT_GAMETYPE_SPECIFIC2
#define STAT_CTF_FLAG_BLEEP	    STAT_GAMETYPE_SPECIFIC3

// CTF unlock and capture timers defaults in seconds:
#define WSW_CTF_CAPTURE_TIME_DEFAULT		3.5 // default capture timer in seconds
#define WSW_CTF_UNLOCK_TIME_DEFAULT		2.0 // default unlock timer in seconds
#define WSW_CTF_FREEZE_TIME_DEFAULT		600 // default freeze in ms
#define WSW_CTF_TIMER_DEFAULT			0.0 // No timer by default

//#define STAT_CTF_FLAG_GAMMA			STAT_GAMETYPE_SPECIFIC3
//#define STAT_CTF_FLAG_DELTA			STAT_GAMETYPE_SPECIFIC4 -> Doesn't exist

// Send a timer for each team in a 16bit int:
// 1 bit to indicate if status-change was caused by current player
// 3 bit flag status
// 12 bit timer ( 0 -> 4095 = 0 to 409,5 secs should be enough? )
#define FLAG_STATE_SAFE			0x0000
#define FLAG_STATE_UNLOCKING		0x2000
#define FLAG_STATE_STOLEN		0x4000
#define FLAG_STATE_CAPTURING		0x6000
#define FLAG_STATE_DROPPED		0x8000
#define FLAG_STATE_STOLEN_TIMER		0xA000
#define FLAG_STATE_RFU1			0xC000
#define FLAG_STATE_RFU2			0xE000
#define FLAG_STATE_SELF_MASK		0x1000  // Indicates that the current player is causing the unlock/reset/...
#define FLAG_STATE_TIMERMASK		0x0FFF

//===================
// TEAMDEATHMATCH
//===================
// to make the code make sense, not easily changable
#define MAX_CAPTURE_AREAS 4

#define STAT_TDM_CAPTURE_AREA_STATUS      STAT_GAMETYPE_SPECIFIC1
#define STAT_TDM_CAPTURE_AREA_CAPTURING1  STAT_GAMETYPE_SPECIFIC2
#define STAT_TDM_CAPTURE_AREA_CAPTURING2  STAT_GAMETYPE_SPECIFIC3

//============
// STAT_TDM_CAPTURE_AREA_STATUS
// each capture area is arranged one after the other
// eg. area 1|area 2|area 3|area 4
//      0010 | 0110 | 0111 | 0101
//============
#define AREA_TOTAL_SIZE      4
#define AREA_TOTAL_BITMASK   15

// 2 bits for 3 different states (captured, capturing, free)
#define AREA_STATE_FREE      4   // |01|00
#define AREA_STATE_CAPTURED  8   // |10|00
//#define AREA_STATE_CAPTURING 12   // |11|00

#define AREA_STATE_SIZE      2
#define AREA_STATE_BITMASK   3

// 2 bits for 4 dfferent teams
#define AREA_TEAM_ALPHA      0   // 00|00|
#define AREA_TEAM_BETA       1   // 00|01|
#define AREA_TEAM_GAMMA      2   // 00|10|
#define AREA_TEAM_DELTA      3   // 00|11|

#define AREA_TEAM_SIZE       2
#define AREA_TEAM_BITMASK    3

//============
// STAT_TDM_CAPTURE_AREA_CAPTURING 1 and 2
// used to tell the client which team is trying to capture an area
// this is different to above in that above is when they have successfully
// captured the flag.
// this is spread over two stats because we want to maximise the size of
// the percentage captured that we can send.
// using two stats we can send every 2%, otherwise it would be a lot less
// each capture area will have 8 bytes
//============
#define AREA_CAPTURING_SIZE     8
#define AREA_CAPTURING_BITMASK  255

// 2 bits for 4 different teams
// already defined for STAT_TDM_CAPTURE_AREA_STATUS

// 6 bits for percentage
// a normal number rightshifted by 2
#define AREA_PERCENTAGE_SIZE    6
#define AREA_PERCENTAGE_BITMASK 63


//===================
// END TEAMDEATHMATCH
//===================

#ifdef __GNUC__
static const char *gs_keyicon_names[] __attribute__( ( unused ) ) =
{
#else
static const char *gs_keyicon_names[] = {
#endif
	"forward",
	"backward",
	"left",
	"right",
	"fire",
	"jump",
	"crouch",
	"special"
};

// STAT_LAYOUTS flag bits meanings
#define	STAT_LAYOUT_UNUSED1			0x00000001
#define	STAT_LAYOUT_UNUSED2			0x00000002
#define	STAT_LAYOUT_SCOREBOARD		0x00000004
#define	STAT_LAYOUT_TEAMTAB			0x00000008
#define	STAT_LAYOUT_CHALLENGER		0x00000010 // player is in challengers queue (used for ingame menu)
#define	STAT_LAYOUT_READY			0x00000020 // player is ready (used for ingame menu)
#define	STAT_LAYOUT_ZOOM			0x00000040
#define STAT_LAYOUT_SPECTEAMONLY	0x00000080

#define STAT_NOTSET					-9999 // used for stats that don't have meaningful value atm.

#define STAT_POWERUP_QUAD			1
#define STAT_POWERUP_WARSHELL		2


#define STAT_WEAPON_STATE_STATUS_CHANGING   ( 1<<7 )

//===============================================================

// dmflags->integer flags
#define	DF_NO_HEALTH	    0x00000001  // 1
#define	DF_NO_ITEMS	    0x00000002  // 2
#define	DF_WEAPONS_STAY	    0x00000004  // 4
//#define	DF_UNUSED		0x00000008	// 8
#define	DF_INSTANT_ITEMS    0x00000010  // 16
//#define	DF_SAME_LEVEL		0x00000020	// 32
//#define DF_UNUSED2		0x00000040	// 64
//#define DF_UNUSED3		0x00000080	// 128
//	#define DF_UNUSED4		0x00000100	// 256
//#define	DF_SPAWN_FARTHEST	0x00000200	// 512
//#define DF_FORCE_RESPAWN	0x00000400	// 1024
#define DF_NO_ARMOR	    0x00000800  // 2048
//#define DF_UNUSED5		0x00001000	// 4096
#define DF_INFINITE_AMMO    0x00002000  // 8192
#define DF_QUAD_DROP	    0x00004000  // 16384
//#define DF_UNUSED6		0x00008000	// 32768

//===============================================================

// wsw : pb : MOD cleanup

// means of death
#define MOD_UNKNOWN	    0

typedef enum
{

	//MOD_UNKNOWN,

	// Weapon damage :
	MOD_GUNBLADE_W = 36,
	MOD_GUNBLADE_S,
	MOD_RIOTGUN_W,
	MOD_RIOTGUN_S,
	MOD_GRENADE_W,
	MOD_GRENADE_S,
	MOD_ROCKET_W,
	MOD_ROCKET_S,
	MOD_PLASMA_W,
	MOD_PLASMA_S,
	MOD_ELECTROBOLT_W,
	MOD_ELECTROBOLT_S,
	MOD_INSTAGUN_W,
	MOD_INSTAGUN_S,
	MOD_LASERGUN_W,
	MOD_LASERGUN_S,
	MOD_GRENADE_SPLASH_W,
	MOD_GRENADE_SPLASH_S,
	MOD_ROCKET_SPLASH_W,
	MOD_ROCKET_SPLASH_S,
	MOD_PLASMA_SPLASH_W,
	MOD_PLASMA_SPLASH_S,

	// World damage
	MOD_WATER,
	MOD_SLIME,
	MOD_LAVA,
	MOD_CRUSH, // moving item blocked by player
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
	MOD_EXPLOSIVE,

	// probably not used
	MOD_BARREL,
	MOD_BOMB,
	MOD_EXIT, // except this one : player leaves
	MOD_SPLASH,
	MOD_TARGET_LASER,
	MOD_TRIGGER_HURT,
	MOD_HIT,
	MOD_TARGET_BLASTER,
	//MOD_LASER_TRAP,
	MOD_GRAPPLE,

} mod_damage_t;

//===============================================================

//
// events, event parms
//
#ifdef THIS_IS_DISABLED
enum
{
	FOOTSTEP_NORMAL,
	FOOTSTEP_BOOT,
	FOOTSTEP_FLESH,
	FOOTSTEP_MECH,
	FOOTSTEP_ENERGY,
	FOOTSTEP_METAL,
	FOOTSTEP_SPLASH,

	FOOTSTEP_TOTAL
};
#endif
enum
{
	PAIN_25,
	PAIN_50,
	PAIN_75,
	PAIN_100,
	PAIN_WARSHELL,

	PAIN_TOTAL
};

//fire modes as event parm
enum
{
	FIRE_MODE_WEAK,
	FIRE_MODE_STRONG,

	FIRE_MODES_COUNT
};

// vsay tokens list
enum
{
	VSAY_GENERIC,
	VSAY_NEEDHEALTH,
	VSAY_NEEDWEAPON,
	VSAY_NEEDARMOR,
	VSAY_AFFIRMATIVE,
	VSAY_NEGATIVE,
	VSAY_YES,
	VSAY_NO,
	VSAY_ONDEFENSE,
	VSAY_ONOFFENSE,
	VSAY_OOPS,
	VSAY_SORRY,
	VSAY_THANKS,
	VSAY_NOPROBLEM,
	VSAY_YEEHAA,
	VSAY_GOODGAME,
	VSAY_DEFEND,
	VSAY_ATTACK,
	VSAY_NEEDBACKUP,
	VSAY_BOOO,
	VSAY_NEEDDEFENSE,
	VSAY_NEEDOFFENSE,
	VSAY_NEEDHELP,
	VSAY_ROGER,
	VSAY_ARMORFREE,
	VSAY_AREASECURED,
	VSAY_SHUTUP,
	VSAY_BOOMSTICK,
	VSAY_GOTOPOWERUP,
	VSAY_GOTOQUAD,
	VSAY_OK,

	VSAY_TOTAL = 128
};

// entity_state_t->event values
#define PREDICTABLE_EVENTS_START 64
typedef enum
{
	EV_NONE,
	EV_ITEM_RESPAWN,
	EV_PAIN,
	EV_DIE,
	EV_GIB,
	EV_MUZZLEFLASH,

	EV_PLAYER_TELEPORT_IN,
	EV_PLAYER_TELEPORT_OUT,

	EV_WEAPONDOWN,

	EV_GESTURE, //splitmodels events
	EV_DROP,
	EV_WEAPONUP,
	EV_SPOG, //debrisbounce

	EV_BLOOD2,
	EV_BLOOD_SAVED,

	//wsw weapon events
	EV_FIRE_BLADE, // unused
	EV_BLADE_IMPACT,
	EV_GUNBLADEBLAST_IMPACT,
	EV_FIRE_BULLET,
	EV_FIRE_RIOTGUN,
	EV_GRENADE_BOUNCE,
	EV_GRENADE_EXPLOSION,
	EV_ROCKET_EXPLOSION,
	EV_PLASMA_EXPLOSION,
	EV_ELECTROTRAIL,
	EV_BOLT_EXPLOSION,
	EV_INSTATRAIL,
	EV_INSTA_EXPLOSION,

	// nine spots reserved for new weapons sfx, so
	// the events below don't change their numbers easily
	EV_FREE1,
	EV_FREE2,
	EV_FREE3,
	EV_FREE4,
	EV_FREE5,
	EV_FREE6,
	EV_FREE7,

	EV_EXPLOSION1,
	EV_EXPLOSION2,

	EV_BLOOD,
	EV_BLASTER,
	EV_SPARKS,
	EV_BULLET_SPARKS,

	EV_SEXEDSOUND,

	EV_VSAY,

	EV_LASER_SPARKS,

	EV_FIRE_SHOTGUN,
	EV_FREE11,
	EV_PNODE,
	EV_GREEN_LASER,

	// func movers
	EV_PLAT_HIT_TOP,
	EV_PLAT_HIT_BOTTOM,
	EV_PLAT_START_MOVING,

	EV_DOOR_HIT_TOP,
	EV_DOOR_HIT_BOTTOM,
	EV_DOOR_START_MOVING,

	EV_BUTTON_FIRE,

	EV_TRAIN_STOP,
	EV_TRAIN_START,

	EV_DASH = PREDICTABLE_EVENTS_START,
	EV_WALLJUMP,
	EV_DOUBLEJUMP,
	EV_JUMP,
	EV_JUMP_PAD,
	EV_FALL,

	MAX_EVENTS = 128
} entity_event_t;

typedef enum
{
	PSEV_NONE = 0,
	PSEV_HIT,
	PSEV_PICKUP,
	PSEV_DAMAGE_BLEND,
	PSEV_INDEXEDSOUND,
	PSEV_NOAMMO,
	PSEV_ANNOUNCER,
	PSEV_ANNOUNCER_QUEUED,

	PSEV_MAX_EVENTS = 0xFF
} playerstate_event_t;
//===============================================================

// entity_state_t->type values
enum
{
	ET_GENERIC,
	ET_PLAYER,
	ET_CORPSE,
	ET_BEAM,
	ET_PORTALSURFACE,
	ET_EVENT,
	ET_PUSH_TRIGGER,

	ET_GIB,         // leave a trail
	ET_BLASTER,     // redlight + trail
	ET_ELECTRO_WEAK,
	ET_ROCKET,      // redlight + trail
	ET_GRENADE,
	ET_PLASMA,
	ET_SHOCKWAVE,

	ET_ITEM,        // for simple items
	ET_LASERBEAM,   // for continuous beams
	ET_CURVELASERBEAM, // for curved beams
	ET_FLAG_BASE,

	ET_CAPTURE_AREA_A, // capture area a
	ET_CAPTURE_AREA_B, // capture area b
	ET_CAPTURE_AREA_C, // capture area c
	ET_CAPTURE_AREA_D, // capture area d

	ET_TOTAL_TYPES, // current count
	MAX_ENTITY_TYPES = 128
};

//===============================================================
// gs_weapons.c

typedef enum
{
	WEAPON_READY,
	WEAPON_ACTIVATING,
	WEAPON_DROPPING,
	WEAPON_FIRING,
	WEAPON_RELOADING,
	WEAPON_COOLDOWN
} weapon_status_t;

#define WEAPON_ACTION_STATE_CHANGE	    ( 1<<0 )
#define WEAPON_ACTION_WEAPON_CHANGE	    ( 1<<1 )
#define WEAPON_ACTION_FIRE		    ( 1<<2 )

typedef struct
{
	weapon_status_t	status;
	qboolean changing;
	int nexttime;
} weapon_state_t;

typedef struct firedef_s
{
	//ammo def
	int weapon_id;
	int fire_mode;
	int ammo_id;
	int usage_count;
	int projectile_count;

	// timings
	unsigned int weaponup_time;
	unsigned int weapondown_time;
	unsigned int reload_time;
	unsigned int cooldown_time;
	unsigned int timeout;

	// damages
	int damage;
	float selfdamage;
	int knockback;
	int splash_radius;
	int splash_min_damage;

	// projectile def
	float speed;
	int spread;

	// ammo amounts
	int ammo_pickup;
	int ammo_max;

} firedef_t;

typedef struct weapon_info_s
{
	char *name;
	int weapon_id;

	firedef_t *firedef;
	firedef_t *firedef_weak;

} weapon_info_t;

extern weapon_info_t gs_weaponInfos[WEAP_TOTAL];

int GS_Weaponstate_Run( weapon_state_t *state, unsigned int msec, firedef_t *firedef, qboolean pressing_fire );

firedef_t *GS_FiredefForAmmo( int tag );
weapon_info_t *GS_FiredefForWeapon( int tag );
void GS_InitWeapons( void );

//===============================================================
// gs_weapondefs.c

extern firedef_t ammoFireDefs[];

//===============================================================
// gs_capture_areas.c

extern vec4_t TDM_FreeCAColour;
extern char *capture_indicators[];
extern char *capture_indicators_simple[];
extern int   capture_indicators_type[];
extern char *capture_area_icons[];

#define TDM_LetterToIndex(x) ( tolower(x) - 'a' )

#endif // __GS_PUBLIC_H
