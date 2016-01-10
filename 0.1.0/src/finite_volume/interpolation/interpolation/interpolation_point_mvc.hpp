// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationPointMVC
// Description
//   Given cell centre values interpolates to vertices and uses these to
//   do a Mean Value Coordinates interpolation.
#ifndef interpolation_point_mvc_hpp_
#define interpolation_point_mvc_hpp_
#include "interpolation.hpp"
#include "point_mvc_weight.hpp"
namespace mousse
{
template<class Type>
class interpolationPointMVC
:
  public interpolation<Type>
{
protected:
  // Protected data
    //- Interpolated volfield
    const GeometricField<Type, pointPatchField, pointMesh> psip_;
public:
  //- Runtime type information
  TYPE_NAME("pointMVC");
  // Constructors
    //- Construct from components
    interpolationPointMVC
    (
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
  // Member Functions
    //- Inherit interpolate from interpolation
    using interpolation<Type>::interpolate;
    //- Interpolate field for the given cellPointWeight
    inline Type interpolate(const pointMVCWeight& cpw) const;
    //- Interpolate field to the given point in the given cell
    inline Type interpolate
    (
      const vector& position,
      const label cellI,
      const label faceI = -1
    ) const;
};
}  // namespace mousse
#include "interpolation_point_mvci.hpp"
#ifdef NoRepository
#   include "interpolation_point_mvc.cpp"
#endif
#endif
