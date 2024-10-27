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

//	bigmac
#ifndef _Q_SHARED_HEADER
#define _Q_SHARED_HEADER
//	/bigmac

// global preprocessor defines
#include "config.h"

// q_shared.h -- included first by ALL program modules
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//==============================================

#ifdef _WIN32

// wsw : pb : zlib 1.2.3
//# define ZLIB_WINAPI

// unknown pragmas are SUPPOSED to be ignored, but....
#pragma warning( disable : 4244 )       // MIPS
// bigmac
/*
#pragma warning( disable : 4136 )       // X86
#pragma warning( disable : 4051 )       // ALPHA
*/
// /bigmac

//# pragma warning(disable : 4018)		// signed/unsigned mismatch
//# pragma warning(disable : 4305)		// truncation from const double to float

// bigmac
/*
#pragma warning( disable : 4514 )       // unreferenced inline function has been removed
#pragma warning( disable : 4152 )       // nonstandard extension, function/data pointer conversion in expression
#pragma warning( disable : 4201 )       // nonstandard extension used : nameless struct/union
#pragma warning( disable : 4054 )       // 'type cast' : from function pointer to data pointer
#pragma warning( disable : 4127 )       // conditional expression is constant
#pragma warning( disable : 4100 )       // unreferenced formal parameter
#pragma warning( disable : 4706 )       // assignment within conditional expression
#pragma warning( disable : 4702 )       // unreachable code
#pragma warning( disable : 4306 )       // conversion from 'int' to 'void *' of greater size
#pragma warning( disable : 4305 )       // truncation from 'void *' to 'int'
*/
// /bigmac

#if defined _M_AMD64
#pragma warning( disable : 4267 )       // conversion from 'size_t' to whatever, possible loss of data
#endif

#define HAVE___INLINE

#define HAVE__SNPRINTF

#define HAVE__VSNPRINTF

#define HAVE__STRICMP

#ifdef LCC_WIN32
#ifndef C_ONLY
#define C_ONLY
#endif
#define HAVE_TCHAR
#define HAVE_MMSYSTEM
#define HAVE_DLLMAIN
#else
#define HAVE_WSIPX
#endif

#define LIB_DIRECTORY "libs"
#define LIB_SUFFIX ".dll"

#define VID_INITFIRST

#define GL_DRIVERNAME "opengl32.dll"

#define VORBISFILE_LIBNAME "libvorbisfile.dll"

#ifdef NDEBUG
#define BUILDSTRING "Win32 RELEASE"
#else
#define BUILDSTRING "Win32 DEBUG"
#endif

#ifdef _M_IX86
#if defined __FreeBSD__
#define CPUSTRING "i386"
#define ARCH "freebsd_i386"
#else
#define CPUSTRING "x86"
#define ARCH "x86"
#endif
#elif defined ( _M_AMD64 )
#if defined __FreeBSD__
#define CPUSTRING "x86_64"
#define ARCH "freebsd_x86_64"
#else
#define CPUSTRING "x64"
#define ARCH "x64"
#endif
#elif defined ( _M_ALPHA )
#define CPUSTRING "axp"
#define ARCH	  "axp"
#endif

// doh, some compilers need a _ prefix for variables so they can be
// used in asm code
#ifdef __GNUC__     // mingw
#define VAR( x )    "_" # x
#else
#define VAR( x )    # x
#endif

#ifdef __GNUC__
# define NOSHFOLDER_HEADER
#endif

// wsw : aiwa : 64bit integers and integer-pointer types
#include <basetsd.h>
typedef INT64 qint64;
typedef UINT64 quint64;
typedef INT_PTR qintptr;
typedef UINT_PTR quintptr;

#endif

//==============================================

#if defined ( __linux__ ) || defined ( __FreeBSD__ )

#define HAVE_INLINE

#ifndef HAVE_STRCASECMP // SDL_config.h seems to define this too...
#define HAVE_STRCASECMP
#endif

#define LIB_DIRECTORY "libs"
#define LIB_SUFFIX ".so"

//# define GL_FORCEFINISH
#define GL_DRIVERNAME  "libGL.so.1"

#define VORBISFILE_LIBNAME "libvorbisfile.so"

#ifdef __FreeBSD__
#define BUILDSTRING "FreeBSD"
#else
#define BUILDSTRING "Linux"
#endif

#ifdef __i386__
#if defined __FreeBSD__
#define ARCH "freebsd_i386"
#define CPUSTRING "i386"
#else
#define ARCH "i386"
#define CPUSTRING "i386"
#endif
#elif defined ( __x86_64__ )
#if defined __FreeBSD__
#define ARCH "freebsd_x86_64"
#define CPUSTRING "x86_64"
#else
#define ARCH "x86_64"
#define CPUSTRING "x86_64"
#endif
#elif defined ( __powerpc__ )
#define CPUSTRING "ppc"
#define ARCH "ppc"
#elif defined ( __alpha__ )
#define CPUSTRING "axp"
#define ARCH "axp"
#else
#define CPUSTRING "Unknown"
#define ARCH "Unknown"
#endif

#define VAR( x ) # x

// wsw : aiwa : 64bit integers and integer-pointer types
#include <stdint.h>
typedef int64_t qint64;
typedef uint64_t quint64;
typedef intptr_t qintptr;
typedef uintptr_t quintptr;

#endif

//==============================================

#if defined ( __APPLE__ ) && defined ( __MACH__ )

#ifndef __MACOSX__
#define __MACOSX__
#endif

#define HAVE_INLINE

#ifndef HAVE_STRCASECMP // SDL_config.h seems to define this too...
#define HAVE_STRCASECMP
#endif

#define LIB_DIRECTORY "libs"
#define LIB_SUFFIX ".dylib"

//# define GL_FORCEFINISH
#define GL_DRIVERNAME  "/System/Library/Frameworks/OpenGL.framework/Libraries/libGL.dylib"

#define VORBISFILE_LIBNAME "libvorbisfile.dylib"

//Mac OSX has universal binaries, no need for cpu dependency
#define BUILDSTRING "MacOSX"
#define CPUSTRING "universal"
#define ARCH "mac"

#define VAR( x ) # x

#include <stdint.h>
typedef int64_t qint64;
typedef uint64_t quint64;
typedef intptr_t qintptr;
typedef uintptr_t quintptr;

#endif

//==============================================

#ifdef HAVE___INLINE
#ifndef inline
#define inline __inline
#endif
#elif !defined ( HAVE_INLINE )
#ifndef inline
#define inline
#endif
#endif

#ifdef HAVE__SNPRINTF
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

#ifdef HAVE__VSNPRINTF
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif
#endif

#ifdef HAVE__STRICMP
#ifndef Q_stricmp
#define Q_stricmp( s1, s2 ) _stricmp( ( s1 ), ( s2 ) )
#endif
#ifndef Q_strnicmp
#define Q_strnicmp( s1, s2, n ) _strnicmp( ( s1 ), ( s2 ), ( n ) )
#endif
#endif

#ifdef HAVE_STRCASECMP
#ifndef Q_stricmp
#define Q_stricmp( s1, s2 ) strcasecmp( ( s1 ), ( s2 ) )
#endif
#ifndef Q_strnicmp
#define Q_strnicmp( s1, s2, n ) strncasecmp( ( s1 ), ( s2 ), ( n ) )
#endif
#endif

