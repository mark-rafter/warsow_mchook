#include "Common.h"

// self explanatory.
namespace Colors
{
	float fBlack[4] =		{ 0.0f, 0.0f, 0.0f, 1.0f };
	float fTransBlack[4] =	{ 0.0f, 0.0f, 0.0f, 0.6f };
	float fRed[4] =			{ 1.0f, 0.0f, 0.0f, 1.0f };
	float fGreen[4] =		{ 0.0f, 1.0f, 0.0f, 1.0f };
	float fWhite[4] =		{ 1.0f, 1.0f, 1.0f, 1.0f };
	float fSteelBlue[4] =	{ 0.26f, 0.39f, 0.61f, 0.69f };
	float fGrey[4] =		{ 0.49f, 0.49f, 0.49f, 0.62f };
	float fOrange[4] =		{ 1.0f, 0.5f, 0.0f, 0.65f };
	float fYellow[4] =		{ 0.85f, 0.85f, 0.4f, 0.9f };
}

// declare 3 different schemes.
CDrawScheme gBlueScheme = CDrawScheme( "Blue", Colors::fBlack, Colors::fSteelBlue, S_RECT );
CDrawScheme gOrangeScheme = CDrawScheme( "Orange", Colors::fBlack, Colors::fOrange, S_RECT );
CDrawScheme gGreyScheme = CDrawScheme( "Grey", Colors::fWhite, Colors::fGrey, S_RECT );
CDrawScheme gYellowScheme = CDrawScheme( "Yellow", Colors::fYellow, Colors::fTransBlack, S_RECT );

// these will be redeclared later;
CButton gBlueButton = CButton( );
CButton gOrangeButton = CButton( );
CButton gGreyButton = CButton( );
CButton gYellowButton = CButton( );

// declare the global gui.
CGui Gui( '*', "DejaVu Sans", FONT_SIZE, &gBlueScheme );
// In this file I reference Gui quite alot (especially RE the mouse), if you make multiple
// Gui's (unlikely), you will have to add a pGui to the CTemplate class and ref it through that.

// just a dummy, used all round the place.
void Fake( CButton *pThis, void *pCallback )
{
	// dont do anything in here.
}

// function called when a button in the scheme window is called.
void fnSchemeChange( CButton *pThis, void *pCallback )
{
	int i = 0;

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		UI.fnUI_Scheme( AppendStr( "/scheme %s", pThis->GetCaption( ) ), &UI );
		Gui.SetKey( K_MOUSE1, false );
	}
}

// function called when an input box is clicked.
void fnInput( CEditBox *pThis )
{
	int i = 0;

    if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 )
			return;

		pThis->SetEditable( true );

		Gui.SetActiveWin( pThis->GetParent( ) );
		Gui.SetActiveRendered( true ); 	// the window has been rendered, this is to prevent it re-rendering in GuiDrawWindows (due to it being the active window).
	}
}

// CTemplate - inherited by lots of classes
CTemplate::CTemplate( void )
{
	memset( m_fDimensions, 0.0f, sizeof( m_fDimensions ) );
}

CTemplate::CTemplate( float x, float y, float fWidth, float fHeight )
{	
	m_fDimensions[0] = x;
	m_fDimensions[1] = y;
	m_fDimensions[2] = fWidth;
	m_fDimensions[3] = fHeight;
}

CTemplate::~CTemplate( void )
{
}

inline float *CTemplate::GetDimensions( void )
{
	return( m_fDimensions );
}

inline float CTemplate::GetDimension( int iDimension )
{
	return( m_fDimensions[ iDimension ] );
}

inline void CTemplate::SetDimensions( float x, float y, float fWidth, float fHeight )
{
	m_fDimensions[0] = x;
	m_fDimensions[1] = y;
	m_fDimensions[2] = fWidth;
	m_fDimensions[3] = fHeight;
}

// CObject - inherited by CEditBox, CButton and CMemo.
CObject::CObject( void ) : CTemplate( )
{
	m_pParentWindow = 0;
}

CObject::CObject( float x, float y, float fWidth, float fHeight, CWindow *pParentWindow, ObjType_t ObjType ) : CTemplate( x, y, fWidth, fHeight )
{
	m_pParentWindow = pParentWindow;
	m_ObjType = ObjType;
}

CObject::~CObject( void )
{
}

void CObject::Draw( void )
{
	float *fInheritedDim = m_pParentWindow->GetDimensions( );

	// set the scheme to draw a LINE_LOOP
	m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_LINE_LOOP );

	m_pParentWindow->GetScheme( )->DrawShape
	( 
		fInheritedDim[0] + m_fDimensions[0], // x and y are inherited from the parent.
		fInheritedDim[1] + m_fDimensions[1],
		m_fDimensions[2],
		m_fDimensions[3]
	);

	// reset the scheme to draw default.
	m_pParentWindow->ResetScheme( );
}

inline ObjType_t CObject::GetObjectType( void )
{
	return( m_ObjType );
}

inline CWindow *CObject::GetParent( void )
{
	return( m_pParentWindow );
}

inline void CObject::SetParent( CWindow *pParentWindow )
{
	m_pParentWindow = pParentWindow;
}

CScrollBar::CScrollBar( float x, float y, float fWidth, float fHeight, void ( *pFunction )( CScrollBar * ), CWindow *pParentWindow, float fMinYOffset, float fMaxYOffset ) : CObject( x, y, fWidth, fHeight, pParentWindow, OBJ_SCROLLBAR )
{
	this->fnCallback = pFunction;

	this->m_fMinYOffset = fMinYOffset; 
	this->m_fMaxYOffset = fMaxYOffset; 
	this->m_fMaxHeight = fHeight;
}

CScrollBar::CScrollBar( void ) : CObject( )
{
}

CScrollBar::~CScrollBar( void )
{
}

void CScrollBar::Draw( void )
{
	float *fInheritedDim = m_pParentWindow->GetDimensions( );

	if( Gui.fMouse[0] >= fInheritedDim[0] + m_fDimensions[0] && Gui.fMouse[0] <= fInheritedDim[0] + m_fDimensions[0] + m_fDimensions[2] 
	&& Gui.fMouse[1] >= fInheritedDim[1] + m_fDimensions[1] && Gui.fMouse[1] <= fInheritedDim[1] + m_fDimensions[1] + m_fDimensions[3] )
		this->fnCallback( this );

	m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_LINE_LOOP );

	m_pParentWindow->GetScheme( )->DrawShape
	( 
		fInheritedDim[0] + m_fDimensions[0], 
		fInheritedDim[1] + m_fDimensions[1], 
		m_fDimensions[2], 
		m_fDimensions[3]
	);

	m_pParentWindow->ResetScheme( );
}

inline float CScrollBar::GetOffset( bool bMin )
{
	return( bMin ? m_fMinYOffset : m_fMaxYOffset );
}

inline float CScrollBar::GetMaxHeight( void )
{
	return( m_fMaxHeight );
}

inline CMemo *CScrollBar::GetMemo( void )
{
	return( m_pParentMemo );
}

inline void CScrollBar::SetDragging( bool bState )
{
	m_bDragging = bState;
}

inline void CScrollBar::SetMemo( CMemo *pParentMemo )
{
	m_pParentMemo = pParentMemo;
}

// this is called when the visible lines are changed (pgup or scroll buttons pressed)
void CScrollBar::UpdatePosition( int iNumLines, int iVisibleLines, int iMaxVisLines )
{
	//a = b*((c-(d+e))/f)
	//c = ((a/b)*f)+(d+e)

	this->SetDimensions
	( 
		this->m_fDimensions[0], // vv Ugliest... code... ever... but still it fixes an annoying 'bug'
		( iVisibleLines == iNumLines - 1 ? ( ( ( ( float )iNumLines - ( float )iMaxVisLines ) / ( float )iNumLines ) * m_fMaxHeight ) + m_pParentMemo->GetDimension( 1 ) + m_fMinYOffset : ( ( ( ( float )iVisibleLines - ( float )iMaxVisLines ) / ( float )iNumLines ) * m_fMaxHeight ) + m_pParentMemo->GetDimension( 1 ) + m_fMinYOffset ),
		this->m_fDimensions[2],
		this->m_fDimensions[3]
	);
}

