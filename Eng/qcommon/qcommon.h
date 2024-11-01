/*
   Copyright (C) 1997-2001 Id Software, Inc.

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

// qcommon.h -- definitions common between client and server, but not game.dll

#ifndef __QCOMMON_H
#define __QCOMMON_H

#include "../game/q_shared.h"
#include "qfiles.h"

// bigmac
//#include "cmodel.h"
//#include "version.h"
// /bigmac

//#define	PARANOID			// speed sapping error checking

#define	DEFAULT_BASEGAME    "basewsw"

//============================================================================

typedef struct
{
	qbyte *data;
	size_t maxsize;
	size_t cursize;
	size_t readcount;
	qboolean compressed;
} msg_t;

// bigmac
/*
// msg.c
void MSG_Init( msg_t *buf, qbyte *data, size_t length );
void MSG_Clear( msg_t *buf );
void *MSG_GetSpace( msg_t *buf, size_t length );
void MSG_WriteData( msg_t *msg, const void *data, size_t length );
void MSG_CopyData( msg_t *buf, const void *data, size_t length );
*/
// /bigmac

//============================================================================

struct usercmd_s;
struct entity_state_s;

// bigmac
/*
void MSG_WriteChar( msg_t *sb, int c );
void MSG_WriteByte( msg_t *sb, int c );
void MSG_WriteShort( msg_t *sb, int c );
void MSG_WriteInt3( msg_t *sb, int c );
void MSG_WriteLong( msg_t *sb, int c );
void MSG_WriteFloat( msg_t *sb, float f );
void MSG_WriteString( msg_t *sb, const char *s );
#define MSG_WriteCoord( sb, f ) ( MSG_WriteInt3( ( sb ), Q_rint( ( f*PM_VECTOR_SNAP ) ) ) )
#define MSG_WritePos( sb, pos ) ( MSG_WriteCoord( ( sb ), ( pos )[0] ), MSG_WriteCoord( sb, ( pos )[1] ), MSG_WriteCoord( sb, ( pos )[2] ) )
#define MSG_WriteAngle( sb, f ) ( MSG_WriteByte( ( sb ), ANGLE2BYTE( ( f ) ) ) )
#define MSG_WriteAngle16( sb, f ) ( MSG_WriteShort( ( sb ), ANGLE2SHORT( ( f ) ) ) )
void MSG_WriteDeltaUsercmd( msg_t *sb, struct usercmd_s *from, struct usercmd_s *cmd, unsigned int baseTime );
void MSG_WriteDeltaEntity( struct entity_state_s *from, struct entity_state_s *to, msg_t *msg, qboolean force, qboolean newentity );
void MSG_WriteDir( msg_t *sb, vec3_t vector );



void MSG_BeginReading( msg_t *sb );

// returns -1 if no more characters are available
int MSG_ReadChar( msg_t *msg );
int MSG_ReadByte( msg_t *msg );
int MSG_ReadShort( msg_t *sb );
int MSG_ReadInt3( msg_t *sb );
int MSG_ReadLong( msg_t *sb );
float MSG_ReadFloat( msg_t *sb );
char *MSG_ReadString( msg_t *sb );
char *MSG_ReadStringLine( msg_t *sb );
#define MSG_ReadCoord( sb ) ( (float)MSG_ReadInt3( ( sb ) )*( 1.0/PM_VECTOR_SNAP ) )
#define MSG_ReadPos( sb, pos ) ( ( pos )[0] = MSG_ReadCoord( ( sb ) ), ( pos )[1] = MSG_ReadCoord( ( sb ) ), ( pos )[2] = MSG_ReadCoord( ( sb ) ) )
#define MSG_ReadAngle( sb ) ( BYTE2ANGLE( MSG_ReadByte( ( sb ) ) ) )
#define MSG_ReadAngle16( sb ) ( SHORT2ANGLE( MSG_ReadShort( ( sb ) ) ) )
void MSG_ReadDeltaUsercmd( msg_t *sb, struct usercmd_s *from, struct usercmd_s *cmd );

void MSG_ReadDir( msg_t *sb, vec3_t vector );
void MSG_ReadData( msg_t *sb, void *buffer, size_t length );
int MSG_SkipData( msg_t *sb, size_t length );

//============================================================================

int COM_Argc( void );
char *COM_Argv( int arg );  // range and null checked
void COM_ClearArgv( int arg );
int COM_CheckParm( char *parm );
void COM_AddParm( char *parm );

void COM_Init( void );
void COM_InitArgv( int argc, char **argv );

char *TempCopyString( const char *in );

// some hax, because we want to save the file and line where the copy was called
// from, not the file and line from ZoneCopyString function
char *_ZoneCopyString( const char *in, const char *filename, int fileline );
#define ZoneCopyString( in ) _ZoneCopyString( in, __FILE__, __LINE__ )

int Com_GlobMatch( const char *pattern, const char *text, const qboolean casecmp );

void Info_Print( char *s );

//============================================================================

// crc.h 
void CRC_Init( unsigned short *crcvalue );
void CRC_ProcessByte( unsigned short *crcvalue, qbyte data );
unsigned short CRC_Value( unsigned short crcvalue );
unsigned short CRC_Block( qbyte *start, int count );

// patch.h 
void Patch_GetFlatness( float maxflat, const float *points, int comp, const int *patch_cp, int *flat );
void Patch_Evaluate( const vec_t *p, int *numcp, const int *tess, vec_t *dest, int comp );
*/
// /bigmac

