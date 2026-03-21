#ifndef POINT_H
#define POINT_H

#include <ostream>

class point
{
public:
   int x;
   int y;
   int pointNum;
};

inline std::ostream& operator << (std::ostream &out, const point & x)
{
   out << x.x << " " << x.y;
return out;
}

#endif
