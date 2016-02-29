#ifndef SAMPLING_SAMPLED_SET_PATCH_CLOUD_SET_HPP_
#define SAMPLING_SAMPLED_SET_PATCH_CLOUD_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::patchCloudSet
// Description
//   Like cloudSet but samples nearest patch face
// SourceFiles
//   patch_cloud_set.cpp
#include "sampled_set.hpp"
#include "dynamic_list.hpp"
#include "hash_set.hpp"
namespace mousse
{
class patchCloudSet
:
  public sampledSet
{
  // Private data
    //- Sampling points
    const List<point> sampleCoords_;
    //- Patches to sample
    const labelHashSet patchSet_;
    //- Maximum distance to look for nearest
    const scalar searchDist_;
  // Private Member Functions
    //- Samples all points in sampleCoords.
    void calcSamples
    (
      DynamicList<point>& samplingPts,
      DynamicList<label>& samplingCells,
      DynamicList<label>& samplingFaces,
      DynamicList<label>& samplingSegments,
      DynamicList<scalar>& samplingCurveDist
    ) const;
    //- Uses calcSamples to obtain samples. Copies them into *this.
    void genSamples();
public:
  //- Runtime type information
  TYPE_NAME("patchCloud");
  // Constructors
    //- Construct from components
    patchCloudSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const List<point>& sampleCoords,
      const labelHashSet& patchSet,
      const scalar searchDist
    );
    //- Construct from dictionary
    patchCloudSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~patchCloudSet();
};
}  // namespace mousse
#endif
