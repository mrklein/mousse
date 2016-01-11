// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_chemistry_model.hpp"
#include "reacting_mixture.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
// Constructors 
template<class CompType, class SolidThermo>
mousse::solidChemistryModel<CompType, SolidThermo>::
solidChemistryModel
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  CompType(mesh, phaseName),
  ODESystem(),
  Ys_(this->solidThermo().composition().Y()),
  reactions_
  (
    dynamic_cast<const reactingMixture<SolidThermo>& >
    (
      this->solidThermo()
    )
  ),
  solidThermo_
  (
    dynamic_cast<const reactingMixture<SolidThermo>& >
    (
      this->solidThermo()
    ).speciesData()
  ),
  nSolids_(Ys_.size()),
  nReaction_(reactions_.size()),
  RRs_(nSolids_),
  reactingCells_(mesh.nCells(), true)
{
  // create the fields for the chemistry sources
  FOR_ALL(RRs_, fieldI)
  {
    RRs_.set
    (
      fieldI,
      new DimensionedField<scalar, volMesh>
      (
        IOobject
        (
          "RRs." + Ys_[fieldI].name(),
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
      )
    );
 }
}
// Destructor 
template<class CompType, class SolidThermo>
mousse::solidChemistryModel<CompType, SolidThermo>::
~solidChemistryModel()
{}
// Member Functions 
template<class CompType, class SolidThermo>
mousse::scalar mousse::solidChemistryModel<CompType, SolidThermo>::solve
(
  const scalarField& /*deltaT*/
)
{
  NOT_IMPLEMENTED
  (
    "solidChemistryModel::solve(const scalarField& deltaT)"
  );
  return 0;
}
template<class CompType, class SolidThermo>
mousse::tmp<mousse::volScalarField>
mousse::solidChemistryModel<CompType, SolidThermo>::tc() const
{
  NOT_IMPLEMENTED
  (
    "solidChemistryModel::tc()"
  );
  return volScalarField::null();
}
template<class CompType, class SolidThermo>
mousse::tmp<mousse::volScalarField>
mousse::solidChemistryModel<CompType, SolidThermo>::Sh() const
{
  tmp<volScalarField> tSh
  (
    new volScalarField
    (
      IOobject
      (
        "Sh",
        this->mesh_.time().timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE,
        false
      ),
      this->mesh_,
      dimensionedScalar("zero", dimEnergy/dimTime/dimVolume, 0.0),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  if (this->chemistry_)
  {
    scalarField& Sh = tSh();
    FOR_ALL(Ys_, i)
    {
      FOR_ALL(Sh, cellI)
      {
        scalar hf = solidThermo_[i].Hc();
        Sh[cellI] -= hf*RRs_[i][cellI];
      }
    }
  }
  return tSh;
}
template<class CompType, class SolidThermo>
mousse::tmp<mousse::volScalarField>
mousse::solidChemistryModel<CompType, SolidThermo>::dQ() const
{
  tmp<volScalarField> tdQ
  (
    new volScalarField
    (
      IOobject
      (
        "dQ",
        this->mesh_.time().timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh_,
      dimensionedScalar("dQ", dimEnergy/dimTime, 0.0),
      zeroGradientFvPatchScalarField::typeName
    )
  );
  if (this->chemistry_)
  {
    volScalarField& dQ = tdQ();
    dQ.dimensionedInternalField() = this->mesh_.V()*Sh()();
  }
  return tdQ;
}
template<class CompType, class SolidThermo>
void mousse::solidChemistryModel<CompType, SolidThermo>::setCellReacting
(
  const label cellI,
  const bool active
)
{
  reactingCells_[cellI] = active;
}
