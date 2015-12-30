// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class CloudType>
inline const mousse::interpolation<mousse::vector>&
mousse::LiftForce<CloudType>::curlUcInterp() const
{
  if (!curlUcInterpPtr_.valid())
  {
    FatalErrorIn
    (
      "inline const mousse::interpolation<mousse::vector>&"
      "mousse::LiftForce<CloudType>::curlUcInterp() const"
    )   << "Carrier phase curlUc interpolation object not set"
      << abort(FatalError);
  }
  return curlUcInterpPtr_();
}
