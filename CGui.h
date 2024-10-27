#ifndef _CGUI_HEADER
#define _CGUI_HEADER

#include "CFont.h"
#include "GL.h"

typedef enum
{
	OBJ_TITLEBAR,
	OBJ_MINIMIZE, // do not change the order of these 2
	OBJ_SCROLLUP,
	OBJ_SCROLLDOWN,
	OBJ_SCROLLBAR,

	OBJ_MISCOBJECT,
	OBJ_MISCBUTTON,
	OBJ_MISCEDITBOX,
	OBJ_MISCMEMO,

	OBJ_TABLE_SIZE
} ObjType_t; // used by CObject.

enum // glBegin types..
{
	S_LINE,
	S_LINE_LOOP,
	S_RECT,
	S_TRI1,
	S_TRI2
}; // used by CDrawScheme

typedef enum
{
	CMD_CONSOLE,
	CMD_TOTAL
} CmdOwner_t; // string starts with a '/' - differentiate between memos.

#define FONT_SIZE 12
#define SCHEME_LEN 4 // TODO: in final, make a table of schemes, use vSchemeTable.size( ).

class CDrawScheme
{
public:
	CDrawScheme( void );
	CDrawScheme( char *, float *, float *, int );
	~CDrawScheme( void );	

	void			DrawShape( float, float, float, float );

	virtual char	*GetSchemeName( void );
	inline float	*GetBorderColor( void );
	inline float 	*GetColor( void );
	inline int 		GetType( void );

	inline void 	SetBackup( bool );
	inline void 	SetBorderColor( float * );
	inline void 	SetColor( float * );
	inline void 	SetType( int );

private:
	char 			m_szSchemeName[64];
	float 			m_fBorderCol[4], m_fOrigBorderCol[4]; // orig's used for reset.
	float 			m_fColor[4], m_fOrigColor[4];
	int 			m_iType; // S_RECT / S_LINE_LOOP etc
	bool 			m_bBackupColor;
};

class CTemplate
{
public:
	CTemplate( void );
	CTemplate( float, float, float, float );
	~CTemplate( void );

	float			*GetDimensions( void );
	virtual float	GetDimension( int );
	void			SetDimensions( float, float, float, float );

protected:
	float			m_fDimensions[4];
};

class CWindow; // define CWindow so we can use it inside CObject

class CObject : public CTemplate // Inherit CTemplate class
{
public:
	CObject( void );
	CObject( float, float, float, float, CWindow *, ObjType_t = OBJ_MISCOBJECT ); // default to misc object, since 0 is obj_titlebar
	~CObject( void );

	virtual void	Draw( void );

	inline ObjType_t GetObjectType( void );
	inline CWindow	*GetParent( void );
	inline void		SetParent( CWindow * );

protected:
	ObjType_t		m_ObjType;
	CWindow			*m_pParentWindow;
};

class CButton : public CObject // inherit CObject class
{
public:
	CButton( void );
	CButton( float, float, float, float, void ( * )( CButton *, void * ), char *, bool, bool, CWindow *, void *, ObjType_t = OBJ_MISCBUTTON );
	~CButton( void );

	void		Draw( void );

	inline char	*GetCaption( void );
	inline bool	GetClick( void );

	virtual void SetCaption( char * );

	void		( *fnCallback )( CButton *, void * );

private:
	bool		m_bClick, m_bShadow, m_bCenterTitle;
	void		*m_pCallback;
	char		m_szCaption[256];
};

class CMemo;

class CScrollBar : public CObject
{
public:
	CScrollBar( void );
	CScrollBar( float, float, float, float, void ( * )( CScrollBar * ), CWindow *, float, float );
	~CScrollBar( void );

	void			Draw( void );

	inline bool		GetClick( void );
	inline float	GetMaxHeight( void );
	inline CMemo	*GetMemo( void );
	inline float	GetOffset( bool );

	inline void		SetDragging( bool );
	inline void 	SetMemo( CMemo * );

	void			UpdateSize( int, int );
	void			UpdatePosition( int, int, int );

	void			( *fnCallback )( CScrollBar * );

private:
	float			m_fMinYOffset, m_fMaxYOffset;
	float			m_fMaxHeight;
	bool			m_bClick, m_bDragging;
	CMemo			*m_pParentMemo;
};

typedef struct
{
	char szLineBuffer[256];

	inline char *GetBuffer( void ) // this is the only way I could get DrawString to work.
	{
		return( szLineBuffer );
	}
} line_t;

class CMemo : public CObject
{
public:
	CMemo( void );
	CMemo( float, float, float, float, CWindow *, CMemo *, ObjType_t = OBJ_MISCMEMO );
	~CMemo( void );

	static void 		fnScrollDown( CButton *, void * );
	static void 		fnScrollUp( CButton *, void * );
	static void			fnScrollBar( CScrollBar * );

