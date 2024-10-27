#ifndef _SYSCALLS_HEADER
#define _SYSCALLS_HEADER

extern cgame_import_t oImports;
extern cgame_export_t oExports;

extern refdef_t SpyCam_rd;
extern bool g_bSpycamRendering;

void DrawSpyCam( void );
bool WorldToScreen( vec3_t, vec2_t );

/*
Exports
*/
void Init_Hook( unsigned int, int, int, qboolean, qboolean, qboolean, unsigned int, unsigned int, int );
void Shutdown_Hook( void );
void RenderView_Hook( float, float, int, unsigned int, float );

/*
Imports
*/
void R_AddEntityToScene_Hook( entity_t * );
void R_RenderScene_Hook( refdef_t * );
void SCR_DrawString_Hook( int, int, int, const char *, struct mufont_s *, vec4_t );

#endif // _SYSCALLS_HEADER