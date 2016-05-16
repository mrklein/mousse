// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_surf_fields.hpp"
#include "ofstream.hpp"
#include "float_scalar.hpp"
#include "write_funs.hpp"
#include "empty_fvs_patch_fields.hpp"
#include "fvs_patch_fields.hpp"


namespace mousse {

// Global Functions 
void writeSurfFields
(
  const bool binary,
  const vtkMesh& vMesh,
  const fileName& fileName,
  const PtrList<surfaceVectorField>& surfVectorFields
)
{
  const fvMesh& mesh = vMesh.mesh();
  std::ofstream str{fileName.c_str()};
  writeFuns::writeHeader
  (
    str,
    binary,
    "surfaceFields"
  );
  str << "DATASET POLYDATA" << std::endl;
  const pointField& fc = mesh.faceCentres();
  str << "POINTS " << mesh.nFaces() << " float" << std::endl;
  DynamicList<floatScalar> pField{3*mesh.nFaces()};
  for (label faceI = 0; faceI < mesh.nFaces(); faceI++) {
    writeFuns::insert(fc[faceI], pField);
  }
  writeFuns::write(str, binary, pField);
  str << "POINT_DATA " << mesh.nFaces() << std::endl
    << "FIELD attributes " << surfVectorFields.size() << std::endl;
  // surfVectorFields
  FOR_ALL(surfVectorFields, fieldI) {
    const surfaceVectorField& svf = surfVectorFields[fieldI];
    str << svf.name() << " 3 "
      << mesh.nFaces() << " float" << std::endl;
    DynamicList<floatScalar> fField{3*mesh.nFaces()};
    for (label faceI = 0; faceI < mesh.nInternalFaces(); faceI++) {
      writeFuns::insert(svf[faceI], fField);
    }
    FOR_ALL(svf.boundaryField(), patchI) {
      const fvsPatchVectorField& pf = svf.boundaryField()[patchI];
      const fvPatch& pp = mesh.boundary()[patchI];
      if (isA<emptyFvsPatchVectorField>(pf)) {
        // Note: loop over polypatch size, not fvpatch size.
        FOR_ALL(pp.patch(), i) {
          writeFuns::insert(vector::zero, fField);
        }
      } else {
        FOR_ALL(pf, i) {
          writeFuns::insert(pf[i], fField);
        }
      }
    }
    writeFuns::write(str, binary, fField);
  }
}

}  // namespace mousse

