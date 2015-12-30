// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mrf_zone_list.hpp"
#include "vol_fields.hpp"
#include "fixed_value_fvs_patch_fields.hpp"
// Constructors 
mousse::MRFZoneList::MRFZoneList
(
  const fvMesh& mesh,
  const dictionary& dict
)
:
  PtrList<MRFZone>(),
  mesh_(mesh)
{
  reset(dict);
  active(true);
}
// Destructor 
mousse::MRFZoneList::~MRFZoneList()
{}
// Member Functions 
bool mousse::MRFZoneList::active(const bool warn) const
{
  bool a = false;
  forAll(*this, i)
  {
    a = a || this->operator[](i).active();
  }
  if (warn && this->size() && !a)
  {
    Info<< "    No MRF zones active" << endl;
  }
  return a;
}
void mousse::MRFZoneList::reset(const dictionary& dict)
{
  label count = 0;
  forAllConstIter(dictionary, dict, iter)
  {
    if (iter().isDict())
    {
      count++;
    }
  }
  this->setSize(count);
  label i = 0;
  forAllConstIter(dictionary, dict, iter)
  {
    if (iter().isDict())
    {
      const word& name = iter().keyword();
      const dictionary& modelDict = iter().dict();
      Info<< "    creating MRF zone: " << name << endl;
      this->set
      (
        i++,
        new MRFZone(name, mesh_, modelDict)
      );
    }
  }
}
bool mousse::MRFZoneList::read(const dictionary& dict)
{
  bool allOk = true;
  forAll(*this, i)
  {
    MRFZone& pm = this->operator[](i);
    bool ok = pm.read(dict.subDict(pm.name()));
    allOk = (allOk && ok);
  }
  return allOk;
}
bool mousse::MRFZoneList::writeData(Ostream& os) const
{
  forAll(*this, i)
  {
    os  << nl;
    this->operator[](i).writeData(os);
  }
  return os.good();
}
void mousse::MRFZoneList::addAcceleration
(
  const volVectorField& U,
  volVectorField& ddtU
) const
{
  forAll(*this, i)
  {
    operator[](i).addCoriolis(U, ddtU);
  }
}
void mousse::MRFZoneList::addAcceleration(fvVectorMatrix& UEqn) const
{
  forAll(*this, i)
  {
    operator[](i).addCoriolis(UEqn);
  }
}
void mousse::MRFZoneList::addAcceleration
(
  const volScalarField& rho,
  fvVectorMatrix& UEqn
) const
{
  forAll(*this, i)
  {
    operator[](i).addCoriolis(rho, UEqn);
  }
}
mousse::tmp<mousse::volVectorField> mousse::MRFZoneList::DDt
(
  const volVectorField& U
) const
{
  tmp<volVectorField> tacceleration
  (
    new volVectorField
    (
      IOobject
      (
        "MRFZoneList:acceleration",
        U.mesh().time().timeName(),
        U.mesh()
      ),
      U.mesh(),
      dimensionedVector("0", U.dimensions()/dimTime, vector::zero)
    )
  );
  volVectorField& acceleration = tacceleration();
  forAll(*this, i)
  {
    operator[](i).addCoriolis(U, acceleration);
  }
  return tacceleration;
}
mousse::tmp<mousse::volVectorField> mousse::MRFZoneList::DDt
(
  const volScalarField& rho,
  const volVectorField& U
) const
{
  return rho*DDt(U);
}
void mousse::MRFZoneList::makeRelative(volVectorField& U) const
{
  forAll(*this, i)
  {
    operator[](i).makeRelative(U);
  }
}
void mousse::MRFZoneList::makeRelative(surfaceScalarField& phi) const
{
  forAll(*this, i)
  {
    operator[](i).makeRelative(phi);
  }
}
mousse::tmp<mousse::surfaceScalarField> mousse::MRFZoneList::relative
(
  const tmp<surfaceScalarField>& phi
) const
{
  tmp<surfaceScalarField> rphi(phi.ptr());
  makeRelative(rphi());
  return rphi;
}
mousse::tmp<mousse::FieldField<mousse::fvsPatchField, mousse::scalar> >
mousse::MRFZoneList::relative
(
  const tmp<FieldField<fvsPatchField, scalar> >& phi
) const
{
  tmp<FieldField<fvsPatchField, scalar> > rphi(phi.ptr());
  forAll(*this, i)
  {
    operator[](i).makeRelative(rphi());
  }
  return rphi;
}
void mousse::MRFZoneList::makeRelative
(
  const surfaceScalarField& rho,
  surfaceScalarField& phi
) const
{
  forAll(*this, i)
  {
    operator[](i).makeRelative(rho, phi);
  }
}
void mousse::MRFZoneList::makeAbsolute(volVectorField& U) const
{
  forAll(*this, i)
  {
    operator[](i).makeAbsolute(U);
  }
}
void mousse::MRFZoneList::makeAbsolute(surfaceScalarField& phi) const
{
  forAll(*this, i)
  {
    operator[](i).makeAbsolute(phi);
  }
}
mousse::tmp<mousse::surfaceScalarField> mousse::MRFZoneList::absolute
(
  const tmp<surfaceScalarField>& phi
) const
{
  tmp<surfaceScalarField> rphi(phi.ptr());
  makeAbsolute(rphi());
  return rphi;
}
void mousse::MRFZoneList::makeAbsolute
(
  const surfaceScalarField& rho,
  surfaceScalarField& phi
) const
{
  forAll(*this, i)
  {
    operator[](i).makeAbsolute(rho, phi);
  }
}
void mousse::MRFZoneList::correctBoundaryVelocity(volVectorField& U) const
{
  forAll(*this, i)
  {
    operator[](i).correctBoundaryVelocity(U);
  }
}
void mousse::MRFZoneList::correctBoundaryFlux
(
  const volVectorField& U,
  surfaceScalarField& phi
) const
{
  FieldField<fvsPatchField, scalar> phibf
  (
    relative(mesh_.Sf().boundaryField() & U.boundaryField())
  );
  forAll(mesh_.boundary(), patchi)
  {
    if
    (
      isA<fixedValueFvsPatchScalarField>(phi.boundaryField()[patchi])
    )
    {
      phi.boundaryField()[patchi] == phibf[patchi];
    }
  }
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const MRFZoneList& models
)
{
  models.writeData(os);
  return os;
}
