// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "alphat_jayatilleke_wall_function_fv_patch_scalar_field.hpp"
#include "turbulent_fluid_thermo_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "wall_fv_patch.hpp"


namespace mousse {
namespace compressible{

// Static Data Members
scalar alphatJayatillekeWallFunctionFvPatchScalarField::maxExp_ = 50.0;
scalar alphatJayatillekeWallFunctionFvPatchScalarField::tolerance_ = 0.01;
label alphatJayatillekeWallFunctionFvPatchScalarField::maxIters_ = 10;


// Private Member Functions 
void alphatJayatillekeWallFunctionFvPatchScalarField::checkType()
{
  if (!isA<wallFvPatch>(patch())) {
    FATAL_ERROR_IN
    (
      "alphatJayatillekeWallFunctionFvPatchScalarField::checkType()"
    )
    << "Patch type for patch " << patch().name() << " must be wall\n"
    << "Current patch type is " << patch().type() << nl
    << exit(FatalError);
  }
}


scalar alphatJayatillekeWallFunctionFvPatchScalarField::Psmooth
(
  const scalar Prat
) const
{
  return 9.24*(pow(Prat, 0.75) - 1.0)*(1.0 + 0.28*exp(-0.007*Prat));
}


scalar alphatJayatillekeWallFunctionFvPatchScalarField::yPlusTherm
(
  const scalar P,
  const scalar Prat
) const
{
  scalar ypt = 11.0;
  for (int i=0; i<maxIters_; i++) {
    scalar f = ypt - (log(E_*ypt)/kappa_ + P)/Prat;
    scalar df = 1.0 - 1.0/(ypt*kappa_*Prat);
    scalar yptNew = ypt - f/df;
    if (yptNew < VSMALL) {
      return 0;
    } else if (mag(yptNew - ypt) < tolerance_) {
      return yptNew;
    } else {
      ypt = yptNew;
    }
  }
  return ypt;
}


// Constructors 
alphatJayatillekeWallFunctionFvPatchScalarField::
alphatJayatillekeWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{p, iF},
  Prt_{0.85},
  Cmu_{0.09},
  kappa_{0.41},
  E_{9.8}
{
  checkType();
}


alphatJayatillekeWallFunctionFvPatchScalarField::
alphatJayatillekeWallFunctionFvPatchScalarField
(
  const alphatJayatillekeWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchScalarField{ptf, p, iF, mapper},
  Prt_{ptf.Prt_},
  Cmu_{ptf.Cmu_},
  kappa_{ptf.kappa_},
  E_{ptf.E_}
{}


alphatJayatillekeWallFunctionFvPatchScalarField::
alphatJayatillekeWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchScalarField{p, iF, dict},
  Prt_{dict.lookupOrDefault<scalar>("Prt", 0.85)},
  Cmu_{dict.lookupOrDefault<scalar>("Cmu", 0.09)},
  kappa_{dict.lookupOrDefault<scalar>("kappa", 0.41)},
  E_{dict.lookupOrDefault<scalar>("E", 9.8)}
{
  checkType();
}


alphatJayatillekeWallFunctionFvPatchScalarField::
alphatJayatillekeWallFunctionFvPatchScalarField
(
  const alphatJayatillekeWallFunctionFvPatchScalarField& awfpsf
)
:
  fixedValueFvPatchScalarField{awfpsf},
  Prt_{awfpsf.Prt_},
  Cmu_{awfpsf.Cmu_},
  kappa_{awfpsf.kappa_},
  E_{awfpsf.E_}
{
  checkType();
}


alphatJayatillekeWallFunctionFvPatchScalarField::
alphatJayatillekeWallFunctionFvPatchScalarField
(
  const alphatJayatillekeWallFunctionFvPatchScalarField& awfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  fixedValueFvPatchScalarField{awfpsf, iF},
  Prt_{awfpsf.Prt_},
  Cmu_{awfpsf.Cmu_},
  kappa_{awfpsf.kappa_},
  E_{awfpsf.E_}
{
  checkType();
}


// Member Functions 
void alphatJayatillekeWallFunctionFvPatchScalarField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const label patchi = patch().index();
  // Retrieve turbulence properties from model
  const compressible::turbulenceModel& turbModel =
    db().lookupObject<compressible::turbulenceModel>
    (
      IOobject::groupName
      (
        compressible::turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const scalar Cmu25 = pow025(Cmu_);
  const scalarField& y = turbModel.y()[patchi];
  const tmp<scalarField> tmuw = turbModel.mu(patchi);
  const scalarField& muw = tmuw();
  const tmp<scalarField> talphaw = turbModel.alpha(patchi);
  const scalarField& alphaw = talphaw();
  scalarField& alphatw = *this;
  const tmp<volScalarField> tk = turbModel.k();
  const volScalarField& k = tk();
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  const scalarField magUp{mag(Uw.patchInternalField() - Uw)};
  const scalarField magGradUw{mag(Uw.snGrad())};
  const scalarField& rhow = turbModel.rho().boundaryField()[patchi];
  const fvPatchScalarField& hew =
    turbModel.transport().he().boundaryField()[patchi];
  // Heat flux [W/m2] - lagging alphatw
  const scalarField qDot
  {
    turbModel.transport().alphaEff(alphatw, patchi)*hew.snGrad()
  };
  // Populate boundary values
  FOR_ALL(alphatw, faceI) {
    label faceCellI = patch().faceCells()[faceI];
    scalar uTau = Cmu25*sqrt(k[faceCellI]);
    scalar yPlus = uTau*y[faceI]/(muw[faceI]/rhow[faceI]);
    // Molecular Prandtl number
    scalar Pr = muw[faceI]/alphaw[faceI];
    // Molecular-to-turbulent Prandtl number ratio
    scalar Prat = Pr/Prt_;
    // Thermal sublayer thickness
    scalar P = Psmooth(Prat);
    scalar yPlusTherm = this->yPlusTherm(P, Prat);
    // Evaluate new effective thermal diffusivity
    scalar alphaEff = 0.0;
    if (yPlus < yPlusTherm) {
      scalar A = qDot[faceI]*rhow[faceI]*uTau*y[faceI];
      scalar B = qDot[faceI]*Pr*yPlus;
      scalar C = Pr*0.5*rhow[faceI]*uTau*sqr(magUp[faceI]);
      alphaEff = A/(B + C + VSMALL);
    } else {
      scalar A = qDot[faceI]*rhow[faceI]*uTau*y[faceI];
      scalar B = qDot[faceI]*Prt_*(1.0/kappa_*log(E_*yPlus) + P);
      scalar magUc = uTau/kappa_*log(E_*yPlusTherm) - mag(Uw[faceI]);
      scalar C =
        0.5*rhow[faceI]*uTau*(Prt_*sqr(magUp[faceI]) + (Pr - Prt_)*sqr(magUc));
      alphaEff = A/(B + C + VSMALL);
    }
    // Update turbulent thermal diffusivity
    alphatw[faceI] = max(0.0, alphaEff - alphaw[faceI]);
    if (debug) {
      Info
        << "    uTau           = " << uTau << nl
        << "    Pr             = " << Pr << nl
        << "    Prt            = " << Prt_ << nl
        << "    qDot           = " << qDot[faceI] << nl
        << "    yPlus          = " << yPlus << nl
        << "    yPlusTherm     = " << yPlusTherm << nl
        << "    alphaEff       = " << alphaEff << nl
        << "    alphaw         = " << alphaw[faceI] << nl
        << "    alphatw        = " << alphatw[faceI] << nl
        << endl;
    }
  }
  fixedValueFvPatchField<scalar>::updateCoeffs();
}


void alphatJayatillekeWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  os.writeKeyword("Prt") << Prt_ << token::END_STATEMENT << nl;
  os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
  os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
  os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
  writeEntry("value", os);
}


MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  alphatJayatillekeWallFunctionFvPatchScalarField
);

}  // namespace compressible
}  // namespace mousse
