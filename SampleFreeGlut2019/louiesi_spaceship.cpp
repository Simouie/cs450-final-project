#include "louiesi_enum.cpp"
#include "louiesi_projectile.cpp"
#include "louiesi_struct.cpp"


#ifndef LOUIESI_SPACESHIP
#define LOUIESI_SPACESHIP

class Spaceship {

    protected:

        // spaceship attributes

        float size, speed;

        SpaceshipState state;
        Direction direction;
        Location location;

    public:

        // constructors

        Spaceship(float size, float speed, Direction d, Location l) {

            this -> size = size;
            this -> speed = speed;

            direction = d;
            location = l;

            state = IsAlive;
        
        }

        // getters

        float getSize() { return size; }

        float getSpeed() { return speed; }

        SpaceshipState getState() { return state; }
        
        Direction getDirection() { return direction; }

        Location getLocation() { return location; }

        // setters

        // void setSize(float s) { size = s; }

        // void setSpeed(float s) { speed = s; }

        void setState(SpaceshipState s) { state = s; }

        // void setDirection(Direction d) { direction = d; }

        void setLocation(Location l) { location = l; }

        // functions

        void move(Direction d) {

            switch (d) {

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

        virtual Projectile shoot() = 0;

};

class PlayerSpaceship : public Spaceship {

    public:

        // constructors

        PlayerSpaceship(float s1, float s2, Direction d, Location l) : Spaceship(s1, s2, d, l) {}

        // functions

        Projectile shoot() { 

            if (state == IsDead) return Projectile(); // if the spaceship is dead, return a dead projectile

            // create projectile at the front of the spaceship
            // the player spaceship always faces upward so its projectiles should always go upward too

            Projectile p(size / 4, 0.01, Up, { location.x, location.y + (size * 2), location.z });
            return p;

        }

};

class EnemySpaceship : public Spaceship {

    public:

        // constructors

        EnemySpaceship(float s1, float s2, Direction d, Location l) : Spaceship(s1, s2, d, l) {}

        // functions

        Projectile shoot() { 

            if (state == IsDead) return Projectile(); // if the spaceship is dead, return a dead projectile

            // create projectile at the front of the spaceship
            // enemies always face downward so their projectiles should always go downward too

            Projectile p(size / 2, 0.01, Down, { location.x, location.y - (size * 2), location.z });
            return p;

        }

};

#endif