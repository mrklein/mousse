// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::searchableSurfaceControl
// Description
// SourceFiles
//   searchable_surface_control.cpp
#ifndef searchable_surface_control_hpp_
#define searchable_surface_control_hpp_
#include "cell_size_function.hpp"
#include "triad.hpp"
namespace mousse
{
class searchableSurfaceControl
:
  public cellSizeAndAlignmentControl
{
  // Private data
    //- Name of the surface
    const word surfaceName_;
    //- Reference to the searchableSurface object holding the geometry data
    const searchableSurface& searchableSurface_;
    const conformationSurfaces& geometryToConformTo_;
    PtrList<cellSizeFunction> cellSizeFunctions_;
    labelList regionToCellSizeFunctions_;
    label maxPriority_;
//        const conformationSurfaces& geometryToConformTo_;
//
//        //- Indices of surfaces in allGeometry that are to be conformed to
//        labelList surfaces_;
//
//        //- A list of all of the cellSizeFunction objects
//        PtrList<cellSizeFunction> cellSizeFunctions_;
//
//        autoPtr<triangulatedMesh> triangulatedMesh_;
//
//
//    // Private Member Functions
//
//        //-
//        tensor requiredAlignment
//        (
//            const point& pt,
//            const vectorField& ptNormals
//        ) const;
    //- Disallow default bitwise copy construct
    searchableSurfaceControl(const searchableSurfaceControl&);
    //- Disallow default bitwise assignment
    void operator=(const searchableSurfaceControl&);
public:
  //- Runtime type information
  TypeName("searchableSurfaceControl");
  // Constructors
    //- Construct from dictionary and references to conformalVoronoiMesh and
    //  searchableSurfaces
    searchableSurfaceControl
    (
      const Time& runTime,
      const word& name,
      const dictionary& controlFunctionDict,
      const conformationSurfaces& geometryToConformTo,
      const scalar& defaultCellSize
    );
  //- Destructor
  ~searchableSurfaceControl();
  // Member Functions
    // Access
//            //- Return reference to the searchableSurfaces object containing
//            //  all of the geometry
//            inline const searchableSurfaces& geometry() const;
//
//            //- Return the surface indices
//            inline const labelList& surfaces() const;
//
//
//        // Query
//
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
    const PtrList<cellSizeFunction>& sizeFunctions() const
    {
      return cellSizeFunctions_;
    }
    virtual label maxPriority() const
    {
      return maxPriority_;
    }
    bool cellSize
    (
      const mousse::point& pt,
      scalar& cellSize,
      label& priority
    ) const;
    // Edit
};
}  // namespace mousse
#endif