void CScrollBar::UpdateSize( int iNumLines, int iMaxVisLines )
{
	if( iNumLines > iMaxVisLines )
	{
		m_fDimensions[3] = m_fMaxHeight * ( ( float )iMaxVisLines / ( float )iNumLines );
		m_fDimensions[1] = ( this->m_pParentMemo->GetDimension( 1 ) + m_fMaxYOffset ) - m_fDimensions[3];
	}
	else // all lines currently visible
		m_fDimensions[3] = m_fMaxHeight;
}

CMemo::CMemo( void ) : CObject( )
{
	m_pScrollDown = new CButton;
	m_pScrollUp = new CButton;
	m_pScrollBar = new CScrollBar;

	m_iMaxVisLines = 0;
	m_iCurVisibleLines = m_iNumLines = ( int )m_vLines.size( );
	m_bUpdateScrollBarPos = false;
}

// Init the Memo, and the scrollbar.
CMemo::CMemo( float x, float y, float fWidth, float fHeight, CWindow *pParentWindow, CMemo *pStaticThis, ObjType_t ObjType ) : CObject( x, y, fWidth, fHeight, pParentWindow, ObjType )
{
	m_iMaxVisLines = 0;
	m_iCurVisibleLines = m_iNumLines = ( int )m_vLines.size( );
	m_bUpdateScrollBarPos = false;

	m_pScrollDown = new CButton;
	m_pScrollUp = new CButton;
	m_pScrollBar = new CScrollBar;

	m_pParentWindow->PushObject
	( 
		m_pScrollDown, 
		x + fWidth - 12, 
		y + fHeight - 11, 
		9, 
		9,
		CMemo::fnScrollDown,
		"", false, false,
		pStaticThis,
		OBJ_SCROLLDOWN
	);

	m_pParentWindow->PushObject
	( 
		m_pScrollUp, 
		x + fWidth - 12, 
		y + 2,
		9, 
		9,
		CMemo::fnScrollUp,
		"", false, false,
		pStaticThis,
		OBJ_SCROLLUP
	);

	m_pParentWindow->PushObject
	(
		m_pScrollBar, 
		x + fWidth - 12, 
		y + 13,
		9, 
		fHeight - 26,
		CMemo::fnScrollBar,
		13, 
		13 + ( fHeight - 26 )
	);
}

CMemo::~CMemo( void )
{
}

// Function called when scroll down button is clicked
void CMemo::fnScrollDown( CButton *pThis, void *pCallback )
{
	static bool bClicked = false;

	int i = 0;

	float *fArea = pThis->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pThis->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pThis->SetDimensions( fArea[0] + 1, fArea[1] + 1, fArea[2], fArea[3] );
		}

		if( ( ( CMemo * )pCallback )->m_iCurVisibleLines < ( ( CMemo * )pCallback )->m_iNumLines - 1 )
		{
			( ( CMemo * )pCallback )->m_iCurVisibleLines++;
			( ( CMemo * )pCallback )->m_bUpdateScrollBarPos = true;
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pThis->SetDimensions( fArea[0] - 1,	fArea[1] - 1, fArea[2],	fArea[3] );
	}
}

// Function called when scroll up button is clicked
void CMemo::fnScrollUp( CButton *pThis, void *pCallback )
{
	static bool bClicked = false;

	int i = 0;
	float *fArea = pThis->GetDimensions( );

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		if( pThis->GetClick( ) && !bClicked )
		{
			bClicked = true;
			pThis->SetDimensions( fArea[0] + 1,	fArea[1] + 1, fArea[2],	fArea[3] );
		}

		if( ( ( CMemo * )pCallback )->m_iCurVisibleLines > ( ( CMemo * )pCallback )->m_iMaxVisLines )
		{
			( ( CMemo * )pCallback )->m_iCurVisibleLines--;
			( ( CMemo * )pCallback )->m_bUpdateScrollBarPos = true;
		}
	}
	else if( bClicked )
	{
		bClicked = false;
		pThis->SetDimensions( fArea[0] - 1, fArea[1] - 1, fArea[2], fArea[3] );
	}
}

// Function called when scroll bar is clicked
void CMemo::fnScrollBar( CScrollBar *pThis )
{
	int i = 0;
	float *fDim = pThis->GetDimensions( ), *fMemoDim = pThis->GetMemo( )->GetDimensions( );
	float fMin = pThis->GetOffset( true ), fMax = pThis->GetOffset( false );

	float fDimY = 0.0f;

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged
			return;

		Gui.fMouse[0] = Gui.fLastMouse[0]; // lock x

		if( fDim[1] + Gui.fMouse[1] - Gui.fLastMouse[1] <= fMemoDim[1] + fMin )
		{
			Gui.fMouse[1] = Gui.fLastMouse[1];
			fDimY = fMemoDim[1] + fMin;
		}
		else if( ( fDim[1] + fDim[3] ) + Gui.fMouse[1] - Gui.fLastMouse[1] >= fMemoDim[1] + fMax )
		{
			Gui.fMouse[1] -= Gui.fMouse[1] - Gui.fLastMouse[1];
			fDimY = ( fMemoDim[1] + fMax ) - fDim[3];
		}
		else
			fDimY = fDim[1] + Gui.fMouse[1] - Gui.fLastMouse[1];

		pThis->SetDimensions
		( 
			fDim[0],
			fDimY,
			fDim[2],
			fDim[3]
		);

		// y / maxheight = %, eg: 0.5; iNumLines * 0.5 = start point (shows the following maxvislines, eg 18 lines)
		pThis->GetMemo( )->SetVisibleLines
		( 
			pThis->GetMemo( )->GetNumLines( ) * ( ( fDimY - ( fMemoDim[1] + fMin ) ) / pThis->GetMaxHeight( ) ),
			false // we've already update the scrollbar position.
		);
	}
}

inline void CMemo::DestroyObjects( void )
{
	delete m_pScrollDown;
	delete m_pScrollUp;
	delete m_pScrollBar;
}

void CMemo::Draw( void )
{
	float *fInheritedDim = m_pParentWindow->GetDimensions( );
	float *fScrollDownDim = m_pScrollDown->GetDimensions( );
	float *fScrollUpDim = m_pScrollUp->GetDimensions( );
	float *fScrollBarDim = m_pScrollBar->GetDimensions( );

	m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_LINE_LOOP );

	// draw the memo box
	m_pParentWindow->GetScheme( )->DrawShape
	(
		fInheritedDim[0] + m_fDimensions[0],
		fInheritedDim[1] + m_fDimensions[1],
		m_fDimensions[2],
		m_fDimensions[3]
	);

	// Grey for the scrollbar.
	m_pParentWindow->SetScheme( Colors::fGrey, Colors::fGrey, S_RECT );

	// draw the scrollbar grey background
	m_pParentWindow->GetScheme( )->DrawShape
	(
		fInheritedDim[0] + m_fDimensions[0] + m_fDimensions[2] - 4 - fScrollUpDim[2],
		fInheritedDim[1] + m_fDimensions[1] + 1,
		fScrollUpDim[2] + 2,
		m_fDimensions[3] - 2
	);

	m_pParentWindow->ResetScheme( );

	if( m_iCurVisibleLines < m_iMaxVisLines )
		m_iCurVisibleLines = m_iNumLines - 1;

	// draw the scroll buttons
	m_pScrollDown->Draw( );
	m_pScrollUp->Draw( );
	m_pScrollBar->Draw( );

	// If they press page down, scroll the memo down.
	if( Gui.GetKey( K_PGDN ) && m_pParentWindow == Gui.GetActiveWin( ) )
	{
		if( m_iCurVisibleLines < m_iNumLines - 1 )
		{
			m_iCurVisibleLines++;
			m_bUpdateScrollBarPos = true;
		}
	}

	// If they press page up, scroll the memo up.
	if( Gui.GetKey( K_PGUP ) && m_pParentWindow == Gui.GetActiveWin( ) )
	{
		if( m_iCurVisibleLines > m_iMaxVisLines )
		{
			m_iCurVisibleLines--;
			m_bUpdateScrollBarPos = true;
		}
	}

	if( m_bUpdateScrollBarPos )
	{
		m_pScrollBar->UpdatePosition( m_iNumLines, m_iCurVisibleLines, m_iMaxVisLines );
		m_bUpdateScrollBarPos = false;
	}

	// Loop through the strings, and print.
	for( int i = m_iCurVisibleLines; i >= ( m_iCurVisibleLines - m_iMaxVisLines < 0 ? 0 : m_iCurVisibleLines - m_iMaxVisLines ); i-- )
		Gui.GetFont( )->DrawString
		(
			fInheritedDim[0] + m_fDimensions[0] + 3,
			fInheritedDim[1] + m_fDimensions[1] + m_fDimensions[3] - ( ( m_iCurVisibleLines - i ) * ( Gui.GetFont( )->GetFontHeight( ) - 2 ) ) - 4,
			Colors::fWhite,
			FL_COLTAGS,
			S_COLOR_WHITE "%s",
			m_vLines[ i ].GetBuffer( )
		);
}

