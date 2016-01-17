// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::COxidationKineticDiffusionLimitedRate
// Description
//   Kinetic/diffusion limited rate surface reaction model for coal parcels.
//   Limited to:
//     C(s) + Sb*O2 -> CO2
//   where Sb is the stoichiometry of the reaction
#ifndef c_oxidation_kinetic_diffusion_limited_rate_hpp_
#define c_oxidation_kinetic_diffusion_limited_rate_hpp_
#include "_surface_reaction_model.hpp"
namespace mousse
{
// Forward class declarations
template<class CloudType>
class COxidationKineticDiffusionLimitedRate;
template<class CloudType>
class COxidationKineticDiffusionLimitedRate
:
  public SurfaceReactionModel<CloudType>
{
  // Private data
    // Model constants
      //- Stoichiometry of reaction
      const scalar Sb_;
      //- Mass diffusion limited rate constant, C1
      const scalar C1_;
      //- Kinetics limited rate pre-exponential constant, C2
      const scalar C2_;
      //- Kinetics limited rate activation energy
      const scalar E_;
    // Addressing
      //- Cs positions in global/local lists
      label CsLocalId_;
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
public:
  //- Runtime type information
  TYPE_NAME("COxidationKineticDiffusionLimitedRate");
  // Constructors
    //- Construct from dictionary
    COxidationKineticDiffusionLimitedRate
    (
      const dictionary& dict,
      CloudType& owner
    );
    //- Construct copy
    COxidationKineticDiffusionLimitedRate
    (
      const COxidationKineticDiffusionLimitedRate<CloudType>& srm
    );
    //- Construct and return a clone
    virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const
    {
      return autoPtr<SurfaceReactionModel<CloudType> >
      (
        new COxidationKineticDiffusionLimitedRate<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~COxidationKineticDiffusionLimitedRate();
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
  #include "c_oxidation_kinetic_diffusion_limited_rate.cpp"
#endif
#endif
