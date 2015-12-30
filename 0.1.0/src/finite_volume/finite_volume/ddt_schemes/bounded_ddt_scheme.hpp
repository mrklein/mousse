// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::boundedDdtScheme
// Description
//   Bounded form of the selected ddt scheme.
//   Boundedness is achieved by subtracting ddt(phi)*vf or Sp(ddt(rho), vf)
//   which is non-conservative if ddt(rho) != 0 but conservative otherwise.
//   Can be used for the ddt of bounded scalar properties to improve stability
//   if insufficient convergence of the pressure equation causes temporary
//   divergence of the flux field.
// SourceFiles
//   bounded_ddt_scheme.cpp
#ifndef bounded_ddt_scheme_hpp_
#define bounded_ddt_scheme_hpp_
#include "ddt_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class boundedDdtScheme
:
  public fv::ddtScheme<Type>
{
  // Private data
    tmp<fv::ddtScheme<Type> > scheme_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    boundedDdtScheme(const boundedDdtScheme&);
    //- Disallow default bitwise assignment
    void operator=(const boundedDdtScheme&);
public:
  //- Runtime type information
  TypeName("bounded");
  // Constructors
    //- Construct from mesh and Istream
    boundedDdtScheme(const fvMesh& mesh, Istream& is)
    :
      ddtScheme<Type>(mesh, is),
      scheme_
      (
        fv::ddtScheme<Type>::New(mesh, is)
      )
    {}
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return fv::ddtScheme<Type>::mesh();
    }
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const dimensioned<Type>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDdt
    (
      const volScalarField& alpha,
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
    tmp<fvMatrix<Type> > fvmDdt
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type> > fvmDdt
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type> > fvmDdt
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type> > fvmDdt
    (
      const volScalarField& alpha,
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );
    typedef typename ddtScheme<Type>::fluxFieldType fluxFieldType;
    tmp<fluxFieldType> fvcDdtUfCorr
    (
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
    );
    tmp<fluxFieldType> fvcDdtPhiCorr
    (
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const fluxFieldType& phi
    );
    tmp<fluxFieldType> fvcDdtUfCorr
    (
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
    );
    tmp<fluxFieldType> fvcDdtPhiCorr
    (
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& U,
      const fluxFieldType& phi
    );
    tmp<surfaceScalarField> meshPhi
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
};
template<>
tmp<surfaceScalarField> boundedDdtScheme<scalar>::fvcDdtUfCorr
(
  const GeometricField<scalar, fvPatchField, volMesh>& U,
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);
template<>
tmp<surfaceScalarField> boundedDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& U,
  const surfaceScalarField& phi
);
template<>
tmp<surfaceScalarField> boundedDdtScheme<scalar>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& Uf
);
template<>
tmp<surfaceScalarField> boundedDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& phi
);
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "bounded_ddt_scheme.cpp"
#endif
#endif