inline int CMemo::GetNumLines( void )
{
	return( m_iNumLines );
}

inline CScrollBar *CMemo::GetScrollBar( void )
{
	return( m_pScrollBar );
}

inline void CMemo::ClearLines( void )
{
	m_vLines.clear( );
}

// This func uses basic truncation (adds ".." to the end)
void CMemo::PushLine( char *szBuf )
{
	bool bTrunc = false;
	char *szTmpString = new char [256];

	memset( szTmpString, '\0', sizeof( szTmpString ) );
	strncpy( szTmpString, szBuf, 256 );

	while( Gui.GetFont( )->GetStringLength( szTmpString, false ) >= m_fDimensions[2] - 22 )
	{
		szTmpString[ strlen( szTmpString ) - 1 ] = '\0';
		bTrunc = true;
	}

	if( bTrunc )
		strcat( szTmpString, ".." );

	line_t *pTmpLine = new line_t;

	strcpy( pTmpLine->szLineBuffer, szTmpString );

	m_vLines.push_back( *pTmpLine );

	delete pTmpLine;

	m_iNumLines = ( int )m_vLines.size( ); // update number of lines

	m_iCurVisibleLines = m_iNumLines - 1; // scroll to bottom

	m_pScrollBar->UpdateSize( m_iNumLines, m_iMaxVisLines ); // update size of scrollbar
}

// This func parses a string, sorts it into numerous lines, and pushes them onto the line buffer.
void CMemo::PushTruncLine( const char *szBuf )
{
	int i = 0, iLineLen = 0;
	int iCurTruncWords = 0, iPrevTruncWords = 0;
	int iLineNum = 0, iNumWords = 0;

	float *fInheritedDim;

	CParser::line_t *szLines;
	CParser::word_t *szWords;

	iNumWords = CParser::GetNumWords( ( char * )szBuf );

	if( !iNumWords ) // no words, dont bother.
		return;

	fInheritedDim = m_pParentWindow->GetDimensions( );

	szLines = new CParser::line_t [16]; // I can't think of a way of figuring out how to do this without another while( ) loop, so 16 is max.
	szWords = new CParser::word_t [iNumWords];

	memset( szLines, '\0', sizeof( CParser::line_t ) * 16 );
	memset( szWords, '\0', sizeof( CParser::word_t ) * iNumWords );

	CParser::GetWords( szBuf, iNumWords, szWords ); // get the words in szBuf, store them seperately in szWords.

	strcpy( szLines[0], szBuf ); // Why did i do this? leave anyway.

	while( 1 )
	{
		iCurTruncWords = 0;

		// While the length of the string is too long to fit in the memo.
		while( Gui.GetFont( )->GetStringLength( szLines[ iLineNum ], true ) + 2 >= m_fDimensions[2] - 13 )
		{
			iCurTruncWords++;

            iLineLen = ( int )strlen( szLines[ iLineNum ] ) - 1;

			szLines[ iLineNum ]
			[ 
				iLineLen - strlen( szWords[ iNumWords - ( iPrevTruncWords + iCurTruncWords ) ] )
			] = '\0';
		}
		// line has been truncated enough...

		if( iCurTruncWords < 1 )
			break; // no words were truncated this line, so no more truncation left to do.

		for( i = 0; i < iCurTruncWords; i++ ) // loop through the truncated words, adding them to the next string.
			strcat( szLines[ iLineNum + 1 ], AppendStr( "%s ", szWords[ ( iNumWords - ( iPrevTruncWords + iCurTruncWords ) ) + i ] ) );

		iLineLen = ( int )strlen( szLines[ iLineNum + 1 ] );

		szLines[ iLineNum + 1 ][ iLineLen - 1 ] = '\0'; // get rid of the space at the end.

		iPrevTruncWords = iCurTruncWords;

		iLineNum++; // increase the line num
	}

	// push the lines onto the line buffer.
	for( i = 0; i <= iLineNum; i++ )
	{
		line_t *pTmpLine = new line_t;

		strcpy( pTmpLine->szLineBuffer, szLines[ i ] );

		m_vLines.push_back( *pTmpLine );

		delete pTmpLine;
	}

	delete [] szWords;
	delete [] szLines;

	m_iNumLines = ( int )m_vLines.size( ); // update number of lines

	m_iCurVisibleLines = m_iNumLines - 1; // scroll to bottom

	m_pScrollBar->UpdateSize( m_iNumLines, m_iMaxVisLines ); // update size of scrollbar
}

// this adds on m_iMaxVisLines for the scrollbar, don't call it elsewhere
inline void CMemo::SetVisibleLines( int iVisibleLines, bool bUpdateScrollBar )
{
	if( m_iMaxVisLines + iVisibleLines < m_iNumLines - 1 )
		m_iCurVisibleLines = m_iMaxVisLines + iVisibleLines;
	else
		m_iCurVisibleLines = m_iNumLines - 1;

	if( bUpdateScrollBar )
		m_pScrollBar->UpdatePosition( m_iNumLines, m_iCurVisibleLines, m_iMaxVisLines );
}

// this func is special because we must call it after the gui font is inited.
inline void CMemo::UpdateMaxVisLines( void )
{
	if( !m_iMaxVisLines ) // Only init Max vis lines once, since the memo isn't resizable.
		m_iMaxVisLines = ( m_fDimensions[3] / Gui.GetFont( )->GetFontHeight( ) ) + ( ( m_fDimensions[3] / Gui.GetFont( )->GetFontHeight( ) ) / 8 ); // This formula is accurate in my 2 experiences
}

CEditBox::CEditBox( void ) : CObject( )
{
	m_iLongestCaptionLen = 1;
	m_bEditable = false;
	m_bListClick = false;
	this->fnCallback = 0;

	memset( m_szInput, '\0', sizeof( m_szInput ) );
	memset( m_szInputDisplay, '\0', sizeof( m_szInput ) );
}

CEditBox::CEditBox( float x, float y, float fWidth, float fHeight, void ( *pFunction )( CEditBox * ), CWindow *pParentWindow, ObjType_t ObjType ) : CObject( x, y, fWidth, fHeight, pParentWindow, ObjType )
{
	m_iLongestCaptionLen = 1;
	m_bEditable = false;
	m_bListClick = false;
	this->fnCallback = pFunction;

	memset( m_szInput, '\0', sizeof( m_szInput ) );
	memset( m_szInputDisplay, '\0', sizeof( m_szInput ) );
}

CEditBox::~CEditBox( void )
{
}

// this function checks to see if a cvar/command has been typed.
void CEditBox::CheckForCmd( char *szInput, CmdOwner_t iCmdOwner )
{
	char *szCaption;
	int i = 0, j = 0;

	for( ; i < ( int )m_vInputList.size( ); i++ )
	{
		szCaption = m_vInputList[ i ]->GetCaption( );

		// double check.
		if( ( strstr( szInput, szCaption ) == NULL ) )
			continue;

		// TODO: loop through iCmdOwner, for all windows blabla
		if( iCmdOwner == CMD_CONSOLE ) // This is the Console input box.
		{
			for( j = 0; j < UI_VARTABLE_SIZE; j++ )
			{
				if( !strcmp( UI.UIVarTable[ j ].szCvar, szCaption ) )
				{
					UI.UIVarTable[ j ].fnCallBack( szInput, &UI );
					return;
				}
			}
		}
	}
}

