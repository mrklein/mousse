#ifndef FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_TLEAST_SQUARES_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_TLEAST_SQUARES_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::LeastSquaresGrad
// Description
//   Gradient calculated using weighted least-squares on an arbitrary stencil.
//   The stencil type is provided via a template argument and any cell-based
//   stencil is supported:
//   \table
//     Stencil                     | Connections     | Scheme name
//     centredCFCCellToCellStencil | cell-face-cell  | Not Instantiated
//     centredCPCCellToCellStencil | cell-point-cell | pointCellsLeastSquares
//     centredCECCellToCellStencil | cell-edge-cell  | edgeCellsLeastSquares
//   \endtable
//   The first of these is not instantiated by default as the standard
//   leastSquaresGrad is equivalent and more efficient.
//   \heading Usage
//   Example of the gradient specification:
//   \verbatim
//   gradSchemes
//   {
//     default         pointCellsLeastSquares;
//   }
//   \endverbatim
// See Also
//   mousse::fv::LeastSquaresVectors
//   mousse::fv::leastSquaresGrad
// SourceFiles
//   _least_squares_grad.cpp
//   _least_squares_vectors.hpp
//   _least_squares_vectors.cpp


#include "grad_scheme.hpp"

namespace mousse
{

namespace fv
{

template<class Type, class Stencil>
class LeastSquaresGrad
:
  public fv::gradScheme<Type>
{
  // Private Data
    //- Minimum determinant criterion to choose extra cells
    scalar minDet_;

public:
  //- Runtime type information
  TYPE_NAME("LeastSquares");

  // Constructors
    //- Construct from Istream
    LeastSquaresGrad(const fvMesh& mesh, Istream& /*schemeData*/)
    :
      gradScheme<Type>{mesh}
    {}

    //- Disallow default bitwise copy construct
    LeastSquaresGrad(const LeastSquaresGrad&) = delete;

    //- Disallow default bitwise assignment
    LeastSquaresGrad& operator=(const LeastSquaresGrad&) = delete;

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

// Add the patch constructor functions to the hash tables
#define MAKE_LEAST_SQUARES_GRAD_TYPE_SCHEME(SS, STENCIL, TYPE)                \
  typedef mousse::fv::LeastSquaresGrad<mousse::TYPE, mousse::STENCIL>         \
    LeastSquaresGrad##TYPE##STENCIL##_;                                       \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
    (LeastSquaresGrad##TYPE##STENCIL##_, #SS, 0);                             \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace fv                                                              \
    {                                                                         \
      typedef LeastSquaresGrad<mousse::TYPE, mousse::STENCIL>                 \
        LeastSquaresGrad##TYPE##STENCIL##_;                                   \
                                                                              \
      gradScheme<mousse::TYPE>::addIstreamConstructorToTable                  \
        <LeastSquaresGrad<mousse::TYPE, mousse::STENCIL> >                    \
        add##SS##STENCIL##TYPE##IstreamConstructorToTable_;                   \
    }                                                                         \
  }

#define MAKE_LEAST_SQUARES_GRAD_SCHEME(SS, STENCIL)                           \
  typedef mousse::fv::LeastSquaresVectors<mousse::STENCIL>                    \
    LeastSquaresVectors##STENCIL##_;                                          \
                                                                              \
  DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                               \
    (LeastSquaresVectors##STENCIL##_, #SS, 0);                                \
                                                                              \
  MAKE_LEAST_SQUARES_GRAD_TYPE_SCHEME(SS,STENCIL,scalar)                      \
  MAKE_LEAST_SQUARES_GRAD_TYPE_SCHEME(SS,STENCIL,vector)

#ifdef NoRepository
#   include "_least_squares_grad.cpp"
#endif
#endif
