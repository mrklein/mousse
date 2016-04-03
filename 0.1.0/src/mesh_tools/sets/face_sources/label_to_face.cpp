// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_to_face.hpp"
#include "poly_mesh.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(labelToFace, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, labelToFace, word);
ADD_TO_RUN_TIME_SELECTION_TABLE(topoSetSource, labelToFace, istream);

}

mousse::topoSetSource::addToUsageTable mousse::labelToFace::usage_
{
  labelToFace::typeName,
  "\n    Usage: labelToFace (i0 i1 .. in)\n\n"
  "    Select faces by label\n\n"
};


// Private Member Functions 
void mousse::labelToFace::combine(topoSet& set, const bool add) const
{
  FOR_ALL(labels_, labelI) {
    addOrDelete(set, labels_[labelI], add);
  }
}


// Constructors 

// Construct from components
mousse::labelToFace::labelToFace
(
  const polyMesh& mesh,
  const labelList& labels
)
:
  topoSetSource{mesh},
  labels_{labels}
{}


// Construct from dictionary
mousse::labelToFace::labelToFace
(
  const polyMesh& mesh,
  const dictionary& dict
)
:
  topoSetSource{mesh},
  labels_{dict.lookup("value")}
{}


// Construct from Istream
mousse::labelToFace::labelToFace
(
  const polyMesh& mesh,
  Istream& is
)
:
  topoSetSource{mesh},
  labels_{checkIs(is)}
{}


// Destructor 
mousse::labelToFace::~labelToFace()
{}


// Member Functions 
void mousse::labelToFace::applyToSet
(
  const topoSetSource::setAction action,
  topoSet& set
) const
{
  if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD)) {
    Info << "    Adding faces mentioned in dictionary" << " ..." << endl;
    combine(set, true);
  } else if (action == topoSetSource::DELETE) {
    Info << "    Removing faces mentioned dictionary" << " ..." << endl;
    combine(set, false);
  }
}

