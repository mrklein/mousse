#include "label.hpp"
#include "hash_set.hpp"
#include "label_vector.hpp"
namespace mousse
{
  class polyMesh;
  class wedgePolyPatch;
  label findOppositeWedge(const polyMesh&, const wedgePolyPatch&);
  //- Check wedge orientation
  bool checkWedges
  (
    const polyMesh&,
    const bool report,
    const Vector<label>&,
    labelHashSet*
  );
  //- Check 0th vertex on coupled faces
  bool checkCoupledPoints(const polyMesh&, const bool report, labelHashSet*);
  label checkGeometry(const polyMesh& mesh, const bool allGeometry);
}
