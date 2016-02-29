// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace PDRDragModels
{
DEFINE_TYPE_NAME_AND_DEBUG(basic, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(PDRDragModel, basic, dictionary);
}
}
// Constructors 
mousse::PDRDragModels::basic::basic
(
  const dictionary& PDRProperties,
  const compressible::RASModel& turbulence,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& phi
)
:
  PDRDragModel{PDRProperties, turbulence, rho, U, phi},
  Csu{"Csu", dimless, PDRDragModelCoeffs_},
  Csk{"Csk", dimless, PDRDragModelCoeffs_},
  Aw_
  {
    {
      "Aw",
      U_.mesh().facesInstance(),
      U_.mesh(),
      IOobject::MUST_READ
    },
    U_.mesh()
  },
  CR_
  {
    {
      "CR",
      U_.mesh().facesInstance(),
      U_.mesh(),
      IOobject::MUST_READ
    },
    U_.mesh()
  }
{}
// Destructor 
mousse::PDRDragModels::basic::~basic()
{}
// Member Functions 
mousse::tmp<mousse::volSymmTensorField> mousse::PDRDragModels::basic::Dcu() const
{
  tmp<volSymmTensorField> tDragDcu
  {
    new volSymmTensorField
    {
      {
        "tDragDcu",
        U_.mesh().time().constant(),
        U_.mesh()
      },
      U_.mesh(),
      {"zero", dimMass/dimTime/pow(dimLength, 3), pTraits<symmTensor>::zero},
      zeroGradientFvPatchSymmTensorField::typeName
    }
  };
  volSymmTensorField& DragDcu = tDragDcu();
  if (on_)
  {
    const volScalarField& betav = U_.db().lookupObject<volScalarField>("betav");
    DragDcu =
      (0.5*rho_)*CR_*mag(U_) + (Csu*I)*betav*turbulence_.muEff()*sqr(Aw_);
  }
  return tDragDcu;
}
mousse::tmp<mousse::volScalarField> mousse::PDRDragModels::basic::Gk() const
{
  tmp<volScalarField> tGk
  {
    new volScalarField
    {
      {
        "tGk",
        U_.mesh().time().constant(),
        U_.mesh()
      },
      U_.mesh(),
      {"zero", dimMass/dimLength/pow(dimTime, 3), 0.0},
      zeroGradientFvPatchVectorField::typeName
    }
  };
  volScalarField& Gk = tGk();
  if (on_)
  {
    const volScalarField& betav =
      U_.db().lookupObject<volScalarField>("betav");
    const volSymmTensorField& CT =
      U_.db().lookupObject<volSymmTensorField>("CT");
    Gk =
      (0.5*rho_)*mag(U_)*(U_ & CT & U_)
      + Csk*betav*turbulence_.muEff()*sqr(Aw_)*magSqr(U_);
  }
  return tGk;
}
bool mousse::PDRDragModels::basic::read(const dictionary& PDRProperties)
{
  PDRDragModel::read(PDRProperties);
  PDRDragModelCoeffs_.lookup("Csu") >> Csu.value();
  PDRDragModelCoeffs_.lookup("Csk") >> Csk.value();
  return true;
}
void mousse::PDRDragModels::basic::writeFields() const
{
  Aw_.write();
  CR_.write();
}
