// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field_value.hpp"
#include "fv_mesh.hpp"
// Member Functions 
inline const mousse::word& mousse::fieldValue::name() const
{
  return name_;
}
inline const mousse::objectRegistry& mousse::fieldValue::obr() const
{
  return obr_;
}
inline const mousse::dictionary& mousse::fieldValue::dict() const
{
  return dict_;
}
inline bool mousse::fieldValue::active() const
{
  return active_;
}
inline const mousse::Switch& mousse::fieldValue::log() const
{
  return log_;
}
inline const mousse::word& mousse::fieldValue::sourceName() const
{
  return sourceName_;
}
inline const mousse::wordList& mousse::fieldValue::fields() const
{
  return fields_;
}
inline const mousse::Switch& mousse::fieldValue::valueOutput() const
{
  return valueOutput_;
}
inline const mousse::fvMesh& mousse::fieldValue::mesh() const
{
  return refCast<const fvMesh>(obr_);
}
inline const mousse::dictionary& mousse::fieldValue::resultDict() const
{
  return resultDict_;
}
