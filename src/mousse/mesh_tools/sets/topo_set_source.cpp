// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "topo_set_source.hpp"
#include "poly_mesh.hpp"
#include "topo_set.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(topoSetSource, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(topoSetSource, word);
DEFINE_RUN_TIME_SELECTION_TABLE(topoSetSource, istream);

template<>
const char* mousse::NamedEnum
<
  mousse::topoSetSource::setAction,
  8
>::names[] =
{
  "clear",
  "new",
  "invert",
  "add",
  "delete",
  "subset",
  "list",
  "remove"
};

}

mousse::HashTable<mousse::string>* mousse::topoSetSource::usageTablePtr_ = NULL;

const mousse::NamedEnum<mousse::topoSetSource::setAction, 8>
  mousse::topoSetSource::actionNames_;

const mousse::string mousse::topoSetSource::illegalSource_
(
  "Illegal topoSetSource name"
);


mousse::autoPtr<mousse::topoSetSource> mousse::topoSetSource::New
(
  const word& topoSetSourceType,
  const polyMesh& mesh,
  const dictionary& dict
)
{
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(topoSetSourceType);
  if (cstrIter == wordConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "topoSetSource::New(const word&, "
      "const polyMesh&, const dictionary&)"
    )
    << "Unknown topoSetSource type " << topoSetSourceType
    << endl << endl
    << "Valid topoSetSource types : " << endl
    << wordConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<topoSetSource>{cstrIter()(mesh, dict)};
}


mousse::autoPtr<mousse::topoSetSource> mousse::topoSetSource::New
(
  const word& topoSetSourceType,
  const polyMesh& mesh,
  Istream& is
)
{
  istreamConstructorTable::iterator cstrIter =
    istreamConstructorTablePtr_->find(topoSetSourceType);
  if (cstrIter == istreamConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "topoSetSource::New(const word&, "
      "const polyMesh&, Istream&)"
    )
    << "Unknown topoSetSource type " << topoSetSourceType
    << endl << endl
    << "Valid topoSetSource types : " << endl
    << istreamConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<topoSetSource>{cstrIter()(mesh, is)};
}


mousse::Istream& mousse::topoSetSource::checkIs(Istream& is)
{
  if (is.good() && !is.eof()) {
    return is;
  } else {
    FATAL_ERROR_IN("cellToFace::cellToFace") << "Istream not good"
      << exit(FatalError);
    return is;
  }
}


// Private Member Functions 
void mousse::topoSetSource::addOrDelete
(
  topoSet& set,
  const label cellI,
  const bool add
) const
{
  if (add) {
    set.insert(cellI);
  } else {
    set.erase(cellI);
  }
}


// Constructors 
mousse::topoSetSource::topoSetSource(const polyMesh& mesh)
:
  mesh_{mesh}
{}


// Destructor 
mousse::topoSetSource::~topoSetSource()
{}

