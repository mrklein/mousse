// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::gaussDivScheme
// Description
//   Basic second-order div using face-gradients and Gauss' theorem.
// SourceFiles
//   gauss_div_scheme.cpp
#ifndef gauss_div_scheme_hpp_
#define gauss_div_scheme_hpp_
#include "div_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class gaussDivScheme
:
  public fv::divScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    gaussDivScheme(const gaussDivScheme&);
    //- Disallow default bitwise assignment
    void operator=(const gaussDivScheme&);
public:
  //- Runtime type information
  TypeName("Gauss");
  // Constructors
    //- Construct null
    gaussDivScheme(const fvMesh& mesh)
    :
      divScheme<Type>(mesh)
    {}
    //- Construct from Istream
    gaussDivScheme(const fvMesh& mesh, Istream& is)
    :
      divScheme<Type>(mesh, is)
    {}
  // Member Functions
    tmp
    <
      GeometricField
      <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
    > fvcDiv
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "gauss_div_scheme.cpp"
#endif
#endif
