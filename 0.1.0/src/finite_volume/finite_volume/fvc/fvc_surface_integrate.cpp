// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_surface_integrate.hpp"
#include "fv_mesh.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
namespace mousse
{
namespace fvc
{
template<class Type>
void surfaceIntegrate
(
  Field<Type>& ivf,
  const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
  const fvMesh& mesh = ssf.mesh();
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  const Field<Type>& issf = ssf;
  FOR_ALL(owner, facei)
  {
    ivf[owner[facei]] += issf[facei];
    ivf[neighbour[facei]] -= issf[facei];
  }
  FOR_ALL(mesh.boundary(), patchi)
  {
    const labelUList& pFaceCells =
      mesh.boundary()[patchi].faceCells();
    const fvsPatchField<Type>& pssf = ssf.boundaryField()[patchi];
    FOR_ALL(mesh.boundary()[patchi], facei)
    {
      ivf[pFaceCells[facei]] += pssf[facei];
    }
  }
  ivf /= mesh.Vsc();
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
surfaceIntegrate
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
  const fvMesh& mesh = ssf.mesh();
  tmp<GeometricField<Type, fvPatchField, volMesh> > tvf
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        "surfaceIntegrate("+ssf.name()+')',
        ssf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh,
      dimensioned<Type>
      (
        "0",
        ssf.dimensions()/dimVol,
        pTraits<Type>::zero
      ),
      zeroGradientFvPatchField<Type>::typeName
    )
  );
  GeometricField<Type, fvPatchField, volMesh>& vf = tvf();
  surfaceIntegrate(vf.internalField(), ssf);
  vf.correctBoundaryConditions();
  return tvf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
surfaceIntegrate
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >& tssf
)
{
  tmp<GeometricField<Type, fvPatchField, volMesh> > tvf
  (
    fvc::surfaceIntegrate(tssf())
  );
  tssf.clear();
  return tvf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
surfaceSum
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
  const fvMesh& mesh = ssf.mesh();
  tmp<GeometricField<Type, fvPatchField, volMesh> > tvf
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        "surfaceSum("+ssf.name()+')',
        ssf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh,
      dimensioned<Type>("0", ssf.dimensions(), pTraits<Type>::zero),
      zeroGradientFvPatchField<Type>::typeName
    )
  );
  GeometricField<Type, fvPatchField, volMesh>& vf = tvf();
  const labelUList& owner = mesh.owner();
  const labelUList& neighbour = mesh.neighbour();
  FOR_ALL(owner, facei)
  {
    vf[owner[facei]] += ssf[facei];
    vf[neighbour[facei]] += ssf[facei];
  }
  FOR_ALL(mesh.boundary(), patchi)
  {
    const labelUList& pFaceCells =
      mesh.boundary()[patchi].faceCells();
    const fvsPatchField<Type>& pssf = ssf.boundaryField()[patchi];
    FOR_ALL(mesh.boundary()[patchi], facei)
    {
      vf[pFaceCells[facei]] += pssf[facei];
    }
  }
  vf.correctBoundaryConditions();
  return tvf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> > surfaceSum
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >& tssf
)
{
  tmp<GeometricField<Type, fvPatchField, volMesh> > tvf = surfaceSum(tssf());
  tssf.clear();
  return tvf;
}
}  // namespace fvc
}  // namespace mousse
