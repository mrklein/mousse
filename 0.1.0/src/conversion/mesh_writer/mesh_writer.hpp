// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::meshWriter
// Description
//   write OpenFOAM meshes and/or results to another CFD format
//   - currently just STAR-CD
// \par Files
//   "constant/boundaryRegion" is an IOMap<dictionary> that contains
//   the boundary type and names. eg,
//   \verbatim
//     (
//       0
//       {
//         BoundaryType    wall;
//         Label           Default_Boundary_Region;
//       }
//       1
//       {
//         BoundaryType    inlet;
//         Label           inlet_1;
//       }
//       ...
//       4
//       {
//         BoundaryType    pressure;
//         Label           outlet;
//       }
//     )
//   \endverbatim
// SourceFiles
//   mesh_writer_i.hpp
//   mesh_writer.cpp
//   mesh_writer_io.cpp
#ifndef mesh_writer_hpp_
#define mesh_writer_hpp_
#include "poly_mesh.hpp"
#include "boundary_region.hpp"
#include "cell_table.hpp"
namespace mousse
{
class meshWriter
{
protected:
  // Protected data
    //- Mesh reference
    const polyMesh& mesh_;
    //- Scaling factor for points (eg, [m] -> [mm])
    scalar scaleFactor_;
    //- Write bnd file
    bool writeBoundary_;
    //- boundaryRegion persistent data saved as a dictionary
    boundaryRegion boundaryRegion_;
    //- cellTable persistent data saved as a dictionary
    cellTable cellTable_;
    //- cellTable IDs for each cell
    labelList cellTableId_;
    //- Pointers to cell shape models
    static const cellModel* unknownModel;
    static const cellModel* tetModel;
    static const cellModel* pyrModel;
    static const cellModel* prismModel;
    static const cellModel* hexModel;
public:
  // Static data members
    //- Specify a default mesh name
    static string defaultMeshName;
  // Constructors
    //- Create a writer obejct
    meshWriter
    (
      const polyMesh&,
      const scalar scaleFactor = 1.0
    );
  // Private Member Functions
    //- Disallow default bitwise copy construct
    meshWriter(const meshWriter&) = delete;
    //- Disallow default bitwise assignment
    meshWriter& operator=(const meshWriter&) = delete;
  //- Destructor
  virtual ~meshWriter();
  // Member Functions
    // Edit
      //- Set points scaling
      void scaleFactor(const scalar scaling)
      {
        scaleFactor_ = scaling;
      }
      //- Suppress writing bnd file
      void noBoundary()
      {
        writeBoundary_ = false;
      }
    // Write
      //- Write volume mesh. Subclass must supply this method
      virtual bool write
      (
        const fileName& timeName = fileName::null
      ) const = 0;
};
}  // namespace mousse
#endif
