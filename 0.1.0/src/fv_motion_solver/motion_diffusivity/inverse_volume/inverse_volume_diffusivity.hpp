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
public:
  //- Runtime type information
  TYPE_NAME("inverseVolume");
  // Constructors
    //- Construct for the given fvMesh and data Istream
    inverseVolumeDiffusivity(const fvMesh& mesh, Istream& mdData);
    //- Disallow default bitwise copy construct
    inverseVolumeDiffusivity(const inverseVolumeDiffusivity&) = delete;
    //- Disallow default bitwise assignment
    inverseVolumeDiffusivity& operator=
    (
      const inverseVolumeDiffusivity&
    ) = delete;
  //- Destructor
  virtual ~inverseVolumeDiffusivity();
  // Member Functions
    //- Correct the motion diffusivity
    virtual void correct();
};
}  // namespace mousse
#endif
