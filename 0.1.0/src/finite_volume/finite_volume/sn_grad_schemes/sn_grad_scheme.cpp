// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv.hpp"
#include "sn_grad_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "hash_table.hpp"

namespace mousse
{

namespace fv
{

// Selectors
template<class Type>
tmp<snGradScheme<Type> > snGradScheme<Type>::New
(
  const fvMesh& mesh,
  Istream& schemeData
)
{
  if (fv::debug)
  {
    Info<< "snGradScheme<Type>::New(const fvMesh&, Istream&)"
       " : constructing snGradScheme<Type>"
      << endl;
  }
  if (schemeData.eof())
  {
    FATAL_IO_ERROR_IN
    (
      "snGradScheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )
    << "Discretisation scheme not specified"
    << endl << endl
    << "Valid schemes are :" << endl
    << MeshConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  const word schemeName(schemeData);
  typename MeshConstructorTable::iterator constructorIter =
    MeshConstructorTablePtr_->find(schemeName);
  if (constructorIter == MeshConstructorTablePtr_->end())
  {
    FATAL_IO_ERROR_IN
    (
      "snGradScheme<Type>::New(const fvMesh&, Istream&)",
      schemeData
    )
    << "Unknown discretisation scheme "
    << schemeName << nl << nl
    << "Valid schemes are :" << endl
    << MeshConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return constructorIter()(mesh, schemeData);
}

// Destructor 
template<class Type>
snGradScheme<Type>::~snGradScheme()
{}

// Member Functions 
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
snGradScheme<Type>::snGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const tmp<surfaceScalarField>& tdeltaCoeffs,
  const word& snGradName
)
{
  const fvMesh& mesh = vf.mesh();
  // construct GeometricField<Type, fvsPatchField, surfaceMesh>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
  {
    new GeometricField<Type, fvsPatchField, surfaceMesh>
    {
      IOobject
      {
        snGradName + "("+vf.name()+')',
        vf.instance(),
        vf.mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      vf.dimensions()*tdeltaCoeffs().dimensions()
    }
  };
  GeometricField<Type, fvsPatchField, surfaceMesh>& ssf = tsf();
  // set reference to difference factors array
  const scalarField& deltaCoeffs = tdeltaCoeffs().internalField();
  // owner/neighbour addressing
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  FOR_ALL(owner, facei)
  {
    ssf[facei] =
      deltaCoeffs[facei]*(vf[neighbour[facei]] - vf[owner[facei]]);
  }
  FOR_ALL(vf.boundaryField(), patchi)
  {
    const fvPatchField<Type>& pvf = vf.boundaryField()[patchi];
    if (pvf.coupled())
    {
      ssf.boundaryField()[patchi] =
        pvf.snGrad(tdeltaCoeffs().boundaryField()[patchi]);
    }
    else
    {
      ssf.boundaryField()[patchi] = pvf.snGrad();
    }
  }
  return tsf;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
snGradScheme<Type>::sndGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const word& sndGradName
)
{
  return snGrad(vf, vf.mesh().nonOrthDeltaCoeffs(), sndGradName);
}


//- Return the face-snGrad of the given cell field
//  with explicit correction
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
snGradScheme<Type>::snGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
  (
    snGrad(vf, deltaCoeffs(vf))
  );
  if (corrected())
  {
    tsf() += correction(vf);
  }
  return tsf;
}


//- Return the face-snGrad of the given cell field
//  with explicit correction
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
snGradScheme<Type>::snGrad
(
  const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
) const
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
  (
    snGrad(tvf())
  );
  tsf.clear();
  return tsf;
}

}  // namespace fv

}  // namespace mousse
