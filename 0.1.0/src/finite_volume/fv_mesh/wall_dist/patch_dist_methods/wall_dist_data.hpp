// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wallDistData
// Description
//   Wall distance calculation. Like wallDist but also transports extra
//   data (template argument).
//   Used for e.g reflection vector calculation or vanDriest damping.
//   Templated on two parameters:
//     - TransferType: type of overall data transported
//      (e.g. wallPointData\<vector\>)
// SourceFiles
//   wall_dist_data.cpp
#ifndef wall_dist_data_hpp_
#define wall_dist_data_hpp_
#include "cell_dist_funcs.hpp"
#include "vol_fields.hpp"
namespace mousse
{
template<class TransferType>
class wallDistData
:
  public volScalarField,
  public cellDistFuncs
{
private:
  typedef typename TransferType::dataType Type;
  // Private Member Data
    //- Reference to field whose data to use (on walls) and update
    //  (every cell and non-wall face)
    GeometricField<Type, fvPatchField, volMesh>& field_;
    //- Do accurate distance calculation for near-wall cells.
    bool correctWalls_;
    //- Number of unset cells and faces.
    label nUnset_;
public:
  // Constructors
    //- Construct from mesh and flag whether or not to correct wall.
    //  Calculate for all cells. correctWalls : correct wall (face&point)
    //  cells for correct distance, searching neighbours.
    wallDistData
    (
      const fvMesh& mesh,
      GeometricField<Type, fvPatchField, volMesh>&,
      bool correctWalls = true
    );
    //- Disallow default bitwise copy construct
    wallDistData(const wallDistData&) = delete;
    //- Disallow default bitwise assignment
    wallDistData& operator=(const wallDistData&) = delete;
  //- Destructor
  virtual ~wallDistData();
  // Member Functions
    const volScalarField& y() const
    {
      return *this;
    }
    label nUnset() const
    {
      return nUnset_;
    }
    //- Access field
    const GeometricField<Type, fvPatchField, volMesh>& data() const
    {
      return field_;
    }
    //- Correct for mesh geom/topo changes
    virtual void correct();
};
}  // namespace mousse
#ifdef NoRepository
#   include "wall_dist_data.cpp"
#endif
#endif
