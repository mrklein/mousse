// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "file_control.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tetrahedron.hpp"
#include "scalar_list.hpp"
#include "vector_tools.hpp"
#include "point_io_field.hpp"
#include "scalar_io_field.hpp"
#include "triad_io_field.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(fileControl, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  cellSizeAndAlignmentControl,
  fileControl,
  dictionary
);

}


// Constructors 
mousse::fileControl::fileControl
(
  const Time& runTime,
  const word& name,
  const dictionary& controlFunctionDict,
  const conformationSurfaces& geometryToConformTo,
  const scalar& defaultCellSize
)
:
  cellSizeAndAlignmentControl
  {
    runTime,
    name,
    controlFunctionDict,
    geometryToConformTo,
    defaultCellSize
  },
  pointsFile_{controlFunctionDict.lookup("pointsFile")},
  sizesFile_{controlFunctionDict.lookup("sizesFile")},
  alignmentsFile_{controlFunctionDict.lookup("alignmentsFile")},
  maxPriority_{readLabel(controlFunctionDict.lookup("priority"))}
{
  Info
    << indent << "Loading " << name << " from file:" << nl
    << indent << "    priority   : " << maxPriority_ << nl
    << indent << "    points     : " << pointsFile_ << nl
    << indent << "    sizes      : " << sizesFile_ << nl
    << indent << "    alignments : " << alignmentsFile_
    << endl;
}


// Destructor 
mousse::fileControl::~fileControl()
{}


// Member Functions 
void mousse::fileControl::cellSizeFunctionVertices
(
  DynamicList<mousse::point>& /*pts*/,
  DynamicList<scalar>& /*sizes*/
) const
{
  return;
}


void mousse::fileControl::initialVertices
(
  pointField& pts,
  scalarField& sizes,
  triadField& alignments
) const
{
  Info << "    Reading points from file     : " << pointsFile_ << endl;
  pointIOField pointsTmp
  {
    {
      pointsFile_,
      runTime_.constant(),
      runTime_,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  pts.transfer(pointsTmp);
  Info << "    Reading sizes from file      : " << sizesFile_ << endl;
  scalarIOField sizesTmp
  {
    {
      sizesFile_,
      runTime_.constant(),
      runTime_,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  sizes.transfer(sizesTmp);
  Info << "    Reading alignments from file : " << alignmentsFile_ << endl;
  triadIOField alignmentsTmp
  {
    {
      alignmentsFile_,
      runTime_.constant(),
      runTime_,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    }
  };
  alignments.transfer(alignmentsTmp);
  if ((pts.size() != sizes.size()) || (pts.size() != alignments.size())) {
    FATAL_ERROR_IN
    (
      "mousse::fileControl::initialVertices"
      "("
      "  pointField&,"
      "  scalarField&,"
      "  Field<triad>&"
      ")"
    )
    << "Size of list of points, sizes and alignments do not match:"
    << nl
    << "Points size     = " << pts.size() << nl
    << "Sizes size      = " << sizes.size() << nl
    << "Alignments size = " << alignments.size()
    << abort(FatalError);
  }
}

