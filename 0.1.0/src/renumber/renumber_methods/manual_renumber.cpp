// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "manual_renumber.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ifstream.hpp"
#include "label_io_list.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(manualRenumber, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    renumberMethod,
    manualRenumber,
    dictionary
  );
}
// Constructors 
mousse::manualRenumber::manualRenumber(const dictionary& renumberDict)
:
  renumberMethod(renumberDict),
  dataFile_
  (
    renumberDict.subDict(typeName+"Coeffs").lookup("dataFile")
  )
{}
// Member Functions 
mousse::labelList mousse::manualRenumber::renumber
(
  const polyMesh& mesh,
  const pointField& points
) const
{
  labelIOList newToOld
  (
    IOobject
    (
      dataFile_,
      mesh.facesInstance(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE,
      false
    )
  );
  // check if the final renumbering is OK
  if (newToOld.size() != points.size())
  {
    FATAL_ERROR_IN
    (
      "manualRenumber::renumber(const pointField&, const scalarField&)"
    )   << "Size of renumber list does not correspond "
      << "to the number of points.  Size: "
      << newToOld.size() << " Number of points: "
      << points.size()
      << ".\n" << "Manual renumbering data read from file "
      << dataFile_ << "." << endl
      << exit(FatalError);
  }
  // Invert to see if one to one
  labelList oldToNew(points.size(), -1);
  FOR_ALL(newToOld, i)
  {
    label origCellI = newToOld[i];
    if (origCellI < 0 || origCellI >= points.size())
    {
      FATAL_ERROR_IN
      (
        "manualRenumber::renumber(const pointField&"
        ", const scalarField&)"
      )   << "Renumbering is not one-to-one. Index "
        << i << " maps onto original cell " << origCellI
        << ".\n" << "Manual renumbering data read from file "
        << dataFile_ << "." << endl
        << exit(FatalError);
    }
    if (oldToNew[origCellI] == -1)
    {
      oldToNew[origCellI] = i;
    }
    else
    {
      FATAL_ERROR_IN
      (
        "manualRenumber::renumber(const pointField&"
        ", const scalarField&)"
      )   << "Renumbering is not one-to-one. Both index "
        << oldToNew[origCellI]
        << " and " << i << " map onto " << origCellI
        << ".\n" << "Manual renumbering data read from file "
        << dataFile_ << "." << endl
        << exit(FatalError);
    }
  }
  return newToOld;
}
