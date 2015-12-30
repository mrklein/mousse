// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::gradScheme
// Description
//   Abstract base class for gradient schemes.
// SourceFiles
//   grad_scheme.cpp
#ifndef grad_scheme_hpp_
#define grad_scheme_hpp_
#include "tmp.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class fvMesh;
namespace fv
{
template<class Type>
class gradScheme
:
  public refCount
{
  // Private data
    const fvMesh& mesh_;
  // Private Member Functions
    //- Disallow copy construct
    gradScheme(const gradScheme&);
    //- Disallow default bitwise assignment
    void operator=(const gradScheme&);
public:
  //- Runtime type information
  virtual const word& type() const = 0;
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
      tmp,
      gradScheme,
      Istream,
      (const fvMesh& mesh, Istream& schemeData),
      (mesh, schemeData)
    );
  // Constructors
    //- Construct from mesh
    gradScheme(const fvMesh& mesh)
    :
      mesh_(mesh)
    {}
  // Selectors
    //- Return a pointer to a new gradScheme created on freestore
    static tmp<gradScheme<Type> > New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );
  //- Destructor
  virtual ~gradScheme();
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Calculate and return the grad of the given field.
    //  Used by grad either to recalculate the cached gradient when it is
    //  out of date with respect to the field or when it is not cached.
    virtual tmp
    <
      GeometricField
      <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > calcGrad
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const word& name
    ) const = 0;
    //- Calculate and return the grad of the given field
    //  which may have been cached
    tmp
    <
      GeometricField
      <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const word& name
    ) const;
    //- Calculate and return the grad of the given field
    //  with the default name
    //  which may have been cached
    tmp
    <
      GeometricField
      <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    //- Calculate and return the grad of the given field
    //  with the default name
    //  which may have been cached
    tmp
    <
      GeometricField
      <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
      const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    ) const;
};
}  // namespace fv
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makeFvGradTypeScheme(SS, Type)                                         \
  defineNamedTemplateTypeNameAndDebug(mousse::fv::SS<mousse::Type>, 0);          \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    namespace fv                                                           \
    {                                                                      \
      gradScheme<Type>::addIstreamConstructorToTable<SS<Type> >          \
        add##SS##Type##IstreamConstructorToTable_;                     \
    }                                                                      \
  }
#define makeFvGradScheme(SS)                                                   \
                                       \
makeFvGradTypeScheme(SS, scalar)                                               \
makeFvGradTypeScheme(SS, vector)
#ifdef NoRepository
#   include "grad_scheme.cpp"
#endif
#endif
