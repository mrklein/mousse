// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lduInterfaceField
// Description
//   An abstract base class for implicitly-coupled interface fields
//   e.g. processor and cyclic patch fields.
// SourceFiles
//   ldu_interface_field.cpp

#ifndef ldu_interface_field_hpp_
#define ldu_interface_field_hpp_

#include "ldu_interface.hpp"
#include "primitive_fields_fwd.hpp"
#include "pstream.hpp"

namespace mousse
{

class lduMatrix;

class lduInterfaceField
{
  // Private data

    //- Reference to the coupled patch this field is defined for
    const lduInterface& interface_;

    //- Update index used so that updateInterfaceMatrix is called only once
    //  during the construction of the matrix
    bool updatedMatrix_;

public:

  //- Runtime type information
  TYPE_NAME("lduInterfaceField");

  // Constructors

    //- Construct given coupled patch
    lduInterfaceField(const lduInterface& patch)
    :
      interface_{patch},
      updatedMatrix_{false}
    {}

    //- Disallow default bitwise copy construct
    lduInterfaceField(const lduInterfaceField&) = delete;

    //- Disallow default bitwise assignment
    lduInterfaceField& operator=(const lduInterfaceField&) = delete;

  //- Destructor
  virtual ~lduInterfaceField();

  // Member Functions

    // Access

      //- Return the interface
      const lduInterface& interface() const
      {
        return interface_;
      }

      //- Return the interface type
      virtual const word& interfaceFieldType() const
      {
        return type();
      }

    // Coupled interface matrix update

      //- Whether matrix has been updated
      bool updatedMatrix() const
      {
        return updatedMatrix_;
      }

      //- Whether matrix has been updated
      bool& updatedMatrix()
      {
        return updatedMatrix_;
      }

      //- Is all data available
      virtual bool ready() const
      {
        return true;
      }

      //- Initialise neighbour matrix update
      virtual void initInterfaceMatrixUpdate
      (
        scalarField&,
        const scalarField&,
        const scalarField&,
        const direction,
        const Pstream::commsTypes
      ) const
      {}

      //- Update result field based on interface functionality
      virtual void updateInterfaceMatrix
      (
        scalarField&,
        const scalarField&,
        const scalarField&,
        const direction,
        const Pstream::commsTypes commsType
      ) const = 0;

};

}  // namespace mousse

#endif
