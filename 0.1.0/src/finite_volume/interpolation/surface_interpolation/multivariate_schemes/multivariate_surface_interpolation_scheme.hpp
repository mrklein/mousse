// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multivariateSurfaceInterpolationScheme
// Description
//   Abstract base class for multi-variate surface interpolation schemes.
// SourceFiles
//   multivariate_surface_interpolation_scheme.cpp

#ifndef multivariate_surface_interpolation_scheme_hpp_
#define multivariate_surface_interpolation_scheme_hpp_

#include "surface_interpolation_scheme.hpp"
#include "hash_table.hpp"

namespace mousse
{
template<class Type>
class multivariateSurfaceInterpolationScheme
:
  public refCount
{
public:
  //- fieldTable
  class fieldTable
  :
    public HashTable<const GeometricField<Type, fvPatchField, volMesh>*>
  {
  public:
    fieldTable()
    {}
    void add(const GeometricField<Type, fvPatchField, volMesh>& f)
    {
      this->insert(f.name(), &f);
    }
  };
private:
  // Private data
    //- Hold reference to mesh
    const fvMesh& mesh_;
    //- HashTable of pointers to the field set
    const fieldTable& fields_;
public:
  //- Runtime type information
  virtual const word& type() const = 0;
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      multivariateSurfaceInterpolationScheme,
      Istream,
      (
        const fvMesh& mesh,
        const fieldTable& fields,
        const surfaceScalarField& faceFlux,
        Istream& is
      ),
      (mesh, fields, faceFlux, is)
    );
  // Constructors
    //- Construct for interpolating given field
    multivariateSurfaceInterpolationScheme
    (
      const fvMesh& mesh,
      const fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );
    //- Disallow default bitwise copy construct
    multivariateSurfaceInterpolationScheme
    (
      const multivariateSurfaceInterpolationScheme&
    ) = delete;
    //- Disallow default bitwise assignment
    multivariateSurfaceInterpolationScheme& operator=
    (
      const multivariateSurfaceInterpolationScheme&
    ) = delete;
  // Selectors
    //- Return a pointer to a new gradScheme created on freestore
    static tmp<multivariateSurfaceInterpolationScheme<Type> > New
    (
      const fvMesh& mesh,
      const fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );
  //- Destructor
  virtual ~multivariateSurfaceInterpolationScheme();
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Return fields to be interpolated
    const fieldTable& fields() const
    {
      return fields_;
    }
  // Member Operators
    //- surfaceInterpolationScheme sub-class returned by operator(field)
    //  which is used as the interpolation scheme for the field
    class fieldScheme
    :
      public surfaceInterpolationScheme<Type>
    {
    public:
      // Constructors
        fieldScheme
        (
          const GeometricField<Type, fvPatchField, volMesh>& field
        )
        :
          surfaceInterpolationScheme<Type>{field.mesh()}
        {}
      // Member Functions
        //- Return the interpolation weighting factors
        virtual tmp<surfaceScalarField> weights
        (
          const GeometricField<Type, fvPatchField, volMesh>& field
        ) const = 0;
    };
    virtual tmp<surfaceInterpolationScheme<Type> > operator()
    (
      const GeometricField<Type, fvPatchField, volMesh>& field
    ) const = 0;
};
}  // namespace mousse

// Add the patch constructor functions to the hash tables
#define MAKE_MULTIVARIATE_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, Type)         \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<Type>, 0);                       \
                                                                              \
multivariateSurfaceInterpolationScheme<Type>::                                \
addIstreamConstructorToTable<SS<Type> >                                       \
  add##SS##Type##ConstructorToTable_;

#define MAKE_MULTIVARIATE_SURFACE_INTERPOLATION_SCHEME(SS)                    \
                                                                              \
MAKE_MULTIVARIATE_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, scalar)               \
MAKE_MULTIVARIATE_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, vector)               \
MAKE_MULTIVARIATE_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, sphericalTensor)      \
MAKE_MULTIVARIATE_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, symmTensor)           \
MAKE_MULTIVARIATE_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, tensor)

#ifdef NoRepository
#   include "multivariate_surface_interpolation_scheme.cpp"
#endif
#endif
