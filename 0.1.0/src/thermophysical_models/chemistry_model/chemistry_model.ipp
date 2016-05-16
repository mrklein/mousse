// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "chemistry_model.hpp"
#include "reacting_mixture.hpp"
#include "uniform_field.hpp"


// Constructors 

template<class CompType, class ThermoType>
mousse::chemistryModel<CompType, ThermoType>::chemistryModel
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  CompType{mesh, phaseName},
  ODESystem{},
  Y_{this->thermo().composition().Y()},
  reactions_
  {
    dynamic_cast<const reactingMixture<ThermoType>&>(this->thermo())
  },
  specieThermo_
  {
    dynamic_cast<const reactingMixture<ThermoType>&>(this->thermo())
      .speciesData()
  },
  nSpecie_{Y_.size()},
  nReaction_{reactions_.size()},
  Treact_{CompType::template lookupOrDefault<scalar>("Treact", 0.0)},
  RR_{nSpecie_}
{
  // create the fields for the chemistry sources
  FOR_ALL(RR_, fieldI) {
    RR_.set
    (
      fieldI,
      new DimensionedField<scalar, volMesh>
      {
        {
          "RR." + Y_[fieldI].name(),
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
  Info << "chemistryModel: Number of species = " << nSpecie_
    << " and reactions = " << nReaction_ << endl;
}


// Destructor 
template<class CompType, class ThermoType>
mousse::chemistryModel<CompType, ThermoType>::~chemistryModel()
{}


// Member Functions 
template<class CompType, class ThermoType>
mousse::tmp<mousse::scalarField>
mousse::chemistryModel<CompType, ThermoType>::omega
(
  const scalarField& c,
  const scalar T,
  const scalar p
) const
{
  scalar pf, cf, pr, cr;
  label lRef, rRef;
  tmp<scalarField> tom{new scalarField{nEqns(), 0.0}};
  scalarField& om = tom();
  FOR_ALL(reactions_, i) {
    const Reaction<ThermoType>& R = reactions_[i];
    scalar omegai = omega(R, c, T, p, pf, cf, lRef, pr, cr, rRef);
    FOR_ALL(R.lhs(), s) {
      const label si = R.lhs()[s].index;
      const scalar sl = R.lhs()[s].stoichCoeff;
      om[si] -= sl*omegai;
    }
    FOR_ALL(R.rhs(), s) {
      const label si = R.rhs()[s].index;
      const scalar sr = R.rhs()[s].stoichCoeff;
      om[si] += sr*omegai;
    }
  }
  return tom;
}


template<class CompType, class ThermoType>
mousse::scalar mousse::chemistryModel<CompType, ThermoType>::omegaI
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
  const Reaction<ThermoType>& R = reactions_[index];
  scalar w = omega(R, c, T, p, pf, cf, lRef, pr, cr, rRef);
  return(w);
}


template<class CompType, class ThermoType>
mousse::scalar mousse::chemistryModel<CompType, ThermoType>::omega
(
  const Reaction<ThermoType>& R,
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
  scalarField c2(nSpecie_, 0.0);
  for (label i = 0; i < nSpecie_; i++) {
    c2[i] = max(0.0, c[i]);
  }
  const scalar kf = R.kf(p, T, c2);
  const scalar kr = R.kr(kf, p, T, c2);
  pf = 1.0;
  pr = 1.0;
  const label Nl = R.lhs().size();
  const label Nr = R.rhs().size();
  label slRef = 0;
  lRef = R.lhs()[slRef].index;
  pf = kf;
  for (label s = 1; s < Nl; s++) {
    const label si = R.lhs()[s].index;
    if (c[si] < c[lRef]) {
      const scalar exp = R.lhs()[slRef].exponent;
      pf *= pow(max(0.0, c[lRef]), exp);
      lRef = si;
      slRef = s;
    } else {
      const scalar exp = R.lhs()[s].exponent;
      pf *= pow(max(0.0, c[si]), exp);
    }
  }

  cf = max(0.0, c[lRef]);

  {
    const scalar exp = R.lhs()[slRef].exponent;
    if (exp < 1.0) {
      if (cf > SMALL) {
        pf *= pow(cf, exp - 1.0);
      } else {
        pf = 0.0;
      }
    } else {
      pf *= pow(cf, exp - 1.0);
    }
  }
  label srRef = 0;
  rRef = R.rhs()[srRef].index;
  // find the matrix element and element position for the rhs
  pr = kr;
  for (label s = 1; s < Nr; s++) {
    const label si = R.rhs()[s].index;
    if (c[si] < c[rRef]) {
      const scalar exp = R.rhs()[srRef].exponent;
      pr *= pow(max(0.0, c[rRef]), exp);
      rRef = si;
      srRef = s;
    } else {
      const scalar exp = R.rhs()[s].exponent;
      pr *= pow(max(0.0, c[si]), exp);
    }
  }

  cr = max(0.0, c[rRef]);

  {
    const scalar exp = R.rhs()[srRef].exponent;
    if (exp < 1.0) {
      if (cr>SMALL) {
        pr *= pow(cr, exp - 1.0);
      } else {
        pr = 0.0;
      }
    } else {
      pr *= pow(cr, exp - 1.0);
    }
  }

  return pf*cf - pr*cr;
}


template<class CompType, class ThermoType>
void mousse::chemistryModel<CompType, ThermoType>::derivatives
(
  const scalar /*time*/,
  const scalarField &c,
  scalarField& dcdt
) const
{
  const scalar T = c[nSpecie_];
  const scalar p = c[nSpecie_ + 1];
  dcdt = omega(c, T, p);
  // constant pressure
  // dT/dt = ...
  scalar rho = 0.0;
  scalar cSum = 0.0;
  for (label i = 0; i < nSpecie_; i++) {
    const scalar W = specieThermo_[i].W();
    cSum += c[i];
    rho += W*c[i];
  }
  scalar cp = 0.0;
  for (label i=0; i<nSpecie_; i++) {
    cp += c[i]*specieThermo_[i].cp(p, T);
  }
  cp /= rho;
  scalar dT = 0.0;
  for (label i = 0; i < nSpecie_; i++) {
    const scalar hi = specieThermo_[i].ha(p, T);
    dT += hi*dcdt[i];
  }
  dT /= rho*cp;
  dcdt[nSpecie_] = -dT;
  // dp/dt = ...
  dcdt[nSpecie_ + 1] = 0.0;
}


template<class CompType, class ThermoType>
void mousse::chemistryModel<CompType, ThermoType>::jacobian
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
  FOR_ALL(c2, i) {
    c2[i] = max(c[i], 0.0);
  }
  for (label i=0; i<nEqns(); i++) {
    for (label j=0; j<nEqns(); j++) {
      dfdc[i][j] = 0.0;
    }
  }
  // Length of the first argument must be nSpecie()
  dcdt = omega(c2, T, p);
  FOR_ALL(reactions_, ri) {
    const Reaction<ThermoType>& R = reactions_[ri];
    const scalar kf0 = R.kf(p, T, c2);
    const scalar kr0 = R.kr(kf0, p, T, c2);
    FOR_ALL(R.lhs(), j) {
      const label sj = R.lhs()[j].index;
      scalar kf = kf0;
      FOR_ALL(R.lhs(), i) {
        const label si = R.lhs()[i].index;
        const scalar el = R.lhs()[i].exponent;
        if (i == j) {
          if (el < 1.0) {
            if (c2[si] > SMALL) {
              kf *= el*pow(c2[si] + VSMALL, el - 1.0);
            } else {
              kf = 0.0;
            }
          } else {
            kf *= el*pow(c2[si], el - 1.0);
          }
        } else {
          kf *= pow(c2[si], el);
        }
      }
      FOR_ALL(R.lhs(), i) {
        const label si = R.lhs()[i].index;
        const scalar sl = R.lhs()[i].stoichCoeff;
        dfdc[si][sj] -= sl*kf;
      }
      FOR_ALL(R.rhs(), i) {
        const label si = R.rhs()[i].index;
        const scalar sr = R.rhs()[i].stoichCoeff;
        dfdc[si][sj] += sr*kf;
      }
    }
    FOR_ALL(R.rhs(), j) {
      const label sj = R.rhs()[j].index;
      scalar kr = kr0;
      FOR_ALL(R.rhs(), i) {
        const label si = R.rhs()[i].index;
        const scalar er = R.rhs()[i].exponent;
        if (i == j) {
          if (er < 1.0) {
            if (c2[si] > SMALL) {
              kr *= er*pow(c2[si] + VSMALL, er - 1.0);
            } else {
              kr = 0.0;
            }
          } else {
            kr *= er*pow(c2[si], er - 1.0);
          }
        } else {
          kr *= pow(c2[si], er);
        }
      }
      FOR_ALL(R.lhs(), i) {
        const label si = R.lhs()[i].index;
        const scalar sl = R.lhs()[i].stoichCoeff;
        dfdc[si][sj] += sl*kr;
      }
      FOR_ALL(R.rhs(), i) {
        const label si = R.rhs()[i].index;
        const scalar sr = R.rhs()[i].stoichCoeff;
        dfdc[si][sj] -= sr*kr;
      }
    }
  }
  // Calculate the dcdT elements numerically
  const scalar delta = 1.0e-3;
  const scalarField dcdT0(omega(c2, T - delta, p));
  const scalarField dcdT1(omega(c2, T + delta, p));
  for (label i = 0; i < nEqns(); i++) {
    dfdc[i][nSpecie()] = 0.5*(dcdT1[i] - dcdT0[i])/delta;
  }
}


template<class CompType, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::chemistryModel<CompType, ThermoType>::tc() const
{
  scalar pf, cf, pr, cr;
  label lRef, rRef;
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
    this->thermo().rho()
  };
  tmp<volScalarField> ttc
  {
    new volScalarField
    {
      {
        "tc",
        this->time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh(),
      {"zero", dimTime, SMALL},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  scalarField& tc = ttc();
  const scalarField& T = this->thermo().T();
  const scalarField& p = this->thermo().p();
  const label nReaction = reactions_.size();
  if (this->chemistry_) {
    FOR_ALL(rho, celli) {
      scalar rhoi = rho[celli];
      scalar Ti = T[celli];
      scalar pi = p[celli];
      scalarField c(nSpecie_);
      scalar cSum = 0.0;
      for (label i=0; i<nSpecie_; i++) {
        scalar Yi = Y_[i][celli];
        c[i] = rhoi*Yi/specieThermo_[i].W();
        cSum += c[i];
      }
      FOR_ALL(reactions_, i) {
        const Reaction<ThermoType>& R = reactions_[i];
        omega(R, c, Ti, pi, pf, cf, lRef, pr, cr, rRef);
        FOR_ALL(R.rhs(), s) {
          scalar sr = R.rhs()[s].stoichCoeff;
          tc[celli] += sr*pf*cf;
        }
      }
      tc[celli] = nReaction*cSum/tc[celli];
    }
  }
  ttc().correctBoundaryConditions();
  return ttc;
}


template<class CompType, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::chemistryModel<CompType, ThermoType>::Sh() const
{
  tmp<volScalarField> tSh
  {
    new volScalarField
    {
      {
        "Sh",
        this->mesh_.time().timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh_,
      {"zero", dimEnergy/dimTime/dimVolume, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  if (this->chemistry_) {
    scalarField& Sh = tSh();
    FOR_ALL(Y_, i) {
      FOR_ALL(Sh, cellI) {
        const scalar hi = specieThermo_[i].Hc();
        Sh[cellI] -= hi*RR_[i][cellI];
      }
    }
  }
  return tSh;
}


template<class CompType, class ThermoType>
mousse::tmp<mousse::volScalarField>
mousse::chemistryModel<CompType, ThermoType>::dQ() const
{
  tmp<volScalarField> tdQ
  {
    new volScalarField
    {
      {
        "dQ",
        this->mesh_.time().timeName(),
        this->mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      this->mesh_,
      {"dQ", dimEnergy/dimTime, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  if (this->chemistry_) {
    volScalarField& dQ = tdQ();
    dQ.dimensionedInternalField() = this->mesh_.V()*Sh()();
  }

  return tdQ;
}


template<class CompType, class ThermoType>
mousse::label mousse::chemistryModel<CompType, ThermoType>::nEqns() const
{
  // nEqns = number of species + temperature + pressure
  return nSpecie_ + 2;
}


template<class CompType, class ThermoType>
mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh>>
mousse::chemistryModel<CompType, ThermoType>::calculateRR
(
  const label reactionI,
  const label speciei
) const
{
  scalar pf, cf, pr, cr;
  label lRef, rRef;
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
    this->thermo().rho()
  };
  tmp<DimensionedField<scalar, volMesh>> tRR
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        "RR",
        this->mesh().time().timeName(),
        this->mesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      this->mesh(),
      {"zero", dimMass/dimVolume/dimTime, 0.0}
    }
  };
  DimensionedField<scalar, volMesh>& RR = tRR();
  const scalarField& T = this->thermo().T();
  const scalarField& p = this->thermo().p();
  FOR_ALL(rho, celli) {
    const scalar rhoi = rho[celli];
    const scalar Ti = T[celli];
    const scalar pi = p[celli];
    scalarField c{nSpecie_, 0.0};
    for (label i=0; i<nSpecie_; i++) {
      const scalar Yi = Y_[i][celli];
      c[i] = rhoi*Yi/specieThermo_[i].W();
    }
    const scalar w =
      omegaI(reactionI, c, Ti, pi, pf, cf, lRef, pr, cr, rRef);
    RR[celli] = w*specieThermo_[speciei].W();
  }
  return tRR;
}


template<class CompType, class ThermoType>
void mousse::chemistryModel<CompType, ThermoType>::calculate()
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
    this->thermo().rho()
  };
  const scalarField& T = this->thermo().T();
  const scalarField& p = this->thermo().p();
  FOR_ALL(rho, celli) {
    const scalar rhoi = rho[celli];
    const scalar Ti = T[celli];
    const scalar pi = p[celli];
    scalarField c{nSpecie_, 0.0};
    for (label i=0; i<nSpecie_; i++) {
      const scalar Yi = Y_[i][celli];
      c[i] = rhoi*Yi/specieThermo_[i].W();
    }
    const scalarField dcdt(omega(c, Ti, pi));
    for (label i=0; i<nSpecie_; i++) {
      RR_[i][celli] = dcdt[i]*specieThermo_[i].W();
    }
  }
}


template<class CompType, class ThermoType>
template<class DeltaTType>
mousse::scalar mousse::chemistryModel<CompType, ThermoType>::solve
(
  const DeltaTType& deltaT
)
{
  CompType::correct();
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
    this->thermo().rho()
  };
  const scalarField& T = this->thermo().T();
  const scalarField& p = this->thermo().p();
  scalarField c{nSpecie_};
  scalarField c0{nSpecie_};
  FOR_ALL(rho, celli) {
    scalar Ti = T[celli];
    if (Ti > Treact_) {
      const scalar rhoi = rho[celli];
      scalar pi = p[celli];
      for (label i=0; i<nSpecie_; i++) {
        c[i] = rhoi*Y_[i][celli]/specieThermo_[i].W();
        c0[i] = c[i];
      }
      // Initialise time progress
      scalar timeLeft = deltaT[celli];
      // Calculate the chemical source terms
      while (timeLeft > SMALL) {
        scalar dt = timeLeft;
        this->solve(c, Ti, pi, dt, this->deltaTChem_[celli]);
        timeLeft -= dt;
      }
      deltaTMin = min(this->deltaTChem_[celli], deltaTMin);
      for (label i=0; i<nSpecie_; i++) {
        RR_[i][celli] =
          (c[i] - c0[i])*specieThermo_[i].W()/deltaT[celli];
      }
    } else {
      for (label i=0; i<nSpecie_; i++) {
        RR_[i][celli] = 0;
      }
    }
  }

  return deltaTMin;
}


template<class CompType, class ThermoType>
mousse::scalar mousse::chemistryModel<CompType, ThermoType>::solve
(
  const scalar deltaT
)
{
  // Don't allow the time-step to change more than a factor of 2
  return
    min
    (
      this->solve<UniformField<scalar>>(UniformField<scalar>(deltaT)),
      2*deltaT
    );
}


template<class CompType, class ThermoType>
mousse::scalar mousse::chemistryModel<CompType, ThermoType>::solve
(
  const scalarField& deltaT
)
{
  return this->solve<scalarField>(deltaT);
}


template<class CompType, class ThermoType>
void mousse::chemistryModel<CompType, ThermoType>::solve
(
  scalarField &/*c*/,
  scalar& /*T*/,
  scalar& /*p*/,
  scalar& /*deltaT*/,
  scalar& /*subDeltaT*/
) const
{
  NOT_IMPLEMENTED
  (
    "chemistryModel::solve"
    "("
      "scalarField&, "
      "scalar&, "
      "scalar&, "
      "scalar&, "
      "scalar&"
    ") const"
  );
}

