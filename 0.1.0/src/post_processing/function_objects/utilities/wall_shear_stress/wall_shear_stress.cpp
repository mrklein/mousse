// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_shear_stress.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "wall_poly_patch.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(wallShearStress, 0);
}
// Protected Member Functions 
void mousse::wallShearStress::writeFileHeader(const label /*i*/)
{
  // Add headers to output data
  writeHeader(file(), "Wall shear stress");
  writeCommented(file(), "Time");
  writeTabbed(file(), "patch");
  writeTabbed(file(), "min");
  writeTabbed(file(), "max");
  file() << endl;
}
void mousse::wallShearStress::calcShearStress
(
  const fvMesh& mesh,
  const volSymmTensorField& Reff,
  volVectorField& shearStress
)
{
  FOR_ALL_CONST_ITER(labelHashSet, patchSet_, iter)
  {
    label patchI = iter.key();
    const polyPatch& pp = mesh.boundaryMesh()[patchI];
    vectorField& ssp = shearStress.boundaryField()[patchI];
    const vectorField& Sfp = mesh.Sf().boundaryField()[patchI];
    const scalarField& magSfp = mesh.magSf().boundaryField()[patchI];
    const symmTensorField& Reffp = Reff.boundaryField()[patchI];
    ssp = (-Sfp/magSfp) & Reffp;
    vector minSsp = gMin(ssp);
    vector maxSsp = gMax(ssp);
    if (Pstream::master())
    {
      file() << mesh.time().value()
        << token::TAB << pp.name()
        << token::TAB << minSsp
        << token::TAB << maxSsp
        << endl;
    }
    if (log_) Info<< "    min/max(" << pp.name() << ") = "
      << minSsp << ", " << maxSsp << endl;
  }
}
// Constructors 
mousse::wallShearStress::wallShearStress
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  functionObjectFile{obr, name, typeName},
  name_{name},
  obr_{obr},
  active_{true},
  log_{true},
  patchSet_{}
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_))
  {
    active_ = false;
    WARNING_IN
    (
      "wallShearStress::wallShearStress"
      "("
        "const word&, "
        "const objectRegistry&, "
        "const dictionary&, "
        "const bool"
      ")"
    )
    << "No fvMesh available, deactivating " << name_ << nl
    << endl;
  }
  if (active_)
  {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volVectorField* wallShearStressPtr
    {
      new volVectorField
      {
        // IOobject
        {
          type(),
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh,
        // dimensionedVector
        {
          "0",
          sqr(dimLength)/sqr(dimTime),
          vector::zero
        }
      }
    };
    mesh.objectRegistry::store(wallShearStressPtr);
  }
  read(dict);
}
// Destructor 
mousse::wallShearStress::~wallShearStress()
{}
// Member Functions 
void mousse::wallShearStress::read(const dictionary& dict)
{
  if (active_)
  {
    log_ = dict.lookupOrDefault<Switch>("log", true);
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    const polyBoundaryMesh& pbm = mesh.boundaryMesh();
    patchSet_ =
      mesh.boundaryMesh().patchSet
      (
        wordReList(dict.lookupOrDefault("patches", wordReList()))
      );
    Info<< type() << " " << name_ << ":" << nl;
    if (patchSet_.empty())
    {
      FOR_ALL(pbm, patchI)
      {
        if (isA<wallPolyPatch>(pbm[patchI]))
        {
          patchSet_.insert(patchI);
        }
      }
      Info<< "    processing all wall patches" << nl << endl;
    }
    else
    {
      Info<< "    processing wall patches: " << nl;
      labelHashSet filteredPatchSet;
      FOR_ALL_CONST_ITER(labelHashSet, patchSet_, iter)
      {
        label patchI = iter.key();
        if (isA<wallPolyPatch>(pbm[patchI]))
        {
          filteredPatchSet.insert(patchI);
          Info<< "        " << pbm[patchI].name() << endl;
        }
        else
        {
          WARNING_IN("void wallShearStress::read(const dictionary&)")
            << "Requested wall shear stress on non-wall boundary "
            << "type patch: " << pbm[patchI].name() << endl;
        }
      }
      Info<< endl;
      patchSet_ = filteredPatchSet;
    }
  }
}
void mousse::wallShearStress::execute()
{
  typedef compressible::turbulenceModel cmpModel;
  typedef incompressible::turbulenceModel icoModel;
  if (active_)
  {
    functionObjectFile::write();
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volVectorField& wallShearStress =
      const_cast<volVectorField&>
      (
        mesh.lookupObject<volVectorField>(type())
      );
    if (log_) Info<< type() << " " << name_ << " output:" << nl;
    tmp<volSymmTensorField> Reff;
    if (mesh.foundObject<cmpModel>(turbulenceModel::propertiesName))
    {
      const cmpModel& model =
        mesh.lookupObject<cmpModel>(turbulenceModel::propertiesName);
      Reff = model.devRhoReff();
    }
    else if (mesh.foundObject<icoModel>(turbulenceModel::propertiesName))
    {
      const icoModel& model =
        mesh.lookupObject<icoModel>(turbulenceModel::propertiesName);
      Reff = model.devReff();
    }
    else
    {
      FATAL_ERROR_IN("void mousse::wallShearStress::execute()")
        << "Unable to find turbulence model in the "
        << "database" << exit(FatalError);
    }
    calcShearStress(mesh, Reff(), wallShearStress);
  }
}
void mousse::wallShearStress::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::wallShearStress::timeSet()
{
  // Do nothing
}
void mousse::wallShearStress::write()
{
  if (active_)
  {
    functionObjectFile::write();
    const volVectorField& wallShearStress =
      obr_.lookupObject<volVectorField>(type());
    if (log_) Info<< type() << " " << name_ << " output:" << nl
      << "    writing field " << wallShearStress.name() << nl
      << endl;
    wallShearStress.write();
  }
}
