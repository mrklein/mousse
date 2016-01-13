// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermal_baffle.hpp"
#include "fvm.hpp"
#include "fvc_div.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "fv_matrices.hpp"
#include "absorption_emission_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace thermalBaffleModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(thermalBaffle, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermalBaffleModel, thermalBaffle, mesh);
ADD_TO_RUN_TIME_SELECTION_TABLE(thermalBaffleModel, thermalBaffle, dictionary);
// Protected Member Functions 
bool thermalBaffle::read()
{
  this->solution().lookup("nNonOrthCorr") >> nNonOrthCorr_;
  return regionModel1D::read();
}
bool thermalBaffle::read(const dictionary& dict)
{
  this->solution().lookup("nNonOrthCorr") >> nNonOrthCorr_;
  return regionModel1D::read(dict);
}
void thermalBaffle::solveEnergy()
{
  if (debug)
  {
    Info<< "thermalBaffle::solveEnergy()" << endl;
  }
  const polyBoundaryMesh& rbm = regionMesh().boundaryMesh();
  tmp<volScalarField> tQ
  (
    new volScalarField
    (
      IOobject
      (
        "tQ",
        regionMesh().time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      regionMesh(),
      dimensionedScalar("zero", dimEnergy/dimVolume/dimTime, 0.0)
    )
  );
  volScalarField& Q = tQ();
  volScalarField rho("rho", thermo_->rho());
  volScalarField alpha("alpha", thermo_->alpha());
  //If region is one-dimension variable thickness
  if (oneD_ && !constantThickness_)
  {
    // Scale K and rhoCp and fill Q in the internal baffle region.
    const label patchI = intCoupledPatchIDs_[0];
    const polyPatch& ppCoupled = rbm[patchI];
    FOR_ALL(ppCoupled, localFaceI)
    {
      const labelList& cells = boundaryFaceCells_[localFaceI];
      FOR_ALL(cells, i)
      {
        const label cellId = cells[i];
        Q[cellId] =
          Qs_.boundaryField()[patchI][localFaceI]
         /thickness_[localFaceI];
        rho[cellId] *= delta_.value()/thickness_[localFaceI];
        alpha[cellId] *= delta_.value()/thickness_[localFaceI];
      }
    }
  }
  else
  {
    Q = Q_;
  }
  fvScalarMatrix hEqn
  (
    fvm::ddt(rho, h_)
   - fvm::laplacian(alpha, h_)
  ==
    Q
  );
  if (moveMesh_)
  {
    surfaceScalarField phiMesh
    (
      fvc::interpolate(rho*h_)*regionMesh().phi()
    );
    hEqn -= fvc::div(phiMesh);
  }
  hEqn.relax();
  hEqn.solve();
  thermo_->correct();
  Info<< "T min/max   = " << min(thermo_->T()) << ", "
    << max(thermo_->T()) << endl;
}
// Constructors 
thermalBaffle::thermalBaffle
(
  const word& modelType,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  thermalBaffleModel(modelType, mesh, dict),
  nNonOrthCorr_(readLabel(solution().lookup("nNonOrthCorr"))),
  thermo_(solidThermo::New(regionMesh(), dict)),
  h_(thermo_->he()),
  Qs_
  (
    IOobject
    (
      "Qs",
      regionMesh().time().timeName(),
      regionMesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    ),
    regionMesh(),
    dimensionedScalar
    (
      "zero",
      dimEnergy/dimArea/dimTime,
      pTraits<scalar>::zero
    )
  ),
  Q_
  (
    IOobject
    (
      "Q",
      regionMesh().time().timeName(),
      regionMesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    ),
    regionMesh(),
    dimensionedScalar
    (
      "zero",
      dimEnergy/dimVolume/dimTime,
      pTraits<scalar>::zero
    )
  ),
  radiation_
  (
    radiation::radiationModel::New
    (
      dict.subDict("radiation"),
      thermo_->T()
    )
  )
{
  init();
  thermo_->correct();
}
thermalBaffle::thermalBaffle
(
  const word& modelType,
  const fvMesh& mesh
)
:
  thermalBaffleModel(modelType, mesh),
  nNonOrthCorr_(readLabel(solution().lookup("nNonOrthCorr"))),
  thermo_(solidThermo::New(regionMesh())),
  h_(thermo_->he()),
  Qs_
  (
    IOobject
    (
      "Qs",
      regionMesh().time().timeName(),
      regionMesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    ),
    regionMesh(),
    dimensionedScalar
    (
      "zero",
      dimEnergy/dimArea/dimTime,
      pTraits<scalar>::zero
    )
  ),
  Q_
  (
    IOobject
    (
      "Q",
      regionMesh().time().timeName(),
      regionMesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::NO_WRITE
    ),
    regionMesh(),
    dimensionedScalar
    (
      "zero",
      dimEnergy/dimVolume/dimTime,
      pTraits<scalar>::zero
    )
  ),
  radiation_
  (
    radiation::radiationModel::New
    (
      thermo_->T()
    )
  )
{
  init();
  thermo_->correct();
}
// Destructor 
thermalBaffle::~thermalBaffle()
{}
// Member Functions 
void thermalBaffle::init()
{
  if (oneD_ && !constantThickness_)
  {
    label patchI = intCoupledPatchIDs_[0];
    const label Qsb = Qs_.boundaryField()[patchI].size();
    if (Qsb!= thickness_.size())
    {
      FATAL_ERROR_IN("thermalBaffle::init()")
        << "the boundary field of Qs is "
        << Qsb << " and " << nl
        << "the field 'thickness' is " << thickness_.size() << nl
        << exit(FatalError);
    }
  }
}
void thermalBaffle::preEvolveRegion()
{}
void thermalBaffle::evolveRegion()
{
  for (int nonOrth=0; nonOrth<=nNonOrthCorr_; nonOrth++)
  {
    solveEnergy();
  }
}
const tmp<volScalarField> thermalBaffle::Cp() const
{
  return thermo_->Cp();
}
const volScalarField& thermalBaffle::kappaRad() const
{
  return radiation_->absorptionEmission().a();
}
const volScalarField& thermalBaffle::rho() const
{
  return thermo_->rho();
}
const volScalarField& thermalBaffle::kappa() const
{
  return thermo_->kappa();
}
const volScalarField& thermalBaffle::T() const
{
  return thermo_->T();
}
const solidThermo& thermalBaffle::thermo() const
{
  return thermo_;
}
void thermalBaffle::info()
{
  const labelList& coupledPatches = intCoupledPatchIDs();
  FOR_ALL(coupledPatches, i)
  {
    const label patchI = coupledPatches[i];
    const fvPatchScalarField& ph = h_.boundaryField()[patchI];
    const word patchName = regionMesh().boundary()[patchI].name();
    Info<< indent << "Q : " << patchName << indent <<
      gSum
      (
        mag(regionMesh().Sf().boundaryField()[patchI])
       * ph.snGrad()
       * thermo_->alpha().boundaryField()[patchI]
      ) << endl;
  }
}
} // end namespace thermalBaffleModels
} // end namespace regionModels
} // end namespace mousse
