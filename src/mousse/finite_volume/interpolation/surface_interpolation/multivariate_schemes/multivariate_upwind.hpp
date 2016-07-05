#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_MULTIVARIATE_SCHEMES_MULTIVARIATE_UPWIND_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_MULTIVARIATE_SCHEMES_MULTIVARIATE_UPWIND_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multivariateUpwind
// Description
//   Upwind-difference form of the multivariate surfaceInterpolationScheme.

#include "multivariate_surface_interpolation_scheme.hpp"
#include "surface_fields.hpp"


namespace mousse {

template<class Type>
class multivariateUpwind
:
  public multivariateSurfaceInterpolationScheme<Type>
{
  // Private data
    const surfaceScalarField& faceFlux_;
public:
  //- Runtime type information
  TYPE_NAME("upwind");
  // Constructors
    //- Construct for field, faceFlux and Istream
    multivariateUpwind
    (
      const fvMesh& mesh,
      const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    )
    :
      multivariateSurfaceInterpolationScheme<Type>
      {
        mesh,
        fields,
        faceFlux,
        schemeData
      },
      faceFlux_{faceFlux}
    {}
    //- Disallow default bitwise copy construct
    multivariateUpwind(const multivariateUpwind&) = delete;
    //- Disallow default bitwise assignment
    multivariateUpwind& operator=(const multivariateUpwind&) = delete;
  // Member Operators
    //- surfaceInterpolationScheme sub-class returned by operator(field)
    //  which is used as the interpolation scheme for the field
    class fieldScheme
    :
      public multivariateSurfaceInterpolationScheme<Type>::fieldScheme
    {
      // Private data
        const surfaceScalarField& faceFlux_;
    public:
      // Constructors
        fieldScheme
        (
          const GeometricField<Type, fvPatchField, volMesh>& field,
          const surfaceScalarField& faceFlux
        )
        :
          multivariateSurfaceInterpolationScheme<Type>::fieldScheme{field},
          faceFlux_{faceFlux}
        {}
      // Member Functions
        //- Return the interpolation weighting factors
        tmp<surfaceScalarField> weights
        (
          const GeometricField<Type, fvPatchField, volMesh>&
        ) const
        {
          return pos(faceFlux_);
        }
    };
    tmp<surfaceInterpolationScheme<Type> > operator()
    (
      const GeometricField<Type, fvPatchField, volMesh>& field
    ) const
    {
      return tmp<surfaceInterpolationScheme<Type> >
      {
        new fieldScheme{field, faceFlux_}
      };
    }
};
}  // namespace mousse
#endif
