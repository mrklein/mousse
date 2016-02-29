#ifndef SAMPLING_SAMPLED_SET_CLOUD_SET_HPP_
#define SAMPLING_SAMPLED_SET_CLOUD_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cloudSet
// Description
// SourceFiles
//   cloud_set.cpp
#include "sampled_set.hpp"
#include "dynamic_list.hpp"
namespace mousse
{
// Forward declaration of classes
class passiveParticle;
template<class Type> class particle;
class cloudSet
:
  public sampledSet
{
  // Private data
    //- Sampling points
    List<point> sampleCoords_;
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
  TYPE_NAME("cloud");
  // Constructors
    //- Construct from components
    cloudSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const List<point>& sampleCoords
    );
    //- Construct from dictionary
    cloudSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~cloudSet();
};
}  // namespace mousse
#endif
