// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::COxidationHurtMitchell
// Description
//   Char oxidation model given by Hurt and Mitchell:
//   Based on the reference:
//     Hurt R. and Mitchell R., "Unified high-temperature char combustion
//     kinetics for a suite of coals of various rank", 24th Symposium in
//     Combustion, The Combustion Institute, 1992, p 1243-1250
//   Model specifies the rate of char combustion.
//     C(s) + Sb*O2 -> CO2
//   where Sb is the stoichiometry of the reaction
//   Model validity:
//     Gas temperature: Tc > 1500 K
//     Particle sizes:  75 um -> 200 um
//     Pox > 0.3 atm
#ifndef c_oxidation_hurt_mitchell_hpp_
#define c_oxidation_hurt_mitchell_hpp_
#include "_surface_reaction_model.hpp"
namespace mousse
{
// Forward class declarations
template<class CloudType>
class COxidationHurtMitchell;
template<class CloudType>
class COxidationHurtMitchell
:
  public SurfaceReactionModel<CloudType>
{
  // Private data
    // Model constants
      //- Stoichiometry of reaction
      const scalar Sb_;
    // Addressing
      //- Cs position in global/local lists
      label CsLocalId_;
      //- Ash position in global/local lists
      label ashLocalId_;
      //- O2 position in global list
      label O2GlobalId_;
      //- CO2 positions in global list
      label CO2GlobalId_;
    // Local copies of thermo properties
      //- Molecular weight of C [kg/kmol]
      scalar WC_;
      //- Molecular weight of O2 [kg/kmol]
      scalar WO2_;
      //- Formation enthalpy for CO2 [J/kg]
      scalar HcCO2_;
      //- Heat of reaction [J/kg] (optional)
      scalar heatOfReaction_;
public:
  //- Runtime type information
  TYPE_NAME("COxidationHurtMitchell");
  // Constructors
    //- Construct from dictionary
    COxidationHurtMitchell
    (
      const dictionary& dict,
      CloudType& owner
    );
    //- Construct copy
    COxidationHurtMitchell
    (
      const COxidationHurtMitchell<CloudType>& srm
    );
    //- Construct and return a clone
    virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const
    {
      return autoPtr<SurfaceReactionModel<CloudType> >
      (
        new COxidationHurtMitchell<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~COxidationHurtMitchell();
  // Member Functions
    //- Update surface reactions
    virtual scalar calculate
    (
      const scalar dt,
      const label cellI,
      const scalar d,
      const scalar T,
      const scalar Tc,
      const scalar pc,
      const scalar rhoc,
      const scalar mass,
      const scalarField& YGas,
      const scalarField& YLiquid,
      const scalarField& YSolid,
      const scalarField& YMixture,
      const scalar N,
      scalarField& dMassGas,
      scalarField& dMassLiquid,
      scalarField& dMassSolid,
      scalarField& dMassSRCarrier
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
  #include "c_oxidation_hurt_mitchell.cpp"
#endif
#endif