	inline void			DestroyObjects( void );
	void 				Draw( void );
	inline int			GetNumLines( void );
	inline CScrollBar	*GetScrollBar( void );
	virtual void		ClearLines( void );
	void				PushLine( char * );
	void 				PushTruncLine( const char * );
	inline void			SetVisibleLines( int, bool );
	inline void			UpdateMaxVisLines( void );

private:
	int					m_iMaxVisLines, m_iNumLines;
	int					m_iCurVisibleLines;
	bool				m_bUpdateScrollBarPos;
	CButton 			*m_pScrollDown;
	CButton				*m_pScrollUp;
	CScrollBar			*m_pScrollBar;
	std::vector<line_t>	m_vLines;
	void				*m_pCallback;
};

class CEditBox : public CObject
{
public:
	CEditBox( void );
	CEditBox( float, float, float, float, void ( * )( CEditBox * ), CWindow *, ObjType_t = OBJ_MISCEDITBOX );
	~CEditBox( void );

	void 					CheckForCmd( char *, CmdOwner_t );
	void 					Draw( void );

	void 					( *fnCallback )( CEditBox * );

	void 					DrawListBox( void );
	void 					PushInput( CButton *, char * );
	inline void 			SetEditable( bool );
	inline void 			SetMemo( CMemo * );

private:
	std::vector<CButton *>	m_vInputList;
	int 					m_iLongestCaptionLen;
	bool 					m_bEditable, m_bListClick;
	char 					m_szInputDisplay[256], m_szInput[256];
	CMemo					*m_pMemo;
};

class CWindow : public CTemplate
{
public:
	CWindow( void );
	CWindow( float, float, float, float, char *, bool, int, bool );
	~CWindow( void );

	inline void				DestroyObjects( void );
	void					Draw( void );

	static void				fnMinimize( CButton *, void * );
	static void				fnTitleBar( CButton *, void * );

	inline bool 			GetDragging( void );
	inline bool 			GetMaximized( void );
	inline size_t 			GetObjectListSize( void );
	inline CObject 			*GetObjectPtr( int );
	inline CDrawScheme 		*GetScheme( void );
	inline char 			*GetTitle( void );

	void 					PushObject( CObject *, float, float, float, float, ObjType_t = OBJ_MISCOBJECT );
	void 					PushObject( CEditBox *,float, float, float, float, void ( * )( CEditBox * ), int, CMemo * );
	void 					PushObject( CMemo *,float, float, float, float );
	void 					PushObject( CButton *, float, float, float, float, void ( * )( CButton *, void * ), char *, bool, bool, void *, ObjType_t = OBJ_MISCBUTTON );
	void					PushObject( CScrollBar *, float, float, float, float, void ( * )( CScrollBar * ), float, float );
	virtual void 			ClearObjects( void );
	inline void 			ResetScheme( void );

	inline void				SetDragging( bool );
	inline void				SetProperties( bool );
	inline void 			SetScheme( float *, float *, int );

private:
	bool					m_bMaximized, m_bDragging, m_bBackground;
	int						m_iWinNum; // for getting this window off the vector stack inside CGui
	char					m_szTitle[64];
	std::vector<CObject *>	m_vObjects;

	CDrawScheme				*m_pScheme;
	CButton					*m_pTitleBar, *m_pMinBox;
};

class CGui
{
public:
	CGui( void );
	CGui( char, char *, int, CDrawScheme * );
	~CGui( void );

	void					DrawGuiWindows( void );

	inline int 				GetActiveState( void );
	inline CWindow 			*GetActiveWin( void );
	inline CWindow			*GetDraggingWin( void );

	inline CFont 			*GetFont( void );
	inline bool 			GetKey( int );
	inline CDrawScheme		*GetScheme( void );
	virtual CDrawScheme		*GetScheme( int );
	inline CWindow 			*GetWindow( int );
	inline int 				GetWindowSize( void );

    virtual bool			HandleMsg( int, bool );
	void					Init( void );

	void					PushWindow( CWindow *, float, float, float, float, char *, bool, bool = true );
	inline void				SetActiveRendered( bool );
	virtual void			SetActiveState( int );
	inline void				SetActiveWin( CWindow * );
	inline void				SetDraggingWin( CWindow * );
	inline void 			SetKey( int, bool );
	virtual void			SetScheme( CDrawScheme * );
	void 					SetScheme( float *, float *, int );
	void					Update( void );

	float					fMouse[2], fLastMouse[2];

	CWindow					ConsoleWnd, RadarWnd, SpycamWnd, SchemeWnd, WinampWnd, WinampPLWnd;
	CButton					WinampControls[TOTAL_WINAMP_CTRLS];
	CEditBox				ConsoleInput;
	CMemo					ConsoleMemo, WinampPLMemo;

private:
	bool					m_bInited, m_bActiveRendered;
	char					m_cActivationKey;
	int						m_iActive;
	CFont					*m_pFont; // could make multiple fonts if u want (CFont TitleBar, EditBox etc)
	std::vector<CWindow *>	m_vWindows;
	CWindow					*m_pActiveWin, *m_pDraggingWin;

	CDrawScheme				*m_SchemeList[SCHEME_LEN];
	CButton					*m_SchemeButton[SCHEME_LEN];
	CDrawScheme				*m_pScheme;
};

extern CGui Gui;// The GUI isn't designed for multiple CGui's, but to do so, just add something like
				// CGui *pParentGui in CWindow/CTemplate and whatever else needs it, CDrawScheme maybe?

extern void Fake( CButton *pThis );

#endif // _CGUI_HEADER