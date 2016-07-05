// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo_single_layer.hpp"
#include "fvc_div.hpp"
#include "fvc_laplacian.hpp"
#include "fvm.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "mapped_field_fv_patch_field.hpp"
#include "map_distribute.hpp"
#include "constants.hpp"
// Sub-models
#include "film_thermo_model.hpp"
#include "film_viscosity_model.hpp"
#include "heat_transfer_model.hpp"
#include "phase_change_model.hpp"
#include "film_radiation_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(thermoSingleLayer, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(surfaceFilmModel, thermoSingleLayer, mesh);


// Private Member Functions 
wordList thermoSingleLayer::hsBoundaryTypes()
{
  wordList bTypes{T_.boundaryField().types()};
  FOR_ALL(bTypes, patchI) {
    if (T_.boundaryField()[patchI].fixesValue()
        || bTypes[patchI] == mappedFieldFvPatchField<scalar>::typeName) {
      bTypes[patchI] = fixedValueFvPatchField<scalar>::typeName;
    }
  }
  return bTypes;
}


// Protected Member Functions 
bool thermoSingleLayer::read()
{
  // no additional properties to read
  return kinematicSingleLayer::read();
}


void thermoSingleLayer::resetPrimaryRegionSourceTerms()
{
  if (debug) {
    Info << "thermoSingleLayer::resetPrimaryRegionSourceTerms()" << endl;
  }
  kinematicSingleLayer::resetPrimaryRegionSourceTerms();
  hsSpPrimary_ == dimensionedScalar("zero", hsSp_.dimensions(), 0.0);
}


void thermoSingleLayer::correctThermoFields()
{
  rho_ == filmThermo_->rho();
  sigma_ == filmThermo_->sigma();
  Cp_ == filmThermo_->Cp();
  kappa_ == filmThermo_->kappa();
}


void thermoSingleLayer::correctHsForMappedT()
{
  T_.correctBoundaryConditions();
  FOR_ALL(T_.boundaryField(), patchI) {
    const fvPatchField<scalar>& Tp = T_.boundaryField()[patchI];
    if (isA<mappedFieldFvPatchField<scalar>>(Tp)) {
      hs_.boundaryField()[patchI] == hs(Tp, patchI);
    }
  }
}


void thermoSingleLayer::updateSurfaceTemperatures()
{
  correctHsForMappedT();
  // Push boundary film temperature into wall temperature internal field
  for (label i=0; i<intCoupledPatchIDs_.size(); i++) {
    label patchI = intCoupledPatchIDs_[i];
    const polyPatch& pp = regionMesh().boundaryMesh()[patchI];
    UIndirectList<scalar>(Tw_, pp.faceCells()) = T_.boundaryField()[patchI];
  }
  Tw_.correctBoundaryConditions();
  // Update film surface temperature
  Ts_ = T_;
  Ts_.correctBoundaryConditions();
}


void thermoSingleLayer::transferPrimaryRegionThermoFields()
{
  if (debug) {
    Info << "thermoSingleLayer::transferPrimaryRegionThermoFields()" << endl;
  }
  kinematicSingleLayer::transferPrimaryRegionThermoFields();
  // Update primary region fields on local region via direct mapped (coupled)
  // boundary conditions
  TPrimary_.correctBoundaryConditions();
  FOR_ALL(YPrimary_, i) {
    YPrimary_[i].correctBoundaryConditions();
  }
}


void thermoSingleLayer::transferPrimaryRegionSourceFields()
{
  if (debug) {
    Info << "thermoSingleLayer::transferPrimaryRegionSourceFields()" << endl;
  }
  kinematicSingleLayer::transferPrimaryRegionSourceFields();
  // Convert accummulated source terms into per unit area per unit time
  const scalar deltaT = time_.deltaTValue();
  FOR_ALL(hsSpPrimary_.boundaryField(), patchI) {
    const scalarField& priMagSf =
      primaryMesh().magSf().boundaryField()[patchI];
    hsSpPrimary_.boundaryField()[patchI] /= priMagSf*deltaT;
  }
  // Retrieve the source fields from the primary region via direct mapped
  // (coupled) boundary conditions
  // - fields require transfer of values for both patch AND to push the
  //   values into the first layer of internal cells
  hsSp_.correctBoundaryConditions();
  // Apply enthalpy source as difference between incoming and actual states
  hsSp_ -= rhoSp_*hs_;
}


