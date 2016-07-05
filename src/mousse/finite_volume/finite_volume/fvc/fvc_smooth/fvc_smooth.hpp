#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SMOOTH_FVC_SMOOTH_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_SMOOTH_FVC_SMOOTH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_smooth.cpp
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  void smooth
  (
    volScalarField& field,
    const scalar coeff
  );
  void spread
  (
    volScalarField& field,
    const volScalarField& alpha,
    const label nLayers,
    const scalar alphaDiff = 0.2,
    const scalar alphaMax = 0.99,
    const scalar alphaMin = 0.01
  );
  void sweep
  (
    volScalarField& field,
    const volScalarField& alpha,
    const label nLayers,
    const scalar alphaDiff = 0.2
  );
}
}
#endif
