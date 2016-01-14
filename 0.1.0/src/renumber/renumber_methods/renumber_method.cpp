// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "renumber_method.hpp"
#include "decomposition_method.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(renumberMethod, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(renumberMethod, dictionary);
}
// Member Functions 
mousse::autoPtr<mousse::renumberMethod> mousse::renumberMethod::New
(
  const dictionary& renumberDict
)
{
  const word methodType(renumberDict.lookup("method"));
  //Info<< "Selecting renumberMethod " << methodType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(methodType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "renumberMethod::New"
      "(const dictionary& renumberDict)"
    )   << "Unknown renumberMethod "
      << methodType << nl << nl
      << "Valid renumberMethods are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<renumberMethod>(cstrIter()(renumberDict));
}
mousse::labelList mousse::renumberMethod::renumber
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
  // Renumber based on agglomerated points
  return renumber(cellCells(), points);
}
mousse::labelList mousse::renumberMethod::renumber
(
  const labelList& /*cellCells*/,
  const labelList& /*offsets*/,
  const pointField& /*cc*/
) const
{
  NOT_IMPLEMENTED
  (
    "renumberMethod::renumber\n"
    "(\n"
    "    const labelList&,\n"
    "    const labelList&,\n"
    "    const pointField&\n"
    ") const"
  );
  return labelList();
}
mousse::labelList mousse::renumberMethod::renumber
(
  const polyMesh& mesh,
  const labelList& fineToCoarse,
  const pointField& coarsePoints
) const
{
  CompactListList<label> coarseCellCells;
  decompositionMethod::calcCellCells
  (
    mesh,
    fineToCoarse,
    coarsePoints.size(),
    false,                      // local only
    coarseCellCells
  );
  // Renumber based on agglomerated points
  labelList coarseDistribution
  (
    renumber
    (
      coarseCellCells(),
      coarsePoints
    )
  );
  // Rework back into renumbering for original mesh_
  labelList fineDistribution(fineToCoarse.size());
  FOR_ALL(fineDistribution, i)
  {
    fineDistribution[i] = coarseDistribution[fineToCoarse[i]];
  }
  return fineDistribution;
}
