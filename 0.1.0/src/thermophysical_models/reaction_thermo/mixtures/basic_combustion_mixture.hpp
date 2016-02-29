#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_BASIC_COMBUSTION_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_BASIC_COMBUSTION_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicCombustionMixture
// Description
//   Specialization of the basicSpecieMixture for combustion.
// SourceFiles
//   basic_combustion_mixture.cpp
#include "basic_specie_mixture.hpp"
namespace mousse
{
class basicCombustionMixture
:
  public basicSpecieMixture
{
public:
  //- Run time type information
  TYPE_NAME("basicCombustionMixture");
  //- The base class of the mixture
  typedef basicCombustionMixture basicMixtureType;
  // Constructors
    //- Construct from dictionary, specie names, mesh and phase name
    basicCombustionMixture
    (
      const dictionary&,
      const wordList& specieNames,
      const fvMesh&,
      const word&
    );
  //- Destructor
  virtual ~basicCombustionMixture()
  {}
  // Member functions
    inline scalar fres(const scalar ft, const scalar stoicRatio) const;
    inline tmp<volScalarField> fres
    (
      const volScalarField& ft,
      const dimensionedScalar& stoicRatio
    ) const;
};
}  // namespace mousse

inline mousse::scalar mousse::basicCombustionMixture::fres
(
  const scalar ft,
  const scalar stoicRatio
) const
{
  return max(ft - (scalar(1) - ft)/stoicRatio, scalar(0));
}
inline mousse::tmp<mousse::volScalarField> mousse::basicCombustionMixture::fres
(
  const volScalarField& ft,
  const dimensionedScalar& stoicRatio
) const
{
  return max(ft - (scalar(1) - ft)/stoicRatio.value(), scalar(0));
}
#endif
