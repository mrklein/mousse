#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TDEVOLATILISATION_MODEL_TNO_DEVOLATILISATION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TDEVOLATILISATION_MODEL_TNO_DEVOLATILISATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoDevolatilisation
// Description
//   Dummy devolatilisation model for 'none'

#include "_devolatilisation_model.hpp"


namespace mousse {

template<class CloudType>
class NoDevolatilisation
:
  public DevolatilisationModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoDevolatilisation(const dictionary&, CloudType& owner);
    //- Construct copy
    NoDevolatilisation(const NoDevolatilisation<CloudType>& dm);
    //- Construct and return a clone
    virtual autoPtr<DevolatilisationModel<CloudType>> clone() const
    {
      return
        autoPtr<DevolatilisationModel<CloudType>>
        {
          new NoDevolatilisation<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~NoDevolatilisation();
  // Member Functions
    //- Flag to indicate whether model activates devolatilisation model
    virtual bool active() const;
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

#include "_no_devolatilisation.ipp"

#endif
