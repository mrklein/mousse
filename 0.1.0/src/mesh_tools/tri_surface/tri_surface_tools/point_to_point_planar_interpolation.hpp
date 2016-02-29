#ifndef MESH_TOOLS_TRI_SURFACE_TRI_SURFACE_TOOLS_POINT_TO_POINT_PLANAR_INTERPOLATION_HPP_
#define MESH_TOOLS_TRI_SURFACE_TRI_SURFACE_TOOLS_POINT_TO_POINT_PLANAR_INTERPOLATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointToPointPlanarInterpolation
// Description
//   Interpolates between two sets of unstructured points using 2D Delaunay
//   triangulation. Used in e.g. timeVaryingMapped bcs.
// SourceFiles
//   point_to_point_planar_interpolation.cpp


#include "fixed_list.hpp"
#include "coordinate_system.hpp"
#include "instant_list.hpp"

namespace mousse
{

class pointToPointPlanarInterpolation
{
  // Private data

    //- Perturbation factor
    const scalar perturb_;

    //- Whether to use nearest point only (avoids triangulation, projection)
    const bool nearestOnly_;

    //- Coordinate system
    coordinateSystem referenceCS_;

    //- Number of source points (for checking)
    label nPoints_;

    //- Current interpolation addressing to face centres of underlying
    //  patch
    List<FixedList<label, 3>> nearestVertex_;

    //- Current interpolation factors to face centres of underlying
    //  patch
    List<FixedList<scalar, 3>> nearestVertexWeight_;

  // Private Member Functions

    //- Calculate a local coordinate system from set of points
    coordinateSystem calcCoordinateSystem(const pointField&) const;

    //- Calculate addressing and weights
    void calcWeights
    (
      const pointField& sourcePoints,
      const pointField& destPoints
    );

public:

  // Declare name of the class and its debug switch
  CLASS_NAME("pointToPointPlanarInterpolation");

  // Constructors

    //- Construct from 3D locations. Determines local coordinate system
    //  from sourcePoints and maps onto that. If nearestOnly skips any
    //  local coordinate system and triangulation and uses nearest vertex
    //  only
    pointToPointPlanarInterpolation
    (
      const pointField& sourcePoints,
      const pointField& destPoints,
      const scalar perturb,
      const bool nearestOnly = false
    );

    //- Construct from coordinate system and locations.
    pointToPointPlanarInterpolation
    (
      const coordinateSystem& referenceCS,
      const pointField& sourcePoints,
      const pointField& destPoints,
      const scalar perturb
    );

  // Member Functions

    //- Return the coordinateSystem
    const coordinateSystem& referenceCS() const
    {
      return referenceCS_;
    }

    //- Number of source points
    label sourceSize() const
    {
      return nPoints_;
    }

    //  patch
    const List<FixedList<label, 3> >& nearestVertex() const
    {
      return nearestVertex_;
    }

    //- Current interpolation factors to face centres of underlying
    //  patch
    const List<FixedList<scalar, 3> >& nearestVertexWeight() const
    {
      return nearestVertexWeight_;
    }

    //- Helper: extract words of times
    static wordList timeNames(const instantList&);

    //- Helper: find time. Return true if succesful.
    static bool findTime
    (
      const instantList& times,
      const label startSampleTime,
      const scalar timeVal,
      label& lo,
      label& hi
    );

    //- Interpolate from field on source points to dest points
    template<class Type>
    tmp<Field<Type> > interpolate(const Field<Type>& sourceFld) const;

};

}  // namespace mousse

#ifdef NoRepository
#   include "point_to_point_planar_interpolation_templates.cpp"
#endif
#endif
