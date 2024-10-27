#ifndef _CWINAMP_HEADER
#define _CWINAMP_HEADER

typedef enum
{
	WINAMP_PREVTRACK,
	WINAMP_PLAY,
	WINAMP_PAUSE,
	WINAMP_STOP,
	WINAMP_NEXTTRACK,
	WINAMP_SPAM,
	WINAMP_PL,
	WINAMP_VOLDOWN,
	WINAMP_VOLUP,
	WINAMP_TRACK,

	TOTAL_WINAMP_CTRLS
} WinampControls_t;

class CButton;

class CWinamp
{
public:
	CWinamp( void );
	~CWinamp( void );

	static void fnPrevTrack( CButton *, void * );
	static void fnPlay( CButton *, void * );
	static void fnPause( CButton *, void * );
	static void fnStop( CButton *, void * );
	static void fnNextTrack( CButton *, void * );
	static void fnSpam( CButton *, void * );
	static void fnPlaylist( CButton *, void * );
	static void fnVolDown( CButton *, void * );
	static void fnVolUp( CButton *, void * );

	virtual char *GetCaption( void );
	virtual int	GetPlaylistLen( void );
	char		*GetTitle( int );
	virtual char *GetTrackTitle( void );

	bool		Init( char *, char * );
	void		UpdateTitle( void );
	void		UpdatePlaylist( void );
	DWORD		ReadWinampMemory( DWORD, void *, DWORD );

	bool		bInited;

private:
	char		m_szCaption[128], m_szTrackTitle[128], m_szWindow[128];
	HANDLE		m_hProcess;
	HWND		m_hWnd;
};

extern CWinamp Winamp;

#endif // _CWINAMP_HEADER