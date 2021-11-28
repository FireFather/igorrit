// 64 bit version (bits operations):
#ifdef _MSC_VER
static __inline int BSF( UINT64 x )
{
	int y;
	_BitScanForward64(&y, x);
	return y;
    }
static __inline int BSR( UINT64 x )
{
	int y;
	_BitScanReverse64(&y, x);
	return y;
}
#else
static int BSF( UINT64 x )
{
	_asm
	{
		bsf rax, x
	}
}
static int BSR( UINT64 x )
{
	_asm
	{
		bsr rax, x
	}
}
#endif

static int POPCNT(UINT64 w)
{
	w = w - ((w >> 1) & 0x5555555555555555ull);
	w = (w & 0x3333333333333333ull) + ((w >> 2) & 0x3333333333333333ull);
	w = (w + (w >> 4)) & 0x0f0f0f0f0f0f0f0full;
	return (w * 0x0101010101010101ull) >> 56;
}

#define bitLIBERO(b, B) B &= (B - 1)
#define bitFISSO(b, B) B |= ((uint64) 1) << (b)
