// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_writer.hpp"
#include "cell_modeller.hpp"
// Static Data Members
mousse::string mousse::meshWriter::defaultMeshName = "meshExport";
const mousse::cellModel* mousse::meshWriter::unknownModel = mousse::cellModeller::
lookup
(
  "unknown"
);
const mousse::cellModel* mousse::meshWriter::tetModel = mousse::cellModeller::
lookup
(
  "tet"
);
const mousse::cellModel* mousse::meshWriter::pyrModel = mousse::cellModeller::
lookup
(
  "pyr"
);
const mousse::cellModel* mousse::meshWriter::prismModel = mousse::cellModeller::
lookup
(
  "prism"
);
const mousse::cellModel* mousse::meshWriter::hexModel = mousse::cellModeller::
lookup
(
  "hex"
);
// Constructors 
mousse::meshWriter::meshWriter(const polyMesh& mesh, const scalar scaleFactor)
:
  mesh_(mesh),
  scaleFactor_(scaleFactor),
  writeBoundary_(true),
  boundaryRegion_(),
  cellTable_(),
  cellTableId_()
{}
// Destructor 
mousse::meshWriter::~meshWriter()
{}
