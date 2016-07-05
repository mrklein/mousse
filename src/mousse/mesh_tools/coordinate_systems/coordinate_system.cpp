// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "iostream.hpp"
#include "axes_rotation.hpp"
#include "coordinate_system.hpp"
#include "coordinate_systems.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(coordinateSystem, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(coordinateSystem, dictionary);

}


// Constructors 
mousse::coordinateSystem::coordinateSystem()
:
  name_{},
  note_{},
  origin_{point::zero},
  R_{new axesRotation(sphericalTensor::I)}
{}


mousse::coordinateSystem::coordinateSystem
(
  const word& name,
  const coordinateSystem& cs
)
:
  name_{name},
  note_{},
  origin_{cs.origin_},
  R_{const_cast<coordinateRotation*>(&cs.R())}
{}


mousse::coordinateSystem::coordinateSystem
(
  const word& name,
  const point& origin,
  const coordinateRotation& cr
)
:
  name_{name},
  note_{},
  origin_{origin},
  R_{const_cast<coordinateRotation*>(&cr)}
{}


mousse::coordinateSystem::coordinateSystem
(
  const word& name,
  const point& origin,
  const vector& axis,
  const vector& dirn
)
:
  name_{name},
  note_{},
  origin_{origin},
  R_{new axesRotation{axis, dirn}}
{}


mousse::coordinateSystem::coordinateSystem
(
  const word& name,
  const dictionary& dict
)
:
  name_{name},
  note_{},
  origin_{point::zero},
  R_{}
{
  init(dict);
}


mousse::coordinateSystem::coordinateSystem(const dictionary& dict)
:
  name_{},
  note_{},
  origin_{point::zero},
  R_{}
{
  init(dict);
}


mousse::coordinateSystem::coordinateSystem
(
  const objectRegistry& obr,
  const dictionary& dict
)
:
  name_{},
  note_{},
  origin_{point::zero},
  R_{}
{
  const entry* entryPtr = dict.lookupEntryPtr(typeName_(), false, false);
  // non-dictionary entry is a lookup into global coordinateSystems
  if (entryPtr && !entryPtr->isDict()) {
    keyType key(entryPtr->stream());
    const coordinateSystems& lst = coordinateSystems::New(obr);
    const label index = lst.findIndex(key);
    if (debug) {
      Info << "coordinateSystem::coordinateSystem"
        "(const objectRegistry&, const dictionary&):"
        << nl << "using global coordinate system: "
        << key << "=" << index << endl;
    }
    if (index < 0) {
      FATAL_ERROR_IN
      (
        "coordinateSystem::coordinateSystem"
        "(const objectRegistry&, const dictionary&):"
      )
      << "could not find coordinate system: " << key << nl
      << "available coordinate systems: " << lst.toc() << nl << nl
      << exit(FatalError);
    }
    // copy coordinateSystem, but assign the name as the typeName
    // to avoid strange things in writeDict()
    operator=(lst[index]);
    name_ = typeName_();
  } else {
    init(dict, obr);
  }
}


mousse::coordinateSystem::coordinateSystem(Istream& is)
:
  name_{is},
  note_{},
  origin_{point::zero},
  R_{}
{
  dictionary dict(is);
  init(dict);
}


// Destructor 
mousse::coordinateSystem::~coordinateSystem()
{}


// Member Functions 
mousse::dictionary mousse::coordinateSystem::dict(bool ignoreType) const
{
  dictionary dict;
  dict.add("name", name_);
  // only write type for derived types
  if (!ignoreType && type() != typeName_()) {
    dict.add("type", type());
  }
  // The note entry is optional
  if (note_.size()) {
    dict.add("note", note_);
  }
  dict.add("origin", origin_);
  dict.add("e1", R_->e1());
  dict.add("e3", R_->e3());
  return dict;
}


mousse::vector mousse::coordinateSystem::localToGlobal
(
  const vector& local,
  bool translate
) const
{
  if (translate) {
    return (R_->transform(local)) + origin_;
  } else {
    return R_->transform(local);
  }
}


mousse::tmp<mousse::vectorField> mousse::coordinateSystem::localToGlobal
(
  const vectorField& local,
  bool translate
) const
{
  if (translate) {
    return (R_->transform(local)) + origin_;
  } else {
    return R_->transform(local);
  }
}


mousse::vector mousse::coordinateSystem::globalToLocal
(
  const vector& global,
  bool translate
) const
{
  if (translate) {
    return R_->invTransform(global - origin_);
  } else {
    return R_->invTransform(global);
  }
}


mousse::tmp<mousse::vectorField> mousse::coordinateSystem::globalToLocal
(
  const vectorField& global,
  bool translate
) const
{
  if (translate) {
    return R_->invTransform(global - origin_);
  } else {
    return R_->invTransform(global);
  }
}


void mousse::coordinateSystem::clear()
{
  note_.clear();
  origin_ = point::zero;
  R_->clear();
}


void mousse::coordinateSystem::write(Ostream& os) const
{
  os << type() << " origin: " << origin() << nl;
  R_->write(os);
}


void mousse::coordinateSystem::writeDict(Ostream& os, bool subDict) const
{
  if (subDict) {
    os << indent << name_ << nl
      << indent << token::BEGIN_BLOCK << incrIndent << nl;
  }
  os.writeKeyword("type") << type() << token::END_STATEMENT << nl;
  // The note entry is optional
  if (note_.size()) {
    os.writeKeyword("note") << note_ << token::END_STATEMENT << nl;
  }
  os.writeKeyword("origin") << origin_ << token::END_STATEMENT << nl;
  R_->write(os);
  if (subDict) {
    os << decrIndent << indent << token::END_BLOCK << endl;
  }
}


// Member Operators 
void mousse::coordinateSystem::init(const dictionary& rhs)
{
  rhs.lookup("origin") >> origin_;
  note_.clear();
  rhs.readIfPresent("note", note_);
  R_.reset(coordinateRotation::New(rhs.subDict("coordinateRotation")).ptr());
}


void mousse::coordinateSystem::init
(
  const dictionary& rhs,
  const objectRegistry& obr
)
{
  if (debug) {
    Pout << "coordinateSystem::operator="
            "("
            "  const dictionary&, "
            "  const objectRegistry&"
            ") : "
      << "assign from " << rhs << endl;
  }
  rhs.lookup("origin") >> origin_;
  // The note entry is optional
  note_.clear();
  rhs.readIfPresent("note", note_);
  R_.reset
  (
    coordinateRotation::New(rhs.subDict("coordinateRotation"), obr).ptr()
  );
}


// Friend Operators 
bool mousse::operator!=(const coordinateSystem& a, const coordinateSystem& b)
{
  return
  (
    a.origin() != b.origin()
    || a.R().R() != b.R().R()
    || a.type() != b.type()
  );
}


// Friend Functions 
mousse::Ostream& mousse::operator<<(Ostream& os, const coordinateSystem& cs)
{
  cs.write(os);
  os.check("Ostream& operator<<(Ostream&, const coordinateSystem&");
  return os;
}

