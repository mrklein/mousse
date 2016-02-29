#ifndef MESH_BLOCK_MESH_CURVED_EDGES_POLY_LINE_HPP_
#define MESH_BLOCK_MESH_CURVED_EDGES_POLY_LINE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyLine
// Description
//   A series of straight line segments, which can also be interpreted as
//   a series of control points for splines, etc.
//   A future implementation could also handle a closed polyLine.
// SourceFiles
//   poly_line.cpp
#include "point_field.hpp"
#include "scalar_list.hpp"
namespace mousse
{
class polyLine
{
protected:
  // Protected data
    //- The control points or ends of each segments
    pointField points_;
    //- The real line length
    scalar lineLength_;
    //- The rational (0-1) cumulative parameter value for each point
    scalarList param_;
  // Protected Member Functions
    //- Precalculate the rational cumulative parameter value
    //  and the line-length
    void calcParam();
    //- Return the line segment and the local parameter [0..1]
    //  corresponding to the global lambda [0..1]
    label localParameter(scalar& lambda) const;
public:
  // Constructors
    //- Construct from components
    polyLine
    (
      const pointField&,
      const bool NOT_IMPLEMENTEDClosed = false
    );
    //- Disallow default bitwise copy construct
    polyLine(const polyLine&) = delete;
    //- Disallow default bitwise assignment
    polyLine& operator=(const polyLine&) = delete;
  // Member Functions
    //- Return const-access to the control-points
    const pointField& points() const;
    //- Return the number of line segments
    label nSegments() const;
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    point position(const scalar) const;
    //- Return the point position corresponding to the local parameter
    //  0 <= lambda <= 1 on the given segment
    point position(const label segment, const scalar) const;
    //- Return the length of the curve
    scalar length() const;
};
}  // namespace mousse
#endif
