#ifndef SAMPLING_SAMPLED_SET_CIRCLE_SET_HPP_
#define SAMPLING_SAMPLED_SET_CIRCLE_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::circleSet
// Description
//   Samples along a circular path

#include "sampled_set.hpp"
#include "dynamic_list.hpp"


namespace mousse {

// Forward declaration of classes
class meshSearch;


class circleSet
:
  public sampledSet
{
  // Private data
    // Circle definition
      //- Origin (x, y, z) in global cartesian co-ordinates
      point origin_;
      //- Axis of the circle
      vector circleAxis_;
      //- Point on circle (x, y, z) in global cartesian co-ordinates
      //  Defines start point
      point startPoint_;
    // Sampling definition
      //- Sampling interval in degrees about the origin
      scalar dTheta_;
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
  TYPE_NAME("circle");
  // Constructors
    //- Construct from components
    circleSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const word& axis,
      const point& origin,
      const vector& circleAxis,
      const point& startPoint,
      const scalar dTheta
    );
    //- Construct from dictionary
    circleSet
    (
      const word& name,
      const polyMesh& mesh,
      const meshSearch& searchEngine,
      const dictionary& dict
    );
  // Destructor
    virtual ~circleSet();
  // Member Functions
    //- Get reference point
    virtual point getRefPoint(const List<point>&) const;
};

}  // namespace mousse

#endif

