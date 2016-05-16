#ifndef THERMOPHYSICAL_MODELS_BASIC_MIXTURES_PURE_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_MIXTURES_PURE_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pureMixture
// Description
//   mousse::pureMixture

#include "basic_mixture.hpp"


namespace mousse {

template<class ThermoType>
class pureMixture
:
  public basicMixture
{
  // Private data
    ThermoType mixture_;
public:
  //- The type of thermodynamics this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    pureMixture(const dictionary&, const fvMesh&, const word&);
    //- Disable construct as copy
    pureMixture(const pureMixture<ThermoType>&) = delete;
  // Member functions
    const ThermoType& cellMixture(const label) const { return mixture_; }
    const ThermoType& patchFaceMixture
    (
      const label,
      const label
    ) const
    {
      return mixture_;
    }
    const ThermoType& cellVolMixture
    (
      const scalar,
      const scalar,
      const label
    ) const
    {
      return mixture_;
    }
    const ThermoType& patchFaceVolMixture
    (
      const scalar,
      const scalar,
      const label,
      const label
    ) const
    {
      return mixture_;
    }
    //- Read dictionary
    void read(const dictionary&);
};

}  // namespace mousse


#include "pure_mixture.ipp"

#endif
