// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::d2dt2Scheme
// Description
//   Abstract base class for d2dt2 schemes.
// SourceFiles
//   d2dt2_scheme.cpp

#ifndef d2dt2_scheme_hpp_
#define d2dt2_scheme_hpp_

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
class d2dt2Scheme
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
      d2dt2Scheme,
      Istream,
      (const fvMesh& mesh, Istream& schemeData),
      (mesh, schemeData)
    );
  // Constructors
    //- Construct from mesh
    d2dt2Scheme(const fvMesh& mesh)
    :
      mesh_{mesh}
    {}

    //- Construct from mesh and Istream
    d2dt2Scheme(const fvMesh& mesh, Istream&)
    :
      mesh_{mesh}
    {}

    //- Disallow copy construct
    d2dt2Scheme(const d2dt2Scheme&) = delete;

    //- Disallow default bitwise assignment
    d2dt2Scheme& operator=(const d2dt2Scheme&) = delete;

  // Selectors
    //- Return a pointer to a new d2dt2Scheme created on freestore
    static tmp<d2dt2Scheme<Type>> New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );

  //- Destructor
  virtual ~d2dt2Scheme();

  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmD2dt2
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmD2dt2
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;

    virtual tmp<fvMatrix<Type> > fvmD2dt2
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;
};
}  // namespace fv
}  // namespace mousse

// Add the patch constructor functions to the hash tables
#define MAKE_FV_D2DT2_TYPE_SCHEME(SS, Type)                                   \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::fv::SS<mousse::Type>, 0); \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace fv                                                              \
    {                                                                         \
      d2dt2Scheme<Type>::addIstreamConstructorToTable<SS<Type> >              \
        add##SS##Type##IstreamConstructorToTable_;                            \
    }                                                                         \
  }

#define MAKE_FV_D2DT2_SCHEME(SS)                                              \
                                                                              \
MAKE_FV_D2DT2_TYPE_SCHEME(SS, scalar)                                         \
MAKE_FV_D2DT2_TYPE_SCHEME(SS, vector)                                         \
MAKE_FV_D2DT2_TYPE_SCHEME(SS, sphericalTensor)                                \
MAKE_FV_D2DT2_TYPE_SCHEME(SS, symmTensor)                                     \
MAKE_FV_D2DT2_TYPE_SCHEME(SS, tensor)

#ifdef NoRepository
#   include "d2dt2_scheme.cpp"
#endif
#endif
