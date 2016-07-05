// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrix.hpp"


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::Su
(
  const DimensionedField<Type, volMesh>& su,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = vf.mesh();
  tmp<fvMatrix<Type>> tfvm
  {
    new fvMatrix<Type>
    {
      vf,
      dimVol*su.dimensions()
    }
  };
  fvMatrix<Type>& fvm = tfvm();
  fvm.source() -= mesh.V()*su.field();

  return tfvm;
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::Su
(
  const tmp<DimensionedField<Type, volMesh>>& tsu,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type>> tfvm = fvm::Su(tsu(), vf);
  tsu.clear();
  return tfvm;
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::Su
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>& tsu,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type>> tfvm = fvm::Su(tsu(), vf);
  tsu.clear();
  return tfvm;
}


template<class Type>
mousse::zeroField
mousse::fvm::Su
(
  const zero&,
  const GeometricField<Type, fvPatchField, volMesh>& /*vf*/
)
{
  return zeroField();
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::Sp
(
  const DimensionedField<scalar, volMesh>& sp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = vf.mesh();
  tmp<fvMatrix<Type>> tfvm
  {
    new fvMatrix<Type>
    {
      vf,
      dimVol*sp.dimensions()*vf.dimensions()
    }
  };
  fvMatrix<Type>& fvm = tfvm();
  fvm.diag() += mesh.V()*sp.field();

  return tfvm;
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::Sp
(
  const tmp<DimensionedField<scalar, volMesh>>& tsp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type>> tfvm = fvm::Sp(tsp(), vf);
  tsp.clear();
  return tfvm;
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::Sp
(
  const tmp<volScalarField>& tsp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type>> tfvm = fvm::Sp(tsp(), vf);
  tsp.clear();
  return tfvm;
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::Sp
(
  const dimensionedScalar& sp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = vf.mesh();
  tmp<fvMatrix<Type>> tfvm
  {
    new fvMatrix<Type>
    {
      vf,
      dimVol*sp.dimensions()*vf.dimensions()
    }
  };
  fvMatrix<Type>& fvm = tfvm();
  fvm.diag() += mesh.V()*sp.value();
  return tfvm;
}


template<class Type>
mousse::zeroField
mousse::fvm::Sp
(
  const zero&,
  const GeometricField<Type, fvPatchField, volMesh>&
)
{
  return zeroField();
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::SuSp
(
  const DimensionedField<scalar, volMesh>& susp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = vf.mesh();
  tmp<fvMatrix<Type>> tfvm
  {
    new fvMatrix<Type>
    {
      vf,
      dimVol*susp.dimensions()*vf.dimensions()
    }
  };
  fvMatrix<Type>& fvm = tfvm();
  fvm.diag() += mesh.V()*max(susp.field(), scalar(0));
  fvm.source() -= mesh.V()*min(susp.field(), scalar(0))
    *vf.internalField();

  return tfvm;
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::SuSp
(
  const tmp<DimensionedField<scalar, volMesh>>& tsusp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type>> tfvm = fvm::SuSp(tsusp(), vf);
  tsusp.clear();
  return tfvm;
}


template<class Type>
mousse::tmp<mousse::fvMatrix<Type>>
mousse::fvm::SuSp
(
  const tmp<volScalarField>& tsusp,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type>> tfvm = fvm::SuSp(tsusp(), vf);
  tsusp.clear();
  return tfvm;
}


template<class Type>
mousse::zeroField
mousse::fvm::SuSp
(
  const zero&,
  const GeometricField<Type, fvPatchField, volMesh>& /*vf*/
)
{
  return zeroField();
}
