#include <mmintrin.h>
#include <emmintrin.h>

__m64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;

__m128i do_it(const __m64& A, const __m64& B, const __m64& C, const __m128i& D)
{
    const int BYTE = 1<<8;
    __m64 resh, resl;
    __m128i F;

    // mm0 = A + B
    mm0 = _mm_add_pi8(A, B);

    // mm1:mm2 = mm0 = A + B
    mm1 = _mm_set_pi16(_mm_extract_pi16(mm0, 3) / BYTE, _mm_extract_pi16(mm0, 3) % BYTE,
                        _mm_extract_pi16(mm0, 2) / BYTE, _mm_extract_pi16(mm0, 2) % BYTE);
    mm2 = _mm_set_pi16(_mm_extract_pi16(mm0, 1) / BYTE, _mm_extract_pi16(mm0, 1) % BYTE,
                        _mm_extract_pi16(mm0, 0) / BYTE, _mm_extract_pi16(mm0, 0) % BYTE);

    // mm3:mm4 = C
    mm3 = _mm_set_pi16(_mm_extract_pi16(C, 3) / BYTE, _mm_extract_pi16(C, 3) % BYTE,
                        _mm_extract_pi16(C, 2) / BYTE, _mm_extract_pi16(C, 2) % BYTE);
    mm4 = _mm_set_pi16(_mm_extract_pi16(C, 1) / BYTE, _mm_extract_pi16(C, 1) % BYTE,
                        _mm_extract_pi16(C, 0) / BYTE, _mm_extract_pi16(C, 0) % BYTE);

    resh = _m_pmullw(mm1, mm3);
    resl = _m_pmullw(mm2, mm4);

    // F = resh:resl = (A + B) * C
    F = _mm_set_epi64(resh, resl);
    // F = (A + B) * C - D
    F = _mm_sub_epi16(F, D);
    
    return F;
}
