// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_radiation.hpp"
#include "vol_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(constantRadiation, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmRadiationModel,
  constantRadiation,
  dictionary
);


// Constructors 
constantRadiation::constantRadiation
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmRadiationModel{typeName, owner, dict},
  QrConst_
  {
    {
      typeName + ":QrConst",
      owner.time().timeName(),
      owner.regionMesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    owner.regionMesh()
  },
  mask_
  {
    {
      typeName + ":mask",
      owner.time().timeName(),
      owner.regionMesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    owner.regionMesh(),
    {"one", dimless, 1.0}
  },
  absorptivity_{readScalar(coeffDict_.lookup("absorptivity"))},
  timeStart_{readScalar(coeffDict_.lookup("timeStart"))},
  duration_{readScalar(coeffDict_.lookup("duration"))}
{
  mask_ = pos(mask_);
}


// Destructor 
constantRadiation::~constantRadiation()
{}


// Member Functions 
void constantRadiation::correct()
{}


tmp<volScalarField> constantRadiation::Shs()
{
  tmp<volScalarField> tShs
  {
    new volScalarField
    {
      {
        typeName + ":Shs",
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner().regionMesh(),
      {"zero", dimMass/pow3(dimTime), 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  const scalar time = owner().time().value();
  if ((time >= timeStart_) && (time <= timeStart_ + duration_)) {
    scalarField& Shs = tShs();
    const scalarField& Qr = QrConst_.internalField();
    const scalarField& alpha = owner_.alpha().internalField();
    Shs = mask_*Qr*alpha*absorptivity_;
  }
  return tShs;
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

