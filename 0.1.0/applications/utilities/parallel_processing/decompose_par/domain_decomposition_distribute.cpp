// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "domain_decomposition.hpp"
#include "decomposition_method.hpp"
#include "cpu_time.hpp"
#include "cell_set.hpp"
#include "region_split.hpp"
#include "tuple2.hpp"
#include "face_set.hpp"
void mousse::domainDecomposition::distributeCells()
{
  Info<< "\nCalculating distribution of cells" << endl;
  cpuTime decompositionTime;
  autoPtr<decompositionMethod> decomposePtr = decompositionMethod::New
  (
    decompositionDict_
  );
  scalarField cellWeights;
  if (decompositionDict_.found("weightField"))
  {
    word weightName = decompositionDict_.lookup("weightField");
    volScalarField weights
    (
      IOobject
      (
        weightName,
        time().timeName(),
        *this,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
      ),
      *this
    );
    cellWeights = weights.internalField();
  }
  cellToProc_ = decomposePtr().decompose(*this, cellWeights);
  Info<< "\nFinished decomposition in "
    << decompositionTime.elapsedCpuTime()
    << " s" << endl;
}
