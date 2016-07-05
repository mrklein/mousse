#include "write_cell_graph.hpp"
#include "vol_fields.hpp"
#include "fv_mesh.hpp"
#include "graph.hpp"


namespace mousse {

void writeCellGraph
(
  const volScalarField& vsf,
  const word& graphFormat
)
{
  fileName path{vsf.time().path()/"graphs"/vsf.time().timeName()};
  mkDir(path);
  graph
  (
    vsf.name(),
    "x",
    vsf.name(),
    vsf.mesh().C().internalField().component(vector::X),
    vsf.internalField()
  ).write(path/vsf.name(), graphFormat);
}

}  // namespace mousse

