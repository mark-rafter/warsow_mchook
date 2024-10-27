#ifndef _DETOUR_HEADER
#define _DETOUR_HEADER

// I should really use a pattern finder... :|

// EXE
extern void Detour_Warsow( void );

#define R_RENDERMESHBUFFER_ADDR ( PBYTE )0x42E270
#define R_RENDERMESHBUFFER_LEN 5

#define CL_MOUSEMOVE_ADDR ( PBYTE )0x4052F0
#define CL_MOUSEMOVE_LEN 8

#define KEY_EVENT_ADDR ( PBYTE )0x41ED30
#define KEY_EVENT_LEN 6

#define MAINWNDPROC_ADDR ( PBYTE )0x45FF40
#define MAINWNDPROC_LEN 6

#define CLS_UCMDHEAD_ADDR ( int * )0x11DDB80
#define CL_CMDS_ADDR ( usercmd_t * )0x8469C4
#define CL_VIEWANGLES_ADDR ( float * )0x1196CDC
#define R_SHADERS_ADDR ( shader_t * )0x6F24C0
#define CL_MOUSEMOVE_ARG_X_ADDR ( int * )0x61D5DC
#define CL_MOUSEMOVE_ARG_Y_ADDR ( int * )0x61D610

// DLL
extern void Detour_CGame( void );

#define CG_DRAWPLAYERNAMES_ADDR ( ( PBYTE )pGetCGameAPI - 0x21F0 )
#define CG_DRAWPLAYERNAMES_LEN 6

#define CG_DRAW2D_ADDR ( ( PBYTE )pGetCGameAPI - 0xD90 )
#define CG_DRAW2D_LEN 6

#define CG_ENTITYEVENT_ADDR ( ( PBYTE )pGetCGameAPI - 0x17A40 )
#define CG_ENTITYEVENT_LEN 6

#define CG_POINTCONTENTS_ADDR ( ( PBYTE )pGetCGameAPI - 0x73A0 )

#define CG_ADDR ( ( PBYTE )pGetCGameAPI + 0x202140 )
#define CGS_ADDR ( ( PBYTE )pGetCGameAPI + 0x1D05E0 )
#define CG_ENTITIES_ADDR ( ( PBYTE )pGetCGameAPI + 0x2977E0 )
#define SCB_PLAYERS_ADDR ( ( PBYTE )pGetCGameAPI + 0x187A70 )
#define CG_ENTPMODELS_ADDR ( ( PBYTE )pGetCGameAPI + 0x1945A0 )

#endif // _DETOUR_HEADER