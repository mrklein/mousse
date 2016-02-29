#ifndef FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_LIMITED_GRAD_SCHEMES_CELL_LIMITED_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_GRAD_SCHEMES_LIMITED_GRAD_SCHEMES_CELL_LIMITED_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::cellLimitedGrad
// Description
//   cellLimitedGrad gradient scheme applied to a runTime selected base gradient
//   scheme.
//   The scalar limiter based on limiting the extrapolated face values
//   between the maximum and minumum cell and cell neighbour values and is
//   applied to all components of the gradient.
// SourceFiles
//   cell_limited_grad.cpp


#include "grad_scheme.hpp"

namespace mousse
{

namespace fv
{

template<class Type>
class cellLimitedGrad
:
  public fv::gradScheme<Type>
{
  // Private Data
    tmp<fv::gradScheme<Type>> basicGradScheme_;

    //- Limiter coefficient
    const scalar k_;
public:
  //- RunTime type information
  TYPE_NAME("cellLimited");

  // Constructors
    //- Construct from mesh and schemeData
    cellLimitedGrad(const fvMesh& mesh, Istream& schemeData)
    :
      gradScheme<Type>{mesh},
      basicGradScheme_{fv::gradScheme<Type>::New(mesh, schemeData)},
      k_{readScalar(schemeData)}
    {
      if (k_ < 0 || k_ > 1)
      {
        FATAL_IO_ERROR_IN
        (
          "cellLimitedGrad(const fvMesh&, Istream& schemeData)",
          schemeData
        )
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
      }
    }

    //- Disallow default bitwise copy construct
    cellLimitedGrad(const cellLimitedGrad&) = delete;

    //- Disallow default bitwise assignment
    cellLimitedGrad& operator=(const cellLimitedGrad&) = delete;

  // Member Functions
    static inline void limitFace
    (
      Type& limiter,
      const Type& maxDelta,
      const Type& minDelta,
      const Type& extrapolate
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
};


// Inline Member Function 
template<>
inline void cellLimitedGrad<scalar>::limitFace
(
  scalar& limiter,
  const scalar& maxDelta,
  const scalar& minDelta,
  const scalar& extrapolate
)
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


template<class Type>
inline void cellLimitedGrad<Type>::limitFace
(
  Type& limiter,
  const Type& maxDelta,
  const Type& minDelta,
  const Type& extrapolate
)
{
  for (direction cmpt=0; cmpt<Type::nComponents; cmpt++)
  {
    cellLimitedGrad<scalar>::limitFace
    (
      limiter.component(cmpt),
      maxDelta.component(cmpt),
      minDelta.component(cmpt),
      extrapolate.component(cmpt)
    );
  }
}


// Template Member Function Specialisations 
template<>
tmp<volVectorField> cellLimitedGrad<scalar>::calcGrad
(
  const volScalarField& vsf,
  const word& name
) const;
template<>
tmp<volTensorField> cellLimitedGrad<vector>::calcGrad
(
  const volVectorField& vsf,
  const word& name
) const;

}  // namespace fv

}  // namespace mousse
#endif
