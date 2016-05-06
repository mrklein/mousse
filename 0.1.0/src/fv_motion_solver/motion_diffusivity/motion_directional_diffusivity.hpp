#ifndef FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_MOTION_DIRECTIONAL_MOTION_DIRECTIONAL_DIFFUSIVITY_HPP_
#define FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_MOTION_DIRECTIONAL_MOTION_DIRECTIONAL_DIFFUSIVITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::motionDirectionalDiffusivity
// Description
//   MotionDirectional finite volume mesh motion diffusivity.

#include "uniform_diffusivity.hpp"


namespace mousse {

class motionDirectionalDiffusivity
:
  public uniformDiffusivity
{
  // Private data
    vector diffusivityVector_;
public:
  //- Runtime type information
  TYPE_NAME("motionDirectional");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    motionDirectionalDiffusivity(const fvMesh& mesh, Istream& mdData);
    //- Disallow default bitwise copy construct
    motionDirectionalDiffusivity(const motionDirectionalDiffusivity&) = delete;
    //- Disallow default bitwise assignment
    motionDirectionalDiffusivity& operator=
    (
      const motionDirectionalDiffusivity&
    ) = delete;
  //- Destructor
  virtual ~motionDirectionalDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};

}  // namespace mousse

#endif