#if ( defined ( _M_IX86 ) || defined ( __i386__ ) || defined ( __ia64__ ) ) && !defined ( C_ONLY )
#define id386
#else
#ifdef id386
#undef id386
#endif
#endif

#ifndef BUILDSTRING
#define BUILDSTRING "NON-WIN32"
#endif

#ifndef CPUSTRING
#define CPUSTRING  "NON-WIN32"
#endif

#ifdef HAVE_TCHAR
#include <tchar.h>
#endif

#if defined ( __GNUC__ )
#define ALIGN( x )   __attribute__( ( aligned( x ) ) )
#elif defined ( _MSC_VER )
#define ALIGN( x )   __declspec( align( 16 ) )
#else
#define ALIGN( x )
#endif

//==============================================

typedef unsigned char qbyte;
typedef enum { qfalse, qtrue }	  qboolean;

typedef unsigned int qwchar;	// Unicode character

#ifndef NULL
#define NULL ( (void *)0 )
#endif

// angle indexes
#define	PITCH		    0       // up / down
#define	YAW		    1       // left / right
#define	ROLL		    2       // fall over

#define	FORWARD		    0
#define	RIGHT		    1
#define	UP		    2

#define	MAX_QPATH	    64      // max length of a quake game pathname

#define	MAX_STRING_CHARS    1024    // max length of a string passed to Cmd_TokenizeString
#define	MAX_STRING_TOKENS   256     // max tokens resulting from Cmd_TokenizeString
#define	MAX_TOKEN_CHARS	    1024    // max length of an individual token
#define MAX_CONFIGSTRING_CHARS MAX_QPATH    // max length of a configstring string

#define MAX_NAME_BYTES	    32      // max length of a player name, including trailing \0
#define MAX_NAME_CHARS	    15      // max visible characters in a name
									// (color tokens and \0 not counted)



//
// per-level limits
//
#define	MAX_CLIENTS	    256     // absolute limit
#define	MAX_EDICTS	    1024    // must change protocol to increase more
#define	MAX_LIGHTSTYLES	    256
#define	MAX_MODELS	    256     // these are sent over the net as bytes
#define	MAX_SOUNDS	    256     // so they cannot be blindly increased
#define	MAX_IMAGES	    256
#define MAX_SKINFILES	    256
#define	MAX_ITEMS	    256
#define MAX_GENERAL	    ( MAX_CLIENTS*2 ) // general config strings

#define MAX_CM_AREAS	    0x100

// command line execution flags
#define	EXEC_NOW    0               // don't return until completed
#define	EXEC_INSERT 1               // insert at current position, but don't run yet
#define	EXEC_APPEND 2               // add to end of the command buffer


//==============================================================
//
//MATHLIB
//
//==============================================================

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

typedef vec_t quat_t[4];

typedef qbyte byte_vec4_t[4];

struct cplane_s;

extern vec3_t vec3_origin;
extern vec3_t axis_identity[3];
extern quat_t quat_identity;

#define	nanmask ( 255<<23 )

#define	IS_NAN( x ) ( ( ( *(int *)&x )&nanmask ) == nanmask )

#ifndef M_PI
#define M_PI	   3.14159265358979323846   // matches value in gcc v2 math.h
#endif

#ifndef M_TWOPI
#define M_TWOPI	   6.28318530717958647692
#endif

#define DEG2RAD( a ) ( a * M_PI ) / 180.0F
#define RAD2DEG( a ) ( a * 180.0F ) / M_PI


// returns b clamped to [a..c] range
//#define bound(a,b,c) (max((a), min((b), (c))))

#ifndef max
#define max( a, b ) ( ( a ) > ( b ) ? ( a ) : ( b ) )
#endif

#ifndef min
#define min( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#endif

#define bound( a, b, c ) ( ( a ) >= ( c ) ? ( a ) : ( b ) < ( a ) ? ( a ) : ( b ) > ( c ) ? ( c ) : ( b ) )

// clamps a (must be lvalue) to [b..c] range
#define clamp( a, b, c ) ( ( b ) >= ( c ) ? ( a ) = ( b ) : ( a ) < ( b ) ? ( a ) = ( b ) : ( a ) > ( c ) ? ( a ) = ( c ) : ( a ) )

#define random()	( ( rand() & 0x7fff ) / ( (float)0x7fff ) )  // 0..1
#define brandom( a, b )	   ( ( a )+random()*( ( b )-( a ) ) )                // a..b
#define crandom()	brandom( -1, 1 )                           // -1..1

int	Q_rand( int *seed );
#define Q_random( seed )      ( ( Q_rand( seed ) & 0x7fff ) / ( (float)0x7fff ) )    // 0..1
#define Q_brandom( seed, a, b )	( ( a )+Q_random( seed )*( ( b )-( a ) ) )                      // a..b
#define Q_crandom( seed )     Q_brandom( seed, -1, 1 )

float	Q_RSqrt( float number );
int	Q_log2( int val );

#define SQRTFAST( x ) ( ( x ) * Q_RSqrt( x ) ) // wsw : jal : //The expression a * rsqrt(b) is intended as a higher performance alternative to a / sqrt(b). The two expressions are comparably accurate, but do not compute exactly the same value in every case. For example, a * rsqrt(a*a + b*b) can be just slightly greater than 1, in rare cases.

#define PlaneDiff( point, plane ) ( ( ( plane )->type < 3 ? ( point )[( plane )->type] : DotProduct( ( point ), ( plane )->normal ) ) - ( plane )->dist )

#define DotProduct( x, y )	   ( ( x )[0]*( y )[0]+( x )[1]*( y )[1]+( x )[2]*( y )[2] )
#define CrossProduct( v1, v2, cross ) ( ( cross )[0] = ( v1 )[1]*( v2 )[2]-( v1 )[2]*( v2 )[1], ( cross )[1] = ( v1 )[2]*( v2 )[0]-( v1 )[0]*( v2 )[2], ( cross )[2] = ( v1 )[0]*( v2 )[1]-( v1 )[1]*( v2 )[0] )

#define PlaneDiff( point, plane ) ( ( ( plane )->type < 3 ? ( point )[( plane )->type] : DotProduct( ( point ), ( plane )->normal ) ) - ( plane )->dist )

