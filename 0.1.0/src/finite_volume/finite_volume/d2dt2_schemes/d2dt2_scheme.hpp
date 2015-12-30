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
  // Private Member Functions
    //- Disallow copy construct
    d2dt2Scheme(const d2dt2Scheme&);
    //- Disallow default bitwise assignment
    void operator=(const d2dt2Scheme&);
public:
  //- Runtime type information
  virtual const word& type() const = 0;
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
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
      mesh_(mesh)
    {}
    //- Construct from mesh and Istream
    d2dt2Scheme(const fvMesh& mesh, Istream&)
    :
      mesh_(mesh)
    {}
  // Selectors
    //- Return a pointer to a new d2dt2Scheme created on freestore
    static tmp<d2dt2Scheme<Type> > New
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
#define makeFvD2dt2TypeScheme(SS, Type)                                        \
  defineNamedTemplateTypeNameAndDebug(mousse::fv::SS<mousse::Type>, 0);          \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    namespace fv                                                           \
    {                                                                      \
      d2dt2Scheme<Type>::addIstreamConstructorToTable<SS<Type> >         \
        add##SS##Type##IstreamConstructorToTable_;                     \
    }                                                                      \
  }
#define makeFvD2dt2Scheme(SS)                                                  \
                                       \
makeFvD2dt2TypeScheme(SS, scalar)                                              \
makeFvD2dt2TypeScheme(SS, vector)                                              \
makeFvD2dt2TypeScheme(SS, sphericalTensor)                                     \
makeFvD2dt2TypeScheme(SS, symmTensor)                                          \
makeFvD2dt2TypeScheme(SS, tensor)
#ifdef NoRepository
#   include "d2dt2_scheme.cpp"
#endif
#endif
