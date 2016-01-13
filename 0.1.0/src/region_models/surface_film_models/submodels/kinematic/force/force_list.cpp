// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "force_list.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
// Constructors 
forceList::forceList(surfaceFilmModel& /*owner*/)
:
  PtrList<force>()
{}
forceList::forceList
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  PtrList<force>()
{
  const wordList models(dict.lookup("forces"));
  Info<< "    Selecting film force models" << endl;
  if (models.size() > 0)
  {
    this->setSize(models.size());
    FOR_ALL(models, i)
    {
      set(i, force::New(owner, dict, models[i]));
    }
  }
  else
  {
    Info<< "        none" << endl;
  }
}
// Destructor 
forceList::~forceList()
{}
// Member Functions 
tmp<fvVectorMatrix> forceList::correct(volVectorField& U)
{
  tmp<fvVectorMatrix> tResult
  (
    new fvVectorMatrix(U, dimForce/dimArea*dimVolume)
  );
  fvVectorMatrix& result = tResult();
  FOR_ALL(*this, i)
  {
    result += this->operator[](i).correct(U);
  }
  return tResult;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
