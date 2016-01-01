// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::magSqr::writeMagSqrField
(
  const IOobject& header,
  const fvMesh& mesh,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (header.headerClassName() == fieldType::typeName)
  {
    Info<< "    Reading " << header.name() << endl;
    fieldType field(header, mesh);
    Info<< "    Calculating magSqr" << header.name() << endl;
    volScalarField magSqrField
    (
      IOobject
      (
        "magSqr" + header.name(),
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ
      ),
      mousse::magSqr(field)
    );
    magSqrField.write();
    processed = true;
  }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
