#ifndef REGION_MODELS_REGION_MODEL_REGION_MODEL_1D_HPP_
#define REGION_MODELS_REGION_MODEL_REGION_MODEL_1D_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModel1D
// Description
//   Base class for 1-D region models
// SourceFiles
//   region_model_1d.cpp
#include "region_model.hpp"
#include "surface_fields.hpp"
#include "label_list.hpp"
namespace mousse
{
namespace regionModels
{
class regionModel1D
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
    // Region addressing - per internally coupled patch face walking out
      //- Global face IDs
      labelListList boundaryFaceFaces_;
      //- Global cell IDs
      labelListList boundaryFaceCells_;
      //- Global boundary face IDs oppossite coupled patch
      labelList boundaryFaceOppositeFace_;
      //- Number of layers in the region
      label nLayers_;
    // Geometry
      //- Face area magnitude normal to patch
      autoPtr<surfaceScalarField> nMagSfPtr_;
      //- Flag to allow mesh movement
      Switch moveMesh_;
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
    //- Read control parameters from dictionary
    virtual bool read(const dictionary& dict);
    //- Move mesh points according to change in cell volumes
    //  Returns map ordered by cell where 1 = cell moved, 0 = cell unchanged
    tmp<labelField> moveMesh
    (
      const scalarList& deltaV,
      const scalar minDelta = 0.0
    );
public:
  //- Runtime type information
  TYPE_NAME("regionModel1D");
  // Constructors
    //- Construct null
    regionModel1D
    (
      const fvMesh& mesh,
      const word& regionType
    );
    //- Construct from mesh, region type and name
    regionModel1D
    (
      const fvMesh& mesh,
      const word& regionType,
      const word& modelName,
      bool readFields = true
    );
    //- Construct from mesh, region type and name and dict
    regionModel1D
    (
      const fvMesh& mesh,
      const word& regionType,
      const word& modelName,
      const dictionary& dict,
      bool readFields = true
    );
    //- Disallow default bitwise copy construct
    regionModel1D(const regionModel1D&) = delete;
    //- Disallow default bitwise assignment
    regionModel1D& operator=(const regionModel1D&) = delete;
  //- Destructor
  virtual ~regionModel1D();
  // Member Functions
    // Access
      // Addressing
        //- Return the global face IDs
        inline const labelListList& boundaryFaceFaces() const;
        //- Return the global cell IDs
        inline const labelListList& boundaryFaceCells() const;
        //- Return the global boundary face IDs oppossite coupled patch
        inline const labelList& boundaryFaceOppositeFace() const;
      // Geometry
        //- Return the face area magnitudes / [m2]
        inline const surfaceScalarField& nMagSf() const;
        //- Return the number of layers in the region
        inline label nLayers() const;
};
}  // namespace regionModels
}  // namespace mousse

inline const mousse::labelListList&
mousse::regionModels::regionModel1D::boundaryFaceFaces() const
{
  return boundaryFaceFaces_;
}
inline const mousse::labelListList&
mousse::regionModels::regionModel1D::boundaryFaceCells() const
{
  return boundaryFaceCells_;
}
inline const mousse::labelList&
mousse::regionModels::regionModel1D::boundaryFaceOppositeFace() const
{
  return boundaryFaceOppositeFace_;
}
inline const mousse::surfaceScalarField&
mousse::regionModels::regionModel1D::nMagSf() const
{
  if (!nMagSfPtr_.valid())
  {
    FATAL_ERROR_IN
    (
      "inline const mousse::surfaceScalarField&"
      "mousse::regionModel1Ds::regionModel1D::nMagSf() const"
    )
    << "Face normal areas not available" << abort(FatalError);
  }
  return nMagSfPtr_();
}
inline mousse::label mousse::regionModels::regionModel1D::nLayers() const
{
  return nLayers_;
}
#endif
