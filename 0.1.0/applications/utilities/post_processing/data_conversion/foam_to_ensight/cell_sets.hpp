#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_CELL_SETS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_ENSIGHT_CELL_SETS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSets

#include "label_list.hpp"


namespace mousse {

class cellSets
{
public:
    label nTets;
    label nPyrs;
    label nPrisms;
    label nHexesWedges;
    label nPolys;
    labelList tets;
    labelList pyrs;
    labelList prisms;
    labelList wedges;
    labelList hexes;
    labelList polys;
  // Constructors
    //- Construct given the number ov cells
    cellSets(const label nCells)
    :
      nTets{0},
      nPyrs{0},
      nPrisms{0},
      nHexesWedges{0},
      nPolys{0},
      tets{nCells},
      pyrs{nCells},
      prisms{nCells},
      wedges{nCells},
      hexes{nCells},
      polys{nCells}
    {}
  // Member Functions
    void setSize(const label nCells)
    {
      nTets = 0;
      nPyrs = 0;
      nPrisms = 0;
      nHexesWedges = 0;
      nPolys = 0;
      tets.setSize(nCells);
      pyrs.setSize(nCells);
      prisms.setSize(nCells);
      wedges.setSize(nCells);
      hexes.setSize(nCells);
      polys.setSize(nCells);
    }
};

}  // namespace mousse

#endif

