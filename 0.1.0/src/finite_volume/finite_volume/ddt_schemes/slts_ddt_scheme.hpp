#ifndef FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_SLTS_DDT_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_SLTS_DDT_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::SLTSDdtScheme
// Description
//   Stabilised local time-step first-order Euler implicit/explicit ddt.
//   The time-step is adjusted locally so that an advective equations remains
//   diagonally dominant.
//   This scheme should only be used for steady-state computations
//   using transient codes where local time-stepping is preferably to
//   under-relaxation for transport consistency reasons.
// See Also
//   mousse::fv::CoEulerDdtScheme

#include "ddt_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type>
class SLTSDdtScheme
:
  public fv::ddtScheme<Type>
{
  // Private Data
    //- Name of the flux field used to calculate the local time-step
    word phiName_;
    //- Name of the density field used to obtain the volumetric flux
    //  from the mass flux if required
    word rhoName_;
    //- Under-relaxation factor
    scalar alpha_;
  // Private Member Functions
    //- Calculate a relaxed diagonal from the given flux field
    void relaxedDiag(scalarField& rD, const surfaceScalarField& phi) const;
    //- Return the reciprocal of the stabilised local time-step
    tmp<volScalarField> SLrDeltaT() const;
public:
  //- Runtime type information
  TYPE_NAME("SLTS");

  // Constructors
    //- Construct from mesh and Istream
    SLTSDdtScheme(const fvMesh& mesh, Istream& is)
    :
      ddtScheme<Type>{mesh, is},
      phiName_{is},
      rhoName_{is},
      alpha_{readScalar(is)}
    {}

    //- Disallow default bitwise copy construct
    SLTSDdtScheme(const SLTSDdtScheme&) = delete;

    //- Disallow default bitwise assignment
    SLTSDdtScheme& operator=(const SLTSDdtScheme&) = delete;

  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return fv::ddtScheme<Type>::mesh();
    }

    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcDdt
    (
      const dimensioned<Type>&
    );

    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcDdt
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );

    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcDdt
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );

    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcDdt
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );

    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcDdt
    (
      const volScalarField& alpha,
      const volScalarField& rho,
      const GeometricField<Type, fvPatchField, volMesh>& psi
    );

    tmp<fvMatrix<Type>> fvmDdt
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );

    tmp<fvMatrix<Type>> fvmDdt
    (
      const dimensionedScalar&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );

    tmp<fvMatrix<Type>> fvmDdt
    (
      const volScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );

    tmp<fvMatrix<Type>> fvmDdt
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
tmp<surfaceScalarField> SLTSDdtScheme<scalar>::fvcDdtUfCorr
(
  const GeometricField<scalar, fvPatchField, volMesh>& U,
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> SLTSDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& U,
  const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> SLTSDdtScheme<scalar>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> SLTSDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& phi
);

}  // namespace fv
}  // namespace mousse

#include "slts_ddt_scheme.ipp"

#endif
