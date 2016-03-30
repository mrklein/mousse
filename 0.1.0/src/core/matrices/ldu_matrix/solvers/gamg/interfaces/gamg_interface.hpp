#ifndef CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACES_GAMG_INTERFACE_HPP_
#define CORE_MATRICES_LDU_MATRIX_SOLVERS_GAMG_INTERFACES_GAMG_INTERFACE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::GAMGInterface
// Description
//   Abstract base class for GAMG agglomerated interfaces.

#include "auto_ptr.hpp"
#include "ldu_interface_ptrs_list.hpp"
#include "gamg_agglomeration.hpp"


namespace mousse {

class GAMGInterface
:
  public lduInterface
{
protected:

  // Protected data

    //- My index in coarseInterfaces
    const label index_;

    //- All interfaces
    const lduInterfacePtrsList& coarseInterfaces_;

    //- Face-cell addressing
    labelList faceCells_;

    //- Face restrict addressing
    labelList faceRestrictAddressing_;

public:

  //- Runtime type information
  TYPE_NAME("GAMGInterface");

  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      GAMGInterface,
      lduInterface,
      (
        const label index,
        const lduInterfacePtrsList& coarseInterfaces,
        const lduInterface& fineInterface,
        const labelField& localRestrictAddressing,
        const labelField& neighbourRestrictAddressing,
        const label fineLevelIndex,
        const label coarseComm
      ),
      (
        index,
        coarseInterfaces,
        fineInterface,
        localRestrictAddressing,
        neighbourRestrictAddressing,
        fineLevelIndex,
        coarseComm
      )
    );

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      GAMGInterface,
      Istream,
      (
        const label index,
        const lduInterfacePtrsList& coarseInterfaces,
        Istream& is
      ),
      (
        index,
        coarseInterfaces,
        is
      )
    );

  // Selectors

    //- Return a pointer to a new interface created on freestore given
    //  the fine interface
    static autoPtr<GAMGInterface> New
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const lduInterface& fineInterface,
      const labelField& localRestrictAddressing,
      const labelField& neighbourRestrictAddressing,
      const label fineLevelIndex,
      const label coarseComm
    );

    //- Return a pointer to a new interface created on freestore given
    //  the fine interface
    static autoPtr<GAMGInterface> New
    (
      const word& coupleType,
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      Istream& is
    );

  // Constructors

    //- Construct from interfaces, restrict addressing set later on
    GAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces
    )
    :
      index_{index},
      coarseInterfaces_{coarseInterfaces}
    {}

    //- Construct from interfaces and restrict addressing
    GAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      const labelUList& faceCells,
      const labelUList& faceRestrictAddressing
    )
    :
      index_{index},
      coarseInterfaces_{coarseInterfaces},
      faceCells_{faceCells},
      faceRestrictAddressing_{faceRestrictAddressing}
    {}

    //- Construct from Istream
    GAMGInterface
    (
      const label index,
      const lduInterfacePtrsList& coarseInterfaces,
      Istream& is
    );

    //- Disallow default bitwise copy construct
    GAMGInterface(const GAMGInterface&) = delete;

    //- Disallow default bitwise assignment
    GAMGInterface& operator=(const GAMGInterface&) = delete;

  // Member Functions

    // Access

      //- Return size
      virtual label size() const
      {
        return faceCells_.size();
      }

      virtual label index() const
      {
        return index_;
      }

      virtual const lduInterfacePtrsList& coarseInterfaces() const
      {
        return coarseInterfaces_;
      }

      //- Return faceCell addressing
      virtual const labelUList& faceCells() const
      {
        return faceCells_;
      }

      //- Return (local)face restrict addressing
      virtual const labelList& faceRestrictAddressing() const
      {
        return faceRestrictAddressing_;
      }

      //- Return non-const access to face restrict addressing
      virtual labelList& faceRestrictAddressing()
      {
        return faceRestrictAddressing_;
      }

      //- Return the interface internal field of the given field
      template<class Type>
      tmp<Field<Type> > interfaceInternalField
      (
        const UList<Type>& internalData
      ) const;

      //- Get the interface internal field of the given field
      template<class Type>
      void interfaceInternalField
      (
        const UList<Type>& internalData,
        List<Type>&
      ) const;

      //- Return the values of the given internal data adjacent to
      //  the interface as a field
      virtual tmp<labelField> interfaceInternalField
      (
        const labelUList& internalData
      ) const;

    // Agglomeration

      //- Merge the next level with this level
      //  combining the face-restrict addressing
      //  and copying the face-cell addressing
      void combine(const GAMGInterface&);

      //- Agglomerating the given fine-level coefficients and return
      virtual tmp<scalarField> agglomerateCoeffs
      (
        const scalarField& fineCoeffs
      ) const;

    // I/O

      //- Write to stream
      virtual void write(Ostream&) const = 0;
};
}  // namespace mousse

#include "gamg_interface.ipp"

#endif
