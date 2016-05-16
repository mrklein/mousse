// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fsd.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "les_model.hpp"
#include "fvc_grad.hpp"
#include "fvc_div.hpp"


namespace mousse {
namespace combustionModels {

// Constructors 
template<class CombThermoType, class ThermoType>
FSD<CombThermoType, ThermoType>::FSD
(
  const word& modelType,
  const fvMesh& mesh,
  const word& phaseName
)
:
  singleStepCombustion<CombThermoType, ThermoType>
  {
    modelType,
    mesh,
    phaseName
  },
  reactionRateFlameArea_
  {
    reactionRateFlameArea::New
    (
      this->coeffs(),
      this->mesh(),
      *this
    )
  },
  ft_
  {
    IOobject
    {
      IOobject::groupName("ft", phaseName),
      this->mesh().time().timeName(),
      this->mesh(),
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    this->mesh(),
    {"zero", dimless, 0.0}
  },
  YFuelFuelStream_{dimensionedScalar("YFuelStream", dimless, 1.0)},
  YO2OxiStream_{dimensionedScalar("YOxiStream", dimless, 0.23)},
  Cv_{readScalar(this->coeffs().lookup("Cv"))},
  C_{5.0},
  ftMin_{0.0},
  ftMax_{1.0},
  ftDim_{300},
  ftVarMin_{readScalar(this->coeffs().lookup("ftVarMin"))}
{}


// Destructor 
template<class CombThermoType, class ThermoType>
FSD<CombThermoType, ThermoType>::~FSD()
{}


// Member Functions 
template<class CombThermoType, class ThermoType>
void FSD<CombThermoType, ThermoType>::calculateSourceNorm()
{
  this->singleMixturePtr_->fresCorrect();
  const label fuelI = this->singleMixturePtr_->fuelIndex();
  const volScalarField& YFuel = this->thermoPtr_->composition().Y()[fuelI];
  const volScalarField& YO2 = this->thermoPtr_->composition().Y("O2");
  const dimensionedScalar s = this->singleMixturePtr_->s();
  ft_ = (s*YFuel - (YO2 - YO2OxiStream_))/(s*YFuelFuelStream_ + YO2OxiStream_);
  volVectorField nft{fvc::grad(ft_)};
  volScalarField mgft{mag(nft)};
  surfaceVectorField SfHat{this->mesh().Sf()/this->mesh().magSf()};
  volScalarField cAux{scalar(1) - ft_};
  dimensionedScalar dMgft = 1.0e-3*
    (ft_*cAux*mgft)().weightedAverage(this->mesh().V())
    /((ft_*cAux)().weightedAverage(this->mesh().V()) + SMALL)
    + dimensionedScalar("ddMgft", mgft.dimensions(), SMALL);
  mgft += dMgft;
  nft /= mgft;
  const volVectorField& U = YO2.db().lookupObject<volVectorField>("U");
  const volScalarField sigma
  {
    (nft & nft)*fvc::div(U) - (nft & fvc::grad(U) & nft)
  };
  reactionRateFlameArea_->correct(sigma);
  const volScalarField& omegaFuel = reactionRateFlameArea_->omega();
  const scalar ftStoich =
    YO2OxiStream_.value()
    /(
      s.value()*YFuelFuelStream_.value() + YO2OxiStream_.value()
     );
  tmp<volScalarField> tPc
  {
    new volScalarField
    {
      {
        IOobject::groupName("Pc", this->phaseName_),
        U.time().timeName(),
        U.db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      U.mesh(),
      {"Pc", dimless, 0}
    }
  };
  volScalarField& pc = tPc();
  tmp<volScalarField> tomegaFuel
  {
    new volScalarField
    {
      {
        IOobject::groupName("omegaFuelBar", this->phaseName_),
        U.time().timeName(),
        U.db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      U.mesh(),
      {"omegaFuelBar", omegaFuel.dimensions(), 0}
    }
  };
  volScalarField& omegaFuelBar = tomegaFuel();
  // Calculation of the mixture fraction variance (ftVar)
  const compressible::LESModel& lesModel =
    YO2.db().lookupObject<compressible::LESModel>("LESProperties");
  const volScalarField& delta = lesModel.delta();
  const volScalarField ftVar{Cv_*sqr(delta)*sqr(mgft)};
  // Thickened flame (average flame thickness for counterflow configuration
  // is 1.5 mm)
  volScalarField  deltaF
  {
    lesModel.delta()/dimensionedScalar{"flame", dimLength, 1.5e-3}
  };
  // Linear correlation between delta and flame thickness
  volScalarField omegaF{max(deltaF*(4.0/3.0) + (2.0/3.0), scalar(1))};
  scalar deltaFt = 1.0/ftDim_;
  FOR_ALL(ft_, cellI) {
    if (ft_[cellI] > ftMin_ && ft_[cellI] < ftMax_) {
      scalar ftCell = ft_[cellI];
      if (ftVar[cellI] > ftVarMin_) {  //sub-grid beta pdf of ft_
        scalar ftVarc = ftVar[cellI];
        scalar a = max(ftCell*(ftCell*(1.0 - ftCell)/ftVarc - 1.0), 0.0);
        scalar b = max(a/ftCell - a, 0.0);
        for (int i=1; i<ftDim_; i++) {
          scalar ft = i*deltaFt;
          pc[cellI] += pow(ft, a-1.0)*pow(1.0 - ft, b - 1.0)*deltaFt;
        }
        for (int i=1; i<ftDim_; i++) {
          scalar ft = i*deltaFt;
          omegaFuelBar[cellI] +=
            omegaFuel[cellI]/omegaF[cellI]
            *exp(-sqr(ft - ftStoich)/(2.0*sqr(0.01*omegaF[cellI])))
            *pow(ft, a - 1.0)
            *pow(1.0 - ft, b - 1.0)
            *deltaFt;
        }
        omegaFuelBar[cellI] /= max(pc[cellI], 1e-4);
      } else {
        omegaFuelBar[cellI] =
         omegaFuel[cellI]/omegaF[cellI]
         *exp(-sqr(ftCell - ftStoich)/(2.0*sqr(0.01*omegaF[cellI])));
      }
    } else {
      omegaFuelBar[cellI] = 0.0;
    }
  }
  // Combustion progress variable, c
  List<label> productsIndex{2, label(-1)};

  {
    label i = 0;
    FOR_ALL(this->singleMixturePtr_->specieProd(), specieI) {
      if (this->singleMixturePtr_->specieProd()[specieI] < 0) {
        productsIndex[i] = specieI;
        i++;
      }
    }
  }

  // Flamelet probability of the progress c based on IFC (reuse pc)
  scalar YprodTotal = 0;
  FOR_ALL(productsIndex, j) {
    YprodTotal += this->singleMixturePtr_->Yprod0()[productsIndex[j]];
  }
  FOR_ALL(ft_, cellI) {
    if (ft_[cellI] < ftStoich) {
      pc[cellI] = ft_[cellI]*(YprodTotal/ftStoich);
    } else {
      pc[cellI] = (1.0 - ft_[cellI])*(YprodTotal/(1.0 - ftStoich));
    }
  }
  tmp<volScalarField> tproducts
  {
    new volScalarField
    {
      {
        IOobject::groupName("products", this->phaseName_),
        U.time().timeName(),
        U.db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      U.mesh(),
      {"products", dimless, 0}
    }
  };
  volScalarField& products = tproducts();
  FOR_ALL(productsIndex, j) {
    label specieI = productsIndex[j];
    const volScalarField& Yp = this->thermoPtr_->composition().Y()[specieI];
    products += Yp;
  }
  volScalarField c{max(scalar(1) - products/max(pc, scalar(1e-5)), scalar(0))};
  pc = min(C_*c, scalar(1));
  const volScalarField fres(this->singleMixturePtr_->fres(fuelI));
  this->wFuel_ == mgft*pc*omegaFuelBar;
}


template<class CombThermoType, class ThermoType>
void FSD<CombThermoType, ThermoType>::correct()
{
  this->wFuel_ ==
    dimensionedScalar{"zero", dimMass/pow3(dimLength)/dimTime, 0.0};
  if (this->active()) {
    calculateSourceNorm();
  }
}


template<class CombThermoType, class ThermoType>
bool FSD<CombThermoType, ThermoType>::read()
{
  if (singleStepCombustion<CombThermoType, ThermoType>::read()) {
    this->coeffs().lookup("Cv") >> Cv_ ;
    this->coeffs().lookup("ftVarMin") >> ftVarMin_;
    reactionRateFlameArea_->read(this->coeffs());
    return true;
  }
  return false;
}

}  // namespace combustionModels
}  // namespace mousse

