// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::layerParameters
// Description
//   Simple container to keep together layer specific information.
// SourceFiles
//   layer_parameters.cpp
#ifndef layer_parameters_hpp_
#define layer_parameters_hpp_
#include "dictionary.hpp"
#include "scalar_field.hpp"
#include "label_list.hpp"
#include "switch.hpp"
namespace mousse
{
// Class forward declarations
class polyBoundaryMesh;
class refinementSurfaces;
class layerParameters
{
public:
  // Public data types
    //- Enumeration defining the layer specification:
    //  - first and total thickness specified
    //  - first and expansion ratio specified
    //  - final and total thickness specified
    //  - final and expansion ratio specified
    //  - total thickness and expansion ratio specified
    enum layerSpecification
    {
      ILLEGAL,
      FIRST_AND_TOTAL,
      FIRST_AND_EXPANSION,
      FINAL_AND_TOTAL,
      FINAL_AND_EXPANSION,
      TOTAL_AND_EXPANSION
    };
private:
  // Static data members
    //- Default angle for faces to be convcave
    static const scalar defaultConcaveAngle;
  // Private data
    const dictionary dict_;
    // Per patch (not region!) information
      //- How many layers to add.
      labelList numLayers_;
      //- Are sizes relative to local cell size
      Switch relativeSizes_;
      //- How thickness is specified.
      layerSpecification layerSpec_;
        scalarField firstLayerThickness_;
        scalarField finalLayerThickness_;
        scalarField thickness_;
        scalarField expansionRatio_;
      //- Minimum total thickness
      scalarField minThickness_;
    scalar featureAngle_;
    scalar concaveAngle_;
    label nGrow_;
    scalar maxFaceThicknessRatio_;
    label nBufferCellsNoExtrude_;
    label nLayerIter_;
    label nRelaxedIter_;
    Switch additionalReporting_;
    word meshShrinker_;
  // Private Member Functions
    //- Calculate expansion ratio from overall size v.s. thickness of
    //  first layer.
    scalar layerExpansionRatio
    (
      const label n,
      const scalar totalOverFirst
    ) const;
public:
  // Constructors
    //- Construct from dictionary
    layerParameters(const dictionary& dict, const polyBoundaryMesh&);
    //- Disallow default bitwise copy construct
    layerParameters(const layerParameters&) = delete;
    //- Disallow default bitwise assignment
    layerParameters& operator=(const layerParameters&) = delete;
  // Member Functions
    const dictionary& dict() const
    {
      return dict_;
    }
    // Per patch information
      //- How many layers to add.
      //  -1 : no specification. Assume 0 layers but allow sliding
      //       to make layers
      //   0 : specified to have 0 layers. No sliding allowed.
      //  >0 : number of layers
      const labelList& numLayers() const
      {
        return numLayers_;
      }
      //- Are size parameters relative to inner cell size or
      //  absolute distances.
      bool relativeSizes() const
      {
        return relativeSizes_;
      }
      // Expansion factor for layer mesh
      const scalarField& expansionRatio() const
      {
        return expansionRatio_;
      }
      //- Wanted thickness of the layer furthest away
      //  from the wall (i.e. nearest the original mesh).
      //  If relativeSize() this number is relative to undistorted
      //  size of the cell outside layer.
      const scalarField& finalLayerThickness() const
      {
        return finalLayerThickness_;
      }
      //- Wanted thickness of the layer nearest to the wall.
      //  If relativeSize() this number is relative to undistorted
      //  size of the cell outside layer.
      const scalarField& firstLayerThickness() const
      {
        return firstLayerThickness_;
      }
      //- Wanted overall thickness of all layers.
      //  If relativeSize() this number is relative to undistorted
      //  size of the cell outside layer.
      const scalarField& thickness() const
      {
        return thickness_;
      }
      //- Minimum overall thickness of cell layer. If for any reason layer
      //  cannot be above minThickness do not add layer.
      //  If relativeSize() this number is relative to undistorted
      //  size of the cell outside layer.
      const scalarField& minThickness() const
      {
        return minThickness_;
      }
    // Control
      //- Number of overall layer addition iterations
      label nLayerIter() const
      {
        return nLayerIter_;
      }
      //- Number of iterations after which relaxed motion rules
      //  are to be used.
      label nRelaxedIter() const
      {
        return nRelaxedIter_;
      }
    // Control
      scalar featureAngle() const
      {
        return featureAngle_;
      }
      scalar concaveAngle() const
      {
        return concaveAngle_;
      }
      //- If points get not extruded do nGrow layers of connected faces
      //  that are not grown. Is used to not do layers at all close to
      //  features.
      label nGrow() const
      {
        return nGrow_;
      }
      //- Stop layer growth on highly warped cells
      scalar maxFaceThicknessRatio() const
      {
        return maxFaceThicknessRatio_;
      }
      //- Create buffer region for new layer terminations
      label nBufferCellsNoExtrude() const
      {
        return nBufferCellsNoExtrude_;
      }
      const Switch& additionalReporting() const
      {
        return additionalReporting_;
      }
      //- Type of mesh shrinker
      const word& meshShrinker() const
      {
        return meshShrinker_;
      }
    // Helper
      //- Determine overall thickness. Uses two of the four parameters
      //  according to the layerSpecification
      scalar layerThickness
      (
        const label nLayers,
        const scalar firstLayerThickess,
        const scalar finalLayerThickess,
        const scalar totalThickness,
        const scalar expansionRatio
      ) const;
      //- Determine expansion ratio. Uses two of the four parameters
      //  according to the layerSpecification
      scalar layerExpansionRatio
      (
        const label nLayers,
        const scalar firstLayerThickess,
        const scalar finalLayerThickess,
        const scalar totalThickness,
        const scalar expansionRatio
      ) const;
      //- Determine first layer (near-wall) thickness. Uses two of the
      //  four parameters according to the layerSpecification
      scalar firstLayerThickness
      (
        const label nLayers,
        const scalar firstLayerThickess,
        const scalar finalLayerThickess,
        const scalar totalThickness,
        const scalar expansionRatio
      ) const;
      //- Determine ratio of final layer thickness to
      //  overall layer thickness
      scalar finalLayerThicknessRatio
      (
        const label nLayers,
        const scalar expansionRatio
      ) const;
};
}  // namespace mousse
#endif