#define VectorSubtract( a, b, c )   ( ( c )[0] = ( a )[0]-( b )[0], ( c )[1] = ( a )[1]-( b )[1], ( c )[2] = ( a )[2]-( b )[2] )
#define VectorAdd( a, b, c )	    ( ( c )[0] = ( a )[0]+( b )[0], ( c )[1] = ( a )[1]+( b )[1], ( c )[2] = ( a )[2]+( b )[2] )
#define VectorCopy( a, b )	   ( ( b )[0] = ( a )[0], ( b )[1] = ( a )[1], ( b )[2] = ( a )[2] )
#define VectorClear( a )	  ( ( a )[0] = ( a )[1] = ( a )[2] = 0 )
#define VectorNegate( a, b )	   ( ( b )[0] = -( a )[0], ( b )[1] = -( a )[1], ( b )[2] = -( a )[2] )
#define VectorSet( v, x, y, z )	  ( ( v )[0] = ( x ), ( v )[1] = ( y ), ( v )[2] = ( z ) )
#define VectorAvg( a, b, c )	    ( ( c )[0] = ( ( a )[0]+( b )[0] )*0.5f, ( c )[1] = ( ( a )[1]+( b )[1] )*0.5f, ( c )[2] = ( ( a )[2]+( b )[2] )*0.5f )
#define VectorMA( a, b, c, d )	     ( ( d )[0] = ( a )[0]+( b )*( c )[0], ( d )[1] = ( a )[1]+( b )*( c )[1], ( d )[2] = ( a )[2]+( b )*( c )[2] )
#define VectorCompare( v1, v2 )	   ( ( v1 )[0] == ( v2 )[0] && ( v1 )[1] == ( v2 )[1] && ( v1 )[2] == ( v2 )[2] )
#define VectorLength( v )	  ( sqrt( DotProduct( ( v ), ( v ) ) ) )
#define VectorInverse( v )	  ( ( v )[0] = -( v )[0], ( v )[1] = -( v )[1], ( v )[2] = -( v )[2] )
#define VectorLerp( a, c, b, v )     ( ( v )[0] = ( a )[0]+( c )*( ( b )[0]-( a )[0] ), ( v )[1] = ( a )[1]+( c )*( ( b )[1]-( a )[1] ), ( v )[2] = ( a )[2]+( c )*( ( b )[2]-( a )[2] ) )
#define VectorScale( in, scale, out ) ( ( out )[0] = ( in )[0]*( scale ), ( out )[1] = ( in )[1]*( scale ), ( out )[2] = ( in )[2]*( scale ) )

#define DistanceSquared( v1, v2 ) ( ( ( v1 )[0]-( v2 )[0] )*( ( v1 )[0]-( v2 )[0] )+( ( v1 )[1]-( v2 )[1] )*( ( v1 )[1]-( v2 )[1] )+( ( v1 )[2]-( v2 )[2] )*( ( v1 )[2]-( v2 )[2] ) )
#define Distance( v1, v2 ) ( sqrt( DistanceSquared( v1, v2 ) ) )

#define VectorLengthFast( v )	  ( SQRTFAST( DotProduct( ( v ), ( v ) ) ) )  // wsw : jal :  //The expression a * rsqrt(b) is intended as a higher performance alternative to a / sqrt(b). The two expressions are comparably accurate, but do not compute exactly the same value in every case. For example, a * rsqrt(a*a + b*b) can be just slightly greater than 1, in rare cases.
#define DistanceFast( v1, v2 )	   ( SQRTFAST( DistanceSquared( v1, v2 ) ) )  // wsw : jal :  //The expression a * rsqrt(b) is intended as a higher performance alternative to a / sqrt(b). The two expressions are comparably accurate, but do not compute exactly the same value in every case. For example, a * rsqrt(a*a + b*b) can be just slightly greater than 1, in rare cases.

#define Vector2Set( v, x, y )	  ( ( v )[0] = ( x ), ( v )[1] = ( y ) )
#define Vector2Copy( a, b )	   ( ( b )[0] = ( a )[0], ( b )[1] = ( a )[1] )
#define Vector2Avg( a, b, c )	    ( ( c )[0] = ( ( ( a[0] )+( b[0] ) )*0.5f ), ( c )[1] = ( ( ( a[1] )+( b[1] ) )*0.5f ) )

#define Vector4Set( v, a, b, c, d )   ( ( v )[0] = ( a ), ( v )[1] = ( b ), ( v )[2] = ( c ), ( v )[3] = ( d ) )
#define Vector4Copy( a, b )	   ( ( b )[0] = ( a )[0], ( b )[1] = ( a )[1], ( b )[2] = ( a )[2], ( b )[3] = ( a )[3] )
#define Vector4Scale( in, scale, out )	    ( ( out )[0] = ( in )[0]*scale, ( out )[1] = ( in )[1]*scale, ( out )[2] = ( in )[2]*scale, ( out )[3] = ( in )[3]*scale )
#define Vector4Add( a, b, c )	    ( ( c )[0] = ( ( ( a[0] )+( b[0] ) ) ), ( c )[1] = ( ( ( a[1] )+( b[1] ) ) ), ( c )[2] = ( ( ( a[2] )+( b[2] ) ) ), ( c )[3] = ( ( ( a[3] )+( b[3] ) ) ) )
#define Vector4Avg( a, b, c )	    ( ( c )[0] = ( ( ( a[0] )+( b[0] ) )*0.5f ), ( c )[1] = ( ( ( a[1] )+( b[1] ) )*0.5f ), ( c )[2] = ( ( ( a[2] )+( b[2] ) )*0.5f ), ( c )[3] = ( ( ( a[3] )+( b[3] ) )*0.5f ) )

vec_t VectorNormalize( vec3_t v );       // returns vector length
vec_t VectorNormalize2( const vec3_t v, vec3_t out );
void  VectorNormalizeFast( vec3_t v );

// just in case you do't want to use the macros
void _VectorMA( const vec3_t veca, float scale, const vec3_t vecb, vec3_t vecc );
vec_t _DotProduct( const vec3_t v1, const vec3_t v2 );
void _VectorSubtract( const vec3_t veca, const vec3_t vecb, vec3_t out );
void _VectorAdd( const vec3_t veca, const vec3_t vecb, vec3_t out );
void _VectorCopy( const vec3_t in, vec3_t out );

void ClearBounds( vec3_t mins, vec3_t maxs );
void AddPointToBounds( const vec3_t v, vec3_t mins, vec3_t maxs );
float RadiusFromBounds( const vec3_t mins, const vec3_t maxs );
qboolean BoundsIntersect( const vec3_t mins1, const vec3_t maxs1, const vec3_t mins2, const vec3_t maxs2 );
qboolean BoundsAndSphereIntersect( const vec3_t mins, const vec3_t maxs, const vec3_t centre, float radius );

#define NUMVERTEXNORMALS    162
int DirToByte( vec3_t dir );
void ByteToDir( int b, vec3_t dir );

void NormToLatLong( const vec3_t normal, qbyte latlong[2] );

// bigmac
qboolean Q_TestRaySphere( vec3_t origin, float radius, const vec3_t start );
// /bigmac
void MakeNormalVectors( const vec3_t forward, vec3_t right, vec3_t up );
void AngleVectors( const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up );
int BoxOnPlaneSide( const vec3_t emins, const vec3_t emaxs, const struct cplane_s *plane );
float anglemod( float a );
float LerpAngle( float a1, float a2, const float frac );
float AngleSubtract( float a1, float a2 );
void AnglesSubtract( vec3_t v1, vec3_t v2, vec3_t v3 );
float AngleNormalize360( float angle );
float AngleNormalize180( float angle );
float AngleDelta( float angle1, float angle2 );
void VecToAngles( const vec3_t vec, vec3_t angles );
void AnglesToAxis( const vec3_t angles, vec3_t axis[3] );
void NormalVectorToAxis( const vec3_t forward, vec3_t axis[3] );
void BuildBoxPoints( vec3_t p[8], const vec3_t org, const vec3_t mins, const vec3_t maxs );

vec_t ColorNormalize( const vec_t *in, vec_t *out );