void CEditBox::Draw( void )
{
	bool bLockEdit = false, bTruncated = false;
	float *fInheritedDim = m_pParentWindow->GetDimensions( );
    int i;

	if( ( Gui.fMouse[0] >= fInheritedDim[0] + m_fDimensions[0] && Gui.fMouse[0] <= fInheritedDim[0] + m_fDimensions[0] + m_fDimensions[2] && Gui.fMouse[1] >= fInheritedDim[1] + m_fDimensions[1] && Gui.fMouse[1] <= fInheritedDim[1] + m_fDimensions[1] + m_fDimensions[3] ) || m_bListClick )
	{
		m_bListClick = false;
		this->fnCallback( this );
	}
	else if( Gui.GetKey( K_MOUSE1 ) )
		bLockEdit = true;

	m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_LINE_LOOP );

	m_pParentWindow->GetScheme( )->DrawShape
	( 
		fInheritedDim[0] + m_fDimensions[0],
		fInheritedDim[1] + m_fDimensions[1], 
		m_fDimensions[2], 
		m_fDimensions[3] 
	);

	m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), Colors::fGrey, S_RECT );

	m_pParentWindow->GetScheme( )->DrawShape
	( 
		fInheritedDim[0] + m_fDimensions[0], 
		fInheritedDim[1] + m_fDimensions[1], 
		m_fDimensions[2], 
		m_fDimensions[3] 
	);

	m_pParentWindow->ResetScheme( );

	// If this edit box is active, and the gui is active...
	if( m_bEditable && Gui.GetActiveState( ) == 1 )
	{
		// loop through the keys.
		for( i = 0; i < 256; i++ ) // TODO: this is inefficient, search the SDK for a similar loop and intercept.
		{
			if( !Gui.GetKey( i ) )
				continue;
			else
			{
				switch( i )
				{
				case K_ENTER:
				case KP_ENTER:
					if( m_szInput[0] )
					{
						//if( strstr( m_pParentWindow->GetTitle( ), "Console" ) ) // TODO: HACK: uncomment these lines if > 1 editbox.
						//{
							this->CheckForCmd( m_szInput, CMD_CONSOLE );

							if( m_szInput[0] != '/' ) // Only send the message if it hasn't a / infront.
								UI.ProcessMessage( m_szInput );
						//}
					}

					memset( m_szInput, '\0', sizeof( m_szInput ) ); // input has been stored, now clear it.
					break;

				case K_DEL:
				case K_BACKSPACE:
					if( m_szInput[0] )
						m_szInput[ strlen( m_szInput ) - 1 ] = '\0';
					break;

				default:
					if( isprint( i ) && strlen( m_szInput ) < sizeof( m_szInput ) - 1 )
						m_szInput[ strlen( m_szInput ) ] = i;
					break;
				}
			}

			if( i != K_PGDN && i != K_PGUP && i != K_MOUSE1 ) // allow these to be used elsewhere
				Gui.SetKey( i, false );
		}

		memcpy( m_szInputDisplay, m_szInput, sizeof( m_szInputDisplay ) );

		while( Gui.GetFont( )->GetStringLength( m_szInputDisplay, false ) + 1 >= m_fDimensions[2] - 4 /*-4 to be safe*/ )
			memcpy( m_szInputDisplay, m_szInputDisplay + 1, sizeof( m_szInputDisplay ) - 1 ); 
		// stop the input box text from drawing over limits.

		Gui.GetFont( )->DrawString
		( 
			fInheritedDim[0] + m_fDimensions[0] + 3,
			fInheritedDim[1] + m_fDimensions[1] + Gui.GetFont( )->GetFontHeight( ) - 2,
			Colors::fWhite,
			FL_COLTAGS,
			S_COLOR_WHITE "%s" S_COLOR_BLACK "_",
			m_szInputDisplay
		);

		if( m_szInputDisplay[0] == '/' )
			this->DrawListBox( );
	}
	else
		Gui.GetFont( )->DrawString
		(
			fInheritedDim[0] + m_fDimensions[0] + 3,
			fInheritedDim[1] + m_fDimensions[1] + Gui.GetFont( )->GetFontHeight( ) - 2,
			Colors::fTransBlack,
			FL_NONE,
			"%s",
			( !m_szInputDisplay[0] ? m_szInput : m_szInputDisplay )
		); // draw in grey, as the box isnt active.

	if( bLockEdit )
		m_bEditable = false;
}

// Draw a list of Cvars.
void CEditBox::DrawListBox( void )
{
	int iCounter = 0, i = 0;
	float *fInheritedDim = m_pParentWindow->GetDimensions( ), *fInputDim;

	for( ; i < ( int )m_vInputList.size( ); i++ )
	{
		if( !strstr( m_vInputList[ i ]->GetCaption( ), m_szInput ) )
			continue;

		m_vInputList[ i ]->SetDimensions
		(
			0,
			fInheritedDim[3] + 2 + ( iCounter * FONT_SIZE + 1 ),
			m_iLongestCaptionLen + 6,
			FONT_SIZE + 1
		);

		iCounter++;

		fInputDim = m_vInputList[ i ]->GetDimensions( );

		if( Gui.fMouse[0] >= fInheritedDim[0] + fInputDim[0] && Gui.fMouse[0] <= fInheritedDim[0] + fInputDim[0] + fInputDim[2] && Gui.fMouse[1] >= fInheritedDim[1] + fInputDim[1] && Gui.fMouse[1] <= fInheritedDim[1] + fInputDim[1] + fInputDim[3] )
		{
			if( Gui.GetKey( K_MOUSE1 ) )
			{
				Gui.SetKey( K_MOUSE1, false );
				strcpy( m_szInput, m_vInputList[ i ]->GetCaption( ) );
				m_bListClick = true;
				return;
			}
		}

		m_vInputList[ i ]->Draw( );
	}
}

// pushes a cvar/command onto the input list.
void CEditBox::PushInput( CButton *pObject, char *szCaption )
{
	int iStrLen;

	*pObject = CButton( 0, 0, 0, 0, &Fake, szCaption, false, false, m_pParentWindow, NULL );

	iStrLen = Gui.GetFont( )->GetStringLength( szCaption, false /*what kind of idiot would put color tags in a cvar?*/ );

	if( iStrLen > m_iLongestCaptionLen )
		m_iLongestCaptionLen = iStrLen;

	m_vInputList.push_back( pObject );
}

inline void CEditBox::SetEditable( bool bState )
{
	m_bEditable = bState;
}

inline void CEditBox::SetMemo( CMemo *pMemo )
{
	m_pMemo = pMemo;
}

CButton::CButton( void ) : CObject( )
{
	m_bCenterTitle = false;
	m_bShadow = false;
	this->fnCallback = 0;
	this->m_pCallback = 0;
	memset( m_szCaption, 0, sizeof( m_szCaption ) );
}

CButton::CButton( float x, float y, float fWidth, float fHeight, void ( *pFunction )( CButton *, void * ), char *szCaption, bool bShadow, bool bCenterTitle, CWindow *pParentWindow, void *pCallback, ObjType_t ObjType ) : CObject( x, y, fWidth, fHeight, pParentWindow, ObjType )
{
	m_bCenterTitle = bCenterTitle;
	m_bShadow = bShadow;
	this->fnCallback = pFunction;
	this->m_pCallback = pCallback;
	memcpy( m_szCaption, szCaption, sizeof( m_szCaption ) );
}

CButton::~CButton( void )
{
}

