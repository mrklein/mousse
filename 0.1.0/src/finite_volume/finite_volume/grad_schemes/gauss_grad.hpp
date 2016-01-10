// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::gaussGrad
// Description
//   Basic second-order gradient scheme using face-interpolation
//   and Gauss' theorem.
// SourceFiles
//   gauss_grad.cpp

#ifndef gauss_grad_hpp_
#define gauss_grad_hpp_

#include "grad_scheme.hpp"
#include "surface_interpolation_scheme.hpp"
#include "linear.hpp"

namespace mousse
{

namespace fv
{

template<class Type>
class gaussGrad
:
  public fv::gradScheme<Type>
{
  // Private data
    tmp<surfaceInterpolationScheme<Type> > tinterpScheme_;

public:
  //- Runtime type information
  TYPE_NAME("Gauss");

  // Constructors
    //- Construct from mesh
    gaussGrad(const fvMesh& mesh)
    :
      gradScheme<Type>{mesh},
      tinterpScheme_{new linear<Type>{mesh}}
    {}

    //- Construct from mesh and Istream
    gaussGrad(const fvMesh& mesh, Istream& is)
    :
      gradScheme<Type>{mesh},
      tinterpScheme_{NULL}
    {
      if (is.eof())
      {
        tinterpScheme_ =
          tmp<surfaceInterpolationScheme<Type>>{new linear<Type>{mesh}};
      }
      else
      {
        tinterpScheme_ =
          tmp<surfaceInterpolationScheme<Type>>
          {
            surfaceInterpolationScheme<Type>::New(mesh, is)
          };
      }
    }

    //- Disallow default bitwise copy construct
    gaussGrad(const gaussGrad&) = delete;

    //- Disallow default bitwise assignment
    gaussGrad& operator=(const gaussGrad&) = delete;

  // Member Functions
    //- Return the gradient of the given field
    //  calculated using Gauss' theorem on the given surface field
    static
    tmp
    <
      GeometricField
      <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > gradf
    (
      const GeometricField<Type, fvsPatchField, surfaceMesh>&,
      const word& name
    );

    //- Return the gradient of the given field to the gradScheme::grad
    //  for optional caching
    virtual tmp
    <
      GeometricField
      <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > calcGrad
    (
      const GeometricField<Type, fvPatchField, volMesh>& vsf,
      const word& name
    ) const;

    //- Correct the boundary values of the gradient using the patchField
    // snGrad functions
    static void correctBoundaryConditions
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      GeometricField
      <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>&
    );
};

}  // namespace fv

}  // namespace mousse
#ifdef NoRepository
#   include "gauss_grad.cpp"
#endif
#endif