float CalcFov( float fov_x, float width, float height );
void AdjustFov( float *fov_x, float *fov_y, float width, float height, qboolean lock_x );

#define Q_rint( x )   ( ( x ) < 0 ? ( (int)( ( x )-0.5f ) ) : ( (int)( ( x )+0.5f ) ) )

#define BOX_ON_PLANE_SIDE( emins, emaxs, p )  \
        ( ( ( p )->type < 3 ) ?                       \
         ( \
                 ( ( p )->dist <= ( emins )[( p )->type] ) ?  \
                 1                               \
		 :                                   \
                 ( \
                         ( ( p )->dist >= ( emaxs )[( p )->type] ) ? \
                         2                           \
			 :                               \
                         3                           \
                 )                                   \
         )                                       \
	 :                                       \
         BoxOnPlaneSide( ( emins ), ( emaxs ), ( p ) ) )

void ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal );
void PerpendicularVector( vec3_t dst, const vec3_t src );
void RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees );

void Matrix_Identity( vec3_t m[3] );
void Matrix_Copy( vec3_t m1[3], vec3_t m2[3] );
qboolean Matrix_Compare( vec3_t m1[3], vec3_t m2[3] );
void Matrix_Multiply( vec3_t m1[3], vec3_t m2[3], vec3_t out[3] );
void Matrix_TransformVector( vec3_t m[3], vec3_t v, vec3_t out );
void Matrix_Transpose( vec3_t in[3], vec3_t out[3] );
void Matrix_EulerAngles( vec3_t m[3], vec3_t angles );
void Matrix_Rotate( vec3_t m[3], vec_t angle, vec_t x, vec_t y, vec_t z );
void Matrix_FromPoints( vec3_t v1, vec3_t v2, vec3_t v3, vec3_t m[3] );

typedef struct
{
	char quaternionstr[128];
} qwat_t;

void Quat_Identity( quat_t q );
void Quat_Copy( const quat_t q1, quat_t q2 );
qboolean Quat_Compare( const quat_t q1, const quat_t q2 );
void Quat_Conjugate( const quat_t q1, quat_t q2 );
vec_t Quat_Normalize( quat_t q );
vec_t Quat_Inverse( const quat_t q1, quat_t q2 );
void Quat_Multiply( const quat_t q1, const quat_t q2, quat_t out );
void Quat_Lerp( const quat_t q1, const quat_t q2, vec_t t, quat_t out );
void Quat_Vectors( const quat_t q, vec3_t f, vec3_t r, vec3_t u );
void Quat_Matrix( const quat_t q, vec3_t m[3] );
void Matrix_Quat( vec3_t m[3], quat_t q );
void Quat_TransformVector( const quat_t q, const vec3_t v, vec3_t out );
void Quat_ConcatTransforms( const quat_t q1, const vec3_t v1, const quat_t q2, const vec3_t v2, quat_t q, vec3_t v );

//=============================================

char *COM_SanitizeFilePath( char *filename );
qboolean COM_ValidateFilename( const char *filename );
qboolean COM_ValidateRelativeFilename( const char *filename );
void COM_StripExtension( char *filename );
const char *COM_FileExtension( const char *in );
void COM_DefaultExtension( char *path, const char *extension, size_t size );
void COM_ReplaceExtension( char *path, const char *extension, size_t size );
const char *COM_FileBase( const char *in );
void COM_StripFilename( char *filename );
int COM_FilePathLength( const char *in );

// data is an in/out parm, returns a parsed out token
char *COM_ParseExt2( const char **data_p, qboolean nl, qboolean sq );
#define COM_ParseExt( data_p, nl ) COM_ParseExt2( (const char **)data_p, nl, qtrue )
#define COM_Parse( data_p )   COM_ParseExt( data_p, qtrue )

int COM_Compress( char *data_p );
const char *COM_RemoveJunkChars( const char *in );
int COM_ReadColorRGBString( const char *in );

// color string functions ("^1text" etc)
#define GRABCHAR_END	0
#define GRABCHAR_CHAR	1
#define GRABCHAR_COLOR	2
int Q_GrabCharFromColorString( const char **pstr, char *c, int *colorindex );
const char *COM_RemoveColorTokensExt( const char *str, qboolean draw );
#define COM_RemoveColorTokens(in) COM_RemoveColorTokensExt(in,qfalse)
int COM_SanitizeColorString (const char *str, char *buf, int bufsize, int maxprintablechars, int startcolor);
char *Q_ColorStringTerminator( const char *str, int finalcolor );

//=============================================

extern const char *SOUND_EXTENSIONS[];
extern const size_t NUM_SOUND_EXTENSIONS;

extern const char *IMAGE_EXTENSIONS[];
extern const size_t NUM_IMAGE_EXTENSIONS;

//=============================================

extern vec4_t colorBlack;
extern vec4_t colorRed;
extern vec4_t colorGreen;
extern vec4_t colorBlue;
extern vec4_t colorYellow;
extern vec4_t colorMagenta;
extern vec4_t colorCyan;
extern vec4_t colorWhite;
extern vec4_t colorLtGrey;
extern vec4_t colorMdGrey;
extern vec4_t colorDkGrey;
extern vec4_t colorOrange;

#define MAX_S_COLORS 10

#define Q_COLOR_ESCAPE	'^'
#define S_COLOR_ESCAPE	"^"

#define COLOR_BLACK	'0'
#define COLOR_RED	'1'
#define COLOR_GREEN	'2'
#define COLOR_YELLOW	'3'
#define COLOR_BLUE	'4'
#define COLOR_CYAN	'5'
#define COLOR_MAGENTA	'6'
#define COLOR_WHITE	'7'
#define COLOR_ORANGE	'8'
#define COLOR_GREY	'9'
#define ColorIndex( c )	  ( ( ( ( c )-'0' ) < MAX_S_COLORS ) && ( ( ( c )-'0' ) >= 0 ) ? ( ( c ) - '0' ) : 7 )

#define	COLOR_R( rgba )	      ( ( rgba ) & 0xFF )
#define	COLOR_G( rgba )	      ( ( ( rgba ) >> 8 ) & 0xFF )
#define	COLOR_B( rgba )	      ( ( ( rgba ) >> 16 ) & 0xFF )
#define	COLOR_A( rgba )	      ( ( ( rgba ) >> 24 ) & 0xFF )
#define COLOR_RGB( r, g, b )	( ( ( r ) << 0 )|( ( g ) << 8 )|( ( b ) << 16 ) )
#define COLOR_RGBA( r, g, b, a ) ( ( ( r ) << 0 )|( ( g ) << 8 )|( ( b ) << 16 )|( ( a ) << 24 ) )

#define S_COLOR_BLACK	"^0"
#define S_COLOR_RED	"^1"
#define S_COLOR_GREEN	"^2"
#define S_COLOR_YELLOW	"^3"
#define S_COLOR_BLUE	"^4"
#define S_COLOR_CYAN	"^5"
#define S_COLOR_MAGENTA	"^6"
#define S_COLOR_WHITE	"^7"
#define S_COLOR_ORANGE	"^8"
#define S_COLOR_GREY	"^9"

extern vec4_t color_table[MAX_S_COLORS];

