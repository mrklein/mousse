// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inversePointDistanceDiffusivity
// Description
//   Inverse distance to the given patches motion diffusivity.
// SourceFiles
//   inverse_point_distance_diffusivity.cpp
#ifndef inverse_point_distance_diffusivity_hpp_
#define inverse_point_distance_diffusivity_hpp_
#include "uniform_diffusivity.hpp"
namespace mousse
{
class inversePointDistanceDiffusivity
:
  public uniformDiffusivity
{
  // Private data
    //- Patches selected to base the distance on
    wordReList patchNames_;
public:
  //- Runtime type information
  TYPE_NAME("inversePointDistance");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    inversePointDistanceDiffusivity(const fvMesh& mesh, Istream& mdData);
    //- Disallow default bitwise copy construct
    inversePointDistanceDiffusivity
    (
      const inversePointDistanceDiffusivity&
    ) = delete;
    //- Disallow default bitwise assignment
    inversePointDistanceDiffusivity& operator=
    (
      const inversePointDistanceDiffusivity&
    ) = delete;
  //- Destructor
  virtual ~inversePointDistanceDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
