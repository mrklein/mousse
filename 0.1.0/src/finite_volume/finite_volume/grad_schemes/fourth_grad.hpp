#ifndef FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_FOURTH_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_FOURTH_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::fourthGrad
// Description
//   Second-order gradient scheme using least-squares.
// SourceFiles
//   fourth_grad.cpp
#include "grad_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class fourthGrad
:
  public fv::gradScheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("fourth");
  // Constructors
    //- Construct from Istream
    fourthGrad(const fvMesh& mesh, Istream&)
    :
      gradScheme<Type>{mesh}
    {}
    //- Disallow default bitwise copy construct
    fourthGrad(const fourthGrad&) = delete;
    //- Disallow default bitwise assignment
    fourthGrad& operator=(const fourthGrad&) = delete;
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
#include "fourth_grad.cpp"
#endif
#endif
