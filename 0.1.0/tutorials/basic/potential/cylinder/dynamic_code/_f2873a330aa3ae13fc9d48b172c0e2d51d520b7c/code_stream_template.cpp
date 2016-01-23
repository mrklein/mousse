// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dictionary.hpp"
#include "ostream.hpp"
#include "pstream.hpp"
#include "unit_conversion.hpp"
//{{{ begin codeInclude
#line 15 "/Users/alexey/Projects/mousse/0.1.0/tutorials/basic/mousse-potential/cylinder/system/blockMeshDict.#codeStream"
#include "point_field.hpp"
//}}} end codeInclude
namespace mousse
{
// Local Functions
//{{{ begin localCode

//}}} end localCode
// Global Functions 
extern "C"
{
  void codeStream_f2873a330aa3ae13fc9d48b172c0e2d51d520b7c
  (
    Ostream& os,
    const dictionary& dict
  )
  {
//{{{ begin code
    #line 20 "/Users/alexey/Projects/mousse/0.1.0/tutorials/basic/mousse-potential/cylinder/system/blockMeshDict.#codeStream"
pointField points(19);
        points[0]  = point(0.5, 0, -0.5);
        points[1]  = point(1, 0, -0.5);
        points[2]  = point(2, 0, -0.5);
        points[3]  = point(2, 0.707107, -0.5);
        points[4]  = point(0.707107, 0.707107, -0.5);
        points[5]  = point(0.353553, 0.353553, -0.5);
        points[6]  = point(2, 2, -0.5);
        points[7]  = point(0.707107, 2, -0.5);
        points[8]  = point(0, 2, -0.5);
        points[9]  = point(0, 1, -0.5);
        points[10] = point(0, 0.5, -0.5);
        points[11] = point(-0.5, 0, -0.5);
        points[12] = point(-1, 0, -0.5);
        points[13] = point(-2, 0, -0.5);
        points[14] = point(-2, 0.707107, -0.5);
        points[15] = point(-0.707107, 0.707107, -0.5);
        points[16] = point(-0.353553, 0.353553, -0.5);
        points[17] = point(-2, 2, -0.5);
        points[18] = point(-0.707107, 2, -0.5);

        // Duplicate z points
        label sz = points.size();
        points.setSize(2*sz);
        for (label i = 0; i < sz; i++)
        {
            const point& pt = points[i];
            points[i+sz] = point(pt.x(), pt.y(), -pt.z());
        }

        os  << points;
//}}} end code
  }
}
}  // namespace mousse

