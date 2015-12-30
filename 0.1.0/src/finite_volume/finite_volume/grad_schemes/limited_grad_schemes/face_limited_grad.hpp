// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::faceLimitedGrad
// Description
//   faceLimitedGrad gradient scheme applied to a runTime selected base gradient
//   scheme.
//   The scalar limiter based on limiting the extrapolated face values
//   between the face-neighbour cell values and is applied to all components
//   of the gradient.
// SourceFiles
//   face_limited_grad.cpp
#ifndef face_limited_grad_hpp_
#define face_limited_grad_hpp_
#include "grad_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class faceLimitedGrad
:
  public fv::gradScheme<Type>
{
  // Private Data
    tmp<fv::gradScheme<Type> > basicGradScheme_;
    //- Limiter coefficient
    const scalar k_;
  // Private Member Functions
    inline void limitFace
    (
      scalar& limiter,
      const scalar maxDelta,
      const scalar minDelta,
      const scalar extrapolate
    ) const;
    //- Disallow default bitwise copy construct
    faceLimitedGrad(const faceLimitedGrad&);
    //- Disallow default bitwise assignment
    void operator=(const faceLimitedGrad&);
public:
  //- RunTime type information
  TypeName("faceLimited");
  // Constructors
    //- Construct from mesh and schemeData
    faceLimitedGrad(const fvMesh& mesh, Istream& schemeData)
    :
      gradScheme<Type>(mesh),
      basicGradScheme_(fv::gradScheme<Type>::New(mesh, schemeData)),
      k_(readScalar(schemeData))
    {
      if (k_ < 0 || k_ > 1)
      {
        FatalIOErrorIn
        (
          "faceLimitedGrad(const fvMesh&, Istream& schemeData)",
          schemeData
        )   << "coefficient = " << k_
          << " should be >= 0 and <= 1"
          << exit(FatalIOError);
      }
    }
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
    ) const
    {
      return grad(vsf);
    }
};
// Inline Member Function 
template<class Type>
inline void faceLimitedGrad<Type>::limitFace
(
  scalar& limiter,
  const scalar maxDelta,
  const scalar minDelta,
  const scalar extrapolate
) const
{
  if (extrapolate > maxDelta + VSMALL)
  {
    limiter = min(limiter, maxDelta/extrapolate);
  }
  else if (extrapolate < minDelta - VSMALL)
  {
    limiter = min(limiter, minDelta/extrapolate);
  }
}
// Template Member Function Specialisations 
template<>
tmp<volVectorField> faceLimitedGrad<scalar>::calcGrad
(
  const volScalarField& vsf,
  const word& name
) const;
template<>
tmp<volTensorField> faceLimitedGrad<vector>::calcGrad
(
  const volVectorField& vsf,
  const word& name
) const;
}  // namespace fv
}  // namespace mousse
#endif
