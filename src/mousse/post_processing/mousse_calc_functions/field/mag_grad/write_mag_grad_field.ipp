// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::magGrad::writeMagGradField
(
  const IOobject& header,
  const fvMesh& mesh,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (header.headerClassName() != fieldType::typeName)
    return;
  Info << "    Reading " << header.name() << endl;
  fieldType field{header, mesh};
  Info << "    Calculating magGrad" << header.name() << endl;
  volScalarField magGradField
  {
    IOobject
    {
      "magGrad" + header.name(),
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ
    },
    mousse::mag(fvc::grad(field))
  };
  magGradField.write();
  processed = true;
}

