#include "write_patch_graph.hpp"
#include "vol_fields.hpp"
#include "fv_mesh.hpp"
#include "graph.hpp"
namespace mousse
{
void writePatchGraph
(
  const volScalarField& vsf,
  const label patchLabel,
  const direction d,
  const word& graphFormat
)
{
  graph
  (
    vsf.name(),
    "position",
    vsf.name(),
    vsf.mesh().boundary()[patchLabel].Cf().component(d),
    vsf.boundaryField()[patchLabel]
  ).write(vsf.time().timePath()/vsf.name(), graphFormat);
}
}  // namespace mousse