/*
   ==============================================================

   BSP FORMATS

   ==============================================================
 */

typedef struct
{
        char *header;
        int version;
        int lightmapWidth;
        int lightmapHeight;
        int flags;
} bspFormatDesc_t;

#define BSP_NONE    0
#define BSP_RAVEN   1

extern int numBspFormats;
extern bspFormatDesc_t bspFormats[];

/*
   ==============================================================

   PROTOCOL

   ==============================================================
 */

// protocol.h -- communications protocols

//=========================================

#define	PORT_MASTER	    27950
#define	PORT_SERVER	    44400
#define	NUM_BROADCAST_PORTS 5

//=========================================

#define	UPDATE_BACKUP	32  // copies of entity_state_t to keep buffered
                            // must be power of two

#define	UPDATE_MASK	( UPDATE_BACKUP-1 )

//==================
// the svc_strings[] array in cl_parse.c should mirror this
//==================

//
// server to client
//
enum svc_ops_e
{
        svc_bad,

        // the rest are private to the client and server
        svc_nop,
        svc_servercmd,          // [string] string
        svc_sound,              // <see code>
        svc_serverdata,         // [int] protocol ...
        svc_spawnbaseline,
        svc_download,           // [short] size [size bytes]
        svc_playerinfo,         // variable
        svc_packetentities,     // [...]
        svc_gamecommands,
        svc_match,
        svc_clcack,
        svc_servercs, //tmp jalfixme : send reliable commands as unreliable
        svc_frame
};

//==============================================

//
// client to server
//
enum clc_ops_e
{
        clc_bad,
        clc_nop,
        clc_move,           // [[usercmd_t]
        clc_svcack,
        clc_clientcommand       // [string] message
};

//==============================================

// serverdata flags
#define SV_BITFLAGS_PURE		( 1<<0 )
#define SV_BITFLAGS_RELIABLE		( 1<<1 )
#define SV_BITFLAGS_TVSERVER		( 1<<2 )

// framesnap flags
#define FRAMESNAP_FLAG_DELTA		( 1<<0 )
#define FRAMESNAP_FLAG_ALLENTITIES	( 1<<1 )
#define FRAMESNAP_FLAG_MULTIPOV		( 1<<2 )

// match_state_t communication
#define MATCHSTATE_FLAG_STATE		( 1<<0 )
#define MATCHSTATE_FLAG_ROUNDSTATE	( 1<<1 )
#define MATCHSTATE_FLAG_TIMELIMIT	( 1<<2 )
#define MATCHSTATE_FLAG_CLOCK_MSECS	( 1<<3 )
#define MATCHSTATE_FLAG_CLOCK_SECS	( 1<<4 )
#define MATCHSTATE_FLAG_CLOCK_MINUTES	    ( 1<<5 )

#define MATCHSTATE_EXTENDEDTIME_BIT	( 1<<31 )

// plyer_state_t communication

#define	PS_M_TYPE	    ( 1<<0 )
#define	PS_M_ORIGIN0	    ( 1<<1 )
#define	PS_M_ORIGIN1	    ( 1<<2 )
#define	PS_M_ORIGIN2	    ( 1<<3 )
#define	PS_M_VELOCITY0	    ( 1<<4 )
#define	PS_M_VELOCITY1	    ( 1<<5 )
#define	PS_M_VELOCITY2	    ( 1<<6 )
#define PS_MOREBITS1	    ( 1<<7 )

