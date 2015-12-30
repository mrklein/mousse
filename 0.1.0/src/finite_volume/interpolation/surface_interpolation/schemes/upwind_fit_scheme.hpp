// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::UpwindFitScheme
// Description
//   Upwind biased fit surface interpolation scheme that applies an explicit
//   correction to linear.
#ifndef upwind_fit_scheme_hpp_
#define upwind_fit_scheme_hpp_
#include "upwind_fit_data.hpp"
#include "linear.hpp"
namespace mousse
{
template<class Type, class Polynomial, class Stencil>
class UpwindFitScheme
:
  public linear<Type>
{
  // Private Data
    //- Reference to the surface flux used to choose upwind direction
    const surfaceScalarField& faceFlux_;
    //- Factor the fit is allowed to deviate from linear.
    //  This limits the amount of high-order correction and increases
    //  stability on bad meshes
    const scalar linearLimitFactor_;
    //- Weights for central stencil
    const scalar centralWeight_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    UpwindFitScheme(const UpwindFitScheme&);
    //- Disallow default bitwise assignment
    void operator=(const UpwindFitScheme&);
public:
  //- Runtime type information
  TypeName("UpwindFitScheme");
  // Constructors
    //- Construct from mesh and Istream
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    UpwindFitScheme(const fvMesh& mesh, Istream& is)
    :
      linear<Type>(mesh),
      faceFlux_(mesh.lookupObject<surfaceScalarField>(word(is))),
      linearLimitFactor_(readScalar(is)),
      centralWeight_(1000)
    {}
    //- Construct from mesh, faceFlux and Istream
    UpwindFitScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      linear<Type>(mesh),
      faceFlux_(faceFlux),
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
      const extendedUpwindCellToFaceStencil& stencil = Stencil::New
      (
        mesh,
        false,          //pureUpwind
        scalar(0.5)
      );
      const UpwindFitData<Polynomial>& ufd =
      UpwindFitData<Polynomial>::New
      (
        mesh,
        stencil,
        true,           //calculate as offset to linear
        linearLimitFactor_,
        centralWeight_
      );
      const List<scalarList>& fo = ufd.owncoeffs();
      const List<scalarList>& fn = ufd.neicoeffs();
      return stencil.weightedSum(faceFlux_, vf, fo, fn);
    }
};
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makeUpwindFitSurfaceInterpolationTypeScheme\
(                                                                             \
  SS,                                                                       \
  POLYNOMIAL,                                                               \
  STENCIL,                                                                  \
  TYPE                                                                      \
)                                                                             \
                                       \
typedef UpwindFitScheme<TYPE, POLYNOMIAL, STENCIL>                            \
  UpwindFitScheme##TYPE##POLYNOMIAL##STENCIL##_;                            \
defineTemplateTypeNameAndDebugWithName                                        \
  (UpwindFitScheme##TYPE##POLYNOMIAL##STENCIL##_, #SS, 0);                  \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshConstructorToTable                   \
<UpwindFitScheme<TYPE, POLYNOMIAL, STENCIL> >                                 \
  add##SS##STENCIL##TYPE##MeshConstructorToTable_;                          \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable               \
<UpwindFitScheme<TYPE, POLYNOMIAL, STENCIL> >                                 \
  add##SS##STENCIL##TYPE##MeshFluxConstructorToTable_;
#define makeUpwindFitSurfaceInterpolationScheme(SS, POLYNOMIAL, STENCIL)      \
                                       \
makeUpwindFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,scalar)     \
makeUpwindFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,vector)     \
makeUpwindFitSurfaceInterpolationTypeScheme                                   \
(                                                                             \
  SS,                                                                       \
  POLYNOMIAL,                                                               \
  STENCIL,                                                                  \
  sphericalTensor                                                           \
)                                                                             \
makeUpwindFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,symmTensor) \
makeUpwindFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,tensor)
#endif
