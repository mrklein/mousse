#ifndef FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_LEAST_SQUARES_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_LEAST_SQUARES_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::leastSquaresGrad
// Description
//   Second-order gradient scheme using least-squares.

#include "grad_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type>
class leastSquaresGrad
:
  public fv::gradScheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("leastSquares");
  // Constructors
    //- Construct from mesh
    leastSquaresGrad(const fvMesh& mesh)
    :
      gradScheme<Type>{mesh}
    {}
    //- Construct from Istream
    leastSquaresGrad(const fvMesh& mesh, Istream&)
    :
      gradScheme<Type>{mesh}
    {}
    //- Disallow default bitwise copy construct
    leastSquaresGrad(const leastSquaresGrad&) = delete;
    //- Disallow default bitwise assignment
    leastSquaresGrad& operator=(const leastSquaresGrad&) = delete;
  // Member Functions
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
};

}  // namespace fv
}  // namespace mousse

#include "least_squares_grad.ipp"

#endif
