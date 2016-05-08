// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

template<class Type>
void mousse::calcTypes::randomise::writeRandomField
(
  const IOobject& header,
  const scalar pertMag,
  Random& rand,
  const fvMesh& mesh,
  bool& processed
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> fieldType;
  if (header.headerClassName() != fieldType::typeName)
    return;
  Info << "    Reading " << header.name() << endl;
  fieldType field{header, mesh};
  FOR_ALL(field, cellI) {
    Type rndPert;
    rand.randomise(rndPert);
    rndPert = 2.0*rndPert - pTraits<Type>::one;
    rndPert /= mag(rndPert);
    field[cellI] += pertMag*rndPert;
  }
  fieldType randomisedField
  {
    IOobject
    {
      header.name() + "Random",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ
    },
    field
  };
  Info << "    Writing " << header.name() << "Random" << endl;
  randomisedField.write();
  processed = true;
}

