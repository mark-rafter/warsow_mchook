#include "Common.h"

// credits to bobbysing's very nice D3D gui for some of this

CWinamp Winamp;

CWinamp::CWinamp( void )
{
	memset( m_szCaption, '\0', sizeof( m_szCaption ) );
	memset( m_szTrackTitle, '\0', sizeof( m_szTrackTitle ) );
	memset( m_szWindow, '\0', sizeof( m_szWindow ) );
}

CWinamp::~CWinamp( void )
{
}

inline char *CWinamp::GetCaption( void )
{
	return( m_szCaption );
}

inline int CWinamp::GetPlaylistLen( void )
{
	return( SendMessage( m_hWnd, WM_USER, 0, 124 ) );
}

char *CWinamp::GetTitle( int iNum )
{
	DWORD dwTitle = 0;

	if( !this->bInited )
		return( 0 );

	SendMessageTimeout( m_hWnd, WM_USER, iNum, 212, SMTO_BLOCK, 100, &dwTitle );

	if( dwTitle )
	{
		char *pszTitle = new char [128];

		DWORD dwRead = ReadWinampMemory( dwTitle, pszTitle, 128 );
		
		if( dwRead )
		{
			pszTitle[ dwRead ] = 0;

			return( pszTitle );
		}

		delete [] pszTitle;
	}

	return( 0 );
}

inline char *CWinamp::GetTrackTitle( void )
{
	return( m_szTrackTitle );
}

bool CWinamp::Init( char *szWindow, char *szOutMsg )
{
	DWORD dwProcessId = 0;

	if( bInited && !strcmp( szWindow, this->m_szWindow ) )
		return( true ); // Winamp is already inited and this window name is the same.

	strcpy( this->m_szWindow, szWindow );

	m_hWnd = FindWindow( this->m_szWindow, NULL );

	if( !m_hWnd )
	{
		sprintf( szOutMsg, S_COLOR_RED "Error" S_COLOR_WHITE ": " S_COLOR_YELLOW "%s " S_COLOR_WHITE "not found.", m_szWindow );
		goto loc_Failed;
	}

	GetWindowThreadProcessId( m_hWnd, &dwProcessId );

	if( !dwProcessId || !( m_hProcess = OpenProcess( PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, dwProcessId ) ) )
	{
#ifdef _DEBUG_ADDLOG_
		AddLog( "ERROR: CWinamp::Init( ): Process ID / OpenProcess( ) Failed on Winamp window: \"%s\"", m_szWindow );
#endif // _DEBUG_ADDLOG_

		sprintf( szOutMsg, S_COLOR_RED "Error" S_COLOR_WHITE ": " S_COLOR_YELLOW "%s " S_COLOR_WHITE "cannot be accessed.", m_szWindow );
		goto loc_Failed;
	}

	bInited = true;

	Winamp.UpdatePlaylist( );
	return( true );

loc_Failed:
	memset( m_szCaption, '\0', sizeof( m_szCaption ) );
	strcpy( m_szCaption, "Winamp Stopped/Inactive" );

	Gui.WinampControls[WINAMP_TRACK].SetCaption( m_szCaption );

	bInited = false;
	return( false );
}

DWORD CWinamp::ReadWinampMemory( DWORD dwAddress, void *lpBuffer, DWORD dwSize )
{
	DWORD dwRead = 0;

	ReadProcessMemory( m_hProcess, reinterpret_cast<const void *>( dwAddress ), lpBuffer, dwSize, &dwRead );

	if( dwRead == dwSize )
		return( dwRead );

	return( 0 );
}

void CWinamp::UpdatePlaylist( void )
{
	Gui.WinampPLMemo.ClearLines( ); // Clear current playlist

	// loop through playlist, add to WinampPLMemo lines.
	for( int i = 0; i < this->GetPlaylistLen( ); i++ )
		Gui.WinampPLMemo.PushLine( this->GetTitle( i ) );

	Gui.WinampPLMemo.SetVisibleLines( 0, true ); // "true" so it scrolls to the top of the PL.
}

void CWinamp::UpdateTitle( void )
{
	// update caption
	bool bTrunc = false;
	char *szTmpString;

	if( !this->bInited )
		return;

	szTmpString = new char [128];

	memset( szTmpString, '\0', sizeof( szTmpString ) );
	
	GetWindowText( this->m_hWnd, szTmpString, 128 );

	if( !strcmp( szTmpString, m_szTrackTitle ) ) // Track title is still same
	{
		delete [] szTmpString;
		return;
	}

	memset( m_szCaption, '\0', sizeof( m_szCaption ) );
	strncpy( m_szCaption, szTmpString, strlen( szTmpString ) - 9 );

	memset( m_szTrackTitle, '\0', sizeof( m_szTrackTitle ) );
	strncpy( m_szTrackTitle, szTmpString, strlen( szTmpString ) - 9 );

	delete [] szTmpString;

	while( Gui.GetFont( )->GetStringLength( m_szCaption, false ) >= Gui.WinampControls[WINAMP_TRACK].GetDimension( 2 ) - 10 )
	{
		m_szCaption[ strlen( m_szCaption ) - 1 ] = '\0';
		bTrunc = true;
	}

	if( bTrunc )
		strcat( m_szCaption, ".." );

	Gui.WinampControls[WINAMP_TRACK].SetCaption( m_szCaption );
}

void CWinamp::fnPrevTrack( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			SendMessage( ( ( CWinamp * )pThis )->m_hWnd, WM_COMMAND, 40044, 0 );
			( ( CWinamp * )pThis )->UpdateTitle( );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnPlay( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			SendMessage( ( ( CWinamp * )pThis )->m_hWnd, WM_COMMAND, 40045, 0 );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnPause( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			SendMessage( ( ( CWinamp * )pThis )->m_hWnd, WM_COMMAND, 40046, 0 );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnStop( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			SendMessage( ( ( CWinamp * )pThis )->m_hWnd, WM_COMMAND, 40047, 0 );
			( ( CWinamp * )pThis )->UpdateTitle( );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnNextTrack( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			SendMessage( ( ( CWinamp * )pThis )->m_hWnd, WM_COMMAND, 40048, 0 );
			( ( CWinamp * )pThis )->UpdateTitle( );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnSpam( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			UI.UIVarTable[ UI_SPAM_VAR ].fnCallBack( "", &UI );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnPlaylist( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			strcpy( UI.UIVarTable[ UI_PLAYLIST_VAR ].szValue, ( !UI.UIVarTable[ UI_PLAYLIST_VAR ].iValue ? "1" : "0" ) );
			UI.UIVarTable[ UI_PLAYLIST_VAR ].iValue = atoi( UI.UIVarTable[ UI_PLAYLIST_VAR ].szValue );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnVolDown( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			SendMessage( ( ( CWinamp * )pThis )->m_hWnd, WM_COMMAND, 40059, 0 );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

void CWinamp::fnVolUp( CButton *pButton, void *pThis )
{
	if( !( ( CWinamp * )pThis )->bInited )
		return;

	static bool bClicked = false;

	int i = 0;

	float *fArea = pButton->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pButton->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pButton->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );

			SendMessage( ( ( CWinamp * )pThis )->m_hWnd, WM_COMMAND, 40058, 0 );
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pButton->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}