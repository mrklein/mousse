#ifndef FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_CO_EULER_DDT_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_CO_EULER_DDT_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::CoEulerDdtScheme
// Description
//   Courant number limited first-order Euler implicit/explicit ddt.
//   The time-step is adjusted locally so that the local Courant number
//   does not exceed the specified value.
//   This scheme should only be used for steady-state computations
//   using transient codes where local time-stepping is preferable to
//   under-relaxation for transport consistency reasons.

#include "ddt_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type>
class CoEulerDdtScheme
:
  public fv::ddtScheme<Type>
{
  // Private Data
    //- Name of the flux field used to calculate the local time-step
    word phiName_;
    //- Name of the density field used to obtain the volumetric flux
    //  from the mass flux if required
    word rhoName_;
    //- Maximum local Courant number
    scalar maxCo_;
  // Private Member Functions
    //- Return the reciprocal of the Courant-number limited time-step
    tmp<volScalarField> CorDeltaT() const;
    //- Return the reciprocal of the face-Courant-number limited time-step
    tmp<surfaceScalarField> CofrDeltaT() const;
public:
  //- Runtime type information
  TYPE_NAME("CoEuler");

  // Constructors
    //- Construct from mesh and Istream
    CoEulerDdtScheme(const fvMesh& mesh, Istream& is)
    :
      ddtScheme<Type>{mesh, is},
      phiName_{is},
      rhoName_{is},
      maxCo_{readScalar(is)}
    {}

    //- Disallow default bitwise copy construct
    CoEulerDdtScheme(const CoEulerDdtScheme&) = delete;

    //- Disallow default bitwise assignment
    CoEulerDdtScheme& operator=(const CoEulerDdtScheme&) = delete;

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
tmp<surfaceScalarField> CoEulerDdtScheme<scalar>::fvcDdtUfCorr
(
  const GeometricField<scalar, fvPatchField, volMesh>& U,
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> CoEulerDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& U,
  const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> CoEulerDdtScheme<scalar>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> CoEulerDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& phi
);

}  // namespace fv
}  // namespace mousse

#include "co_euler_ddt_scheme.ipp"

#endif