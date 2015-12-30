// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchSeedSet
// Description
//   Initialises points on or just off patch
// SourceFiles
//   patch_seed_set.cpp
#ifndef patch_seed_set_hpp_
#define patch_seed_set_hpp_
#include "sampled_set.hpp"
#include "dynamic_list.hpp"
#include "hash_set.hpp"
#include "random.hpp"
namespace mousse
{
class patchSeedSet
:
  public sampledSet
{
  // Private data
    //- Patches to sample
    const labelHashSet patchSet_;
//        //- Maximum distance to look for nearest
//        const scalar searchDist_;
//
//        //- Offset distance
//        const scalar offsetDist_;
    //- Maximum number of patch faces to seed
    const label maxPoints_;
    //- Random number generator (if maxPoints < num patch faces)
    autoPtr<Random> rndGenPtr_;
  // Private Member Functions
    //- Samples all points in sampleCoords.
    void calcSamples
    (
      DynamicList<point>& samplingPts,
      DynamicList<label>& samplingCells,
      DynamicList<label>& samplingFaces,
      DynamicList<label>& samplingSegments,
      DynamicList<scalar>& samplingCurveDist
    );
    //- Uses calcSamples to obtain samples. Copies them into *this.
    void genSamples();
public:
  //- Runtime type information
  TypeName("patchSeed");
  // Constructors
    //- Construct from dictionary
    patchSeedSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~patchSeedSet();
};
}  // namespace mousse
#endif
