#include <cstdlib>
#include <ctime>
#include <deque>

#include "louiesi_enum.cpp"
#include "louiesi_projectile.cpp"
#include "louiesi_spaceship.cpp"
#include "louiesi_struct.cpp"

using std::deque;


#ifndef LOUIESI_GAME
#define LOUIESI_GAME

class Game {

    private:

        // variables

        int score, spaceshipValue;

        float xLimit, yLimit;

        float timeLastEnemyFire, timeLastEnemySpawn;

        bool w, a, s, d;

        PlayerSpaceship *player;

        deque <EnemySpaceship> enemies;
        deque <Projectile> projectiles;

        deque <Location> explosionLocations;
        
        // game boundary functions

        bool outOfBounds(Location l) {
            
            bool badX = l.x > xLimit || l.x < -xLimit;
            bool badY = l.y > yLimit || l.y < -yLimit;

            return badX || badY;

        }

        void fixPlayerLocation() {

            // if the player reaches or goes beyond the boundaries 
            // move the player back inside the boundaries

            float s = player -> getSize();
            Location l = player -> getLocation();

            if (l.x + s > xLimit) l.x = xLimit - s;

            if (l.x - s < -xLimit) l.x = -xLimit + s;

            if (l.y + s * 2 > yLimit) l.y = yLimit - s * 2;

            if (l.y < -yLimit) l.y = -yLimit;

            player -> setLocation(l);

        }

        // clean-up functions

        void destroyDeadEnemies() {

            while (1) {

                if (enemies.empty()) return;

                if (enemies.front().getState() != IsDead) return;
                
                enemies.pop_front();

            }

            return;

        }

        void destroyDeadProjectiles() {

            while (1) {

                if (projectiles.empty()) return;

                if (projectiles.front().isAlive()) return;
                
                projectiles.pop_front();

            }

            return;

        }

        // collision functions

        void doSpaceshipCollisions() { }

        bool projectileHitSpaceship(Projectile p, Spaceship *s) {

            if (s == NULL) return false;

            bool xCollision, yCollision;

            float pS = p.getSize();
            float sS = s -> getSize();

            Direction pD = p.getDirection();
            Direction sD = s -> getDirection();
        
            Location pL = p.getLocation();
            Location sL = s -> getLocation();

            if (pD == sD) return false;
            

            xCollision = pL.x - pS < sL.x + sS && pL.x + pS > sL.x - sS;
            yCollision = pL.y + pS > sL.y - sS && pL.y - pS < sL.y + sS;

            return xCollision && yCollision;

        }
 
        void doProjectileCollisions() {

            for (int i = 0 ; i < projectiles.size(); i += 1) {

                Projectile p = projectiles.at(i);

                if (p.isDead()) continue;

                for (int j = 0 ; j < enemies.size(); j += 1) {
                    
                    EnemySpaceship e = enemies.at(j);

                    if (e.getState() == IsDead) continue;

                    if (projectileHitSpaceship(p, &e)) {

                        projectiles.at(i).setDead();
                        enemies.at(j).setState(IsDead);

                        explosionLocations.push_back(e.getLocation());

                        score += spaceshipValue;

                        break;

                    }

                }

                if (p.isDead()) continue;

                if (projectileHitSpaceship(p, player)) {

                    projectiles.at(i).setDead();
                    player -> setState(WasDamaged);

                    score -= spaceshipValue * 2;

                }

            }

        }

        void checkForCollisions() {

            doProjectileCollisions();
            doSpaceshipCollisions();

        }

        // projectile functions

        void addProjectile(Projectile p) { projectiles.push_back(p); }

        void moveProjectiles() {
    
            for (int n = 0; n < projectiles.size(); n += 1) {
            
                projectiles.at(n).move();

                Location l = projectiles.at(n).getLocation();

                if (outOfBounds(l)) 
                    projectiles.at(n).setDead();
            
            }

            return;

        }

        // enemy functions

        void addEnemy(EnemySpaceship e) { enemies.push_back(e); }

