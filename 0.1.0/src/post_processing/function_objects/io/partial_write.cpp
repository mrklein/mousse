// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "partial_write.hpp"
#include "dictionary.hpp"
#include "time.hpp"
#include "ioobject_list.hpp"
#include "poly_mesh.hpp"
#include "cloud.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(partialWrite, 0);

}


// Constructors 
mousse::partialWrite::partialWrite
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr}
{
  read(dict);
}


// Destructor 
mousse::partialWrite::~partialWrite()
{}


// Member Functions 
void mousse::partialWrite::read(const dictionary& dict)
{
  dict.lookup("objectNames") >> objectNames_;
  dict.lookup("writeInterval") >> writeInterval_;
  writeInstance_ = 0;
  Info << type() << " " << name() << ":" << nl
    << "    dumping every " << writeInterval_
    << " th outputTime : " << nl << endl ;
  FOR_ALL_CONST_ITER(HashSet<word>, objectNames_, iter) {
    Info << ' ' << iter.key();
  }
  if (writeInterval_ < 1) {
    FATAL_IO_ERROR_IN("partialWrite::read(const dictionary&)", dict)
      << "Illegal value for writeInterval " << writeInterval_
      << ". It should be >= 1."
      << exit(FatalIOError);
  }
  // Clear out any previously loaded fields
  vsf_.clear();
  vvf_.clear();
  vSpheretf_.clear();
  vSymmtf_.clear();
  vtf_.clear();
  ssf_.clear();
  svf_.clear();
  sSpheretf_.clear();
  sSymmtf_.clear();
  stf_.clear();
  FOR_ALL_CONST_ITER(HashSet<word>, objectNames_, iter) {
    loadField<scalar>(iter.key(), vsf_, ssf_);
    loadField<vector>(iter.key(), vvf_, svf_);
    loadField<sphericalTensor>(iter.key(), vSpheretf_, sSpheretf_);
    loadField<symmTensor>(iter.key(), vSymmtf_, sSymmtf_);
    loadField<tensor>(iter.key(), vtf_, stf_);
  }
}


void mousse::partialWrite::execute()
{}


void mousse::partialWrite::end()
{
  // Do nothing - only valid on write
}


void mousse::partialWrite::timeSet()
{
  if (!obr_.time().outputTime())
    return;
  writeInstance_++;
  if (writeInstance_ == writeInterval_) {
    // Next overall dump corresponds to partial write. Change
    // write options to AUTO_WRITE
    writeInstance_ = 0;
    changeWriteOptions<scalar>(vsf_, ssf_, IOobject::AUTO_WRITE);
    changeWriteOptions<vector>(vvf_, svf_, IOobject::AUTO_WRITE);
    changeWriteOptions<sphericalTensor>
    (
      vSpheretf_,
      sSpheretf_,
      IOobject::AUTO_WRITE
    );
    changeWriteOptions<symmTensor>
    (
      vSymmtf_,
      sSymmtf_,
      IOobject::AUTO_WRITE
    );
    changeWriteOptions<tensor>(vtf_, stf_, IOobject::AUTO_WRITE);
  } else {
    changeWriteOptions<scalar>(vsf_, ssf_, IOobject::NO_WRITE);
    changeWriteOptions<vector>(vvf_, svf_, IOobject::NO_WRITE);
    changeWriteOptions<sphericalTensor>
    (
      vSpheretf_,
      sSpheretf_,
      IOobject::NO_WRITE
    );
    changeWriteOptions<symmTensor>
    (
      vSymmtf_,
      sSymmtf_,
      IOobject::NO_WRITE
    );
    changeWriteOptions<tensor>(vtf_, stf_, IOobject::NO_WRITE);
  }
}


void mousse::partialWrite::write()
{
  // Do nothing. The fields get written through the
  // standard dump
}

