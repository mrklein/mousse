// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_specie_mixture.hpp"
// Static Member Functions
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(basicSpecieMixture, 0);
}
// Constructors 
mousse::basicSpecieMixture::basicSpecieMixture
(
  const dictionary& thermoDict,
  const wordList& specieNames,
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicMultiComponentMixture(thermoDict, specieNames, mesh, phaseName)
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::basicSpecieMixture::W() const
{
  const PtrList<volScalarField>& Y(basicMultiComponentMixture::Y());
  tmp<volScalarField> trW
  (
    new volScalarField
    (
      IOobject
      (
        IOobject::groupName("W", Y[0].group()),
        Y[0].time().timeName(),
        Y[0].mesh()
      ),
      Y[0].mesh(),
      dimensionedScalar("zero", dimless, 0)
    )
  );
  volScalarField& rW = trW();
  FOR_ALL(Y, i)
  {
    rW += Y[i]/W(i);
  }
  rW = 1.0/rW;
  return trW;
}
