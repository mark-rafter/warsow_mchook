#ifndef _ENGINE_HEADER
#define _ENGINE_HEADER

extern cgame_export_t *( *pGetCGameAPI )( cgame_import_t * );
extern cgame_export_t *GetCGameAPI_Hook( cgame_import_t * );

extern void CG_DrawPlayerNames_Hook( struct mufont_s *font );
extern void ( *pCG_DrawPlayerNames )( struct mufont_s * );

extern void CG_Draw2D_Hook( void );
extern void ( *pCG_Draw2D )( void );

extern void CG_EntityEvent_Hook( void );
extern void ( *pCG_EntityEvent )( void );

extern void CG_AddShellEffects_Hook( void );
extern void ( *pCG_AddShellEffects )( void );

extern void R_RenderMeshBuffer_Hook( void );
extern void ( *pR_RenderMeshBuffer )( void );

extern void CL_MouseMove_Hook( usercmd_t * );
extern void ( *pCL_MouseMove )( usercmd_t * );

extern void Key_Event_Hook( int, qboolean );
extern void ( *pKey_Event )( int, qboolean );

extern LONG __stdcall MainWndProc_Hook( HWND, UINT, WPARAM, LPARAM );
extern LONG ( __stdcall *pMainWndProc )( HWND, UINT, WPARAM, LPARAM );

extern int ( *pCG_PointContents )( vec3_t );

extern bool g_bShellEffect, g_bDrawingGLPlayer, g_bUpdatePlayers;
extern int g_iCounter;

extern cg_static_t	*pCgs;
extern cg_state_t	*pCg;
extern centity_t	*pCg_Entities;
extern scb_playertab_t *pSCB_Players;
extern pmodel_t		*pCg_EntPModels;
extern centity_t	*g_pCent;
extern usercmd_t	*pCL_Cmds;
extern int			*pCLS_ucmdHead;
extern float		*g_pfViewangles;
extern shader_t		g_shaderCursor, g_shaderCrosshair, g_shaderArrowUp, g_shaderShell;
extern int			g_iSizeSmallFont;

#endif // _ENGINE_HEADER