#define DEFAULT_FONT_SMALL "bitstream_10"
#define DEFAULT_FONT_MEDIUM "bitstream_12"
#define DEFAULT_FONT_BIG "virtue_16"
#define DEFAULT_FONT_SCOREBOARD "virtue_10"

#define ALIGN_LEFT_TOP	    0
#define ALIGN_CENTER_TOP    1
#define ALIGN_RIGHT_TOP	    2
#define ALIGN_LEFT_MIDDLE   3
#define ALIGN_CENTER_MIDDLE 4
#define ALIGN_RIGHT_MIDDLE  5
#define ALIGN_LEFT_BOTTOM   6
#define ALIGN_CENTER_BOTTOM 7
#define ALIGN_RIGHT_BOTTOM  8

//=============================================

void Q_strncpyz( char *dest, const char *src, size_t size );
void Q_strncatz( char *dest, const char *src, size_t size );
int Q_vsnprintfz( char *dest, size_t size, const char *format, va_list argptr );
void Q_snprintfz( char *dest, size_t size, const char *format, ... );
char *Q_strupr( char *s );
char *Q_strlwr( char *s );
const char *Q_strrstr( const char *s, const char *substr );
qboolean Q_WildCmp( const char *pattern, const char *text );
qboolean Q_isdigit( const char *str );

#ifdef WITH_UTF8
char *Q_western_to_utf8 (const char *src);
char *Q_utf8_to_western (const char *src);
char *Q_WCharToUtf8( qwchar wc );
qwchar Q_GrabWCharFromUtf8String (const char **pstr);
qwchar Q_GrabWCharFromColorString( const char **pstr, qwchar *wc, int *colorindex );
#define UTF8SYNC_LEFT 0
#define UTF8SYNC_RIGHT 1
int Q_Utf8SyncPos( const char *str, int pos, int dir );
#endif

//=============================================
#if !defined ( ENDIAN_LITTLE ) && !defined ( ENDIAN_BIG )
#if defined ( __i386__ ) || defined ( __ia64__ ) || defined ( WIN32 ) || ( defined ( __alpha__ ) || defined ( __alpha ) ) || defined ( __arm__ ) || ( defined ( __mips__ ) && defined ( __MIPSEL__ ) ) || defined ( __LITTLE_ENDIAN__ ) || defined ( __x86_64__ )
#define ENDIAN_LITTLE
#else
#define ENDIAN_BIG
#endif
#endif

short ShortSwap( short l );
int LongSwap( int l );
float FloatSwap( float f );

#ifdef ENDIAN_LITTLE
// little endian
#define BigShort( l ) ShortSwap( l )
#define LittleShort( l ) ( l )
#define BigLong( l ) LongSwap( l )
#define LittleLong( l ) ( l )
#define BigFloat( l ) FloatSwap( l )
#define LittleFloat( l ) ( l )
#elif defined ( ENDIAN_BIG )
// big endian
#define BigShort( l ) ( l )
#define LittleShort( l ) ShortSwap( l )
#define BigLong( l ) ( l )
#define LittleLong( l ) LongSwap( l )
#define BigFloat( l ) ( l )
#define LittleFloat( l ) FloatSwap( l )
#else
// figure it out at runtime
extern short ( *BigShort )(short l);
extern short ( *LittleShort )(short l);
extern int ( *BigLong )(int l);
extern int ( *LittleLong )(int l);
extern float ( *BigFloat )(float l);
extern float ( *LittleFloat )(float l);
#endif

void	Swap_Init( void );

float *tv( float x, float y, float z );
char *vtos( vec3_t v );
char *va( const char *format, ... );

//=============================================

//
// key / value info strings
//
#define	MAX_INFO_KEY	    64
#define	MAX_INFO_VALUE	    64
#define	MAX_INFO_STRING	    512

char *Info_ValueForKey( const char *s, const char *key );
void Info_RemoveKey( char *s, const char *key );
qboolean Info_SetValueForKey( char *s, const char *key, const char *value );
qboolean Info_Validate( const char *s );


//==============================================================
//
//SYSTEM SPECIFIC
//
//==============================================================


// this is only here so the functions in q_shared.c and q_math.c can link
void Sys_Error( const char *error, ... );
void Com_Printf( const char *msg, ... );


//==============================================================
//
//FILESYSTEM
//
//==============================================================


#define FS_READ	    0
#define FS_WRITE    1
#define FS_APPEND   2

#define FS_SEEK_CUR 0
#define FS_SEEK_SET 1
#define FS_SEEK_END 2


//==========================================================
//
//CVARS (console variables)
//
//==========================================================


#ifndef CVAR
#define CVAR

typedef int cvar_flag_t;

// bit-masked cvar flags
#define CVAR_ARCHIVE	    1   // set to cause it to be saved to vars.rc
#define CVAR_USERINFO	    2   // added to userinfo  when changed
#define CVAR_SERVERINFO	    4   // added to serverinfo when changed
#define CVAR_NOSET	    8   // don't allow change from console at all,
// but can be set from the command line
#define CVAR_LATCH	    16  // save changes until map restart
#define CVAR_LATCH_VIDEO    32  // save changes until video restart
#define CVAR_LATCH_SOUND    64  // save changes until video restart
#define CVAR_CHEAT	    128 // will be reset to default unless cheats are enabled
#define CVAR_READONLY	    256 // don't allow changing by user, ever
#define CVAR_DEVELOPER	    512 // allow changing in dev builds, hide in release builds

// nothing outside the Cvar_*() functions should access these fields!!!
typedef struct cvar_s
{
        char *name;
        char *string;
        char *dvalue;
        char *latched_string;       // for CVAR_LATCH vars
        cvar_flag_t flags;
        qboolean modified;          // set each time the cvar is changed
        float value;
        int integer;
} cvar_t;

#endif // CVAR


//==========================================================
//
//DYNVARS (dynamic variables)
//
//==========================================================


#ifndef DYNVAR
#define DYNVAR

// layout of dynvar_s structure hidden from users
struct dynvar_s;
typedef struct dynvar_s dynvar_t;

typedef enum dynvar_get_status_t
{
        DYNVAR_GET_OK,
        DYNVAR_GET_WRITEONLY,
        DYNVAR_GET_TRANSIENT
} dynvar_get_status_t;

typedef enum dynvar_set_status_t
{
        DYNVAR_SET_OK,
        DYNVAR_SET_READONLY,
        DYNVAR_SET_INVALID,
        DYNVAR_SET_TRANSIENT
} dynvar_set_status_t;

// getter function signature
// getter writes current value to *val (if not WRITEONLY) and returns status
typedef dynvar_get_status_t ( *dynvar_getter_f )( void **val );

// setter function signature
// setter reads new value from val (if not READONLY) and returns status
typedef dynvar_set_status_t ( *dynvar_setter_f )( void *val );

// listener function signature
// called by Dynvar_SetValue, after the setter function returned with DYNVAR_SET_OK
typedef void ( *dynvar_listener_f )( void *val );

#endif // DYNVAR


//==============================================================
//
//COLLISION DETECTION
//
//==============================================================


// lower bits are stronger, and will eat weaker brushes completely
#define	CONTENTS_SOLID		1       // an eye is never valid in a solid
#define	CONTENTS_LAVA		8
#define	CONTENTS_SLIME		16
#define	CONTENTS_WATER		32
#define	CONTENTS_FOG		64

