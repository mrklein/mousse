// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_option_list.hpp"
#include "surface_fields.hpp"


// Static Data Members
namespace mousse {
namespace fv {

DEFINE_TYPE_NAME_AND_DEBUG(optionList, 0);

}
}


// Protected Member Functions 
const mousse::dictionary& mousse::fv::optionList::optionsDict
(
  const dictionary& dict
) const
{
  if (dict.found("options")) {
    return dict.subDict("options");
  }
  return dict;
}


bool mousse::fv::optionList::readOptions(const dictionary& dict)
{
  checkTimeIndex_ = mesh_.time().timeIndex() + 2;
  bool allOk = true;
  FOR_ALL(*this, i) {
    option& bs = this->operator[](i);
    bool ok = bs.read(dict.subDict(bs.name()));
    allOk = (allOk && ok);
  }
  return allOk;
}


void mousse::fv::optionList::checkApplied() const
{
  if (mesh_.time().timeIndex() == checkTimeIndex_) {
    FOR_ALL(*this, i) {
      const option& bs = this->operator[](i);
      bs.checkApplied();
    }
  }
}


// Constructors 
mousse::fv::optionList::optionList(const fvMesh& mesh, const dictionary& dict)
:
  PtrList<option>{},
  mesh_{mesh},
  checkTimeIndex_{mesh_.time().startTimeIndex() + 2}
{
  reset(optionsDict(dict));
}


mousse::fv::optionList::optionList(const fvMesh& mesh)
:
  PtrList<option>{},
  mesh_{mesh},
  checkTimeIndex_{mesh_.time().startTimeIndex() + 2}
{}


// Member Functions 
void mousse::fv::optionList::reset(const dictionary& dict)
{
  // Count number of active fvOptions
  label count = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter) {
    if (iter().isDict()) {
      count++;
    }
  }
  this->setSize(count);
  label i = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter) {
    if (iter().isDict()) {
      const word& name = iter().keyword();
      const dictionary& sourceDict = iter().dict();
      this->set(i++, option::New(name, sourceDict, mesh_));
    }
  }
}


bool mousse::fv::optionList::read(const dictionary& dict)
{
  return readOptions(optionsDict(dict));
}


bool mousse::fv::optionList::writeData(Ostream& os) const
{
  // Write list contents
  FOR_ALL(*this, i) {
    os << nl;
    this->operator[](i).writeHeader(os);
    this->operator[](i).writeData(os);
    this->operator[](i).writeFooter(os);
  }
  // Check state of IOstream
  return os.good();
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const fv::optionList& options)
{
  options.writeData(os);
  return os;
}

