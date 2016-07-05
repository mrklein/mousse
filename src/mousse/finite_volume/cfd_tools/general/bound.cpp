// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bound.hpp"
#include "vol_fields.hpp"
#include "fvc.hpp"


// Global Functions 
mousse::volScalarField&
mousse::bound(volScalarField& vsf, const dimensionedScalar& lowerBound)
{
  const scalar minVsf = min(vsf).value();
  if (minVsf < lowerBound.value()) {
    Info << "bounding " << vsf.name()
      << ", min: " << minVsf
      << " max: " << max(vsf).value()
      << " average: " << gAverage(vsf.internalField())
      << endl;
    vsf.internalField() = max
    (
      max
      (
        vsf.internalField(),
        fvc::average
        (
          max(vsf, lowerBound)
        )().internalField()*pos(-vsf.internalField())
      ),
      lowerBound.value()
    );
    vsf.boundaryField() = max(vsf.boundaryField(), lowerBound.value());
  }

  return vsf;
}
