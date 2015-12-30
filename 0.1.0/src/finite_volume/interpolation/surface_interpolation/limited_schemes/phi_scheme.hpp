// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PhiScheme
// Description
//   Class to create the weighting-factors based on the face-flux.
//   The particular differencing scheme class is supplied as a template
//   argument, the weight function of which is called by the weight function
//   of this class for the internal faces as well as faces of coupled
//   patches (e.g. processor-processor patches). The weight function is
//   supplied with the central-differencing weighting factor, the face-flux,
//   the face neighbour cell values and the face area.
//   This code organisation is both neat and efficient, allowing for
//   convenient implementation of new schemes to run on parallelised cases.
// SourceFiles
//   phi_scheme.cpp
#ifndef phi_scheme_hpp_
#define phi_scheme_hpp_
#include "limited_surface_interpolation_scheme.hpp"
namespace mousse
{
template<class Type, class PhiLimiter>
class PhiScheme
:
  public limitedSurfaceInterpolationScheme<Type>,
  public PhiLimiter
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    PhiScheme(const PhiScheme&);
    //- Disallow default bitwise assignment
    void operator=(const PhiScheme&);
public:
  //- Runtime type information
  TypeName("PhiScheme");
  // Constructors
    //- Construct from mesh, faceFlux and blendingFactor
    PhiScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      const PhiLimiter& weight
    )
    :
      limitedSurfaceInterpolationScheme<Type>(mesh, faceFlux),
      PhiLimiter(weight)
    {}
    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    PhiScheme
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      limitedSurfaceInterpolationScheme<Type>(mesh, is),
      PhiLimiter(is)
    {}
    //- Construct from mesh, faceFlux and Istream
    PhiScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      limitedSurfaceInterpolationScheme<Type>(mesh, faceFlux),
      PhiLimiter(is)
    {}
  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> limiter
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makePhiSurfaceInterpolationScheme(SS, WEIGHT, TYPE)                    \
                                       \
typedef PhiScheme<TYPE, WEIGHT> Phischeme##WEIGHT_;                            \
defineTemplateTypeNameAndDebugWithName(Phischeme##WEIGHT_, #SS, 0);            \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshConstructorToTable                    \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshConstructorToTable_;             \
                                       \
surfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable                \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshFluxConstructorToTable_;         \
                                       \
limitedSurfaceInterpolationScheme<TYPE>::addMeshConstructorToTable             \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshConstructorToLimitedTable_;      \
                                       \
limitedSurfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable         \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshFluxConstructorToLimitedTable_;
#ifdef NoRepository
#   include "phi_scheme.cpp"
#endif
#endif
