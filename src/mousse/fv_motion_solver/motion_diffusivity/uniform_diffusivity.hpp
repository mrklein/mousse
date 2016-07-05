#ifndef FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_UNIFORM_UNIFORM_DIFFUSIVITY_HPP_
#define FV_MOTION_SOLVER_MOTION_DIFFUSIVITY_UNIFORM_UNIFORM_DIFFUSIVITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformDiffusivity
// Description
//   Uniform uniform finite volume mesh motion diffusivity.

#include "motion_diffusivity.hpp"
#include "surface_fields.hpp"


namespace mousse {

class uniformDiffusivity
:
  public motionDiffusivity
{
protected:
  // Protected data
    surfaceScalarField faceDiffusivity_;
public:
  //- Runtime type information
  TYPE_NAME("uniform");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    uniformDiffusivity(const fvMesh& mesh, Istream& mdData);
    //- Disallow default bitwise copy construct
    uniformDiffusivity(const uniformDiffusivity&) = delete;
    //- Disallow default bitwise assignment
    uniformDiffusivity& operator=(const uniformDiffusivity&) = delete;
  //- Destructor
  virtual ~uniformDiffusivity();
  // Member Functions
    //- Return diffusivity field
    virtual tmp<surfaceScalarField> operator()() const
    {
      return faceDiffusivity_;
    }
    //- Do not correct the motion diffusivity
    virtual void correct()
    {}
};

}  // namespace mousse

#endif

