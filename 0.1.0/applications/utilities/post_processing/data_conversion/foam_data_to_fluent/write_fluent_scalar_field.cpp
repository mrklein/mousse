// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_fluent_fields.hpp"
#include "empty_fv_patch_fields.hpp"

namespace mousse
{
void writeFluentField
(
  const volScalarField& phi,
  const label fluentFieldIdentifier,
  Ostream& stream
)
{
  const scalarField& phiInternal = phi.internalField();
  // Writing cells
  stream
    << "(300 ("
    << fluentFieldIdentifier << " "  // Field identifier
    << "1 "                  // Zone ID: (cells=1, internal faces=2,
                // patch faces=patchI+10)
    << "1 "                  // Number of components (scalar=1, vector=3)
    << "0 0 "                // Unused
    << "1 " << phiInternal.size() // Start and end of list
    << ")(" << endl;
  FOR_ALL(phiInternal, cellI)
  {
    stream << phiInternal[cellI] << endl;
  }
  stream
    << "))" << endl;
  label nWrittenFaces = phiInternal.size();
  // Writing boundary faces
  FOR_ALL(phi.boundaryField(), patchI)
  {
    if (isType<emptyFvPatchScalarField>(phi.boundaryField()[patchI]))
    {
      // Form empty patch field repeat the internal field to
      // allow for the node interpolation in Fluent
      const scalarField& phiInternal = phi.internalField();
      // Get reference to internal cells
      const labelList emptyFaceCells =
        phi.boundaryField()[patchI].patch().patch().faceCells();
      // Writing cells for empty patch
      stream
        << "(300 ("
        << fluentFieldIdentifier << " "  // Field identifier
        << patchI + 10 << " "            // Zone ID: patchI+10
        << "1 "             // Number of components (scalar=1, vector=3)
        << "0 0 "                // Unused
        << nWrittenFaces + 1 << " "
        << nWrittenFaces + emptyFaceCells.size()// Start and end of list
        << ")(" << endl;
      nWrittenFaces += emptyFaceCells.size();
      FOR_ALL(emptyFaceCells, faceI)
      {
        stream << phiInternal[emptyFaceCells[faceI]] << endl;
      }
      stream
        << "))" << endl;
    }
    else
    {
      // Regular patch
      label nWrittenFaces = phiInternal.size();
      const scalarField& patchPhi = phi.boundaryField()[patchI];
      // Write header
      stream
        << "(300 ("
        << fluentFieldIdentifier << " "  // Field identifier
        << patchI + 10 << " "            // Zone ID: patchI+10
        << "1 "          // Number of components (scalar=1, vector=3)
        << "0 0 "            // Unused
        << nWrittenFaces + 1 << " " << nWrittenFaces + patchPhi.size()
                // Start and end of list
        << ")(" << endl;
      nWrittenFaces += patchPhi.size();
      FOR_ALL(patchPhi, faceI)
      {
        stream << patchPhi[faceI] << endl;
      }
      stream
        << "))" << endl;
    }
  }
}
}  // namespace mousse
