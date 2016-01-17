// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar_transport.hpp"
#include "surface_fields.hpp"
#include "dictionary.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "fv_scalar_matrix.hpp"
#include "fvm_ddt.hpp"
#include "fvm_div.hpp"
#include "fvc_div.hpp"
#include "fvm_laplacian.hpp"
#include "fvm_sup.hpp"
#include "turbulent_transport_model.hpp"
#include "turbulent_fluid_thermo_model.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(scalarTransport, 0);
}
// Private Member Functions 
mousse::wordList mousse::scalarTransport::boundaryTypes() const
{
  const volVectorField& U = mesh_.lookupObject<volVectorField>(UName_);
  wordList bTypes(U.boundaryField().size());
  FOR_ALL(bTypes, patchI)
  {
    const fvPatchField<vector>& pf = U.boundaryField()[patchI];
    if (isA<fixedValueFvPatchVectorField>(pf))
    {
      bTypes[patchI] = fixedValueFvPatchScalarField::typeName;
    }
    else
    {
      bTypes[patchI] = zeroGradientFvPatchScalarField::typeName;
    }
  }
  return bTypes;
}
mousse::tmp<mousse::volScalarField> mousse::scalarTransport::DT
(
  const surfaceScalarField& phi
) const
{
  typedef incompressible::turbulenceModel icoModel;
  typedef compressible::turbulenceModel cmpModel;
  if (userDT_)
  {
    return tmp<volScalarField>
    (
      new volScalarField
      (
        IOobject
        (
          "DT",
          mesh_.time().timeName(),
          mesh_.time(),
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("DT", phi.dimensions()/dimLength, DT_)
      )
    );
  }
  else if (mesh_.foundObject<icoModel>(turbulenceModel::propertiesName))
  {
    const icoModel& model = mesh_.lookupObject<icoModel>
    (
      turbulenceModel::propertiesName
    );
    return model.nuEff();
  }
  else if (mesh_.foundObject<cmpModel>(turbulenceModel::propertiesName))
  {
    const cmpModel& model = mesh_.lookupObject<cmpModel>
    (
      turbulenceModel::propertiesName
    );
    return model.muEff();
  }
  else
  {
    return tmp<volScalarField>
    (
      new volScalarField
      (
        IOobject
        (
          "DT",
          mesh_.time().timeName(),
          mesh_.time(),
          IOobject::NO_READ,
          IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("DT", phi.dimensions()/dimLength, 0.0)
      )
    );
  }
}
// Constructors 
mousse::scalarTransport::scalarTransport
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  mesh_{refCast<const fvMesh>(obr)},
  active_{true},
  phiName_{"phi"},
  UName_{"U"},
  rhoName_{"rho"},
  DT_{0.0},
  userDT_{false},
  resetOnStartUp_{false},
  nCorr_{0},
  autoSchemes_{false},
  fvOptions_{mesh_},
  T_
  {
    // IOobject
    {
      name,
      mesh_.time().timeName(),
      mesh_,
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"zero", dimless, 0.0},
    boundaryTypes()
  }
{
  read(dict);
  if (resetOnStartUp_)
  {
    T_ == dimensionedScalar{"zero", dimless, 0.0};
  }
}
// Destructor 
mousse::scalarTransport::~scalarTransport()
{}
// Member Functions 
void mousse::scalarTransport::read(const dictionary& dict)
{
  if (active_)
  {
    Info<< type() << ":" << nl;
    phiName_ = dict.lookupOrDefault<word>("phiName", "phi");
    UName_ = dict.lookupOrDefault<word>("UName", "U");
    rhoName_ = dict.lookupOrDefault<word>("rhoName", "rho");
    userDT_ = false;
    if (dict.readIfPresent("DT", DT_))
    {
      userDT_ = true;
    }
    dict.lookup("resetOnStartUp") >> resetOnStartUp_;
    dict.readIfPresent("nCorr", nCorr_);
    dict.lookup("autoSchemes") >> autoSchemes_;
    fvOptions_.reset(dict.subDict("fvOptions"));
  }
}
void mousse::scalarTransport::execute()
{
  if (active_)
  {
    Info<< type() << " output:" << endl;
    const surfaceScalarField& phi =
      mesh_.lookupObject<surfaceScalarField>(phiName_);
    // calculate the diffusivity
    volScalarField DT{this->DT(phi)};
    // set schemes
    word schemeVar = T_.name();
    if (autoSchemes_)
    {
      schemeVar = UName_;
    }
    word divScheme{"div(phi," + schemeVar + ")"};
    word laplacianScheme{"laplacian(" + DT.name() + "," + schemeVar + ")"};
    // set under-relaxation coeff
    scalar relaxCoeff = 0.0;
    if (mesh_.relaxEquation(schemeVar))
    {
      relaxCoeff = mesh_.equationRelaxationFactor(schemeVar);
    }
    if (phi.dimensions() == dimMass/dimTime)
    {
      const volScalarField& rho =
        mesh_.lookupObject<volScalarField>(rhoName_);
      // solve
      for (label i = 0; i <= nCorr_; i++)
      {
        fvScalarMatrix TEqn
        {
          fvm::ddt(rho, T_)
         + fvm::div(phi, T_, divScheme)
         - fvm::laplacian(DT, T_, laplacianScheme)
        ==
          fvOptions_(rho, T_)
        };
        TEqn.relax(relaxCoeff);
        fvOptions_.constrain(TEqn);
        TEqn.solve(mesh_.solverDict(schemeVar));
      }
    }
    else if (phi.dimensions() == dimVolume/dimTime)
    {
      // solve
      for (label i = 0; i <= nCorr_; i++)
      {
        fvScalarMatrix TEqn
        {
          fvm::ddt(T_)
         + fvm::div(phi, T_, divScheme)
         - fvm::laplacian(DT, T_, laplacianScheme)
        ==
          fvOptions_(T_)
        };
        TEqn.relax(relaxCoeff);
        fvOptions_.constrain(TEqn);
        TEqn.solve(mesh_.solverDict(schemeVar));
      }
    }
    else
    {
      FATAL_ERROR_IN("void mousse::scalarTransport::execute()")
        << "Incompatible dimensions for phi: " << phi.dimensions() << nl
        << "Dimensions should be " << dimMass/dimTime << " or "
        << dimVolume/dimTime << endl;
    }
    Info<< endl;
  }
}
void mousse::scalarTransport::end()
{
  if (active_)
  {
    execute();
  }
}
void mousse::scalarTransport::timeSet()
{
  // Do nothing
}
void mousse::scalarTransport::write()
{
  // Do nothing
}
