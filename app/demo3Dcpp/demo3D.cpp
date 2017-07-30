// STL
#include <cstdlib>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

// Lib
#include "coeffContainer.h"
#include "Wavelet2D.h"

using T = float;

int main(int argc, char **argv) {


  int sizeX=8;
  int sizeY=8;
  int sizeZ=8;

  auto modify = [sizeX,sizeY,sizeZ](auto* ptr) {
    for(int k =0; k<sizeZ; k++) {
      for(int j=0; j<sizeY; j++) {
        for(int i=0; i<sizeX; i++) {
          ptr[k*sizeX*sizeY+j*sizeX+i]=i;
        }
      }
    }
  };

  auto print = [sizeX,sizeY, sizeZ](auto* ptr) {
    for(int k =0; k<sizeZ; k++) {
      for(int j=0; j<sizeY; j++) {
        for(int i=0; i<sizeX; i++) {
          std::cout<<ptr[k*sizeX*sizeY+j*sizeX+i]<<", ";
        }
        std::cout<<std::endl;
      }
      std::cout<<std::endl;
      std::cout<<std::endl;
    }
  };
  auto print2 = [](auto& in){ std::cout<<in<<", "; };


  // Define input/output
  std::vector<T> in(sizeX*sizeY*sizeZ);
  //std::iota(in.begin(), in.end(),0);
  //std::fill(in.begin(), in.end(), 2);
  modify(in.data());

  std::cout<<"Input is: ";
  print(in.data());

  // Define wavelet tranform
  //Dummy2_2D<T> w(in.data(),sizeX,sizeY,1,false,"Daub2",1);
  Daub2_2D<T> w(in.data(),sizeX,sizeY,sizeZ,false,"Daub2",1);
  //Anto97_BiOrth_1D<T> w(in.data(),sizeX,sizeY,1,false,"Anto97",1);
  //REVERSE_QSHIFT6_Orth_1D<T> w(in.data(),sizeX,sizeY,false,"QSHIFT6",1);
  //dtwAnto97QSHIFT6_2D<T> w(in.data(),sizeX,sizeY,1,false,"DTCWT",3); 
    

  // print coeffs when initialized
  //std::cout<<"Coefficient after initialization (should be 0)"<<std::endl;
  //std::for_each(w.get_coeff().begin(),w.get_coeff().end(),print);
  //std::cout<<std::endl;

  // perform forward transform
  w.forward();
  //Delete previous image
  std::fill(in.begin(),in.end(),0);

  // print coeffs
  //std::cout<<"Coefficient after forward tranform"<<std::endl;
  //std::for_each(w.get_coeff().begin(),w.get_coeff().end(),print2);
  //std::cout<<std::endl;

  // perform inverse transform
  w.backward(); 
  
  std::cout<<"Output is: ";
  print(in.data());

  return EXIT_SUCCESS;
}