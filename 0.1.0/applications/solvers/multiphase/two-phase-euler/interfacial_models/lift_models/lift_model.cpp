// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lift_model.hpp"
#include "phase_pair.hpp"
#include "fvc_curl.hpp"
#include "surface_interpolate.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(liftModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(liftModel, dictionary);
}
const mousse::dimensionSet mousse::liftModel::dimF{1, -2, -2, 0, 0};
// Constructors 
mousse::liftModel::liftModel
(
  const dictionary& /*dict*/,
  const phasePair& pair
)
:
  pair_{pair}
{}
// Destructor 
mousse::liftModel::~liftModel()
{}
// Member Functions 
mousse::tmp<mousse::volVectorField> mousse::liftModel::Fi() const
{
  return
    Cl()*pair_.continuous().rho()*(
      pair_.Ur() ^ fvc::curl(pair_.continuous().U())
    );
}
mousse::tmp<mousse::volVectorField> mousse::liftModel::F() const
{
  return pair_.dispersed()*Fi();
}
mousse::tmp<mousse::surfaceScalarField> mousse::liftModel::Ff() const
{
  const fvMesh& mesh = this->pair_.phase1().mesh();
  return
    fvc::interpolate(pair_.dispersed())
    *(fvc::interpolate(Fi()) & mesh.Sf());
}
