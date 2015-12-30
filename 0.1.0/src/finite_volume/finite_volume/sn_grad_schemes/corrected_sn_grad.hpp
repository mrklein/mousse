// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::correctedSnGrad
// Description
//   Simple central-difference snGrad scheme with non-orthogonal correction.
// SourceFiles
//   corrected_sn_grad.cpp
#ifndef corrected_sn_grad_hpp_
#define corrected_sn_grad_hpp_
#include "sn_grad_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class correctedSnGrad
:
  public snGradScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const correctedSnGrad&);
public:
  //- Runtime type information
  TypeName("corrected");
  // Constructors
    //- Construct from mesh
    correctedSnGrad(const fvMesh& mesh)
    :
      snGradScheme<Type>(mesh)
    {}
    //- Construct from mesh and data stream
    correctedSnGrad(const fvMesh& mesh, Istream&)
    :
      snGradScheme<Type>(mesh)
    {}
  //- Destructor
  virtual ~correctedSnGrad();
  // Member Functions
    //- Return the interpolation weighting factors for the given field
    virtual tmp<surfaceScalarField> deltaCoeffs
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return this->mesh().nonOrthDeltaCoeffs();
    }
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return true;
    }
    //- Return the explicit correction to the correctedSnGrad
    //  for the given field using the gradient of the field
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    fullGradCorrection
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    //- Return the explicit correction to the correctedSnGrad
    //  for the given field using the gradients of the field components
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction(const GeometricField<Type, fvPatchField, volMesh>&) const;
};
// Template Member Function Specialisations 
template<>
tmp<surfaceScalarField> correctedSnGrad<scalar>::correction
(
  const volScalarField& vsf
) const;
template<>
tmp<surfaceVectorField> correctedSnGrad<vector>::correction
(
  const volVectorField& vvf
) const;
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "corrected_sn_grad.cpp"
#endif
#endif
