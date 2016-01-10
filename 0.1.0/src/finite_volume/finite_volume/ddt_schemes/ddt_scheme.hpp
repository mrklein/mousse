// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::ddtScheme
// Description
//   Abstract base class for ddt schemes.
// SourceFiles
//   ddt_scheme.cpp

#ifndef ddt_scheme_hpp_
#define ddt_scheme_hpp_

#include "tmp.hpp"
#include "dimensioned_type.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"

namespace mousse
{
template<class Type>
class fvMatrix;
class fvMesh;
namespace fv
{
template<class Type>
class ddtScheme
:
  public refCount
{
protected:
  // Protected data
    const fvMesh& mesh_;
public:
  //- Runtime type information
  virtual const word& type() const = 0;

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      ddtScheme,
      Istream,
      (const fvMesh& mesh, Istream& schemeData),
      (mesh, schemeData)
    );
  // Constructors
    //- Construct from mesh
    ddtScheme(const fvMesh& mesh)
    :
      mesh_{mesh}
    {}

    //- Construct from mesh and Istream
    ddtScheme(const fvMesh& mesh, Istream&)
    :
      mesh_{mesh}
    {}

    //- Disallow copy construct
    ddtScheme(const ddtScheme&) = delete;

    //- Disallow default bitwise assignment
    ddtScheme& operator=(const ddtScheme&) = delete;

  // Selectors
    //- Return a pointer to a new ddtScheme created on freestore
    static tmp<ddtScheme<Type> > New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );

  //- Destructor
  virtual ~ddtScheme();

  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const dimensioned<Type>&
    ) = 0;

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const volScalarField& alpha,
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmDdt
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmDdt
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmDdt
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmDdt
    (
      const volScalarField& alpha,
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) = 0;

    typedef GeometricField
    <
      typename flux<Type>::type,
      fvsPatchField,
      surfaceMesh
    > fluxFieldType;
    tmp<surfaceScalarField> fvcDdtPhiCoeff
    (
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const fluxFieldType& phi,
      const fluxFieldType& phiCorr
    );

    tmp<surfaceScalarField> fvcDdtPhiCoeff
    (
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const fluxFieldType& phi
    );

    virtual tmp<fluxFieldType> fvcDdtUfCorr
    (
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
    ) = 0;

    virtual tmp<fluxFieldType> fvcDdtPhiCorr
    (
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const fluxFieldType& phi
    ) = 0;

    virtual tmp<fluxFieldType> fvcDdtUfCorr
    (
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
    ) = 0;

    virtual tmp<fluxFieldType> fvcDdtPhiCorr
    (
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const fluxFieldType& phi
    ) = 0;

    virtual tmp<surfaceScalarField> meshPhi
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;
};
}  // namespace fv
}  // namespace mousse

// Add the patch constructor functions to the hash tables
#define MAKE_FV_DDT_TYPE_SCHEME(SS, Type)                                     \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::fv::SS<mousse::Type>, 0); \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace fv                                                              \
    {                                                                         \
      ddtScheme<Type>::addIstreamConstructorToTable<SS<Type> >                \
        add##SS##Type##IstreamConstructorToTable_;                            \
    }                                                                         \
  }

#define MAKE_FV_DDT_SCHEME(SS)                                                \
                                                                              \
MAKE_FV_DDT_TYPE_SCHEME(SS, scalar)                                           \
MAKE_FV_DDT_TYPE_SCHEME(SS, vector)                                           \
MAKE_FV_DDT_TYPE_SCHEME(SS, sphericalTensor)                                  \
MAKE_FV_DDT_TYPE_SCHEME(SS, symmTensor)                                       \
MAKE_FV_DDT_TYPE_SCHEME(SS, tensor)                                           \
                                                                              \
namespace mousse                                                              \
{                                                                             \
namespace fv                                                                  \
{                                                                             \
                                                                              \
template<>                                                                    \
tmp<surfaceScalarField> SS<scalar>::fvcDdtUfCorr                              \
(                                                                             \
  const volScalarField& /*U*/,                                                \
  const surfaceScalarField& /*Uf*/                                            \
)                                                                             \
{                                                                             \
  NOT_IMPLEMENTED(#SS"<scalar>::fvcDdtUfCorr");                               \
  return surfaceScalarField::null();                                          \
}                                                                             \
                                                                              \
template<>                                                                    \
tmp<surfaceScalarField> SS<scalar>::fvcDdtPhiCorr                             \
(                                                                             \
  const volScalarField& /*U*/,                                                \
  const surfaceScalarField& /*phi*/                                           \
)                                                                             \
{                                                                             \
  NOT_IMPLEMENTED(#SS"<scalar>::fvcDdtPhiCorr");                              \
  return surfaceScalarField::null();                                          \
}                                                                             \
                                                                              \
template<>                                                                    \
tmp<surfaceScalarField> SS<scalar>::fvcDdtUfCorr                              \
(                                                                             \
  const volScalarField& /*rho*/,                                              \
  const volScalarField& /*U*/,                                                \
  const surfaceScalarField& /*Uf*/                                            \
)                                                                             \
{                                                                             \
  NOT_IMPLEMENTED(#SS"<scalar>::fvcDdtUfCorr");                               \
  return surfaceScalarField::null();                                          \
}                                                                             \
                                                                              \
template<>                                                                    \
tmp<surfaceScalarField> SS<scalar>::fvcDdtPhiCorr                             \
(                                                                             \
  const volScalarField& /*rho*/,                                              \
  const volScalarField& /*U*/,                                                \
  const surfaceScalarField& /*phi*/                                           \
)                                                                             \
{                                                                             \
  NOT_IMPLEMENTED(#SS"<scalar>::fvcDdtPhiCorr");                              \
  return surfaceScalarField::null();                                          \
}                                                                             \
                                                                              \
}                                                                             \
}

#ifdef NoRepository
#   include "ddt_scheme.cpp"
#endif
#endif
