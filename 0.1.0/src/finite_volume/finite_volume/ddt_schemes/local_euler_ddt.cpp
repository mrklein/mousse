// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "local_euler_ddt_scheme.hpp"
#include "fv_mesh.hpp"
#include "time.hpp"
#include "vol_fields.hpp"

// Static Data Members
mousse::word mousse::fv::localEulerDdt::rDeltaTName("rDeltaT");
mousse::word mousse::fv::localEulerDdt::rSubDeltaTName("rSubDeltaTName");
bool mousse::fv::localEulerDdt::enabled(const fvMesh& mesh)
{
  return
    word(mesh.ddtScheme("default"))
  == fv::localEulerDdtScheme<scalar>::typeName;
}
const mousse::volScalarField& mousse::fv::localEulerDdt::localRDeltaT
(
  const fvMesh& mesh
)
{
  return mesh.objectRegistry::lookupObject<volScalarField>
  (
    mesh.time().subCycling() ? rSubDeltaTName : rDeltaTName
  );
}
mousse::tmp<mousse::volScalarField> mousse::fv::localEulerDdt::localRSubDeltaT
(
  const fvMesh& mesh,
  const label nAlphaSubCycles
)
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      rSubDeltaTName,
      nAlphaSubCycles
     *mesh.objectRegistry::lookupObject<volScalarField>
      (
        rDeltaTName
      )
    )
  );
}
