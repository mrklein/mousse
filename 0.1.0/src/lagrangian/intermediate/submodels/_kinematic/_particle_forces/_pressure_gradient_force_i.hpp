// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const mousse::interpolation<mousse::vector>&
mousse::PressureGradientForce<CloudType>::DUcDtInterp() const
{
  if (!DUcDtInterpPtr_.valid())
  {
    FatalErrorIn
    (
      "inline const mousse::interpolation<mousse::vector>&"
      "mousse::PressureGradientForce<CloudType>::DUcDtInterp() const"
    )   << "Carrier phase DUcDt interpolation object not set"
      << abort(FatalError);
  }
  return DUcDtInterpPtr_();
}
