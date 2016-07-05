#ifndef FINITE_VOLUME_FINITE_VOLUME_SN_GRAD_SCHEMES_ORTHOGONAL_SN_GRAD_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_SN_GRAD_SCHEMES_ORTHOGONAL_SN_GRAD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::orthogonalSnGrad
// Description
//   Simple central-difference snGrad scheme without non-orthogonal correction.

#include "sn_grad_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type>
class orthogonalSnGrad
:
  public snGradScheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("orthogonal");

  // Constructors
    
    //- Construct from mesh
    orthogonalSnGrad(const fvMesh& mesh)
    :
      snGradScheme<Type>{mesh}
    {}

    //- Construct from mesh and data stream
    orthogonalSnGrad(const fvMesh& mesh, Istream&)
    :
      snGradScheme<Type>{mesh}
    {}

    //- Disallow default bitwise assignment
    orthogonalSnGrad& operator=(const orthogonalSnGrad&) = delete;

  //- Destructor
  virtual ~orthogonalSnGrad();

  // Member Functions
    //- Return the interpolation weighting factors for the given field
    virtual tmp<surfaceScalarField> deltaCoeffs
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return this->mesh().deltaCoeffs();
    }

    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return false;
    }

    //- Return the explicit correction to the orthogonalSnGrad
    //  for the given field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
    correction(const GeometricField<Type, fvPatchField, volMesh>&) const;

};

}  // namespace fv
}  // namespace mousse

#include "orthogonal_sn_grad.ipp"

#endif
