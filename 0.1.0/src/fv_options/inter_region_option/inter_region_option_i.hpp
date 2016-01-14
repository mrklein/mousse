// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

inline const mousse::word&
mousse::fv::interRegionOption::nbrRegionName() const
{
  return nbrRegionName_;
}
inline const mousse::meshToMesh&
mousse::fv::interRegionOption::meshInterp() const
{
  if (!meshInterpPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "const meshToMesh& interRegionOption::meshInterp() const"
    )   << "Interpolation object not set"
      << abort(FatalError);
  }
  return meshInterpPtr_();
}
