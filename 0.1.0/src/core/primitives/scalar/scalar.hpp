#ifndef CORE_PRIMITIVES_SCALAR_SCALAR_HPP_
#define CORE_PRIMITIVES_SCALAR_SCALAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   scalar.cpp
#include "float_scalar.hpp"
#include "double_scalar.hpp"
#if defined(WM_SP)
// Define scalar as a float
namespace mousse
{
  typedef floatScalar scalar;
  static const scalar GREAT = floatScalarGREAT;
  static const scalar VGREAT = floatScalarVGREAT;
  static const scalar ROOTVGREAT = floatScalarROOTVGREAT;
  static const scalar SMALL = floatScalarSMALL;
  static const scalar ROOTSMALL = floatScalarROOTSMALL;
  static const scalar VSMALL = floatScalarVSMALL;
  static const scalar ROOTVSMALL = floatScalarROOTVSMALL;
  scalar readScalar(Istream& is);
}
#elif defined(WM_DP)
// Define scalar as a double
namespace mousse
{
  typedef doubleScalar scalar;
  static const scalar GREAT = doubleScalarGREAT;
  static const scalar VGREAT = doubleScalarVGREAT;
  static const scalar ROOTVGREAT = doubleScalarROOTVGREAT;
  static const scalar SMALL = doubleScalarSMALL;
  static const scalar ROOTSMALL = doubleScalarROOTSMALL;
  static const scalar VSMALL = doubleScalarVSMALL;
  static const scalar ROOTVSMALL = doubleScalarROOTVSMALL;
  scalar readScalar(Istream& is);
}
#endif
#endif