void CButton::Draw( void )
{
	float *fInheritedDim = m_pParentWindow->GetDimensions( );

	if( ( this == m_pParentWindow->GetObjectPtr( OBJ_TITLEBAR ) && m_pParentWindow->GetDragging( ) ) 
	|| Gui.fMouse[0] >= fInheritedDim[0] + m_fDimensions[0] && Gui.fMouse[0] <= fInheritedDim[0] + m_fDimensions[0] + m_fDimensions[2] 
	&& Gui.fMouse[1] >= fInheritedDim[1] + m_fDimensions[1] && Gui.fMouse[1] <= fInheritedDim[1] + m_fDimensions[1] + m_fDimensions[3] )
		this->fnCallback( this, m_pCallback );

	m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_LINE_LOOP );

	m_pParentWindow->GetScheme( )->DrawShape
	( 
		fInheritedDim[0] + m_fDimensions[0], 
		fInheritedDim[1] + m_fDimensions[1], 
		m_fDimensions[2], 
		m_fDimensions[3]
	);

	m_pParentWindow->ResetScheme( );

	if( m_bShadow )
	{
		m_pParentWindow->SetScheme( Colors::fBlack, m_pParentWindow->GetScheme( )->GetColor( ), S_LINE );

		m_pParentWindow->GetScheme( )->DrawShape
		(
			fInheritedDim[0] + m_fDimensions[0] + m_fDimensions[2] + 2,
			fInheritedDim[1] + m_fDimensions[1],
			1,
			m_fDimensions[3] + 1
		);

		m_pParentWindow->GetScheme( )->DrawShape
		( 
			fInheritedDim[0] + m_fDimensions[0] + 1,
			fInheritedDim[1] + m_fDimensions[1] + m_fDimensions[3] + 1,
			m_fDimensions[2] + 1,
			1
		);

		m_pParentWindow->ResetScheme( );
	}

	switch( m_ObjType )
	{
	case OBJ_TITLEBAR:
		m_pParentWindow->SetScheme( Colors::fGrey, Colors::fGrey, S_LINE );

		m_pParentWindow->GetScheme( )->DrawShape
		( 
			fInheritedDim[0] + m_fDimensions[0] + 1,
			fInheritedDim[1] + m_fDimensions[1],
			1,
			m_fDimensions[3]
		);

		m_pParentWindow->GetScheme( )->DrawShape
		(
			fInheritedDim[0] + m_fDimensions[0] + 1,
			fInheritedDim[1] + m_fDimensions[1],
			m_fDimensions[2],
			1
		);

		m_pParentWindow->ResetScheme( );
		break;
	
	case OBJ_MINIMIZE:
		m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_RECT );

		m_pParentWindow->GetScheme( )->DrawShape
		(
			fInheritedDim[0] + m_fDimensions[0] + 2,
			fInheritedDim[1] + m_fDimensions[1] + 2,
			m_fDimensions[2] - 4,
			m_fDimensions[3] - 4
		);

		m_pParentWindow->ResetScheme( );
		return; // no need to call drawstring func on a minimize box.

	case OBJ_SCROLLDOWN:
		m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_TRI1 );

		m_pParentWindow->GetScheme( )->DrawShape
		( 
			fInheritedDim[0] + m_fDimensions[0] + 2,
			fInheritedDim[1] + m_fDimensions[1] + 2,
			m_fDimensions[2] - 3,
			m_fDimensions[3] - 4
		);

		m_pParentWindow->ResetScheme( );
		return; // no need to call drawstring on a scrolldown button

	case OBJ_SCROLLUP:
		m_pParentWindow->SetScheme( m_pParentWindow->GetScheme( )->GetBorderColor( ), m_pParentWindow->GetScheme( )->GetColor( ), S_TRI1 );

		m_pParentWindow->GetScheme( )->DrawShape
		( 
			fInheritedDim[0] + m_fDimensions[0] + 2,
			fInheritedDim[1] + m_fDimensions[1] + m_fDimensions[3] - 2,
			m_fDimensions[2] - 3,
			-( m_fDimensions[3] - 3 )
		);

		m_pParentWindow->ResetScheme( );
		return; // no need to call drawstring on a scrollup button
	}

	Gui.GetFont( )->DrawString
	( 
		( m_bCenterTitle ? fInheritedDim[0] + m_fDimensions[0] + ( m_fDimensions[2] / 2 ) : fInheritedDim[0] + m_fDimensions[0] + 3 ),
		( m_bCenterTitle ? fInheritedDim[1] + m_fDimensions[1] + ( m_fDimensions[3] / 2 ) : fInheritedDim[1] + m_fDimensions[1] + Gui.GetFont( )->GetFontHeight( ) - 3 ),
		m_pParentWindow->GetScheme( )->GetBorderColor( ), 
		( m_bCenterTitle ? FL_CENTERX | FL_CENTERY | FL_COLTAGS : FL_COLTAGS ), 
		S_COLOR_WHITE "%s", 
		m_szCaption
	);
}

inline char *CButton::GetCaption( void )
{
	return( m_szCaption );
}

inline bool CButton::GetClick( void )
{
	return( m_bClick );
}

inline void CButton::SetCaption( char *szNewCaption )
{
	strcpy( m_szCaption, szNewCaption );
}

CWindow::CWindow( void ) : CTemplate( )
{
	m_iWinNum = 0;
	m_bMaximized = false;
	m_bDragging = false;
	m_bBackground = true;

	m_pScheme = new CDrawScheme;
	m_pTitleBar = new CButton;
	m_pMinBox = new CButton;

	strcpy( m_szTitle, "Window_Default" );
}

CWindow::CWindow( float x, float y, float fWidth, float fHeight, char *szTitle, bool bMaximized, int iWinNum, bool bBackground ) : CTemplate( x, y, fWidth, fHeight )
{
	m_iWinNum = iWinNum;
	m_bMaximized = bMaximized;
	m_bDragging = false;
	m_bBackground = bBackground;

	m_pScheme = new CDrawScheme( "Window", Gui.GetScheme( )->GetBorderColor( ), Gui.GetScheme( )->GetColor( ), ( m_bBackground ? Gui.GetScheme( )->GetType( ) : S_LINE_LOOP ) );
	m_pTitleBar = new CButton;
	m_pMinBox = new CButton;

	strcpy( m_szTitle, szTitle );

	this->PushObject( m_pTitleBar, 0, 0, fWidth, FONT_SIZE, CWindow::fnTitleBar, m_szTitle, false, false, NULL, OBJ_TITLEBAR );

	this->PushObject( m_pMinBox, fWidth - 12, 3, 9, FONT_SIZE - 5, CWindow::fnMinimize, "", false, false, NULL, OBJ_MINIMIZE );
	/*co-ordinates are inherited from their parent: x +, y +*/
}

CWindow::~CWindow( void )
{
}

inline void CWindow::DestroyObjects( void )
{
	delete m_pTitleBar;
	delete m_pMinBox;
	delete m_pScheme;
}

void CWindow::Draw( void )
{
	int i = 0;

	for( ; i < UI_WINTABLE_SIZE; i++ )
	{
		if( this == UI.UIVarTable[ i ].pWindow && UI.UIVarTable[ i ].szValue[0] == '0' ) // strings = enabled (winamp spec.)
			return;
	}

	if( !m_bMaximized )
	{
		m_pTitleBar->Draw( );
		m_pMinBox->Draw( );
		return;
	}

	this->ResetScheme( );

	m_pScheme->DrawShape
	( 
		m_fDimensions[0],
		m_fDimensions[1],
		m_fDimensions[2],
		m_fDimensions[3]
	);

	for( i = 0; i < ( int )m_vObjects.size( ); i++ )
	{
		if( m_vObjects[ i ]->GetObjectType( ) >= OBJ_SCROLLUP && m_vObjects[ i ]->GetObjectType( ) <= OBJ_SCROLLBAR )
			continue; // these get drawn seperately on the top of the grey

		m_vObjects[ i ]->Draw( );
	}
}

// call this when a minimize button is clicked.
void CWindow::fnMinimize( CButton *pThis, void *pCallback )
{
	int i = 0;

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )pThis->GetParent( ) ) // Check that a window isn't being dragged, and check if it is ours
			return;														// because the minbox is on the titlebar, so is clicked at same time.

		pThis->GetParent( )->SetProperties( !pThis->GetParent( )->GetMaximized( ) );
		Gui.SetKey( K_MOUSE1, false );
	}
}

