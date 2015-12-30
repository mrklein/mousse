// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::convectionScheme
// Description
//   Abstract base class for convection schemes.
// SourceFiles
//   convection_scheme.cpp
#ifndef convection_scheme_hpp_
#define convection_scheme_hpp_
#include "tmp.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
#include "multivariate_surface_interpolation_scheme.hpp"
namespace mousse
{
template<class Type>
class fvMatrix;
class fvMesh;
namespace fv
{
template<class Type>
class convectionScheme
:
  public refCount
{
  // Private data
    const fvMesh& mesh_;
public:
  //- Runtime type information
  virtual const word& type() const = 0;
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
      tmp,
      convectionScheme,
      Istream,
      (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& schemeData
      ),
      (mesh, faceFlux, schemeData)
    );
    declareRunTimeSelectionTable
    (
      tmp,
      convectionScheme,
      Multivariate,
      (
        const fvMesh& mesh,
        const typename multivariateSurfaceInterpolationScheme<Type>::
          fieldTable& fields,
        const surfaceScalarField& faceFlux,
        Istream& schemeData
      ),
      (mesh, fields, faceFlux, schemeData)
    );
  // Constructors
    //- Copy construct
    convectionScheme(const convectionScheme&);
    //- Construct from mesh, flux and Istream
    convectionScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField&
    )
    :
      mesh_(mesh)
    {}
  // Selectors
    //- Return a pointer to a new convectionScheme created on freestore
    static tmp<convectionScheme<Type> > New
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );
    //- Return a pointer to a new multivariate convectionScheme
    //  created on freestore
    static tmp<convectionScheme<Type> > New
    (
      const fvMesh& mesh,
      const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );
  //- Destructor
  virtual ~convectionScheme();
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const = 0;
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const = 0;
    virtual tmp<fvMatrix<Type> > fvmDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const = 0;
    virtual tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const = 0;
  // Member operators
    void operator=(const convectionScheme<Type>&);
};
}  // namespace fv
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makeFvConvectionTypeScheme(SS, Type)                                   \
  defineNamedTemplateTypeNameAndDebug(mousse::fv::SS<mousse::Type>, 0);          \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    namespace fv                                                           \
    {                                                                      \
      convectionScheme<Type>::addIstreamConstructorToTable<SS<Type> >    \
        add##SS##Type##IstreamConstructorToTable_;                     \
    }                                                                      \
  }
#define makeFvConvectionScheme(SS)                                             \
                                       \
makeFvConvectionTypeScheme(SS, scalar)                                         \
makeFvConvectionTypeScheme(SS, vector)                                         \
makeFvConvectionTypeScheme(SS, sphericalTensor)                                \
makeFvConvectionTypeScheme(SS, symmTensor)                                     \
makeFvConvectionTypeScheme(SS, tensor)
#define makeMultivariateFvConvectionTypeScheme(SS, Type)                       \
  defineNamedTemplateTypeNameAndDebug(mousse::fv::SS<mousse::Type>, 0);          \
                                       \
  namespace mousse                                                             \
  {                                                                          \
    namespace fv                                                           \
    {                                                                      \
      convectionScheme<Type>::                                           \
        addMultivariateConstructorToTable<SS<Type> >                   \
        add##SS##Type##MultivariateConstructorToTable_;                \
    }                                                                      \
  }
#define makeMultivariateFvConvectionScheme(SS)                                 \
                                       \
makeMultivariateFvConvectionTypeScheme(SS, scalar)                             \
makeMultivariateFvConvectionTypeScheme(SS, vector)                             \
makeMultivariateFvConvectionTypeScheme(SS, sphericalTensor)                    \
makeMultivariateFvConvectionTypeScheme(SS, symmTensor)                         \
makeMultivariateFvConvectionTypeScheme(SS, tensor)
#ifdef NoRepository
#   include "convection_scheme.cpp"
#endif
#endif
