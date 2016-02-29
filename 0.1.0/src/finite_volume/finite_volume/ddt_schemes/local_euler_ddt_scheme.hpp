#ifndef FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_LOCAL_EULER_DDT_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_DDT_SCHEMES_LOCAL_EULER_DDT_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::localEulerDdtScheme
// Description
//   Local time-step first-order Euler implicit/explicit ddt.
//   The reciprocal of the local time-step field is looked-up from the database.
//   This scheme should only be used for steady-state computations using
//   transient codes where local time-stepping is preferably to under-relaxation
//   for transport consistency reasons.
// See Also
//   mousse::fv::CoEulerDdtScheme
// SourceFiles
//   local_euler_ddt.cpp
//   local_euler_ddt_scheme.cpp
//   local_euler_ddt_schemes.cpp


#include "ddt_scheme.hpp"

namespace mousse
{

namespace fv
{

class localEulerDdt
{
public:
  //- Name of the reciprocal local time-step field
  static word rDeltaTName;

  //- Name of the reciprocal local sub-cycling time-step field
  static word rSubDeltaTName;

  // Constructors
    localEulerDdt()
    {}

  // Member Functions

    //- Return true if LTS is enabled
    static bool enabled(const fvMesh& mesh);

    //- Return the reciprocal of the local time-step
    //  looked-up from the objectRegistry
    static const volScalarField& localRDeltaT(const fvMesh& mesh);

    //- Calculate and return the reciprocal of the local sub-cycling
    //  time-step
    static tmp<volScalarField> localRSubDeltaT
    (
      const fvMesh& mesh,
      const label nAlphaSubCycles
    );
};

template<class Type>
class localEulerDdtScheme
:
  public localEulerDdt,
  public fv::ddtScheme<Type>
{
  // Private Member Functions

    //- Return the reciprocal of the local time-step
    const volScalarField& localRDeltaT() const;

public:

  //- Runtime type information
  TYPE_NAME("localEuler");

  // Constructors
    //- Construct from mesh
    localEulerDdtScheme(const fvMesh& mesh)
    :
      ddtScheme<Type>{mesh}
    {}

    //- Construct from mesh and Istream
    localEulerDdtScheme(const fvMesh& mesh, Istream& is)
    :
      ddtScheme<Type>{mesh, is}
    {}

    //- Disallow default bitwise copy construct
    localEulerDdtScheme(const localEulerDdtScheme&) = delete;

    //- Disallow default bitwise assignment
    localEulerDdtScheme& operator=(const localEulerDdtScheme&) = delete;

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
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtUfCorr
(
  const GeometricField<scalar, fvPatchField, volMesh>& U,
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& U,
  const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> localEulerDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& phi
);

}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "local_euler_ddt_scheme.cpp"
#endif
#endif