// call this when a titlebar is clicked.
void CWindow::fnTitleBar( CButton *pThis, void *pCallback )
{
	int i = 0;
	float *fParentDim = pThis->GetParent( )->GetDimensions( );

	float fDimX = 0.0f, fDimY = 0.0f;

	if( Gui.GetKey( K_MOUSE1 ) )
	{
		if( Gui.GetDraggingWin( ) != ( CWindow * )-1 ) // A window is being dragged.
		{
			if( Gui.GetDraggingWin( ) != pThis->GetParent( ) ) // not our window.
				return;
		}

		Gui.SetDraggingWin( pThis->GetParent( ) );

		pThis->GetParent( )->SetDragging( true );

		// This bit of code is messy, what its doing is checking if the place we are trying to drag to is out of the screen.
		// For the left / top side of the screen, just check if its <= 0, but for the bottom side of the screen it has to
		// check whether the window is maximized, and if it is, don't allow the bottom corners out of the screen, if it isn't, just
		// then don't worry, since the person doesn't need to view the window. Also the code stops the mouse from being moved if
		// the window isn't being moved at the same time (it's a half-assed attempt but it sorta works).

        if( fParentDim[0] + Gui.fMouse[0] - Gui.fLastMouse[0] <= 0 )
		{
			Gui.fMouse[0] = Gui.fLastMouse[0];
			fDimX = 1;
		}
		else if( ( pThis->GetParent( )->GetMaximized( ) ? fParentDim[0] + fParentDim[2] : fParentDim[0] ) + Gui.fMouse[0] - Gui.fLastMouse[0] >= pCgs->vidWidth )
		{
			Gui.fMouse[0] -= Gui.fMouse[0] - Gui.fLastMouse[0];
			fDimX = ( pThis->GetParent( )->GetMaximized( ) ? pCgs->vidWidth - fParentDim[2] : pCgs->vidWidth );
		}

		if( fParentDim[1] + Gui.fMouse[1] - Gui.fLastMouse[1] <= 0 )
		{
			Gui.fMouse[1] = Gui.fLastMouse[1];
			fDimY = 1;
		}
		else if( ( pThis->GetParent( )->GetMaximized( ) ? fParentDim[1] + fParentDim[3] : fParentDim[1] )  + Gui.fMouse[1] - Gui.fLastMouse[1] >= pCgs->vidHeight )
		{
			Gui.fMouse[1] -= Gui.fMouse[1] - Gui.fLastMouse[1];
			fDimY = ( pThis->GetParent( )->GetMaximized( ) ? pCgs->vidHeight - fParentDim[3] : pCgs->vidHeight );
		}

		pThis->GetParent( )->SetDimensions
		( 
			( fDimX ? fDimX : fParentDim[0] + Gui.fMouse[0] - Gui.fLastMouse[0] ),
			( fDimY ? fDimY : fParentDim[1] + Gui.fMouse[1] - Gui.fLastMouse[1] ),
			fParentDim[2],
			fParentDim[3]
		);

		Gui.SetActiveWin( pThis->GetParent( ) );
		Gui.SetActiveRendered( true ); 
		// the window has been rendered, this is to prevent it re-rendering in GuiDrawWindows (due to it being the active window).
	}
	else
	{
		Gui.SetDraggingWin( ( CWindow * )-1 );
		pThis->GetParent( )->SetDragging( false );
	}
}

inline void CWindow::ResetScheme( void )
{
	this->SetScheme( Gui.GetScheme( )->GetBorderColor( ), Gui.GetScheme( )->GetColor( ), ( m_bBackground ? Gui.GetScheme( )->GetType( ) : S_LINE_LOOP ) );
}

inline void CWindow::SetDragging( bool bState )
{
	m_bDragging = bState;
}

inline void CWindow::SetProperties( bool bMaximized )
{
	m_bMaximized = bMaximized;
}

void CWindow::SetScheme( float *fBorderCol, float *fColor, int iType )
{
	m_pScheme->SetBorderColor( fBorderCol );
	m_pScheme->SetColor( fColor );
	m_pScheme->SetType( iType );
}

// The following four functions push an object onto the window's object list, depending on what object it is.

void CWindow::PushObject( CObject *pObject, float x, float y, float fWidth, float fHeight, ObjType_t ObjType )
{
	*pObject = CObject( x, y, fWidth, fHeight, Gui.GetWindow( m_iWinNum ), ObjType );

	m_vObjects.push_back( pObject );
}

void CWindow::PushObject( CButton *pObject, float x, float y, float fWidth, float fHeight, void ( *pFunction )( CButton *, void * ), char *szCaption, bool bShadow, bool bCenterTitle, void *pCallback, ObjType_t ObjType )
{
	*pObject = CButton( x, y, fWidth, fHeight, pFunction, szCaption, bShadow, bCenterTitle, Gui.GetWindow( m_iWinNum ), pCallback, ObjType );

	switch( m_vObjects.size( ) )
	{
	case OBJ_TITLEBAR:
		m_pTitleBar = pObject;
		break;

	case OBJ_MINIMIZE:
		m_pMinBox = pObject;
		break;
	}

	m_vObjects.push_back( pObject );
}

void CWindow::PushObject( CScrollBar *pObject, float x, float y, float fWidth, float fHeight, void ( *pFunction )( CScrollBar * ), float fMinYOffset, float fMaxYOffset )
{
	*pObject = CScrollBar( x, y, fWidth, fHeight, pFunction, Gui.GetWindow( m_iWinNum ), fMinYOffset, fMaxYOffset );

	m_vObjects.push_back( pObject );
}

void CWindow::PushObject( CEditBox *pObject, float x, float y, float fWidth, float fHeight, void ( *pFunction )( CEditBox * ), int iMemoHeight, CMemo *pMemo )
{
	*pObject = CEditBox( x, y, fWidth, fHeight, pFunction, Gui.GetWindow( m_iWinNum ) );

	m_vObjects.push_back( pObject );

	*pMemo = CMemo( x, y - iMemoHeight - 2, fWidth, iMemoHeight, Gui.GetWindow( m_iWinNum ), pMemo );

	m_vObjects.push_back( pMemo );

	pObject->SetMemo( pMemo );

	pMemo->GetScrollBar( )->SetMemo( pMemo );
	pMemo->UpdateMaxVisLines( );
}

// this is for memo's without an editbox, rarely used but still sometimes, e.g. winamp.
void CWindow::PushObject( CMemo *pMemo, float x, float y, float fWidth, float fHeight )
{
	*pMemo = CMemo( x, y, fWidth, fHeight, Gui.GetWindow( m_iWinNum ), pMemo );

	m_vObjects.push_back( pMemo );

	pMemo->GetScrollBar( )->SetMemo( pMemo );
	pMemo->UpdateMaxVisLines( );
}

inline void CWindow::ClearObjects( void )
{
	m_vObjects.clear( );

	m_vObjects.push_back( m_pTitleBar );
	m_vObjects.push_back( m_pMinBox );
}

inline bool CWindow::GetDragging( void )
{
	return( m_bDragging );
}

inline bool CWindow::GetMaximized( void )
{
	return( m_bMaximized );
}

inline CDrawScheme *CWindow::GetScheme( void )
{
	return( m_pScheme );
}

inline char *CWindow::GetTitle( void )
{
	return( m_szTitle );
}

inline CObject *CWindow::GetObjectPtr( int iNum )
{
	switch( iNum )
	{
	case OBJ_TITLEBAR:
		return( m_pTitleBar );

	case OBJ_MINIMIZE:
		return( m_pMinBox );

	default:
		break;
	}

	return( m_vObjects[ iNum ] );
}

inline size_t CWindow::GetObjectListSize( void )
{
	return( m_vObjects.size( ) );
}

CDrawScheme::CDrawScheme( void )
{
	memset( m_fBorderCol, 0, sizeof( m_fBorderCol ) );
	memset( m_fColor, 0, sizeof( m_fColor ) );
	memset( m_fOrigBorderCol, 0, sizeof( m_fBorderCol ) );
	memset( m_fOrigColor, 0, sizeof( m_fColor ) );

	strcpy( m_szSchemeName, "Default Scheme" );
	m_iType = 0;
	m_bBackupColor = false;
}

