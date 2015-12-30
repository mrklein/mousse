// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CentredFitSnGradScheme
// Description
//   Centred fit snGrad scheme which applies an explicit correction to snGrad
#ifndef centred_fit_sn_grad_scheme_hpp_
#define centred_fit_sn_grad_scheme_hpp_
#include "centred_fit_sn_grad_data.hpp"
#include "sn_grad_scheme.hpp"
namespace mousse
{
class fvMesh;
namespace fv
{
template<class Type, class Polynomial, class Stencil>
class CentredFitSnGradScheme
:
  public snGradScheme<Type>
{
  // Private Data
    //- Factor the fit is allowed to deviate from linear.
    //  This limits the amount of high-order correction and increases
    //  stability on bad meshes
    const scalar linearLimitFactor_;
    //- Weights for central stencil
    const scalar centralWeight_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    CentredFitSnGradScheme(const CentredFitSnGradScheme&);
    //- Disallow default bitwise assignment
    void operator=(const CentredFitSnGradScheme&);
public:
  //- Runtime type information
  TypeName("CentredFitSnGradScheme");
  // Constructors
    //- Construct from mesh and Istream
    CentredFitSnGradScheme(const fvMesh& mesh, Istream& is)
    :
      snGradScheme<Type>(mesh),
      linearLimitFactor_(readScalar(is)),
      centralWeight_(1000)
    {}
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
    //- Return the explicit correction to the face-interpolate
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      const fvMesh& mesh = this->mesh();
      const extendedCentredCellToFaceStencil& stencil = Stencil::New
      (
        mesh
      );
      const CentredFitSnGradData<Polynomial>& cfd =
        CentredFitSnGradData<Polynomial>::New
        (
          mesh,
          stencil,
          linearLimitFactor_,
          centralWeight_
        );
      tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > sft
      (
        stencil.weightedSum(vf, cfd.coeffs())
      );
      sft().dimensions() /= dimLength;
      return sft;
    }
};
}  // namespace fv
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makeCentredFitSnGradTypeScheme(SS, POLYNOMIAL, STENCIL, TYPE)          \
  typedef mousse::fv::CentredFitSnGradScheme                                   \
    <mousse::TYPE, mousse::POLYNOMIAL, mousse::STENCIL>                          \
    CentredFitSnGradScheme##TYPE##POLYNOMIAL##STENCIL##_;                  \
                                       \
  defineTemplateTypeNameAndDebugWithName                                     \
    (CentredFitSnGradScheme##TYPE##POLYNOMIAL##STENCIL##_, #SS, 0);        \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    namespace fv                                                           \
    {                                                                      \
      snGradScheme<TYPE>::addMeshConstructorToTable                      \
        <CentredFitSnGradScheme<TYPE, POLYNOMIAL, STENCIL> >           \
        add##SS##STENCIL##TYPE##MeshConstructorToTable_;               \
    }                                                                      \
  }
#define makeCentredFitSnGradScheme(SS, POLYNOMIAL, STENCIL)                    \
                                       \
  makeCentredFitSnGradTypeScheme(SS,POLYNOMIAL,STENCIL,scalar)               \
  makeCentredFitSnGradTypeScheme(SS,POLYNOMIAL,STENCIL,vector)               \
  makeCentredFitSnGradTypeScheme(SS,POLYNOMIAL,STENCIL,sphericalTensor)      \
  makeCentredFitSnGradTypeScheme(SS,POLYNOMIAL,STENCIL,symmTensor)           \
  makeCentredFitSnGradTypeScheme(SS,POLYNOMIAL,STENCIL,tensor)
#endif
