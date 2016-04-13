#ifndef CORE_FIELDS_FIELDS_FIELD_MAPPER_HPP_
#define CORE_FIELDS_FIELDS_FIELD_MAPPER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::FieldMapper
// Description
//   Abstract base class to hold the Field mapping addressing and weights.

namespace mousse {

class FieldMapper
{
public:
  // Constructors
    //- Null constructor
    FieldMapper()
    {}
  //- Destructor
  virtual ~FieldMapper()
  {}
  // Member Functions
    virtual label size() const = 0;
    virtual bool direct() const = 0;
    //- Are there unmapped values? I.e. do all size() elements get
    //  get value
    virtual bool hasUnmapped() const = 0;
    virtual const labelUList& directAddressing() const
    {
      FATAL_ERROR_IN("FieldMapper::directAddressing() const")
        << "attempt to access null direct addressing"
        << abort(FatalError);
      return labelUList::null();
    }
    virtual const labelListList& addressing() const
    {
      FATAL_ERROR_IN("FieldMapper::addressing() const")
        << "attempt to access null interpolation addressing"
        << abort(FatalError);
      return labelListList::null();
    }
    virtual const scalarListList& weights() const
    {
      FATAL_ERROR_IN("FieldMapper::weights() const")
        << "attempt to access null interpolation weights"
        << abort(FatalError);
      return scalarListList::null();
    }
  // Member Operators
    template<class Type>
    tmp<Field<Type> > operator()(const Field<Type>& f) const
    {
      return tmp<Field<Type>>{new Field<Type>{f, *this}};
    }
};
}  // namespace mousse
#endif
