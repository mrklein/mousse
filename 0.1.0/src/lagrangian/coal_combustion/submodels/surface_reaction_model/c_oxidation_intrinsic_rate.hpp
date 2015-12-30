// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::COxidationIntrinsicRate
// Description
//   Intrinsic char surface reaction mndel
//     C(s) + Sb*O2 -> CO2
//   where Sb is the stoichiometry of the reaction
#ifndef c_oxidation_intrinsic_rate_hpp_
#define c_oxidation_intrinsic_rate_hpp_
#include "_surface_reaction_model.hpp"
namespace mousse
{
// Forward class declarations
template<class CloudType>
class COxidationIntrinsicRate;
template<class CloudType>
class COxidationIntrinsicRate
:
  public SurfaceReactionModel<CloudType>
{
  // Private data
    // Model constants
      //- Stoichiometry of reaction []
      const scalar Sb_;
      //- Mass diffusion limited rate constant
      const scalar C1_;
      //- Mean pore radius [m]
      const scalar rMean_;
      //- Char porosity [] = 1 - rho_apparent/rho_true
      const scalar theta_;
      //- Pre-exponential factor
      const scalar Ai_;
      //- Activation energy
      const scalar Ei_;
      //- Char specific internal area [m2/kg]
      const scalar Ag_;
      //- Pore tortuosity []; default to sqrt(2)
      const scalar tau_;
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
  TypeName("COxidationIntrinsicRate");
  // Constructors
    //- Construct from dictionary
    COxidationIntrinsicRate
    (
      const dictionary& dict,
      CloudType& owner
    );
    //- Construct copy
    COxidationIntrinsicRate
    (
      const COxidationIntrinsicRate<CloudType>& srm
    );
    //- Construct and return a clone
    virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const
    {
      return autoPtr<SurfaceReactionModel<CloudType> >
      (
        new COxidationIntrinsicRate<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~COxidationIntrinsicRate();
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
  #include "c_oxidation_intrinsic_rate.cpp"
#endif
#endif
