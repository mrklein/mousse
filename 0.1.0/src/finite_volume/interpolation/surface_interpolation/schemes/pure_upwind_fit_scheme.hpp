// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PureUpwindFitScheme
// Description
//   Upwind biased fit surface interpolation scheme that applies an explicit
//   correction to upwind.
#ifndef pure_upwind_fit_scheme_hpp_
#define pure_upwind_fit_scheme_hpp_
#include "upwind_fit_data.hpp"
#include "upwind.hpp"
#include "switch.hpp"
namespace mousse
{
template<class Type, class Polynomial, class Stencil>
class PureUpwindFitScheme
:
  public upwind<Type>
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
    PureUpwindFitScheme(const PureUpwindFitScheme&);
    //- Disallow default bitwise assignment
    void operator=(const PureUpwindFitScheme&);
public:
  //- Runtime type information
  TypeName("PureUpwindFitScheme");
  // Constructors
    //- Construct from mesh and Istream
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    PureUpwindFitScheme(const fvMesh& mesh, Istream& is)
    :
      upwind<Type>
      (
        mesh,
        mesh.lookupObject<surfaceScalarField>(word(is))
      ),
      linearLimitFactor_(readScalar(is)),
      centralWeight_(1000)
    {}
    //- Construct from mesh, faceFlux and Istream
    PureUpwindFitScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      upwind<Type>(mesh, faceFlux),
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
      // Use the owner/neighbour splitting constructor
      const extendedUpwindCellToFaceStencil& stencil = Stencil::New(mesh);
      const UpwindFitData<Polynomial>& ufd =
      UpwindFitData<Polynomial>::New
      (
        mesh,
        stencil,
        false,              //offset to upwind
        linearLimitFactor_,
        centralWeight_
      );
      const List<scalarList>& fo = ufd.owncoeffs();
      const List<scalarList>& fn = ufd.neicoeffs();
      return stencil.weightedSum(this->faceFlux_, vf, fo, fn);
    }
};
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makePureUpwindFitSurfaceInterpolationTypeScheme\
(                                                                             \
  SS,                                                                       \
  POLYNOMIAL,                                                               \
  STENCIL,                                                                  \
  TYPE                                                                      \
)                                                                             \
                                       \
typedef PureUpwindFitScheme<TYPE, POLYNOMIAL, STENCIL>                        \
  PureUpwindFitScheme##TYPE##POLYNOMIAL##STENCIL##_;                        \
defineTemplateTypeNameAndDebugWithName                                        \
  (PureUpwindFitScheme##TYPE##POLYNOMIAL##STENCIL##_, #SS, 0);              \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshConstructorToTable                   \
<PureUpwindFitScheme<TYPE, POLYNOMIAL, STENCIL> >                             \
  add##SS##STENCIL##TYPE##MeshConstructorToTable_;                          \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable               \
<PureUpwindFitScheme<TYPE, POLYNOMIAL, STENCIL> >                             \
  add##SS##STENCIL##TYPE##MeshFluxConstructorToTable_;
#define makePureUpwindFitSurfaceInterpolationScheme(SS, POLYNOMIAL, STENCIL)  \
                                       \
makePureUpwindFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,scalar) \
makePureUpwindFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,vector) \
makePureUpwindFitSurfaceInterpolationTypeScheme                               \
(                                                                             \
  SS,                                                                       \
  POLYNOMIAL,                                                               \
  STENCIL,                                                                  \
  sphericalTensor                                                           \
)                                                                             \
makePureUpwindFitSurfaceInterpolationTypeScheme                               \
(                                                                             \
  SS,                                                                       \
  POLYNOMIAL,                                                               \
  STENCIL,                                                                  \
  symmTensor                                                                \
)                                                                             \
makePureUpwindFitSurfaceInterpolationTypeScheme(SS,POLYNOMIAL,STENCIL,tensor)
#endif
