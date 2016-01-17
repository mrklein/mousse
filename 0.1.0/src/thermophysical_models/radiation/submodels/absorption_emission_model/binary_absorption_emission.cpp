// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "binary_absorption_emission.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace radiation
  {
    DEFINE_TYPE_NAME_AND_DEBUG(binaryAbsorptionEmission, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE
    (
      absorptionEmissionModel,
      binaryAbsorptionEmission,
      dictionary
    );
  }
}
// Constructors 
mousse::radiation::binaryAbsorptionEmission::binaryAbsorptionEmission
(
  const dictionary& dict,
  const fvMesh& mesh
)
:
  absorptionEmissionModel(dict, mesh),
  coeffsDict_(dict.subDict(typeName + "Coeffs")),
  model1_
  (
    absorptionEmissionModel::New(coeffsDict_.subDict("model1"), mesh)
  ),
  model2_
  (
    absorptionEmissionModel::New(coeffsDict_.subDict("model2"), mesh)
  )
{}
// Destructor 
mousse::radiation::binaryAbsorptionEmission::~binaryAbsorptionEmission()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::radiation::binaryAbsorptionEmission::aCont(const label bandI) const
{
  return model1_->aCont(bandI) + model2_->aCont(bandI);
}
mousse::tmp<mousse::volScalarField>
mousse::radiation::binaryAbsorptionEmission::aDisp(const label bandI) const
{
  return model1_->aDisp(bandI) + model2_->aDisp(bandI);
}
mousse::tmp<mousse::volScalarField>
mousse::radiation::binaryAbsorptionEmission::eCont(const label bandI) const
{
  return model1_->eCont(bandI) + model2_->eCont(bandI);
}
mousse::tmp<mousse::volScalarField>
mousse::radiation::binaryAbsorptionEmission::eDisp(const label bandI) const
{
  return model1_->eDisp(bandI) + model2_->eDisp(bandI);
}
mousse::tmp<mousse::volScalarField>
mousse::radiation::binaryAbsorptionEmission::ECont(const label bandI) const
{
  return model1_->ECont(bandI) + model2_->ECont(bandI);
}
mousse::tmp<mousse::volScalarField>
mousse::radiation::binaryAbsorptionEmission::EDisp(const label bandI) const
{
  return model1_->EDisp(bandI) + model2_->EDisp(bandI);
}
