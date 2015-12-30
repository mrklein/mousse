// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multivariateIndependentScheme
// Description
//   Generic multi-variate discretisation scheme class for which any of the
//   NVD, CNVD or NVDV schemes may be selected for each variable and applied
//   independently.
//   This is equivalent to using separate "div" terms and schemes for each
//   variable/equation.
// SourceFiles
//   multivariate_independent_scheme.cpp
#ifndef multivariate_independent_scheme_hpp_
#define multivariate_independent_scheme_hpp_
#include "multivariate_surface_interpolation_scheme.hpp"
#include "limited_surface_interpolation_scheme.hpp"
#include "surface_fields.hpp"
namespace mousse
{
template<class Type>
class multivariateIndependentScheme
:
  public multivariateSurfaceInterpolationScheme<Type>
{
  // Private data
    dictionary schemes_;
    const surfaceScalarField& faceFlux_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    multivariateIndependentScheme(const multivariateIndependentScheme&);
    //- Disallow default bitwise assignment
    void operator=(const multivariateIndependentScheme&);
public:
  //- Runtime type information
  TypeName("multivariateIndependent");
  // Constructors
    //- Construct for field, faceFlux and Istream
    multivariateIndependentScheme
    (
      const fvMesh& mesh,
      const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );
  // Member Operators
    tmp<surfaceInterpolationScheme<Type> > operator()
    (
      const GeometricField<Type, fvPatchField, volMesh>& field
    ) const
    {
      return surfaceInterpolationScheme<Type>::New
      (
        faceFlux_.mesh(),
        faceFlux_,
        schemes_.lookup(field.name())
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "multivariate_independent_scheme.cpp"
#endif
#endif
