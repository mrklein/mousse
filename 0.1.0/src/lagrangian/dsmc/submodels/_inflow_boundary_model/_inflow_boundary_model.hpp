#ifndef LAGRANGIAN_DSMC_SUBMODELS_TINFLOW_BOUNDARY_MODEL_TINFLOW_BOUNDARY_MODEL_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TINFLOW_BOUNDARY_MODEL_TINFLOW_BOUNDARY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InflowBoundaryModel
// Description
//   Templated inflow boundary model class

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {

template<class CloudType>
class InflowBoundaryModel
{
  // Private data
    //- The cloud dictionary
    const dictionary& dict_;
    // Reference to the owner cloud class
    CloudType& owner_;
    //- The coefficients dictionary
    const dictionary coeffDict_;
public:
  //- Runtime type information
  TYPE_NAME("InflowBoundaryModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    InflowBoundaryModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    InflowBoundaryModel(CloudType& owner);
    //- Construct from dictionary
    InflowBoundaryModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
  //- Destructor
  virtual ~InflowBoundaryModel();
  //- Selector
  static autoPtr<InflowBoundaryModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Access
    //- Return const access the owner cloud object
    inline const CloudType& owner() const;
    //- Return non-const access the owner cloud object for manipulation
    inline CloudType& owner();
    //- Return the owner cloud dictionary
    inline const dictionary& dict() const;
    //- Return the coefficients dictionary
    inline const dictionary& coeffDict() const;
  // Mapping
    //- Remap the particles to the correct cells following mesh change
    virtual void autoMap(const mapPolyMesh&)
    {}
  //- Introduce particles
  virtual void inflow() = 0;
};

}  // namespace mousse


#define MAKE_INFLOW_BOUNDARY_MODEL(CloudType)                                 \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(InflowBoundaryModel<CloudType>, 0);\
                                                                              \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    InflowBoundaryModel<CloudType>,                                           \
    dictionary                                                                \
  );

#define MAKE_INFLOW_BOUNDARY_MODEL_TYPE(SS, CloudType)                        \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<CloudType>, 0);                \
                                                                              \
  InflowBoundaryModel<CloudType>::                                            \
    adddictionaryConstructorToTable<SS<CloudType>>                            \
      add##SS##CloudType##ConstructorToTable_;


#include "_inflow_boundary_model.ipp"

#endif

