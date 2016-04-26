#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_EGR_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_EGR_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::egrMixture
// Description
//   mousse::egrMixture

#include "basic_combustion_mixture.hpp"


namespace mousse {

template<class ThermoType>
class egrMixture
:
  public basicCombustionMixture
{
  // Private data
    static const int nSpecies_ = 3;
    static const char* specieNames_[3];
    dimensionedScalar stoicRatio_;
    ThermoType fuel_;
    ThermoType oxidant_;
    ThermoType products_;
    mutable ThermoType mixture_;
    //- Mixture fraction
    volScalarField& ft_;
    //- Regress variable
    volScalarField& b_;
    //- Residual gases
    volScalarField& egr_;
public:
  //- The type of thermodynamics this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phaseName
    egrMixture(const dictionary&, const fvMesh&, const word&);
    //- Disable construct as copy
    egrMixture(const egrMixture<ThermoType>&) = delete;
  //- Destructor
  virtual ~egrMixture()
  {}
  // Member functions
    const dimensionedScalar& stoicRatio() const { return stoicRatio_; }
    const ThermoType& mixture
    (
      const scalar,
      const scalar,
      const scalar
    ) const;
    const ThermoType& cellMixture(const label celli) const
    {
      return mixture(ft_[celli], b_[celli], egr_[celli]);
    }
    const ThermoType& patchFaceMixture
    (
      const label patchi,
      const label facei
    ) const
    {
      return
        mixture
        (
          ft_.boundaryField()[patchi][facei],
          b_.boundaryField()[patchi][facei],
          egr_.boundaryField()[patchi][facei]
        );
    }
    const ThermoType& cellReactants(const label celli) const
    {
      return mixture(ft_[celli], 1, egr_[celli]);
    }
    const ThermoType& patchFaceReactants
    (
      const label patchi,
      const label facei
    ) const
    {
      return
        mixture
        (
          ft_.boundaryField()[patchi][facei],
          1,
          egr_.boundaryField()[patchi][facei]
        );
    }
    const ThermoType& cellProducts(const label celli) const
    {
      return mixture(ft_[celli], 0, 0);
    }
    const ThermoType& patchFaceProducts
    (
      const label patchi,
      const label facei
    ) const
    {
      return mixture(ft_.boundaryField()[patchi][facei], 0, 0);
    }
    //- Read dictionary
    void read(const dictionary&);
    //- Return thermo based on index
    const ThermoType& getLocalThermo(const label speciei) const;
};

}  // namespace mousse


#include "egr_mixture.ipp"

#endif
