#ifndef UVEC3_H_INCLUDED
#define UVEC3_H_INCLUDED

struct uvec3 {
	unsigned int x, y, z;

    uvec3 () {}

    uvec3(unsigned int _x, unsigned int _y, unsigned int _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

#endif 
