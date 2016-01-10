// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::faceOnlySet
// Description
// SourceFiles
//   face_only_set.cpp
#ifndef face_only_set_hpp_
#define face_only_set_hpp_
#include "sampled_set.hpp"
#include "dynamic_list.hpp"
#include "passive_particle_cloud.hpp"
namespace mousse
{
class faceOnlySet
:
  public sampledSet
{
  // Private data
    //- Starting point
    point start_;
    //- End point
    point end_;
  // Private Member Functions
    //- Samples from startTrackPt/CellI. Updates particle/samplePt/sampleI
    //  and puts
    //  samples in the DynamicLists. Returns false if end of all samples
    //  reached
    bool trackToBoundary
    (
      passiveParticleCloud& particleCloud,
      passiveParticle& singleParticle,
      DynamicList<point>& samplingPts,
      DynamicList<label>& samplingCells,
      DynamicList<label>& samplingFaces,
      DynamicList<scalar>& samplingCurve
    ) const;
    //- Samples from start_ to end_. samplingSegments contains segmentNo
    //  for each sample.
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
  TYPE_NAME("face");
  // Constructors
    //- Construct from components
    faceOnlySet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const point& start,
      const point& end
    );
    //- Construct from dictionary
    faceOnlySet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~faceOnlySet();
  // Member Functions
    const point& start() const
    {
      return start_;
    }
    const point& end() const
    {
      return end_;
    }
};
}  // namespace mousse
#endif
