// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class KeyType, class DataType>
void mousse::PrintTable<KeyType, DataType>::add
(
  const KeyType& K,
  const DataType& D
)
{
  table_.set(K, D);
}
