// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "porosity_model_list.hpp"
#include "vol_fields.hpp"
// Constructors 
mousse::porosityModelList::porosityModelList
(
  const fvMesh& mesh,
  const dictionary& dict
)
:
  PtrList<porosityModel>{},
  mesh_{mesh}
{
  reset(dict);
  active(true);
}
// Destructor 
mousse::porosityModelList::~porosityModelList()
{}
// Member Functions 
bool mousse::porosityModelList::active(const bool warn) const
{
  bool a = false;
  FOR_ALL(*this, i)
  {
    a = a || this->operator[](i).active();
  }
  if (warn && this->size() && !a)
  {
    Info<< "No porosity models active" << endl;
  }
  return a;
}
void mousse::porosityModelList::reset(const dictionary& dict)
{
  label count = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter)
  {
    if (iter().isDict())
    {
      count++;
    }
  }
  this->setSize(count);
  label i = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter)
  {
    if (iter().isDict())
    {
      const word& name = iter().keyword();
      const dictionary& modelDict = iter().dict();
      this->set
      (
        i++,
        porosityModel::New(name, mesh_, modelDict)
      );
    }
  }
}
bool mousse::porosityModelList::read(const dictionary& dict)
{
  bool allOk = true;
  FOR_ALL(*this, i)
  {
    porosityModel& pm = this->operator[](i);
    bool ok = pm.read(dict.subDict(pm.name()));
    allOk = (allOk && ok);
  }
  return allOk;
}
bool mousse::porosityModelList::writeData(Ostream& os) const
{
  FOR_ALL(*this, i)
  {
    os  << nl;
    this->operator[](i).writeData(os);
  }
  return os.good();
}
void mousse::porosityModelList::addResistance
(
  fvVectorMatrix& UEqn
)
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).addResistance(UEqn);
  }
}
void mousse::porosityModelList::addResistance
(
  fvVectorMatrix& UEqn,
  const volScalarField& rho,
  const volScalarField& mu
)
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).addResistance(UEqn, rho, mu);
  }
}
void mousse::porosityModelList::addResistance
(
  const fvVectorMatrix& UEqn,
  volTensorField& AU,
  bool correctAUprocBC         
)
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).addResistance(UEqn, AU, correctAUprocBC);
  }
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const porosityModelList& models
)
{
  models.writeData(os);
  return os;
}
