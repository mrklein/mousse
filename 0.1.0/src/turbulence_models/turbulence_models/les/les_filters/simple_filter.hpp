#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_FILTERS_SIMPLE_FILTER_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_LES_LES_FILTERS_SIMPLE_FILTER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::simpleFilter
// Description
//   Simple top-hat filter used in dynamic LES models.
//   Implemented as a surface integral of the face interpolate of the field.
// SourceFiles
//   simple_filter.cpp
#include "les_filter.hpp"
namespace mousse
{
class simpleFilter
:
  public LESfilter
{
public:
  //- Runtime type information
  TYPE_NAME("simple");
  // Constructors
    //- Construct from components
    simpleFilter(const fvMesh& mesh);
    //- Construct from IOdictionary
    simpleFilter(const fvMesh& mesh, const dictionary&);
    //- Disallow default bitwise copy construct and assignment
    simpleFilter(const simpleFilter&) = delete;
    simpleFilter& operator=(const simpleFilter&) = delete;
  //- Destructor
  virtual ~simpleFilter()
  {}
  // Member Functions
    //- Read the LESfilter dictionary
    virtual void read(const dictionary&);
  // Member Operators
    virtual tmp<volScalarField> operator()
    (
      const tmp<volScalarField>&
    ) const;
    virtual tmp<volVectorField> operator()
    (
      const tmp<volVectorField>&
    ) const;
    virtual tmp<volSymmTensorField> operator()
    (
      const tmp<volSymmTensorField>&
    ) const;
    virtual tmp<volTensorField> operator()
    (
      const tmp<volTensorField>&
    ) const;
};
}  // namespace mousse
#endif
