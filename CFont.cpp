/*
This font class was created by Azorbix for Game-Deception (http://www.gamedeception.net).
Original print style created by Jeff Molofee (http://nehe.gamedev.net)

Credits:
Azorbix (for making it) [http://zox.gamedeception.net]
Crusader (Help with original glPrint function) [http://crusader.gamedeception.net]
LanceVorgin (Help with character lengths) [http://lvgl.gamedeception.net]
Xnient (Many improvements to fontclass) [http://zox.gamedeception.net]

This code may not be reproduced anywhere without concent from Xnient or Azorbix.
If you are going to use this code in any program, please credit the names above.

Dont be a RIPPER!

bigmac: edited and optimized some stuff.
*/

#include "Common.h"

// bigmac: using Q3 color table in Q_Math.cpp
/*float fColorTable[10][4] = 
{
	{ 0.0, 0.0, 0.0, 1.0 }, // 0 black
	{ 1.0, 0.0, 0.0, 1.0 }, // 1 red
	{ 0.0, 1.0, 0.0, 1.0 }, // 2 green
	{ 1.0, 1.0, 0.0, 1.0 }, // 3 was blue, now yellow
	{ 0.0, 0.0, 1.0, 1.0 }, // 4 was yellow, now blue
	{ 0.0, 1.0, 1.0, 1.0 }, // 5 cyan
	{ 1.0, 0.0, 1.0, 1.0 }, // 6 magenta
	{ 1.0, 1.0, 1.0, 1.0 }, // 7 was orange, now white
	{ 1.0, 0.5, 0.0, 1.0 }, // 8 was purple, now orange
	{ 0.5, 0.5, 0.5, 1.0 }	// 9 was white, now grey
};*/

CFont::CFont( void )
{
	strcpy( this->m_szFontName, "Tahoma" );
	this->m_iFontSize = 14;
	this->m_iFontFlag = FW_NORMAL;
}

CFont::CFont( char *szFontName, int iFontSize, int iFontFlag )
{
	strcpy( this->m_szFontName, szFontName );
	this->m_iFontSize = iFontSize;
	this->m_iFontFlag = iFontFlag;
}

CFont::~CFont( void )
{
	m_bFontInited = false;
}

void CFont::InitFont( void )
{
	HFONT hFont;
	HFONT hOldFont;
	HDC hDC;
	int i;

	hDC = pwglGetCurrentDC( );

	this->m_iFontID = pglGenLists( 256 );

	hFont = CreateFont( m_iFontSize, 0, 0, 0, m_iFontFlag, 0, 0, 0, ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_szFontName );

	hOldFont = ( HFONT )SelectObject( hDC, hFont );

	pwglUseFontBitmapsA( hDC, 0, 255, m_iFontID );

	for( i = 0; i < 255; i++ )
	{
		SIZE Size = { 0 };

		char szLine[2] = { ( char )i, 0 }; // credits to LanceVorgin

		GetTextExtentPoint( hDC, szLine, 1, &Size );

		this->m_iFontWidth[ i ] = ( short )Size.cx;
		this->m_iFontHeight = ( short )Size.cy;
	}

	SelectObject( hDC, hOldFont );

	DeleteObject( hFont );

	m_bFontInited = true;
}

inline short CFont::GetFontHeight( void )
{
	return( m_iFontHeight );
}

inline int CFont::GetStringLength( char *szBuf, bool bColor )
{
	int iDrawLen = 0;
	char *szTemp = new char [256];

	memset( szTemp, '\0', sizeof( szTemp ) );

	if( bColor )
		iDrawLen = ParseString( szBuf, szTemp );
	else
	{
		for( unsigned char *p = ( unsigned char * )szBuf; *p; p++ ) // bigmac: converted into unsigned char * (so char's like ñ and ' can be used).
			iDrawLen += m_iFontWidth[ *p ];
	}

	delete [] szTemp;
	return( iDrawLen );
}

