// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pyrolysis_chemistry_model.hpp"
#include "solid_reaction.hpp"
#include "basic_thermo.hpp"


// Constructors 
template<class CompType, class SolidThermo, class GasThermo>
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
pyrolysisChemistryModel
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  solidChemistryModel<CompType, SolidThermo>{mesh, phaseName},
  pyrolisisGases_{this->reactions_[0].gasSpecies()},
  gasThermo_{pyrolisisGases_.size()},
  nGases_{pyrolisisGases_.size()},
  nSpecie_{this->Ys_.size() + nGases_},
  RRg_{nGases_},
  Ys0_{this->nSolids_},
  cellCounter_{0}
{
  // create the fields for the chemistry sources
  FOR_ALL(this->RRs_, fieldI) {
    IOobject header
    {
      this->Ys_[fieldI].name() + "0",
      mesh.time().timeName(),
      mesh,
      IOobject::NO_READ
    };
    // check if field exists and can be read
    if (header.headerOk()) {
      Ys0_.set
      (
        fieldI,
        new volScalarField
        {
          // IOobject
          {
            this->Ys_[fieldI].name() + "0",
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
          },
          mesh
        }
      );
    } else {
      volScalarField Y0Default
      {
        // IOobject
        {
          "Y0Default",
          mesh.time().timeName(),
          mesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        mesh
      };
      Ys0_.set
      (
        fieldI,
        new volScalarField
        {
          // IOobject
          {
            this->Ys_[fieldI].name() + "0",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
          },
          Y0Default
        }
      );
      // Calculate inital values of Ysi0 = rho*delta*Yi
      Ys0_[fieldI].internalField() =
        this->solidThermo().rho()*max(this->Ys_[fieldI], scalar(0.001))*mesh.V();
    }
  }
  FOR_ALL(RRg_, fieldI) {
    RRg_.set
    (
      fieldI,
      new DimensionedField<scalar, volMesh>
      {
        // IOobject
        {
          "RRg." + pyrolisisGases_[fieldI],
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        mesh,
        {"zero", dimMass/dimVolume/dimTime, 0.0}
      }
    );
  }
  FOR_ALL(gasThermo_, gasI) {
    dictionary thermoDict =
      mesh.lookupObject<dictionary>(basicThermo::dictName)
        .subDict(pyrolisisGases_[gasI]);
    gasThermo_.set
    (
      gasI,
      new GasThermo{thermoDict}
    );
  }
  Info << "pyrolysisChemistryModel: " << nl;
  Info << indent << "Number of solids = " << this->nSolids_ << nl;
  Info << indent << "Number of gases = " << nGases_ << nl;
  FOR_ALL(this->reactions_, i) {
    Info <<
      dynamic_cast<const solidReaction<SolidThermo>&>(this->reactions_[i])
      << nl;
  }
}


// Destructor 
template<class CompType, class SolidThermo, class GasThermo>
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
~pyrolysisChemistryModel()
{}


// Member Functions 
template<class CompType, class SolidThermo, class GasThermo>
mousse::scalarField mousse::
pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::omega
(
  const scalarField& c,
  const scalar T,
  const scalar p,
  const bool updateC0
) const
{
  scalar pf, cf, pr, cr;
  label lRef, rRef;
  const label cellI = cellCounter_;
  scalarField om{nEqns(), 0.0};
  FOR_ALL(this->reactions_, i) {
    const Reaction<SolidThermo>& R = this->reactions_[i];
    scalar omegai =
      omega(R, c, T, p, pf, cf, lRef, pr, cr, rRef);
    scalar rhoL = 0.0;
    FOR_ALL(R.lhs(), s) {
      label si = R.lhs()[s].index;
      om[si] -= omegai;
      rhoL = this->solidThermo_[si].rho(p, T);
    }
    scalar sr = 0.0;
    FOR_ALL(R.rhs(), s) {
      label si = R.rhs()[s].index;
      scalar rhoR = this->solidThermo_[si].rho(p, T);
      sr = rhoR/rhoL;
      om[si] += sr*omegai;
      if (updateC0) {
        Ys0_[si][cellI] += sr*omegai;
      }
    }
    FOR_ALL(R.grhs(), g) {
      label gi = R.grhs()[g].index;
      om[gi + this->nSolids_] += (1.0 - sr)*omegai;
    }
  }
  return om;
}


template<class CompType, class SolidThermo, class GasThermo>
mousse::scalar
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::omega
(
  const Reaction<SolidThermo>& R,
  const scalarField& c,
  const scalar T,
  const scalar p,
  scalar& /*pf*/,
  scalar& /*cf*/,
  label& /*lRef*/,
  scalar& /*pr*/,
  scalar& /*cr*/,
  label& /*rRef*/
) const
{
  scalarField c1{nSpecie_, 0.0};
  label cellI = cellCounter_;
  for (label i=0; i<nSpecie_; i++) {
    c1[i] = max(0.0, c[i]);
  }
  scalar kf = R.kf(p, T, c1);
  const label Nl = R.lhs().size();
  for (label s=0; s<Nl; s++) {
    label si = R.lhs()[s].index;
    const scalar exp = R.lhs()[si].exponent;
    kf *= pow(c1[si]/Ys0_[si][cellI], exp)*(Ys0_[si][cellI]);
  }
  return kf;
}


template<class CompType, class SolidThermo, class GasThermo>
mousse::scalar mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
omegaI
(
  const label index,
  const scalarField& c,
  const scalar T,
  const scalar p,
  scalar& pf,
  scalar& cf,
  label& lRef,
  scalar& pr,
  scalar& cr,
  label& rRef
) const
{
  const Reaction<SolidThermo>& R = this->reactions_[index];
  scalar w = omega(R, c, T, p, pf, cf, lRef, pr, cr, rRef);
  return(w);
}


template<class CompType, class SolidThermo, class GasThermo>
void mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
derivatives
(
  const scalar /*time*/,
  const scalarField &c,
  scalarField& dcdt
) const
{
  const scalar T = c[nSpecie_];
  const scalar p = c[nSpecie_ + 1];
  dcdt = 0.0;
  dcdt = omega(c, T, p);
  //Total mass concentration
  scalar cTot = 0.0;
  for (label i=0; i<this->nSolids_; i++) {
    cTot += c[i];
  }
  scalar newCp = 0.0;
  scalar newhi = 0.0;
  for (label i=0; i<this->nSolids_; i++) {
    scalar dYidt = dcdt[i]/cTot;
    scalar Yi = c[i]/cTot;
    newCp += Yi*this->solidThermo_[i].Cp(p, T);
    newhi -= dYidt*this->solidThermo_[i].Hc();
  }
  scalar dTdt = newhi/newCp;
  scalar dtMag = min(500.0, mag(dTdt));
  dcdt[nSpecie_] = dTdt*dtMag/(mag(dTdt) + 1.0e-10);
  // dp/dt = ...
  dcdt[nSpecie_ + 1] = 0.0;
}


template<class CompType, class SolidThermo, class GasThermo>
void mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
jacobian
(
  const scalar /*t*/,
  const scalarField& c,
  scalarField& dcdt,
  scalarSquareMatrix& dfdc
) const
{
  const scalar T = c[nSpecie_];
  const scalar p = c[nSpecie_ + 1];
  scalarField c2{nSpecie_, 0.0};
  for (label i=0; i<this->nSolids_; i++) {
    c2[i] = max(c[i], 0.0);
  }
  for (label i=0; i<nEqns(); i++) {
    for (label j=0; j<nEqns(); j++) {
      dfdc[i][j] = 0.0;
    }
  }
  // length of the first argument must be nSolids
  dcdt = omega(c2, T, p);
  for (label ri=0; ri<this->reactions_.size(); ri++) {
    const Reaction<SolidThermo>& R = this->reactions_[ri];
    scalar kf0 = R.kf(p, T, c2);
    FOR_ALL(R.lhs(), j) {
      label sj = R.lhs()[j].index;
      scalar kf = kf0;
      FOR_ALL(R.lhs(), i) {
        label si = R.lhs()[i].index;
        scalar exp = R.lhs()[i].exponent;
        if (i == j) {
          if (exp < 1.0) {
            if (c2[si]>SMALL) {
              kf *= exp*pow(c2[si] + VSMALL, exp - 1.0);
            } else {
              kf = 0.0;
            }
          } else {
            kf *= exp*pow(c2[si], exp - 1.0);
          }
        } else {
          Info << "Solid reactions have only elements on slhs" << endl;
          kf = 0.0;
        }
      }
      FOR_ALL(R.lhs(), i) {
        label si = R.lhs()[i].index;
        dfdc[si][sj] -= kf;
      }
      FOR_ALL(R.rhs(), i) {
        label si = R.rhs()[i].index;
        dfdc[si][sj] += kf;
      }
    }
  }
  // calculate the dcdT elements numerically
  scalar delta = 1.0e-8;
  scalarField dcdT0 = omega(c2, T - delta, p);
  scalarField dcdT1 = omega(c2, T + delta, p);
  for (label i=0; i<nEqns(); i++) {
    dfdc[i][nSpecie_] = 0.5*(dcdT1[i] - dcdT0[i])/delta;
  }
}


template<class CompType, class SolidThermo, class GasThermo>
mousse::label mousse::
pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::nEqns() const
{
  // nEqns = number of solids + gases + temperature + pressure
  return (nSpecie_ + 2);
}


template<class CompType, class SolidThermo, class GasThermo>
void mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::
calculate()
{
  if (!this->chemistry_) {
    return;
  }
  const volScalarField rho
  {
    {
      "rho",
      this->time().timeName(),
      this->mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    this->solidThermo().rho()
  };
  FOR_ALL(this->RRs_, i) {
    this->RRs_[i].field() = 0.0;
  }
  FOR_ALL(RRg_, i) {
    RRg_[i].field() = 0.0;
  }
  FOR_ALL(rho, celli) {
    cellCounter_ = celli;
    const scalar delta = this->mesh().V()[celli];
    if (this->reactingCells_[celli]) {
      scalar rhoi = rho[celli];
      scalar Ti = this->solidThermo().T()[celli];
      scalar pi = this->solidThermo().p()[celli];
      scalarField c{nSpecie_, 0.0};
      for (label i=0; i<this->nSolids_; i++) {
        c[i] = rhoi*this->Ys_[i][celli]*delta;
      }
      const scalarField dcdt = omega(c, Ti, pi, true);
      FOR_ALL(this->RRs_, i) {
        this->RRs_[i][celli] = dcdt[i]/delta;
      }
      FOR_ALL(RRg_, i) {
        RRg_[i][celli] = dcdt[this->nSolids_ + i]/delta;
      }
    }
  }
}


template<class CompType, class SolidThermo, class GasThermo>
mousse::scalar
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::solve
(
  const scalar deltaT
)
{
  scalar deltaTMin = GREAT;
  if (!this->chemistry_) {
    return deltaTMin;
  }
  const volScalarField rho
  {
    {
      "rho",
      this->time().timeName(),
      this->mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    this->solidThermo().rho()
  };
  FOR_ALL(this->RRs_, i) {
    this->RRs_[i].field() = 0.0;
  }
  FOR_ALL(RRg_, i) {
    RRg_[i].field() = 0.0;
  }
  const scalarField& T = this->solidThermo().T();
  const scalarField& p = this->solidThermo().p();
  scalarField c{nSpecie_, 0.0};
  scalarField c0{nSpecie_, 0.0};
  scalarField dc{nSpecie_, 0.0};
  scalarField delta{this->mesh().V()};
  FOR_ALL(rho, celli) {
    if (this->reactingCells_[celli]) {
      cellCounter_ = celli;
      scalar rhoi = rho[celli];
      scalar pi = p[celli];
      scalar Ti = T[celli];
      for (label i=0; i<this->nSolids_; i++) {
        c[i] = rhoi*this->Ys_[i][celli]*delta[celli];
      }
      c0 = c;
      // Initialise time progress
      scalar timeLeft = deltaT;
      // calculate the chemical source terms
      while (timeLeft > SMALL) {
        scalar dt = timeLeft;
        this->solve(c, Ti, pi, dt, this->deltaTChem_[celli]);
        timeLeft -= dt;
      }
      deltaTMin = min(this->deltaTChem_[celli], deltaTMin);
      dc = c - c0;
      FOR_ALL(this->RRs_, i) {
        this->RRs_[i][celli] = dc[i]/(deltaT*delta[celli]);
      }
      FOR_ALL(RRg_, i) {
        RRg_[i][celli] = dc[this->nSolids_ + i]/(deltaT*delta[celli]);
      }
      // Update Ys0_
      dc = omega(c0, Ti, pi, true);
    }
  }
  // Don't allow the time-step to change more than a factor of 2
  deltaTMin = min(deltaTMin, 2*deltaT);
  return deltaTMin;
}


template<class CompType, class SolidThermo,class GasThermo>
mousse::tmp<mousse::volScalarField>
mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::gasHs
(
  const volScalarField& p,
  const volScalarField& T,
  const label index
) const
{
  tmp<volScalarField> tHs
  {
    new volScalarField
    {
      {
        "Hs_" + pyrolisisGases_[index],
        this->mesh_.time().timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh_,
      {"zero", dimEnergy/dimMass, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  volScalarField::InternalField& gasHs = tHs().internalField();
  const GasThermo& mixture = gasThermo_[index];
  FOR_ALL(gasHs, cellI) {
    gasHs[cellI] = mixture.Hs(p[cellI], T[cellI]);
  }
  return tHs;
}


template<class CompType, class SolidThermo, class GasThermo>
void mousse::pyrolysisChemistryModel<CompType, SolidThermo, GasThermo>::solve
(
  scalarField& /*c*/,
  scalar& /*T*/,
  scalar& /*p*/,
  scalar& /*deltaT*/,
  scalar& /*subDeltaT*/
) const
{
  NOT_IMPLEMENTED
  (
    "pyrolysisChemistryModel::solve"
    "("
      "scalarField&, "
      "scalar&, "
      "scalar&, "
      "scalar&, "
      "scalar&  "
    ") const"
  );
}

