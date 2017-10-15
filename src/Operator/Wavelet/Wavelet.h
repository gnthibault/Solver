#ifndef WAVELET_H
#define WAVELET_H

// STL
#include <iostream>
#include <list>
#include <memory>
#include <string>

// Local


/** \struct w_info
 * \brief Description of the workload
 *
 * \author Pierre Paleo
 */
struct w_info {
  /// Number of columns in the image
  int Nx;
  /// Number of rows of the image (2D)
  int Ny;
  /// Number of slices in the image (3D)
  int Nz;
};

/** \struct WaveletWrapper
 * \brief This class is only here to provide a non templated interface to
 * all possible types of wavelet defined in this software
 *
 * \author Thibault Notargiacomo
 */
template<typename T>
class WaveletWrapper {
 public:
  WaveletWrapper()=default;
  virtual int forward()=0;
  virtual int backward()=0;
  virtual int inverse()=0;
  virtual int set_image(T* img)=0;
//  virtual int get_coeff() const=0;
};

/** \class Wavelet
 * \brief The wavelet class manage the lifecycle of the wavelet transform
 * coefficients.
 *
 * \author Pierre Paleo
 */
template<typename T, class CoeffContainerT, class WaveletSchemeT>
class Wavelet :public WaveletWrapper<T> {
 public:
  /// Constructor with zero initialization
  Wavelet() : m_image{nullptr}, m_coeff{nullptr}, m_doCycleSpinning{false},
    m_currentShift{}, m_name{}, m_info{0} {}

  /// Constructor : Wavelet from image
  Wavelet(T* img, int Nx, int Ny, int Nz, bool doCycleSpinning,
      const std::string& wname, int level) : Wavelet() {
    m_image = img;
    m_info.Nx = Nx;
    m_info.Ny = Ny;
    m_info.Nz = Nz;
    m_doCycleSpinning = doCycleSpinning;
    m_name = wname;
    m_level = level;
  }
  /// Copy constructor deleted because of unique_ptr
  Wavelet(const Wavelet& w)=delete;
  /// Default destructor
  virtual ~Wavelet()=default;

  /// Print wavelet transform informations
  virtual void print_informations() {
    std::cout<<"Informations"<<std::endl;
  }

  /// Forward wavelet tranform
  virtual int forward() override {
    return 1;
  }
  /// Backward wavelet transform: transpose of the forward transpose
  virtual int backward() override {
    return 1;
  }
  /// Inverse of the wavelet tranform
  virtual int inverse() override {
    return 1;
  }

  /// Soft thresholding: proximity operator for L1 norm
  //void soft_threshold(T beta, int do_thresh_appcoeffs = 0, int normalize = 0);
  /// Hard thresholding: best k-term approximation for wavelets
  //void hard_threshold(T beta, int do_thresh_appcoeffs = 0, int normalize = 0);
  /// Wavelet shrinkage
  //void shrink(T beta, int do_thresh_appcoeffs = 1);
  /// Projection on the \f$ l-\infty \f$ ball
  //void proj_linf(T beta, int do_thresh_appcoeffs = 1);
  /// Circular shift
  //void circshift(int sr, int sc, int inplace = 1);


  /// Compute the \f$ l-2 \f$ norm of the vector of coefficients
  //T norm2sq();
  /// Compute the \f$ l-1 \f$ norm of the vector of coefficients
  //T norm1();
  /// Get image pointer
  //virtual int get_image(T* img) const override {
  //  std::copy(m_image,m_image+m_info.Nx*m_info.Ny*m_info.Nz,img);
  //  return 1;
  //}
  /// Return a reference to wavelet coefficients
  virtual CoeffContainerT& get_coeff() {
    return *m_coeff;
  }
  /// Set input image to be transformed in the wavelet domain
  int virtual set_image(T* img) override {
    m_image=img;
    return 1;
  }
  /// Set coefficients to be reconstructed into an image
  int virtual set_coeff(CoeffContainerT* coeff) {
    m_coeff.reset(coeff);
    return 1;
  }

  /// set filter for forward transform
  //int set_filters_forward(char* filtername, uint len, T* filter1, T* filter2,
  //    T* filter3 = NULL, T* filter4 = NULL);
  /// set filters for inverse transform
  //int set_filters_inverse(T* filter1, T* filter2, T* filter3 = NULL,
  //    T* filter4 = NULL);
  /// Add wavelet
  //int add_wavelet(Wavelet W, T alpha=1.0f);

 protected:
  /// Image (input or result of reconstruction), on device
  T* m_image;
  /// Wavelet coefficients, on device
  std::unique_ptr<CoeffContainerT> m_coeff;
  /** If cycle spinning is enabled, use image shifting for translation
   * invariant denoising
   */
  bool m_doCycleSpinning;
  /// Current shift for the cycle spinning process
  std::list<int> m_currentShift;
  /// Wavelet name
  std::string m_name;
  /// Informations about the wavelet tranform
  w_info m_info;
  /// Number of level od dyadic decomposition
  int m_level;
};

#endif //WAVELET_H
