// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "double_scalar.hpp"
#include "iostreams.hpp"
#include <sstream>


#define Scalar doubleScalar
#define ScalarVGREAT doubleScalarVGREAT
#define ScalarVSMALL doubleScalarVSMALL
#define ScalarROOTVGREAT doubleScalarROOTVGREAT
#define ScalarROOTVSMALL doubleScalarROOTVSMALL
#define readScalar readDoubleScalar


#include "scalar.ipp"


#undef Scalar
#undef ScalarVGREAT
#undef ScalarVSMALL
#undef ScalarROOTVGREAT
#undef ScalarROOTVSMALL
#undef readScalar