#define	PS_M_TIME	    ( 1<<8 )
#define	PS_M_FLAGS	    ( 1<<9 )
#define	PS_M_DELTA_ANGLES0  ( 1<<10 )
#define	PS_M_DELTA_ANGLES1  ( 1<<11 )
#define	PS_M_DELTA_ANGLES2  ( 1<<12 )
#define	PS_EVENT	    ( 1<<13 )
#define	PS_VIEWANGLES	    ( 1<<14 )
#define PS_MOREBITS2	    ( 1<<15 )

#define	PS_M_GRAVITY	    ( 1<<16 )
#define	PS_FOV		    ( 1<<17 )
#define	PS_POVNUM	    ( 1<<18 )
#define	PS_VIEWHEIGHT	    ( 1<<19 )
#define PS_PMOVESTATS	    ( 1<<20 )
#define PS_WEAPONLIST	    ( 1<<21 )
#define PS_PLRKEYS	    ( 1<<22 )
#define PS_MOREBITS3	    ( 1<<23 )




//==============================================

// user_cmd_t communication

//#define	CMD_BACKUP		64	// allow a lot of command backups for very fast systems
//#define CMD_MASK		(CMD_BACKUP-1)

// ms and light always sent, the others are optional
#define	CM_ANGLE1   ( 1<<0 )
#define	CM_ANGLE2   ( 1<<1 )
#define	CM_ANGLE3   ( 1<<2 )
#define	CM_FORWARD  ( 1<<3 )
#define	CM_SIDE	    ( 1<<4 )
#define	CM_UP	    ( 1<<5 )
#define	CM_BUTTONS  ( 1<<6 )

//==============================================

// a sound without an ent or pos will be a local only sound
#define	SND_VOLUME	( 1<<0 )      // a byte
#define	SND_ATTENUATION	( 1<<1 )      // a byte
#define SND_POS0_8	( 1<<2 )      // a byte
#define SND_POS0_16	( 1<<3 )      // a short
#define SND_POS1_8	( 1<<4 )      // a byte
#define SND_POS1_16	( 1<<5 )      // a short
#define SND_POS2_8	( 1<<6 )      // a byte
#define SND_POS2_16	( 1<<7 )      // a short

#define DEFAULT_SOUND_PACKET_VOLUME 1.0
#define DEFAULT_SOUND_PACKET_ATTENUATION 1.0

//==============================================

// entity_state_t communication

// try to pack the common update flags into the first byte
#define	U_ORIGIN1	( 1<<0 )
#define	U_ORIGIN2	( 1<<1 )
#define	U_ORIGIN3	( 1<<2 )
#define	U_ANGLE2	( 1<<3 )
#define	U_ANGLE3	( 1<<4 )
#define	U_EVENT		( 1<<5 )
#define	U_REMOVE	( 1<<6 )      // REMOVE this entity, don't add it
#define	U_MOREBITS1	( 1<<7 )      // read one additional byte

// second byte
#define	U_NUMBER16	( 1<<8 )      // NUMBER8 is implicit if not set
#define	U_FRAME8	( 1<<9 )      // frame is a byte
#define	U_ANGLE1	( 1<<10 )
#define	U_MODEL		( 1<<11 )
#define U_TYPE		( 1<<12 )
#define	U_OTHERORIGIN	( 1<<13 )     // FIXME: get rid of this
#define U_EVENT2	( 1<<14 )
#define	U_MOREBITS2	( 1<<15 )     // read one additional byte

// third byte
#define	U_EFFECTS8	( 1<<16 )     // autorotate, trails, etc
#define U_WEAPON	( 1<<17 )
#define	U_SOUND		( 1<<18 )
#define	U_MODEL2	( 1<<19 )     // weapons, flags, etc
#define U_LIGHT		( 1<<20 )
#define	U_SOLID		( 1<<21 )     // angles are short if bmodel (precise)
#define	U_SKIN8		( 1<<22 )
#define	U_MOREBITS3	( 1<<23 )     // read one additional byte

// fourth byte
#define	U_SKIN16	( 1<<24 )
#define	U_SVFLAGS	( 1<<25 )     // for multiview, info need for culling
#define	U_RENDERFX8	( 1<<26 )
#define	U_EFFECTS16	( 1<<27 )
#define U_RENDERFX16	( 1<<28 )
#define	U_FRAME16	( 1<<29 )     // frame is a short
#define	U_TEAM		( 1<<30 )     // gameteam. Will rarely change

