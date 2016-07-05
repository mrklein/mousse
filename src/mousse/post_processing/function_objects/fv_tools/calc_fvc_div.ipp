// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "fvc_div.hpp"


// Member Functions 
template<class FieldType>
void mousse::calcFvcDiv::calcDiv
(
  const word& fieldName,
  const word& resultName,
  bool& processed
)
{
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  if (!mesh.foundObject<FieldType>(fieldName))
    return;
  const FieldType& vf = mesh.lookupObject<FieldType>(fieldName);
  volScalarField& field = divField(resultName, vf.dimensions());
  field = fvc::div(vf);
  processed = true;
}

