#ifndef FINITE_VOLUME_FINITE_VOLUME_DIV_SCHEMES_GAUSS_DIV_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_DIV_SCHEMES_GAUSS_DIV_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::gaussDivScheme
// Description
//   Basic second-order div using face-gradients and Gauss' theorem.
// SourceFiles
//   gauss_div_scheme.cpp
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
public:
  //- Runtime type information
  TYPE_NAME("Gauss");
  // Constructors
    //- Construct null
    gaussDivScheme(const fvMesh& mesh)
    :
      divScheme<Type>{mesh}
    {}
    //- Construct from Istream
    gaussDivScheme(const fvMesh& mesh, Istream& is)
    :
      divScheme<Type>{mesh, is}
    {}
    //- Disallow default bitwise copy construct
    gaussDivScheme(const gaussDivScheme&) = delete;
    //- Disallow default bitwise assignment
    gaussDivScheme& operator=(const gaussDivScheme&) = delete;
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