/*
   ==============================================================

   Library

   Dynamic library loading

   ==============================================================
 */

#ifdef __cplusplus
#define EXTERN_API_FUNC	   extern "C"
#else
#define EXTERN_API_FUNC	   extern
#endif

// bigmac
/*
// qcommon/library.c
typedef struct { char *name; void **funcPointer; } dllfunc_t;

void	Com_UnloadLibrary( void **lib );
void	*Com_LoadLibrary( const char *name, dllfunc_t *funcs ); // NULL-terminated array of functions

void	*Com_LoadSoundLibrary( const char *basename, void *parms );
void	Com_UnloadSoundLibrary( void );

void	*Com_LoadGameLibrary( const char *basename, const char *apifuncname, void **handle, void *parms, qboolean pure );
void	Com_UnloadGameLibrary( void **handle );

void	    Cbuf_Init( void );
void	    Cbuf_Shutdown( void );
void	    Cbuf_AddText( const char *text );
void	    Cbuf_InsertText( const char *text );
void	    Cbuf_ExecuteText( int exec_when, const char *text );
void	    Cbuf_AddEarlyCommands( qboolean clear );
qboolean    Cbuf_AddLateCommands( void );
void	    Cbuf_Execute( void );

//===========================================================================


typedef void ( *xcommand_t )( void );

void	    Cmd_PreInit( void );
void	    Cmd_Init( void );
void	    Cmd_Shutdown( void );
void	    Cmd_AddCommand( const char *cmd_name, xcommand_t function );
void	    Cmd_RemoveCommand( const char *cmd_name );
qboolean    Cmd_Exists( const char *cmd_name );
qboolean	Cmd_CheckForCommand( char *text );
char *Cmd_CompleteCommand( const char *partial );
void	    Cmd_WriteAliases( int file );
int	    Cmd_CompleteAliasCountPossible( const char *partial );
char **Cmd_CompleteAliasBuildList( const char *partial );
int	    Cmd_CompleteCountPossible( const char *partial );
char **Cmd_CompleteBuildList( const char *partial );
char *Cmd_CompleteAlias( const char *partial );
int	    Cmd_Argc( void );
char *Cmd_Argv( int arg );
char *Cmd_Args( void );
void	    Cmd_TokenizeString( const char *text );
void	    Cmd_ExecuteString( const char *text );
void	    Cmd_ForwardToServer( void );

#include "cvar.h"

#include "dynvar.h"

// 
void Irc_Connect_f( void );
void Irc_Disconnect_f( void );
dynvar_get_status_t Irc_GetConnected_f( void **connected );
dynvar_set_status_t Irc_SetConnected_f( void *connected );


int SVN_RevNumber( void );
const char *SVN_RevString( void );


// net.h -- quake's interface to the networking layer

#define	PACKET_HEADER		10          // two ints, and a short

#define	MAX_RELIABLE_COMMANDS	64          // max string commands buffered for restransmit
#define	MAX_PACKETLEN		1400        // max size of a network packet
#define	MAX_MSGLEN		32768       // max length of a message, which may be fragmented into multiple packets
// wsw: Medar: doubled the MSGLEN as a temporary solution for multiview on bigger servers
#define	FRAGMENT_SIZE		( MAX_PACKETLEN - 96 )
#define	FRAGMENT_LAST		( 1<<14 )
#define	FRAGMENT_BIT		( 1<<31 )

typedef enum
{
        NA_NOTRANSMIT,      // wsw : jal : fakeclients
        NA_LOOPBACK,
        NA_BROADCAST,
        NA_IP,
} netadrtype_t;

typedef struct netadr_s
{
        netadrtype_t type;
        qbyte ip[4];
        unsigned short port;
} netadr_t;

typedef enum
{
        SOCKET_LOOPBACK,
        SOCKET_UDP
#ifdef TCP_SUPPORT
        , SOCKET_TCP
#endif
} socket_type_t;

typedef struct
{
        qboolean open;

        socket_type_t type;
        netadr_t address;
        qboolean server;

        qboolean connected;
        netadr_t remoteAddress;

        int handle;
} socket_t;

typedef enum
{
        CONNECTION_FAILED = -1,
        CONNECTION_INPROGRESS = 0,
        CONNECTION_SUCCEEDED = 1
} connection_status_t;

void	    NET_Init( void );
void	    NET_Shutdown( void );

qboolean    NET_OpenSocket( socket_t *socket, socket_type_t type, const netadr_t *address, qboolean server );
void	    NET_CloseSocket( socket_t *socket );

connection_status_t NET_Connect( socket_t *socket, const netadr_t *address );
connection_status_t NET_CheckConnect( socket_t *socket );
qboolean	    NET_Listen( const socket_t *socket );
int		    NET_Accept( const socket_t *socket, socket_t *newsocket, netadr_t *address );

int	    NET_GetPacket( const socket_t *socket, netadr_t *address, msg_t *message );
qboolean    NET_SendPacket( const socket_t *socket, const void *data, size_t length, const netadr_t *address );

int	    NET_Get( const socket_t *socket, netadr_t *address, void *data, size_t length );
qboolean    NET_Send( const socket_t *socket, const void *data, size_t length, const netadr_t *address );

void	    NET_Sleep( int msec, socket_t *sockets[] );
const char *NET_ErrorString( void );
void	    NET_SetErrorString( const char *format, ... );
void	    NET_ShowIP( void );

const char *NET_SocketTypeToString( socket_type_t type );
const char *NET_SocketToString( const socket_t *socket );
char *NET_AddressToString( const netadr_t *address );
qboolean    NET_StringToAddress( const char *s, netadr_t *address );
qboolean    NET_StringToBaseAddress( const char *s, netadr_t *address );

qboolean    NET_CompareAddress( const netadr_t *a, const netadr_t *b );
qboolean    NET_CompareBaseAddress( const netadr_t *a, const netadr_t *b );
qboolean    NET_IsLANAddress( const netadr_t *address );
qboolean    NET_IsLocalAddress( const netadr_t *address );
void	    NET_NoTransmitAddress( netadr_t *address );
void	    NET_LoopbackAddress( netadr_t *address );
void	    NET_BroadcastAddress( netadr_t *address, int port );

//============================================================================

typedef struct
{
        const socket_t *socket;

        int dropped;                // between last packet and previous

        netadr_t remoteAddress;
        int game_port;              // game port value to write when transmitting

        // sequencing variables
        int incomingSequence;
        int incoming_acknowledged;
        int outgoingSequence;

        // incoming fragment assembly buffer
        int fragmentSequence;
        size_t fragmentLength;
        qbyte fragmentBuffer[MAX_MSGLEN];

        // outgoing fragment buffer
        // we need to space out the sending of large fragmented messages
        qboolean unsentFragments;
        size_t unsentFragmentStart;
        size_t unsentLength;
        qbyte unsentBuffer[MAX_MSGLEN];
        qboolean unsentIsCompressed;

        qboolean fatal_error;
} netchan_t;

extern netadr_t	net_from;


void Netchan_Init( void );
void Netchan_Shutdown( void );
void Netchan_Setup( netchan_t *chan, const socket_t *socket, const netadr_t *address, int qport );
qboolean Netchan_Process( netchan_t *chan, msg_t *msg );
qboolean Netchan_Transmit( netchan_t *chan, msg_t *msg );
qboolean Netchan_PushAllFragments( netchan_t *chan );
qboolean Netchan_TransmitNextFragment( netchan_t *chan );
int Netchan_CompressMessage( msg_t *msg );
int Netchan_DecompressMessage( msg_t *msg );
void Netchan_OutOfBand( const socket_t *socket, const netadr_t *address, size_t length, const qbyte *data );
void Netchan_OutOfBandPrint( const socket_t *socket, const netadr_t *address, const char *format, ... );
int Netchan_GamePort( void );


void	    FS_Init( void );
void	    FS_Frame( void );
void	    FS_Shutdown( void );

const char *FS_GameDirectory( void );
const char *FS_BaseGameDirectory( void );
qboolean    FS_SetGameDirectory( const char *dir, qboolean force );
int	    FS_GetGameDirectoryList( char *buf, size_t bufsize );
int	    FS_GetExplicitPurePakList( char ***paknames );

// handling of absolute filenames
// only to be used if necessary (library not supporting custom file handling functions etc.)
const char *FS_WriteDirectory( void );
void	    FS_CreateAbsolutePath( const char *path );
const char *FS_AbsoluteNameForFile( const char *filename );
const char *FS_AbsoluteNameForBaseFile( const char *filename );

// // game and base files
// file streaming
int	    FS_FOpenFile( const char *filename, int *filenum, int mode );
int	    FS_FOpenBaseFile( const char *filename, int *filenum, int mode );
int		FS_FOpenAbsoluteFile( const char *filename, int *filenum, int mode );
void	    FS_FCloseFile( int file );
void	    FS_FCloseBaseFile( int file );

int	    FS_Read( void *buffer, size_t len, int file );
int	    FS_Print( int file, const char *msg );
int	    FS_Printf( int file, const char *format, ... );
int	    FS_Write( const void *buffer, size_t len, int file );
int	    FS_Tell( int file );
int	    FS_Seek( int file, int offset, int whence );
int	    FS_Eof( int file );
int	    FS_Flush( int file );

// file loading
int	    FS_LoadFileExt( const char *path, void **buffer, void *stack, size_t stackSize, const char *filename, int fileline );
int	    FS_LoadBaseFileExt( const char *path, void **buffer, void *stack, size_t stackSize, const char *filename, int fileline );
void	FS_FreeFile( void *buffer );
void	FS_FreeBaseFile( void *buffer );
#define FS_LoadFile(path,buffer,stack,stacksize) FS_LoadFileExt(path,buffer,stack,stacksize,__FILE__,__LINE__)
#define FS_LoadBaseFile(path,buffer,stack,stacksize) FS_LoadBaseFileExt(path,buffer,stack,stacksize,__FILE__,__LINE__)

// util functions
qboolean    FS_CopyFile( const char *src, const char *dst );
qboolean    FS_CopyBaseFile( const char *src, const char *dst );
qboolean    FS_MoveFile( const char *src, const char *dst );
qboolean    FS_MoveBaseFile( const char *src, const char *dst );
qboolean    FS_RemoveFile( const char *filename );
qboolean    FS_RemoveBaseFile( const char *filename );
qboolean    FS_RemoveAbsoluteFile( const char *filename );
qboolean    FS_RemoveDirectory( const char *dirname );
qboolean    FS_RemoveBaseDirectory( const char *dirname );
qboolean    FS_RemoveAbsoluteDirectory( const char *dirname );
unsigned    FS_ChecksumAbsoluteFile( const char *filename );
unsigned    FS_ChecksumBaseFile( const char *filename );

// // only for game files
const char *FS_FirstExtension( const char *filename, const char *extensions[], int num_extensions );
const char *FS_PakNameForFile( const char *filename );
qboolean    FS_IsPureFile( const char *pakname );
const char *FS_BaseNameForFile( const char *filename );

int	    FS_GetFileList( const char *dir, const char *extension, char *buf, size_t bufsize, int start, int end );
int	    FS_GetFileListExt( const char *dir, const char *extension, char *buf, size_t *bufsize, int start, int end );

// // only for base files
void	    FS_AddPakFile( const char *pakname );
qboolean    FS_IsPakValid( const char *filename, unsigned *checksum );
qboolean    FS_AddPurePak( unsigned checksum );
void	    FS_RemovePurePaks( void );


#define	ERR_FATAL	0       // exit the entire game with a popup window
#define	ERR_DROP	1       // print to console and disconnect from game

#define MAX_PRINTMSG	3072

void	    Com_BeginRedirect( int target, char *buffer, int buffersize, void ( *flush ), const void *extra );
void	    Com_EndRedirect( void );
void	    Com_Printf( const char *format, ... );
void	    Com_DPrintf( const char *format, ... );
void	    Com_Error( int code, const char *format, ... );
void	    Com_Quit( void );

int	    Com_ClientState( void );        // this should have just been a cvar...
void	    Com_SetClientState( int state );

qboolean    Com_DemoPlaying( void );
void	    Com_SetDemoPlaying( qboolean state );

int	    Com_ServerState( void );        // this should have just been a cvar...
void	    Com_SetServerState( int state );

unsigned    Com_BlockChecksum( void *buffer, size_t length );
qbyte	    COM_BlockSequenceCRCByte( qbyte *base, size_t length, int sequence );

void	    Com_PageInMemory( qbyte *buffer, int size );

unsigned int Com_HashKey( const char *name, int hashsize );

extern cvar_t *developer;
extern cvar_t *dedicated;
extern cvar_t *host_speeds;
extern cvar_t *log_stats;
extern cvar_t *versioncvar;
extern cvar_t *revisioncvar;

extern int log_stats_file;

// host_speeds times
extern unsigned int time_before_game;
extern unsigned int time_after_game;
extern unsigned int time_before_ref;
extern unsigned int time_after_ref;
*/
// /bigmac