        void createEnemies() {

            // there are 5 preset enemy starting positions 
            // the Y position is the same for all but not X

            float x[] = { -15.0, -7.5, 0.0, 7.5, 15.0 };

            // create a placeholder enemy

            EnemySpaceship e(1.0, 0.005, Down, { 0.0, 0.0, 0.0 });

            // randomly select an enemy formation

            switch (rand() % 4) {

                case 0: // turrets

                    for (int n = 0; n < 5; n += 1) {

                        e = EnemySpaceship(1.0, 0.0, Down, { x[n], yLimit, 0.0 });
                        addEnemy(e);

                    }

                    break;

                case 1: // kamikaze

                    e = EnemySpaceship(1.0, 0.005, Down, { x[0], yLimit, 0.0 });
                    addEnemy(e);

                    e = EnemySpaceship(1.0, 0.005, Down, { x[2], yLimit, 0.0 });
                    addEnemy(e);

                    e = EnemySpaceship(1.0, 0.005, Down, { x[4], yLimit, 0.0 });
                    addEnemy(e);

                    break;

                case 2: // left strafers

                    e = EnemySpaceship(1.0, 0.005, Left, { x[3], yLimit, 0.0 });
                    addEnemy(e);

                    e = EnemySpaceship(1.0, 0.005, Left, { x[4], yLimit, 0.0 });
                    addEnemy(e);
                    
                    break;

                case 3: // right strafers

                    e = EnemySpaceship(1.0, 0.005, Right, { x[0], yLimit, 0.0 });
                    addEnemy(e);

                    e = EnemySpaceship(1.0, 0.005, Right, { x[1], yLimit, 0.0 });
                    addEnemy(e);

                    break;

            }
            
            return;

        }

        void moveEnemies() {

            for (int n = 0; n < enemies.size(); n += 1) {

                Direction d = enemies.at(n).getDirection();
                
                enemies.at(n).move(d);

                Location l = enemies.at(n).getLocation();

                if (outOfBounds(l)) 
                    enemies.at(n).setState(IsDead);

            }

            return;
            
        }

        void fireEnemyWeapons() {

            int count = enemies.size();

            for (int n = 0; n < count; n += 1)
                addProjectile(enemies.at(n).shoot());       

        }

        // player functions 

        void movePlayer() {

            bool b[] = { w, a, s, d };
            Direction d[] = { Up, Left, Down, Right };

            for (int n = 0; n < 4; n += 1) 
                if (b[n]) (*player).move(d[n]);
                
            fixPlayerLocation();
       
        }

        void firePlayerWeapon() { addProjectile(player -> shoot()); }

    public:

        // constructor

        Game() {

            // initialize pseudo random number generator
            // it will be used to randomly choose which enemy formation to use

            srand(time(NULL)); 

            // initalize scoring values

            score = 0;
            spaceshipValue = 100;

            // define the boundaries of the playable area
            // this is based on the window size and what the camera can show within the window

            xLimit = yLimit = 19.0;

            // initialize timing values

            timeLastEnemyFire = timeLastEnemySpawn = 0.0;

            // create the player spaceship

            player = new PlayerSpaceship(1.0, 0.005, Up, { 0.0, -19.0, 0.0 });

        }

        // destructor

        ~Game() { 
            
            delete player; 

            enemies.clear();
            projectiles.clear();
            
        }

        // getters 

        PlayerSpaceship getPlayer() { return *player; }

        deque <EnemySpaceship> getEnemies() { return enemies; }

        deque <Projectile> getProjectiles() { return projectiles; }

        deque <Location> getExplosionLocations() {return explosionLocations; }

        // functions

        int getScore() { return score; }

        void keyDown(char c) {

            switch (c) {

                case 'w':

                    w = true;
                    break;

                case 'a':

                    a = true;
                    break;

                case 's':

                    s = true;
                    break;

                case 'd':

                    d = true;
                    break;

                case ' ':

                    firePlayerWeapon();
                    break;

            }

            return;

        }

        void keyUp(char c) {

            switch (c) {

                case 'w':

                    w = false;
                    break;

                case 'a':

                    a = false;
                    break;

                case 's':

                    s = false;
                    break;

                case 'd':
                
                    d = false;
                    break;

            }

            return;

        }

        void update(float time) {

            // clear the explosions list ASAP to avoid rendering the same explosions over and over

            if (!explosionLocations.empty()) explosionLocations.clear();

            // enemies should fire every 1.25 seconds

            if (time - timeLastEnemyFire >= 1.25) {

                printf("Open fire!\n");
                timeLastEnemyFire = time;
                fireEnemyWeapons();

            }

            // move everything

            movePlayer();
            moveEnemies();
            moveProjectiles();

            // check for collisions

            checkForCollisions();

            // clean up

            destroyDeadEnemies();
            destroyDeadProjectiles();

            // enemies should spawn every 5 seconds
            // but only when there are no enemies remaining

            if (time - timeLastEnemySpawn >= 5.0) {

                if (enemies.empty()) {

                    printf("Incoming!\n");
                    createEnemies();

                }

                timeLastEnemySpawn = time;

            }

            return;

        }

};

#endif