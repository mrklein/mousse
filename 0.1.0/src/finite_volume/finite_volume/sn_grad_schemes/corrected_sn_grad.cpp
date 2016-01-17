// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "corrected_sn_grad.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "linear.hpp"
#include "fvc_grad.hpp"
#include "gauss_grad.hpp"

// Destructor 
template<class Type>
mousse::fv::correctedSnGrad<Type>::~correctedSnGrad()
{}


// Member Functions 
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::fv::correctedSnGrad<Type>::fullGradCorrection
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  const fvMesh& mesh = this->mesh();
  // construct GeometricField<Type, fvsPatchField, surfaceMesh>
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tssf =
    mesh.nonOrthCorrectionVectors()
   & linear<typename outerProduct<vector, Type>::type>(mesh).interpolate
    (
      gradScheme<Type>::New
      (
        mesh,
        mesh.gradScheme("grad(" + vf.name() + ')')
      )().grad(vf, "grad(" + vf.name() + ')')
    );
  tssf().rename("snGradCorr(" + vf.name() + ')');
  return tssf;
}


template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvsPatchField, mousse::surfaceMesh> >
mousse::fv::correctedSnGrad<Type>::correction
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
        "snGradCorr("+vf.name()+')',
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
      correctedSnGrad<typename pTraits<Type>::cmptType>(mesh)
     .fullGradCorrection(vf.component(cmpt))
    );
  }
  return tssf;
}
