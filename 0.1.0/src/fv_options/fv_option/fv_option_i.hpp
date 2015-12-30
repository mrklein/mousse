// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
inline const mousse::word& mousse::fv::option::name() const
{
  return name_;
}
inline const mousse::fvMesh& mousse::fv::option::mesh() const
{
  return mesh_;
}
inline const mousse::dictionary& mousse::fv::option::coeffs() const
{
  return coeffs_;
}
inline bool mousse::fv::option::active() const
{
  return active_;
}
inline void mousse::fv::option::setApplied(const label fieldI)
{
  applied_[fieldI] = true;
}
inline mousse::Switch& mousse::fv::option::active()
{
  return active_;
}
