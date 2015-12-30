// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh.hpp"
#include "fvc_grad.hpp"
// Private Member Functions 
template<class Type>
mousse::GeometricField
<
  typename mousse::outerProduct<mousse::vector, Type>::type,
  mousse::fvPatchField,
  mousse::volMesh
>&
mousse::calcFvcGrad::gradField(const word& gradName, const dimensionSet& dims)
{
  Info<< "gradField" << endl;
  typedef typename outerProduct<vector, Type>::type gradType;
  typedef GeometricField<gradType, fvPatchField, volMesh> vfGradType;
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  if (!mesh.foundObject<vfGradType>(gradName))
  {
    vfGradType* gradFieldPtr
    (
      new vfGradType
      (
        IOobject
        (
          gradName,
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensioned<gradType>
        (
          "zero",
          dims/dimLength,
          pTraits<gradType>::zero
        )
      )
    );
    mesh.objectRegistry::store(gradFieldPtr);
  }
  const vfGradType& field = mesh.lookupObject<vfGradType>(gradName);
  return const_cast<vfGradType&>(field);
}
template<class Type>
void mousse::calcFvcGrad::calcGrad
(
  const word& fieldName,
  const word& resultName,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> vfType;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;
  typedef typename outerProduct<vector, Type>::type gradType;
  typedef GeometricField<gradType, fvPatchField, volMesh> vfGradType;
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  if (mesh.foundObject<vfType>(fieldName))
  {
    const vfType& vf = mesh.lookupObject<vfType>(fieldName);
    vfGradType& field = gradField<Type>(resultName, vf.dimensions());
    // De-reference the tmp to avoid a clash with the cached grad field
    field = fvc::grad(vf)();
    processed = true;
  }
  else if (mesh.foundObject<sfType>(fieldName))
  {
    const sfType& sf = mesh.lookupObject<sfType>(fieldName);
    vfGradType& field = gradField<Type>(resultName, sf.dimensions());
    // De-reference the tmp to avoid a clash with the cached grad field
    field = fvc::grad(sf)();
    processed = true;
  }
}
