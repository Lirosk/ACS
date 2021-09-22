#include <stdio.h>
#include <mmintrin.h>
#include <emmintrin.h>
#include <experimental/random>

using namespace std;
using namespace experimental;

__m64 mm0 = _mm_setzero_si64(),
    mm1 = _mm_setzero_si64(), 
    mm2 = _mm_setzero_si64(), 
    mm3 = _mm_setzero_si64(), 
    mm4 = _mm_setzero_si64(), 
    mm5 = _mm_setzero_si64(), 
    mm6 = _mm_setzero_si64(), 
    mm7 = _mm_setzero_si64();

void print8x8(__m64 vec)
{
    printf("%d %d %d %d %d %d %d %d", 
                                    (char)(_mm_extract_pi16(vec, 3) >> 8), (char)((_mm_extract_pi16(vec, 3) << 8)>>8),
                                    (char)(_mm_extract_pi16(vec, 2) >> 8), (char)((_mm_extract_pi16(vec, 2) << 8)>>8),
                                    (char)(_mm_extract_pi16(vec, 1) >> 8), (char)((_mm_extract_pi16(vec, 1) << 8)>>8),
                                    (char)(_mm_extract_pi16(vec, 0) >> 8), (char)((_mm_extract_pi16(vec, 0) << 8)>>8));
}

void print4x16(__m64 vec)
{
    printf("%d %d %d %d", (short)_mm_extract_pi16(vec, 3), (short)_mm_extract_pi16(vec, 2),
                                    (short)_mm_extract_pi16(vec, 1), (short)_mm_extract_pi16(vec, 0));
}

void print8x16(__m128i vec)
{
    printf("%d %d %d %d %d %d %d %d", (short)_mm_extract_epi16(vec, 7), (short)_mm_extract_epi16(vec, 6),
                                            (short)_mm_extract_epi16(vec, 5), (short)_mm_extract_epi16(vec, 4),
                                            (short)_mm_extract_epi16(vec, 3), (short)_mm_extract_epi16(vec, 2),
                                            (short)_mm_extract_epi16(vec, 1), (short)_mm_extract_epi16(vec, 0));
}

__m128i do_it(const __m64& A, const __m64& B, const __m64& C, const __m128i& D)
{
    
    __m64 resh, resl;
    __m128i F;

    mm0 = _mm_add_pi8(A, B);
    
    int64_t buf = _mm_cvtm64_si64(mm0);
    
    mm1 = _mm_set_pi16(buf >> 56, (__int64_t)(buf << 8) >> 56, (__int64_t)(buf << 16) >> 56, (__int64_t)(buf << 24) >> 56);
    mm2 = _mm_set_pi16((__int64_t)(buf << 32) >> 56, (__int64_t)(buf << 40) >> 56, (__int64_t)(buf << 48) >> 56, (__int64_t)(buf << 56) >> 56);

    buf = _mm_cvtm64_si64(C);

    mm3 = _mm_set_pi16(buf >> 56, (__int64_t)(buf << 8) >> 56, (__int64_t)(buf << 16) >> 56, (__int64_t)(buf << 24) >> 56);
    mm4 = _mm_set_pi16((__int64_t)(buf << 32) >> 56, (__int64_t)(buf << 40) >> 56, (__int64_t)(buf << 48) >> 56, (__int64_t)(buf << 56) >> 56);

    printf("A+B: ");
    print8x8(mm0);
    printf("\n\n");

    resh = _m_pmullw(mm1, mm3);
    resl = _m_pmullw(mm2, mm4);

    F = _mm_set_epi64(resh, resl);

    printf("(A+B)*C: ");
    print8x16(F);
    printf("\n\n");

    F = _mm_sub_epi16(F, D);

    printf("(A+B)*C-D: ");
    print8x16(F);
    printf("\n\n");

    return F;
}

int main()
{
    char* a = new char(8);
    char* b = new char(8);
    char* c = new char(8);
    short* d = new short(8);
    short* f = new short(8);

    for (int i = 0; i < 8; i++)
    {
        a[i] = (char)randint(0, 1<<8);
        b[i] = (char)randint(0, 1<<8);
        c[i] = (char)randint(0, 1<<8);
        d[i] = (short)randint(0, 1<<16);
        f[i] = (char)(a[i] + b[i]);
        f[i] = (short)(f[i]) * (short)(c[i]);
        f[i] = f[i] - d[i];
    }

    const __m64 A = _mm_set_pi8(a[7], a[6], a[5], a[4], a[3], a[2], a[1], a[0]),
                B = _mm_set_pi8(b[7], b[6], b[5], b[4], b[3], b[2], b[1], b[0]),
                C = _mm_set_pi8(c[7], c[6], c[5], c[4], c[3], c[2], c[1], c[0]);
    
    const __m128i D = _mm_set_epi16(d[7], d[6], d[5], d[4], d[3], d[2], d[1], d[0]);

    __m128i F;

    printf("A: ");
    print8x8(A);
    printf("\nB: ");
    print8x8(B);
    printf("\nC: ");
    print8x8(C);
    printf("\nD: ");
    print8x16(D);
    printf("\n\n");

    F = do_it(A, B, C, D);


    print8x16(F);
    printf("   by mmx\n");
    for (int i = 7; i > -1; i--)
    {
        printf("%d ", f[i]);
    }
    printf("  by cpp\n\n\n");

    delete[] a, b, c, d, f;
}
