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

// cg_public.h -- client game dll information visible to engine

#define	CGAME_API_VERSION   44

//
// structs and variables shared with the main engine
//

typedef struct
{
	qboolean all;
	qboolean targets[MAX_CLIENTS];
	size_t commandOffset;               // offset of the data in gamecommandsData
} gcommand_t;

#define	MAX_PARSE_ENTITIES	1024
#define	MAX_PARSE_GAMECOMMANDS	64
typedef struct
{
	qboolean valid;             // cleared if delta parsing was invalid
	int serverFrame;
	unsigned int serverTime;    // time in the server when frame was created
	unsigned int ucmdExecuted;
	qboolean delta;
	qboolean allentities;
	qboolean multipov;
	int deltaFrameNum;
	qbyte areabits[MAX_CM_AREAS/8];             // portalarea visibility bits
	int numplayers;
	player_state_t playerState;
	player_state_t playerStates[MAX_CLIENTS];
	int numEntities;
	entity_state_t parsedEntities[MAX_PARSE_ENTITIES];
	match_state_t match;
	int numgamecommands;
	gcommand_t gamecommands[MAX_PARSE_GAMECOMMANDS];
	char gamecommandsData[MAX_STRING_CHARS * ( MAX_PARSE_GAMECOMMANDS / 4 )];
	size_t gamecommandsDataHead;
	int numsounds;
	sound_t	sounds[MAX_PARSE_SOUNDS];
} frame_t;

//===============================================================

