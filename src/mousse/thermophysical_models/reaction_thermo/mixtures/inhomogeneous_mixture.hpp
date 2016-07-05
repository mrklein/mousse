#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_INHOMOGENEOUS_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_INHOMOGENEOUS_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inhomogeneousMixture
// Description
//   mousse::inhomogeneousMixture

#include "basic_combustion_mixture.hpp"


namespace mousse {

template<class ThermoType>
class inhomogeneousMixture
:
  public basicCombustionMixture
{
  // Private data
    static const int nSpecies_ = 2;
    static const char* specieNames_[2];
    dimensionedScalar stoicRatio_;
    ThermoType fuel_;
    ThermoType oxidant_;
    ThermoType products_;
    mutable ThermoType mixture_;
    //- Mixture fraction
    volScalarField& ft_;
    //- Regress variable
    volScalarField& b_;
public:
  //- The type of thermodynamics this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    inhomogeneousMixture(const dictionary&, const fvMesh&, const word&);
    //- Disable construct as copy
    inhomogeneousMixture(const inhomogeneousMixture<ThermoType>&) = delete;
  //- Destructor
  virtual ~inhomogeneousMixture()
  {}
  // Member functions
    const dimensionedScalar& stoicRatio() const { return stoicRatio_; }
    const ThermoType& mixture(const scalar, const scalar) const;
    const ThermoType& cellMixture(const label celli) const
    {
      return mixture(ft_[celli], b_[celli]);
    }
    const ThermoType& patchFaceMixture
    (
      const label patchi,
      const label facei
    ) const
    {
      return mixture(ft_.boundaryField()[patchi][facei],
                     b_.boundaryField()[patchi][facei]);
    }
    const ThermoType& cellReactants(const label celli) const
    {
      return mixture(ft_[celli], 1);
    }
    const ThermoType& patchFaceReactants
    (
      const label patchi,
      const label facei
    ) const
    {
      return mixture(ft_.boundaryField()[patchi][facei], 1);
    }
    const ThermoType& cellProducts(const label celli) const
    {
      return mixture(ft_[celli], 0);
    }
    const ThermoType& patchFaceProducts
    (
      const label patchi,
      const label facei
    ) const
    {
      return mixture(ft_.boundaryField()[patchi][facei], 0);
    }
    //- Read dictionary
    void read(const dictionary&);
    //- Return thermo based on index
    const ThermoType& getLocalThermo(const label speciei) const;
};

}  // namespace mousse

#include "inhomogeneous_mixture.ipp"

#endif
