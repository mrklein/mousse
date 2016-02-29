#ifndef FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_STEADY_STATE_DDT_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_STEADY_STATE_DDT_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::steadyStateDdtScheme
// Description
//   SteadyState implicit/explicit ddt which returns 0.
// SourceFiles
//   steady_state_ddt_scheme.cpp


#include "ddt_scheme.hpp"

namespace mousse
{
namespace fv
{
template<class Type>
class steadyStateDdtScheme
:
  public fv::ddtScheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("steadyState");

  // Constructors
    //- Construct from mesh
    steadyStateDdtScheme(const fvMesh& mesh)
    :
      ddtScheme<Type>{mesh}
    {}

    //- Construct from mesh and Istream
    steadyStateDdtScheme(const fvMesh& mesh, Istream& is)
    :
      ddtScheme<Type>{mesh, is}
    {}

    //- Disallow default bitwise copy construct
    steadyStateDdtScheme(const steadyStateDdtScheme&) = delete;

    //- Disallow default bitwise assignment
    steadyStateDdtScheme& operator=(const steadyStateDdtScheme&) = delete;

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
tmp<surfaceScalarField> steadyStateDdtScheme<scalar>::fvcDdtUfCorr
(
  const GeometricField<scalar, fvPatchField, volMesh>& U,
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> steadyStateDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& U,
  const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> steadyStateDdtScheme<scalar>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> steadyStateDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& phi
);
}  // namespace fv
}  // namespace mousse

#ifdef NoRepository
#   include "steady_state_ddt_scheme.cpp"
#endif
#endif