//
// functions provided by the main engine
//
typedef struct
{
	// drops to console a client game error
	void ( *Error )( char *msg );

	// console messages
	void ( *Print )( char *msg );

	// dynvars
	dynvar_t *( *Dynvar_Create )( const char *name, qboolean console, dynvar_getter_f getter, dynvar_setter_f setter );
	void ( *Dynvar_Destroy )( dynvar_t *dynvar );
	dynvar_t *( *Dynvar_Lookup )( const char *name );
	const char *( *Dynvar_GetName )( dynvar_t *dynvar );
	dynvar_get_status_t ( *Dynvar_GetValue )( dynvar_t *dynvar, void **value );
	dynvar_set_status_t ( *Dynvar_SetValue )( dynvar_t *dynvar, void *value );
	void ( *Dynvar_AddListener )( dynvar_t *dynvar, dynvar_listener_f listener );
	void ( *Dynvar_RemoveListener )( dynvar_t *dynvar, dynvar_listener_f listener );

	// console variable interaction
	cvar_t *( *Cvar_Get )( const char *name, const char *value, int flags );
	cvar_t *( *Cvar_Set )( const char *name, const char *value );
	void ( *Cvar_SetValue )( const char *name, float value );
	cvar_t *( *Cvar_ForceSet )( const char *name, const char *value );      // will return 0 0 if not found
	float ( *Cvar_Value )( const char *name );
	const char *( *Cvar_String )( const char *name );

	void ( *Cmd_TokenizeString )( const char *text );
	int ( *Cmd_Argc )( void );
	char *( *Cmd_Argv )( int arg );
	char *( *Cmd_Args )( void );        // concatenation of all argv >= 1

	void ( *Cmd_AddCommand )( const char *name, void ( *cmd )( void ) );
	void ( *Cmd_RemoveCommand )( const char *cmd_name );
	void ( *Cmd_ExecuteText )( int exec_when, const char *text );
	void ( *Cmd_Execute )( void );

	// files will be memory mapped read only
	// the returned buffer may be part of a larger pak file,
	// or a discrete file from anywhere in the quake search path
	// a -1 return means the file does not exist
	// NULL can be passed for buf to just determine existance
	int ( *FS_FOpenFile )( const char *filename, int *filenum, int mode );
	int ( *FS_Read )( void *buffer, size_t len, int file );
	int ( *FS_Write )( const void *buffer, size_t len, int file );
	int ( *FS_Print )( int file, const char *msg );
	int ( *FS_Tell )( int file );
	int ( *FS_Seek )( int file, int offset, int whence );
	int ( *FS_Eof )( int file );
	int ( *FS_Flush )( int file );
	void ( *FS_FCloseFile )( int file );
	qboolean ( *FS_RemoveFile )( const char *filename );
	int ( *FS_GetFileList )( const char *dir, const char *extension, char *buf, size_t bufsize, int start, int end );
	const char *( *FS_FirstExtension )( const char *filename, const char *extensions[], int num_extensions );
	qboolean ( *FS_IsPureFile )( const char *filename );

	// key bindings
	const char *( *Key_GetBindingBuf )( int binding );
	const char *( *Key_KeynumToString )( int keynum );

	void ( *GetConfigString )( int i, char *str, int size );
	unsigned int ( *Milliseconds )( void );
	qboolean ( *DownloadRequest )( const char *filename, qboolean requestpak );

	void ( *NET_GetUserCmd )( int frame, usercmd_t *cmd );
	int ( *NET_GetCurrentUserCmdNum )( void );
	void ( *NET_GetCurrentState )( int *incomingAcknowledged, int *outgoingSequence, int *outgoingSent );

	// refresh system
	void ( *R_UpdateScreen )( void );
	int ( *R_GetClippedFragments )( const vec3_t origin, float radius, vec3_t axis[3], int maxfverts, vec3_t *fverts, int maxfragments, fragment_t *fragments );
	void ( *R_ClearScene )( void );
	void ( *R_AddEntityToScene )( entity_t *ent );
	void ( *R_AddLightToScene )( const vec3_t org, float intensity, float r, float g, float b, const struct shader_s *shader );
	void ( *R_AddPolyToScene )( const poly_t *poly );
	void ( *R_AddLightStyleToScene )( int style, float r, float g, float b );
	void ( *R_RenderScene )( refdef_t *fd );
	const char *( *R_SpeedsMessage )( char *out, size_t size );
	void ( *R_RegisterWorldModel )( const char *name );
	void ( *R_ModelBounds )( const struct model_s *mod, vec3_t mins, vec3_t maxs );
	struct model_s *( *R_RegisterModel )( const char *name );
	struct shader_s *( *R_RegisterPic )( const char *name );
	struct shader_s *( *R_RegisterSkin )( const char *name );
	struct skinfile_s *( *R_RegisterSkinFile )( const char *name );
	qboolean ( *R_LerpTag )( orientation_t *orient, const struct model_s *mod, int oldframe, int frame, float lerpfrac, const char *name );
	void ( *R_SetCustomColor )( int num, int r, int g, int b );
	void ( *R_LightForOrigin )( const vec3_t origin, vec3_t dir, vec4_t ambient, vec4_t diffuse, float radius );
	void ( *R_DrawStretchPic )( int x, int y, int w, int h, float s1, float t1, float s2, float t2, const vec4_t color, const struct shader_s *shader );
	void ( *R_TransformVectorToScreen )( const refdef_t *rd, const vec3_t in, vec2_t out );
	int ( *R_SkeletalGetNumBones )( const struct model_s *mod, int *numFrames );
	int ( *R_SkeletalGetBoneInfo )( const struct model_s *mod, int bone, char *name, size_t name_size, int *flags );
	void ( *R_SkeletalGetBonePose )( const struct model_s *mod, int bone, int frame, bonepose_t *bonepose );

	// collision detection
	int ( *CM_NumInlineModels )( void );
	struct cmodel_s	*( *CM_InlineModel )( int num );
	struct cmodel_s	*( *CM_ModelForBBox )( vec3_t mins, vec3_t maxs );
	void ( *CM_BoxTrace )( trace_t *tr, vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, struct cmodel_s *cmodel, int brushmask );
	void ( *CM_TransformedBoxTrace )( trace_t *tr, vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, struct cmodel_s *cmodel, int brushmask, vec3_t origin, vec3_t angles );
	int ( *CM_PointContents )( vec3_t p, struct cmodel_s *cmodel );
	int ( *CM_TransformedPointContents )( vec3_t p, struct cmodel_s *cmodel, vec3_t origin, vec3_t angles );
	void ( *CM_InlineModelBounds )( struct cmodel_s *cmodel, vec3_t mins, vec3_t maxs );

	// sound system
	struct sfx_s *( *S_RegisterSound )( const char *name );
	void ( *S_StartFixedSound )( struct sfx_s *sfx, const vec3_t origin, int entchannel, float fvol, float attenuation );
	void ( *S_StartRelativeSound )( struct sfx_s *sfx, int entnum, int entchannel, float fvol, float attenuation );
	void ( *S_StartGlobalSound )( struct sfx_s *sfx, int entchannel, float fvol );
	void ( *S_Update )( const vec3_t origin, const vec3_t velocity, const vec3_t v_forward, const vec3_t v_right, const vec3_t v_up );
	void ( *S_AddLoopSound )( struct sfx_s *sfx, int entnum, float fvol, float attenuation );
	void ( *S_StartBackgroundTrack )( const char *intro, const char *loop );
	void ( *S_StopBackgroundTrack )( void );

	// fonts
	struct mufont_s *( *SCR_RegisterFont )( const char *name );
	void ( *SCR_DrawString )( int x, int y, int align, const char *str, struct mufont_s *font, vec4_t color );
	int ( *SCR_DrawStringWidth )( int x, int y, int align, const char *str, int maxwidth, struct mufont_s *font, vec4_t color );
	void ( *SCR_DrawClampString )( int x, int y, const char *str, int xmin, int ymin, int xmax, int ymax, struct mufont_s *font, vec4_t color );
	size_t ( *SCR_strHeight )( struct mufont_s *font );
	size_t ( *SCR_strWidth )( const char *str, struct mufont_s *font, int maxlen );
	size_t ( *SCR_StrlenForWidth )( const char *str, struct mufont_s *font, size_t maxwidth );

	// managed memory allocation
	void *( *Mem_Alloc )( size_t size, const char *filename, int fileline );
	void ( *Mem_Free )( void *data, const char *filename, int fileline );
} cgame_import_t;

//
// functions exported by the client game subsystem
//
typedef struct
{
	// if API is different, the dll cannot be used
	int ( *API )( void );

	// the init function will be called at each restart
	void ( *Init )( unsigned int playerNum, int vidWidth, int vidHeight, qboolean demoplaying, qboolean pure, qboolean tv, unsigned int baseServerTime, unsigned int snapFrameTime, int protocol );

	void ( *Shutdown )( void );

	void ( *ConfigString )( int number, char *value );

	void ( *EscapeKey )( void );

	void ( *GetEntitySpatilization )( int entNum, vec3_t origin, vec3_t velocity );

	float ( *SetSensitivityScale )( const float sens );

	void ( *Trace )( trace_t *tr, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passent, int contentmask );

	void ( *RenderView )( float frameTime, float realFrameTime, int realTime, unsigned int serverTime, float stereo_separation );

	void ( *NewFrameSnapshot )( frame_t *newSnapshot, frame_t *currentSnapshot );
} cgame_export_t;