CDrawScheme::CDrawScheme( char *szSchemeName, float *fBorderCol, float *fColor, int iType )
{
	memcpy( m_fBorderCol, fBorderCol, sizeof( m_fBorderCol ) );
	memcpy( m_fColor, fColor, sizeof( m_fColor ) );

	memcpy( m_fOrigBorderCol, fBorderCol, sizeof( m_fBorderCol ) );
	memcpy( m_fOrigColor, fColor, sizeof( m_fColor ) );

	sprintf( m_szSchemeName, "%s Scheme", szSchemeName );
	m_iType = iType;
	m_bBackupColor = false;
}

CDrawScheme::~CDrawScheme( void )
{
}

// Draws a 2d shape.
void CDrawScheme::DrawShape( float x, float y, float fWidth, float fHeight )
{
	static float fFrame = 0;

	//pglGetIntegerv
	pglEnable( GL_BLEND );
	pglBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	pglDisable( GL_TEXTURE_2D );

	switch( m_iType )
	{
	case S_LINE:
		pglColor4fv( m_fBorderCol );
		pglBegin( GL_LINES );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y + fHeight );
		pglEnd( );
		break;

	case S_LINE_LOOP:
		pglColor4fv( m_fBorderCol );
		pglBegin( GL_LINE_LOOP );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y );
		pglVertex2f( x + fWidth, y + fHeight );
		pglVertex2f( x, y + fHeight );
		pglEnd( );
		break;

	case S_RECT:
		pglColor4fv( m_fColor );
		pglBegin( GL_QUADS );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y );
		pglVertex2f( x + fWidth, y + fHeight );
		pglVertex2f( x, y + fHeight );
		pglEnd( );

		pglColor4fv( m_fBorderCol );
		pglBegin( GL_LINE_LOOP );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y );
		pglVertex2f( x + fWidth, y + fHeight );
		pglVertex2f( x, y + fHeight );
		pglEnd( );
		break;

	case S_TRI1:
		pglColor4fv( m_fColor );
		pglBegin( GL_TRIANGLES );
		pglVertex2f( x + ( fWidth / 2 ), y + fHeight );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y );
		pglEnd( );

		pglColor4fv( m_fBorderCol );
		pglBegin( GL_LINES );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y );

		pglVertex2f( x + fWidth, y );
		pglVertex2f( x + ( fWidth / 2 ), y + fHeight );

		pglVertex2f( x + ( fWidth / 2 ), y + fHeight );
		pglVertex2f( x, y );
		pglEnd( );
		break;

	case S_TRI2:
		pglColor4fv( m_fColor );
		pglBegin( GL_TRIANGLES );
		pglVertex2f( x, y + fHeight );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y );
		pglEnd( );

		pglColor4fv( m_fBorderCol );
		pglBegin( GL_LINES );
		pglVertex2f( x, y );
		pglVertex2f( x + fWidth, y );

		pglVertex2f( x + fWidth, y );
		pglVertex2f( x, y + fHeight );

		pglVertex2f( x, y + fHeight );
		pglVertex2f( x, y );
		pglEnd( );
		break;
	}

	pglEnable( GL_TEXTURE_2D );
}

inline float *CDrawScheme::GetBorderColor( void )
{
	return( m_bBackupColor ? m_fOrigBorderCol : m_fBorderCol );
}

inline float *CDrawScheme::GetColor( void )
{
	return( m_bBackupColor ? m_fOrigColor : m_fColor );
}

inline int CDrawScheme::GetType( void )
{
	return( m_iType );
}

inline char *CDrawScheme::GetSchemeName( void )
{
	return( m_szSchemeName );
}

inline void CDrawScheme::SetBackup( bool bBackup )
{
	m_bBackupColor = bBackup;
}

inline void CDrawScheme::SetBorderColor( float *fBorderCol )
{
	Vector4Copy( fBorderCol, m_fBorderCol );
}

inline void CDrawScheme::SetColor( float *fColor )
{
	Vector4Copy( fColor, m_fColor );
}

inline void CDrawScheme::SetType( int iType )
{
	m_iType = iType;
}

CGui::CGui( void )
{
	m_pFont = new CFont;

	this->fMouse[0] = 200;
	this->fMouse[1] = 200;

	m_iActive = 0;
	m_cActivationKey = '*';
	m_bInited = false;
	m_pDraggingWin = ( CWindow * )-1;

	m_SchemeList[0] = &gBlueScheme;
	m_SchemeList[1] = &gOrangeScheme;
	m_SchemeList[2] = &gGreyScheme;
	m_SchemeList[3] = &gYellowScheme;

	m_SchemeButton[0] = &gBlueButton;
	m_SchemeButton[1] = &gOrangeButton;
	m_SchemeButton[2] = &gGreyButton;
	m_SchemeButton[3] = &gYellowButton;
}

CGui::CGui( char cActivationKey, char *szFontName, int iFontSize, CDrawScheme *pScheme )
{
	m_pFont = new CFont( szFontName, iFontSize, FW_BOLD );
	m_pScheme = pScheme;

	this->fMouse[0] = 200;
	this->fMouse[1] = 200;

	m_iActive = 0;
	m_cActivationKey = cActivationKey;
	m_bInited = false;
	m_pDraggingWin = ( CWindow * )-1;

	// TODO: make a table at the top with all schemes (also see CGui::CGui( ))
	m_SchemeList[0] = &gBlueScheme;
	m_SchemeList[1] = &gOrangeScheme;
	m_SchemeList[2] = &gGreyScheme;
	m_SchemeList[3] = &gYellowScheme;

	m_SchemeButton[0] = &gBlueButton;
	m_SchemeButton[1] = &gOrangeButton;
	m_SchemeButton[2] = &gGreyButton;
	m_SchemeButton[3] = &gYellowButton;
}

// clean up all the allocated memory.
CGui::~CGui( void )
{
	delete m_pFont, m_pScheme; // might be unnecessary

	m_bInited = false;
	m_iActive = 0;
	m_pDraggingWin = ( CWindow * )-1;
    
	this->ConsoleWnd.DestroyObjects( );
	this->WinampWnd.DestroyObjects( );
	this->WinampPLWnd.DestroyObjects( );

	this->ConsoleMemo.DestroyObjects( );
	this->WinampPLMemo.DestroyObjects( );
}

