// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CentredFitScheme
// Description
//   Centred fit surface interpolation scheme which applies an explicit
//   correction to linear.
#ifndef centred_fit_scheme_hpp_
#define centred_fit_scheme_hpp_
#include "centred_fit_data.hpp"
#include "linear.hpp"
namespace mousse
{
template<class Type, class Polynomial, class Stencil>
class CentredFitScheme
:
  public linear<Type>
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
    CentredFitScheme(const CentredFitScheme&);
    //- Disallow default bitwise assignment
    void operator=(const CentredFitScheme&);
public:
  //- Runtime type information
  TypeName("CentredFitScheme");
  // Constructors
    //- Construct from mesh and Istream
    CentredFitScheme(const fvMesh& mesh, Istream& is)
    :
      linear<Type>(mesh),
      linearLimitFactor_(readScalar(is)),
      centralWeight_(1000)
    {}
    //- Construct from mesh, faceFlux and Istream
    CentredFitScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      linear<Type>(mesh),
      linearLimitFactor_(readScalar(is)),
      centralWeight_(1000)
    {}
  // Member Functions
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
      const CentredFitData<Polynomial>& cfd =
      CentredFitData<Polynomial>::New
      (
        mesh,
        stencil,
        linearLimitFactor_,
        centralWeight_
      );
      const List<scalarList>& f = cfd.coeffs();
      return stencil.weightedSum(vf, f);
    }
};
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makeCentredFitSurfaceInterpolationTypeScheme\
(                                                                             \
  SS,                                                                       \
  POLYNOMIAL,                                                               \
  STENCIL,                                                                  \
  TYPE                                                                      \
)                                                                             \
                                       \
typedef CentredFitScheme<TYPE, POLYNOMIAL, STENCIL>                           \
  CentredFitScheme##TYPE##POLYNOMIAL##STENCIL##_;                           \
defineTemplateTypeNameAndDebugWithName                                        \
  (CentredFitScheme##TYPE##POLYNOMIAL##STENCIL##_, #SS, 0);                 \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshConstructorToTable                   \
<CentredFitScheme<TYPE, POLYNOMIAL, STENCIL> >                                \
  add##SS##STENCIL##TYPE##MeshConstructorToTable_;                          \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable               \
<CentredFitScheme<TYPE, POLYNOMIAL, STENCIL> >                                \
  add##SS##STENCIL##TYPE##MeshFluxConstructorToTable_;
#define makeCentredFitSurfaceInterpolationScheme(SS, POLYNOMIAL, STENCIL)     \
                                       \
makeCentredFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,scalar)    \
makeCentredFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,vector)    \
makeCentredFitSurfaceInterpolationTypeScheme                                  \
(                                                                             \
  SS,                                                                       \
  POLYNOMIAL,                                                               \
  STENCIL,                                                                  \
  sphericalTensor                                                           \
)                                                                             \
makeCentredFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,symmTensor)\
makeCentredFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,tensor)
#endif
