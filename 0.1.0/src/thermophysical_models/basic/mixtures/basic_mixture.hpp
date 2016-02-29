#ifndef THERMOPHYSICAL_MODELS_BASIC_MIXTURES_BASIC_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_MIXTURES_BASIC_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicMixture
// Description
//   mousse::basicMixture
// SourceFiles
//   basic_mixture.cpp
namespace mousse
{
class fvMesh;
class dictionary;
class basicMixture
{
public:
  //- The base class of the mixture
  typedef basicMixture basicMixtureType;
  // Constructors
    //- Construct from dictionary, mesh and phase name
    basicMixture(const dictionary&, const fvMesh&, const word&)
    {}
};
}  // namespace mousse
#endif
