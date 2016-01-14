// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::COxidationMurphyShaddix
// Description
//   Limited to C(s) + O2 -> CO2
//   Loosely based on the reference:
//     Murphy, J. J., Shaddix, C. R., Combustion kinetics of coal chars
//     in oxygen-enriched environments, Combustion and Flame 144,
//     pp710-729, 2006
#ifndef c_oxidation_murphy_shaddix_hpp_
#define c_oxidation_murphy_shaddix_hpp_
#include "_surface_reaction_model.hpp"
namespace mousse
{
// Forward class declarations
template<class CloudType>
class COxidationMurphyShaddix;
template<class CloudType>
class COxidationMurphyShaddix
:
  public SurfaceReactionModel<CloudType>
{
  // Private data
    // Model constants
      //- Reference diffusion constant
      const scalar D0_;
      //- Reference density for reference diffusion constant
      const scalar rho0_;
      //- Reference temperature for reference diffusion constant
      const scalar T0_;
      //- Exponent for diffusion equation
      const scalar Dn_;
      //- Kinetic rate coefficient
      const scalar A_;
      //- Kinetic rate activation energy
      const scalar E_;
      //- Reaction order
      const scalar n_;
      //- Effective molecular weight of gaseous volatiles
      const scalar WVol_;
    // Static constants
      //- Maximum number of iterations
      static label maxIters_;
      //- Tolerance used in inner iterations
      static scalar tolerance_;
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
  TYPE_NAME("COxidationMurphyShaddix");
  // Constructors
    //- Construct from dictionary
    COxidationMurphyShaddix
    (
      const dictionary& dict,
      CloudType& owner
    );
    //- Construct copy
    COxidationMurphyShaddix
    (
      const COxidationMurphyShaddix<CloudType>& srm
    );
    //- Construct and return a clone
    virtual autoPtr<SurfaceReactionModel<CloudType> > clone() const
    {
      return autoPtr<SurfaceReactionModel<CloudType> >
      (
        new COxidationMurphyShaddix<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~COxidationMurphyShaddix();
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
  #include "c_oxidation_murphy_shaddix.cpp"
#endif
#endif
