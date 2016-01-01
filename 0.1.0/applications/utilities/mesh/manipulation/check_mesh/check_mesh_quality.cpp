#include "check_mesh_quality.hpp"
#include "poly_mesh.hpp"
#include "cell_set.hpp"
#include "face_set.hpp"
#include "motion_smoother.hpp"
mousse::label mousse::checkMeshQuality
(
  const polyMesh& mesh,
  const dictionary& dict
)
{
  label noFailedChecks = 0;
  {
    faceSet faces(mesh, "meshQualityFaces", mesh.nFaces()/100+1);
    motionSmoother::checkMesh(false, mesh, dict, faces);
    label nFaces = returnReduce(faces.size(), sumOp<label>());
    if (nFaces > 0)
    {
      noFailedChecks++;
      Info<< "  <<Writing " << nFaces
        << " faces in error to set " << faces.name() << endl;
      faces.instance() = mesh.pointsInstance();
      faces.write();
    }
  }
  return noFailedChecks;
}
