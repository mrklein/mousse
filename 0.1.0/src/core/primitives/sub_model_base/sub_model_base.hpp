#ifndef CORE_PRIMITIVES_SUB_MODEL_BASE_SUB_MODEL_BASE_HPP_
#define CORE_PRIMITIVES_SUB_MODEL_BASE_SUB_MODEL_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::subModelBase
// Description
//   Base class for generic sub-models requiring to be read from dictionary.
//   Provides a mechanism to read and write properties from a dictionary to
//   enable clean re-starts.  Used by, e.g. clou dsub-models.
// SourceFiles
//   sub_model_base.cpp

#include "dictionary.hpp"


namespace mousse {

class subModelBase
{
protected:
  // Protected Data
    //- Name of the sub-model
    const word modelName_;
    //- Reference to properties dictionary e.g. for restart
    dictionary& properties_;
    //- Copy of dictionary used during construction
    const dictionary dict_;
    //- Name of the sub-model base class
    const word baseName_;
    //- Type of the sub-model
    const word modelType_;
    //- Coefficients dictionary
    const dictionary coeffDict_;
  // Protected Member Functions
    //- Flag to indicate whether data is/was read in-line
    bool inLine() const;
public:
  // Constructors
    //- Construct null
    subModelBase(dictionary& properties);
    //- Construct from components without name
    subModelBase
    (
      dictionary& properties,
      const dictionary& dict,
      const word& baseName,
      const word& modelType,
      const word& dictExt = "Coeffs"
    );
    //- Construct from components with name
    subModelBase
    (
      const word& modelName,
      dictionary& properties,
      const dictionary& dict,
      const word& baseName,
      const word& modelType
    );
    //- Construct as copy
    subModelBase(const subModelBase& smb);
    //- Disallow default bitwise assignment
    subModelBase& operator=(const subModelBase&) = delete;
  //- Destructor
  virtual ~subModelBase();
  // Member Functions
    // Access
      //- Return const access to the name of the sub-model
      const word& modelName() const;
      //- Return const access to the cloud dictionary
      const dictionary& dict() const;
      //- Return const access to the base name of the sub-model
      const word& baseName() const;
      //- Return const access to the sub-model type
      const word& modelType() const;
      //- Return const access to the coefficients dictionary
      const dictionary& coeffDict() const;
      //- Return const access to the properties dictionary
      const dictionary& properties() const;
      //- Returns true if defaultCoeffs is true and outputs on printMsg
      virtual bool defaultCoeffs(const bool printMsg) const;
      //- Return the model 'active' status - default active = true
      virtual bool active() const;
      //- Cache dependant sub-model fields
      virtual void cacheFields(const bool store);
      //- Flag to indicate when to write a property
      virtual bool outputTime() const;
    // Edit
      // Base properties
        //- Retrieve generic property from the base model
        template<class Type>
        Type getBaseProperty
        (
          const word& entryName,
          const Type& defaultValue = pTraits<Type>::zero
        ) const;
        //- Retrieve generic property from the base model
        template<class Type>
        void getBaseProperty(const word& entryName, Type& value) const;
        //- Add generic property to the base model
        template<class Type>
        void setBaseProperty(const word& entryName, const Type& value);
      // Model properties
        //- Retrieve generic property from the sub-model
        template<class Type>
        void getModelProperty(const word& entryName, Type& value) const;
        //- Retrieve generic property from the sub-model
        template<class Type>
        Type getModelProperty
        (
          const word& entryName,
          const Type& defaultValue = pTraits<Type>::zero
        ) const;
        //- Add generic property to the sub-model
        template<class Type>
        void setModelProperty(const word& entryName, const Type& value);
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};

}  // namespace mousse

#include "sub_model_base.ipp"

#endif
