// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::steadyStateD2dt2Scheme
// Description
//   SteadyState d2dt2 which returns 0.
// SourceFiles
//   steady_state_d2dt2_scheme.cpp
#ifndef steady_state_d2dt2_scheme_hpp_
#define steady_state_d2dt2_scheme_hpp_
#include "d2dt2_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class steadyStateD2dt2Scheme
:
  public fv::d2dt2Scheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    steadyStateD2dt2Scheme(const steadyStateD2dt2Scheme&);
    //- Disallow default bitwise assignment
    void operator=(const steadyStateD2dt2Scheme&);
public:
  //- Runtime type information
  TypeName("steadyState");
  // Constructors
    //- Construct from mesh
    steadyStateD2dt2Scheme(const fvMesh& mesh)
    :
      d2dt2Scheme<Type>(mesh)
    {}
    //- Construct from mesh and Istream
    steadyStateD2dt2Scheme(const fvMesh& mesh, Istream& is)
    :
      d2dt2Scheme<Type>(mesh, is)
    {}
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return fv::d2dt2Scheme<Type>::mesh();
    }
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcD2dt2
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type> > fvmD2dt2
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type> > fvmD2dt2
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type> > fvmD2dt2
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "steady_state_d2dt2_scheme.cpp"
#endif
#endif
