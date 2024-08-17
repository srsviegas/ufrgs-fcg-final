/* Projectiles */
#define MAX_PROJECTILES 10 // Maximum number of player projectiles at once

/* Powerups */
#define MAX_POWERUPS 10            // Maximum number of powerups at once
#define POWERUP_COLLECT_SPEED 0.7f // Speed of powerup item movement

/* Player default values */
#define MAX_HEALTH 100              // Player health
#define MAX_MANA 100                // Player mana
#define MANA_REGENFACTOR 5          // Player mana regeneration factor
#define HEALTH_REGENFACTOR 0.1      // Player health regeneration factor
#define DEFAULT_SPEED 2.5           // Player default movement speed
#define DEFAULT_COLLISIONRADIUS 0.5 // Player model default collision radius

/* Object ID's */
#define TXT_FLOOR 1         // Floor plane
#define TXT_WALL 2          // Wall plane
#define TXT_CEIL 3          // Ceiling plane
#define HUD_HEALTH 4        // Health HUD frame
#define HUD_HEALTH_BAR 5    // Health HUD bar
#define HUD_MANA 6          // Mana HUD frame
#define HUD_MANA_BAR 7      // Mana HUD bar
#define PROJECTILE_WATER 8  // Water projectile
#define LEFT_ARM 9          // Player's left arm
#define RIGHT_ARM 10        // Player's right arm
#define TORCH 11            // Map torches
#define OBJECTIVE_PORTAL 12 // Objective portal
#define ENEMY_TYPE_1 13     // Enemy 1
#define POTION_HEALTH 14    // Health potion

#define OBJECTIVE_RADIUS 0.5 // Radius of objective collision

#define TORCH_CHANCE_PER_WALL 25 // Torch chance per wall (0-100%)

/* Game Entity states */
#define STATE_IDLE 0
#define STATE_ROAMING 1
#define STATE_CHASING 2

/* Entity Controller values */
#define ENEMY_CHANCE_PER_TILE 30 // Enemy spawn chance per tile (0-100%)
#define MAX_ENTITIES 30          // Maximum number of enemy entities