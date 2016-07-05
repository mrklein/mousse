// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_fluent_fields.hpp"


namespace mousse {

void writeFluentField
(
  const volVectorField& phi,
  const label fluentFieldIdentifier,
  Ostream& stream
)
{
  const vectorField& phiInternal = phi.internalField();
  // Writing cells
  stream
    << "(300 ("
    << fluentFieldIdentifier << " "  // Field identifier
    << "1 "                  // Zone ID: (cells=1, internal faces=2,
                             // patch faces=patchI+10)
    << "3 "                  // Number of components (scalar=1, vector=3)
    << "0 0 "                // Unused
    << "1 " << phiInternal.size() // Start and end of list
    << ")(" << endl;
  FOR_ALL(phiInternal, cellI) {
    stream
      << phiInternal[cellI].x() << " "
      << phiInternal[cellI].y() << " "
      << phiInternal[cellI].z() << " "
      << endl;
  }
  stream << "))" << endl;
  label nWrittenFaces = phiInternal.size();
  // Writing boundary faces
  FOR_ALL(phi.boundaryField(), patchI) {
    const vectorField& patchPhi = phi.boundaryField()[patchI];
    // Write header
    stream
      << "(300 ("
      << fluentFieldIdentifier << " "  // Field identifier
      << patchI + 10 << " "            // Zone ID: patchI+10
      << "3 "              // Number of components (scalar=1, vector=3)
      << "0 0 "            // Unused
      << nWrittenFaces + 1 << " " << nWrittenFaces + patchPhi.size()
                           // Start and end of list
      << ")(" << endl;
    nWrittenFaces += patchPhi.size();
    FOR_ALL(patchPhi, faceI) {
      stream
        << patchPhi[faceI].x() << " "
        << patchPhi[faceI].y() << " "
        << patchPhi[faceI].z() << " "
        << endl;
    }
    stream << "))" << endl;
  }
}

}  // namespace mousse

