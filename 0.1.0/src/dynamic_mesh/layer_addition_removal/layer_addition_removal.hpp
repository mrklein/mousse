// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::layerAdditionRemoval
// Description
//   Cell layer addition mesh modifier
// SourceFiles
//   layer_addition_removal.cpp
//   add_cell_layer.cpp
//   remove_cell_layer.cpp
#ifndef layer_addition_removal_hpp_
#define layer_addition_removal_hpp_
#include "poly_mesh_modifier.hpp"
#include "primitive_face_patch.hpp"
#include "zone_i_ds.hpp"
namespace mousse
{
class layerAdditionRemoval
:
  public polyMeshModifier
{
  // Private data
    //- Master face zone ID
    faceZoneID faceZoneID_;
    //- Min thickness of extrusion layer.  Triggers layer removal
    mutable scalar minLayerThickness_;
    //- Max thickness of extrusion layer.  Triggers layer addition
    mutable scalar maxLayerThickness_;
    //- Switch to calculate thickness as volume/area
    //  If false, thickness calculated from edges
    const bool thicknessFromVolume_;
    //- Layer thickness from previous step
    //  Used to decide whether to add or remove layers
    mutable scalar oldLayerThickness_;
    //- Point pairing
    mutable labelList* pointsPairingPtr_;
    //- Face pairing
    mutable labelList* facesPairingPtr_;
    //- Layer removal trigger time index
    mutable label triggerRemoval_;
    //- Layer addition trigger time index
    mutable label triggerAddition_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    layerAdditionRemoval(const layerAdditionRemoval&);
    //- Disallow default bitwise assignment
    void operator=(const layerAdditionRemoval&);
    //- Check validity of construction data
    void checkDefinition();
    // Topological changes
      //- Check for valid layer
      bool validCollapse() const;
      //- Set layer pairing.  Return true if a valid layer exists
      bool setLayerPairing() const;
      //- Return points pairing in a layer (not automatic!)
      const labelList& pointsPairing() const;
      //- Return faces pairing in a layer (not automatic!)
      const labelList& facesPairing() const;
      //- Calculate the offset to the next layer
      tmp<vectorField> extrusionDir() const;
      //- Add a layer of cells
      void addCellLayer(polyTopoChange&) const;
      //- Remove a layer of cells
      void removeCellLayer(polyTopoChange&) const;
      //- Clear addressing
      void clearAddressing() const;
    // Helpers
      //- Optionally read old thickness
      static scalar readOldThickness(const dictionary&);
  // Static data members
    //- Thickness insertion fraction for the pre-motion
    static const scalar addDelta_;
    //- Thickness removal fraction for the cell collapse
    //  Note: the cell will be collapsed to this relative
    //  thickness before the layer is removed.
    static const scalar removeDelta_;
public:
  //- Runtime type information
  TypeName("layerAdditionRemoval");
  // Constructors
    //- Construct from components
    layerAdditionRemoval
    (
      const word& name,
      const label index,
      const polyTopoChanger& ptc,
      const word& zoneName,
      const scalar minThickness,
      const scalar maxThickness,
      const Switch thicknessFromVolume = true
    );
    //- Construct from dictionary
    layerAdditionRemoval
    (
      const word& name,
      const dictionary& dict,
      const label index,
      const polyTopoChanger& ptc
    );
  //- Destructor
  virtual ~layerAdditionRemoval();
  // Member Functions
    //- Check for topology change
    virtual bool changeTopology() const;
    //- Insert the layer addition/removal instructions
    //  into the topological change
    virtual void setRefinement(polyTopoChange&) const;
    //- Modify motion points to comply with the topological change
    virtual void modifyMotionPoints(pointField& motionPoints) const;
    //- Force recalculation of locally stored data on topological change
    virtual void updateMesh(const mapPolyMesh&);
    // Edit
      //- Return min layer thickness which triggers removal
      scalar minLayerThickness() const
      {
        return minLayerThickness_;
      }
      //- Set min layer thickness which triggers removal
     void setMinLayerThickness(const scalar t) const;
      //- Return max layer thickness which triggers removal
      scalar maxLayerThickness() const
      {
        return maxLayerThickness_;
      }
      //- Set max layer thickness which triggers removal
     void setMaxLayerThickness(const scalar t) const;
    //- Write
    virtual void write(Ostream&) const;
    //- Write dictionary
    virtual void writeDict(Ostream&) const;
};
}  // namespace mousse
#endif
