// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "injection_model_list.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Constructors 
injectionModelList::injectionModelList(surfaceFilmModel& owner)
:
  PtrList<injectionModel>{},
  filmSubModelBase{owner}
{}


injectionModelList::injectionModelList
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  PtrList<injectionModel>{},
  filmSubModelBase
  {
    "injectionModelList",
    owner,
    dict,
    "injectionModelList",
    "injectionModelList"
  },
  massInjected_{owner.intCoupledPatchIDs().size(), 0.0}
{
  const wordList activeModels{dict.lookup("injectionModels")};
  wordHashSet models;
  FOR_ALL(activeModels, i) {
    models.insert(activeModels[i]);
  }
  Info << "    Selecting film injection models" << endl;
  if (models.size() > 0) {
    this->setSize(models.size());
    label i = 0;
    FOR_ALL_CONST_ITER(wordHashSet, models, iter) {
      const word& model = iter.key();
      set(i, injectionModel::New(owner, dict, model));
      i++;
    }
  } else {
    Info << "        none" << endl;
  }
}


// Destructor 
injectionModelList::~injectionModelList()
{}


// Member Functions 
void injectionModelList::correct
(
  scalarField& availableMass,
  volScalarField& massToInject,
  volScalarField& diameterToInject
)
{
  // Correct models that accumulate mass and diameter transfers
  FOR_ALL(*this, i) {
    injectionModel& im = operator[](i);
    im.correct(availableMass, massToInject, diameterToInject);
  }
  // Push values to boundaries ready for transfer to the primary region
  massToInject.correctBoundaryConditions();
  diameterToInject.correctBoundaryConditions();
  const labelList& patchIDs = owner().intCoupledPatchIDs();
  FOR_ALL(patchIDs, i) {
    label patchi = patchIDs[i];
    massInjected_[i] =
      massInjected_[i] + sum(massToInject.boundaryField()[patchi]);
  }
}


void injectionModelList::info(Ostream& os)
{
  const polyBoundaryMesh& pbm = owner().regionMesh().boundaryMesh();
  scalar injectedMass = 0;
  scalarField patchInjectedMasses{pbm.size(), 0};
  FOR_ALL(*this, i) {
    const injectionModel& im = operator[](i);
    injectedMass += im.injectedMassTotal();
    im.patchInjectedMassTotals(patchInjectedMasses);
  }
  os << indent << "injected mass      = " << injectedMass << nl;
  FOR_ALL(pbm, patchi) {
    if (mag(patchInjectedMasses[patchi]) > VSMALL) {
      os << indent << indent << "from patch " << pbm[patchi].name()
        << " = " << patchInjectedMasses[patchi] << nl;
    }
  }
  scalarField mass0{massInjected_.size(), 0.0};
  this->getBaseProperty("massInjected", mass0);
  scalarField mass{massInjected_};
  Pstream::listCombineGather(mass, plusEqOp<scalar>());
  mass += mass0;
  const labelList& patchIDs = owner().intCoupledPatchIDs();
  FOR_ALL(patchIDs, i) {
    label patchi = patchIDs[i];
    Info << indent << "  - patch: " << pbm[patchi].name() << ": "
      << mass[i] << endl;
  }
  if (owner().time().outputTime()) {
    setBaseProperty("massInjected", mass);
    massInjected_ = 0.0;
  }
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

