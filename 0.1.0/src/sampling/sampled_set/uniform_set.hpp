// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformSet
// Description
// SourceFiles
//   uniform_set.cpp
#ifndef uniform_set_hpp_
#define uniform_set_hpp_
#include "passive_particle_cloud.hpp"
#include "sampled_set.hpp"
#include "dynamic_list.hpp"
namespace mousse
{
class uniformSet
:
  public sampledSet
{
  // Private data
    //- Starting point
    point start_;
    //- End point
    point end_;
    //- Number of points
    label nPoints_;
  // Private Member Functions
    //- Calculates - starting at samplePt - the first sampling point
    //  on or after currentPt. smallDist is the tolerance used to compare
    //  positions. Returns false if end of samples reached.
    bool nextSample
    (
      const point& currentPt,
      const vector& offset,
      const scalar smallDist,
      point& samplePt,
      label& sampleI
    ) const;
    //- Samples from startTrackPt/CellI. Updates particle/samplePt/sampleI
    //  and puts
    //  samples in the DynamicLists. Returns false if end of all samples
    //  reached
    bool trackToBoundary
    (
      passiveParticleCloud& particleCloud,
      passiveParticle& singleParticle,
      point& samplePt,
      label& sampleI,
      DynamicList<point>& samplingPts,
      DynamicList<label>& samplingCells,
      DynamicList<label>& samplingFaces,
      DynamicList<scalar>& samplingCurveDist
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
  TypeName("uniform");
  // Constructors
    //- Construct from components
    uniformSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const point& start,
      const point& end,
      const label nPoints
    );
    //- Construct from dictionary
    uniformSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~uniformSet();
};
}  // namespace mousse
#endif
