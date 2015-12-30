// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_solid_chemistry_model.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
/* * * * * * * * * * * * * * * private static data * * * * * * * * * * * * * */
namespace mousse
{
  defineTypeNameAndDebug(basicSolidChemistryModel, 0);
  defineRunTimeSelectionTable(basicSolidChemistryModel, fvMesh);
}
// Constructors 
mousse::basicSolidChemistryModel::basicSolidChemistryModel
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  basicChemistryModel(mesh, phaseName),
  solidThermo_(solidReactionThermo::New(mesh, phaseName))
{}
// Destructor 
mousse::basicSolidChemistryModel::~basicSolidChemistryModel()
{}
const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::basicSolidChemistryModel::RR(const label i) const
{
  notImplemented
  (
    "const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&"
    "basicSolidChemistryModel::RR(const label)"
  );
  return (DimensionedField<scalar, volMesh>::null());
}
mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::basicSolidChemistryModel::RR(const label i)
{
  notImplemented
  (
    "mousse::DimensionedField<mousse::scalar, mousse::volMesh>&"
    "basicSolidChemistryModel::RR(const label)"
  );
  return dynamic_cast<DimensionedField<scalar, volMesh>&>
  (
    const_cast<DimensionedField<scalar, volMesh>& >
    (
      DimensionedField<scalar, volMesh>::null()
    )
  );
}
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::basicSolidChemistryModel::calculateRR
(
  const label reactionI,
  const label speciei
) const
{
  notImplemented
  (
    "mousse::DimensionedField<mousse::scalar, mousse::volMesh>&"
    "basicSolidChemistryModel::calculateRR(const label)"
  );
  return dynamic_cast<tmp<DimensionedField<scalar, volMesh> >&>
  (
    const_cast<DimensionedField<scalar, volMesh>& >
    (
      DimensionedField<scalar, volMesh>::null()
    )
  );
}
