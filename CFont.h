#ifndef _CFONT_HEADER
#define _CFONT_HEADER

#define FL_NONE 0
#define FL_SHADOW 1
#define FL_UNDERLINE 2
#define FL_CENTERX 4
#define FL_CENTERY 8
#define FL_COLTAGS 16
#define FL_ALL 255

class CFont
{
public:
	CFont( void );
	CFont( char *, int, int );

	void			DrawString( int, int, float *, unsigned char, char *, ... );
	virtual short	GetFontHeight( void );
	virtual int		GetStringLength( char *, bool );
	void			InitFont( void );

	~CFont( void );
private:
	int				ParseString( char *, char * );
	void			Render( int, int, float *, char * );
	void			RenderC( int, int, float *, char * );

	int				m_iFontFlag, m_iFontSize;
	short			m_iFontHeight, m_iFontWidth[255];
	unsigned int	m_iFontID;
	char			m_szFontName[32];
	bool			m_bFontInited;
};

#endif // _CFONT_HEADER