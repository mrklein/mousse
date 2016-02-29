#ifndef FINITE_VOLUME_FINITE_VOLUME_LAPLACIAN_SCHEMES_LAPLACIAN_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_LAPLACIAN_SCHEMES_LAPLACIAN_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::laplacianScheme
// Description
//   Abstract base class for laplacian schemes.
// SourceFiles
//   laplacian_scheme.cpp


#include "tmp.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "linear.hpp"
#include "corrected_sn_grad.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"

namespace mousse
{

template<class Type>
class fvMatrix;

class fvMesh;

namespace fv
{

template<class Type, class GType>
class laplacianScheme
:
  public refCount
{
protected:
  // Protected data
    const fvMesh& mesh_;

    tmp<surfaceInterpolationScheme<GType> > tinterpGammaScheme_;

    tmp<snGradScheme<Type> > tsnGradScheme_;

public:
  //- Runtime type information
  virtual const word& type() const = 0;

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      laplacianScheme,
      Istream,
      (const fvMesh& mesh, Istream& schemeData),
      (mesh, schemeData)
    );

  // Constructors
    //- Construct from mesh
    laplacianScheme(const fvMesh& mesh)
    :
      mesh_{mesh},
      tinterpGammaScheme_{new linear<GType>{mesh}},
      tsnGradScheme_{new correctedSnGrad<Type>{mesh}}
    {}

    //- Construct from mesh and Istream
    laplacianScheme(const fvMesh& mesh, Istream& is)
    :
      mesh_{mesh},
      tinterpGammaScheme_{NULL},
      tsnGradScheme_{NULL}
    {
      tinterpGammaScheme_ = tmp<surfaceInterpolationScheme<GType> >
      {
        surfaceInterpolationScheme<GType>::New(mesh, is)
      };
      tsnGradScheme_ = tmp<snGradScheme<Type> >
      {
        snGradScheme<Type>::New(mesh, is)
      };
    }

    //- Construct from mesh, interpolation and snGradScheme schemes
    laplacianScheme
    (
      const fvMesh& mesh,
      const tmp<surfaceInterpolationScheme<GType> >& igs,
      const tmp<snGradScheme<Type> >& sngs
    )
    :
      mesh_(mesh),
      tinterpGammaScheme_(igs),
      tsnGradScheme_(sngs)
    {}

    //- Disallow copy construct
    laplacianScheme(const laplacianScheme&) = delete;

    //- Disallow default bitwise assignment
    laplacianScheme& operator=(const laplacianScheme&) = delete;

  // Selectors
    //- Return a pointer to a new laplacianScheme created on freestore
    static tmp<laplacianScheme<Type, GType> > New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );

  //- Destructor
  virtual ~laplacianScheme();

  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }

    virtual tmp<fvMatrix<Type> > fvmLaplacian
    (
      const GeometricField<GType, fvsPatchField, surfaceMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmLaplacian
    (
      const GeometricField<GType, fvPatchField, volMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
    (
      const GeometricField<GType, fvsPatchField, surfaceMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
    (
      const GeometricField<GType, fvPatchField, volMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );

};

}  // namespace fv

}  // namespace mousse

// Add the patch constructor functions to the hash tables
#define MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, GType, Type)                        \
  typedef mousse::fv::SS<mousse::Type, mousse::GType> SS##Type##GType;        \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS##Type##GType, 0);              \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace fv                                                              \
    {                                                                         \
      typedef SS<Type, GType> SS##Type##GType;                                \
                                                                              \
      laplacianScheme<Type, GType>::                                          \
        addIstreamConstructorToTable<SS<Type, GType> >                        \
        add##SS##Type##GType##IstreamConstructorToTable_;                     \
    }                                                                         \
  }

#define MAKE_FV_LAPLACIAN_SCHEME(SS)                                          \
                                                                              \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, scalar, scalar)                             \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, symmTensor, scalar)                         \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, tensor, scalar)                             \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, scalar, vector)                             \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, symmTensor, vector)                         \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, tensor, vector)                             \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, scalar, sphericalTensor)                    \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, symmTensor, sphericalTensor)                \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, tensor, sphericalTensor)                    \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, scalar, symmTensor)                         \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, symmTensor, symmTensor)                     \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, tensor, symmTensor)                         \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, scalar, tensor)                             \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, symmTensor, tensor)                         \
MAKE_FV_LAPLACIAN_TYPE_SCHEME(SS, tensor, tensor)

#ifdef NoRepository
#   include "laplacian_scheme.cpp"
#endif

#endif
