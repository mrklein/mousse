// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_surface.hpp"
template<class Type>
bool mousse::sampledSurface::checkFieldSize(const Field<Type>& field) const
{
  if (faces().empty() || field.empty())
  {
    return false;
  }
  if (field.size() != faces().size())
  {
    FatalErrorIn
    (
      "sampledSurface::checkFieldSize(const Field<Type>&) const"
    )
      << "size mismatch: "
      << "field (" << field.size()
      << ") != surface (" << faces().size() << ")"
      << exit(FatalError);
  }
  return true;
}
template<class Type>
Type mousse::sampledSurface::integrate(const Field<Type>& field) const
{
  Type value = pTraits<Type>::zero;
  if (checkFieldSize(field))
  {
    value = sum(field*magSf());
  }
  reduce(value, sumOp<Type>());
  return value;
}
template<class Type>
Type mousse::sampledSurface::integrate(const tmp<Field<Type> >& field) const
{
  Type value = integrate(field());
  field.clear();
  return value;
}
template<class Type>
Type mousse::sampledSurface::average(const Field<Type>& field) const
{
  Type value = pTraits<Type>::zero;
  if (checkFieldSize(field))
  {
    value = sum(field*magSf());
  }
  reduce(value, sumOp<Type>());
  // avoid divide-by-zero
  if (area())
  {
    return value/area();
  }
  else
  {
    return pTraits<Type>::zero;
  }
}
template<class Type>
Type mousse::sampledSurface::average(const tmp<Field<Type> >& field) const
{
  Type value = average(field());
  field.clear();
  return value;
}
template<class ReturnType, class Type>
void mousse::sampledSurface::project
(
  Field<ReturnType>& res,
  const Field<Type>& field
) const
{
  if (checkFieldSize(field))
  {
    const vectorField& norm = Sf();
    forAll(norm, faceI)
    {
      res[faceI] = field[faceI] & (norm[faceI]/mag(norm[faceI]));
    }
  }
  else
  {
    res.clear();
  }
}
template<class ReturnType, class Type>
void mousse::sampledSurface::project
(
  Field<ReturnType>& res,
  const tmp<Field<Type> >& field
) const
{
  project(res, field());
  field.clear();
}
template<class ReturnType, class Type>
mousse::tmp<mousse::Field<ReturnType> >
mousse::sampledSurface::project
(
  const tmp<Field<Type> >& field
) const
{
  tmp<Field<ReturnType> > tRes(new Field<ReturnType>(faces().size()));
  project(tRes(), field);
  return tRes;
}
template<class Type>
mousse::tmp<mousse::GeometricField<Type, mousse::fvPatchField, mousse::volMesh> >
mousse::sampledSurface::pointAverage
(
  const GeometricField<Type, pointPatchField, pointMesh>& pfld
) const
{
  const fvMesh& mesh = dynamic_cast<const fvMesh&>(pfld.mesh()());
  tmp<GeometricField<Type, fvPatchField, volMesh> > tcellAvg
  (
    new GeometricField<Type, fvPatchField, volMesh>
    (
      IOobject
      (
        "cellAvg",
        mesh.time().timeName(),
        pfld.db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      mesh,
      dimensioned<Type>("zero", dimless, pTraits<Type>::zero)
    )
  );
  GeometricField<Type, fvPatchField, volMesh>& cellAvg = tcellAvg();
  labelField nPointCells(mesh.nCells(), 0);
  {
    for (label pointI = 0; pointI < mesh.nPoints(); pointI++)
    {
      const labelList& pCells = mesh.pointCells(pointI);
      forAll(pCells, i)
      {
        label cellI = pCells[i];
        cellAvg[cellI] += pfld[pointI];
        nPointCells[cellI]++;
      }
    }
  }
  forAll(cellAvg, cellI)
  {
    cellAvg[cellI] /= nPointCells[cellI];
  }
  // Give value to calculatedFvPatchFields
  cellAvg.correctBoundaryConditions();
  return tcellAvg;
}
