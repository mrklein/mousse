// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solution.hpp"
#include "time.hpp"
// These are for old syntax compatibility:
#include "biccg.hpp"
#include "iccg.hpp"
#include "istring_stream.hpp"

// Static Data Members
namespace mousse {
DEFINE_DEBUG_SWITCH_WITH_NAME(solution, "solution", 0);
}

// List of sub-dictionaries to rewrite
//! \cond localScope
static const mousse::List<mousse::word> subDictNames
(
  mousse::IStringStream("(preconditioner smoother)")()
);

//! \endcond
// Private Member Functions 
void mousse::solution::read(const dictionary& dict)
{
  if (dict.found("cache"))
  {
    cache_ = dict.subDict("cache");
    caching_ = cache_.lookupOrDefault("active", true);
  }
  if (dict.found("relaxationFactors"))
  {
    const dictionary& relaxDict(dict.subDict("relaxationFactors"));
    if (relaxDict.found("fields") || relaxDict.found("equations"))
    {
      if (relaxDict.found("fields"))
      {
        fieldRelaxDict_ = relaxDict.subDict("fields");
      }
      if (relaxDict.found("equations"))
      {
        eqnRelaxDict_ = relaxDict.subDict("equations");
      }
    }
    else
    {
      // backwards compatibility
      fieldRelaxDict_.clear();
      const wordList entryNames(relaxDict.toc());
      FOR_ALL(entryNames, i)
      {
        const word& e = entryNames[i];
        scalar value = readScalar(relaxDict.lookup(e));
        if (e(0, 1) == "p")
        {
          fieldRelaxDict_.add(e, value);
        }
        else if (e.length() >= 3)
        {
          if (e(0, 3) == "rho")
          {
            fieldRelaxDict_.add(e, value);
          }
        }
      }
      eqnRelaxDict_ = relaxDict;
    }
    fieldRelaxDefault_ =
      fieldRelaxDict_.lookupOrDefault<scalar>("default", 0.0);
    eqnRelaxDefault_ =
      eqnRelaxDict_.lookupOrDefault<scalar>("default", 0.0);
    if (debug)
    {
      Info<< "relaxation factors:" << nl
        << "fields: " << fieldRelaxDict_ << nl
        << "equations: " << eqnRelaxDict_ << endl;
    }
  }
  if (dict.found("solvers"))
  {
    solvers_ = dict.subDict("solvers");
    upgradeSolverDict(solvers_);
  }
}


// Constructors 
mousse::solution::solution
(
  const objectRegistry& obr,
  const fileName& dictName
)
:
  IOdictionary
  {
    IOobject
    {
      dictName,
      obr.time().system(),
      obr,
      (
        obr.readOpt() == IOobject::MUST_READ
      || obr.readOpt() == IOobject::READ_IF_PRESENT
       ? IOobject::MUST_READ_IF_MODIFIED
       : obr.readOpt()
      ),
      IOobject::NO_WRITE
    }
  },
  cache_{dictionary::null},
  caching_{false},
  fieldRelaxDict_{dictionary::null},
  eqnRelaxDict_{dictionary::null},
  fieldRelaxDefault_{0},
  eqnRelaxDefault_{0},
  solvers_{dictionary::null}
{
  if
  (
    readOpt() == IOobject::MUST_READ
  || readOpt() == IOobject::MUST_READ_IF_MODIFIED
  || (readOpt() == IOobject::READ_IF_PRESENT && headerOk())
  )
  {
    read(solutionDict());
  }
}


