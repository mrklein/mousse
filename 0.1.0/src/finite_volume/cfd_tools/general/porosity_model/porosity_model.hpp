// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::porosityModel
// Description
//   Top level model for porosity models
// SourceFiles
//   porosity_model.cpp
//   porosity_model_new.cpp

#ifndef porosity_model_hpp_
#define porosity_model_hpp_

#include "fv_mesh.hpp"
#include "dictionary.hpp"
#include "fv_matrices_fwd.hpp"
#include "run_time_selection_tables.hpp"
#include "coordinate_system.hpp"
#include "dimensioned_vector.hpp"
#include "key_type.hpp"

namespace mousse
{
class porosityModel
:
  public regIOobject
{
protected:
  // Protected data
    //- Porosity name
    word name_;
    //- Reference to the mesh database
    const fvMesh& mesh_;
    //- Dictionary used for model construction
    const dictionary dict_;
    //- Model coefficients dictionary
    dictionary coeffs_;
    //- Porosity active flag
    bool active_;
    //- Name(s) of cell-zone
    keyType zoneName_;
    //- Cell zone IDs
    labelList cellZoneIDs_;
    //- Local co-ordinate system
    coordinateSystem coordSys_;
  // Protected Member Functions
    //- Transform the model data wrt mesh changes
    virtual void calcTranformModelData() = 0;
    //- Adjust negative resistance values to be multiplier of max value
    void adjustNegativeResistance(dimensionedVector& resist);
    //- Calculate the porosity force
    virtual void calcForce
    (
      const volVectorField& U,
      const volScalarField& rho,
      const volScalarField& mu,
      vectorField& force
    ) const = 0;
    virtual void correct(fvVectorMatrix& UEqn) const = 0;
    virtual void correct
    (
      fvVectorMatrix& UEqn,
      const volScalarField& rho,
      const volScalarField& mu
    ) const = 0;
    virtual void correct
    (
      const fvVectorMatrix& UEqn,
      volTensorField& AU
    ) const = 0;
    //- Return label index
    label fieldIndex(const label index) const;

public:
  //- Runtime type information
  TYPE_NAME("porosityModel");
  //- Selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    porosityModel,
    mesh,
    (
      const word& modelName,
      const word& name,
      const fvMesh& mesh,
      const dictionary& dict,
      const word& cellZoneName
    ),
    (modelName, name, mesh, dict, cellZoneName)
  );
  //- Constructor
  porosityModel
  (
    const word& name,
    const word& modelType,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& cellZoneName = word::null
  );
  //- Return pointer to new porosityModel object created on the freestore
  //  from an Istream
  class iNew
  {
    //- Reference to the mesh database
    const fvMesh& mesh_;
    const word& name_;
  public:
    iNew
    (
      const fvMesh& mesh,
      const word& name
    )
    :
      mesh_(mesh),
      name_(name)
    {}
    autoPtr<porosityModel> operator()(Istream& is) const
    {
      const dictionary dict(is);
      return autoPtr<porosityModel>
      (
        porosityModel::New
        (
          name_,
          mesh_,
          dict
        )
      );
    }
  };
  //- Selector
  static autoPtr<porosityModel> New
  (
    const word& name,
    const fvMesh& mesh,
    const dictionary& dict,
    const word& cellZoneName = word::null
  );
  //- Disallow default bitwise copy construct
  porosityModel(const porosityModel&) = delete;
  //- Disallow default bitwise assignment
  porosityModel& operator=(const porosityModel&) = delete;
  //- Destructor
  virtual ~porosityModel();
  // Member Functions
    //- Return const access to the porosity model name
    inline const word& name() const;
    //- Return const access to the porosity active flag
    inline bool active() const;
    //- Return const access to the cell zone IDs
    inline const labelList& cellZoneIDs() const;
    //- Transform the model data wrt mesh changes
    virtual void transformModelData();
    //- Return the force over the cell zone(s)
    virtual tmp<vectorField> force
    (
      const volVectorField& U,
      const volScalarField& rho,
      const volScalarField& mu
    );
    //- Add resistance
    virtual void addResistance(fvVectorMatrix& UEqn);
    //- Add resistance
    virtual void addResistance
    (
      fvVectorMatrix& UEqn,
      const volScalarField& rho,
      const volScalarField& mu
    );
    //- Add resistance
    virtual void addResistance
    (
      const fvVectorMatrix& UEqn,
      volTensorField& AU,
      bool correctAUprocBC
    );
  // I-O
    //- Write
    virtual bool writeData(Ostream& os) const;
    //- Inherit read from regIOobject
    using regIOobject::read;
    //- Read porosity dictionary
    virtual bool read(const dictionary& dict);
};
}  // namespace mousse
inline const mousse::word& mousse::porosityModel::name() const
{
  return name_;
}
inline bool mousse::porosityModel::active() const
{
  return active_;
}
inline const mousse::labelList& mousse::porosityModel::cellZoneIDs() const
{
  return cellZoneIDs_;
}
#endif
