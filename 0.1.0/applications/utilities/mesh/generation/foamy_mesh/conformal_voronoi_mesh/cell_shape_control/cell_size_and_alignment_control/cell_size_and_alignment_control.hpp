#ifndef UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_HPP_
#define UTILITIES_MESH_GENERATION_FOAMY_MESH_CONFORMAL_VORONOI_MESH_CELL_SHAPE_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_CELL_SIZE_AND_ALIGNMENT_CONTROL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::cellSizeAndAlignmentControl
// Description
// SourceFiles
//   cell_size_and_alignment_control_i.hpp
//   cell_size_and_alignment_control.cpp
#include "dictionary.hpp"
#include "conformation_surfaces.hpp"
#include "time.hpp"
#include "quaternion.hpp"
#include "triad_field.hpp"
namespace mousse
{
class cellSizeAndAlignmentControl
{
protected:
    const Time& runTime_;
    const scalar& defaultCellSize_;
    Switch forceInitialPointInsertion_;
private:
  // Private data
    const word name_;
public:
  //- Runtime type information
  TYPE_NAME("cellSizeAndAlignmentControl");
  // Declare run-time constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      cellSizeAndAlignmentControl,
      dictionary,
      (
        const Time& runTime,
        const word& name,
        const dictionary& controlFunctionDict,
        const conformationSurfaces& geometryToConformTo,
        const scalar& defaultCellSize
      ),
      (
        runTime,
        name,
        controlFunctionDict,
        geometryToConformTo,
        defaultCellSize
      )
    );
  // Constructors
    //- Construct from dictionary and references to conformalVoronoiMesh and
    //  searchableSurfaces
    cellSizeAndAlignmentControl
    (
      const Time& runTime,
      const word& name,
      const dictionary& controlFunctionDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
    //- Disallow default bitwise copy construct
    cellSizeAndAlignmentControl(const cellSizeAndAlignmentControl&) = delete;
    //- Disallow default bitwise assignment
    void operator=(const cellSizeAndAlignmentControl&) = delete;
  // Selectors
    //- Return a reference to the selected cellShapeControl
    static autoPtr<cellSizeAndAlignmentControl> New
    (
      const Time& runTime,
      const word& name,
      const dictionary& controlFunctionDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
  //- Destructor
  virtual ~cellSizeAndAlignmentControl();
  // Member Functions
    // Access
      const word& name() const
      {
        return name_;
      }
      const Switch& forceInitialPointInsertion() const
      {
        return forceInitialPointInsertion_;
      }
    // Query
      virtual label maxPriority() const = 0;
      virtual void cellSizeFunctionVertices
      (
        DynamicList<mousse::point>& pts,
        DynamicList<scalar>& sizes
      ) const = 0;
      virtual void initialVertices
      (
        pointField& pts,
        scalarField& sizes,
        triadField& alignments
      ) const = 0;
};
}  // namespace mousse
#endif