//#define MEMCLUMPING
//#define MEMTRASH

#define POOLNAMESIZE 128

#ifdef MEMCLUMPING

// give malloc padding so we can't waste most of a page at the end
#define MEMCLUMPSIZE ( 65536 - 1536 )

// smallest unit we care about is this many bytes
#define MEMUNIT 8
#define MEMBITS ( MEMCLUMPSIZE / MEMUNIT )
#define MEMBITINTS ( MEMBITS / 32 )
#define MEMCLUMP_SENTINEL 0xABADCAFE

#endif

#define MEMHEADER_SENTINEL1 0xDEADF00D
#define MEMHEADER_SENTINEL2 0xDF

typedef struct memheader_s
{
        // next and previous memheaders in chain belonging to pool
        struct memheader_s *next;
        struct memheader_s *prev;

        // pool this memheader belongs to
        struct mempool_s *pool;

#ifdef MEMCLUMPING
        // clump this memheader lives in, NULL if not in a clump
        struct memclump_s *clump;
#endif

        // size of the memory after the header (excluding header and sentinel2)
        size_t size;

        // file name and line where Mem_Alloc was called
        const char *filename;
        int fileline;

        // should always be MEMHEADER_SENTINEL1
        unsigned int sentinel1;
        // immediately followed by data, which is followed by a MEMHEADER_SENTINEL2 byte
} memheader_t;

