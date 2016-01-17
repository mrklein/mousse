// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scotch_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
static const char* notImplementedMessage =
"You are trying to use scotch but do not have the scotchDecomp library loaded."
"\nThis message is from the dummy scotchDecomp stub library instead.\n"
"\n"
#if defined(darwin64)
"Please install scotch and make sure that libscotch.dylib is in your "
"DYLD_LIBRARY_PATH.\n"
#else
"Please install scotch and make sure that libscotch.so is in your "
"LD_LIBRARY_PATH.\n"
#endif
"The scotchDecomp library can then be built in "
"$MOUSSE_SRC/parallel/decompose/decomposition_methods/scotch_decomp\n";

namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(scotchDecomp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  decompositionMethod,
  scotchDecomp,
  dictionary
);
}

// Private Member Functions 
void mousse::scotchDecomp::check(const int /*retVal*/, const char* /*str*/)
{}
mousse::label mousse::scotchDecomp::decompose
(
  const fileName& /*meshPath*/,
  const List<label>& /*adjncy*/,
  const List<label>& /*xadj*/,
  const scalarField& /*cWeights*/,
  List<label>& /*finalDecomp*/
)
{
  FATAL_ERROR_IN
  (
    "label scotchDecomp::decompose\n"
    "(\n"
      "const fileName& meshPath,\n"
      "const List<label>&,\n"
      "const List<label>&,\n"
      "const scalarField&,\n"
      "List<label>&\n"
    ")\n"
  )   << notImplementedMessage << exit(FatalError);
  return -1;
}
// Constructors 
mousse::scotchDecomp::scotchDecomp
(
  const dictionary& decompositionDict
)
:
  decompositionMethod(decompositionDict)
{}
// Member Functions 
mousse::labelList mousse::scotchDecomp::decompose
(
  const polyMesh&,
  const pointField&,
  const scalarField& /*pointWeights*/
)
{
  FATAL_ERROR_IN
  (
    "labelList scotchDecomp::decompose"
    "("
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList::null();
}
mousse::labelList mousse::scotchDecomp::decompose
(
  const polyMesh&,
  const labelList& /*agglom*/,
  const pointField& /*agglomPoints*/,
  const scalarField& /*pointWeights*/
)
{
  FATAL_ERROR_IN
  (
    "labelList scotchDecomp::decompose"
    "("
      "const labelList&, "
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList::null();
}
mousse::labelList mousse::scotchDecomp::decompose
(
  const labelListList& /*globalCellCells*/,
  const pointField& /*cellCentres*/,
  const scalarField& /*cWeights*/
)
{
  FATAL_ERROR_IN
  (
    "labelList scotchDecomp::decompose"
    "("
      "const labelListList&, "
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList::null();
}
