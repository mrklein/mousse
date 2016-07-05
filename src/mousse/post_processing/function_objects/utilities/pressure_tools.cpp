// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_tools.hpp"
#include "vol_fields.hpp"
#include "dictionary.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(pressureTools, 0);

}


// Private Member Functions 
mousse::word mousse::pressureTools::pName() const
{
  word fieldName = pName_;
  if (calcTotal_) {
    fieldName = "total(" + fieldName + ")";
  } else {
    fieldName = "static(" + fieldName + ")";
  }
  if (calcCoeff_) {
    fieldName = fieldName + "_coeff";
  }
  return fieldName;
}


mousse::dimensionedScalar mousse::pressureTools::rhoScale
(
  const volScalarField& p
) const
{
  if (p.dimensions() == dimPressure) {
    return dimensionedScalar("1", dimless,  1.0);
  } else {
    return dimensionedScalar("rhoRef", dimDensity, rhoInf_);
  }
}


mousse::tmp<mousse::volScalarField> mousse::pressureTools::rho
(
  const volScalarField& p
) const
{
  if (p.dimensions() == dimPressure) {
    return p.mesh().lookupObject<volScalarField>(rhoName_);
  } else {
    return
      tmp<volScalarField>
      {
        new volScalarField
        {
          {
            "rho",
            p.mesh().time().timeName(),
            p.mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
          },
          p.mesh(),
          {"zero", dimDensity, rhoInf_}
        }
      };
  }
}


mousse::dimensionedScalar mousse::pressureTools::pRef() const
{
  dimensionedScalar value{"pRef", dimPressure, 0.0};
  if (calcTotal_) {
    value.value() += pRef_;
  }
  return value;
}


mousse::tmp<mousse::volScalarField> mousse::pressureTools::pDyn
(
  const volScalarField& p
) const
{
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  tmp<volScalarField> tpDyn
  {
    new volScalarField
    {
      {
        "pDyn",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh,
      {"zero", dimPressure, 0.0}
    }
  };
  if (calcTotal_) {
    const volVectorField& U = obr_.lookupObject<volVectorField>(UName_);
    tpDyn() == rho(p)*0.5*magSqr(U);
  }
  return tpDyn;
}


mousse::tmp<mousse::volScalarField> mousse::pressureTools::convertToCoeff
(
  const volScalarField& p
) const
{
  tmp<volScalarField> tCoeff{p};
  if (calcCoeff_) {
    tCoeff() -= dimensionedScalar{"pInf", dimPressure, pInf_};
    const dimensionedScalar p0{"p0", dimPressure, SMALL};
    const dimensionedVector U{"U", dimVelocity, UInf_};
    const dimensionedScalar rho{"rho", dimDensity, rhoInf_};
    tCoeff() /= 0.5*rho*magSqr(U) + p0;
  }
  return tCoeff;
}


// Constructors 
mousse::pressureTools::pressureTools
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  active_{true},
  pName_{"p"},
  UName_{"U"},
  rhoName_{"rho"},
  calcTotal_{false},
  pRef_{0.0},
  calcCoeff_{false},
  pInf_{0.0},
  UInf_{vector::zero},
  rhoInf_{0.0}
{
  // Check if the available mesh is an fvMesh, otherwise deactivate
  if (!isA<fvMesh>(obr_)) {
    active_ = false;
    WARNING_IN
    (
      "pressureTools::pressureTools"
      "("
      "  const word&, "
      "  const objectRegistry&, "
      "  const dictionary&, "
      "  const bool"
      ")"
    )
    << "No fvMesh available, deactivating " << name_ << nl
    << endl;
  }
  read(dict);
  if (active_) {
    dimensionSet pDims{dimPressure};
    if (calcCoeff_) {
      pDims /= dimPressure;
    }
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    volScalarField* pPtr
    {
      new volScalarField
      {
        {
          pName(),
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh,
        {"0", pDims, 0.0}
      }
    };
    mesh.objectRegistry::store(pPtr);
  }
}


// Destructor 
mousse::pressureTools::~pressureTools()
{}


// Member Functions 
void mousse::pressureTools::read(const dictionary& dict)
{
  if (!active_)
    return;
  dict.readIfPresent("pName", pName_);
  dict.readIfPresent("UName", UName_);
  dict.readIfPresent("rhoName", rhoName_);
  if (rhoName_ == "rhoInf") {
    dict.lookup("rhoInf") >> rhoInf_;
  }
  dict.lookup("calcTotal") >> calcTotal_;
  if (calcTotal_) {
    dict.lookup("pRef") >> pRef_;
  }
  dict.lookup("calcCoeff") >> calcCoeff_;
  if (calcCoeff_) {
    dict.lookup("pInf") >> pInf_;
    dict.lookup("UInf") >> UInf_;
    dict.lookup("rhoInf") >> rhoInf_;
    scalar zeroCheck = 0.5*rhoInf_*magSqr(UInf_) + pInf_;
    if (mag(zeroCheck) < ROOTVSMALL) {
      WARNING_IN("void mousse::pressureTools::read(const dictionary&)")
        << type() << " " << name_ << ": "
        << "Coefficient calculation requested, but reference "
        << "pressure level is zero.  Please check the supplied "
        << "values of pInf, UInf and rhoInf" << endl;
    }
  }
}


void mousse::pressureTools::execute()
{
  if (!active_)
    return;
  const volScalarField& p = obr_.lookupObject<volScalarField>(pName_);
  volScalarField& pResult =
    const_cast<volScalarField&>
    (
      obr_.lookupObject<volScalarField>(pName())
    );
  pResult == convertToCoeff(rhoScale(p)*p + pDyn(p) + pRef());
}


void mousse::pressureTools::end()
{
  if (active_) {
    execute();
  }
}


void mousse::pressureTools::timeSet()
{
  // Do nothing
}


void mousse::pressureTools::write()
{
  if (!active_)
    return;
  const volScalarField& pResult =
    obr_.lookupObject<volScalarField>(pName());
  Info << type() << " " << name_ << " output:" << nl
    << "    writing field " << pResult.name() << nl
    << endl;
  pResult.write();
}

