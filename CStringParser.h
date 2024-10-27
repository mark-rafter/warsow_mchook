#ifndef _CSTRINGPARSER_HEADER
#define _CSTRINGPARSER_HEADER

// this isn't actually a class, but i wanted to keep the header names tidy.

namespace CParser
{
	typedef char arg_t [64];
	typedef char line_t [256];
	typedef char file_line_t [96];
	typedef char word_t [48];

	// I commented these functions heavily because they confused me with all the problems they -were- throwing at me.

	static void GetLines( const char *szBuffer, int iNumLines, file_line_t *szOutput )
	{
		int iBufferPos = 0, iOutputPos = 0, i = 0;

		for( ; i < iNumLines; i++ ) // loop through the number of lines there are.
		{
			iOutputPos = 0; // reset the output buffer position to 0, so we write at the beginning of the line.

			while( szBuffer[ iBufferPos ] != '\r' && szBuffer[ iBufferPos ] != '\0' ) // while the current line has not ended.
			{
				szOutput[ i ][ iOutputPos ] = szBuffer[ iBufferPos ]; // copy the current letter to the output.
				iBufferPos++; // increase buffer position.
				iOutputPos++; // increase output buffer pos.
			}

			iBufferPos += 2; // we've finished that line now skip a buffer pos because we don't want the "\r\n" included in our next line.

			szOutput[ i ][ iOutputPos ] = '\0'; // null terminate.
		}
	}

	static int GetNumWords( char *szInput )
	{
		int i = 0, iNumWords = 1; // there must be at least one word (and if not, we won't return iNumWords anyway)

		if( szInput[ 0 ] == '\0' )
			return( 0 ); // no words...

		while( szInput[ strlen( szInput ) - 1 ] == ' ' )
			szInput[ strlen( szInput ) - 1 ] = '\0';

		// this is to get rid of any spaces at the end of the string.
	
		for( i = 1; i < ( int )strlen( szInput ); i++ )
		{
			if( szInput[ i ] == ' ' && szInput[ i - 1 ] != ' ' )	// check for a space, but ignore it if
				iNumWords++;										// it's already been done (double space)
		}

		return( iNumWords );
	}

	static void GetWords( const char *szBuffer, int iNumWords, word_t *szOutput )
	{
		int iBufferPos = 0, iOutputPos = 0, i = 0;

		for( ; i < iNumWords; i++ ) // loop through the number of words there are.
		{
			iOutputPos = 0; // reset the output buffer position to 0, so we write at the beginning of the string.

			if( i + 1 == iNumWords ) // check if its the last word.
			{
				while( szBuffer[ iBufferPos ] != '\0' ) // while the string hasnt ended (this is the last word, so there is no space)
				{
					szOutput[ i ][ iOutputPos ] = szBuffer[ iBufferPos ]; // copy the current letter to the output.
					iBufferPos++; // increase buffer position.
					iOutputPos++; // increase output buffer pos.
				}
			}
			else
			{
				while( szBuffer[ iBufferPos ] != ' ' ) // while the current word has not ended.
				{
					szOutput[ i ][ iOutputPos ] = szBuffer[ iBufferPos ]; // copy the current letter to the output.
					iBufferPos++; // increase buffer position.
					iOutputPos++; // increase output buffer pos.
				}
				iBufferPos++; // we've finished that word now skip a buffer pos because we don't want the space included in our next word.
			}

			szOutput[ i ][ iOutputPos ] = '\0'; // null terminate.
		}
	}

	static char *GetArgBuffer( char *szInput, int iCmdLen, int *iNumArgs )
	{
		int iBufferPos = iCmdLen, i = 0;
		char *szBuffer;
		
		*iNumArgs = 0;
		// to start off with, there are 0 arguments.

		if( szInput[ iBufferPos ] == '\0' || szInput[ iBufferPos + 1 ] == '\0' )
			return( 0 );

		*iNumArgs = 1; // there must be at least one arg otherwise we would have returned 0.

		szBuffer = szInput + iBufferPos + 1;

		while( szBuffer[ strlen( szBuffer ) - 1 ] == ' ' )
			szBuffer[ strlen( szBuffer ) - 1 ] = '\0'; // get rid of any spaces at the end, that might be tricking us.

		for( i = 1; i < ( int )strlen( szBuffer ); i++ ) // loop through the buffer
		{
			if( szBuffer[ i ] == ' ' && szBuffer[ i - 1 ] != ' ' ) // check for spaces, if it is a double/triple/etc space ignore it.
				*iNumArgs = 1 + *iNumArgs; // since there is a space, increase the number of arguments
		}

		return( szBuffer );
	}

	static void GetArgs( char *szBuffer, int iNumArgs, arg_t *szOutput )
	{
		int iBufferPos = 0, iOutputPos = 0, i = 0;

		for( ; i < iNumArgs; i++ ) // loop through the number of arguments there are.
		{
			iOutputPos = 0; // reset the output buffer position to 0, so we write at the beginning of the string.

			if( i + 1 == iNumArgs ) // check if its the last argument.
			{
				while( szBuffer[ iBufferPos ] != '\0' ) // while the string hasnt ended (this is the last arg, so there is no space)
				{
					szOutput[ i ][ iOutputPos ] = szBuffer[ iBufferPos ]; // copy the current letter to the output.
					iBufferPos++; // increase buffer position.
					iOutputPos++; // increase output buffer pos.
				}
			}
			else
			{
				while( szBuffer[ iBufferPos ] != ' ' ) // while the current arg has not ended.
				{
					szOutput[ i ][ iOutputPos ] = szBuffer[ iBufferPos ]; // copy the current letter to the output.
					iBufferPos++; // increase buffer position.
					iOutputPos++; // increase output buffer pos.
				}
				iBufferPos++; // we've finished that arg, now skip a buffer pos because we don't want the space included in our next arg.
			}

			szOutput[ i ][ iOutputPos ] = '\0'; // null terminate.
		}
	}
}

#endif // _CSTRINGPARSER_HEADER