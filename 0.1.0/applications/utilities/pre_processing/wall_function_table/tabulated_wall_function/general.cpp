// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "general.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "tuple2.hpp"


// Static Data Members
namespace mousse {
namespace tabulatedWallFunctions {

DEFINE_TYPE_NAME_AND_DEBUG(general, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  tabulatedWallFunction,
  general,
  dictionary
);

}

template<>
const char* mousse::NamedEnum
<
  mousse::tabulatedWallFunctions::general::interpolationType,
  1
>::names[] =
{
  "linear"
};

}

const
mousse::NamedEnum<mousse::tabulatedWallFunctions::general::interpolationType, 1>
  mousse::tabulatedWallFunctions::general::interpolationTypeNames_;


// Private Member Functions 
void mousse::tabulatedWallFunctions::general::invertTable()
{
  scalarList Rey{uPlus_.size(), 0.0};
  // Calculate Reynolds number
  FOR_ALL(uPlus_, i) {
    Rey[i] = yPlus_[i]*uPlus_[i];
    if (invertedTable_.log10()) {
      Rey[i] = ::log10(max(ROOTVSMALL, Rey[i]));
    }
  }
  // Populate the U+ vs Re table
  FOR_ALL(invertedTable_, i) {
    scalar Re = i*invertedTable_.dx() + invertedTable_.x0();
    invertedTable_[i] = max(0, interpolate(Re, Rey, uPlus_));
  }
}


mousse::scalar mousse::tabulatedWallFunctions::general::interpolate
(
  const scalar xi,
  const scalarList& x,
  const scalarList& fx
) const
{
  switch (interpType_) {
    case itLinear:
      {
        if (xi <= x[0]) {
          return fx[0];
        } else if (xi >= x.last()) {
          return fx.last();
        } else {
          label i2 = 0;
          while (x[i2] < xi) {
            i2++;
          }
          label i1 = i2 - 1;
          return (xi - x[i1])/(x[i2] - x[i1])*(fx[i2] - fx[i1]) + fx[i1];
        }
        break;
      }
    default:
      {
        FATAL_ERROR_IN
        (
          "tabulatedWallFunctions::general::interpolate"
          "("
          "const scalar, "
          "const scalarList&, "
          "const scalarList&"
          ")"
        )
        << "Unknown interpolation method" << nl
        << abort(FatalError);
      }
  }
  return 0.0;
}


// Constructors 
mousse::tabulatedWallFunctions::general::general
(
  const dictionary& dict,
  const polyMesh& mesh
)
:
  tabulatedWallFunction{dict, mesh, typeName},
  interpType_{interpolationTypeNames_[coeffDict_.lookup("interpType")]},
  yPlus_{},
  uPlus_{},
  log10YPlus_{coeffDict_.lookup("log10YPlus")},
  log10UPlus_{coeffDict_.lookup("log10UPlus")}
{
  List<Tuple2<scalar, scalar>> inputTable = coeffDict_.lookup("inputTable");
  if (inputTable.size() < 2) {
    FATAL_ERROR_IN
    (
      "tabulatedWallFunctions::general::general"
      "("
        "const dictionary&, "
        "const polyMesh&"
      ")"
    )
    << "Input table must have at least 2 values" << nl
    << exit(FatalError);
  }
  yPlus_.setSize(inputTable.size());
  uPlus_.setSize(inputTable.size());
  FOR_ALL(inputTable, i) {
    if (log10YPlus_) {
      yPlus_[i] = pow(10, inputTable[i].first());
    } else {
      yPlus_[i] = inputTable[i].first();
    }
    if (log10UPlus_) {
      uPlus_[i] = pow(10, inputTable[i].second());
    } else {
      uPlus_[i] = inputTable[i].second();
    }
  }
  invertTable();
  if (debug) {
    writeData(Info);
  }
}


// Destructor 
mousse::tabulatedWallFunctions::general::~general()
{}


// Member Functions 
mousse::scalar mousse::tabulatedWallFunctions::general::yPlus
(
  const scalar uPlus
) const
{
  return interpolate(uPlus, uPlus_, yPlus_);
}


mousse::scalar mousse::tabulatedWallFunctions::general::Re
(
  const scalar uPlus
) const
{
  return uPlus*yPlus(uPlus);
}


// Member Operators 
void mousse::tabulatedWallFunctions::general::writeData(Ostream& os) const
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
    FOR_ALL(invertedTable_, i) {
      scalar uPlus = invertedTable_[i];
      scalar Re = this->Re(uPlus);
      scalar yPlus = this->yPlus(uPlus);
      os << Re << ", " << yPlus << ", " << uPlus << endl;
    }
  }
}

