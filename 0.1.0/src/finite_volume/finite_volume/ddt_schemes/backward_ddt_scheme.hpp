#ifndef FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_BACKWARD_DDT_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_BACKWARD_DDT_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::backwardDdtScheme
// Description
//   Second-order backward-differencing ddt using the current and
//   two previous time-step values.
// SourceFiles
//   backward_ddt_scheme.cpp


#include "ddt_scheme.hpp"

namespace mousse
{

namespace fv
{

template<class Type>
class backwardDdtScheme
:
  public fv::ddtScheme<Type>
{
  // Private Member Functions

    //- Return the current time-step
    scalar deltaT_() const;

    //- Return the previous time-step
    scalar deltaT0_() const;

    //- Return the previous time-step or GREAT if the old timestep field
    //  wasn't available in which case Euler ddt is used
    template<class GeoField>
    scalar deltaT0_(const GeoField&) const;

public:

  //- Runtime type information
  TYPE_NAME("backward");

  // Constructors

    //- Construct from mesh
    backwardDdtScheme(const fvMesh& mesh)
    :
      ddtScheme<Type>{mesh}
    {}

    //- Construct from mesh and Istream
    backwardDdtScheme(const fvMesh& mesh, Istream& is)
    :
      ddtScheme<Type>{mesh, is}
    {}

    //- Disallow default bitwise copy construct
    backwardDdtScheme(const backwardDdtScheme&) = delete;

    //- Disallow default bitwise assignment
    backwardDdtScheme& operator=(const backwardDdtScheme&) = delete;

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
tmp<surfaceScalarField> backwardDdtScheme<scalar>::fvcDdtUfCorr
(
  const GeometricField<scalar, fvPatchField, volMesh>& U,
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> backwardDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& U,
  const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> backwardDdtScheme<scalar>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> backwardDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& phi
);

}  // namespace fv

}  // namespace mousse
#ifdef NoRepository
#   include "backward_ddt_scheme.cpp"
#endif
#endif