int CFont::ParseString( char *szIn, char *szOut )
{
	int i = 0;
	int iDrawLen = 0;

	for( i = 0; *szIn; szIn++ )
	{
		if( *szIn == '^' )
		{	
			szIn++;

			if( !*szIn )
				break;

			continue;
		}
		szOut[ i ] = *szIn;
		i++;
		iDrawLen += m_iFontWidth[ *szIn ];
	}

	szOut[ i ] = 0;

	return( iDrawLen );
}

void CFont::DrawString( int x, int y, float *fColor, unsigned char cFlags, char *szString, ... )
{
	unsigned char bTex2D = false;

	pglGetBooleanv( GL_TEXTURE_2D, &bTex2D );

	if( bTex2D )
		pglDisable( GL_TEXTURE_2D );

	int iDrawLen = 0;
	char szText[512] = { 0 };
	char szPrintText[512] = { 0 };
	va_list argPtr = 0;

	va_start( argPtr, szString );				
	vsprintf( szText, szString, argPtr );			
	va_end( argPtr );

	if( cFlags & FL_COLTAGS )
		iDrawLen = ParseString( szText, szPrintText );
	else
	{
		for( char *p = szText; *p; p++ )
			iDrawLen += m_iFontWidth[ *p ];

		strcpy( szPrintText, szText );
	}

	if( cFlags & FL_CENTERX )
		x -= ( iDrawLen / 2 );

	if( cFlags & FL_CENTERY )
		y += ( m_iFontHeight - 3 ) / 2;

	if( cFlags & FL_SHADOW )
	{
		Render( x + 2, y + 2, color_table[0], szPrintText );

		if( cFlags & FL_UNDERLINE )
		{
			pglColor4fv( colorBlack );
			pglLineWidth( 1.0f );

			pglBegin( GL_LINES );
			pglVertex2f( x + 2, y + 5 ); // bigmac: 2i to 2f.
			pglVertex2f( x + iDrawLen + 2, y + 5 ); // ^^
			pglEnd( );
		}
	}

	if( cFlags & FL_UNDERLINE )
	{
		pglColor4fv( fColor );
		pglLineWidth( 1.0f );

		pglBegin( GL_LINES );
		pglVertex2f( x, y + 3 ); // ^^
		pglVertex2f( x + iDrawLen, y + 3 ); // ^^
		pglEnd( );
	}

	( cFlags & FL_COLTAGS ? RenderC( x, y, fColor, szText ) : Render( x, y, fColor, szText ) );

	if( bTex2D )
		pglEnable( GL_TEXTURE_2D );
}

void CFont::RenderC( int x, int y, float *fColor, char *szString )
{
	int iWidth = 0;
	int i = 0;
	float fNewColor[4] = { fColor[0], fColor[1], fColor[2], fColor[3] };
	char szBuf[512];

	for( char *p = szString; *p; p++ )
	{
		if( *p == '^' )
		{
			if( i )
				Render( x, y, fNewColor, szBuf );

			p++;

			if( !*p )
				break;

			int c = *p - 48;

			if( c > 9 )	c = 9;
			if( c < 0 )	c = 0;

			fNewColor[0] = ::color_table[ c ][0];
			fNewColor[1] = ::color_table[ c ][1];
			fNewColor[2] = ::color_table[ c ][2];
			fNewColor[3] = ::color_table[ c ][3];

			x += iWidth;
			i = iWidth = 0;
		}
		else
		{
			szBuf[ i ] = *p;
			szBuf[ i + 1 ] = 0;

			i++;

			iWidth += m_iFontWidth[ *p ];
		}
	}

	if( i )
		Render( x, y, fNewColor, szBuf );
}

void CFont::Render( int x, int y, float *fColor, char *szString )
{
	int i = 0;

	while( x < 0 )
	{
		x += m_iFontWidth[ szString[ i ] ];
		i++;

		if( !szString[ i ] )
			return;
	}

	pglColor4fv( fColor );
	pglRasterPos2i( x, y );

	pglHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	pglPushAttrib( GL_LIST_BIT );							
	pglListBase( m_iFontID );
	pglCallLists( ( int )strlen( szString ) - i, GL_UNSIGNED_BYTE, szString + i );
	pglPopAttrib( );
}