// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_corrected_sn_grad.hpp"
#include "vol_point_interpolation.hpp"
#include "triangle.hpp"

// Destructor 
template<class Type>
mousse::fv::faceCorrectedSnGrad<Type>::~faceCorrectedSnGrad()
{}


// Member Functions 
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::fv::faceCorrectedSnGrad<Type>::fullGradCorrection
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  const fvMesh& mesh = this->mesh();
  GeometricField<Type, pointPatchField, pointMesh> pvf
  {
    volPointInterpolation::New(mesh).interpolate(vf)
  };
  // construct GeometricField<Type, fvsPatchField, surfaceMesh>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr
  {
    new GeometricField<Type, fvsPatchField, surfaceMesh>
    {
      IOobject
      {
        "snGradCorr(" + vf.name() + ')',
        vf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      vf.dimensions()*mesh.nonOrthDeltaCoeffs().dimensions()
    }
  };
  Field<Type>& sfCorr = tsfCorr().internalField();
  const pointField& points = mesh.points();
  const faceList& faces = mesh.faces();
  const vectorField& Sf = mesh.Sf().internalField();
  const vectorField& C = mesh.C().internalField();
  const scalarField& magSf = mesh.magSf().internalField();
  const labelList& owner = mesh.owner();
  const labelList& neighbour = mesh.neighbour();
  FOR_ALL(sfCorr, facei)
  {
    typename outerProduct<vector, Type>::type fgrad
    (
      outerProduct<vector, Type>::type::zero
    );
    const face& fi = faces[facei];
    vector nf(Sf[facei]/magSf[facei]);
    for (label pi=0; pi<fi.size(); pi++)
    {
      // Next point index
      label pj = (pi+1)%fi.size();
      // Edge normal in plane of face
      vector edgen(nf^(points[fi[pj]] - points[fi[pi]]));
      // Edge centre field value
      Type pvfe(0.5*(pvf[fi[pj]] + pvf[fi[pi]]));
      // Integrate face gradient
      fgrad += edgen*pvfe;
    }
    // Finalize face-gradient by dividing by face area
    fgrad /= magSf[facei];
    // Calculate correction vector
    vector dCorr(C[neighbour[facei]] - C[owner[facei]]);
    dCorr /= (nf & dCorr);
    // if (mag(dCorr) > 2) dCorr *= 2/mag(dCorr);
    sfCorr[facei] = dCorr&fgrad;
  }
  tsfCorr().boundaryField() = pTraits<Type>::zero;

  return tsfCorr;
}


template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::fv::faceCorrectedSnGrad<Type>::correction
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  const fvMesh& mesh = this->mesh();
  // construct GeometricField<Type, fvsPatchField, surfaceMesh>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tssf
  {
    new GeometricField<Type, fvsPatchField, surfaceMesh>
    {
      IOobject
      {
        "snGradCorr(" + vf.name() + ')',
        vf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      vf.dimensions()*mesh.nonOrthDeltaCoeffs().dimensions()
    }
  };
  GeometricField<Type, fvsPatchField, surfaceMesh>& ssf = tssf();
  for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
  {
    ssf.replace
    (
      cmpt,
      faceCorrectedSnGrad<typename pTraits<Type>::cmptType>(mesh)
     .fullGradCorrection(vf.component(cmpt))
    );
  }

  return tssf;
}
