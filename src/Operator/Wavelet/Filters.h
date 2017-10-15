///
/// Wavelets coefficients, extracted from http://wavelets.pybytes.com
///

#ifndef FILTERS_H
#define FILTERS_H

// STL
#include <array>
#include <string>

// Local
#include "vectorization/Vectorization.h"

/** \struct GenericFilter
 * \brief Generic interface that should be implemented by filters. It has been
 * designed to provide various metaprogramming helper in order to setup
 * explicit vectorization or enable advanced compiler inlining/optimization
 * 
 * TAP_SIZE_LEFT does only account for number of elements at left,
 * without the current one.
 * TAP_SIZE_RIGHT does only account for number of elements at right,
 * without the current one.
 *
 * \author Thibault Notargiacomo
 */
template<typename T, int TAP_SIZE_LEFT, int TAP_SIZE_RIGHT>
struct GenericFilter {
  /// Defaulted constructor
  GenericFilter()=default;

  /// Typedef for scalar main type
  using ScalarType=T;
  
  /// Typedef for vectorization type
  using VectorType=PackType<T>;
  
  /// Redefine template parameters
  static constexpr const int VecSize = sizeof(VectorType)/sizeof(T);

  /// Compile time definition of filter feature: left tap number
  static constexpr const int TapSizeLeft = TAP_SIZE_LEFT;
  
  /// Compile time definition of filter feature: right tap number
  static constexpr const int TapSizeRight = TAP_SIZE_RIGHT;

  /// Total size of the filter, in number of elements
  static constexpr const int TapSize =
    TapSizeLeft + TapSizeRight + 1; //+1 = the center pixel

  /// Convenient variables used to compute upsampled convolution
  static constexpr const int TapHalfSizeLeft = TapSizeLeft/2;

  /// Convenient variables used to compute upsampled convolution
  static constexpr const int TapHalfSizeRight = TapSizeRight/2;

  /// Convenient variables used to compute upsampled convolution
  static constexpr const int TapHalfFloorSizeLeft = (TapSizeLeft-1)/2;

  /// Convenient variables used to compute upsampled convolution
  static constexpr const int TapHalfCeilSizeRight = (TapSizeRight+1)/2;

  /// Convenient variables used to compute upsampled convolution
  static constexpr const int EvenSubSampOffset = TapSizeLeft&1;

  /// Convenient variables used to compute upsampled convolution
  static constexpr const int OddSubSampOffset = 1-(TapSizeLeft&1);

  /// How many vector are needed to load a single filter support
  static constexpr const int NbVecPerFilt = (TapSize+VecSize-1)/(VecSize);
 };

/** \struct Filter
 * \bried We define an inheritance of the fully generic filter for a structure
 * that will be able to handle its own storage for the filter
 *
 * \author Thibault Notargiacomo
 */
template<typename T, int TAP_SIZE_LEFT, int TAP_SIZE_RIGHT,
  typename FilterEnum, FilterEnum FilterVal>
struct Filter : public GenericFilter<T,TAP_SIZE_LEFT,TAP_SIZE_RIGHT> {
  using ParenT =  GenericFilter<T,TAP_SIZE_LEFT,TAP_SIZE_RIGHT>; 

  /// Defaulted constructor
  Filter()=default;
  
  // Actual storage for the filter
  static constexpr const std::array<T,ParenT::TapSize> Buff = {0};
};

/** \struct wFilter
 * \brief A group of filters that defines a full forward and backward filtering
 * process
 *
 * \author Pierre Paleo and Thibault Notargiacomo
 */
template<class ForwardLowT, class ForwardHighT, class InverseLowT,
  class InverseHighT>
struct wFilter {
  /// A small string that defines the wavelet system name
  std::string wname;
  
  /// Forward lowpass filter
  using f_l = ForwardLowT;
  /// Forward highpasas filter
  using f_h = ForwardHighT;    
  /// Inverse lowpass filter
  using i_l = InverseLowT; 
  /// Inverse highpass filter
  using i_h = InverseHighT;
};

/** \struct dtwFilter
 * \brief A group of filters that defines a full forward and backward complex
 * wavelet filter process
 *
 * \author Thibault Notargiacomo
 */
template<class Stage0RealT,
         class Stage0ImagT,
         class StageNRealT,
         class StageNImagT>
struct dtwFilter {
  /// A small string that defines the wavelet system name
  std::string wname;
  
