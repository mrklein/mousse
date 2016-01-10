// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type, class DType, class LUType>
template<class T>
inline void mousse::LduMatrix<Type, DType, LUType>::solver::readControl
(
  const dictionary& controlDict,
  T& control,
  const word& controlName
)
{
  if (controlDict.found(controlName))
  {
    controlDict.lookup(controlName) >> control;
  }
}
