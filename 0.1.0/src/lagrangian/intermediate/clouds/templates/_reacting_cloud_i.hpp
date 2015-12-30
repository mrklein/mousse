// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class CloudType>
inline const mousse::ReactingCloud<CloudType>&
mousse::ReactingCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::ReactingCloud<CloudType>::constProps() const
{
  return constProps_;
}
template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::ReactingCloud<CloudType>::constProps()
{
  return constProps_;
}
template<class CloudType>
inline const mousse::CompositionModel<mousse::ReactingCloud<CloudType> >&
mousse::ReactingCloud<CloudType>::composition() const
{
  return compositionModel_;
}
template<class CloudType>
inline const mousse::PhaseChangeModel<mousse::ReactingCloud<CloudType> >&
mousse::ReactingCloud<CloudType>::phaseChange() const
{
  return phaseChangeModel_;
}
template<class CloudType>
inline mousse::PhaseChangeModel<mousse::ReactingCloud<CloudType> >&
mousse::ReactingCloud<CloudType>::phaseChange()
{
  return phaseChangeModel_();
}
template<class CloudType>
inline mousse::DimensionedField<mousse::scalar, mousse::volMesh>&
mousse::ReactingCloud<CloudType>::rhoTrans(const label i)
{
  return rhoTrans_[i];
}
template<class CloudType>
inline
const mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >&
mousse::ReactingCloud<CloudType>::rhoTrans() const
{
  return rhoTrans_;
}
template<class CloudType>
inline mousse::PtrList<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >&
mousse::ReactingCloud<CloudType>::rhoTrans()
{
  return rhoTrans_;
}
template<class CloudType>
inline mousse::tmp<mousse::fvScalarMatrix> mousse::ReactingCloud<CloudType>::SYi
(
  const label i,
  volScalarField& Yi
) const
{
  if (this->solution().coupled())
  {
    if (this->solution().semiImplicit("Yi"))
    {
      tmp<volScalarField> trhoTrans
      (
        new volScalarField
        (
          IOobject
          (
            this->name() + ":rhoTrans",
            this->db().time().timeName(),
            this->db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
          ),
          this->mesh(),
          dimensionedScalar("zero", dimMass/dimTime/dimVolume, 0.0)
        )
      );
      volScalarField& sourceField = trhoTrans();
      sourceField.internalField() =
        rhoTrans_[i]/(this->db().time().deltaTValue()*this->mesh().V());
      const dimensionedScalar YiSMALL("YiSMALL", dimless, SMALL);
      return
        fvm::Sp(neg(sourceField)*sourceField/(Yi + YiSMALL), Yi)
       + pos(sourceField)*sourceField;
    }
    else
    {
      tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(Yi, dimMass/dimTime));
      fvScalarMatrix& fvm = tfvm();
      fvm.source() = -rhoTrans_[i]/this->db().time().deltaTValue();
      return tfvm;
    }
  }
  return tmp<fvScalarMatrix>(new fvScalarMatrix(Yi, dimMass/dimTime));
}
template<class CloudType>
inline mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::ReactingCloud<CloudType>::Srho(const label i) const
{
  tmp<DimensionedField<scalar, volMesh> > tRhoi
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        this->name() + ":rhoTrans",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh(),
      dimensionedScalar
      (
        "zero",
        rhoTrans_[0].dimensions()/dimTime/dimVolume,
        0.0
      )
    )
  );
  if (this->solution().coupled())
  {
    scalarField& rhoi = tRhoi();
    rhoi = rhoTrans_[i]/(this->db().time().deltaTValue()*this->mesh().V());
  }
  return tRhoi;
}
template<class CloudType>
inline mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh> >
mousse::ReactingCloud<CloudType>::Srho() const
{
  tmp<DimensionedField<scalar, volMesh> > trhoTrans
  (
    new DimensionedField<scalar, volMesh>
    (
      IOobject
      (
        this->name() + ":rhoTrans",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      this->mesh(),
      dimensionedScalar
      (
        "zero",
        rhoTrans_[0].dimensions()/dimTime/dimVolume,
        0.0
      )
    )
  );
  if (this->solution().coupled())
  {
    scalarField& sourceField = trhoTrans();
    forAll(rhoTrans_, i)
    {
      sourceField += rhoTrans_[i];
    }
    sourceField /= this->db().time().deltaTValue()*this->mesh().V();
  }
  return trhoTrans;
}
template<class CloudType>
inline mousse::tmp<mousse::fvScalarMatrix>
mousse::ReactingCloud<CloudType>::Srho(volScalarField& rho) const
{
  if (this->solution().coupled())
  {
    tmp<volScalarField> trhoTrans
    (
      new volScalarField
      (
        IOobject
        (
          this->name() + ":rhoTrans",
          this->db().time().timeName(),
          this->db(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        ),
        this->mesh(),
        dimensionedScalar("zero", dimMass/dimTime/dimVolume, 0.0)
      )
    );
    scalarField& sourceField = trhoTrans();
    if (this->solution().semiImplicit("rho"))
    {
      forAll(rhoTrans_, i)
      {
        sourceField += rhoTrans_[i];
      }
      sourceField /= this->db().time().deltaTValue()*this->mesh().V();
      return fvm::SuSp(trhoTrans()/rho, rho);
    }
    else
    {
      tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(rho, dimMass/dimTime));
      fvScalarMatrix& fvm = tfvm();
      forAll(rhoTrans_, i)
      {
        sourceField += rhoTrans_[i];
      }
      fvm.source() = -trhoTrans()/this->db().time().deltaT();
      return tfvm;
    }
  }
  return tmp<fvScalarMatrix>(new fvScalarMatrix(rho, dimMass/dimTime));
}
