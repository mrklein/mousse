// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "region_model_1d.hpp"
inline const mousse::labelListList&
mousse::regionModels::regionModel1D::boundaryFaceFaces() const
{
  return boundaryFaceFaces_;
}
inline const mousse::labelListList&
mousse::regionModels::regionModel1D::boundaryFaceCells() const
{
  return boundaryFaceCells_;
}
inline const mousse::labelList&
mousse::regionModels::regionModel1D::boundaryFaceOppositeFace() const
{
  return boundaryFaceOppositeFace_;
}
inline const mousse::surfaceScalarField&
mousse::regionModels::regionModel1D::nMagSf() const
{
  if (!nMagSfPtr_.valid())
  {
    FatalErrorIn
    (
      "inline const mousse::surfaceScalarField&"
      "mousse::regionModel1Ds::regionModel1D::nMagSf() const"
    )<< "Face normal areas not available" << abort(FatalError);
  }
  return nMagSfPtr_();
}
inline mousse::label mousse::regionModels::regionModel1D::nLayers() const
{
  return nLayers_;
}