#define	CONTENTS_AREAPORTAL	0x8000

#define	CONTENTS_PLAYERCLIP	0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

#define	CONTENTS_TELEPORTER	0x40000
#define	CONTENTS_JUMPPAD	0x80000
#define CONTENTS_CLUSTERPORTAL	0x100000
#define CONTENTS_DONOTENTER	0x200000

#define	CONTENTS_ORIGIN		0x1000000   // removed before bsping an entity

#define	CONTENTS_BODY		0x2000000   // should never be on a brush, only in game
#define	CONTENTS_CORPSE		0x4000000
#define	CONTENTS_DETAIL		0x8000000   // brushes not used for the bsp
#define	CONTENTS_STRUCTURAL	0x10000000  // brushes used for the bsp
#define	CONTENTS_TRANSLUCENT	0x20000000  // don't consume surface fragments inside
#define	CONTENTS_TRIGGER	0x40000000
#define	CONTENTS_NODROP		0x80000000  // don't leave bodies or items (death fog, lava)



#define	SURF_NODAMAGE		0x1     // never give falling damage
#define	SURF_SLICK		0x2     // effects game physics
#define	SURF_SKY		0x4     // lighting from environment map
#define	SURF_LADDER		0x8
#define	SURF_NOIMPACT		0x10    // don't make missile explosions
#define	SURF_NOMARKS		0x20    // don't leave missile marks
#define	SURF_FLESH		0x40    // make flesh sounds and effects
#define	SURF_NODRAW		0x80    // don't generate a drawsurface at all
#define	SURF_HINT		0x100   // make a primary bsp splitter
#define	SURF_SKIP		0x200   // completely ignore, allowing non-closed brushes
#define	SURF_NOLIGHTMAP		0x400   // surface doesn't need a lightmap
#define	SURF_POINTLIGHT		0x800   // generate lighting info at vertexes
#define	SURF_METALSTEPS		0x1000  // clanking footsteps
#define	SURF_NOSTEPS		0x2000  // no footstep sounds
#define	SURF_NONSOLID		0x4000  // don't collide against curves with this set
#define SURF_LIGHTFILTER	0x8000  // act as a light filter during q3map -light
#define	SURF_ALPHASHADOW	0x10000 // do per-pixel light shadow casting in q3map
#define	SURF_NODLIGHT		0x20000 // never add dynamic lights
#define SURF_DUST		0x40000 // leave a dust trail when walking on this surface



// content masks
#define	MASK_ALL		( -1 )
#define	MASK_SOLID		( CONTENTS_SOLID )
#define	MASK_PLAYERSOLID	( CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY )
#define	MASK_DEADSOLID		( CONTENTS_SOLID|CONTENTS_PLAYERCLIP )
#define	MASK_MONSTERSOLID	( CONTENTS_SOLID|CONTENTS_MONSTERCLIP|CONTENTS_BODY )
#define	MASK_WATER		( CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME )
#define	MASK_OPAQUE		( CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA )
#define	MASK_SHOT		( CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE )


// gi.BoxEdicts() can return a list of either solid or trigger entities
// FIXME: eliminate AREA_ distinction?
#define	AREA_SOLID	1
#define	AREA_TRIGGERS	2

// 0-2 are axial planes
#define	PLANE_X		0
#define	PLANE_Y		1
#define	PLANE_Z		2

// 3 is not axial
#define	PLANE_NONAXIAL	3

// cplane_t structure
typedef struct cplane_s
{
        vec3_t normal;
        float dist;
        short type;         // for fast side tests
        short signbits;     // signx + (signy<<1) + (signz<<1)
} cplane_t;

int SignbitsForPlane( const cplane_t *out );
int PlaneTypeForNormal( const vec3_t normal );
void CategorizePlane( cplane_t *plane );
void PlaneFromPoints( vec3_t verts[3], cplane_t *plane );

qboolean ComparePlanes( const vec3_t p1normal, vec_t p1dist, const vec3_t p2normal, vec_t p2dist );
void SnapVector( vec3_t normal );
void SnapPlane( vec3_t normal, vec_t *dist );

// a trace is returned when a box is swept through the world
typedef struct
{
        qboolean allsolid;  // if true, plane is not valid
        qboolean startsolid; // if true, the initial point was in a solid area
        float fraction;     // time completed, 1.0 = didn't hit anything
        vec3_t endpos;      // final position
        cplane_t plane;     // surface normal at impact
        int surfFlags;      // surface hit
        int contents;       // contents on other side of surface hit
        int ent;            // not set by CM_*() functions
} trace_t;


// this structure needs to be communicated bit-accurate
// from the server to the client to guarantee that
// prediction stays in sync, so no floats are used.
// if any part of the game code modifies this struct, it
// will result in a prediction error of some degree.

#define PM_VECTOR_SNAP 16

enum
{
//	PM_STAT_JUMPTIME,
        PM_STAT_WJCOUNT,
        PM_STAT_NOUSERCONTROL,
        PM_STAT_KNOCKBACK,
        PM_STAT_DASHTIME,
        PM_STAT_WJTIME,
        PM_STAT_SLOW,

        PM_STAT_SIZE
};

typedef struct
{
        int pm_type;

        float origin[3];                // 12.3
        float velocity[3];              // 12.3

        int pm_flags;               // ducked, jump_held, etc
        int pm_time;                // each unit = 8 ms
        short stats[PM_STAT_SIZE];  // Kurim : timers for knockback, stun, doublejump, walljump
        int gravity;
        short delta_angles[3];      // add to command angles to get view direction
                                    // changed by spawns, rotating objects, and teleporters
} pmove_state_t;


//
// button bits
//
#define	BUTTON_ATTACK	    1
#define	BUTTON_WALK	    2
#define	BUTTON_SPECIAL	    4
#define	BUTTON_USE	    8
#define	BUTTON_ZOOM	    16
#define	BUTTON_BUSYICON	    32
#define	BUTTON_ANY	    128     // any key whatsoever

enum
{
        KEYICON_FORWARD = 0,
        KEYICON_BACKWARD,
        KEYICON_LEFT,
        KEYICON_RIGHT,
        KEYICON_FIRE,
        KEYICON_JUMP,
        KEYICON_CROUCH,
        KEYICON_SPECIAL,
        KEYICON_TOTAL
};

// user command communications
#define	CMD_BACKUP	64  // allow a lot of command backups for very fast systems
#define CMD_MASK	( CMD_BACKUP-1 )

#define UCMD_PUSHFRAC_SNAPSIZE 127.0f //32767.0f//send as char or short

// usercmd_t is sent to the server each client frame
typedef struct usercmd_s
{
        qbyte msec;
        qbyte buttons;
        short angles[3];
        float forwardfrac, sidefrac, upfrac;
        short forwardmove, sidemove, upmove;
        unsigned int serverTimeStamp;
} usercmd_t;


// pmove_state_t is the information necessary for client side movement
// prediction
typedef enum
{
        // can accelerate and turn
        PM_NORMAL,
        PM_SPECTATOR,
        // no acceleration or turning
        PM_DEAD,
        PM_GIB, // different bounding box
        PM_FREEZE,
        PM_CHASECAM // same as freeze, but so client knows it's in chasecam
} pmtype_t;

