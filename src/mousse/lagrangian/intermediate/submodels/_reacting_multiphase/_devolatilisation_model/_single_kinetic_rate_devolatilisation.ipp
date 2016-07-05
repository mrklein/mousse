// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_single_kinetic_rate_devolatilisation.hpp"


// Constructors 
template<class CloudType>
mousse::SingleKineticRateDevolatilisation<CloudType>::
SingleKineticRateDevolatilisation
(
  const dictionary& dict,
  CloudType& owner
)
:
  DevolatilisationModel<CloudType>{dict, owner, typeName},
  volatileData_{this->coeffDict().lookup("volatileData")},
  YVolatile0_{volatileData_.size()},
  volatileToGasMap_{volatileData_.size()},
  residualCoeff_{readScalar(this->coeffDict().lookup("residualCoeff"))}
{
  if (volatileData_.empty()) {
    WARNING_IN
    (
      "mousse::SingleKineticRateDevolatilisation<CloudType>::"
      "SingleKineticRateDevolatilisation"
      "("
        "const dictionary& dict, "
        "CloudType& owner"
      ")"
    )
    << "Devolatilisation model selected, but no volatiles defined"
    << nl << endl;
  } else {
    Info << "Participating volatile species:" << endl;
    // Determine mapping between active volatiles and cloud gas components
    const label idGas = owner.composition().idGas();
    const scalar YGasTot = owner.composition().YMixture0()[idGas];
    const scalarField& YGas = owner.composition().Y0(idGas);
    FOR_ALL(volatileData_, i) {
      const word& specieName = volatileData_[i].name();
      const label id = owner.composition().localId(idGas, specieName);
      volatileToGasMap_[i] = id;
      YVolatile0_[i] = YGasTot*YGas[id];
      Info << "    " << specieName << ": particle mass fraction = "
        << YVolatile0_[i] << endl;
    }
  }
}


template<class CloudType>
mousse::SingleKineticRateDevolatilisation<CloudType>::
SingleKineticRateDevolatilisation
(
  const SingleKineticRateDevolatilisation<CloudType>& dm
)
:
  DevolatilisationModel<CloudType>{dm},
  volatileData_{dm.volatileData_},
  YVolatile0_{dm.YVolatile0_},
  volatileToGasMap_{dm.volatileToGasMap_},
  residualCoeff_{dm.residualCoeff_}
{}


// Destructor 
template<class CloudType>
mousse::SingleKineticRateDevolatilisation<CloudType>::
~SingleKineticRateDevolatilisation()
{}


// Member Functions 
template<class CloudType>
void mousse::SingleKineticRateDevolatilisation<CloudType>::calculate
(
  const scalar dt,
  const scalar /*age*/,
  const scalar mass0,
  const scalar mass,
  const scalar T,
  const scalarField& YGasEff,
  const scalarField& /*YLiquidEff*/,
  const scalarField& /*YSolidEff*/,
  label& canCombust,
  scalarField& dMassDV
) const
{
  bool done = true;
  FOR_ALL(volatileData_, i) {
    const label id = volatileToGasMap_[i];
    const scalar massVolatile0 = mass0*YVolatile0_[i];
    const scalar massVolatile = mass*YGasEff[id];
    // Combustion allowed once all volatile components evolved
    done = done && (massVolatile <= residualCoeff_*massVolatile0);
    // Model coefficients
    const scalar A1 = volatileData_[i].A1();
    const scalar E = volatileData_[i].E();
    // Kinetic rate
    const scalar kappa = A1*exp(-E/(RR*T));
    // Mass transferred from particle to carrier gas phase
    dMassDV[id] = min(dt*kappa*massVolatile, massVolatile);
  }
  if (done && canCombust != -1) {
    canCombust = 1;
  }
}

