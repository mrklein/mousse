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
#ifndef _least_squares_grad_hpp_
#define _least_squares_grad_hpp_
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
  // Private Member Functions
    //- Disallow default bitwise copy construct
    LeastSquaresGrad(const LeastSquaresGrad&);
    //- Disallow default bitwise assignment
    void operator=(const LeastSquaresGrad&);
public:
  //- Runtime type information
  TypeName("LeastSquares");
  // Constructors
    //- Construct from Istream
    LeastSquaresGrad(const fvMesh& mesh, Istream& schemeData)
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

// Add the patch constructor functions to the hash tables
#define makeLeastSquaresGradTypeScheme(SS, STENCIL, TYPE)                     \
  typedef mousse::fv::LeastSquaresGrad<mousse::TYPE, mousse::STENCIL>         \
    LeastSquaresGrad##TYPE##STENCIL##_;                                       \
                                                                              \
  defineTemplateTypeNameAndDebugWithName                                      \
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

#define makeLeastSquaresGradScheme(SS, STENCIL)                               \
  typedef mousse::fv::LeastSquaresVectors<mousse::STENCIL>                    \
    LeastSquaresVectors##STENCIL##_;                                          \
                                                                              \
  defineTemplateTypeNameAndDebugWithName                                      \
    (LeastSquaresVectors##STENCIL##_, #SS, 0);                                \
                                                                              \
  makeLeastSquaresGradTypeScheme(SS,STENCIL,scalar)                           \
  makeLeastSquaresGradTypeScheme(SS,STENCIL,vector)

#ifdef NoRepository
#   include "_least_squares_grad.cpp"
#endif
#endif
