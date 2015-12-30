// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::div::writeDivField
(
  const IOobject& header,
  const fvMesh& mesh,
  bool& processed
)
{
  if (header.headerClassName() == Type::typeName)
  {
    Info<< "    Reading " << header.name() << endl;
    Type field(header, mesh);
    Info<< "    Calculating div" << header.name() << endl;
    volScalarField divField
    (
      IOobject
      (
        "div" + header.name(),
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ
      ),
      fvc::div(field)
    );
    divField.write();
    processed = true;
  }
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
