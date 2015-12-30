// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicCombustionMixture
// Description
//   Specialization of the basicSpecieMixture for combustion.
// SourceFiles
//   basic_combustion_mixture.cpp
#ifndef basic_combustion_mixture_hpp_
#define basic_combustion_mixture_hpp_
#include "basic_specie_mixture.hpp"
namespace mousse
{
class basicCombustionMixture
:
  public basicSpecieMixture
{
public:
  //- Run time type information
  TypeName("basicCombustionMixture");
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
#include "basic_combustion_mixture_i.hpp"
#endif
