#ifndef SAMPLING_SAMPLED_SET_POLY_LINE_SET_HPP_
#define SAMPLING_SAMPLED_SET_POLY_LINE_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyLineSet
// Description
//   Sample along poly line defined by a list of points (knots)

#include "sampled_set.hpp"
#include "dynamic_list.hpp"
#include "passive_particle_cloud.hpp"


namespace mousse {

class polyLineSet
:
  public sampledSet
{
  // Private data
    //- Sampling points
    List<point> sampleCoords_;
  // Private Member Functions
    //- Sample till hits boundary. Called with singleParticle at position
    //  inbetween sampleCoords_[sampleI] and sampleCoords_[sampleI+1].
    //  Returns false if end of samples reached.
    bool trackToBoundary
    (
      passiveParticleCloud& particleCloud,
      passiveParticle& singleParticle,
      label& sampleI,
      DynamicList<point>& samplingPts,
      DynamicList<label>& samplingCells,
      DynamicList<label>& samplingFaces,
      DynamicList<scalar>& samplingCurveDist
    ) const;
    //- Samples all point in sampleCoords_
    //  samplingSegments contains segmentNo for each sample.
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
  TYPE_NAME("polyLine");
  // Constructors
    //- Construct from components
    polyLineSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const List<point>& samplePoints
    );
    //- Construct from dictionary
    polyLineSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~polyLineSet();
};
}  // namespace mousse
#endif
