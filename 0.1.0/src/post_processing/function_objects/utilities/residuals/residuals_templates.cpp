// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "residuals.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"
#include "time.hpp"
#include "list_ops.hpp"
template<class Type>
void mousse::residuals::writeResidual
(
  const word& fieldName
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (obr_.foundObject<fieldType>(fieldName))
  {
    const fieldType& field = obr_.lookupObject<fieldType>(fieldName);
    const fvMesh& mesh = field.mesh();
    const mousse::dictionary& solverDict = mesh.solverPerformanceDict();
    if (solverDict.found(fieldName))
    {
      const List<solverPerformance> sp(solverDict.lookup(fieldName));
      const scalar residual = sp.first().initialResidual();
      file() << token::TAB << residual;
    }
  }
}
