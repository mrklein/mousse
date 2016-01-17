// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spring_renumber.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "decomposition_method.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(springRenumber, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    renumberMethod,
    springRenumber,
    dictionary
  );
}
// Constructors 
mousse::springRenumber::springRenumber(const dictionary& renumberDict)
:
  renumberMethod(renumberDict),
  dict_(renumberDict.subDict(typeName+"Coeffs")),
  maxCo_(readScalar(dict_.lookup("maxCo"))),
  maxIter_(readLabel(dict_.lookup("maxIter"))),
  freezeFraction_(readScalar(dict_.lookup("freezeFraction")))
{}
// Member Functions 
mousse::labelList mousse::springRenumber::renumber
(
  const polyMesh& mesh,
  const pointField& points
) const
{
  CompactListList<label> cellCells;
  decompositionMethod::calcCellCells
  (
    mesh,
    identity(mesh.nCells()),
    mesh.nCells(),
    false,                      // local only
    cellCells
  );
  return renumber(cellCells(), points);
}
mousse::labelList mousse::springRenumber::renumber
(
  const labelListList& cellCells,
  const pointField& /*points*/
) const
{
  // Look at cell index as a 1D position parameter.
  // Move cells to the average 'position' of their neighbour.
  scalarField position(cellCells.size());
  FOR_ALL(position, cellI)
  {
    position[cellI] = cellI;
  }
  labelList oldToNew(identity(cellCells.size()));
  scalar maxCo = maxCo_ * cellCells.size();
  for (label iter = 0; iter < maxIter_; iter++)
  {
    //Pout<< "Iteration : " << iter << nl
    //    << "------------"
    //    << endl;
    //Pout<< "Position :" << nl
    //    << "    min : " << min(position) << nl
    //    << "    max : " << max(position) << nl
    //    << "    avg : " << average(position) << nl
    //    << endl;
    // Sum force per cell.
    scalarField sumForce(cellCells.size(), 0.0);
    FOR_ALL(cellCells, oldCellI)
    {
      const labelList& cCells = cellCells[oldCellI];
      label cellI = oldToNew[oldCellI];
      FOR_ALL(cCells, i)
      {
        label nbrCellI = oldToNew[cCells[i]];
        sumForce[cellI] += (position[nbrCellI]-position[cellI]);
      }
    }
    //Pout<< "Force :" << nl
    //    << "    min    : " << min(sumForce) << nl
    //    << "    max    : " << max(sumForce) << nl
    //    << "    avgMag : " << average(mag(sumForce)) << nl
    //    << "DeltaT : " << deltaT << nl
    //    << endl;
    // Limit displacement
    scalar deltaT = maxCo / max(mag(sumForce));
    Info<< "Iter:" << iter
      << "  maxCo:" << maxCo
      << "  deltaT:" << deltaT
      << "  average force:" << average(mag(sumForce)) << endl;
    // Determine displacement.
    scalarField displacement(deltaT*sumForce);
    //Pout<< "Displacement :" << nl
    //    << "    min    : " << min(displacement) << nl
    //    << "    max    : " << max(displacement) << nl
    //    << "    avgMag : " << average(mag(displacement)) << nl
    //    << endl;
    // Calculate new position and scale to be within original range
    // (0..nCells-1) for ease of postprocessing.
    position += displacement;
    position -= min(position);
    position *= (position.size()-1)/max(position);
    // Slowly freeze.
    maxCo *= freezeFraction_;
  }
  //writeOBJ("endPosition.obj", cellCells, position);
  // Move cells to new position
  labelList shuffle;
  sortedOrder(position, shuffle);
  // Reorder oldToNew
  inplaceReorder(shuffle, oldToNew);
  return invert(oldToNew.size(), oldToNew);
}
