// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::veryInhomogeneousMixture
// Description
//   mousse::veryInhomogeneousMixture
// SourceFiles
//   very_inhomogeneous_mixture.cpp
#ifndef very_inhomogeneous_mixture_hpp_
#define very_inhomogeneous_mixture_hpp_
#include "basic_combustion_mixture.hpp"
namespace mousse
{
template<class ThermoType>
class veryInhomogeneousMixture
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
    //- Fuel mass fraction
    volScalarField& fu_;
    //- Regress variable
    volScalarField& b_;
    //- Construct as copy (not implemented)
    veryInhomogeneousMixture(const veryInhomogeneousMixture<ThermoType>&);
public:
  //- The type of thermodynamics this mixture is instantiated for
  typedef ThermoType thermoType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    veryInhomogeneousMixture
    (
      const dictionary&,
      const fvMesh&,
      const word&
    );
  //- Destructor
  virtual ~veryInhomogeneousMixture()
  {}
  // Member functions
    const dimensionedScalar& stoicRatio() const
    {
      return stoicRatio_;
    }
    const ThermoType& mixture(const scalar, const scalar) const;
    const ThermoType& cellMixture(const label celli) const
    {
      return mixture(ft_[celli], fu_[celli]);
    }
    const ThermoType& patchFaceMixture
    (
      const label patchi,
      const label facei
    ) const
    {
      return mixture
      (
        ft_.boundaryField()[patchi][facei],
        fu_.boundaryField()[patchi][facei]
      );
    }
    const ThermoType& cellReactants(const label celli) const
    {
      return mixture(ft_[celli], ft_[celli]);
    }
    const ThermoType& patchFaceReactants
    (
      const label patchi,
      const label facei
    ) const
    {
      return mixture
      (
        ft_.boundaryField()[patchi][facei],
        ft_.boundaryField()[patchi][facei]
      );
    }
    const ThermoType& cellProducts(const label celli) const
    {
      scalar ft = ft_[celli];
      return mixture(ft, fres(ft, stoicRatio().value()));
    }
    const ThermoType& patchFaceProducts
    (
      const label patchi,
      const label facei
    ) const
    {
      scalar ft = ft_.boundaryField()[patchi][facei];
      return mixture(ft, fres(ft, stoicRatio().value()));
    }
    //- Read dictionary
    void read(const dictionary&);
    //- Return thermo based on index
    const ThermoType& getLocalThermo(const label speciei) const;
};
}  // namespace mousse
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#ifdef NoRepository
#   include "very_inhomogeneous_mixture.cpp"
#endif
#endif