// Member Functions 
mousse::label mousse::solution::upgradeSolverDict
(
  dictionary& dict,
  const bool verbose
)
{
  label nChanged = 0;
  // backward compatibility:
  // recast primitive entries into dictionary entries
  FOR_ALL_ITER(dictionary, dict, iter)
  {
    if (!iter().isDict())
    {
      Istream& is = iter().stream();
      word name(is);
      dictionary subdict;
      if (name == "BICCG")
      {
        // special treatment for very old syntax
        subdict = BICCG::solverDict(is);
      }
      else if (name == "ICCG")
      {
        // special treatment for very old syntax
        subdict = ICCG::solverDict(is);
      }
      else
      {
        subdict.add("solver", name);
        subdict <<= dictionary(is);
        // preconditioner and smoother entries can be
        // 1) primitiveEntry w/o settings,
        // 2) or a dictionaryEntry.
        // transform primitiveEntry with settings -> dictionaryEntry
        FOR_ALL(subDictNames, dictI)
        {
          const word& dictName = subDictNames[dictI];
          entry* ePtr = subdict.lookupEntryPtr(dictName,false,false);
          if (ePtr && !ePtr->isDict())
          {
            Istream& is = ePtr->stream();
            is >> name;
            if (!is.eof())
            {
              dictionary newDict;
              newDict.add(dictName, name);
              newDict <<= dictionary(is);
              subdict.set(dictName, newDict);
            }
          }
        }
      }
      // write out information to help people adjust to the new syntax
      if (verbose && Pstream::master())
      {
        Info<< "// using new solver syntax:\n"
          << iter().keyword() << subdict << endl;
      }
      // overwrite with dictionary entry
      dict.set(iter().keyword(), subdict);
      nChanged++;
    }
  }
  return nChanged;
}


bool mousse::solution::cache(const word& name) const
{
  if (caching_)
  {
    if (debug)
    {
      Info<< "Cache: find entry for " << name << endl;
    }
    return cache_.found(name);
  }
  else
  {
    return false;
  }
}


bool mousse::solution::relaxField(const word& name) const
{
  if (debug)
  {
    Info<< "Field relaxation factor for " << name
      << " is " << (fieldRelaxDict_.found(name) ? "set" : "unset")
      << endl;
  }
  return fieldRelaxDict_.found(name) || fieldRelaxDict_.found("default");
}


bool mousse::solution::relaxEquation(const word& name) const
{
  if (debug)
  {
    Info<< "Find equation relaxation factor for " << name << endl;
  }
  return eqnRelaxDict_.found(name) || eqnRelaxDict_.found("default");
}


mousse::scalar mousse::solution::fieldRelaxationFactor(const word& name) const
{
  if (debug)
  {
    Info<< "Lookup variable relaxation factor for " << name << endl;
  }
  if (fieldRelaxDict_.found(name))
  {
    return readScalar(fieldRelaxDict_.lookup(name));
  }
  else if (fieldRelaxDefault_ > SMALL)
  {
    return fieldRelaxDefault_;
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "mousse::solution::fieldRelaxationFactor(const word&)",
      fieldRelaxDict_
    )
    << "Cannot find variable relaxation factor for '" << name
    << "' or a suitable default value."
    << exit(FatalIOError);
    return 0;
  }
}


mousse::scalar mousse::solution::equationRelaxationFactor(const word& name) const
{
  if (debug)
  {
    Info<< "Lookup equation relaxation factor for " << name << endl;
  }
  if (eqnRelaxDict_.found(name))
  {
    return readScalar(eqnRelaxDict_.lookup(name));
  }
  else if (eqnRelaxDefault_ > SMALL)
  {
    return eqnRelaxDefault_;
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "mousse::solution::eqnRelaxationFactor(const word&)",
      eqnRelaxDict_
    )
    << "Cannot find equation relaxation factor for '" << name
    << "' or a suitable default value."
    << exit(FatalIOError);
    return 0;
  }
}


const mousse::dictionary& mousse::solution::solutionDict() const
{
  if (found("select"))
  {
    return subDict(word(lookup("select")));
  }
  else
  {
    return *this;
  }
}


const mousse::dictionary& mousse::solution::solverDict(const word& name) const
{
  if (debug)
  {
    INFO_IN("solution::solverDict(const word&)")
      << "Lookup solver for " << name << endl;
  }
  return solvers_.subDict(name);
}


const mousse::dictionary& mousse::solution::solver(const word& name) const
{
  if (debug)
  {
    INFO_IN("solution::solver(const word&)")
      << "Lookup solver for " << name << endl;
  }
  return solvers_.subDict(name);
}


bool mousse::solution::read()
{
  if (regIOobject::read())
  {
    read(solutionDict());
    return true;
  }
  else
  {
    return false;
  }
}
