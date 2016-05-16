// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_region_diff_no.hpp"
#include "fvc.hpp"

mousse::scalar mousse::solidRegionDiffNo
(
  const fvMesh& mesh,
  const Time& runTime,
  const volScalarField& Cprho,
  const volScalarField& kappa
)
{
  scalar DiNum = 0.0;
  scalar meanDiNum = 0.0;
  //- Take care: can have fluid domains with 0 cells so do not test for
  //  zero internal faces.
  surfaceScalarField kapparhoCpbyDelta
  {
    mesh.surfaceInterpolation::deltaCoeffs()
    *fvc::interpolate(kappa)/fvc::interpolate(Cprho)
  };
  DiNum = gMax(kapparhoCpbyDelta.internalField())*runTime.deltaT().value();
  meanDiNum = (average(kapparhoCpbyDelta)).value()*runTime.deltaT().value();
  Info << "Region: " << mesh.name() << " Diffusion Number mean: " << meanDiNum
    << " max: " << DiNum << endl;
  return DiNum;
}
