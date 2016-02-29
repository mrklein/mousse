#ifndef SAMPLING_SAMPLED_SET_ARRAY_SET_HPP_
#define SAMPLING_SAMPLED_SET_ARRAY_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::arraySet
// Description
// SourceFiles
//   array_set.cpp
#include "sampled_set.hpp"
#include "dynamic_list.hpp"
#include "coordinate_system.hpp"
namespace mousse
{
// Forward declaration of classes
class passiveParticle;
template<class Type> class particle;
class arraySet
:
  public sampledSet
{
  // Private data
    //- Coordinate syste
    coordinateSystem coordSys_;
    //- Point density vector
    Vector<label> pointsDensity_;
    //- Span box
    Vector<scalar> spanBox_;
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
  TYPE_NAME("array");
  // Constructors
    //- Construct from components
    arraySet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const coordinateSystem& coordSys,
      const Vector<label>& pointsDensity,
      const Vector<scalar>& spanBox
    );
    //- Construct from dictionary
    arraySet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  //- Destructor
  virtual ~arraySet();
};
}  // namespace mousse
#endif
