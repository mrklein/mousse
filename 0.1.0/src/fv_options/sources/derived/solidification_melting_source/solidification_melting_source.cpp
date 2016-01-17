// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solidification_melting_source.hpp"
#include "fv_matrices.hpp"
#include "basic_thermo.hpp"
#include "uniform_dimensioned_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "geometric_one_field.hpp"
// Static Member Functions
namespace mousse
{
  template<>
  const char* NamedEnum
  <
    fv::solidificationMeltingSource::thermoMode,
    2
  >::names[] =
  {
    "thermo",
    "lookup"
  };
  namespace fv
  {
    DEFINE_TYPE_NAME_AND_DEBUG(solidificationMeltingSource, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE
    (
      option,
      solidificationMeltingSource,
      dictionary
    );
  }
}
const mousse::NamedEnum<mousse::fv::solidificationMeltingSource::thermoMode, 2>
  mousse::fv::solidificationMeltingSource::thermoModeTypeNames_;
// Private Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::fv::solidificationMeltingSource::Cp() const
{
  switch (mode_)
  {
    case mdThermo:
    {
      const basicThermo& thermo =
        mesh_.lookupObject<basicThermo>(basicThermo::dictName);
      return thermo.Cp();
      break;
    }
    case mdLookup:
    {
      if (CpName_ == "CpRef")
      {
        scalar CpRef = readScalar(coeffs_.lookup("CpRef"));
        return tmp<volScalarField>
        (
          new volScalarField
          (
            IOobject
            (
              name_ + ":Cp",
              mesh_.time().timeName(),
              mesh_,
              IOobject::NO_READ,
              IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar
            (
              "Cp",
              dimEnergy/dimMass/dimTemperature,
              CpRef
            ),
            zeroGradientFvPatchScalarField::typeName
          )
        );
      }
      else
      {
        return mesh_.lookupObject<volScalarField>(CpName_);
      }
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "mousse::tmp<mousse::volScalarField> "
        "mousse::fv::solidificationMeltingSource::Cp() const"
      )
        << "Unhandled thermo mode: " << thermoModeTypeNames_[mode_]
        << abort(FatalError);
    }
  }
  return tmp<volScalarField>(NULL);
}
mousse::vector mousse::fv::solidificationMeltingSource::g() const
{
  if (mesh_.foundObject<uniformDimensionedVectorField>("g"))
  {
    const uniformDimensionedVectorField& value =
      mesh_.lookupObject<uniformDimensionedVectorField>("g");
    return value.value();
  }
  else
  {
    return coeffs_.lookup("g");
  }
}
void mousse::fv::solidificationMeltingSource::update(const volScalarField& Cp)
{
  if (curTimeIndex_ == mesh_.time().timeIndex())
  {
    return;
  }
  if (debug)
  {
    Info<< type() << ": " << name_ << " - updating phase indicator" << endl;
  }
  // update old time alpha1 field
  alpha1_.oldTime();
  const volScalarField& T = mesh_.lookupObject<volScalarField>(TName_);
  FOR_ALL(cells_, i)
  {
    label cellI = cells_[i];
    scalar Tc = T[cellI];
    scalar Cpc = Cp[cellI];
    scalar alpha1New = alpha1_[cellI] + relax_*Cpc*(Tc - Tmelt_)/L_;
    alpha1_[cellI] = max(0, min(alpha1New, 1));
    deltaT_[i] = Tc - Tmelt_;
  }
  alpha1_.correctBoundaryConditions();
  curTimeIndex_ = mesh_.time().timeIndex();
}
// Constructors 
mousse::fv::solidificationMeltingSource::solidificationMeltingSource
(
  const word& sourceName,
  const word& modelType,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  cellSetOption(sourceName, modelType, dict, mesh),
  Tmelt_(readScalar(coeffs_.lookup("Tmelt"))),
  L_(readScalar(coeffs_.lookup("L"))),
  relax_(coeffs_.lookupOrDefault("relax", 0.9)),
  mode_(thermoModeTypeNames_.read(coeffs_.lookup("thermoMode"))),
  rhoRef_(readScalar(coeffs_.lookup("rhoRef"))),
  TName_(coeffs_.lookupOrDefault<word>("TName", "T")),
  CpName_(coeffs_.lookupOrDefault<word>("CpName", "Cp")),
  UName_(coeffs_.lookupOrDefault<word>("UName", "U")),
  phiName_(coeffs_.lookupOrDefault<word>("phiName", "phi")),
  Cu_(coeffs_.lookupOrDefault<scalar>("Cu", 100000)),
  q_(coeffs_.lookupOrDefault("q", 0.001)),
  beta_(readScalar(coeffs_.lookup("beta"))),
  alpha1_
  (
    IOobject
    (
      name_ + ":alpha1",
      mesh.time().timeName(),
      mesh,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar("alpha1", dimless, 0.0),
    zeroGradientFvPatchScalarField::typeName
  ),
  curTimeIndex_(-1),
  deltaT_(cells_.size(), 0)
{
  fieldNames_.setSize(2);
  fieldNames_[0] = UName_;
  switch (mode_)
  {
    case mdThermo:
    {
      const basicThermo& thermo =
        mesh_.lookupObject<basicThermo>(basicThermo::dictName);
      fieldNames_[1] = thermo.he().name();
      break;
    }
    case mdLookup:
    {
      fieldNames_[1] = TName_;
      break;
    }
    default:
    {
      FATAL_ERROR_IN
      (
        "fv::solidificationMeltingSource::solidificationMeltingSource"
      )   << "Unhandled thermo mode: " << thermoModeTypeNames_[mode_]
        << abort(FatalError);
    }
  }
  applied_.setSize(2, false);
}
// Member Functions 
void mousse::fv::solidificationMeltingSource::addSup
(
  fvMatrix<scalar>& eqn,
  const label /*fieldI*/
)
{
  apply(geometricOneField(), eqn);
}
void mousse::fv::solidificationMeltingSource::addSup
(
  const volScalarField& rho,
  fvMatrix<scalar>& eqn,
  const label /*fieldI*/
)
{
  apply(rho, eqn);
}
void mousse::fv::solidificationMeltingSource::addSup
(
  fvMatrix<vector>& eqn,
  const label /*fieldI*/
)
{
  if (debug)
  {
    Info<< type() << ": applying source to " << eqn.psi().name() << endl;
  }
  const volScalarField Cp(this->Cp());
  update(Cp);
  vector g = this->g();
  scalarField& Sp = eqn.diag();
  vectorField& Su = eqn.source();
  const scalarField& V = mesh_.V();
  FOR_ALL(cells_, i)
  {
    label cellI = cells_[i];
    scalar Vc = V[cellI];
    scalar alpha1c = alpha1_[cellI];
    scalar S = -Cu_*sqr(1.0 - alpha1c)/(pow3(alpha1c) + q_);
    vector Sb = rhoRef_*g*beta_*deltaT_[i];
    Sp[cellI] += Vc*S;
    Su[cellI] += Vc*Sb;
  }
}
void mousse::fv::solidificationMeltingSource::addSup
(
  const volScalarField& /*rho*/,
  fvMatrix<vector>& eqn,
  const label fieldI
)
{
  // Momentum source uses a Boussinesq approximation - redirect
  addSup(eqn, fieldI);
}
