// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair_potential.hpp"
#include "iostreams.hpp"


bool mousse::pairPotential::writeEnergyAndForceTables(Ostream& os) const
{
  Info << "Writing energy and force tables to file for potential "
    << name_ << endl;
  List<Pair<scalar>> eTab{energyTable()};
  List<Pair<scalar>> fTab{forceTable()};
  FOR_ALL(eTab, e) {
    os << eTab[e].first()
      << token::SPACE
      << eTab[e].second()
      << token::SPACE
      << fTab[e].second()
      << nl;
  }
  return os.good();
}