void thermoSingleLayer::correctAlpha()
{
  if (hydrophilic_) {
    const scalar hydrophilicDry = hydrophilicDryScale_*deltaWet_;
    const scalar hydrophilicWet = hydrophilicWetScale_*deltaWet_;
    FOR_ALL(alpha_, i) {
      if ((alpha_[i] < 0.5) && (delta_[i] > hydrophilicWet)) {
        alpha_[i] = 1.0;
      } else if ((alpha_[i] > 0.5) && (delta_[i] < hydrophilicDry)) {
        alpha_[i] = 0.0;
      }
    }
    alpha_.correctBoundaryConditions();
  } else {
    alpha_ ==
      pos(delta_ - dimensionedScalar("deltaWet", dimLength, deltaWet_));
  }
}


void thermoSingleLayer::updateSubmodels()
{
  if (debug) {
    Info << "thermoSingleLayer::updateSubmodels()" << endl;
  }
  // Update heat transfer coefficient sub-models
  htcs_->correct();
  htcw_->correct();
  phaseChange_->correct
  (
    time_.deltaTValue(),
    availableMass_,
    primaryMassPCTrans_,
    primaryEnergyPCTrans_
  );
  // Update radiation
  radiation_->correct();
  // Update kinematic sub-models
  kinematicSingleLayer::updateSubmodels();
  // Update source fields
  hsSp_ += primaryEnergyPCTrans_/magSf()/time().deltaT();
  rhoSp_ += primaryMassPCTrans_/magSf()/time().deltaT();
  // Vapour recoil pressure
  pSp_ -= sqr(primaryMassPCTrans_/magSf()/time().deltaT())/2.0/rhoPrimary_;
}


tmp<fvScalarMatrix> thermoSingleLayer::q(volScalarField& hs) const
{
  return
    (
      - fvm::Sp(htcs_->h()/Cp_, hs)
      - htcs_->h()*(constant::standard::Tstd - TPrimary_)
      - fvm::Sp(htcw_->h()/Cp_, hs)
      - htcw_->h()*(constant::standard::Tstd - Tw_)
    );
}


void thermoSingleLayer::solveEnergy()
{
  if (debug) {
    Info << "thermoSingleLayer::solveEnergy()" << endl;
  }
  updateSurfaceTemperatures();
  solve
  (
    fvm::ddt(deltaRho_, hs_)
  + fvm::div(phi_, hs_)
  ==
  - hsSp_
  + q(hs_)
  + radiation_->Shs()
  - rhoSp_*hs_
  );
  correctThermoFields();
  // evaluate viscosity from user-model
  viscosity_->correct(pPrimary_, T_);
}


