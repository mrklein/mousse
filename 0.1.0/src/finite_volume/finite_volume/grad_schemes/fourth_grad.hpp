// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::fourthGrad
// Description
//   Second-order gradient scheme using least-squares.
// SourceFiles
//   fourth_grad.cpp
#ifndef fourth_grad_hpp_
#define fourth_grad_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    fourthGrad(const fourthGrad&);
    //- Disallow default bitwise assignment
    void operator=(const fourthGrad&);
public:
  //- Runtime type information
  TypeName("fourth");
  // Constructors
    //- Construct from Istream
    fourthGrad(const fvMesh& mesh, Istream&)
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
#   include "fourth_grad.cpp"
#endif
#endif
