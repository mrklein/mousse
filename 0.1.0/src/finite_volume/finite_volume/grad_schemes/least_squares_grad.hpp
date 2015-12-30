// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::leastSquaresGrad
// Description
//   Second-order gradient scheme using least-squares.
// SourceFiles
//   least_squares_grad.cpp
#ifndef least_squares_grad_hpp_
#define least_squares_grad_hpp_
#include "grad_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class leastSquaresGrad
:
  public fv::gradScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    leastSquaresGrad(const leastSquaresGrad&);
    //- Disallow default bitwise assignment
    void operator=(const leastSquaresGrad&);
public:
  //- Runtime type information
  TypeName("leastSquares");
  // Constructors
    //- Construct from mesh
    leastSquaresGrad(const fvMesh& mesh)
    :
      gradScheme<Type>(mesh)
    {}
    //- Construct from Istream
    leastSquaresGrad(const fvMesh& mesh, Istream&)
    :
      gradScheme<Type>(mesh)
    {}
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
#ifdef NoRepository
#   include "least_squares_grad.cpp"
#endif
#endif
