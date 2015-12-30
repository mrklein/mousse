// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "physico_chemical_constants.hpp"
using namespace mousse::constant;
// Member Functions 
template<class CloudType>
inline const mousse::ThermoCloud<CloudType>&
mousse::ThermoCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::ThermoCloud<CloudType>::constProps() const
{
  return constProps_;
}
template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::ThermoCloud<CloudType>::constProps()
{
  return constProps_;
}
template<class CloudType>
inline const mousse::SLGThermo& mousse::ThermoCloud<CloudType>::thermo() const
{
  return thermo_;
}
template<class CloudType>
inline const mousse::volScalarField& mousse::ThermoCloud<CloudType>::T() const
{
  return T_;
}
template<class CloudType>
inline const mousse::volScalarField& mousse::ThermoCloud<CloudType>::p() const
{
  return p_;
}
template<class CloudType>
inline const mousse::HeatTransferModel<mousse::ThermoCloud<CloudType> >&
mousse::ThermoCloud<CloudType>::heatTransfer() const
{
  return heatTransferModel_;
}
template<class CloudType>
inline const mousse::scalarIntegrationScheme&
mousse::ThermoCloud<CloudType>::TIntegrator() const
{
  return TIntegrator_;
}
template<class CloudType>
inline bool mousse::ThermoCloud<CloudType>::radiation() const
{
  return radiation_;
}
template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaP()
{
  if (!radiation_)
  {
    FatalErrorIn
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaP()"
    )   << "Radiation field requested, but radiation model not active"
      << abort(FatalError);
  }
  return radAreaP_();
}
template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaP() const
{
  if (!radiation_)
  {
    FatalErrorIn
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaP()"
    )   << "Radiation field requested, but radiation model not active"
      << abort(FatalError);
  }
  return radAreaP_();
}
template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radT4()
{
  if (!radiation_)
  {
    FatalErrorIn
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radT4()"
    )   << "Radiation field requested, but radiation model not active"
      << abort(FatalError);
  }
  return radT4_();
}
template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radT4() const
{
  if (!radiation_)
  {
    FatalErrorIn
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radT4()"
    )   << "Radiation field requested, but radiation model not active"
      << abort(FatalError);
  }
  return radT4_();
}
template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaPT4()
{
  if (!radiation_)
  {
    FatalErrorIn
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaPT4()"
    )   << "Radiation field requested, but radiation model not active"
      << abort(FatalError);
  }
  return radAreaPT4_();
}
template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::radAreaPT4() const
{
  if (!radiation_)
  {
    FatalErrorIn
    (
      "inline mousse::DimensionedField<mousse::scalar, mousse::volMesh> "
      "mousse::ThermoCloud<CloudType>::radAreaPT4()"
    )   << "Radiation field requested, but radiation model not active"
      << abort(FatalError);
  }
  return radAreaPT4_();
}
template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsTrans()
{
  return hsTrans_();
}
template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsTrans() const
{
  return hsTrans_();
}
template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsCoeff()
{
  return hsCoeff_();
}
template<class CloudType>
inline const mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ThermoCloud<CloudType>::hsCoeff() const
{
  return hsCoeff_();
}
template<class CloudType>
inline mousse::tmp<mousse::fvScalarMatrix>
mousse::ThermoCloud<CloudType>::Sh(volScalarField& hs) const
{
  if (debug)
  {
    Info<< "hsTrans min/max = " << min(hsTrans()).value() << ", "
      << max(hsTrans()).value() << nl
      << "hsCoeff min/max = " << min(hsCoeff()).value() << ", "
      << max(hsCoeff()).value() << endl;
  }
  if (this->solution().coupled())
  {
    if (this->solution().semiImplicit("h"))
    {
      const volScalarField Cp(thermo_.thermo().Cp());
      const DimensionedField<scalar, volMesh>
        Vdt(this->mesh().V()*this->db().time().deltaT());
      return
        hsTrans()/Vdt
       - fvm::SuSp(hsCoeff()/(Cp*Vdt), hs)
       + hsCoeff()/(Cp*Vdt)*hs;
    }
    else
    {
      tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(hs, dimEnergy/dimTime));
      fvScalarMatrix& fvm = tfvm();
      fvm.source() = -hsTrans()/(this->db().time().deltaT());
      return tfvm;
    }
  }
  return tmp<fvScalarMatrix>(new fvScalarMatrix(hs, dimEnergy/dimTime));
}
template<class CloudType>
inline mousse::tmp<mousse::volScalarField> mousse::ThermoCloud<CloudType>::Ep() const
{
  tmp<volScalarField> tEp
  (
    new volScalarField
    (
      IOobject
      (
        this->name() + ":radiation:Ep",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh(),
      dimensionedScalar("zero", dimMass/dimLength/pow3(dimTime), 0.0)
    )
  );
  if (radiation_)
  {
    scalarField& Ep = tEp().internalField();
    const scalar dt = this->db().time().deltaTValue();
    const scalarField& V = this->mesh().V();
    const scalar epsilon = constProps_.epsilon0();
    const scalarField& sumAreaPT4 = radAreaPT4_->field();
    Ep = sumAreaPT4*epsilon*physicoChemical::sigma.value()/V/dt;
  }
  return tEp;
}
template<class CloudType>
inline mousse::tmp<mousse::volScalarField> mousse::ThermoCloud<CloudType>::ap() const
{
  tmp<volScalarField> tap
  (
    new volScalarField
    (
      IOobject
      (
        this->name() + ":radiation:ap",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh(),
      dimensionedScalar("zero", dimless/dimLength, 0.0)
    )
  );
  if (radiation_)
  {
    scalarField& ap = tap().internalField();
    const scalar dt = this->db().time().deltaTValue();
    const scalarField& V = this->mesh().V();
    const scalar epsilon = constProps_.epsilon0();
    const scalarField& sumAreaP = radAreaP_->field();
    ap = sumAreaP*epsilon/V/dt;
  }
  return tap;
}
template<class CloudType>
inline mousse::tmp<mousse::volScalarField>
mousse::ThermoCloud<CloudType>::sigmap() const
{
  tmp<volScalarField> tsigmap
  (
    new volScalarField
    (
      IOobject
      (
        this->name() + ":radiation:sigmap",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh(),
      dimensionedScalar("zero", dimless/dimLength, 0.0)
    )
  );
  if (radiation_)
  {
    scalarField& sigmap = tsigmap().internalField();
    const scalar dt = this->db().time().deltaTValue();
    const scalarField& V = this->mesh().V();
    const scalar epsilon = constProps_.epsilon0();
    const scalar f = constProps_.f0();
    const scalarField& sumAreaP = radAreaP_->field();
    sigmap *= sumAreaP*(1.0 - f)*(1.0 - epsilon)/V/dt;
  }
  return tsigmap;
}
template<class CloudType>
inline mousse::scalar mousse::ThermoCloud<CloudType>::Tmax() const
{
  scalar T = -GREAT;
  scalar n = 0;
  forAllConstIter(typename ThermoCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    T = max(T, p.T());
    n++;
  }
  reduce(T, maxOp<scalar>());
  reduce(n, sumOp<label>());
  if (n > 0)
  {
    return T;
  }
  else
  {
    return 0.0;
  }
}
template<class CloudType>
inline mousse::scalar mousse::ThermoCloud<CloudType>::Tmin() const
{
  scalar T = GREAT;
  scalar n = 0;
  forAllConstIter(typename ThermoCloud<CloudType>, *this, iter)
  {
    const parcelType& p = iter();
    T = min(T, p.T());
    n++;
  }
  reduce(T, minOp<scalar>());
  reduce(n, sumOp<label>());
  if (n > 0)
  {
    return T;
  }
  else
  {
    return 0.0;
  }
}