#ifdef MEMCLUMPING

typedef struct memclump_s
{
        // contents of the clump
        qbyte block[MEMCLUMPSIZE];

        // should always be MEMCLUMP_SENTINEL
        unsigned int sentinel1;

        // if a bit is on, it means that the MEMUNIT bytes it represents are
        // allocated, otherwise free
        int bits[MEMBITINTS];

        // should always be MEMCLUMP_SENTINEL
        unsigned int sentinel2;

        // if this drops to 0, the clump is freed
        int blocksinuse;

        // largest block of memory available (this is reset to an optimistic
        // number when anything is freed, and updated when alloc fails the clump)
        int largestavailable;

        // next clump in the chain
        struct memclump_s *chain;
} memclump_t;

#endif

#define MEMPOOL_TEMPORARY	1
#define MEMPOOL_GAMEPROGS	2
#define MEMPOOL_USERINTERFACE	4
#define MEMPOOL_CLIENTGAME	8
#define MEMPOOL_SOUND		16
#define MEMPOOL_DB        32

typedef struct mempool_s
{
        // should always be MEMHEADER_SENTINEL1
        unsigned int sentinel1;

        // chain of individual memory allocations
        struct memheader_s *chain;

#ifdef MEMCLUMPING
        // chain of clumps (if any)
        struct memclump_s *clumpchain;
#endif

        // temporary, etc
        int flags;

        // total memory allocated in this pool (inside memheaders)
        int totalsize;

        // total memory allocated in this pool (actual malloc total)
        int realsize;

        // updated each time the pool is displayed by memlist, shows change from previous time (unless pool was freed)
        int lastchecksize;

        // name of the pool
        char name[POOLNAMESIZE];

        // linked into global mempool list or parent's children list
        struct mempool_s *next;

        struct mempool_s *parent;
        struct mempool_s *child;

        // file name and line where Mem_AllocPool was called
        const char *filename;

        int fileline;

        // should always be MEMHEADER_SENTINEL1
        unsigned int sentinel2;
} mempool_t;

