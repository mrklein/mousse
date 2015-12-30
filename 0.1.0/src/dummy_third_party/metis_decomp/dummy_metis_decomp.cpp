// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "metis_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
static const char* notImplementedMessage =
"You are trying to use metis but do not have the metisDecomp library loaded."
"\nThis message is from the dummy metisDecomp stub library instead.\n"
"\n"
#if defined(darwin64)
"Please install metis and make sure that libmetis.dylib is in your "
"DYLD_LIBRARY_PATH.\n"
#else
"Please install metis and make sure that libmetis.so is in your "
"LD_LIBRARY_PATH.\n"
#endif
"The metisDecomp library can then be built from "
"$MOUSSE_SRC/parallel/decompose/metisDecomp and dynamically loading or linking"
" this library will add metis as a decomposition method.\n"
"Please be aware that there are license restrictions on using Metis.";
namespace mousse
{
  defineTypeNameAndDebug(metisDecomp, 0);
  addToRunTimeSelectionTable
  (
    decompositionMethod,
    metisDecomp,
    dictionary
  );
}
// Private Member Functions 
mousse::label mousse::metisDecomp::decompose
(
  const List<label>& adjncy,
  const List<label>& xadj,
  const scalarField& cellWeights,
  List<label>& finalDecomp
)
{
  FatalErrorIn
  (
    "labelList metisDecomp::decompose"
    "("
      "const List<label>&, "
      "const List<label>&, "
      "const scalarField&, "
      "List<label>&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return -1;
}
// Constructors 
mousse::metisDecomp::metisDecomp
(
  const dictionary& decompositionDict
)
:
  decompositionMethod(decompositionDict)
{}
// Member Functions 
mousse::labelList mousse::metisDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& points,
  const scalarField& pointWeights
)
{
  FatalErrorIn
  (
    "labelList metisDecomp::decompose"
    "("
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList();
}
mousse::labelList mousse::metisDecomp::decompose
(
  const polyMesh& mesh,
  const labelList& agglom,
  const pointField& agglomPoints,
  const scalarField& agglomWeights
)
{
  FatalErrorIn
  (
    "labelList metisDecomp::decompose"
    "("
      "const labelList&, "
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList();
}
mousse::labelList mousse::metisDecomp::decompose
(
  const labelListList& globalCellCells,
  const pointField& cellCentres,
  const scalarField& cellWeights
)
{
  FatalErrorIn
  (
    "labelList metisDecomp::decompose"
    "("
      "const labelListList&, "
      "const pointField&, "
      "const scalarField&"
    ")"
  )   << notImplementedMessage << exit(FatalError);
  return labelList();
}