  /// Forward lowpass filter stage 0 real part
  using f_l0r = typename Stage0RealT::f_l;
  /// Forward lowpass filter stage 0 imaginary part
  using f_l0i = typename Stage0ImagT::f_l;
  /// Forward highpass filter stage 0 real part
  using f_h0r = typename Stage0RealT::f_h;
  /// Forward highpass filter stage 0 imaginary part
  using f_h0i = typename Stage0ImagT::f_h;
  /// Forward lowpass filter stage n real part
  using f_lnr = typename StageNRealT::f_l;
  /// Forward lowpass filter stage n imaginary part
  using f_lni = typename StageNImagT::f_l;
  /// Forward highpass filter stage n real part
  using f_hnr = typename StageNRealT::f_h;
  /// Forward highpass filter stage n imaginary part
  using f_hni = typename StageNImagT::f_h;
  /// Inverse lowpass filter stage 0 real part
  using i_l0r = typename Stage0RealT::i_l;
  /// Inverse lowpass filter stage 0 imaginary part
  using i_l0i = typename Stage0ImagT::i_l;
  /// Inverse highpass filter stage 0 real part
  using i_h0r = typename Stage0RealT::i_h;
  /// Inverse highpass filter stage 0 imaginary part
  using i_h0i = typename Stage0ImagT::i_h;
  /// Inverse lowpass filter stage n real part
  using i_lnr = typename StageNRealT::i_l;
  /// Inverse lowpass filter stage n imaginary part
  using i_lni = typename StageNImagT::i_l;
  /// Inverse highpass filter stage n real part
  using i_hnr = typename StageNRealT::i_h;
  /// Inverse highpass filter stage n imaginary part
  using i_hni = typename StageNImagT::i_h;
};

enum class filterDB {
  DB2_L,
  DB2_H,
  DB2_I_L,
  DB2_I_H,
  DB3_L,
  DB3_H,
  DB3_I_L,
  DB3_I_H,
  DB4_L,
  DB4_H,
  DB4_I_L,
  DB4_I_H,
  DB5_L,
  DB5_H,
  DB5_I_L,
  DB5_I_H,
  ANTO9_L,
  ANTO7_H,
  ANTO7_I_L,
  ANTO9_I_H,
  QSHIFT6_L,
  QSHIFT6_H,
  QSHIFT6_I_L,
  QSHIFT6_I_H,
  REVERSE_QSHIFT6_L,
  REVERSE_QSHIFT6_H,
  REVERSE_QSHIFT6_I_L,
  REVERSE_QSHIFT6_I_H,
  DM2_L,
  DM2_H,
  DM2_I_L,
  DM2_I_H
};

