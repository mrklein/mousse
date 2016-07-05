// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "manual_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "ifstream.hpp"
#include "label_io_list.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(manualDecomp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  decompositionMethod,
  manualDecomp,
  dictionary
);

}


// Constructors 
mousse::manualDecomp::manualDecomp(const dictionary& decompositionDict)
:
  decompositionMethod{decompositionDict},
  decompDataFile_
  {
    decompositionDict
      .subDict(word{decompositionDict.lookup("method")}  // ??? type()
               + "Coeffs").lookup("dataFile")
  }
{}


// Member Functions 
mousse::labelList mousse::manualDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& points,
  const scalarField& /*pointWeights*/
)
{
  labelIOList finalDecomp
  {
    {
      decompDataFile_,
      mesh.facesInstance(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE,
      false
    }
  };
  // check if the final decomposition is OK
  if (finalDecomp.size() != points.size()) {
    FATAL_ERROR_IN
    (
      "manualDecomp::decompose(const pointField&, const scalarField&)"
    )
    << "Size of decomposition list does not correspond "
    << "to the number of points.  Size: "
    << finalDecomp.size() << " Number of points: "
    << points.size()
    << ".\n" << "Manual decomposition data read from file "
    << decompDataFile_ << "." << endl
    << exit(FatalError);
  }
  if (min(finalDecomp) < 0 || max(finalDecomp) > nProcessors_ - 1) {
    FATAL_ERROR_IN
    (
      "manualDecomp::decompose(const pointField&, const scalarField&)"
    )
    << "According to the decomposition, cells assigned to "
    << "impossible processor numbers.  Min processor = "
    << min(finalDecomp) << " Max processor = " << max(finalDecomp)
    << ".\n" << "Manual decomposition data read from file "
    << decompDataFile_ << "." << endl
    << exit(FatalError);
  }
  return finalDecomp;
}

