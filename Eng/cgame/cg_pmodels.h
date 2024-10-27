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
// cg_pmodels.h -- local definitions for pmodels and view weapon

//=============================================================================
//
//							SPLITMODELS
//
//=============================================================================

// bigmac
/*
extern cvar_t *cg_weaponFlashes;
extern cvar_t *cg_ejectBrass; //eject brass-debris
extern cvar_t *cg_gunx;
extern cvar_t *cg_guny;
extern cvar_t *cg_gunz;
extern cvar_t *cg_debugPlayerModels;
extern cvar_t *cg_debugWeaponModels;
extern cvar_t *cg_gunbob;
extern cvar_t *cg_gun_fov;
extern cvar_t *cg_handOffset;

enum
{

	VWEAP_STANDBY = 1,
	VWEAP_ATTACK_WEAK,
	VWEAP_ATTACK_STRONG,
	VWEAP_WEAPDOWN,
	VWEAP_WEAPONUP,

	VWEAP_MAXANIMS
};


//equivalent to pmodelinfo_t. Shared by different players, etc.
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

extern weaponinfo_t cg_pWeaponModelInfos[WEAP_TOTAL];
*/
// /bigmac

typedef struct
{
	unsigned int flashtime;
	unsigned int barreltime;
	unsigned int reloadedtime;
	vec3_t angles;                  //for barrel rotation
	float rotationSpeed;
} pweapon_t;

enum
{
	BASIC_CHANNEL,
	EVENT_CHANNEL,

	ANIMBUFFER_CHANNELS
};

typedef struct
{
	int newanim[PMODEL_PARTS];

} animationbuffer_t;

typedef struct
{
	int current[PMODEL_PARTS];          //running animation
	int currentChannel[PMODEL_PARTS];   //what channel it came from

	int frame[PMODEL_PARTS];
	int oldframe[PMODEL_PARTS];

	unsigned int prevframetime; //interpolation timing stuff
	unsigned int nextframetime;
	float lerpFrac;

	animationbuffer_t buffer[ANIMBUFFER_CHANNELS];

} animationinfo_t;

#define SKM_MAX_BONES 256

//pmodelinfo_t is the playermodel structure as originally readed
//Consider it static 'read-only', cause it is shared by different players
typedef struct pmodelinfo_s
{
	char *name;
	int sex;
	struct shader_s	*icon;

	struct	model_s	*model;
	struct cg_sexedSfx_s *sexedSfx;

	int numRotators[PMODEL_PARTS];
	int rotator[PMODEL_PARTS][16];
	int rootanims[PMODEL_PARTS];

	int firstframe[PMODEL_MAX_ANIMS];                   //animation script
	int lastframe[PMODEL_MAX_ANIMS];
	int loopingframes[PMODEL_MAX_ANIMS];
	unsigned int fps;

	struct pmodelinfo_s *next;
} pmodelinfo_t;

typedef struct
{
	//static data
	pmodelinfo_t *pmodelinfo;
	struct skinfile_s *skin;

	//dynamic
	animationinfo_t	anim;                       //animation state
	pweapon_t pweapon;                          //active weapon state

	vec3_t angles[PMODEL_PARTS];                //for rotations
	vec3_t oldangles[PMODEL_PARTS];             //for rotations

	//effects
	orientation_t projectionSource;     // for projectiles

} pmodel_t;

// bigmac
/*
extern pmodel_t	cg_entPModels[MAX_EDICTS];      //a pmodel handle for each cg_entity

//
// cg_pmodels.c
//

//utils
void CG_AddShellEffects( entity_t *ent, int effects );
void CG_AddColorShell( entity_t *ent, int renderfx );
qboolean CG_GrabTag( orientation_t *tag, entity_t *ent, char *tagname );
void CG_PlaceModelOnTag( entity_t *ent, entity_t *dest, orientation_t *tag );
void CG_PlaceRotatedModelOnTag( entity_t *ent, entity_t *dest, orientation_t *tag );
void CG_MoveToTag( vec3_t move_origin, vec3_t move_axis[3], vec3_t dest_origin, vec3_t dest_axis[3], vec3_t tag_origin, vec3_t tag_axis[3] );

//pmodels
void CG_PModelsInit( void );
void CG_ResetPModels( void );
void CG_RegisterBasePModel( void );
struct pmodelinfo_s *CG_RegisterPlayerModel( char *filename );
void CG_LoadClientPmodel( int cenum, char *model_name, char *skin_name );
void CG_AddPModel( centity_t *cent );
void CG_AddPModelAnimation( int entNum, int loweranim, int upperanim, int headanim, int channel );
void CG_PModel_StartShootEffect( int entNum );
void CG_ClearEventAnimations( int entNum );
qboolean CG_PModel_GetProjectionSource( int entnum, orientation_t *tag_result );
void CG_UpdatePlayerModelEnt( centity_t *cent );

//
// cg_wmodels.c
//
struct weaponinfo_s *CG_CreateWeaponZeroModel( char *cgs_name );
struct weaponinfo_s *CG_RegisterWeaponModel( char *cgs_name, int weaponTag );
void CG_AddWeaponOnTag( entity_t *ent, orientation_t *tag, int weapon, pweapon_t *pweapon, int effects, orientation_t *projectionSource );
struct weaponinfo_s *CG_GetWeaponInfo( int currentweapon );
void CG_ViewWeapon_StartShootEffect( int fireMode );
void CG_ViewWeapon_StartFallKickEff( int parms );
unsigned int CG_ViewWeapon_GetTotalReloadTime( firedef_t *firedef );


//=================================================
//				VIEW WEAPON
//=================================================

enum
{
	CAM_INEYES,
	CAM_THIRDPERSON,

	CAM_MODES
};

typedef struct
{

	int mode;
	unsigned int cmd_mode_delay;

} cg_chasecam_t;

extern cg_chasecam_t chaseCam;

typedef struct
{
	vec3_t origin;
	vec3_t axis[3];

	pweapon_t pweapon;              //weapon state
	int weapon;

	int currentAnim;
	int newAnim;
	int frame;
	int oldframe;

	unsigned int nextframetime;
	unsigned int prevframetime;
	float backlerp;

	unsigned int fallEff_Time;      //fallKickEffect
	int fallEff_rebTime;

	orientation_t projectionSource;
} viewweaponinfo_t;

extern viewweaponinfo_t	vweap;
*/
// /bigmac