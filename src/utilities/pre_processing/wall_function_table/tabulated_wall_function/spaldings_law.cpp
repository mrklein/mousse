// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "spaldings_law.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace tabulatedWallFunctions {

DEFINE_TYPE_NAME_AND_DEBUG(SpaldingsLaw, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  tabulatedWallFunction,
  SpaldingsLaw,
  dictionary
);

}
}

const mousse::label mousse::tabulatedWallFunctions::SpaldingsLaw::maxIters_ = 1000;

const mousse::scalar
  mousse::tabulatedWallFunctions::SpaldingsLaw::tolerance_ = 1e-4;


// Private Member Functions 
void mousse::tabulatedWallFunctions::SpaldingsLaw::invertFunction()
{
  // Initialise u+ and R
  scalar Re = 0.0;
  scalar uPlus = 1;
  // Populate the table
  FOR_ALL(invertedTable_, i) {
    if (invertedTable_.log10()) {
      Re = pow(10, (i*invertedTable_.dx() + invertedTable_.x0()));
    } else {
      Re = i*invertedTable_.dx() + invertedTable_.x0();
    }
    // Use latest available u+ estimate
    if (i > 0) {
      uPlus = invertedTable_[i-1];
    }
    // Newton iterations to determine u+
    label iter = 0;
    scalar error = GREAT;
    do {
      scalar kUPlus = min(kappa_*uPlus, 50);
      scalar A =
        E_*sqr(uPlus) + uPlus
        *(exp(kUPlus) - pow3(kUPlus)/6 - 0.5*sqr(kUPlus) - kUPlus - 1);
      scalar f = - Re + A/E_;
      scalar df =
        (
          2*E_*uPlus
          + exp(kUPlus)*(kUPlus + 1)
          - 2/3*pow3(kUPlus)
          - 1.5*sqr(kUPlus)
          - 2*kUPlus
          - 1
        )/E_;
      scalar uPlusNew = uPlus - f/(df + ROOTVSMALL);
      error = mag((uPlus - uPlusNew)/uPlusNew);
      uPlus = uPlusNew;
    } while (error > tolerance_ && ++iter < maxIters_);
    if (iter == maxIters_) {
      WARNING_IN("SpaldingsLaw::invertFunction()")
        << "Newton iterations not converged:" << nl
        << "    iters = " << iter << ", error = " << error << endl;
    }
    // Set new values - constrain u+ >= 0
    invertedTable_[i] = max(0, uPlus);
  }
}


// Constructors 
mousse::tabulatedWallFunctions::SpaldingsLaw::SpaldingsLaw
(
  const dictionary& dict,
  const polyMesh& mesh
)
:
  tabulatedWallFunction{dict, mesh, typeName},
  kappa_{readScalar(coeffDict_.lookup("kappa"))},
  E_{readScalar(coeffDict_.lookup("E"))}
{
  invertFunction();
  if (debug) {
    writeData(Info);
  }
}


// Destructor 
mousse::tabulatedWallFunctions::SpaldingsLaw::~SpaldingsLaw()
{}


// Member Functions 
mousse::scalar mousse::tabulatedWallFunctions::SpaldingsLaw::yPlus
(
  const scalar uPlus
) const
{
  scalar kUPlus = min(kappa_*uPlus, 50);
  return
    uPlus + 1/E_*(exp(kUPlus) - pow3(kUPlus)/6 - 0.5*sqr(kUPlus) - kUPlus - 1);
}


mousse::scalar mousse::tabulatedWallFunctions::SpaldingsLaw::Re
(
  const scalar uPlus
) const
{
  return uPlus*yPlus(uPlus);
}


void mousse::tabulatedWallFunctions::SpaldingsLaw::writeData(Ostream& os) const
{
  if (invertedTable_.log10()) {
    os << "log10(Re), y+, u+:" << endl;
    FOR_ALL(invertedTable_, i) {
      scalar uPlus = invertedTable_[i];
      scalar Re = ::log10(this->Re(uPlus));
      scalar yPlus = this->yPlus(uPlus);
      os << Re << ", " << yPlus << ", " << uPlus << endl;
    }
  } else {
    os << "Re, y+, u+:" << endl;
    FOR_ALL(invertedTable_, i)
    {
      scalar uPlus = invertedTable_[i];
      scalar Re = this->Re(uPlus);
      scalar yPlus = this->yPlus(uPlus);
      os << Re << ", " << yPlus << ", " << uPlus << endl;
    }
  }
}

