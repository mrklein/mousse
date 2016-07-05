// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "float_scalar.hpp"
#include "iostreams.hpp"
#include <sstream>


#define Scalar floatScalar
#define ScalarVGREAT floatScalarVGREAT
#define ScalarVSMALL floatScalarVSMALL
#define ScalarROOTVGREAT floatScalarROOTVGREAT
#define ScalarROOTVSMALL floatScalarROOTVSMALL
#define readScalar readFloatScalar

#include "scalar.ipp"

#undef Scalar
#undef ScalarVSMALL
#undef ScalarVSMALL
#undef ScalarROOTVGREAT
#undef ScalarROOTVSMALL
#undef readScalar
