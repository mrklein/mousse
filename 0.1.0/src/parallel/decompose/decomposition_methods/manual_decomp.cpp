// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "manual_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ifstream.hpp"
#include "label_io_list.hpp"
namespace mousse
{
  defineTypeNameAndDebug(manualDecomp, 0);
  addToRunTimeSelectionTable
  (
    decompositionMethod,
    manualDecomp,
    dictionary
  );
}
// Constructors 
mousse::manualDecomp::manualDecomp(const dictionary& decompositionDict)
:
  decompositionMethod(decompositionDict),
  decompDataFile_
  (
    decompositionDict.subDict(word(decompositionDict.lookup("method"))
   + "Coeffs").lookup("dataFile")
  )
{}
// Member Functions 
mousse::labelList mousse::manualDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& points,
  const scalarField& pointWeights
)
{
  labelIOList finalDecomp
  (
    IOobject
    (
      decompDataFile_,
      mesh.facesInstance(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE,
      false
    )
  );
  // check if the final decomposition is OK
  if (finalDecomp.size() != points.size())
  {
    FatalErrorIn
    (
      "manualDecomp::decompose(const pointField&, const scalarField&)"
    )   << "Size of decomposition list does not correspond "
      << "to the number of points.  Size: "
      << finalDecomp.size() << " Number of points: "
      << points.size()
      << ".\n" << "Manual decomposition data read from file "
      << decompDataFile_ << "." << endl
      << exit(FatalError);
  }
  if (min(finalDecomp) < 0 || max(finalDecomp) > nProcessors_ - 1)
  {
    FatalErrorIn
    (
      "manualDecomp::decompose(const pointField&, const scalarField&)"
    )   << "According to the decomposition, cells assigned to "
      << "impossible processor numbers.  Min processor = "
      << min(finalDecomp) << " Max processor = " << max(finalDecomp)
      << ".\n" << "Manual decomposition data read from file "
      << decompDataFile_ << "." << endl
      << exit(FatalError);
  }
  return finalDecomp;
}
