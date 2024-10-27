/************************************************************************/
/* WARNING                                                              */
/* define this when we compile for a public release                     */
/* this will protect dangerous and untested pieces of code              */
/************************************************************************/
#define WSW_RELEASE

//==============================================
// wsw : jal :	these defines affect every project file. They are
//				work-in-progress stuff which is, sooner or later,
//				going to be removed by keeping or discarding it.
//==============================================

// pretty solid
#define CGAMEGETLIGHTORIGIN
#define MOREGRAVITY
#define ALT_ZLIB_COMPRESSION
#define HARDWARE_OUTLINES

//==============================================
// undecided status

#define PUTCPU2SLEEP

//#define UCMDTIMENUDGE
//#define SERVER_DOWNLOAD_COMMAND // needs curl on server side

//#define TCP_SUPPORT // problems on unix

//#define ENABLE_MATCHMAKER

#define DUEL_ARENA

#ifndef WSW_RELEASE
#define WSW_CTF_CVARS   // KoFFiE: enables CVARS to enable unlock and capture CVAR timers
#endif

#ifdef MOREGRAVITY
#define OLDGRAVITY 800
//#define NEWGRAVITY 900
#define NEWGRAVITY 850
#define GRAVITY_COMPENSATE ( (float)NEWGRAVITY/(float)OLDGRAVITY )
#endif

//#define QUAKE2_JUNK

// weapon experiments
#define SPIRALRIOT

// collaborations
//==============================================

// symbol address retrieval
//==============================================
// #define SYS_SYMBOL		// adds "sys_symbol" command and symbol exports to binary release
#if defined ( SYS_SYMBOL ) && defined ( _WIN32 )
#define SYMBOL_EXPORT __declspec( dllexport )
#else
#define SYMBOL_EXPORT
#endif


#define WITH_UTF8	// Unicode support

#define WITH_PROPERCHATHANDLING
