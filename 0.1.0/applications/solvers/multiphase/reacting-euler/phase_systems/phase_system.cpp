// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_system.hpp"
#include "surface_tension_model.hpp"
#include "aspect_ratio_model.hpp"
#include "surface_interpolate.hpp"
#include "fvc_ddt.hpp"
// Static Data Members
namespace mousse {
DEFINE_TYPE_NAME_AND_DEBUG(phaseSystem, 0);
}
const mousse::word mousse::phaseSystem::propertiesName{"phaseProperties"};
// Protected Member Functions 
mousse::tmp<mousse::surfaceScalarField> mousse::phaseSystem::calcPhi
(
  const phaseModelList& phaseModels
) const
{
  tmp<surfaceScalarField> tmpPhi
  {
    new surfaceScalarField
    {
      "phi",
      fvc::interpolate(phaseModels[0])*phaseModels[0].phi()
    }
  };
  for (label phasei=1; phasei<phaseModels.size(); phasei++)
  {
    tmpPhi() +=
      fvc::interpolate(phaseModels[phasei])*phaseModels[phasei].phi();
  }
  return tmpPhi;
}
void mousse::phaseSystem::generatePairs
(
  const dictTable& modelDicts
)
{
  FOR_ALL_CONST_ITER(dictTable, modelDicts, iter)
  {
    const phasePairKey& key = iter.key();
    // pair already exists
    if (phasePairs_.found(key))
    {
      // do nothing ...
    }
    // new ordered pair
    else if (key.ordered())
    {
      phasePairs_.insert
      (
        key,
        autoPtr<phasePair>
        {
          new orderedPhasePair
          {
            phaseModels_[key.first()],
            phaseModels_[key.second()]
          }
        }
      );
    }
    // new unordered pair
    else
    {
      phasePairs_.insert
      (
        key,
        autoPtr<phasePair>
        {
          new phasePair
          {
            phaseModels_[key.first()],
            phaseModels_[key.second()]
          }
        }
      );
    }
  }
}
// Constructors 
mousse::phaseSystem::phaseSystem
(
  const fvMesh& mesh
)
:
  IOdictionary
  {
    {
      "phaseProperties",
      mesh.time().constant(),
      mesh,
      IOobject::MUST_READ_IF_MODIFIED
    }
  },
  mesh_{mesh},
  phaseModels_{lookup("phases"), phaseModel::iNew(*this)},
  phi_{calcPhi(phaseModels_)},
  dpdt_
  {
    {
      "dpdt",
      mesh.time().timeName(),
      mesh
    },
    mesh,
    {"dpdt", dimPressure/dimTime, 0}
  },
  MRF_{mesh_},
  fvOptions_{mesh_}
{
  phi_.writeOpt() = IOobject::AUTO_WRITE;
  // Blending methods
  FOR_ALL_CONST_ITER(dictionary, subDict("blending"), iter)
  {
    blendingMethods_.insert
    (
      iter().dict().dictName(),
      blendingMethod::New
      (
        iter().dict(),
        phaseModels_.toc()
      )
    );
  }
  // Sub-models
  generatePairsAndSubModels("surfaceTension", surfaceTensionModels_);
  generatePairsAndSubModels("aspectRatio", aspectRatioModels_);
  correctKinematics();
}
// Destructor 
mousse::phaseSystem::~phaseSystem()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::phaseSystem::rho() const
{
  tmp<volScalarField> tmpRho{phaseModels_[0]*phaseModels_[0].rho()};
  for (label phasei=1; phasei<phaseModels_.size(); phasei++)
  {
    tmpRho() += phaseModels_[phasei]*phaseModels_[phasei].rho();
  }
  return tmpRho;
}
mousse::tmp<mousse::volVectorField> mousse::phaseSystem::U() const
{
  tmp<volVectorField> tmpU{phaseModels_[0]*phaseModels_[0].U()};
  for (label phasei=1; phasei<phaseModels_.size(); phasei++)
  {
    tmpU() += phaseModels_[phasei]*phaseModels_[phasei].U();
  }
  return tmpU;
}
mousse::tmp<mousse::volScalarField>
mousse::phaseSystem::sigma(const phasePairKey& key) const
{
  if (surfaceTensionModels_.found(key))
  {
    return surfaceTensionModels_[key]->sigma();
  }
  else
  {
    return tmp<volScalarField>
    {
      new volScalarField
      {
        {
          surfaceTensionModel::typeName + ":sigma",
          this->mesh_.time().timeName(),
          this->mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        this->mesh_,
        {"zero", surfaceTensionModel::dimSigma, 0}
      }
    };
  }
}
void mousse::phaseSystem::solve()
{}
void mousse::phaseSystem::correct()
{
  FOR_ALL(phaseModels_, phasei)
  {
    phaseModels_[phasei].correct();
  }
}
void mousse::phaseSystem::correctKinematics()
{
  bool updateDpdt = false;
  FOR_ALL(phaseModels_, phasei)
  {
    phaseModels_[phasei].correctKinematics();
    updateDpdt = updateDpdt || phaseModels_[phasei].thermo().dpdt();
  }
  // Update the pressure time-derivative if required
  if (updateDpdt)
  {
    dpdt_ = fvc::ddt(phaseModels_.begin()().thermo().p());
  }
}
void mousse::phaseSystem::correctThermo()
{
  FOR_ALL(phaseModels_, phasei)
  {
    phaseModels_[phasei].correctThermo();
  }
}
void mousse::phaseSystem::correctTurbulence()
{
  FOR_ALL(phaseModels_, phasei)
  {
    phaseModels_[phasei].correctTurbulence();
  }
}
void mousse::phaseSystem::correctEnergyTransport()
{
  FOR_ALL(phaseModels_, phasei)
  {
    phaseModels_[phasei].correctEnergyTransport();
  }
}
bool mousse::phaseSystem::read()
{
  if (regIOobject::read())
  {
    bool readOK = true;
    FOR_ALL(phaseModels_, phasei)
    {
      readOK &= phaseModels_[phasei].read();
    }
    // models ...
    return readOK;
  }
  else
  {
    return false;
  }
}
