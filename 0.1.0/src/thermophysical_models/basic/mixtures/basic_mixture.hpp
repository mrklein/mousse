// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicMixture
// Description
//   mousse::basicMixture
// SourceFiles
//   basic_mixture.cpp
#ifndef basic_mixture_hpp_
#define basic_mixture_hpp_
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
