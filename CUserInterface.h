#ifndef _CUSERINTERFACE_HEADER
#define _CUSERINTERFACE_HEADER

enum
{
	// Window cvars
	UI_RADAR_VAR,
	UI_SPYCAM_VAR,
	UI_WINAMP_VAR,
	UI_PLAYLIST_VAR,
	// Hack cvars
	UI_ESP_VAR,	
	UI_AIMBOT_VAR,
	UI_WALLHACK_VAR,
	UI_QUADCHAMS_VAR,
	UI_LIGHT_VAR,
	UI_AUTOSHOOT_VAR,
	UI_SPYPLAYER_VAR,
	UI_FOV_VAR,
	// Misc cvars
	UI_BIND_VAR,
	UI_SCHEME_VAR,
	UI_HELP_VAR,
	UI_QUIT_VAR,
	UI_SPAM_VAR,
	UI_VARTABLE_SIZE,
	UI_WINTABLE_SIZE = UI_PLAYLIST_VAR + 1
};

enum
{
	BINDACTION_TOGGLE,
	BINDACTION_SET,
	BINDACTION_TABLE_SIZE
};

class CUserInterface;
class CButton;

typedef struct 
{
	char *szKeyCode;
	bool bDown;			// bDown is if it is down while the Gui is active
	bool bGlobalDown;	// bGlobalDown is if it is truely down (so the gui doesn't need to be drawing)
} Key_t;

typedef struct
{
	char	*szCvar;
	char	szValue[32];
	int		iValue; // atoi
	void	( *fnCallBack )( char *, CUserInterface * );
	CButton *pVarButton;
	CWindow *pWindow;

	// bind stuff
	CParser::arg_t	*szBindArgs;
	Key_t	*pKey;
	int		iBindType, iNumBindArgs;
} Variable_t;

typedef struct
{
	char	*szAction;
	char	*szArgs[64];
	int		iNumArgs;
	void	( *fnCallBack )( CUserInterface *, int, CParser::arg_t * );
} BindAction_t;

extern Key_t Keys[256];

extern CButton *pRadarVar, *pSpycamVar, *pWinampVar, *pPlaylistVar, *pEspVar, *pAimbotVar, *pWallhackVar, *pQuadchamsVar, *pLightVar, *pAutoshootVar, *pSpyplayerVar, *pFovVar;
extern CButton *pBindVar, *pSchemeVar, *pHelpVar, *pQuitVar, *pSpamVar;

class CUserInterface
{
public:
	CUserInterface( void );
	~CUserInterface( void );

	inline void ProcessMessage( char * );
	virtual void SetMemo( CMemo * );
	void UpdateCvars( void );

	static void fnUI_Radar( char *, CUserInterface * );
	static void fnUI_Spycam( char *, CUserInterface * );
	static void fnUI_Winamp( char *, CUserInterface * );
	static void fnUI_Playlist( char *, CUserInterface * );
	static void fnUI_Esp( char *, CUserInterface * );
	static void fnUI_Aimbot( char *, CUserInterface * );
	static void fnUI_Wallhack( char *, CUserInterface * );
	static void fnUI_Quadchams( char *, CUserInterface * );
	static void fnUI_Light( char *, CUserInterface * );
	static void fnUI_Autoshoot( char *, CUserInterface * );
	static void fnUI_Spyplayer( char *, CUserInterface * );
	static void fnUI_Fov( char *, CUserInterface * );

	static void fnUI_Bind( char *, CUserInterface * );
	static void fnUI_Scheme( char *, CUserInterface * );
	static void fnUI_Help( char *, CUserInterface * );
	static void fnUI_Quit( char *, CUserInterface * );
	static void fnUI_Spam( char *, CUserInterface * );

	Variable_t UIVarTable[UI_VARTABLE_SIZE];	
	BindAction_t BindActionTable[BINDACTION_TABLE_SIZE];

private:
	CMemo *m_pMemo;
};

extern CUserInterface UI;

#endif // _CUSERINTERFACE_HEADER