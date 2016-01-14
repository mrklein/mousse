// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::exponentialDiffusivity
// Description
//   Mesh motion diffusivity maipulator which returns the exp(-alpha/D)
//   of the given diffusivity D.
// SourceFiles
//   exponential_diffusivity.cpp
#ifndef exponential_diffusivity_hpp_
#define exponential_diffusivity_hpp_
#include "motion_diffusivity.hpp"
namespace mousse
{
class exponentialDiffusivity
:
  public motionDiffusivity
{
  // Private data
    scalar alpha_;
    autoPtr<motionDiffusivity> basicDiffusivityPtr_;
public:
  //- Runtime type information
  TYPE_NAME("exponential");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    exponentialDiffusivity(const fvMesh& mesh, Istream& mdData);
    //- Disallow default bitwise copy construct
    exponentialDiffusivity(const exponentialDiffusivity&) = delete;
    //- Disallow default bitwise assignment
    exponentialDiffusivity& operator=(const exponentialDiffusivity&) = delete;
  //- Destructor
  virtual ~exponentialDiffusivity();
  // Member Functions
    //- Return diffusivity field
    virtual tmp<surfaceScalarField> operator()() const;
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
