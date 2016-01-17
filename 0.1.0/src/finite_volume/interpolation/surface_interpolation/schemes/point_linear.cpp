// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_linear.hpp"
#include "fv_mesh.hpp"
#include "vol_point_interpolation.hpp"
#include "triangle.hpp"
#include "surface_fields.hpp"

template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::pointLinear<Type>::
correction
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  const fvMesh& mesh = this->mesh();
  GeometricField<Type, pointPatchField, pointMesh> pvf
  (
    volPointInterpolation::New(mesh).interpolate(vf)
  );
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr =
    linearInterpolate(vf);
  Field<Type>& sfCorr = tsfCorr().internalField();
  const pointField& points = mesh.points();
  const pointField& C = mesh.C().internalField();
  const faceList& faces = mesh.faces();
  const scalarField& w = mesh.weights().internalField();
  const labelList& owner = mesh.owner();
  const labelList& neighbour = mesh.neighbour();
  FOR_ALL(sfCorr, facei)
  {
    point pi =
      w[owner[facei]]*C[owner[facei]]
     + (1.0 - w[owner[facei]])*C[neighbour[facei]];
    const face& f = faces[facei];
    scalar at = triangle<point, const point&>
    (
      pi,
      points[f[0]],
      points[f[f.size()-1]]
    ).mag();
    scalar sumAt = at;
    Type sumPsip = at*(1.0/3.0)*
    (
      sfCorr[facei]
     + pvf[f[0]]
     + pvf[f[f.size()-1]]
    );
    for (label pointi=1; pointi<f.size(); pointi++)
    {
      at = triangle<point, const point&>
      (
        pi,
        points[f[pointi]],
        points[f[pointi-1]]
      ).mag();
      sumAt += at;
      sumPsip += at*(1.0/3.0)*
      (
        sfCorr[facei]
       + pvf[f[pointi]]
       + pvf[f[pointi-1]]
      );
    }
    sfCorr[facei] = sumPsip/sumAt - sfCorr[facei];
  }
  typename GeometricField<Type, fvsPatchField, surfaceMesh>::
    GeometricBoundaryField& bSfCorr = tsfCorr().boundaryField();
  FOR_ALL(bSfCorr, patchi)
  {
    fvsPatchField<Type>& pSfCorr = bSfCorr[patchi];
    if (pSfCorr.coupled())
    {
      const fvPatch& fvp = mesh.boundary()[patchi];
      const scalarField& pWghts = mesh.weights().boundaryField()[patchi];
      const labelUList& pOwner = fvp.faceCells();
      const vectorField& pNbrC = mesh.C().boundaryField()[patchi];
      FOR_ALL(pOwner, facei)
      {
        label own = pOwner[facei];
        point pi =
          pWghts[facei]*C[own]
         + (1.0 - pWghts[facei])*pNbrC[facei];
        const face& f = faces[facei+fvp.start()];
        scalar at = triangle<point, const point&>
        (
          pi,
          points[f[0]],
          points[f[f.size()-1]]
        ).mag();
        scalar sumAt = at;
        Type sumPsip = at*(1.0/3.0)*
        (
          pSfCorr[facei]
         + pvf[f[0]]
         + pvf[f[f.size()-1]]
        );
        for (label pointi=1; pointi<f.size(); pointi++)
        {
          at = triangle<point, const point&>
          (
            pi,
            points[f[pointi]],
            points[f[pointi-1]]
          ).mag();
          sumAt += at;
          sumPsip += at*(1.0/3.0)*
          (
            pSfCorr[facei]
           + pvf[f[pointi]]
           + pvf[f[pointi-1]]
          );
        }
        pSfCorr[facei] = sumPsip/sumAt - pSfCorr[facei];
      }
    }
    else
    {
      pSfCorr = pTraits<Type>::zero;
    }
  }
  return tsfCorr;
}

namespace mousse
{
MAKE_SURFACE_INTERPOLATION_SCHEME(pointLinear);
}
