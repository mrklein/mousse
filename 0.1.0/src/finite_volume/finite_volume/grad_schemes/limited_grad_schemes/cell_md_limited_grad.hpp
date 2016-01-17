// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::cellMDLimitedGrad
// Description
//   cellMDLimitedGrad gradient scheme applied to a runTime selected base
//   gradient scheme.
//   The scalar limiter based on limiting the extrapolated face values
//   between the maximum and minimum cell and cell neighbour values and is
//   applied to the gradient in each face direction separately.
// SourceFiles
//   cell_md_limited_grad.cpp

#ifndef cell_md_limited_grad_hpp_
#define cell_md_limited_grad_hpp_

#include "grad_scheme.hpp"

namespace mousse
{

namespace fv
{

template<class Type>
class cellMDLimitedGrad
:
  public fv::gradScheme<Type>
{
  // Private Data
    tmp<fv::gradScheme<Type>> basicGradScheme_;

    //- Limiter coefficient
    const scalar k_;

public:
  //- RunTime type information
  TYPE_NAME("cellMDLimited");

  // Constructors
    //- Construct from mesh and schemeData
    cellMDLimitedGrad(const fvMesh& mesh, Istream& schemeData)
    :
      gradScheme<Type>{mesh},
      basicGradScheme_{fv::gradScheme<Type>::New(mesh, schemeData)},
      k_{readScalar(schemeData)}
    {
      if (k_ < 0 || k_ > 1)
      {
        FATAL_IO_ERROR_IN
        (
          "cellMDLimitedGrad(const fvMesh&, Istream& schemeData)",
          schemeData
        )
        << "coefficient = " << k_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
      }
    }

    //- Disallow default bitwise copy construct
    cellMDLimitedGrad(const cellMDLimitedGrad&) = delete;

    //- Disallow default bitwise assignment
    cellMDLimitedGrad& operator=(const cellMDLimitedGrad&) = delete;

  // Member Functions
    static inline void limitFace
    (
      typename outerProduct<vector, Type>::type& g,
      const Type& maxDelta,
      const Type& minDelta,
      const vector& dcf
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

template<>
inline void cellMDLimitedGrad<scalar>::limitFace
(
  vector& g,
  const scalar& maxDelta,
  const scalar& minDelta,
  const vector& dcf
)
{
  scalar extrapolate = dcf & g;
  if (extrapolate > maxDelta)
  {
    g = g + dcf*(maxDelta - extrapolate)/magSqr(dcf);
  }
  else if (extrapolate < minDelta)
  {
    g = g + dcf*(minDelta - extrapolate)/magSqr(dcf);
  }
}


template<class Type>
inline void cellMDLimitedGrad<Type>::limitFace
(
  typename outerProduct<vector, Type>::type& g,
  const Type& maxDelta,
  const Type& minDelta,
  const vector& dcf
)
{
  for (direction cmpt=0; cmpt<Type::nComponents; cmpt++)
  {
    vector gi(g[cmpt], g[cmpt+3], g[cmpt+6]);
    cellMDLimitedGrad<scalar>::limitFace
    (
      gi,
      maxDelta.component(cmpt),
      minDelta.component(cmpt),
      dcf
    );
    g[cmpt] = gi.x();
    g[cmpt+3] = gi.y();
    g[cmpt+6] = gi.z();
  }
}


// Template Member Function Specialisations 
template<>
tmp<volVectorField> cellMDLimitedGrad<scalar>::calcGrad
(
  const volScalarField& vsf,
  const word& name
) const;

template<>
tmp<volTensorField> cellMDLimitedGrad<vector>::calcGrad
(
  const volVectorField& vsf,
  const word& name
) const;

}  // namespace fv

}  // namespace mousse

#endif
