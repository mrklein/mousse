// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv.hpp"
#include "hash_table.hpp"
#include "surface_interpolate.hpp"
#include "fv_matrix.hpp"
#include "cyclic_ami_fv_patch.hpp"

namespace mousse
{

namespace fv
{

// Selectors
template<class Type>
tmp<ddtScheme<Type>> ddtScheme<Type>::New
(
  const fvMesh& mesh,
  Istream& schemeData
)
{
  if (fv::debug)
  {
    Info<< "ddtScheme<Type>::New(const fvMesh&, Istream&) : "
       "constructing ddtScheme<Type>"
      << endl;
  }
  if (schemeData.eof())
  {
    FATAL_IO_ERROR_IN
    (
      "ddtScheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )
    << "Ddt scheme not specified" << endl << endl
    << "Valid ddt schemes are :" << endl
    << IstreamConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  const word schemeName(schemeData);
  typename IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(schemeName);
  if (cstrIter == IstreamConstructorTablePtr_->end())
  {
    FATAL_IO_ERROR_IN
    (
      "ddtScheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )
    << "Unknown ddt scheme " << schemeName << nl << nl
    << "Valid ddt schemes are :" << endl
    << IstreamConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return cstrIter()(mesh, schemeData);
}


// Destructor 
template<class Type>
ddtScheme<Type>::~ddtScheme()
{}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>> ddtScheme<Type>::fvcDdt
(
  const volScalarField& /*alpha*/,
  const volScalarField& /*rho*/,
  const GeometricField<Type, fvPatchField, volMesh>& /*vf*/
)
{
  NOT_IMPLEMENTED("fvmDdt(alpha, rho, psi");
  return tmp<GeometricField<Type, fvPatchField, volMesh>>
  (
    GeometricField<Type, fvPatchField, volMesh>::null()
  );
}


template<class Type>
tmp<fvMatrix<Type>> ddtScheme<Type>::fvmDdt
(
  const volScalarField& alpha,
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  NOT_IMPLEMENTED("fvmDdt(alpha, rho, psi");
  return tmp<fvMatrix<Type>>
  {
    new fvMatrix<Type>
    {
      vf,
      alpha.dimensions()*rho.dimensions()
      *vf.dimensions()*dimVol/dimTime
    }
  };
}


template<class Type>
tmp<surfaceScalarField> ddtScheme<Type>::fvcDdtPhiCoeff
(
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const fluxFieldType& phi,
  const fluxFieldType& phiCorr
)
{
  tmp<surfaceScalarField> tddtCouplingCoeff = scalar(1)
   - min
    (
      mag(phiCorr)
     /(mag(phi) + dimensionedScalar("small", phi.dimensions(), SMALL)),
      scalar(1)
    );
  surfaceScalarField& ddtCouplingCoeff = tddtCouplingCoeff();
  FOR_ALL(U.boundaryField(), patchi)
  {
    if (U.boundaryField()[patchi].fixesValue()
        || isA<cyclicAMIFvPatch>(mesh().boundary()[patchi]))
    {
      ddtCouplingCoeff.boundaryField()[patchi] = 0.0;
    }
  }
  if (debug > 1)
  {
    Info<< "ddtCouplingCoeff mean max min = "
      << gAverage(ddtCouplingCoeff.internalField())
      << " " << gMax(ddtCouplingCoeff.internalField())
      << " " << gMin(ddtCouplingCoeff.internalField())
      << endl;
  }
  return tddtCouplingCoeff;
}


template<class Type>
tmp<surfaceScalarField> ddtScheme<Type>::fvcDdtPhiCoeff
(
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const fluxFieldType& phi
)
{
  return fvcDdtPhiCoeff(U, phi, phi - (mesh().Sf() & fvc::interpolate(U)));
}

}  // namespace fv

}  // namespace mousse
