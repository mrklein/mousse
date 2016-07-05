// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud_info.hpp"
#include "dictionary.hpp"
#include "kinematic_cloud.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(cloudInfo, 0);

}


// Protected Member Functions 
void mousse::cloudInfo::writeFileHeader(const label /*i*/)
{
  writeHeader(file(), "Cloud information");
  writeCommented(file(), "Time");
  writeTabbed(file(), "nParcels");
  writeTabbed(file(), "mass");
  file() << endl;
}


// Constructors 
mousse::cloudInfo::cloudInfo
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  functionObjectFile{obr, name},
  name_{name},
  obr_{obr},
  active_{true}
{
  read(dict);
}


// Destructor 
mousse::cloudInfo::~cloudInfo()
{}


// Member Functions 
void mousse::cloudInfo::read(const dictionary& dict)
{
  if (!active_)
    return;
  functionObjectFile::resetNames(dict.lookup("clouds"));
  Info << type() << " " << name_ << ": ";
  if (names().size()) {
    Info << "applying to clouds:" << nl;
    FOR_ALL(names(), i) {
      Info << "    " << names()[i] << nl;
    }
    Info << endl;
  } else {
    Info<< "no clouds to be processed" << nl << endl;
  }
}


void mousse::cloudInfo::execute()
{}


void mousse::cloudInfo::end()
{}


void mousse::cloudInfo::timeSet()
{}


void mousse::cloudInfo::write()
{
  if (!active_)
    return;
  functionObjectFile::write();
  FOR_ALL(names(), i) {
    const word& cloudName = names()[i];
    const kinematicCloud& cloud =
      obr_.lookupObject<kinematicCloud>(cloudName);
    label nParcels = returnReduce(cloud.nParcels(), sumOp<label>());
    scalar massInSystem =
      returnReduce(cloud.massInSystem(), sumOp<scalar>());
    if (Pstream::master()) {
      file(i)
        << obr_.time().value() << token::TAB
        << nParcels << token::TAB
        << massInSystem << endl;
    }
  }
}

