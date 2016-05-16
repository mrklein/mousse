// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_moment.hpp"


// Constructors 
template<class Type>
mousse::AveragingMethods::Moment<Type>::Moment
(
  const IOobject& io,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  AveragingMethod<Type>{io, dict, mesh, labelList(4, mesh.nCells())},
  data_{FieldField<Field, Type>::operator[](0)},
  dataX_{FieldField<Field, Type>::operator[](1)},
  dataY_{FieldField<Field, Type>::operator[](2)},
  dataZ_{FieldField<Field, Type>::operator[](3)},
  transform_{mesh.nCells(), symmTensor::zero},
  scale_{0.5*pow(mesh.V(), 1.0/3.0)}
{
  scalar a = 1.0/24.0;
  scalar b = 0.5854101966249685;
  scalar c = 0.1381966011250105;
  scalarField wQ{4};
  wQ[0] = a;
  wQ[1] = a;
  wQ[2] = a;
  wQ[3] = a;
  vectorField xQ{4};
  xQ[0] = vector(b, c, c);
  xQ[1] = vector(c, b, c);
  xQ[2] = vector(c, c, b);
  xQ[3] = vector(c, c, c);
  FOR_ALL(mesh.C(), cellI) {
    const List<tetIndices> cellTets =
      polyMeshTetDecomposition::cellTetIndices(mesh, cellI);
    symmTensor A{symmTensor::zero};
    FOR_ALL(cellTets, tetI) {
      const tetIndices& tetIs = cellTets[tetI];
      const label faceI = tetIs.face();
      const face& f = mesh.faces()[faceI];
      const tensor T
      {
        tensor
        {
          mesh.points()[f[tetIs.faceBasePt()]] - mesh.C()[cellI],
          mesh.points()[f[tetIs.facePtA()]] - mesh.C()[cellI],
          mesh.points()[f[tetIs.facePtB()]] - mesh.C()[cellI]
        }.T()
      };
      const vectorField d{(T & xQ)/scale_[cellI]};
      const scalar v{6.0*tetIs.tet(mesh).mag()/mesh.V()[cellI]};
      A += v*sum(wQ*sqr(d));
    }
    transform_[cellI] = inv(A);
  }
}


template<class Type>
mousse::AveragingMethods::Moment<Type>::Moment
(
  const Moment<Type>& am
)
:
  AveragingMethod<Type>{am},
  data_{FieldField<Field, Type>::operator[](0)},
  dataX_{FieldField<Field, Type>::operator[](1)},
  dataY_{FieldField<Field, Type>::operator[](2)},
  dataZ_{FieldField<Field, Type>::operator[](3)},
  transform_{am.transform_}
{}


// Destructor 
template<class Type>
mousse::AveragingMethods::Moment<Type>::~Moment()
{}


// Private Member Functions 
template<class Type>
void mousse::AveragingMethods::Moment<Type>::updateGrad()
{}


// Member Functions 
template<class Type>
void mousse::AveragingMethods::Moment<Type>::add
(
  const point position,
  const tetIndices& tetIs,
  const Type& value
)
{
  const label cellI = tetIs.cell();
  const Type v = value/this->mesh_.V()[cellI];
  const TypeGrad dv =
    transform_[cellI] & (v*(position - this->mesh_.C()[cellI])/scale_[cellI]);
  data_[cellI] += v;
  dataX_[cellI] += v + dv.x();
  dataY_[cellI] += v + dv.y();
  dataZ_[cellI] += v + dv.z();
}


template<class Type>
Type mousse::AveragingMethods::Moment<Type>::interpolate
(
  const point position,
  const tetIndices& tetIs
) const
{
  const label cellI = tetIs.cell();
  return
    data_[cellI]
    + (TypeGrad
       (
         dataX_[cellI] - data_[cellI],
         dataY_[cellI] - data_[cellI],
         dataZ_[cellI] - data_[cellI]
       ) & (position - this->mesh_.C()[cellI])/scale_[cellI]);
}


template<class Type>
typename mousse::AveragingMethods::Moment<Type>::TypeGrad
mousse::AveragingMethods::Moment<Type>::interpolateGrad
(
  const point /*position*/,
  const tetIndices& tetIs
) const
{
  const label cellI{tetIs.cell()};
  return
    TypeGrad
    {
      dataX_[cellI] - data_[cellI],
      dataY_[cellI] - data_[cellI],
      dataZ_[cellI] - data_[cellI]
    }/scale_[cellI];
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::AveragingMethods::Moment<Type>::internalField() const
{
  return tmp<Field<Type>>{data_};
}

