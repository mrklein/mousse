// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Private Member Functions 
inline mousse::label mousse::block::vtxLabel(label i, label j, label k) const
{
  return
  (
    i
   + j * (meshDensity().x() + 1)
   + k * (meshDensity().x() + 1) * (meshDensity().y() + 1)
  );
}
// Member Functions 
inline const mousse::blockDescriptor& mousse::block::blockDef() const
{
  return *this;
}
