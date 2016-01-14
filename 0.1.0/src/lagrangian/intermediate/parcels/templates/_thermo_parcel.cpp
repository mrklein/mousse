// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_thermo_parcel.hpp"
#include "physico_chemical_constants.hpp"
using namespace mousse::constant;
//  Protected Member Functions
template<class ParcelType>
template<class TrackData>
void mousse::ThermoParcel<ParcelType>::setCellValues
(
  TrackData& td,
  const scalar dt,
  const label cellI
)
{
  ParcelType::setCellValues(td, dt, cellI);
  tetIndices tetIs = this->currentTetIndices();
  Cpc_ = td.CpInterp().interpolate(this->position(), tetIs);
  Tc_ = td.TInterp().interpolate(this->position(), tetIs);
  if (Tc_ < td.cloud().constProps().TMin())
  {
    if (debug)
    {
      WARNING_IN
      (
        "void mousse::ThermoParcel<ParcelType>::setCellValues"
        "("
          "TrackData&, "
          "const scalar, "
          "const label"
        ")"
      )   << "Limiting observed temperature in cell " << cellI << " to "
        << td.cloud().constProps().TMin() <<  nl << endl;
    }
    Tc_ = td.cloud().constProps().TMin();
  }
}
template<class ParcelType>
template<class TrackData>
void mousse::ThermoParcel<ParcelType>::cellValueSourceCorrection
(
  TrackData& td,
  const scalar /*dt*/,
  const label cellI
)
{
  this->Uc_ += td.cloud().UTrans()[cellI]/this->massCell(cellI);
  const scalar CpMean = td.CpInterp().psi()[cellI];
  Tc_ += td.cloud().hsTrans()[cellI]/(CpMean*this->massCell(cellI));
  if (Tc_ < td.cloud().constProps().TMin())
  {
    if (debug)
    {
      WARNING_IN
      (
        "void mousse::ThermoParcel<ParcelType>::cellValueSourceCorrection"
        "("
          "TrackData&, "
          "const scalar, "
          "const label"
        ")"
      )
      << "Limiting observed temperature in cell " << cellI << " to "
      << td.cloud().constProps().TMin() <<  nl << endl;
    }
    Tc_ = td.cloud().constProps().TMin();
  }
}
template<class ParcelType>
template<class TrackData>
void mousse::ThermoParcel<ParcelType>::calcSurfaceValues
(
  TrackData& td,
  const label /*cellI*/,
  const scalar T,
  scalar& Ts,
  scalar& rhos,
  scalar& mus,
  scalar& Pr,
  scalar& kappas
) const
{
  // Surface temperature using two thirds rule
  Ts = (2.0*T + Tc_)/3.0;
  if (Ts < td.cloud().constProps().TMin())
  {
    if (debug)
    {
      WARNING_IN
      (
        "void mousse::ThermoParcel<ParcelType>::calcSurfaceValues"
        "("
          "TrackData&, "
          "const label, "
          "const scalar, "
          "scalar&, "
          "scalar&, "
          "scalar&, "
          "scalar&, "
          "scalar&"
        ") const"
      )   << "Limiting parcel surface temperature to "
        << td.cloud().constProps().TMin() <<  nl << endl;
    }
    Ts = td.cloud().constProps().TMin();
  }
  // Assuming thermo props vary linearly with T for small d(T)
  const scalar TRatio = Tc_/Ts;
  rhos = this->rhoc_*TRatio;
  tetIndices tetIs = this->currentTetIndices();
  mus = td.muInterp().interpolate(this->position(), tetIs)/TRatio;
  kappas = td.kappaInterp().interpolate(this->position(), tetIs)/TRatio;
  Pr = Cpc_*mus/kappas;
  Pr = max(ROOTVSMALL, Pr);
}
template<class ParcelType>
template<class TrackData>
void mousse::ThermoParcel<ParcelType>::calc
(
  TrackData& td,
  const scalar dt,
  const label cellI
)
{
  // Define local properties at beginning of time step
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  const scalar np0 = this->nParticle_;
  const scalar mass0 = this->mass();
  // Store T for consistent radiation source
  const scalar T0 = this->T_;
  // Calc surface values
  // ~~~~~~~~~~~~~~~~~~~
  scalar Ts, rhos, mus, Pr, kappas;
  calcSurfaceValues(td, cellI, this->T_, Ts, rhos, mus, Pr, kappas);
  // Reynolds number
  scalar Re = this->Re(this->U_, this->d_, rhos, mus);
  // Sources
  // ~~~~~~~
  // Explicit momentum source for particle
  vector Su = vector::zero;
  // Linearised momentum source coefficient
  scalar Spu = 0.0;
  // Momentum transfer from the particle to the carrier phase
  vector dUTrans = vector::zero;
  // Explicit enthalpy source for particle
  scalar Sh = 0.0;
  // Linearised enthalpy source coefficient
  scalar Sph = 0.0;
  // Sensible enthalpy transfer from the particle to the carrier phase
  scalar dhsTrans = 0.0;
  // Heat transfer
  // ~~~~~~~~~~~~~
  // Sum Ni*Cpi*Wi of emission species
  scalar NCpW = 0.0;
  // Calculate new particle temperature
  this->T_ =
    this->calcHeatTransfer
    (
      td,
      dt,
      cellI,
      Re,
      Pr,
      kappas,
      NCpW,
      Sh,
      dhsTrans,
      Sph
    );
  // Motion
  // ~~~~~~
  // Calculate new particle velocity
  this->U_ =
    this->calcVelocity(td, dt, cellI, Re, mus, mass0, Su, dUTrans, Spu);
  //  Accumulate carrier phase source terms
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (td.cloud().solution().coupled())
  {
    // Update momentum transfer
    td.cloud().UTrans()[cellI] += np0*dUTrans;
    // Update momentum transfer coefficient
    td.cloud().UCoeff()[cellI] += np0*Spu;
    // Update sensible enthalpy transfer
    td.cloud().hsTrans()[cellI] += np0*dhsTrans;
    // Update sensible enthalpy coefficient
    td.cloud().hsCoeff()[cellI] += np0*Sph;
    // Update radiation fields
    if (td.cloud().radiation())
    {
      const scalar ap = this->areaP();
      const scalar T4 = pow4(T0);
      td.cloud().radAreaP()[cellI] += dt*np0*ap;
      td.cloud().radT4()[cellI] += dt*np0*T4;
      td.cloud().radAreaPT4()[cellI] += dt*np0*ap*T4;
    }
  }
}
template<class ParcelType>
template<class TrackData>
mousse::scalar mousse::ThermoParcel<ParcelType>::calcHeatTransfer
(
  TrackData& td,
  const scalar dt,
  const label /*cellI*/,
  const scalar Re,
  const scalar Pr,
  const scalar kappa,
  const scalar NCpW,
  const scalar Sh,
  scalar& dhsTrans,
  scalar& Sph
)
{
  if (!td.cloud().heatTransfer().active())
  {
    return T_;
  }
  const scalar d = this->d();
  const scalar rho = this->rho();
  // Calc heat transfer coefficient
  scalar htc = td.cloud().heatTransfer().htc(d, Re, Pr, kappa, NCpW);
  if (mag(htc) < ROOTVSMALL && !td.cloud().radiation())
  {
    return
      max
      (
        T_ + dt*Sh/(this->volume(d)*rho*Cp_),
        td.cloud().constProps().TMin()
      );
  }
  htc = max(htc, ROOTVSMALL);
  const scalar As = this->areaS(d);
  scalar ap = Tc_ + Sh/(As*htc);
  scalar bp = 6.0*(Sh/As + htc*(Tc_ - T_));
  if (td.cloud().radiation())
  {
    tetIndices tetIs = this->currentTetIndices();
    const scalar Gc = td.GInterp().interpolate(this->position(), tetIs);
    const scalar sigma = physicoChemical::sigma.value();
    const scalar epsilon = td.cloud().constProps().epsilon0();
    // Assume constant source
    scalar s = epsilon*(Gc/4.0 - sigma*pow4(T_));
    ap += s/htc;
    bp += 6.0*s;
  }
  bp /= rho*d*Cp_*(ap - T_) + ROOTVSMALL;
  // Integrate to find the new parcel temperature
  IntegrationScheme<scalar>::integrationResult Tres =
    td.cloud().TIntegrator().integrate(T_, dt, ap*bp, bp);
  scalar Tnew =
    min
    (
      max
      (
        Tres.value(),
        td.cloud().constProps().TMin()
      ),
      td.cloud().constProps().TMax()
    );
  Sph = dt*htc*As;
  dhsTrans += Sph*(Tres.average() - Tc_);
  return Tnew;
}
// Constructors 
template<class ParcelType>
mousse::ThermoParcel<ParcelType>::ThermoParcel
(
  const ThermoParcel<ParcelType>& p
)
:
  ParcelType(p),
  T_(p.T_),
  Cp_(p.Cp_),
  Tc_(p.Tc_),
  Cpc_(p.Cpc_)
{}
template<class ParcelType>
mousse::ThermoParcel<ParcelType>::ThermoParcel
(
  const ThermoParcel<ParcelType>& p,
  const polyMesh& mesh
)
:
  ParcelType(p, mesh),
  T_(p.T_),
  Cp_(p.Cp_),
  Tc_(p.Tc_),
  Cpc_(p.Cpc_)
{}
// IOStream operators 
#include "_thermo_parcel_io.cpp"
