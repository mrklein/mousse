// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud_solution.hpp"
#include "time.hpp"
// Constructors 
mousse::cloudSolution::cloudSolution
(
  const fvMesh& mesh,
  const dictionary& dict
)
:
  mesh_(mesh),
  dict_(dict),
  active_(dict.lookup("active")),
  transient_(false),
  calcFrequency_(1),
  maxCo_(0.3),
  iter_(1),
  trackTime_(0.0),
  coupled_(false),
  cellValueSourceCorrection_(false),
  maxTrackTime_(0.0),
  resetSourcesOnStartup_(true),
  schemes_()
{
  if (active_)
  {
    read();
  }
}
mousse::cloudSolution::cloudSolution
(
  const cloudSolution& cs
)
:
  mesh_(cs.mesh_),
  dict_(cs.dict_),
  active_(cs.active_),
  transient_(cs.transient_),
  calcFrequency_(cs.calcFrequency_),
  maxCo_(cs.maxCo_),
  iter_(cs.iter_),
  trackTime_(cs.trackTime_),
  coupled_(cs.coupled_),
  cellValueSourceCorrection_(cs.cellValueSourceCorrection_),
  maxTrackTime_(cs.maxTrackTime_),
  resetSourcesOnStartup_(cs.resetSourcesOnStartup_),
  schemes_(cs.schemes_)
{}
mousse::cloudSolution::cloudSolution
(
  const fvMesh& mesh
)
:
  mesh_(mesh),
  dict_(dictionary::null),
  active_(false),
  transient_(false),
  calcFrequency_(0),
  maxCo_(GREAT),
  iter_(0),
  trackTime_(0.0),
  coupled_(false),
  cellValueSourceCorrection_(false),
  maxTrackTime_(0.0),
  resetSourcesOnStartup_(false),
  schemes_()
{}
// Destructor 
mousse::cloudSolution::~cloudSolution()
{}
// Member Functions 
void mousse::cloudSolution::read()
{
  dict_.lookup("transient") >> transient_;
  dict_.lookup("coupled") >> coupled_;
  dict_.lookup("cellValueSourceCorrection") >> cellValueSourceCorrection_;
  dict_.readIfPresent("maxCo", maxCo_);
  if (steadyState())
  {
    dict_.lookup("calcFrequency") >> calcFrequency_;
    dict_.lookup("maxTrackTime") >> maxTrackTime_;
    if (coupled_)
    {
      dict_.subDict("sourceTerms").lookup("resetOnStartup")
        >> resetSourcesOnStartup_;
    }
  }
  if (coupled_)
  {
    const dictionary&
      schemesDict(dict_.subDict("sourceTerms").subDict("schemes"));
    wordList vars(schemesDict.toc());
    schemes_.setSize(vars.size());
    forAll(vars, i)
    {
      // read solution variable name
      schemes_[i].first() = vars[i];
      // set semi-implicit (1) explicit (0) flag
      Istream& is = schemesDict.lookup(vars[i]);
      const word scheme(is);
      if (scheme == "semiImplicit")
      {
        schemes_[i].second().first() = true;
      }
      else if (scheme == "explicit")
      {
        schemes_[i].second().first() = false;
      }
      else
      {
        FatalErrorIn("void cloudSolution::read()")
          << "Invalid scheme " << scheme << ". Valid schemes are "
          << "explicit and semiImplicit" << exit(FatalError);
      }
      // read under-relaxation factor
      is  >> schemes_[i].second().second();
    }
  }
}
mousse::scalar mousse::cloudSolution::relaxCoeff(const word& fieldName) const
{
  forAll(schemes_, i)
  {
    if (fieldName == schemes_[i].first())
    {
      return schemes_[i].second().second();
    }
  }
  FatalErrorIn("scalar cloudSolution::relaxCoeff(const word&) const")
    << "Field name " << fieldName << " not found in schemes"
    << abort(FatalError);
  return 1.0;
}
bool mousse::cloudSolution::semiImplicit(const word& fieldName) const
{
  forAll(schemes_, i)
  {
    if (fieldName == schemes_[i].first())
    {
      return schemes_[i].second().first();
    }
  }
  FatalErrorIn("bool cloudSolution::semiImplicit(const word&) const")
    << "Field name " << fieldName << " not found in schemes"
    << abort(FatalError);
  return false;
}
bool mousse::cloudSolution::solveThisStep() const
{
  return
    active_
  && (
      mesh_.time().outputTime()
    || (mesh_.time().timeIndex() % calcFrequency_ == 0)
    );
}
bool mousse::cloudSolution::canEvolve()
{
  if (transient_)
  {
    trackTime_ = mesh_.time().deltaTValue();
  }
  else
  {
    trackTime_ = maxTrackTime_;
  }
  return solveThisStep();
}
bool mousse::cloudSolution::output() const
{
  return active_ && mesh_.time().outputTime();
}
