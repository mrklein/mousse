#ifndef MESH_BLOCK_MESH_CURVED_EDGES_CATMULL_ROM_SPLINE_HPP_
#define MESH_BLOCK_MESH_CURVED_EDGES_CATMULL_ROM_SPLINE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CatmullRomSpline
// Description
//   An implementation of Catmull-Rom splines
//   (sometimes known as Overhauser splines).
//   In this implementation, the end tangents are created automatically
//   by reflection.
//   In matrix form, the \e local interpolation on the interval t=[0..1] is
//   described as follows:
//   \verbatim
//   P(t) = 1/2 * [ t^3 t^2 t 1 ] * [ -1  3 -3  1 ] * [ P-1 ]
//                  [  2 -5  4 -1 ]   [ P0 ]
//                  [ -1  0  1  0 ]   [ P1 ]
//                  [  0  2  0  0 ]   [ P2 ]
//   \endverbatim
//   Where P-1 and P2 represent the neighbouring points or the extrapolated
//   end points. Simple reflection is used to automatically create the end
//   points.
//   The spline is discretized based on the chord length of the individual
//   segments. In rare cases (sections with very high curvatures), the
//   resulting distribution may be sub-optimal.
//   A future implementation could also handle closed splines.
// SeeAlso
//   http://www.algorithmist.net/catmullrom.html provides a nice
//   introduction
// SourceFiles
//   catmull_rom_spline.cpp
#include "poly_line.hpp"
namespace mousse
{
class CatmullRomSpline
:
  public polyLine
{
public:
  // Constructors
    //- Construct from components
    CatmullRomSpline
    (
      const pointField& knots,
      const bool NOT_IMPLEMENTEDClosed = false
    );
    //- Disallow default bitwise copy construct
    CatmullRomSpline(const CatmullRomSpline&) = delete;
    //- Disallow default bitwise assignment
    CatmullRomSpline& operator=(const CatmullRomSpline&) = delete;
  // Member Functions
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    point position(const scalar lambda) const;
    //- Return the point position corresponding to the local parameter
    //  0 <= lambda <= 1 on the given segment
    point position(const label segment, const scalar lambda) const;
    //- Return the length of the curve
    scalar length() const;
};
}  // namespace mousse
#endif
