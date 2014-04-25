#ifndef __DECOMPRESS_HH_INCLUDED__
#define __DECOMPRESS_HH_INCLUDED__

#include <QByteArray>
#include <string>

using std::string;

QByteArray zlibDecompress( const char * bufptr, unsigned length );

string decompressZlib( const char * bufptr, unsigned length );

#ifdef MAKE_BZ2_SUPPORT

string decompressBzip2( const char * bufptr, unsigned length );

#endif

#ifdef MAKE_ZIM_SUPPORT

string decompressLzma2( const char * bufptr, unsigned length );

#endif

#endif // DECOMPRESS_HH
