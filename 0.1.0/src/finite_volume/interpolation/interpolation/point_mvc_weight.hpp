#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_POINT_MVC_WEIGHT_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_POINT_MVC_WEIGHT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointMVCWeight
// Description
//   Container to calculate weights for interpolating directly from vertices
//   of cell using Mean Value Coordinates.
//   Based on (VTK's vtkMeanValueCoordinatesInterpolator's) implementation
//   of "Spherical Barycentric Coordinates"
//   2006 paper Eurographics Symposium on Geometry Processing
//   by Torsten Langer, Alexander Belyaev and Hans-Peter Seide
// SourceFiles
//   point_mvc_weight.cpp
#include "scalar_field.hpp"
#include "vector_field.hpp"
#include "map.hpp"
#include "dynamic_list.hpp"
#include "point.hpp"
#include "point_fields.hpp"
namespace mousse
{
class polyMesh;
class pointMesh;
template<class T> class pointPatchField;
template<class Type, template<class> class PatchField, class GeoMesh>
class GeometricField;
class face;
class pointMVCWeight
{
protected:
  // Protected data
    //- Cell index
    const label cellIndex_;
    //- Weights applied to cell vertices
    scalarField weights_;
  // Protected Member Functions
    //- Calculate weights from single face's vertices only
    void calcWeights
    (
      const Map<label>& toLocal,
      const face& f,
      const DynamicList<point>& u,
      const scalarField& dist,
      scalarField& weights
    ) const;
    //- Calculate weights from all cell's vertices
    void calcWeights
    (
      const polyMesh& mesh,
      const labelList& toGlobal,
      const Map<label>& toLocal,
      const vector& position,
      const vectorField& uVec,
      const scalarField& dist,
      scalarField& weights
    ) const;
public:
  //- Debug switch
  static int debug;
  //- Tolerance used in calculating barycentric co-ordinates
  //  (applied to normalised values)
  static scalar tol;
  // Constructors
    //- Construct from components
    pointMVCWeight
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label faceI = -1
    );
  // Member Functions
    //- Cell index
    inline label cell() const
    {
      return cellIndex_;
    }
    //- Interpolation weights (in order of cellPoints)
    inline const scalarField& weights() const
    {
      return weights_;
    }
    //- Interpolate field
    template<class Type>
    inline Type interpolate
    (
      const GeometricField<Type, pointPatchField, pointMesh>& psip
    ) const;
};
}  // namespace mousse

// Member Functions 
template<class Type>
inline Type mousse::pointMVCWeight::interpolate
(
  const GeometricField<Type, pointPatchField, pointMesh>& psip
) const
{
  const labelList& vertices = psip.mesh()().cellPoints()[cellIndex_];
  Type t = pTraits<Type>::zero;
  FOR_ALL(vertices, i)
  {
    t += psip[vertices[i]]*weights_[i];
  }
  return t;
}
#endif