void CGui::Init( void )
{
	int i = 0;

	if( m_bInited )
		return;

	this->m_pFont->InitFont( );

	// Init the console.
	this->PushWindow( &this->ConsoleWnd, 20, 90, 285, 300, "mcHook Console", true );
	
	// Init the input box w/ memo.
	this->ConsoleWnd.PushObject( &this->ConsoleInput, 3, 300/*Win Height*/ - ( FONT_SIZE + FONT_SIZE / 2 ), 285/*Win Width*/ - 6, 15, &fnInput, 300/*Win Height*/ - 40, &this->ConsoleMemo );

	UI.SetMemo( &this->ConsoleMemo );

	UI.ProcessMessage( "Warsow mcHook (for 0.42)" );
	UI.ProcessMessage( "by " S_COLOR_YELLOW "bigmac" );
	UI.ProcessMessage( "Main Credits To:" );
	UI.ProcessMessage( S_COLOR_YELLOW "Azorbix " S_COLOR_WHITE "- Fonts (" S_COLOR_GREEN "zox.gamedeception.net" S_COLOR_WHITE ")" );
	UI.ProcessMessage( S_COLOR_YELLOW "noskill " S_COLOR_WHITE "- Various" );
	UI.ProcessMessage( S_COLOR_YELLOW "Warsow SDK " S_COLOR_WHITE "- Various" );
	UI.ProcessMessage( "type " S_COLOR_YELLOW "/help " S_COLOR_WHITE "for a list of commands" );

	// Loop through the var table, and push the cvar's on to the console's input list.
	for( i = 0; i < UI_VARTABLE_SIZE; i++ )
		this->ConsoleInput.PushInput( UI.UIVarTable[ i ].pVarButton, UI.UIVarTable[ i ].szCvar );

	this->PushWindow( &this->WinampWnd, 20, 400, 260, 48, "Winamp", true ); 

	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_PREVTRACK], 3, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnPrevTrack, "<<", false, true, &Winamp ); 
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_PLAY], 3 + 3 + 20, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnPlay, "|>", false, true, &Winamp );
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_PAUSE], 3 + 3 + 3 + 20 + 20, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnPause, "||", false, true, &Winamp );
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_STOP], 3 + 3 + 3 + 3 + 20 + 20 + 20, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnStop, "[]", false, true, &Winamp );
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_NEXTTRACK], 3 + 3 + 3 + 3 + 3 + 20 + 20 + 20 + 20, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnNextTrack, ">>", false, true, &Winamp );
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_SPAM], 3 + 3 + 3 + 3 + 3 + 3 + 20 + 20 + 20 + 20 + 20, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 40, FONT_SIZE + 3, &Winamp.fnSpam, "Spam", false, true, &Winamp );
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_PL], 3 + 3 + 3 + 3 + 3 + 3 + 3 + 20 + 20 + 20 + 20 + 20 + 40/*Prev Obj = 40, not 20*/, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnPlaylist, "PL", false, true, &Winamp );

	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_VOLDOWN], 260/*Win Width*/ - 46, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnVolDown, "-", false, true, &Winamp );
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_VOLUP], 260/*Win Width*/ - 23, 48/*Win Height*/ - ( 15/*Obj Height*/ + 3 ), 20, FONT_SIZE + 3, &Winamp.fnVolUp, "+", false, true, &Winamp );
	this->WinampWnd.PushObject( &this->WinampControls[WINAMP_TRACK], 3, FONT_SIZE + 3, 260/*Win Width*/ - 6, FONT_SIZE + 1, &Fake, "", false, false, NULL );

	this->PushWindow( &this->WinampPLWnd, 20, 450, 260, 200, "Winamp Playlist", true );

	this->WinampPLWnd.PushObject( &this->WinampPLMemo, 3, FONT_SIZE + 3, 260/*Win Width*/ - 6, 200/*Win Height*/ - ( FONT_SIZE + 3 + 3 ) );
	
	// Read the config. (always do this after winamp stuff).
	CConfig::ReadConfig( "mcHook_default.cfg", &this->ConsoleMemo, &this->ConsoleInput );

	// init the scheme window.
	this->PushWindow( &this->SchemeWnd, 20, 8, 125, 3 + ( FONT_SIZE + 1 ) * ( SCHEME_LEN + 1 ) + ( SCHEME_LEN * 2 ), "Schemes", false );

	// push the schemes onto the scheme list.
	for( i = 0; i < SCHEME_LEN; i++ )
	{
		// Nasty :>
		this->SchemeWnd.PushObject
		( 
			m_SchemeButton[ i ], 
			3, 
			2 + ( i * 2 ) + ( FONT_SIZE + 1 ) * ( i + 1 ), 
			125 - 6, 
			( 3 + ( FONT_SIZE + 1 ) * ( SCHEME_LEN + 1 ) + ( SCHEME_LEN * 2 ) ) + // don't ask me wtf is going on here.
			( FONT_SIZE + 2 ) - ( ( FONT_SIZE + 1 ) * ( SCHEME_LEN + 1 ) + ( SCHEME_LEN * 3 ) ), 
			&fnSchemeChange, 
			m_SchemeList[ i ]->GetSchemeName( ), 
			false, false, NULL
		);
	}

	// done.
	m_bInited = true;
}

void CGui::DrawGuiWindows( void )
{
	int i = 0;

	m_bActiveRendered = false;

	for( ; i < ( int )m_vWindows.size( ); i++ )
	{
		if( m_vWindows[ i ] == m_pActiveWin )
			continue;

		m_vWindows[ i ]->Draw( );
	}

	if( m_pActiveWin && !m_bActiveRendered ) // no point putting active rendered to true, since its the end of the func.
		m_pActiveWin->Draw( );
}

inline int CGui::GetActiveState( void )
{
	return( m_iActive );
}

inline CWindow *CGui::GetActiveWin( void )
{
	return( m_pActiveWin );
}

inline CWindow *CGui::GetDraggingWin( void )
{
	return( m_pDraggingWin );
}

inline CFont *CGui::GetFont( void )
{
	return( m_pFont );
}

inline bool CGui::GetKey( int iKey )
{
	return( Keys[ iKey ].bDown );
}

inline CDrawScheme *CGui::GetScheme( void )
{
	return( m_pScheme );
}

inline CDrawScheme *CGui::GetScheme( int iScheme )
{
	return( m_SchemeList[ iScheme ] );
}

inline CWindow *CGui::GetWindow( int iNumber )
{
	return( m_vWindows[ iNumber ] );
}

inline int CGui::GetWindowSize( void )
{
	return( ( int )m_vWindows.size( ) );
}

inline bool CGui::HandleMsg( int iKey, bool bDown )
{
	if( bDown && iKey == ( int )m_cActivationKey )
	{
		( m_iActive < 2 ? m_iActive++ : m_iActive = 0 );

		Keys[ iKey ].bDown = false;
		return( false );
	}

	return( true );
}

void CGui::PushWindow( CWindow *pWindow, float x, float y, float fWidth, float fHeight, char *szWinName, bool bMaximized, bool bBackground )
{
	m_vWindows.push_back( pWindow );

	*pWindow = CWindow( x, y, fWidth, fHeight, szWinName, bMaximized, ( int )m_vWindows.size( ) - 1, bBackground );
}

inline void CGui::SetActiveRendered( bool bState )
{
	m_bActiveRendered = bState;
}

inline void CGui::SetActiveState( int iActive )
{
	m_iActive = iActive;
}

inline void CGui::SetActiveWin( CWindow *pWin )
{
	m_pActiveWin = pWin;
}

inline void CGui::SetDraggingWin( CWindow *pWin )
{
	m_pDraggingWin = pWin;
}

inline void CGui::SetKey( int iKey, bool bState )
{
	if( iKey != m_cActivationKey )
		Keys[ iKey ].bDown = bState;
}

inline void CGui::SetScheme( CDrawScheme *pNewScheme )
{
	m_pScheme = pNewScheme;
}

void CGui::SetScheme( float *fBorderCol, float *fColor, int iType )
{
	m_pScheme->SetBorderColor( fBorderCol );
	m_pScheme->SetColor( fColor );
	m_pScheme->SetType( iType );
}

void CGui::Update( void )
{
	if( m_bInited && m_iActive )
	{
		static int iFrameNum = 899;

		this->DrawGuiWindows( );

		float *fRadDim = this->RadarWnd.GetDimensions( );
		float *fSpycamDim = this->SpycamWnd.GetDimensions( );

		// Draw the local player on the radar.
		if( this->RadarWnd.GetMaximized( ) && UI.UIVarTable[ UI_RADAR_VAR ].iValue >= 1 )
			oImports.R_DrawStretchPic( fRadDim[0] + ( fRadDim[2] / 2 ) - 5, fRadDim[1] + ( fRadDim[3] / 2 ), 10.0f, 10.0f, 0.0f, 0.0f, 0.9f, 0.9f, colorWhite, &g_shaderArrowUp );

		// Draw the spycam crosshair.
		if( this->SpycamWnd.GetMaximized( ) && UI.UIVarTable[ UI_SPYCAM_VAR ].iValue > 0 )
			oImports.R_DrawStretchPic( ( SpyCam_rd.x + ( SpyCam_rd.width / 2 ) ) - 13, ( SpyCam_rd.y + ( SpyCam_rd.height / 2 ) ) - 13, 26, 26, 0, 0, 1, 1, colorWhite, &g_shaderCrosshair );

		iFrameNum++;
		if( this->WinampWnd.GetMaximized( ) && Winamp.bInited && iFrameNum >= 900 )
		{
			iFrameNum = 0;
			Winamp.UpdateTitle( );
		}

		// Draw the mouse.
		if( m_iActive == 1 )
			oImports.R_DrawStretchPic( this->fMouse[0] - 8, this->fMouse[1] - 8, 20, 20, 0, 0, 1, 1, colorWhite, &g_shaderCursor );

		this->fLastMouse[0] = fMouse[0];
		this->fLastMouse[1] = fMouse[1];
	}
}