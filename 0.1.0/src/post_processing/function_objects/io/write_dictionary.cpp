// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_dictionary.hpp"
#include "dictionary.hpp"
#include "time.hpp"
#include "hash_set.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(writeDictionary, 0);

}


// Private Member Functions 
bool mousse::writeDictionary::tryDirectory
(
  const label dictI,
  const word& location,
  bool& firstDict
)
{
  IOobject dictIO
  {
    dictNames_[dictI],
    location,
    obr_,
    IOobject::MUST_READ,
    IOobject::NO_WRITE,
    false
  };
  if (dictIO.headerOk()) {
    IOdictionary dict{dictIO};
    if (dict.digest() != digests_[dictI]) {
      if (firstDict) {
        Info << type() << " " << name_ << " output:" << nl << endl;
        IOobject::writeDivider(Info);
        Info << endl;
        firstDict = false;
      }
      Info << dict.dictName() << dict << nl;
      IOobject::writeDivider(Info);
      digests_[dictI] = dict.digest();
    }
    return true;
  }
  return false;
}


// Constructors 
mousse::writeDictionary::writeDictionary
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  dictNames_{},
  digests_{}
{
  read(dict);
  execute();
}


// Destructor 
mousse::writeDictionary::~writeDictionary()
{}


// Member Functions 
void mousse::writeDictionary::read(const dictionary& dict)
{
  wordList dictNames{dict.lookup("dictNames")};
  HashSet<word> uniqueNames{dictNames};
  dictNames_ = uniqueNames.toc();
  digests_.setSize(dictNames_.size(), SHA1Digest());
  Info << type() << " " << name_ << ": monitoring dictionaries:" << nl;
  if (dictNames_.size()) {
    FOR_ALL(dictNames_, i) {
      Info << "    " << dictNames_[i] << endl;
    }
  } else {
    Info << "    none" << nl;
  }
  Info << endl;
}


void mousse::writeDictionary::execute()
{
  bool firstDict = true;
  FOR_ALL(dictNames_, i)
  {
    if (obr_.foundObject<dictionary>(dictNames_[i])) {
      const dictionary& dict = obr_.lookupObject<dictionary>(dictNames_[i]);
      if (dict.digest() != digests_[i]) {
        if (firstDict) {
          Info << type() << " " << name_ << " output:" << nl << endl;
          IOobject::writeDivider(Info);
          Info << endl;
          firstDict = false;
        }
        digests_[i] = dict.digest();
        Info << dict.dictName() << dict << nl;
        IOobject::writeDivider(Info);
        Info << endl;
      }
    } else {
      bool processed = tryDirectory(i, obr_.time().timeName(), firstDict);
      if (!processed) {
        processed = tryDirectory(i, obr_.time().constant(), firstDict);
      }
      if (!processed) {
        processed = tryDirectory(i, obr_.time().system(), firstDict);
      }
      if (!processed) {
        Info << "    Unable to locate dictionary " << dictNames_[i]
          << nl << endl;
      } else {
        Info << endl;
      }
    }
  }
}


void mousse::writeDictionary::end()
{
  execute();
}


void mousse::writeDictionary::timeSet()
{
  // do nothing
}


void mousse::writeDictionary::write()
{
  // do nothing
}

