#include <stdio.h>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

int main() {
	__m64 A, B, C, buf;
	__m128i D, F;
	
	A = _mm_set_pi16(1, 2, 3, 4);
	B = _mm_set_pi16(5, 6, 7, 8);
	C = _mm_set_pi16(1, 7, 2, 8);
	D = _mm_set_epi32(420, 207, 42, 27);
	F = _mm_setzero_si128();

	buf = _mm_setzero_si64();

	__asm
	{
		movq mm0, A
		paddsb mm0, B
		
		pmullw mm0, C

		movq buf, mm0

		emms
	}

	printf("%d %d %d %d \n", _m_pextrw(buf, 3), _m_pextrw(buf, 2), _m_pextrw(buf, 1), _m_pextrw(buf, 0));
}
