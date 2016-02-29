#ifndef FINITE_VOLUME_FINITE_VOLUME_DIV_SCHEMES_DIV_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_DIV_SCHEMES_DIV_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::divScheme
// Description
//   Abstract base class for div schemes.
// SourceFiles
//   div_scheme.cpp


#include "tmp.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "linear.hpp"
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
class divScheme
:
  public refCount
{
protected:
  // Protected data
    const fvMesh& mesh_;
    tmp<surfaceInterpolationScheme<Type> > tinterpScheme_;

public:
  //- Runtime type information
  virtual const word& type() const = 0;

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      divScheme,
      Istream,
      (const fvMesh& mesh, Istream& schemeData),
      (mesh, schemeData)
    );

  // Constructors
    //- Construct from mesh
    divScheme(const fvMesh& mesh)
    :
      mesh_{mesh},
      tinterpScheme_{new linear<Type>{mesh}}
    {}

    //- Construct from mesh and Istream
    divScheme(const fvMesh& mesh, Istream& is)
    :
      mesh_{mesh},
      tinterpScheme_{surfaceInterpolationScheme<Type>::New(mesh, is)}
    {}

    //- Disallow copy construct
    divScheme(const divScheme&) = delete;

    //- Disallow default bitwise assignment
    divScheme& operator=(const divScheme&) = delete;

  // Selectors
    //- Return a pointer to a new divScheme created on freestore
    static tmp<divScheme<Type>> New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );

  //- Destructor
  virtual ~divScheme();

  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }

    virtual tmp
    <
      GeometricField
      <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
    > fvcDiv
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) = 0;
};

}  // namespace fv
}  // namespace mousse

// Add the patch constructor functions to the hash tables
#define MAKE_FV_DIV_TYPE_SCHEME(SS, Type)                                     \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::fv::SS<mousse::Type>, 0); \
                                                                              \
  namespace mousse                                                            \
  {                                                                           \
    namespace fv                                                              \
    {                                                                         \
      divScheme<Type>::addIstreamConstructorToTable<SS<Type> >                \
        add##SS##Type##IstreamConstructorToTable_;                            \
    }                                                                         \
  }

#define MAKE_FV_DIV_SCHEME(SS)                                                \
                                                                              \
MAKE_FV_DIV_TYPE_SCHEME(SS, vector)                                           \
MAKE_FV_DIV_TYPE_SCHEME(SS, sphericalTensor)                                  \
MAKE_FV_DIV_TYPE_SCHEME(SS, symmTensor)                                       \
MAKE_FV_DIV_TYPE_SCHEME(SS, tensor)

#ifdef NoRepository
#   include "div_scheme.cpp"
#endif
#endif
