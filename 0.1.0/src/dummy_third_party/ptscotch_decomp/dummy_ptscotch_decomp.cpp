// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ptscotch_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"

static const char* notImplementedMessage =
"You are trying to use ptscotch but do not have the "
"ptscotchDecomp library loaded."
"\nThis message is from the dummy ptscotchDecomp stub library instead.\n"
"\n"
#if defined(darwin64)
"Please install ptscotch and make sure that libptscotch.dylib is in your "
"DYLD_LIBRARY_PATH.\n"
#else
"Please install ptscotch and make sure that libptscotch.so is in your "
"LD_LIBRARY_PATH.\n"
#endif
"The mousse_ptscotch_decomp library can then be built in "
"$MOUSSE_SRC/parallel/decompose/ptscotch_decomp\n";

namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(ptscotchDecomp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  decompositionMethod,
  ptscotchDecomp,
  dictionary
);
}
// Private Member Functions
void mousse::ptscotchDecomp::check(const int /*retVal*/, const char* /*str*/)
{}

mousse::label mousse::ptscotchDecomp::decompose
(
  const fileName& /*meshPath*/,
  const List<label>& /*initxadj*/,
  const List<label>& /*initadjncy*/,
  const scalarField& /*initcWeights*/,
  List<label>& /*finalDecomp*/
) const
{
  FATAL_ERROR_IN
  (
    "label ptscotchDecomp::decompose"
    "("
      "onst fileName&,"
      "const List<label>&, "
      "const List<label>&, "
      "const scalarField&, "
      "List<label>&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return -1;
}
mousse::label mousse::ptscotchDecomp::decompose
(
  const fileName& /*meshPath*/,
  const label /*adjncySize*/,
  const label /*adjncy*/[],
  const label /*xadjSize*/,
  const label /*xadj*/[],
  const scalarField& /*cWeights*/,
  List<label>& /*finalDecomp*/
) const
{
  FATAL_ERROR_IN
  (
    "label ptscotchDecomp::decompose"
    "("
      "const fileName&,"
      "const int,"
      "const int,"
      "const int,"
      "const int,"
      "const scalarField&,"
      "List<label>&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return -1;
}
// Constructors
mousse::ptscotchDecomp::ptscotchDecomp
(
  const dictionary& decompositionDict
)
:
  decompositionMethod(decompositionDict)
{}

// Member Functions
mousse::labelList mousse::ptscotchDecomp::decompose
(
  const polyMesh&,
  const pointField&,
  const scalarField& /*pointWeights*/
)
{
  FATAL_ERROR_IN
  (
    "labelList ptscotchDecomp::decompose"
    "("
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList::null();
}
mousse::labelList mousse::ptscotchDecomp::decompose
(
  const polyMesh&,
  const labelList& /*agglom*/,
  const pointField& /*agglomPoints*/,
  const scalarField& /*pointWeights*/
)
{
  FATAL_ERROR_IN
  (
    "labelList ptscotchDecomp::decompose"
    "("
      "const labelList&, "
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList::null();
}
mousse::labelList mousse::ptscotchDecomp::decompose
(
  const labelListList& /*globalCellCells*/,
  const pointField& /*cellCentres*/,
  const scalarField& /*cWeights*/
)
{
  FATAL_ERROR_IN
  (
    "labelList ptscotchDecomp::decompose"
    "("
      "const labelListList&, "
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList::null();
}
