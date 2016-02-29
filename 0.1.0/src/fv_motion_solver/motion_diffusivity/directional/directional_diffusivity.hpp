#ifndef FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_DIRECTIONAL_DIRECTIONAL_DIFFUSIVITY_HPP_
#define FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_DIRECTIONAL_DIRECTIONAL_DIFFUSIVITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::directionalDiffusivity
// Description
//   Directional finite volume mesh motion diffusivity.
// SourceFiles
//   directional_diffusivity.cpp
#include "uniform_diffusivity.hpp"
namespace mousse
{
class directionalDiffusivity
:
  public uniformDiffusivity
{
  // Private data
    vector diffusivityVector_;
public:
  //- Runtime type information
  TYPE_NAME("directional");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    directionalDiffusivity(const fvMesh& mesh, Istream& mdData);
    //- Disallow default bitwise copy construct
    directionalDiffusivity(const directionalDiffusivity&) = delete;
    //- Disallow default bitwise assignment
    directionalDiffusivity& operator=(const directionalDiffusivity&) = delete;
  //- Destructor
  virtual ~directionalDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
