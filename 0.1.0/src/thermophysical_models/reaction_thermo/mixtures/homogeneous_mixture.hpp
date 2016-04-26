#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_HOMOGENEOUS_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_HOMOGENEOUS_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::homogeneousMixture
// Description
//   mousse::homogeneousMixture

#include "basic_combustion_mixture.hpp"


namespace mousse {

template<class ThermoType>
class homogeneousMixture
:
  public basicCombustionMixture
{
  // Private data
    static const int nSpecies_ = 1;
    static const char* specieNames_[1];
    ThermoType reactants_;
    ThermoType products_;
    mutable ThermoType mixture_;
    //- Regress variable
    volScalarField& b_;
public:
  //- The type of thermodynamics this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    homogeneousMixture(const dictionary&, const fvMesh&, const word&);
    //- Disable construct as copy
    homogeneousMixture(const homogeneousMixture<ThermoType>&) = delete;
  //- Destructor
  virtual ~homogeneousMixture()
  {}
  // Member functions
    const ThermoType& mixture(const scalar) const;
    const ThermoType& cellMixture(const label celli) const
    {
      return mixture(b_[celli]);
    }
    const ThermoType& patchFaceMixture
    (
      const label patchi,
      const label facei
    ) const
    {
      return mixture(b_.boundaryField()[patchi][facei]);
    }
    const ThermoType& cellReactants(const label) const
    {
      return reactants_;
    }
    const ThermoType& patchFaceReactants(const label, const label) const
    {
      return reactants_;
    }
    const ThermoType& cellProducts(const label) const
    {
      return products_;
    }
    const ThermoType& patchFaceProducts(const label, const label) const
    {
      return products_;
    }
    //- Read dictionary
    void read(const dictionary&);
    //- Return thermo based on index
    const ThermoType& getLocalThermo(const label speciei) const;
};
}  // namespace mousse

#include "homogeneous_mixture.ipp"

#endif
