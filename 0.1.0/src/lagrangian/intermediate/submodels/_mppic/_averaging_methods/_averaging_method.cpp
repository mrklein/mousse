// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_averaging_method.hpp"
#include "run_time_selection_tables.hpp"
#include "point_mesh.hpp"
// Constructors 
template<class Type>
mousse::AveragingMethod<Type>::AveragingMethod
(
  const IOobject& io,
  const dictionary& dict,
  const fvMesh& mesh,
  const labelList& size
)
:
  regIOobject(io),
  FieldField<Field, Type>(),
  dict_(dict),
  mesh_(mesh)
{
  forAll(size, i)
  {
    FieldField<Field, Type>::append
    (
      new Field<Type>(size[i], pTraits<Type>::zero)
    );
  }
}
template<class Type>
mousse::AveragingMethod<Type>::AveragingMethod
(
  const AveragingMethod<Type>& am
)
:
  regIOobject(am),
  FieldField<Field, Type>(am),
  dict_(am.dict_),
  mesh_(am.mesh_)
{}
// Selectors
template<class Type>
mousse::autoPtr<mousse::AveragingMethod<Type> >
mousse::AveragingMethod<Type>::New
(
  const IOobject& io,
  const dictionary& dict,
  const fvMesh& mesh
)
{
  word averageType(dict.lookup(typeName));
  //Info<< "Selecting averaging method "
  //    << averageType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(averageType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "mousse::AveragingMethod<Type>::New"
      "("
        "const IOobject&, "
        "const dictionary&, "
        "const fvMesh&"
      ")"
    )   << "Unknown averaging method " << averageType
      << ", constructor not in hash table" << nl << nl
      << "    Valid averaging methods are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<AveragingMethod<Type> >(cstrIter()(io, dict, mesh));
}
// Destructor 
template<class Type>
mousse::AveragingMethod<Type>::~AveragingMethod()
{}
// Private Member Functions 
template<class Type>
void mousse::AveragingMethod<Type>::updateGrad()
{
  // do nothing
}
// Member Functions 
template<class Type>
void mousse::AveragingMethod<Type>::average()
{
  updateGrad();
}
template<class Type>
void mousse::AveragingMethod<Type>::average
(
  const AveragingMethod<scalar>& weight
)
{
  updateGrad();
  *this /= max(weight, SMALL);
}
template<class Type>
bool mousse::AveragingMethod<Type>::writeData(Ostream& os) const
{
  return os.good();
}
template<class Type>
bool mousse::AveragingMethod<Type>::write() const
{
  const pointMesh pointMesh_(mesh_);
  // point volumes
  Field<scalar> pointVolume(mesh_.nPoints(), 0);
  // output fields
  GeometricField<Type, fvPatchField, volMesh> cellValue
  (
    IOobject
    (
      this->name() + ":cellValue",
      this->time().timeName(),
      mesh_
    ),
    mesh_,
    dimensioned<Type>("zero", dimless, pTraits<Type>::zero)
  );
  GeometricField<TypeGrad, fvPatchField, volMesh> cellGrad
  (
    IOobject
    (
      this->name() + ":cellGrad",
      this->time().timeName(),
      mesh_
    ),
    mesh_,
    dimensioned<TypeGrad>("zero", dimless, pTraits<TypeGrad>::zero)
  );
  GeometricField<Type, pointPatchField, pointMesh> pointValue
  (
    IOobject
    (
      this->name() + ":pointValue",
      this->time().timeName(),
      mesh_
    ),
    pointMesh_,
    dimensioned<Type>("zero", dimless, pTraits<Type>::zero)
  );
  GeometricField<TypeGrad, pointPatchField, pointMesh> pointGrad
  (
    IOobject
    (
      this->name() + ":pointGrad",
      this->time().timeName(),
      mesh_
    ),
    pointMesh_,
    dimensioned<TypeGrad>("zero", dimless, pTraits<TypeGrad>::zero)
  );
  // tet-volume weighted sums
  forAll(mesh_.C(), cellI)
  {
    const List<tetIndices> cellTets =
      polyMeshTetDecomposition::cellTetIndices(mesh_, cellI);
    forAll(cellTets, tetI)
    {
      const tetIndices& tetIs = cellTets[tetI];
      const scalar v = tetIs.tet(mesh_).mag();
      cellValue[cellI] += v*interpolate(mesh_.C()[cellI], tetIs);
      cellGrad[cellI] += v*interpolateGrad(mesh_.C()[cellI], tetIs);
      const face& f = mesh_.faces()[tetIs.face()];
      labelList vertices(3);
      vertices[0] = f[tetIs.faceBasePt()];
      vertices[1] = f[tetIs.facePtA()];
      vertices[2] = f[tetIs.facePtB()];
      forAll(vertices, vertexI)
      {
        const label pointI = vertices[vertexI];
        pointVolume[pointI] += v;
        pointValue[pointI] +=
          v*interpolate(mesh_.points()[pointI], tetIs);
        pointGrad[pointI] +=
          v*interpolateGrad(mesh_.points()[pointI], tetIs);
      }
    }
  }
  // average
  cellValue.internalField() /= mesh_.V();
  cellGrad.internalField() /= mesh_.V();
  pointValue.internalField() /= pointVolume;
  pointGrad.internalField() /= pointVolume;
  // write
  if (!cellValue.write()) return false;
  if (!cellGrad.write()) return false;
  if (!pointValue.write()) return false;
  if (!pointGrad.write()) return false;
  return true;
}