// bigmac
/*
void Memory_Init( void );
void Memory_InitCommands( void );
void Memory_Shutdown( void );
void Memory_ShutdownCommands( void );

void *_Mem_AllocExt( mempool_t *pool, size_t size, int z, int musthave, int canthave, const char *filename, int fileline );
void *_Mem_Alloc( mempool_t *pool, size_t size, int musthave, int canthave, const char *filename, int fileline );
void *_Mem_Realloc( void *data, size_t size, const char *filename, int fileline );
void _Mem_Free( void *data, int musthave, int canthave, const char *filename, int fileline );
mempool_t *_Mem_AllocPool( mempool_t *parent, const char *name, int flags, const char *filename, int fileline );
mempool_t *_Mem_AllocTempPool( const char *name, const char *filename, int fileline );
void _Mem_FreePool( mempool_t **pool, int musthave, int canthave, const char *filename, int fileline );
void _Mem_EmptyPool( mempool_t *pool, int musthave, int canthave, const char *filename, int fileline );

void _Mem_CheckSentinels( void *data, const char *filename, int fileline );
void _Mem_CheckSentinelsGlobal( const char *filename, int fileline );

#define Mem_AllocExt( pool, size, z ) _Mem_AllocExt( pool, size, z, 0, 0, __FILE__, __LINE__ )
#define Mem_Alloc( pool, size ) _Mem_Alloc( pool, size, 0, 0, __FILE__, __LINE__ )
#define Mem_Realloc( data, size ) _Mem_Realloc( data, size, __FILE__, __LINE__ )
#define Mem_Free( mem ) _Mem_Free( mem, 0, 0, __FILE__, __LINE__ )
#define Mem_AllocPool( parent, name ) _Mem_AllocPool( parent, name, 0, __FILE__, __LINE__ )
#define Mem_AllocTempPool( name ) _Mem_AllocTempPool( name, __FILE__, __LINE__ )
#define Mem_FreePool( pool ) _Mem_FreePool( pool, 0, 0, __FILE__, __LINE__ )
#define Mem_EmptyPool( pool ) _Mem_EmptyPool( pool, 0, 0, __FILE__, __LINE__ )

#define Mem_CheckSentinels( data ) _Mem_CheckSentinels( data, __FILE__, __LINE__ )
#define Mem_CheckSentinelsGlobal() _Mem_CheckSentinelsGlobal( __FILE__, __LINE__ )

// used for temporary allocations
extern mempool_t *tempMemPool;
extern mempool_t *zoneMemPool;

#define Mem_ZoneMallocExt( size, z ) Mem_AllocExt( zoneMemPool, size, z )
#define Mem_ZoneMalloc( size ) Mem_Alloc( zoneMemPool, size )
#define Mem_ZoneFree( data ) Mem_Free( data )

#define Mem_TempMallocExt( size, z ) Mem_AllocExt( tempMemPool, size, z )
#define Mem_TempMalloc( size ) Mem_Alloc( tempMemPool, size )
#define Mem_TempFree( data ) Mem_Free( data )

void *Q_malloc( int cnt );
void Q_free( void *buf );

void Qcommon_Init( int argc, char **argv );
void Qcommon_Frame( unsigned int realmsec );
void Qcommon_Shutdown( void );


// directory searching
#define SFF_ARCH    0x01
#define SFF_HIDDEN  0x02
#define SFF_RDONLY  0x04
#define SFF_SUBDIR  0x08
#define SFF_SYSTEM  0x10

void	Sys_Init( void );
void	Sys_InitDynvars( void );

void	Sys_AppActivate( void );

unsigned int	Sys_Milliseconds( void );
quint64		Sys_Microseconds( void );
void		Sys_Sleep( unsigned int millis );

char *Sys_ConsoleInput( void );
void	Sys_ConsoleOutput( char *string );
void	Sys_SendKeyEvents( void );
void	Sys_Error( const char *error, ... );
void	Sys_Quit( void );
char *Sys_GetClipboardData( qboolean primary );
void	Sys_FreeClipboardData( char *data );

// wsw : aiwa : get symbol address in executable
#ifdef SYS_SYMBOL
void *Sys_GetSymbol( const char *moduleName, const char *symbolName );
#endif


void CL_Init( void );
void CL_InitDynvars( void );
void CL_Disconnect( const char *message );
void CL_Shutdown( void );
void CL_Frame( int realmsec, int gamemsec );
void Con_Print( char *text );
void SCR_BeginLoadingPlaque( void );

void SV_Init( void );
void SV_Shutdown( char *finalmsg );
void SV_ShutdownGame( char *finalmsg, qboolean reconnect );
void SV_Frame( int realmsec, int gamemsec );


void MM_Init( void );
void MM_Shutdown( void );
void MM_Frame( const int realmsec );


void ML_Init( void );
void ML_Shutdown( void );
void ML_Restart( qboolean forcemaps );

const char *ML_GetFilenameExt( const char *fullname, qboolean recursive );
const char *ML_GetFilename( const char *fullname );
const char *ML_GetFullname( const char *filename );

qboolean ML_FilenameExists( const char *filename );

qboolean ML_ValidateFilename( const char *filename );
qboolean ML_ValidateFullname( const char *fullname );

void ML_AddDownloadedMap( const char *filename );
size_t ML_GetMapByNum( int num, char *out, size_t size );
*/
// /bigmac

#endif // __QCOMMON_H