//Semi specialization, type agnostic
template<typename T>
struct Filter<T,1,2,filterDB,filterDB::DB2_L> : public
    GenericFilter<T,1,2> {
  static constexpr const std::array<T,4> Buff = {
    -0.12940952255092145,
    0.22414386804185735,
    0.836516303737469,
    0.48296291314469025
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,1,2,filterDB,filterDB::DB2_H> : public
    GenericFilter<T,1,2> {
  static constexpr const std::array<T,4> Buff = {
    -0.48296291314469025,
    0.836516303737469,
    -0.22414386804185735,
    -0.12940952255092145
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,1,filterDB,filterDB::DB2_I_L> : public
    GenericFilter<T,2,1> {
  static constexpr const std::array<T,4> Buff = {
    0.48296291314469025,
    0.836516303737469,
    0.22414386804185735,
    -0.12940952255092145
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,1,filterDB,filterDB::DB2_I_H> : public
    GenericFilter<T,2,1> {
  static constexpr const std::array<T,4> Buff = {
    -0.12940952255092145,
    -0.22414386804185735,
    0.836516303737469,
    -0.48296291314469025
  };
};
/// The Daubechies2 wavelet system, type agnostic
template<typename T>
using Daub2 = wFilter<
    Filter<T,1,2,filterDB,filterDB::DB2_L>,
    Filter<T,1,2,filterDB,filterDB::DB2_H>,
    Filter<T,2,1,filterDB,filterDB::DB2_I_L>,
    Filter<T,2,1,filterDB,filterDB::DB2_I_H>>;
template <typename T>
const std::array<T,4> Filter<T,1,2,filterDB,filterDB::DB2_L>::Buff;
template <typename T>
const std::array<T,4> Filter<T,1,2,filterDB,filterDB::DB2_H>::Buff;
template <typename T>
const std::array<T,4> Filter<T,2,1,filterDB,filterDB::DB2_I_L>::Buff;
template <typename T>
const std::array<T,4> Filter<T,2,1,filterDB,filterDB::DB2_I_H>::Buff;


//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,3,filterDB,filterDB::DB3_L> : public
    GenericFilter<T,2,3> {
  static constexpr const std::array<T,6> Buff = {
    0.035226291882100656,
    -0.08544127388224149,
    -0.13501102001039084,
    0.4598775021193313, 
    0.8068915093133388,
    0.3326705529509569
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,3,filterDB,filterDB::DB3_H> : public
    GenericFilter<T,2,3> {
  static constexpr const std::array<T,6> Buff = {
    -0.3326705529509569,
    0.8068915093133388,
    -0.4598775021193313,
    -0.13501102001039084,
    0.08544127388224149,
    0.035226291882100656
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,3,2,filterDB,filterDB::DB3_I_L> : public
    GenericFilter<T,3,2> {
  static constexpr const std::array<T,6> Buff = {
    0.3326705529509569, 
    0.8068915093133388,
    0.4598775021193313,
    -0.13501102001039084,
    -0.08544127388224149,
    0.035226291882100656
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,3,2,filterDB,filterDB::DB3_I_H> : public
    GenericFilter<T,3,2> {
  static constexpr const std::array<T,6> Buff = {
    0.035226291882100656,
    0.08544127388224149,
    -0.13501102001039084,
    -0.4598775021193313,
    0.8068915093133388,
    -0.3326705529509569
  };
};
/// The Daubechies3 wavelet system, type agnostic
template<typename T>
using Daub3 = wFilter<
    Filter<T,2,3,filterDB,filterDB::DB3_L>,
    Filter<T,2,3,filterDB,filterDB::DB3_H>,
    Filter<T,3,2,filterDB,filterDB::DB3_I_L>,
    Filter<T,3,2,filterDB,filterDB::DB3_I_H>>;
template <typename T>
const std::array<T,6> Filter<T,2,3,filterDB,filterDB::DB3_L>::Buff;
template <typename T>
const std::array<T,6> Filter<T,2,3,filterDB,filterDB::DB3_H>::Buff;
template <typename T>
const std::array<T,6> Filter<T,3,2,filterDB,filterDB::DB3_I_L>::Buff;
template <typename T>
const std::array<T,6> Filter<T,3,2,filterDB,filterDB::DB3_I_H>::Buff;


//Semi specialization, type agnostic
template<typename T>
struct Filter<T,3,4,filterDB,filterDB::DB4_L> : public
    GenericFilter<T,3,4> {
  static constexpr const std::array<T,8> Buff = {
    -0.010597401784997278,
    0.032883011666982945 ,
    0.030841381835986965 ,
    -0.18703481171888114 ,
    -0.02798376941698385 ,
    0.6308807679295904   ,
    0.7148465705525415   ,
    0.23037781330885523
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,3,4,filterDB,filterDB::DB4_H> : public
    GenericFilter<T,3,4> {
  static constexpr const std::array<T,8> Buff = {
    -0.23037781330885523 ,
    0.7148465705525415   ,
    -0.6308807679295904  ,
    -0.02798376941698385 ,
    0.18703481171888114  ,
    0.030841381835986965 ,
    -0.032883011666982945,
    -0.010597401784997278
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,4,3,filterDB,filterDB::DB4_I_L> : public
    GenericFilter<T,4,3> {
  static constexpr const std::array<T,8> Buff = {
    0.23037781330885523  ,
    0.7148465705525415   ,
    0.6308807679295904   ,
    -0.02798376941698385 ,
    -0.18703481171888114 ,
    0.030841381835986965 ,
    0.032883011666982945 ,
    -0.010597401784997278
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,4,3,filterDB,filterDB::DB4_I_H> : public
    GenericFilter<T,4,3> {
  static constexpr const std::array<T,8> Buff = {
    -0.010597401784997278,
    -0.032883011666982945,
    0.030841381835986965 ,
    0.18703481171888114  ,
    -0.02798376941698385 ,
    -0.6308807679295904  ,
    0.7148465705525415   ,
    -0.23037781330885523
  };
};
/// The Daubechies4 wavelet system, type agnostic
template<typename T>
using Daub4 = wFilter<
    Filter<T,3,4,filterDB,filterDB::DB4_L>,
    Filter<T,3,4,filterDB,filterDB::DB4_H>,
    Filter<T,4,3,filterDB,filterDB::DB4_I_L>,
    Filter<T,4,3,filterDB,filterDB::DB4_I_H>>;
template <typename T>
const std::array<T,8> Filter<T,3,4,filterDB,filterDB::DB4_L>::Buff;
template <typename T>
const std::array<T,8> Filter<T,3,4,filterDB,filterDB::DB4_H>::Buff;
template <typename T>
const std::array<T,8> Filter<T,4,3,filterDB,filterDB::DB4_I_L>::Buff;
template <typename T>
const std::array<T,8> Filter<T,4,3,filterDB,filterDB::DB4_I_H>::Buff;


//Semi specialization, type agnostic
template<typename T>
struct Filter<T,4,5,filterDB,filterDB::DB5_L> : public
    GenericFilter<T,4,5> {
  static constexpr const std::array<T,10> Buff = {
    0.003335725285001549,
    -0.012580751999015526,
    -0.006241490213011705,
    0.07757149384006515,
    -0.03224486958502952,
    -0.24229488706619015,
    0.13842814590110342,
    0.7243085284385744,
    0.6038292697974729,
    0.160102397974125
 };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,4,5,filterDB,filterDB::DB5_H> : public
    GenericFilter<T,4,5> {
  static constexpr const std::array<T,10> Buff = {
    -0.160102397974125,
    0.6038292697974729,
    -0.7243085284385744,
    0.13842814590110342,
    0.24229488706619015,
    -0.03224486958502952,
    -0.07757149384006515,
    -0.006241490213011705,
    0.012580751999015526,
    0.003335725285001549
 };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,5,4,filterDB,filterDB::DB5_I_L> : public
    GenericFilter<T,5,4> {
  static constexpr const std::array<T,10> Buff = {
    0.160102397974125,
    0.6038292697974729,
    0.7243085284385744,
    0.13842814590110342,
    -0.24229488706619015,
    -0.03224486958502952,
    0.07757149384006515,
    -0.006241490213011705,
    -0.012580751999015526,
    0.003335725285001549
 };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,5,4,filterDB,filterDB::DB5_I_H> : public
    GenericFilter<T,5,4> {
  static constexpr const std::array<T,10> Buff = {
    0.003335725285001549,
    0.012580751999015526,
    -0.006241490213011705,
    -0.07757149384006515,
    -0.03224486958502952,
    0.24229488706619015,
    0.13842814590110342,
    -0.7243085284385744,
    0.6038292697974729,
    -0.160102397974125
 };
};
/// The Daubechies5 wavelet system, type agnostic
template<typename T>
using Daub5 = wFilter<
    Filter<T,4,5,filterDB,filterDB::DB5_L>,
    Filter<T,4,5,filterDB,filterDB::DB5_H>,
    Filter<T,5,4,filterDB,filterDB::DB5_I_L>,
    Filter<T,5,4,filterDB,filterDB::DB5_I_H>>;
template <typename T>
const std::array<T,10> Filter<T,4,5,filterDB,filterDB::DB5_L>::Buff;
template <typename T>
const std::array<T,10> Filter<T,4,5,filterDB,filterDB::DB5_H>::Buff;
template <typename T>
const std::array<T,10> Filter<T,5,4,filterDB,filterDB::DB5_I_L>::Buff;
template <typename T>
const std::array<T,10> Filter<T,5,4,filterDB,filterDB::DB5_I_H>::Buff;


//Semi specialization, type agnostic
template<typename T>
struct Filter<T,4,4,filterDB,filterDB::ANTO9_L> : public
    GenericFilter<T,4,4> {
  static constexpr const std::array<T,9> Buff = {
    0.02674875741081,
    -0.01686411844287,
    -0.07822326652899,
    0.26686411844288,
    0.60294901823636,
    0.26686411844287,
    -0.07822326652899,
    -0.01686411844287,
    0.02674875741081
 };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,4,filterDB,filterDB::ANTO7_H> : public
    GenericFilter<T,2,4> {
  static constexpr const std::array<T,7> Buff = {
    0.04563588155712,
    -0.02877176311425,
    -0.29563588155712,
    0.55754352622850,
    -0.29563588155713,
    -0.02877176311425,
    0.04563588155712,
 };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,3,3,filterDB,filterDB::ANTO7_I_L> : public
    GenericFilter<T,3,3> {
  static constexpr const std::array<T,7> Buff = {
    -0.09127176311424,
    -0.05754352622850,
    0.59127176311424,
    1.11508705245700,
    0.59127176311426,
    -0.05754352622850,
    -0.09127176311424
 };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,5,3,filterDB,filterDB::ANTO9_I_H> : public
    GenericFilter<T,5,3> {
  static constexpr const std::array<T,9> Buff = {
    0.05349751482162,
    0.03372823688574,
    -0.15644653305798,
    -0.53372823688576,
    1.20589803647272,
    -0.53372823688574,
    -0.15644653305798,
    0.03372823688574,
    0.05349751482162
 };
};
/// The (9,7) tap bi-orthogonal Antonini filter, type agnostic
template<typename T>
using Anto97_BiOrth = wFilter<
    Filter<T,4,4,filterDB,filterDB::ANTO9_L>,
    Filter<T,2,4,filterDB,filterDB::ANTO7_H>,
    Filter<T,3,3,filterDB,filterDB::ANTO7_I_L>,
    Filter<T,5,3,filterDB,filterDB::ANTO9_I_H>>;

template <typename T>
const std::array<T,9> Filter<T,4,4,filterDB,filterDB::ANTO9_L>::Buff;
template <typename T>
const std::array<T,7> Filter<T,2,4,filterDB,filterDB::ANTO7_H>::Buff;
template <typename T>
const std::array<T,7> Filter<T,3,3,filterDB,filterDB::ANTO7_I_L>::Buff;
template <typename T>
const std::array<T,9> Filter<T,5,3,filterDB,filterDB::ANTO9_I_H>::Buff;


//Semi specialization, type agnostic
template<typename T>
struct Filter<T,4,3,filterDB,filterDB::QSHIFT6_L> : public
    GenericFilter<T,4,3> {
  static constexpr const std::array<T,8> Buff = {
    0.03516384000000,
    0.0,
    -0.08832942000000,
    0.23389032000000,
    0.76027237000000,
	0.58751830000000,
    0.0,
    -0.11430184000000
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,5,2,filterDB,filterDB::REVERSE_QSHIFT6_I_L> : public
    GenericFilter<T,5,2> {
  static constexpr const std::array<T,8> Buff =
    Filter<T,4,3,filterDB,filterDB::QSHIFT6_L>::Buff;
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,5,filterDB,filterDB::QSHIFT6_H> : public
    GenericFilter<T,2,5> {
  static constexpr const std::array<T,8> Buff = {
    -0.11430184000000,
    0.0,
    0.58751830000000,
    -0.76027237000000, 
    0.23389032000000,
    0.08832942000000,
    0.0,
    -0.03516384000000
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,3,4,filterDB,filterDB::REVERSE_QSHIFT6_I_H> : public
    GenericFilter<T,3,4> {
  static constexpr const std::array<T,8> Buff =
    Filter<T,2,5,filterDB,filterDB::QSHIFT6_H>::Buff;
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,3,4,filterDB,filterDB::QSHIFT6_I_L> : public
    GenericFilter<T,3,4> {
  static constexpr const std::array<T,8> Buff = {
    -0.11430184000000,
    0.0,
    0.58751830000000,
    0.76027237000000,
    0.23389032000000,
    -0.08832942000000,
    0.0,
    0.03516384000000
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,5,filterDB,filterDB::REVERSE_QSHIFT6_L> : public
    GenericFilter<T,2,5> {
  static constexpr const std::array<T,8> Buff =
    Filter<T,3,4,filterDB,filterDB::QSHIFT6_I_L>::Buff;
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,5,2,filterDB,filterDB::QSHIFT6_I_H> : public
    GenericFilter<T,5,2> {
  static constexpr const std::array<T,8> Buff = {
    -0.03516384000000,
    0.0,
    0.08832942000000,
    0.23389032000000,
    -0.76027237000000,
    0.58751830000000,
    0.0,
    -0.11430184000000
  };
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,4,3,filterDB,filterDB::REVERSE_QSHIFT6_H> : public
    GenericFilter<T,4,3> {
  static constexpr const std::array<T,8> Buff =
    Filter<T,5,2,filterDB,filterDB::QSHIFT6_I_H>::Buff;
};

/// The 6 tap orthogonal Q-Shift filter, type agnostic
template<typename T>
using QSHIFT6_Orth = wFilter<
    Filter<T,4,3,filterDB,filterDB::QSHIFT6_L>,
    Filter<T,2,5,filterDB,filterDB::QSHIFT6_H>,
    Filter<T,3,4,filterDB,filterDB::QSHIFT6_I_L>,
    Filter<T,5,2,filterDB,filterDB::QSHIFT6_I_H>>;

template <typename T>
const std::array<T,8> Filter<T,4,3,filterDB,filterDB::QSHIFT6_L>::Buff;
template <typename T>
const std::array<T,8> Filter<T,2,5,filterDB,filterDB::QSHIFT6_H>::Buff;
template <typename T>
const std::array<T,8> Filter<T,3,4,filterDB,filterDB::QSHIFT6_I_L>::Buff;
template <typename T>
const std::array<T,8> Filter<T,5,2,filterDB,filterDB::QSHIFT6_I_H>::Buff;

template<typename T>
using REVERSE_QSHIFT6_Orth = wFilter<
    Filter<T,2,5,filterDB,filterDB::REVERSE_QSHIFT6_L>,
    Filter<T,4,3,filterDB,filterDB::REVERSE_QSHIFT6_H>,
    Filter<T,5,2,filterDB,filterDB::REVERSE_QSHIFT6_I_L>,
    Filter<T,3,4,filterDB,filterDB::REVERSE_QSHIFT6_I_H>>;
template <typename T>
const std::array<T,8> Filter<T,2,5,filterDB,
  filterDB::REVERSE_QSHIFT6_L>::Buff;
template <typename T>
const std::array<T,8> Filter<T,4,3,
  filterDB,filterDB::REVERSE_QSHIFT6_H>::Buff;
template <typename T>
const std::array<T,8> Filter<T,5,2,filterDB,
  filterDB::REVERSE_QSHIFT6_I_L>::Buff;
template <typename T>
const std::array<T,8> Filter<T,3,4,
  filterDB,filterDB::REVERSE_QSHIFT6_I_H>::Buff;

template <typename T>
using dtwAnto97QSHIFT6 = dtwFilter<
         Anto97_BiOrth<T>,
         Anto97_BiOrth<T>,
         QSHIFT6_Orth<T>,
         REVERSE_QSHIFT6_Orth<T>>;

//Semi specialization, type agnostic
template<typename T>
struct Filter<T,1,2,filterDB,filterDB::DM2_L> : public
    GenericFilter<T,1,2> {
  static constexpr const std::array<T,4> Buff = {1,2,3,4};
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,1,2,filterDB,filterDB::DM2_H> : public
    GenericFilter<T,1,2> {
  static constexpr const std::array<T,4> Buff = {1,2,3,4};
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,1,filterDB,filterDB::DM2_I_L> : public
    GenericFilter<T,2,1> {
  static constexpr const std::array<T,4> Buff = {1,2,3,4};
};
//Semi specialization, type agnostic
template<typename T>
struct Filter<T,2,1,filterDB,filterDB::DM2_I_H> : public
    GenericFilter<T,2,1> {
  static constexpr const std::array<T,4> Buff = {1,2,3,4};
};
/// The Daubechies2 wavelet system, type agnostic
template<typename T>
using Dummy2 = wFilter<
    Filter<T,1,2,filterDB,filterDB::DM2_L>,
    Filter<T,1,2,filterDB,filterDB::DM2_H>,
    Filter<T,2,1,filterDB,filterDB::DM2_I_L>,
    Filter<T,2,1,filterDB,filterDB::DM2_I_H>>;
template <typename T>
const std::array<T,4> Filter<T,1,2,filterDB,filterDB::DM2_L>::Buff;
template <typename T>
const std::array<T,4> Filter<T,1,2,filterDB,filterDB::DM2_H>::Buff;
template <typename T>
const std::array<T,4> Filter<T,2,1,filterDB,filterDB::DM2_I_L>::Buff;
template <typename T>
const std::array<T,4> Filter<T,2,1,filterDB,filterDB::DM2_I_H>::Buff;


#endif
