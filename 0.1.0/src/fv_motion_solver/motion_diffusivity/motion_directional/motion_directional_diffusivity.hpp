// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::motionDirectionalDiffusivity
// Description
//   MotionDirectional finite volume mesh motion diffusivity.
// SourceFiles
//   motion_directional_diffusivity.cpp
#ifndef motion_directional_diffusivity_hpp_
#define motion_directional_diffusivity_hpp_
#include "uniform_diffusivity.hpp"
namespace mousse
{
class motionDirectionalDiffusivity
:
  public uniformDiffusivity
{
  // Private data
    vector diffusivityVector_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    motionDirectionalDiffusivity(const motionDirectionalDiffusivity&);
    //- Disallow default bitwise assignment
    void operator=(const motionDirectionalDiffusivity&);
public:
  //- Runtime type information
  TypeName("motionDirectional");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    motionDirectionalDiffusivity(const fvMesh& mesh, Istream& mdData);
  //- Destructor
  virtual ~motionDirectionalDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