// Constructors 
thermoSingleLayer::thermoSingleLayer
(
  const word& modelType,
  const fvMesh& mesh,
  const dimensionedVector& g,
  const word& regionType,
  const bool readFields
)
:
  kinematicSingleLayer{modelType, mesh, g, regionType, false},
  thermo_{mesh.lookupObject<SLGThermo>("SLGThermo")},
  Cp_
  {
    {
      "Cp",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh(),
    {"Cp", dimEnergy/dimMass/dimTemperature, 0.0},
    zeroGradientFvPatchScalarField::typeName
  },
  kappa_
  {
    {
      "kappa",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh(),
    {"kappa", dimEnergy/dimTime/dimLength/dimTemperature, 0.0},
    zeroGradientFvPatchScalarField::typeName
  },
  T_
  {
    {
      "Tf",
      time().timeName(),
      regionMesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    regionMesh()
  },
  Ts_
  {
    {
      "Tsf",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    T_,
    zeroGradientFvPatchScalarField::typeName
  },
  Tw_
  {
    {
      "Twf",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    T_,
    zeroGradientFvPatchScalarField::typeName
  },
  hs_
  {
    {
      "hf",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    regionMesh(),
    {"zero", dimEnergy/dimMass, 0.0},
    hsBoundaryTypes()
  },
  primaryMassPCTrans_
  {
    {
      "primaryMassPCTrans",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    regionMesh(),
    {"zero", dimMass, 0},
    zeroGradientFvPatchScalarField::typeName
  },
  primaryEnergyPCTrans_
  {
    {
      "primaryEnergyPCTrans",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    regionMesh(),
    {"zero", dimEnergy, 0},
    zeroGradientFvPatchScalarField::typeName
  },
  deltaWet_{readScalar(coeffs_.lookup("deltaWet"))},
  hydrophilic_{readBool(coeffs_.lookup("hydrophilic"))},
  hydrophilicDryScale_{0.0},
  hydrophilicWetScale_{0.0},
  hsSp_
  {
    {
      "hsSp",
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    regionMesh(),
    {"zero", dimEnergy/dimArea/dimTime, 0.0},
    this->mappedPushedFieldPatchTypes<scalar>()
  },
  hsSpPrimary_
  {
    {
      hsSp_.name(), // must have same name as hSp_ to enable mapping
      time().timeName(),
      primaryMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    primaryMesh(),
    {"zero", hsSp_.dimensions(), 0.0}
  },
  TPrimary_
  {
    {
      "T", // same name as T on primary region to enable mapping
      time().timeName(),
      regionMesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    regionMesh(),
    {"zero", dimTemperature, 0.0},
    this->mappedFieldAndInternalPatchTypes<scalar>()
  },
  YPrimary_{},
  viscosity_{filmViscosityModel::New(*this, coeffs(), mu_)},
  htcs_
  {
    heatTransferModel::New(*this, coeffs().subDict("upperSurfaceModels"))
  },
  htcw_
  {
    heatTransferModel::New(*this, coeffs().subDict("lowerSurfaceModels"))
  },
  phaseChange_{phaseChangeModel::New(*this, coeffs())},
  radiation_{filmRadiationModel::New(*this, coeffs())},
  Tmin_{-VGREAT},
  Tmax_{VGREAT}
{
  if (coeffs().readIfPresent("Tmin", Tmin_)) {
    Info << "    limiting minimum temperature to " << Tmin_ << endl;
  }
  if (coeffs().readIfPresent("Tmax", Tmax_)) {
    Info << "    limiting maximum temperature to " << Tmax_ << endl;
  }
  if (thermo_.hasMultiComponentCarrier()) {
    YPrimary_.setSize(thermo_.carrier().species().size());
    FOR_ALL(thermo_.carrier().species(), i) {
      YPrimary_.set
      (
        i,
        new volScalarField
        {
          {
            thermo_.carrier().species()[i],
            time().timeName(),
            regionMesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
          },
          regionMesh(),
          {"zero", dimless, 0.0},
          pSp_.boundaryField().types()
        }
      );
    }
  }
  if (hydrophilic_) {
    coeffs_.lookup("hydrophilicDryScale") >> hydrophilicDryScale_;
    coeffs_.lookup("hydrophilicWetScale") >> hydrophilicWetScale_;
  }
  if (readFields) {
    transferPrimaryRegionThermoFields();
    correctAlpha();
    correctThermoFields();
    // Update derived fields
    hs_ == hs(T_);
    deltaRho_ == delta_*rho_;
    surfaceScalarField phi0
    {
      {
        "phi",
        time().timeName(),
        regionMesh(),
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE,
        false
      },
      fvc::interpolate(deltaRho_*U_) & regionMesh().Sf()
    };
    phi_ == phi0;
    // evaluate viscosity from user-model
    viscosity_->correct(pPrimary_, T_);
  }
}


// Destructor 
thermoSingleLayer::~thermoSingleLayer()
{}


// Member Functions 
void thermoSingleLayer::addSources
(
  const label patchI,
  const label faceI,
  const scalar massSource,
  const vector& momentumSource,
  const scalar pressureSource,
  const scalar energySource
)
{
  kinematicSingleLayer::addSources
  (
    patchI,
    faceI,
    massSource,
    momentumSource,
    pressureSource,
    energySource
  );
  if (debug) {
    Info << "    energy   = " << energySource << nl << endl;
  }
  hsSpPrimary_.boundaryField()[patchI][faceI] -= energySource;
}


void thermoSingleLayer::preEvolveRegion()
{
  if (debug) {
    Info << "thermoSingleLayer::preEvolveRegion()" << endl;
  }
  kinematicSingleLayer::preEvolveRegion();
  // Update phase change
  primaryMassPCTrans_ == dimensionedScalar{"zero", dimMass, 0.0};
  primaryEnergyPCTrans_ == dimensionedScalar{"zero", dimEnergy, 0.0};
}


void thermoSingleLayer::evolveRegion()
{
  if (debug) {
    Info << "thermoSingleLayer::evolveRegion()" << endl;
  }
  // Update film coverage indicator
  correctAlpha();
  // Update film wall and surface velocities
  updateSurfaceVelocities();
  // Update film wall and surface temperatures
  updateSurfaceTemperatures();
  // Update sub-models to provide updated source contributions
  updateSubmodels();
  // Solve continuity for deltaRho_
  solveContinuity();
  for (int oCorr=1; oCorr<=nOuterCorr_; oCorr++) {
    // Explicit pressure source contribution
    tmp<volScalarField> tpu{this->pu()};
    // Implicit pressure source coefficient
    tmp<volScalarField> tpp{this->pp()};
    // Solve for momentum for U_
    tmp<fvVectorMatrix> UEqn = solveMomentum(tpu(), tpp());
    // Solve energy for hs_ - also updates thermo
    solveEnergy();
    // Film thickness correction loop
    for (int corr=1; corr<=nCorr_; corr++) {
      // Solve thickness for delta_
      solveThickness(tpu(), tpp(), UEqn());
    }
  }
  // Update deltaRho_ with new delta_
  deltaRho_ == delta_*rho_;
  // Update temperature using latest hs_
  T_ == T(hs_);
  // Reset source terms for next time integration
  resetPrimaryRegionSourceTerms();
}


const volScalarField& thermoSingleLayer::Cp() const
{
  return Cp_;
}


const volScalarField& thermoSingleLayer::kappa() const
{
  return kappa_;
}


const volScalarField& thermoSingleLayer::T() const
{
  return T_;
}


const volScalarField& thermoSingleLayer::Ts() const
{
  return Ts_;
}


const volScalarField& thermoSingleLayer::Tw() const
{
  return Tw_;
}


const volScalarField& thermoSingleLayer::hs() const
{
  return hs_;
}


tmp<volScalarField> thermoSingleLayer::primaryMassTrans() const
{
  return primaryMassPCTrans_;
}


void thermoSingleLayer::info()
{
  kinematicSingleLayer::info();
  const scalarField& Tinternal = T_.internalField();
  Info << indent << "min/max(T)         = " << gMin(Tinternal) << ", "
    << gMax(Tinternal) << nl;
  phaseChange_->info(Info);
}


tmp<DimensionedField<scalar, volMesh>> thermoSingleLayer::Srho() const
{
  tmp<DimensionedField<scalar, volMesh>> tSrho
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        "thermoSingleLayer::Srho",
        time().timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      primaryMesh(),
      {"zero", dimMass/dimVolume/dimTime, 0.0}
    }
  };
  scalarField& Srho = tSrho();
  const scalarField& V = primaryMesh().V();
  const scalar dt = time_.deltaTValue();
  FOR_ALL(intCoupledPatchIDs(), i) {
    const label filmPatchI = intCoupledPatchIDs()[i];
    scalarField patchMass =
      primaryMassPCTrans_.boundaryField()[filmPatchI];
    toPrimary(filmPatchI, patchMass);
    const label primaryPatchI = primaryPatchIDs()[i];
    const unallocLabelList& cells =
      primaryMesh().boundaryMesh()[primaryPatchI].faceCells();
    FOR_ALL(patchMass, j) {
      Srho[cells[j]] = patchMass[j]/(V[cells[j]]*dt);
    }
  }
  return tSrho;
}


tmp<DimensionedField<scalar, volMesh>> thermoSingleLayer::Srho
(
  const label i
) const
{
  const label vapId = thermo_.carrierId(filmThermo_->name());
  tmp<DimensionedField<scalar, volMesh>> tSrho
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        "thermoSingleLayer::Srho(" + mousse::name(i) + ")",
        time_.timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      primaryMesh(),
      {"zero", dimMass/dimVolume/dimTime, 0.0}
    }
  };
  if (vapId == i) {
    scalarField& Srho = tSrho();
    const scalarField& V = primaryMesh().V();
    const scalar dt = time().deltaTValue();
    FOR_ALL(intCoupledPatchIDs_, i) {
      const label filmPatchI = intCoupledPatchIDs_[i];
      scalarField patchMass =
        primaryMassPCTrans_.boundaryField()[filmPatchI];
      toPrimary(filmPatchI, patchMass);
      const label primaryPatchI = primaryPatchIDs()[i];
      const unallocLabelList& cells =
        primaryMesh().boundaryMesh()[primaryPatchI].faceCells();
      FOR_ALL(patchMass, j) {
        Srho[cells[j]] = patchMass[j]/(V[cells[j]]*dt);
      }
    }
  }
  return tSrho;
}


tmp<DimensionedField<scalar, volMesh>> thermoSingleLayer::Sh() const
{
  tmp<DimensionedField<scalar, volMesh>> tSh
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        "thermoSingleLayer::Sh",
        time().timeName(),
        primaryMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      primaryMesh(),
      {"zero", dimEnergy/dimVolume/dimTime, 0.0}
    }
  };
  return tSh;
}


} // end namespace mousse
} // end namespace regionModels
} // end namespace surfaceFilmModels