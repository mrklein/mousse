// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "geometric_field.hpp"
#include "vol_mesh.hpp"
#include "surface_mesh.hpp"
// Member Functions 
template<class Type>
Type mousse::fieldValues::fieldValueDelta::applyOperation
(
  const Type& value1,
  const Type& value2
) const
{
  Type result = pTraits<Type>::zero;
  switch (operation_)
  {
    case opAdd:
    {
      result = value1 + value2;
      break;
    }
    case opSubtract:
    {
      result = value1 - value2;
      break;
    }
    case opMin:
    {
      result = min(value1, value2);
      break;
    }
    case opMax:
    {
      result = max(value1, value2);
      break;
    }
    case opAverage:
    {
      result = 0.5*(value1 + value2);
      break;
    }
    default:
    {
      FatalErrorIn
      (
        "Type mousse::fieldValues::fieldValueDelta::applyOperation"
        "("
          "const Type&, "
          "const Type&"
        ") const"
      )
        << "Unable to process operation "
        << operationTypeNames_[operation_]
        << abort(FatalError);
    }
  }
  return result;
}
template<class Type>
void mousse::fieldValues::fieldValueDelta::processFields(bool& found)
{
  typedef GeometricField<Type, fvPatchField, volMesh> vf;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> sf;
  const wordList& fields1 = source1Ptr_->fields();
  const dictionary& results1 = source1Ptr_->resultDict();
  const dictionary& results2 = source2Ptr_->resultDict();
  Type r1(pTraits<Type>::zero);
  Type r2(pTraits<Type>::zero);
  forAll(fields1, i)
  {
    const word& fieldName = fields1[i];
    if
    (
      (obr_.foundObject<vf>(fieldName) || obr_.foundObject<sf>(fieldName))
    && results2.found(fieldName)
    )
    {
      results1.lookup(fieldName) >> r1;
      results2.lookup(fieldName) >> r2;
      Type result = applyOperation(r1, r2);
      if (log_) Info<< "    " << operationTypeNames_[operation_]
        << "(" << fieldName << ") = " << result
        << endl;
      if (Pstream::master())
      {
        file()<< tab << result;
      }
      found = true;
    }
  }
}
