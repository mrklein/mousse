// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::regionModels::regionModel&
mousse::regionModels::regionModelFunctionObjectList::owner() const
{
  return owner_;
}
inline mousse::regionModels::regionModel&
mousse::regionModels::regionModelFunctionObjectList::owner()
{
  return owner_;
}
inline const mousse::dictionary&
mousse::regionModels::regionModelFunctionObjectList::dict() const
{
  return dict_;
}
