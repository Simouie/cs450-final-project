#include "louiesi_enum.cpp"
#include "louiesi_struct.cpp"


#ifndef LOUIESI_PROJECTILE
#define LOUIESI_PROJECTILE

class Projectile { 

    private:

        float size, speed;
        
        bool dead;
    
        Direction direction;
        Location location; 

    public:

        // constructors

        Projectile() {

            size = speed = 0.0;

            direction = Up;
            location = { 0.0, 0.0, 0.0 };

            dead = true;

        }

        Projectile(float size, float speed, Direction d, Location l) {

            this -> size = size;
            this -> speed = speed;

            direction = d;
            location = l;

            dead = false;

        }
        
        // getters 

        float getSize() { return size; }

        float getSpeed() { return speed; }

        bool isDead() { return dead; }

        bool isAlive() { return !dead; }

        Direction getDirection() { return direction; }

        Location getLocation() { return location; }

        // setters 

        // void setSize(float s) { size = s; }

        // void setSpeed(float s) { speed = s; }

        void setAlive() { dead = false; }

        void setDead() { dead = true; }

        // void setDirection(Direction d) { direction = d; }

        // void setLocation(Location l) { location = l; }

        // functions

        void move() {

            switch (direction) {

                case Up:
                    location.y += speed;
                    break;

                case Down:
                    location.y -= speed;
                    break;

                case Left:
                    location.x -= speed;
                    break;

                case Right:
                    location.x += speed;
                    break;

            }

            return;

        }
    
};

#endif