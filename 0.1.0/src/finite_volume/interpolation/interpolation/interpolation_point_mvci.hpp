// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
inline Type mousse::interpolationPointMVC<Type>::interpolate
(
  const pointMVCWeight& cpw
) const
{
  return cpw.interpolate(psip_);
}
template<class Type>
inline Type mousse::interpolationPointMVC<Type>::interpolate
(
  const vector& position,
  const label cellI,
  const label faceI
) const
{
  return interpolate
  (
    pointMVCWeight(this->pMesh_, position, cellI, faceI)
  );
}
