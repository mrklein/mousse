// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_injection.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(patchInjection, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(injectionModel, patchInjection, dictionary);


// Constructors 
patchInjection::patchInjection
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  injectionModel{type(), owner, dict},
  deltaStable_{coeffDict_.lookupOrDefault<scalar>("deltaStable", 0.0)}
{
  const polyBoundaryMesh& pbm = owner.regionMesh().boundaryMesh();
  patchIDs_.setSize(pbm.size());
  if (coeffDict_.found("patches")) {
    wordReList patchNames{coeffDict_.lookup("patches")};
    const labelHashSet patchSet = pbm.patchSet(patchNames);
    Info << "        applying to patches:" << nl;
    label pidi = 0;
    FOR_ALL_CONST_ITER(labelHashSet, patchSet, iter) {
      label patchi = iter.key();
      patchIDs_[pidi++] = patchi;
      Info << "            " << pbm[patchi].name() << endl;
    }
    patchIDs_.setSize(pidi);
    patchInjectedMasses_.setSize(pidi, 0);
  } else {
    Info << "            applying to all patches" << endl;
    FOR_ALL(patchIDs_, patchi) {
      patchIDs_[patchi] = patchi;
    }
    patchInjectedMasses_.setSize(pbm.size(), 0);
  }
  if (!patchIDs_.size()) {
    FATAL_ERROR_IN("patchInjection::patchInjection")
      << "No patches selected"
      << exit(FatalError);
  }
}


// Destructor 
patchInjection::~patchInjection()
{}


// Member Functions 
void patchInjection::correct
(
  scalarField& availableMass,
  scalarField& massToInject,
  scalarField& /*diameterToInject*/
)
{
  // Do not correct if no patches selected
  if (!patchIDs_.size())
    return;
  const scalarField& delta = owner().delta();
  const scalarField& rho = owner().rho();
  const scalarField& magSf = owner().magSf();
  const polyBoundaryMesh& pbm = owner().regionMesh().boundaryMesh();
  FOR_ALL(patchIDs_, pidi) {
    label patchi = patchIDs_[pidi];
    const polyPatch& pp = pbm[patchi];
    const labelList& faceCells = pp.faceCells();
    // Accumulate the total mass removed from patch
    scalar dMassPatch = 0;
    FOR_ALL(faceCells, fci) {
      label celli = faceCells[fci];
      scalar ddelta = max(0.0, delta[celli] - deltaStable_);
      scalar dMass = ddelta*rho[celli]*magSf[celli];
      massToInject[celli] += dMass;
      availableMass[celli] -= dMass;
      dMassPatch += dMass;
    }
    patchInjectedMasses_[pidi] += dMassPatch;
    addToInjectedMass(dMassPatch);
  }
  injectionModel::correct();
  if (outputTime()) {
    scalarField patchInjectedMasses0
    {
      getModelProperty<scalarField>
      (
        "patchInjectedMasses",
        scalarField(patchInjectedMasses_.size(), 0)
      )
    };
    scalarField patchInjectedMassTotals{patchInjectedMasses_};
    Pstream::listCombineGather(patchInjectedMassTotals, plusEqOp<scalar>());
    patchInjectedMasses0 += patchInjectedMassTotals;
    setModelProperty<scalarField>
    (
      "patchInjectedMasses",
      patchInjectedMasses0
    );
    patchInjectedMasses_ = 0;
  }
}


void patchInjection::patchInjectedMassTotals(scalarField& patchMasses) const
{
  // Do not correct if no patches selected
  if (!patchIDs_.size())
    return;
  scalarField patchInjectedMasses
  {
    getModelProperty<scalarField>
    (
      "patchInjectedMasses",
      scalarField(patchInjectedMasses_.size(), 0)
    )
  };
  scalarField patchInjectedMassTotals{patchInjectedMasses_};
  Pstream::listCombineGather(patchInjectedMassTotals, plusEqOp<scalar>());
  FOR_ALL(patchIDs_, pidi) {
    label patchi = patchIDs_[pidi];
    patchMasses[patchi] +=
      patchInjectedMasses[pidi] + patchInjectedMassTotals[pidi];
  }
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

