#ifndef REGION_MODELS_REGION_MODEL_SINGLE_LAYER_REGION_HPP_
#define REGION_MODELS_REGION_MODEL_SINGLE_LAYER_REGION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::singleLayerRegion
// Description
//   Base class for single layer region models

#include "region_model.hpp"


namespace mousse {
namespace regionModels {

class singleLayerRegion
:
  public regionModel
{
private:
  // Private Member Functions
    //- Construct region mesh and fields
    void constructMeshObjects();
    //- Initialise the region
    void initialise();
protected:
  // Protected data
    // Region addressing
      //- Patch normal vectors
      autoPtr<volVectorField> nHatPtr_;
      //- Face area magnitudes / [m2]
      autoPtr<volScalarField> magSfPtr_;
    // Addressing
      //- List of patch IDs opposite to internally coupled patches
      labelList passivePatchIDs_;
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
public:
  //- Runtime type information
  TYPE_NAME("regionModel");
  // Constructors
    //- Construct null
    singleLayerRegion(const fvMesh& mesh, const word& regionType);
    //- Construct from mesh, region type and name
    singleLayerRegion
    (
      const fvMesh& mesh,
      const word& regionType,
      const word& modelName,
      bool readFields = true
    );
    //- Disallow default bitwise copy construct
    singleLayerRegion(const singleLayerRegion&) = delete;
    //- Disallow default bitwise assignment
    singleLayerRegion& operator=(const singleLayerRegion&) = delete;
  //- Destructor
  virtual ~singleLayerRegion();
  // Member Functions
    // Access
      // Region geometry
        //- Return the patch normal vectors
        virtual const volVectorField& nHat() const;
        //- Return the face area magnitudes / [m2]
        virtual const volScalarField& magSf() const;
      // Addressing
        //- Return the list of patch IDs opposite to internally
        //  coupled patches
        virtual const labelList& passivePatchIDs() const;
    // Patch type information
      //- Return boundary types for mapped field patches
      //  Also maps internal field value
      //  Mapping region prescribed by underlying mapped poly patch
      template<class Type>
      wordList mappedFieldAndInternalPatchTypes() const;
      //- Return boundary types for pushed mapped field patches
      //  Mapping region prescribed by underlying mapped poly patch
      template<class Type>
      wordList mappedPushedFieldPatchTypes() const;
};

}  // namespace regionModels
}  // namespace mousse

#include "single_layer_region.ipp"

#endif
