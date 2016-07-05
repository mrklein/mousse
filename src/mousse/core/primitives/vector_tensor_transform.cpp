// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vector_tensor_transform.hpp"
#include "iostreams.hpp"
#include "ostring_stream.hpp"


// Static Data Members
const char* const mousse::vectorTensorTransform::typeName =
  "vectorTensorTransform";
const mousse::vectorTensorTransform mousse::vectorTensorTransform::zero
(
  vector::zero,
  tensor::zero,
  false
);


const mousse::vectorTensorTransform mousse::vectorTensorTransform::I
(
  vector::zero,
  sphericalTensor::I,
  false
);


// Constructors 
mousse::vectorTensorTransform::vectorTensorTransform(Istream& is)
{
  is  >> *this;
}


// Member Functions 
mousse::word mousse::name(const vectorTensorTransform& s)
{
  OStringStream buf;
  buf << '(' << s.t() << ',' << s.R() << ')';
  return buf.str();
}


template<>
mousse::tmp<mousse::Field<bool> > mousse::vectorTensorTransform::transform
(
  const Field<bool>& fld
) const
{
  return fld;
}


template<>
mousse::tmp<mousse::Field<mousse::label> > mousse::vectorTensorTransform::transform
(
  const Field<label>& fld
) const
{
  return fld;
}


template<>
mousse::tmp<mousse::Field<mousse::scalar> > mousse::vectorTensorTransform::transform
(
  const Field<scalar>& fld
) const
{
  return fld;
}


// IOstream Operators 
mousse::Istream& mousse::operator>>(Istream& is, vectorTensorTransform& tr)
{
  // Read beginning of vectorTensorTransform
  is.readBegin("vectorTensorTransform");
  is >> tr.t_ >> tr.R_ >> tr.hasR_;
  // Read end of vectorTensorTransform
  is.readEnd("vectorTensorTransform");
  // Check state of Istream
  is.check("operator>>(Istream&, vectorTensorTransform&)");
  return is;
}


mousse::Ostream& mousse::operator<<(Ostream& os, const vectorTensorTransform& tr)
{
  os << token::BEGIN_LIST
    << tr.t() << token::SPACE << tr.R() << token::SPACE << tr.hasR()
    << token::END_LIST;
  return os;
}