// pmove->pm_flags
#define	PMF_DUCKED	    1
#define	PMF_JUMP_HELD	    2
#define	PMF_ON_GROUND	    4
#define	PMF_TIME_WATERJUMP  8   // pm_time is waterjump
#define	PMF_TIME_LAND	    16  // pm_time is time before rejump
#define	PMF_TIME_TELEPORT   32  // pm_time is non-moving time
#define PMF_NO_PREDICTION   64  // temporarily disables prediction (used for grappling hook)
#define PMF_DASHING	    128 // Dashing flag
#define PMF_SPECIAL_HELD    256 // Special flag
#define PMF_WALLJUMPING	    512 // WJ starting flag
#define PMF_DOUBLEJUMPED    1024 // DJ stat flag
#define PMF_JUMPPAD_TIME    2048    // temporarily disables fall damage

//#ifdef WJDOUBLECLICKED
//#define PMF_WALLJUMPARMED	0x00000800	// 2048
//#endif

#define	MAXTOUCH    32
typedef struct
{
        // state (in / out)
        pmove_state_t *s;

        // command (in)
        usercmd_t cmd;
        qboolean snapinitial;       // if s has been changed outside pmove

        // results (out)
        int numtouch;
        int touchents[MAXTOUCH];
        float step;                 // used for smoothing the player view

        vec3_t viewangles;          // clamped
        float viewheight;

        vec3_t mins, maxs;          // bounding box size

        int groundentity;
        int watertype;
        int waterlevel;

        // jal : wsw : must be set before calling pmove
        int max_walljumps;

        int passent;
        int contentmask;

		int movestyle;
} pmove_t;


// connection state of the client in the server
typedef enum
{
        CS_FREE,    // can be reused for a new connection
        CS_ZOMBIE,  // client has been disconnected, but don't reuse
        // connection for a couple seconds
        CS_CONNECTING, // has send a "new" command, is awaiting for fetching configstrings
        CS_CONNECTED, // has been assigned to a client_t, but not in game yet
        CS_SPAWNED  // client is fully in game
} sv_client_state_t;


// entity_state_t->effects
// Effects are things handled on the client side (lights, particles, frame animations)
// that happen constantly on the given entity.
// An entity that has effects will be sent to the client
// even if it has a zero index model.
#define	EF_ROTATE_AND_BOB   1       // rotate and bob (bonus items)
#define EF_SHELL	    2
#define EF_STRONG_WEAPON    4
#define	EF_QUAD		    8
#define EF_UNUSED  16
#define EF_BUSYICON	    32
#define EF_ENEMY_FLAG	    64      // player is carrying the enemy flag
#define EF_TAKEDAMAGE	    128
#define EF_RESPAWN_INDICATOR	256

// these ones can be only set from client side
#define EF_AMMOBOX	    ( 1<<16 )
#define EF_RACEGHOST	    ( 1<<17 )
#define EF_OUTLINE	    ( 1<<18 )

// entity_state_t->renderfx flags
#define	RF_MINLIGHT	    1       // always have some light (viewmodel)
#define	RF_FULLBRIGHT	    2       // always draw full intensity
#define	RF_FRAMELERP	    4
#define RF_COLORSHELL_RED   8
#define	RF_COLORSHELL_GREEN 16
#define RF_COLORSHELL_BLUE  32
#define RF_NOSHADOW	    64

// these flags are set locally by client
#define	RF_VIEWERMODEL	    256     // don't draw through eyes, only mirrors
#define	RF_WEAPONMODEL	    512     // only draw through eyes and depth hack
#define RF_CULLHACK	    1024
#define RF_FORCENOLOD	    2048
#define RF_PLANARSHADOW	    4096
#define RF_OCCLUSIONTEST    8192

// sound channels
// channel 0 never willingly overrides
// other channels (1-7) always override a playing sound on that channel
#define	CHAN_AUTO		0
#define	CHAN_PAIN		1
#define	CHAN_VOICE		2
#define	CHAN_ITEM		3
#define	CHAN_BODY		4
#define	CHAN_MUZZLEFLASH	5
// modifier flags
#define	CHAN_NO_PHS_ADD		8   // send to all clients, not just ones in PHS (ATTN 0 will also do this)
#define	CHAN_RELIABLE		16  // send by reliable message, not datagram
/*
   // sound attenuation values (s_attenuation_model 1)
 #define S_DEFAULT_ATTENUATION_MODEL			"1"

 #define	ATTN_NONE				0	// full volume the entire level
 #define	ATTN_DISTANT			2	// distant sound (most likely explosions)
 #define	ATTN_NORM				5	// players, weapons, etc
 #define	ATTN_IDLE				8	// stuff around you
 #define	ATTN_STATIC				10	// diminish very rapidly with distance
 #define	ATTN_FOOTSTEPS			21	// must be very close to hear it
 */

// sound attenuation values (s_attenuation_model 3)
#define S_DEFAULT_ATTENUATION_MODEL	    "3"
#define S_DEFAULT_ATTENUATION_MAXDISTANCE   "12000"  //"8024"

#define	ATTN_NONE		0   // full volume the entire level
#define	ATTN_DISTANT		1   // distant sound (most likely explosions)
#define	ATTN_NORM		2   // players, weapons, etc
#define	ATTN_IDLE		4   // stuff around you
#define	ATTN_STATIC		6   // diminish very rapidly with distance
#define	ATTN_FOOTSTEPS		10  // must be very close to hear it

#define ATTN_LOOP		ATTN_STATIC

typedef enum
{
        key_game,
        key_console,
        key_message,
        key_menu,
        key_delegate
} keydest_t;


//==========================================================
//
//  ELEMENTS COMMUNICATED ACROSS THE NET
//
//==========================================================


// note that Q_rint was causing problems here
// (spawn looking straight up\down at delta_angles wrapping)

#define	ANGLE2SHORT( x )  ( (int)( ( x )*65536/360 ) & 65535 )
#define	SHORT2ANGLE( x )  ( ( x )*( 360.0/65536 ) )

#define	ANGLE2BYTE( x )	  ( (int)( ( x )*256/360 ) & 255 )
#define	BYTE2ANGLE( x )	  ( ( x )*( 360.0/256 ) )

#define MAX_GAMECOMMANDS    64  // command names for command completion
#define MAX_LOCATIONS	    64

//
// config strings are a general means of communication from
// the server to all connected clients.
// Each config string can be at most MAX_QPATH characters.
//
#define	CS_MESSAGE	    0
#define	CS_MAPNAME	    1
#define	CS_AUDIOTRACK	    2
#define CS_HOSTNAME	    3
#define CS_SERVERSETTINGS   4 // "%i %i %i %i %i", tv, hasChallengerQueue, maxteams, gametype, instagib, falldamage
#define CS_SKYBOX	    5

#define	CS_MAXCLIENTS	    30
#define	CS_MAPCHECKSUM	    31      // for catching cheater maps

