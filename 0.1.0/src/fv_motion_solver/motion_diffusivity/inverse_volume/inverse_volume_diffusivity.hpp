// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inverseVolumeDiffusivity
// Description
//   Inverse cell-volume motion diffusivity.
// SourceFiles
//   inverse_volume_diffusivity.cpp
#ifndef inverse_volume_diffusivity_hpp_
#define inverse_volume_diffusivity_hpp_
#include "uniform_diffusivity.hpp"
namespace mousse
{
class inverseVolumeDiffusivity
:
  public uniformDiffusivity
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    inverseVolumeDiffusivity(const inverseVolumeDiffusivity&);
    //- Disallow default bitwise assignment
    void operator=(const inverseVolumeDiffusivity&);
public:
  //- Runtime type information
  TypeName("inverseVolume");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    inverseVolumeDiffusivity(const fvMesh& mesh, Istream& mdData);
  //- Destructor
  virtual ~inverseVolumeDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
