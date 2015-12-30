// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::InflowBoundaryModel
// Description
//   Templated inflow boundary model class
// SourceFiles
//   _inflow_boundary_model.cpp
//   _inflow_boundary_model_new.cpp
#ifndef _inflow_boundary_model_hpp_
#define _inflow_boundary_model_hpp_

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"

namespace mousse
{
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
  TypeName("InflowBoundaryModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
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
  static autoPtr<InflowBoundaryModel<CloudType> > New
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

#define makeInflowBoundaryModel(CloudType)                                    \
                                                                              \
  defineNamedTemplateTypeNameAndDebug(InflowBoundaryModel<CloudType>, 0);     \
                                                                              \
  defineTemplateRunTimeSelectionTable                                         \
  (                                                                           \
    InflowBoundaryModel<CloudType>,                                           \
    dictionary                                                                \
  );

#define makeInflowBoundaryModelType(SS, CloudType)                            \
                                                                              \
  defineNamedTemplateTypeNameAndDebug(SS<CloudType>, 0);                      \
                                                                              \
  InflowBoundaryModel<CloudType>::                                            \
    adddictionaryConstructorToTable<SS<CloudType> >                           \
      add##SS##CloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_inflow_boundary_model.cpp"
#endif
#endif
