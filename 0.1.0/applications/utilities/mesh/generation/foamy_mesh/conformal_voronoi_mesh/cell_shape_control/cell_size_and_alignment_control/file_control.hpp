#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_FILE_CONTROL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_FILE_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileControl
// Description
// SourceFiles
//   file_control.cpp
#include "cell_size_and_alignment_control.hpp"
namespace mousse
{
class fileControl
:
  public cellSizeAndAlignmentControl
{
  // Private data
    const fileName pointsFile_;
    const fileName sizesFile_;
    const fileName alignmentsFile_;
    label maxPriority_;
public:
  //- Runtime type information
  TYPE_NAME("fileControl");
  // Constructors
    //- Construct from dictionary and references to conformalVoronoiMesh and
    //  searchableSurfaces
    fileControl
    (
      const Time& runTime,
      const word& name,
      const dictionary& controlFunctionDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
    //- Disallow default bitwise copy construct
    fileControl(const fileControl&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const fileControl&) = delete;
  //- Destructor
  ~fileControl();
  // Member Functions
    // Access
    // Query
//            //- Return the cell size at the given location
//            virtual scalar cellSize(const point& pt) const;
//
//            //- Return the cell alignment at the given location
//            virtual tensor cellAlignment(const point& pt) const;
//
//            virtual void cellSizeAndAlignment
//            (
//                const point& pt,
//                scalar& size,
//                tensor& alignment
//            ) const;
      virtual label maxPriority() const
      {
        return maxPriority_;
      }
    // Edit
      virtual void cellSizeFunctionVertices
      (
        DynamicList<mousse::point>& pts,
        DynamicList<scalar>& sizes
      ) const;
      virtual void initialVertices
      (
        pointField& pts,
        scalarField& sizes,
        triadField& alignments
      ) const;
};
}  // namespace mousse
#endif
