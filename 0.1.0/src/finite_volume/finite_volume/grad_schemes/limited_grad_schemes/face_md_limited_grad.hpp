// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::faceMDLimitedGrad
// Description
//   faceMDLimitedGrad gradient scheme applied to a runTime selected
//   base gradient scheme.
//   The scalar limiter based on limiting the extrapolated face values
//   between the face-neighbour cell values and is applied to the gradient
//   in each face direction separately.
// SourceFiles
//   face_md_limited_grad.cpp
#ifndef face_md_limited_grad_hpp_
#define face_md_limited_grad_hpp_
#include "grad_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class faceMDLimitedGrad
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
    faceMDLimitedGrad(const faceMDLimitedGrad&);
    //- Disallow default bitwise assignment
    void operator=(const faceMDLimitedGrad&);
public:
  //- RunTime type information
  TypeName("faceMDLimited");
  // Constructors
    //- Construct from mesh and schemeData
    faceMDLimitedGrad(const fvMesh& mesh, Istream& schemeData)
    :
      gradScheme<Type>(mesh),
      basicGradScheme_(fv::gradScheme<Type>::New(mesh, schemeData)),
      k_(readScalar(schemeData))
    {
      if (k_ < 0 || k_ > 1)
      {
        FatalIOErrorIn
        (
          "faceMDLimitedGrad(const fvMesh&, Istream& schemeData)",
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
    ) const;
};
// Template Member Function Specialisations 
template<>
tmp<volVectorField> faceMDLimitedGrad<scalar>::calcGrad
(
  const volScalarField& vsf,
  const word& name
) const;
template<>
tmp<volTensorField> faceMDLimitedGrad<vector>::calcGrad
(
  const volVectorField& vsf,
  const word& name
) const;
}  // namespace fv
}  // namespace mousse
#endif
