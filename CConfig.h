#ifndef _CCONFIG_HEADER
#define _CCONFIG_HEADER

// Cfg stuff..

class CMemo;
class CEditBox;

namespace CConfig
{
	static bool ReadConfig( char *szFileName, CMemo *pMemo, CEditBox *pEditBox )
	{
		if( szFileName[0] == '\0' )
		{
			pMemo->PushTruncLine( S_COLOR_RED "Error" S_COLOR_WHITE ": NULL filename" );
			return( false );
		}

		int i = 0, iNumLines = 1;
		CmdOwner_t iCmdOwner;
		long lFileSize = 0;
				
		FILE *pFile = fopen( szFileName, "rb" );

		if( !pFile )
		{
			pMemo->PushTruncLine( AppendStr( S_COLOR_RED "Error" S_COLOR_WHITE ": Failed to read " S_COLOR_MAGENTA "%s", szFileName ) );
			return( false );
		}

		fseek( pFile , 0, SEEK_END );
		lFileSize = ftell( pFile );
		rewind( pFile );

		char *szBuffer = new char [lFileSize + 1];

		szBuffer[ 0 ] = ( char )getc( pFile );

		if( szBuffer[ 0 ] == '\0' )
		{
			pMemo->PushTruncLine( AppendStr( S_COLOR_RED "Error" S_COLOR_WHITE ": " S_COLOR_MAGENTA "%s " S_COLOR_WHITE "contains NULL characters", szFileName ) );
			delete [] szBuffer;
			return( false );
		}

		do
		{
			i++;
			szBuffer[ i ] = ( char )getc( pFile );

			if( szBuffer[ i ] == '\r' )
				iNumLines++;
		} while( szBuffer[ i ] != EOF );

		szBuffer[ i ] = '\0';

		CParser::file_line_t *szLines = new CParser::file_line_t [iNumLines];

		CParser::GetLines( szBuffer, iNumLines, szLines );

		delete [] szBuffer;

		pMemo->PushTruncLine( AppendStr( "Successfully loaded " S_COLOR_MAGENTA "%s", szFileName ) );

		for( i = 0; i < iNumLines; i++ )
		{
			if( !strcmp( szLines[ i ], "[ui]" ) )
			{
				iCmdOwner = CMD_CONSOLE; // check user interface cvars.
				continue; // go to next line
			}

			if( szLines[ i ][0] != '/' ) // ignore anything that doesn't have a /
				continue;

			pEditBox->CheckForCmd( szLines[ i ], iCmdOwner );
		}

		delete [] szLines;

		fclose( pFile );

		return( true );
	}
}

#endif // _CCONFIG_HEADER