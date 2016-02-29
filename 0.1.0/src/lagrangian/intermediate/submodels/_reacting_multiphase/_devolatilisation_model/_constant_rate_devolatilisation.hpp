#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TDEVOLATILISATION_MODEL_TCONSTANT_RATE_DEVOLATILISATION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TDEVOLATILISATION_MODEL_TCONSTANT_RATE_DEVOLATILISATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ConstantRateDevolatilisation
// Description
//   Constant rate devolatisation model
//   - need to set vapourisation temperature to 600 K
#include "_devolatilisation_model.hpp"
namespace mousse
{
template<class CloudType>
class ConstantRateDevolatilisation
:
  public DevolatilisationModel<CloudType>
{
  // Private data
    // Model constants
      //- List of volatile data - (name A0)
      List<Tuple2<word, scalar> > volatileData_;
      //- List of initial volatile mass fractions
      List<scalar> YVolatile0_;
      //- Mapping between local and cloud gaseous species
      List<label> volatileToGasMap_;
      //- Volatile residual coefficient (0-1)
      //  When the fraction of volatiles are depleted below this
      //  threshold, combustion can occur
      const scalar residualCoeff_;
public:
  //- Runtime type information
  TYPE_NAME("constantRateDevolatilisation");
  // Constructors
    //- Construct from dictionary
    ConstantRateDevolatilisation(const dictionary& dict, CloudType& owner);
    //- Construct copy
    ConstantRateDevolatilisation
    (
      const ConstantRateDevolatilisation<CloudType>& dm
    );
    //- Construct and return a clone
    virtual autoPtr<DevolatilisationModel<CloudType> > clone() const
    {
      return autoPtr<DevolatilisationModel<CloudType> >
      (
        new ConstantRateDevolatilisation<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ConstantRateDevolatilisation();
  // Member Functions
    //- Update model
    virtual void calculate
    (
      const scalar dt,
      const scalar age,
      const scalar mass0,
      const scalar mass,
      const scalar T,
      const scalarField& YGasEff,
      const scalarField& YLiquidEff,
      const scalarField& YSolidEff,
      label& canCombust,
      scalarField& dMassDV
    ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_constant_rate_devolatilisation.cpp"
#endif
#endif
