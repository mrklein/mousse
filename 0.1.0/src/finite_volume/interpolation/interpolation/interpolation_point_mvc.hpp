#ifndef FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_POINT_MVC_HPP_
#define FINITE_VOLUME_INTERPOLATION_INTERPOLATION_INTERPOLATION_POINT_MVC_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::interpolationPointMVC
// Description
//   Given cell centre values interpolates to vertices and uses these to
//   do a Mean Value Coordinates interpolation.

#include "interpolation.hpp"
#include "point_mvc_weight.hpp"


namespace mousse {

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

// Member Functions 
template<class Type>
inline Type mousse::interpolationPointMVC<Type>::interpolate
(
  const pointMVCWeight& cpw
) const
{
  return cpw.interpolate(psip_);
}


template<class Type>
inline Type mousse::interpolationPointMVC<Type>::interpolate
(
  const vector& position,
  const label cellI,
  const label faceI
) const
{
  return interpolate
  (
    pointMVCWeight(this->pMesh_, position, cellI, faceI)
  );
}


#include "interpolation_point_mvc.ipp"

#endif
