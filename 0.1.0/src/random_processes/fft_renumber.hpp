#ifndef RANDOM_PROCESSES_FFT_RENUMBER_HPP_
#define RANDOM_PROCESSES_FFT_RENUMBER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Global
//   fftRenumber
//
// Description
//   Multi-dimensional renumbering used in the Numerical Recipes
//   fft routine.
// Source files
//   fft_renumber.cpp
#include "complex.hpp"
#include "list.hpp"
#include "label_list.hpp"
namespace mousse
{
void fftRenumber
(
  List<complex>& data,
  const labelList& nn
);
}  // namespace mousse
#endif