//precache stuff begins here
#define	CS_MODELS	    32
#define	CS_SOUNDS	    ( CS_MODELS+MAX_MODELS )
#define	CS_IMAGES	    ( CS_SOUNDS+MAX_SOUNDS )
#define	CS_SKINFILES	    ( CS_IMAGES+MAX_IMAGES )
#define	CS_LIGHTS	    ( CS_SKINFILES+MAX_SKINFILES )
#define	CS_ITEMS	    ( CS_LIGHTS+MAX_LIGHTSTYLES )
#define	CS_PLAYERINFOS	    ( CS_ITEMS+MAX_ITEMS )
#define CS_GAMECOMMANDS	    ( CS_PLAYERINFOS+MAX_CLIENTS )
#define CS_LOCATIONS	    ( CS_GAMECOMMANDS+MAX_GAMECOMMANDS )
#define CS_GENERAL	    ( CS_LOCATIONS+MAX_LOCATIONS )
#define	MAX_CONFIGSTRINGS   ( CS_GENERAL+MAX_GENERAL )

//==============================================

// wsw : jal : masterservers cvar is shared by client and server. This ensures both have the same default string
#define	DEFAULT_MASTER_SERVERS_IPS  "dpmaster.deathmask.net ghdigital.com excalibur.nvg.ntnu.no eu.master.warsow.net"
#define SERVER_PINGING_TIMEOUT 500
#define DEFAULT_PLAYERMODEL "viciious"
#define DEFAULT_PLAYERSKIN  "default"

#ifdef UCMDTIMENUDGE
#define MAX_UCMD_TIMENUDGE 50
#endif


// entity_state_t is the information conveyed from the server
// in an update message about entities that the client will
// need to render in some way

#define ET_INVERSE	128

// edict->svflags
#define	SVF_NOCLIENT		0x00000001  // don't send entity to clients, even if it has effects
#define SVF_PORTAL		0x00000002  // merge PVS at old_origin
#define	SVF_NOORIGIN2		0x00000004  // don't send old_origin (non-continuous events)
#define	SVF_TRANSMITORIGIN2	0x00000008  // always send old_origin (beams, etc), just check one point for PHS if not SVF_PORTAL (must be non-solid)
#define	SVF_MONSTER		0x00000010  // treat as CONTENTS_MONSTER for collision
#define SVF_FAKECLIENT		0x00000020  // do not try to send anything to this client
#define SVF_BROADCAST		0x00000040  // always transmit
#define SVF_CORPSE		0x00000080  // treat as CONTENTS_CORPSE for collision
#define SVF_PROJECTILE		0x00000100  // sets s.solid to SOLID_NOT for prediction
#define SVF_ONLYTEAM		0x00000200  // this entity is only transmited to clients with the same ent->s.team value

// edict->solid values
typedef enum
{
        SOLID_NOT,      // no interaction with other objects
        SOLID_TRIGGER,  // only touch when inside, after moving
        SOLID_BBOX,     // touch on edge
        SOLID_BSP       // bsp clip, touch on edge
} solid_t;

#define SOLID_BMODEL	31  // special value for bmodel

// entity_state_t->event values
// entity events are for effects that take place relative
// to an existing entities origin.  Very network efficient.

#define EV_INVERSE	128

typedef struct entity_state_s
{
        int number;                 // edict index

        int svflags;

        int type;                   // ET_GENERIC, ET_BEAM, etc
        qboolean linearProjectile;          // is sent inside "type" as ET_INVERSE flag
        vec3_t linearProjectileVelocity;      // this is transmitted instead of origin when linearProjectile is true

        vec3_t origin;
        vec3_t angles;

        union
        {
                vec3_t old_origin;  // for lerping
                vec3_t origin2;     // ET_BEAM, ET_PORTALSURFACE, ET_EVENT specific
	};

        unsigned int modelindex;
        union {
                unsigned int modelindex2;
                int bodyOwner;          // ET_PLAYER specific, for dead bodies
	};

        union
        {
                int frame;
                int ownerNum;       // ET_EVENT specific
	};

        union
        {
                int skinnum;
                int firemode;       // for weapon events
                int damage;         // EV_BLOOD2, EV_BLOOD_SAVED
                int targetNum;      // ET_EVENT specific
                int colorRGBA;      // ET_BEAM, ET_EVENT specific
                int range;          // ET_LASERBEAM, ET_CURVELASERBEAM specific
	};

        int weapon;                 // WEAP_ for players
        qboolean teleported;        // the entity was teleported this snap (sent inside "weapon" as ET_INVERSE flag)

        unsigned int effects;

        union
        {
                int renderfx;
                int eventCount;     // ET_EVENT specific
                //int			ownerNum2;		// for bodies
	};

        int solid;                  // for client side prediction, 8*(bits 0-4) is x/y radius
                                    // 8*(bits 5-9) is z down distance, 8(bits10-15) is z up
                                    // GClip_LinkEntity sets this properly
        int sound;                  // for looping sounds, to guarantee shutoff

        int events[2];              // impulse events -- muzzle flashes, footsteps, etc
                                    // events only go out for a single frame, they
                                    // are automatically cleared each frame
        int eventParms[2];

        union
        {
                unsigned int linearProjectileTimeStamp;
                int light;              // constant light glow
	};

        int team;                   // team in the game
} entity_state_t;

//==============================================

typedef enum
{
        CA_UNINITIALIZED,
        CA_DISCONNECTED, // not talking to a server
        CA_CONNECTING,  // sending request packets to the server
        CA_HANDSHAKE,   // netchan_t established, waiting for svc_serverdata
        CA_CONNECTED,   // connection established, game module not loaded
        CA_LOADING,     // loading game module
        CA_ACTIVE       // game views should be displayed
} connstate_t;

enum
{
        DROP_TYPE_GENERAL,
        DROP_TYPE_PASSWORD,
        DROP_TYPE_NORECONNECT,
        DROP_TYPE_TOTAL
};

#define DROP_FLAG_AUTORECONNECT 1 // it's okay try reconnectting automatically

//==============================================

typedef struct
{
        int state;
        int roundstate;
        qboolean paused;

        unsigned int clock_msecs;
        unsigned int clock_secs;
        unsigned int clock_mins;

        unsigned int timelimit;

        qboolean extendedtime;
} match_state_t;

//==============================================

#define	MAX_PARSE_SOUNDS	64
typedef struct
{
        vec3_t pos;
        int entnum;
        int channel;
        int num;
        float volume;
        float attenuation;
} sound_t;

//==============================================

// player_state_t is the information needed in addition to pmove_state_t
// to rendered a view.  There will only be 10 player_state_t sent each second,
// but the number of pmove_state_t changes will be relative to client
// frame rates
#define	PS_MAX_STATS	32
#define MAX_WEAPLIST_STATS 16   // short

typedef struct
{
        pmove_state_t pmove;    // for prediction

        // these fields do not need to be communicated bit-precise

        vec3_t viewangles;          // for fixed views

        int event;
        unsigned int POVnum;        // entity number of the player in POV
        float viewheight;
        float fov;                  // horizontal field of view
        unsigned int weaponlist[MAX_WEAPLIST_STATS][3];
        short stats[PS_MAX_STATS];              // fast status bar updates
        qbyte plrkeys;              // infos on the pressed keys of chased player (self if not chasing)
} player_state_t;

// bigmac
#endif // _Q_SHARED_HEADER
// /bigmac