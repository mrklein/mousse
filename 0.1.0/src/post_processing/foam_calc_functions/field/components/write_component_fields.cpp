// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::components::writeComponentFields
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
    for (direction i=0; i<Type::nComponents; i++)
    {
      Info<< "    Calculating " << header.name()
        << Type::componentNames[i] << endl;
      volScalarField componentField
      (
        IOobject
        (
          header.name() + word(Type::componentNames[i]),
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ
        ),
        field.component(i)
      );
      componentField.write();
    }
    processed = true;
  }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
