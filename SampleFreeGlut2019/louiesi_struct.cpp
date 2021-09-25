#ifndef LOUIESI_STRUCT
#define LOUIESI_STRUCT

struct Location { float x, y, z; };

struct Particle {

    float x, y, z;
    float vx, vy, vz;

};

struct Explosion {

    float x, y, z;
    float r, g, b;

    float startTime, endTime;
    
    Particle *particles;

};

#endif