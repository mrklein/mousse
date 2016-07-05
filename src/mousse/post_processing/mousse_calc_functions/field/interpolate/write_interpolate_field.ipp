// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::interpolate::writeInterpolateField
(
  const IOobject& header,
  const fvMesh& mesh,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  typedef GeometricField<Type, fvsPatchField, surfaceMesh> surfaceFieldType;
  if (header.headerClassName() != fieldType::typeName)
    return;
  Info << "    Reading " << header.name() << endl;
  fieldType field{header, mesh};
  Info << "    Calculating interpolate" << header.name() << endl;
  surfaceFieldType interpolateField
  {
    IOobject
    {
      "interpolate" + header.name(),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ
    },
    fvc::interpolate(field)
  };
  interpolateField.write();
  processed = true;
}

