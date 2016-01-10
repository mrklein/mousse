// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ensight_parts.hpp"
template<class Type>
void mousse::ensightParts::writeField
(
  ensightFile& os,
  const GeometricField<Type, fvPatchField, volMesh>& field
) const
{
  // find offset to patch parts (ie, the first face data)
  label patchOffset = 0;
  FOR_ALL(partsList_, partI)
  {
    if (partsList_[partI].isFaceData())
    {
      patchOffset = partI;
      break;
    }
  }
  FOR_ALL(partsList_, partI)
  {
    label patchI = partI - patchOffset;
    if (partsList_[partI].isCellData())
    {
      partsList_[partI].writeField
      (
        os,
        field
      );
    }
    else if (patchI < field.boundaryField().size())
    {
      partsList_[partI].writeField
      (
        os,
        field.boundaryField()[patchI]
      );
    }
  }
}
