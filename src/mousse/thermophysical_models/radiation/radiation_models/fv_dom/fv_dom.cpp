// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_dom.hpp"
#include "absorption_emission_model.hpp"
#include "scatter_model.hpp"
#include "constants.hpp"
#include "fvm.hpp"
#include "add_to_run_time_selection_table.hpp"


using namespace mousse::constant;
using namespace mousse::constant::mathematical;

// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(fvDOM, 0);
ADD_TO_RADIATION_RUN_TIME_SELECTION_TABLES(fvDOM);

}
}


// Private Member Functions 
void mousse::radiation::fvDOM::initialise()
{
  // 3D
  if (mesh_.nSolutionD() == 3) {
    nRay_ = 4*nPhi_*nTheta_;
    IRay_.setSize(nRay_);
    scalar deltaPhi = pi/(2.0*nPhi_);
    scalar deltaTheta = pi/nTheta_;
    label i = 0;
    for (label n = 1; n <= nTheta_; n++) {
      for (label m = 1; m <= 4*nPhi_; m++) {
        scalar thetai = (2.0*n - 1.0)*deltaTheta/2.0;
        scalar phii = (2.0*m - 1.0)*deltaPhi/2.0;
        IRay_.set
        (
          i,
          new radiativeIntensityRay
          {
            *this,
            mesh_,
            phii,
            thetai,
            deltaPhi,
            deltaTheta,
            nLambda_,
            absorptionEmission_,
            blackBody_,
            i
          }
        );
        i++;
      }
    }
  } else if (mesh_.nSolutionD() == 2) {  // 2D
    // Currently 2D solution is limited to the x-y plane
    if (mesh_.solutionD()[vector::Z] != -1) {
      FATAL_ERROR_IN("fvDOM::initialise()")
        << "Currently 2D solution is limited to the x-y plane"
        << exit(FatalError);
    }
    scalar thetai = piByTwo;
    scalar deltaTheta = pi;
    nRay_ = 4*nPhi_;
    IRay_.setSize(nRay_);
    scalar deltaPhi = pi/(2.0*nPhi_);
    label i = 0;
    for (label m = 1; m <= 4*nPhi_; m++) {
      scalar phii = (2.0*m - 1.0)*deltaPhi/2.0;
      IRay_.set
      (
        i,
        new radiativeIntensityRay
        {
          *this,
          mesh_,
          phii,
          thetai,
          deltaPhi,
          deltaTheta,
          nLambda_,
          absorptionEmission_,
          blackBody_,
          i
        }
      );
      i++;
    }
  } else {  // 1D
    // Currently 1D solution is limited to the x-direction
    if (mesh_.solutionD()[vector::X] != 1) {
      FATAL_ERROR_IN("fvDOM::initialise()")
        << "Currently 1D solution is limited to the x-direction"
        << exit(FatalError);
    }
    scalar thetai = piByTwo;
    scalar deltaTheta = pi;
    nRay_ = 2;
    IRay_.setSize(nRay_);
    scalar deltaPhi = pi;
    label i = 0;
    for (label m = 1; m <= 2; m++) {
      scalar phii = (2.0*m - 1.0)*deltaPhi/2.0;
      IRay_.set
      (
        i,
        new radiativeIntensityRay
        {
          *this,
          mesh_,
          phii,
          thetai,
          deltaPhi,
          deltaTheta,
          nLambda_,
          absorptionEmission_,
          blackBody_,
          i
        }
      );
      i++;
    }
  }
  // Construct absorption field for each wavelength
  FOR_ALL(aLambda_, lambdaI) {
    aLambda_.set
    (
      lambdaI,
      new volScalarField
      {
        {
          "aLambda_" + mousse::name(lambdaI) ,
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        a_
      }
    );
  }
  Info << "fvDOM : Allocated " << IRay_.size()
    << " rays with average orientation:" << nl;
  if (cacheDiv_) {
    Info << "Caching div fvMatrix..."<< endl;
    for (label lambdaI = 0; lambdaI < nLambda_; lambdaI++) {
      fvRayDiv_[lambdaI].setSize(nRay_);
      FOR_ALL(IRay_, rayId) {
        const surfaceScalarField Ji(IRay_[rayId].dAve() & mesh_.Sf());
        const volScalarField& iRayLambdaI = IRay_[rayId].ILambda(lambdaI);
        fvRayDiv_[lambdaI].set
          (
            rayId,
            new fvScalarMatrix{fvm::div(Ji, iRayLambdaI, "div(Ji,Ii_h)")}
          );
      }
    }
  }
  FOR_ALL(IRay_, rayId) {
    if (omegaMax_ <  IRay_[rayId].omega()) {
      omegaMax_ = IRay_[rayId].omega();
    }
    Info << '\t' << IRay_[rayId].I().name() << " : " << "omega : "
      << '\t' << IRay_[rayId].omega() << nl;
  }
  Info << endl;
}


// Constructors 
mousse::radiation::fvDOM::fvDOM(const volScalarField& T)
:
  radiationModel{typeName, T},
  G_
  {
    {
      "G",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"G", dimMass/pow3(dimTime), 0.0}
  },
  Qr_
  {
    {
      "Qr",
      mesh_.time().timeName(),
      mesh_,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"Qr", dimMass/pow3(dimTime), 0.0}
  },
  Qem_
  {
    {
      "Qem",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"Qem", dimMass/pow3(dimTime), 0.0}
  },
  Qin_
  {
    {
      "Qin",
      mesh_.time().timeName(),
      mesh_,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"Qin", dimMass/pow3(dimTime), 0.0}
  },
  a_
  {
    {
      "a",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"a", dimless/dimLength, 0.0}
  },
  nTheta_{readLabel(coeffs_.lookup("nTheta"))},
  nPhi_{readLabel(coeffs_.lookup("nPhi"))},
  nRay_{0},
  nLambda_{absorptionEmission_->nBands()},
  aLambda_{nLambda_},
  blackBody_{nLambda_, T},
  IRay_{0},
  convergence_{coeffs_.lookupOrDefault<scalar>("convergence", 0.0)},
  maxIter_{coeffs_.lookupOrDefault<label>("maxIter", 50)},
  fvRayDiv_{nLambda_},
  cacheDiv_{coeffs_.lookupOrDefault<bool>("cacheDiv", false)},
  omegaMax_{0}
{
  initialise();
}


mousse::radiation::fvDOM::fvDOM
(
  const dictionary& dict,
  const volScalarField& T
)
:
  radiationModel{typeName, dict, T},
  G_
  {
    {
      "G",
      mesh_.time().timeName(),
      mesh_,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"G", dimMass/pow3(dimTime), 0.0}
  },
  Qr_
  {
    {
      "Qr",
      mesh_.time().timeName(),
      mesh_,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"Qr", dimMass/pow3(dimTime), 0.0}
  },
  Qem_
  {
    {
      "Qem",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"Qem", dimMass/pow3(dimTime), 0.0}
  },
  Qin_
  {
    {
      "Qin",
      mesh_.time().timeName(),
      mesh_,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"Qin", dimMass/pow3(dimTime), 0.0}
  },
  a_
  {
    {
      "a",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"a", dimless/dimLength, 0.0}
  },
  nTheta_{readLabel(coeffs_.lookup("nTheta"))},
  nPhi_{readLabel(coeffs_.lookup("nPhi"))},
  nRay_{0},
  nLambda_{absorptionEmission_->nBands()},
  aLambda_{nLambda_},
  blackBody_{nLambda_, T},
  IRay_{0},
  convergence_{coeffs_.lookupOrDefault<scalar>("convergence", 0.0)},
  maxIter_{coeffs_.lookupOrDefault<label>("maxIter", 50)},
  fvRayDiv_{nLambda_},
  cacheDiv_{coeffs_.lookupOrDefault<bool>("cacheDiv", false)},
  omegaMax_{0}
{
  initialise();
}


// Destructor 
mousse::radiation::fvDOM::~fvDOM()
{}


// Member Functions 
bool mousse::radiation::fvDOM::read()
{
  if (radiationModel::read()) {
    // Only reading solution parameters - not changing ray geometry
    coeffs_.readIfPresent("convergence", convergence_);
    coeffs_.readIfPresent("maxIter", maxIter_);
    return true;
  }
  return false;
}


void mousse::radiation::fvDOM::calculate()
{
  absorptionEmission_->correct(a_, aLambda_);
  updateBlackBodyEmission();
  // Set rays convergence false
  List<bool> rayIdConv{nRay_, false};
  scalar maxResidual = 0.0;
  label radIter = 0;
  do {
    Info << "Radiation solver iter: " << radIter << endl;
    radIter++;
    maxResidual = 0.0;
    FOR_ALL(IRay_, rayI) {
      if (!rayIdConv[rayI]) {
        scalar maxBandResidual = IRay_[rayI].correct();
        maxResidual = max(maxBandResidual, maxResidual);
        if (maxBandResidual < convergence_) {
          rayIdConv[rayI] = true;
        }
      }
    }
  } while (maxResidual > convergence_ && radIter < maxIter_);
  updateG();
}


mousse::tmp<mousse::volScalarField> mousse::radiation::fvDOM::Rp() const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "Rp",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        // Only include continuous phase emission
        4*absorptionEmission_->aCont()*physicoChemical::sigma
      }
    };
}


mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::radiation::fvDOM::Ru() const
{
  const DimensionedField<scalar, volMesh>& G = G_.dimensionedInternalField();
  const DimensionedField<scalar, volMesh> E =
    absorptionEmission_->ECont()().dimensionedInternalField();
  // Only include continuous phase absorption
  const DimensionedField<scalar, volMesh> a =
    absorptionEmission_->aCont()().dimensionedInternalField();
  return a*G - E;
}


void mousse::radiation::fvDOM::updateBlackBodyEmission()
{
  for (label j=0; j < nLambda_; j++) {
    blackBody_.correct(j, absorptionEmission_->bands(j));
  }
}


void mousse::radiation::fvDOM::updateG()
{
  const dimensionedScalar ZERO{"zero",dimMass/pow3(dimTime), 0.0};
  G_ = ZERO;
  Qr_ = ZERO;
  Qem_ = ZERO;
  Qin_ = ZERO;
  FOR_ALL(IRay_, rayI) {
    IRay_[rayI].addIntensity();
    G_ += IRay_[rayI].I()*IRay_[rayI].omega();
    Qr_.boundaryField() += IRay_[rayI].Qr().boundaryField();
    Qem_.boundaryField() += IRay_[rayI].Qem().boundaryField();
    Qin_.boundaryField() += IRay_[rayI].Qin().boundaryField();
  }
}


void mousse::radiation::fvDOM::setRayIdLambdaId
(
  const word& name,
  label& rayId,
  label& lambdaId
) const
{
  // assuming name is in the form: CHARS_rayId_lambdaId
  size_type i1 = name.find_first_of("_");
  size_type i2 = name.find_last_of("_");
  rayId = readLabel(IStringStream(name.substr(i1+1, i2-1))());
  lambdaId = readLabel(IStringStream(name.substr(i2+1, name.size()-1))());
}

