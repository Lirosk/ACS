#include <stdio.h>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <bitset>

__m64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;

__m128i do_it(const __m64& A, const __m64& B, const __m64& C, const __m128i& D)
{
    const int tmp = 1<<8; 
    __m64 resh, resl;
    __m128i F;

    mm0 = _mm_add_pi8(A, B);

    mm1 = _mm_set_pi16(_mm_extract_pi16(mm0, 3) / tmp, _mm_extract_pi16(mm0, 3) % tmp,
                        _mm_extract_pi16(mm0, 2) / tmp, _mm_extract_pi16(mm0, 2) % tmp);
    mm2 = _mm_set_pi16(_mm_extract_pi16(mm0, 1) / tmp, _mm_extract_pi16(mm0, 1) % tmp,
                        _mm_extract_pi16(mm0, 0) / tmp, _mm_extract_pi16(mm0, 0) % tmp);

    mm3 = _mm_set_pi16(_mm_extract_pi16(C, 3) / tmp, _mm_extract_pi16(C, 3) % tmp,
                        _mm_extract_pi16(C, 2) / tmp, _mm_extract_pi16(C, 2) % tmp);
    mm4 = _mm_set_pi16(_mm_extract_pi16(C, 1) / tmp, _mm_extract_pi16(C, 1) % tmp,
                        _mm_extract_pi16(C, 0) / tmp, _mm_extract_pi16(C, 0) % tmp);

    printf("mm0     %d %d %d %d %d %d %d %d    8x8\n", 
                                            _mm_extract_pi16(mm0, 3) / tmp, _mm_extract_pi16(mm0, 3) % tmp,
                                            _mm_extract_pi16(mm0, 2) / tmp, _mm_extract_pi16(mm0, 2) % tmp,
                                            _mm_extract_pi16(mm0, 1) / tmp, _mm_extract_pi16(mm0, 1) % tmp,
                                            _mm_extract_pi16(mm0, 0) / tmp, _mm_extract_pi16(mm0, 0) % tmp);
    printf("mm1:mm2 %d %d %d %d:%d %d %d %d    4x16\n\n", 
                                            _mm_extract_pi16(mm1, 3), _mm_extract_pi16(mm1, 2),
                                            _mm_extract_pi16(mm1, 1), _mm_extract_pi16(mm1, 0),
                                            _mm_extract_pi16(mm2, 3), _mm_extract_pi16(mm2, 2),
                                            _mm_extract_pi16(mm2, 1), _mm_extract_pi16(mm2, 0));

    resh = _m_pmullw(mm1, mm3);
    resl = _m_pmullw(mm2, mm4);

    mm0 = resh;
    printf("resh %d %d %d %d\n", _mm_extract_pi16(mm0, 3), _mm_extract_pi16(mm0, 2),
                                _mm_extract_pi16(mm0, 1), _mm_extract_pi16(mm0, 0));
    mm0 = resl;
    printf("resl %d %d %d %d\n\n", _mm_extract_pi16(mm0, 3), _mm_extract_pi16(mm0, 2),
                                _mm_extract_pi16(mm0, 1), _mm_extract_pi16(mm0, 0));

    F = _mm_set_epi64(resh, resl);

    printf("F %d %d %d %d %d %d %d %d\n", _mm_extract_epi16(F, 7), _mm_extract_epi16(F, 6),
                                            _mm_extract_epi16(F, 5), _mm_extract_epi16(F, 4),
                                            _mm_extract_epi16(F, 3), _mm_extract_epi16(F, 2),
                                            _mm_extract_epi16(F, 1), _mm_extract_epi16(F, 0));

    F = _mm_sub_epi16(F, D);

    printf("F %d %d %d %d %d %d %d %d\n", _mm_extract_epi16(F, 7), _mm_extract_epi16(F, 6),
                                            _mm_extract_epi16(F, 5), _mm_extract_epi16(F, 4),
                                            _mm_extract_epi16(F, 3), _mm_extract_epi16(F, 2),
                                            _mm_extract_epi16(F, 1), _mm_extract_epi16(F, 0));
    
    return F;
}

int main()
{
    const __m64 A = _mm_set_pi8(1, 2, 3, 4, 5, 6, 7, 8),
                B = _mm_set_pi8(0, 0, 0, 0, 0, 0, 0, 0),
                C = _mm_set_pi8(100, 100, 100, 100, 100, 100, 100, 100);
    
    const __m128i D = _mm_set_epi16(-10000, -10000, -10000, -10000, -10000, -10000, -10000, -10000);
    __m128i F;

    mm0 = _mm_setzero_si64();
    mm1 = _mm_setzero_si64();
    mm2 = _mm_setzero_si64();
    mm3 = _mm_setzero_si64();
    mm4 = _mm_setzero_si64();
    mm5 = _mm_setzero_si64();
    mm6 = _mm_setzero_si64();
    mm7 = _mm_setzero_si64(); 

    F = do_it(A, B, C, D);
}
