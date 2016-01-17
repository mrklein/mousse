// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::extrudeModels::sector
// Description
//   Extrudes by rotating a surface around an axis
//   - extrusion is opposite the surface/patch normal so inwards the source
//    mesh
//   - axis direction has to be consistent with this.
//   - use -mergeFaces option if doing full 360 and want to merge front and back
//   - note direction of axis. This should be consistent with rotating against
//    the patch normal direction. If you get it wrong you'll see all cells
//    with extreme aspect ratio and internal faces wrong way around in
//    checkMesh
#ifndef sector_hpp_
#define sector_hpp_
#include "extrude_model.hpp"
namespace mousse
{
namespace extrudeModels
{
class sector
:
  public extrudeModel
{
  // Private data
    //- Point on axis
    const point axisPt_;
    //- Normalized direction of axis
    const vector axis_;
    //- Overall angle (radians)
    const scalar angle_;
public:
  //- Runtime type information
  TYPE_NAME("sector");
  // Constructors
    //- Construct from dictionary
    sector(const dictionary& dict);
  //- Destructor
  virtual ~sector();
  // Member Operators
    point operator()
    (
      const point& surfacePoint,
      const vector& surfaceNormal,
      const label layer
    ) const;
};
}  // namespace extrudeModels
}  // namespace mousse
#endif
