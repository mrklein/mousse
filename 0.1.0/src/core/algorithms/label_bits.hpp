// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::labelBits
// Description
//   A 29bits label and 3bits direction packed into single label
// SourceFiles

#ifndef label_bits_hpp_
#define label_bits_hpp_

#include "label.hpp"
//#include "ulabel.hpp"
#include "direction.hpp"
#include "error.hpp"

namespace mousse
{

class labelBits
{
  // Private data
    label data_;
    inline static label pack(const label val, const direction bits)
    {
#ifdef FULLDEBUG
      if (bits > 7 || (((val<<3)>>3) != val))
      {
        FATAL_ERROR_IN
        (
          "labelBits::pack(const label, const direction)"
        )
        << "Direction " << bits << " outside range 0..7"
        << " or value " << val << " negative or larger than "
        << label(8*sizeof(label)-3) << " bit representation"
        << abort(FatalError);
      }
#endif
      return (val<<3) | bits;
    }

public:
  // Constructors
    //- Construct null
    inline labelBits()
    {}

    //- Construct from components
    inline labelBits(const label val, const direction bits)
    :
      data_{pack(val, bits)}
    {}

    //- Construct from Istream
    inline labelBits(Istream& is)
    {
      is >> data_;
    }

  // Member Functions
    inline label val() const
    {
      return data_ >> 3;
    }

    inline direction bits() const
    {
      return data_ & 7;
    }

    inline void setVal(const label val)
    {
      data_ = pack(val, bits());
    }

    inline void setBits(const direction bits)
    {
      data_ = pack(val(), bits);
    }

  // Member Operators
    inline friend bool operator==(const labelBits& a, const labelBits& b)
    {
      return a.data_ == b.data_;
    }

    inline friend bool operator!=(const labelBits& a, const labelBits& b)
    {
      return !(a == b);
    }

  // IOstream Operators
    inline friend Istream& operator>>(Istream& is, labelBits& lb)
    {
      return is >> lb.data_;
    }

    inline friend Ostream& operator<<(Ostream& os, const labelBits& lb)
    {
      return os << lb.data_;
    }
};

}  // namespace mousse
#endif
