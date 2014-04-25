#include "decompress.hh"
#include "zlib.h"

#ifdef MAKE_BZ2_SUPPORT
#include "bzlib.h"
#endif

#ifdef MAKE_ZIM_SUPPORT
#include "lzma.h"
#endif

#define CHUNK_SIZE 2048

QByteArray zlibDecompress( const char * bufptr, unsigned length )
{
z_stream zs;
char buf[CHUNK_SIZE];
QByteArray str;
int res;
    memset( &zs, 0, sizeof(zs) );
    zs.next_in = (Bytef *)bufptr;
    zs.avail_in = length;
    while( 1 )
    {
        res = inflateInit( &zs );
        if( res != Z_OK )
            break;
        while( res != Z_STREAM_END )
        {
            zs.next_out = (Bytef *)buf;
            zs.avail_out = CHUNK_SIZE;
            res = inflate( &zs, Z_SYNC_FLUSH );
            str.append( buf, CHUNK_SIZE - zs.avail_out );
            if( res != Z_OK && res != Z_STREAM_END )
                 break;
        }
        break;
    }
    inflateEnd( &zs );
    if( res != Z_STREAM_END )
        str.clear();
    return str;
}

string decompressZlib( const char * bufptr, unsigned length )
{
  QByteArray b = zlibDecompress( bufptr, length );
  return string( b.constData(), b.size() );
}

#ifdef MAKE_BZ2_SUPPORT

string decompressBzip2( const char * bufptr, unsigned length )
{
bz_stream zs;
char buf[CHUNK_SIZE];
string str;
int res;
    memset( &zs, 0, sizeof(zs) );
    zs.next_in = (char *)bufptr;
    zs.avail_in = length;
    zs.total_in_lo32 = length;
    while( 1 )
    {
        res = BZ2_bzDecompressInit( &zs, 0, 0 );
        if( res != BZ_OK )
            break;
        while( res != BZ_STREAM_END )
        {
            zs.next_out = buf;
            zs.avail_out = CHUNK_SIZE;
            zs.total_out_lo32 = length;
            res = BZ2_bzDecompress( &zs );
            str.append( buf, CHUNK_SIZE - zs.avail_out );
            if( res != BZ_OK && res != BZ_STREAM_END )
                break;
        }
        break;
    }
    BZ2_bzDecompressEnd( &zs );
    if( res != BZ_STREAM_END )
        str.clear();
    return str;
}

#endif

#ifdef MAKE_ZIM_SUPPORT

#define BUFSIZE 0xFFFF

string decompressLzma2( const char * bufptr, unsigned length )
{
string str;
lzma_ret res;
char buf[BUFSIZE];

  lzma_stream strm = LZMA_STREAM_INIT;
  strm.next_in = reinterpret_cast< const uint8_t * >( bufptr );
  strm.avail_in = length;

  while( 1 )
  {
    res = lzma_stream_decoder( &strm, UINT64_MAX, 0 );
    if( res != LZMA_OK )
      break;

    while ( res != LZMA_STREAM_END )
    {
      strm.next_out = reinterpret_cast< uint8_t * >( buf );
      strm.avail_out = BUFSIZE;
      res = lzma_code( &strm, LZMA_RUN );
      str.append( buf, BUFSIZE - strm.avail_out );
      if( res != LZMA_OK && res != LZMA_STREAM_END )
        break;
    }
    lzma_end( &strm );
    if( res != LZMA_STREAM_END )
      str.clear();

    break;
  }
  return str;
}

#endif
