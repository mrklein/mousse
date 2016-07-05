// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_basic.hpp"


// Constructors 
template<class Type>
mousse::AveragingMethods::Basic<Type>::Basic
(
  const IOobject& io,
  const dictionary& dict,
  const fvMesh& mesh
)
:
  AveragingMethod<Type>{io, dict, mesh, labelList(1, mesh.nCells())},
  data_{FieldField<Field, Type>::operator[](0)},
  dataGrad_{mesh.nCells()}
{}


template<class Type>
mousse::AveragingMethods::Basic<Type>::Basic
(
  const Basic<Type>& am
)
:
  AveragingMethod<Type>{am},
  data_{FieldField<Field, Type>::operator[](0)},
  dataGrad_{am.dataGrad_}
{}


// Destructor 
template<class Type>
mousse::AveragingMethods::Basic<Type>::~Basic()
{}


// Private Member Functions 
template<class Type>
void mousse::AveragingMethods::Basic<Type>::updateGrad()
{
  GeometricField<Type, fvPatchField, volMesh> tempData
  {
    {
      "BasicAverage::Data",
      this->mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    this->mesh_,
    {"zero", dimless, pTraits<Type>::zero},
    zeroGradientFvPatchField<Type>::typeName
  };
  tempData.internalField() = data_;
  tempData.correctBoundaryConditions();
  dataGrad_ = fvc::grad(tempData)->internalField();
}


// Member Functions 
template<class Type>
void mousse::AveragingMethods::Basic<Type>::add
(
  const point /*position*/,
  const tetIndices& tetIs,
  const Type& value
)
{
  data_[tetIs.cell()] += value/this->mesh_.V()[tetIs.cell()];
}


template<class Type>
Type mousse::AveragingMethods::Basic<Type>::interpolate
(
  const point /*position*/,
  const tetIndices& tetIs
) const
{
  return data_[tetIs.cell()];
}


template<class Type>
typename mousse::AveragingMethods::Basic<Type>::TypeGrad
mousse::AveragingMethods::Basic<Type>::interpolateGrad
(
  const point /*position*/,
  const tetIndices& tetIs
) const
{
  return dataGrad_[tetIs.cell()];
}


template<class Type>
mousse::tmp<mousse::Field<Type> >
mousse::AveragingMethods::Basic<Type>::internalField() const
{
  return tmp<Field<Type>>{data_};
}

