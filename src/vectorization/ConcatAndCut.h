#ifndef CONCATANDCUT_H
#define CONCATANDCUT_H

// Local
#include "MetaHelper.h"
#include "vectorization.h"

//Perform left and right shift
template<typename T, class VecT, int SHIFT>
class VectorizedShift {
 public:
  //Defaulted implementation for scalar type: no shift
  constexpr static VecT LeftShift( VecT input ) {
    return SHIFT != 0 ? 0 : input;
  }
  //Defaulted implementation for scalar type: no shift
  constexpr static VecT RightShift( VecT input ) {
    return SHIFT != 0 ? 0 : input;
  }
};

/*
 * Concatenate two vectors that are given as input
 * then right shift the results of RIGHT_SHIFT elements
 */
template<typename T, class VecT, int RIGHT_SHIFT>
class VectorizedConcatAndCut {
 public:
  //Defaulted implementation is to rely on individual shifting
  //and add
  static VecT Concat( VecT left, VecT right ) {
    //Perform shift on both operand
    //The left one should be right shifted and right one should be left shifted
    VecT r = VectorizedShift<T,VecT,RightShiftBytes>::RightShift(left);
    VecT l = VectorizedShift<T,VecT,LeftShiftBytes>::LeftShift(right);
    //return sum of the two
    return r + l;
  }
 protected:
  constexpr static int VecSize = sizeof(VecT)/sizeof(T);
  constexpr static int RightShiftBytes = RIGHT_SHIFT*sizeof(T);
  constexpr static int LeftShiftBytes = (VecSize-RIGHT_SHIFT)*sizeof(T);
};

#ifdef USE_AVX
template<int SHIFT>
class VectorizedShift<float,__m128,SHIFT> {
 public:
  constexpr static __m128 LeftShift( __m128 input ) {
    return (__m128)_mm_slli_si128( (__m128i)input, SHIFT );
  }
  constexpr static __m128 RightShift( __m128 input ) {
    return (__m128)_mm_srli_si128( (__m128i)input, SHIFT );
  }
};
template<int SHIFT>
class VectorizedShift<double,__m128d,SHIFT> {
 public:
  constexpr static __m128d LeftShift( __m128d input ) {
    return (__m128d)_mm_slli_si128( (__m128i)input, SHIFT );
  }
  constexpr static __m128d RightShift( __m128d input ) {
    return (__m128d)_mm_srli_si128( (__m128i)input, SHIFT );
  }
};
#elif defined USE_AVX2
template<typename T, typename vecT, int Val, class enable=void>
struct AVX256ConcatandCut {
  static vecT Concat(vecT left, vecT right) {
    assert(("Vectorized Shift AVX256 cannot account for shift > 256 bits",
          false));
    return left;
  }
};

template<int Val>
struct AVX256ConcatandCut<float, __m256, Val,
    typename ctrange<0, 1, Val>::enabled> {
  static __m256 Concat(__m256 left, __m256 right) {
    return left;
  }
};
template<int Val>
struct AVX256ConcatandCut<float, __m256, Val,
    typename ctrange<1, 4, Val>::enabled> {
  static __m256 Concat(__m256 left, __m256 right) {
    return (__m256)_mm256_alignr_epi8(
      (__m256i)_mm256_permute2f128_ps(left,right,33),
      (__m256i)left, Val*sizeof(float));
  }
};
template<int Val>
struct AVX256ConcatandCut<float, __m256, Val,
    typename ctrange<4, 5, Val>::enabled> {
  static __m256 Concat(__m256 left, __m256 right) {
    return _mm256_permute2f128_ps(left,right,33);
  }
};
template<int Val>
struct AVX256ConcatandCut<float, __m256, Val,
    typename ctrange<5, 8, Val>::enabled> {
  static __m256 Concat(__m256 left, __m256 right) {
    return (__m256)_mm256_alignr_epi8((__m256i)right,
      (__m256i)_mm256_permute2f128_ps(left,right,33), (Val-4)*sizeof(float));
  }
};
template<int Val>
struct AVX256ConcatandCut<float, __m256, Val,
    typename ctrange<8, 9, Val>::enabled> {
  __m256 static Concat(__m256 left, __m256 right) {
    return right;
  }
};
template<int Val>
struct AVX256ConcatandCut<double, __m256d, Val,
    typename ctrange<0, 1, Val>::enabled> {
  static __m256d Concat(__m256d left, __m256d right) {
    return left;
  }
};
template<int Val>
struct AVX256ConcatandCut<double, __m256d, Val,
    typename ctrange<1, 2, Val>::enabled> {
  static __m256d Concat(__m256d left, __m256d right) {
    return (__m256d)_mm256_alignr_epi8(
      (__m256i)_mm256_permute2f128_pd(left,right,33),
      (__m256i)left, Val*sizeof(double));
  }
};
template<int Val>
struct AVX256ConcatandCut<double, __m256d, Val,
    typename ctrange<2, 3, Val>::enabled> {
  static __m256d Concat(__m256d left, __m256d right) {
    return _mm256_permute2f128_pd(left,right,33);
  }
};
template<int Val>
struct AVX256ConcatandCut<double, __m256d, Val,
    typename ctrange<3, 4, Val>::enabled> {
  static __m256d Concat(__m256d left, __m256d right) {
    return (__m256d)_mm256_alignr_epi8((__m256i)right,
      (__m256i)_mm256_permute2f128_pd(left,right,33), (Val-2)*sizeof(double));
  }
};
template<int Val>
struct AVX256ConcatandCut<double, __m256d, Val,
    typename ctrange<4, 5, Val>::enabled> {
  static __m256d Concat(__m256d left, __m256d right) {
    return right;
  }
};

template<int RIGHT_SHIFT>
class VectorizedConcatAndCut<float,__m256,RIGHT_SHIFT> {
 public:
  //Optimized specific intrinsic for concat / shift / cut in AVX
  static __m256 Concat( __m256 left, __m256 right ) {
    return AVX256ConcatandCut<float,__m256,RIGHT_SHIFT>::Concat(left,right);
  }
};
template<int RIGHT_SHIFT>
class VectorizedConcatAndCut<double,__m256d,RIGHT_SHIFT> {
 public:
  //Optimized specific intrinsic for concat / shift / cut in AVX
  static __m256d Concat( __m256d left, __m256d right ) {
    return AVX256ConcatandCut<double,__m256d,RIGHT_SHIFT>::Concat(left,right);
  }
};
#elif defined USE_NEON
template<int RIGHT_SHIFT>
class VectorizedConcatAndCut<float,float32x4_t,RIGHT_SHIFT> {
 public:
  //Optimized specific intrinsic for concat / shift / cut in AVX
  static float32x4_t Concat( float32x4_t left, float32x4_t right ) {
    return vextq_f32( left, right, RIGHT_SHIFT) ;
  }
};
template<int RIGHT_SHIFT>
class VectorizedConcatAndCut<double,float64x2_t,RIGHT_SHIFT> {
 public:
  //Optimized specific intrinsic for concat / shift / cut in AVX
  static float64x2_t Concat( float64x2_t left, float64x2_t right ) {
    return vextq_f64( left, right, RIGHT_SHIFT) ;
  }
};
#endif
#endif // CONCATANDCUT_H
