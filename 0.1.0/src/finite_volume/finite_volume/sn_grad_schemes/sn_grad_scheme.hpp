// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::snGradScheme
// Description
//   Abstract base class for snGrad schemes.
// SourceFiles
//   sn_grad_scheme.cpp
#ifndef sn_grad_scheme_hpp_
#define sn_grad_scheme_hpp_
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
class snGradScheme
:
  public refCount
{
  // Private data
    //- Hold reference to mesh
    const fvMesh& mesh_;
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const snGradScheme&);
public:
  //- Runtime type information
  virtual const word& type() const = 0;
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
      tmp,
      snGradScheme,
      Mesh,
      (const fvMesh& mesh, Istream& schemeData),
      (mesh, schemeData)
    );
  // Constructors
    //- Construct from mesh
    snGradScheme(const fvMesh& mesh)
    :
      mesh_(mesh)
    {}
  // Selectors
    //- Return new tmp interpolation scheme
    static tmp<snGradScheme<Type> > New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );
  //- Destructor
  virtual ~snGradScheme();
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Return the snGrad of the given cell field with the given deltaCoeffs
    static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    snGrad
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const tmp<surfaceScalarField>&,
      const word& snGradName = "snGrad"
    );
    //- Return the sndGrad of the given cell field
    static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    sndGrad
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const word& snGradName = "sndGrad"
    );
    //- Return the interpolation weighting factors for the given field
    virtual tmp<surfaceScalarField> deltaCoeffs
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const = 0;
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return false;
    }
    //- Return the explicit correction to the snGrad
    //  for the given field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction(const GeometricField<Type, fvPatchField, volMesh>&) const
    {
      return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >(NULL);
    }
    //- Return the snGrad of the given cell field
    //  with explicit correction
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    snGrad(const GeometricField<Type, fvPatchField, volMesh>&) const;
    //- Return the snGrad of the given tmp cell field
    //  with explicit correction
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    snGrad
    (
      const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    ) const;
};
}  // namespace fv
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makeSnGradTypeScheme(SS, Type)                                         \
  defineNamedTemplateTypeNameAndDebug(mousse::fv::SS<mousse::Type>, 0);          \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    namespace fv                                                           \
    {                                                                      \
      snGradScheme<Type>::addMeshConstructorToTable<SS<Type> >           \
        add##SS##Type##MeshConstructorToTable_;                        \
    }                                                                      \
  }
#define makeSnGradScheme(SS)                                                   \
                                       \
makeSnGradTypeScheme(SS, scalar)                                               \
makeSnGradTypeScheme(SS, vector)                                               \
makeSnGradTypeScheme(SS, sphericalTensor)                                      \
makeSnGradTypeScheme(SS, symmTensor)                                           \
makeSnGradTypeScheme(SS, tensor)
#ifdef NoRepository
#   include "sn_grad_scheme.cpp"
#endif
#endif
