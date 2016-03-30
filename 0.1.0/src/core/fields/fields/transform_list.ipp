// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "transform_list.hpp"


template<class T>
mousse::List<T> mousse::transform
(
  const tensor& rotTensor,
  const UList<T>& field
)
{
  List<T> newField{field.size()};
  FOR_ALL(field, i) {
    newField[i] = transform(rotTensor, field[i]);
  }
  return newField;
}


template<class T>
void mousse::transformList(const tensor& rotTensor, UList<T>& field)
{
  FOR_ALL(field, i) {
    field[i] = transform(rotTensor, field[i]);
  }
}


template<class T>
void mousse::transformList(const tensorField& rotTensor, UList<T>& field)
{
  if (rotTensor.size() == 1) {
    FOR_ALL(field, i) {
      field[i] = transform(rotTensor[0], field[i]);
    }
  } else if (rotTensor.size() == field.size()) {
    FOR_ALL(field, i) {
      field[i] = transform(rotTensor[i], field[i]);
    }
  } else {
    FATAL_ERROR_IN
    (
      "transformList(const tensorField&, UList<T>&)"
    )
    << "Sizes of field and transformation not equal. field:"
    << field.size() << " transformation:" << rotTensor.size()
    << abort(FatalError);
  }
}


template<class T>
void mousse::transformList(const tensor& rotTensor, Map<T>& field)
{
  FOR_ALL_ITER(typename Map<T>, field, iter) {
    iter() = transform(rotTensor[0], iter());
  }
}


template<class T>
void mousse::transformList(const tensorField& rotTensor, Map<T>& field)
{
  if (rotTensor.size() == 1) {
    FOR_ALL_ITER(typename Map<T>, field, iter) {
      iter() = transform(rotTensor[0], iter());
    }
  } else {
    FATAL_ERROR_IN
    (
      "transformList(const tensorField&, Map<T>&)"
    )
    << "Multiple transformation tensors not supported. field:"
    << field.size() << " transformation:" << rotTensor.size()
    << abort(FatalError);
  }
}


template<class T>
void mousse::transformList(const tensor& rotTensor, EdgeMap<T>& field)
{
  FOR_ALL_ITER(typename EdgeMap<T>, field, iter) {
    iter() = transform(rotTensor[0], iter());
  }
}


template<class T>
void mousse::transformList(const tensorField& rotTensor, EdgeMap<T>& field)
{
  if (rotTensor.size() == 1) {
    FOR_ALL_ITER(typename EdgeMap<T>, field, iter) {
      iter() = transform(rotTensor[0], iter());
    }
  } else {
    FATAL_ERROR_IN
    (
      "transformList(const tensorField&, EdgeMap<T>&)"
    )
    << "Multiple transformation tensors not supported. field:"
    << field.size() << " transformation:" << rotTensor.size()
    << abort(FatalError);
  }
}
