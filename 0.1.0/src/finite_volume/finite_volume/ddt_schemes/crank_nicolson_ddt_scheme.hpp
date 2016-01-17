// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::CrankNicolsonDdtScheme
// Description
//   Second-oder Crank-Nicolson implicit ddt using the current and
//   previous time-step fields as well as the previous time-step ddt.
//   The Crank-Nicolson scheme is often unstable for complex flows in complex
//   geometries and it is necessary to "off-centre" the scheme to stabilize it
//   while retaining greater temporal accuracy than the first-order
//   Euler-implicit scheme.  Off-centering is specified via the mandatory
//   coefficient in the range [0,1] following the scheme name e.g.
//   \verbatim
//   ddtSchemes
//   {
//     default         CrankNicolson 0.9;
//   }
//   \endverbatim
//   With a coefficient of 1 the scheme is fully centred and second-order,
//   with a coefficient of 0 the scheme is equivalent to Euler-implicit.
//   A coefficient of 0.9 has been found to be suitable for a range of cases for
//   which higher-order accuracy in time is needed and provides similar accuracy
//   and stability to the backward scheme.  However, the backward scheme has
//   been found to be more robust and provides formal second-order accuracy in
//   time.
//   The advantage of the Crank-Nicolson scheme over backward is that only the
//   new and old-time values are needed, the additional terms relating to the
//   fluxes and sources are evaluated at the mid-point of the time-step which
//   provides the opportunity to limit the fluxes in such a way as to ensure
//   boundedness while maintaining greater accuracy in time compared to the
//   Euler-implicit scheme.  This approach is now used with MULES in the
//   mousse-inter family of solvers.  Boundedness cannot be guaranteed with the
//   backward scheme.
// Note
//   The Crank-Nicolson coefficient for the implicit part of the RHS is related
//   to the off-centering coefficient by
//   \verbatim
//     cnCoeff = 1.0/(1.0 + ocCoeff);
//   \endverbatim
// See Also
//   mousse::Euler
//   mousse::backward
// SourceFiles
//   crank_nicolson_ddt_scheme.cpp

#ifndef crank_nicolson_ddt_scheme_hpp_
#define crank_nicolson_ddt_scheme_hpp_

#include "ddt_scheme.hpp"
#include "ioobject.hpp"

namespace mousse
{

namespace fv
{

template<class Type>
class CrankNicolsonDdtScheme
:
  public fv::ddtScheme<Type>
{
  // Private Data

    //- Class to store the ddt0 fields on the objectRegistry for use in the
    //  next time-step.  The start-time index of the CN scheme is also
    //  stored to help handle the transition from Euler to CN
    template<class GeoField>
    class DDt0Field
    :
      public GeoField
    {
      label startTimeIndex_;

    public:

      //- Constructor from file for restart.
      DDt0Field
      (
        const IOobject& io,
        const fvMesh& mesh
      );

      //- Constructor from components, initisalised to zero with given
      //  dimensions.
      DDt0Field
      (
        const IOobject& io,
        const fvMesh& mesh,
        const dimensioned<typename GeoField::value_type>& dimType
      );

      //- Return the start-time index
      label startTimeIndex() const;

      //- Cast to the underlying GeoField
      GeoField& operator()();

      //- Assignment to a GeoField
      void operator=(const GeoField& gf);

    };

    //- Off-centering coefficient, 1 -> CN, less than one blends with EI
    scalar ocCoeff_;

  // Private Member Functions

    template<class GeoField>
    DDt0Field<GeoField>& ddt0_
    (
      const word& name,
      const dimensionSet& dims
    );

    //- Check if the ddt0 needs to be evaluated for this time-step
    template<class GeoField>
    bool evaluate(const DDt0Field<GeoField>& ddt0) const;

    //- Return the coefficient for Euler scheme for the first time-step
    //  for and CN thereafter
    template<class GeoField>
    scalar coef_(const DDt0Field<GeoField>&) const;

    //- Return the old time-step coefficient for Euler scheme for the
    //  second time-step and for CN thereafter
    template<class GeoField>
    scalar coef0_(const DDt0Field<GeoField>&) const;

    //- Return the reciprocal time-step coefficient for Euler for the
    //  first time-step and CN thereafter
    template<class GeoField>
    dimensionedScalar rDtCoef_(const DDt0Field<GeoField>&) const;

    //- Return the reciprocal old time-step coefficient for Euler for the
    //  second time-step and CN thereafter
    template<class GeoField>
    dimensionedScalar rDtCoef0_(const DDt0Field<GeoField>&) const;

    //- Return ddt0 multiplied by the off-centreing coefficient
    template<class GeoField>
    tmp<GeoField> offCentre_(const GeoField& ddt0) const;

public:

  //- Runtime type information
  TYPE_NAME("CrankNicolson");

  // Constructors

    //- Construct from mesh
    CrankNicolsonDdtScheme(const fvMesh& mesh)
    :
      ddtScheme<Type>{mesh},
      ocCoeff_{1.0}
    {}

    //- Construct from mesh and Istream
    CrankNicolsonDdtScheme(const fvMesh& mesh, Istream& is)
    :
      ddtScheme<Type>{mesh, is},
      ocCoeff_{readScalar(is)}
    {
      if (ocCoeff_ < 0 || ocCoeff_ > 1)
      {
        FATAL_IO_ERROR_IN
        (
          "CrankNicolsonDdtScheme(const fvMesh& mesh, Istream& is)",
          is
        )
        << "Off-centreing coefficient = " << ocCoeff_
        << " should be >= 0 and <= 1"
        << exit(FatalIOError);
      }
    }

    //- Disallow default bitwise copy construct
    CrankNicolsonDdtScheme(const CrankNicolsonDdtScheme&) = delete;

    //- Disallow default bitwise assignment
    CrankNicolsonDdtScheme& operator=(const CrankNicolsonDdtScheme&) = delete;

  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return fv::ddtScheme<Type>::mesh();
    }

    //- Return the off-centreing coefficient
    scalar ocCoeff() const
    {
      return ocCoeff_;
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
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtUfCorr
(
  const GeometricField<scalar, fvPatchField, volMesh>& U,
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& Uf
);

template<>
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& U,
  const surfaceScalarField& phi
);

template<>
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& Uf
);

template<>
tmp<surfaceScalarField> CrankNicolsonDdtScheme<scalar>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const volScalarField& U,
  const surfaceScalarField& phi
);

}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "crank_nicolson_ddt_scheme.cpp"
#endif
#endif
