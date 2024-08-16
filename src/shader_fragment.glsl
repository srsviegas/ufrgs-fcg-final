#version 330 core

in vec4 position_world;
in vec4 normal;

// Vertex position
in vec4 position_model;

// OBJ Coordinates
in vec2 texcoords;

// GPU Matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Light sources
#define MAX_TORCHLIGHTS 100
uniform vec4 torchlight_position[MAX_TORCHLIGHTS];
uniform vec3 torchlight_color[MAX_TORCHLIGHTS];
uniform int torchlight_count;

// Object ID
#define TXT_FLOOR 1
#define TXT_WALL 2
#define TXT_CEIL 3
#define HUD_HEALTH 4
#define HUD_MANA 5
#define HUD_MAPTILE 6
#define PROJECTILE_WATER 7
#define LEFT_ARM 8
#define RIGHT_ARM 9
#define TORCH 10
#define ENEMY_TYPE_1 12
uniform int object_id;

// Model's bounding box parameters
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Texture variables
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;

// Fragment's color
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

float atan2(float x, float y)
{
    if (x > 0) return atan(y / x);
    else if (x < 0) return atan(y / x) + ((y >= x) ? M_PI : -M_PI);
    return (y > 0) ? M_PI/2 : -M_PI/2;
}

void main()
{
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec4 normal = normalize(normal);
    vec4 camera_direction = normalize(camera_position - position_world);

    vec3 lambert = vec3(0.0);

    // Torch light illumination
    for (int i = 0; i < torchlight_count; i++)
    {
        vec4 torchlight_direction = normalize(torchlight_position[i] - position_world);
        float distance = length(torchlight_position[i] - position_world);
        float attenuation = 1.0 / (1.0 + 0.05 * distance + 0.025 * distance * distance);
        lambert += max(0, dot(normal, torchlight_direction)) * torchlight_color[i] * attenuation;
    }
    
    float U = 0.0;
    float V = 0.0;

    vec3 Kd;

    U = texcoords.x;
    V = texcoords.y;

    switch (object_id) 
    {
        case PROJECTILE_WATER:
            vec4 bbox_center = (bbox_min + bbox_max) / 2.0;
            float theta;
            float pho;
            float radius = length(position_model - bbox_center);

            theta = atan2(position_model.x, position_model.z);
            pho = asin(position_model.y/radius);

            U = (theta + M_PI) / (2 * M_PI);
            V = (pho + M_PI/2) / (M_PI);
            Kd = texture(TextureImage3, vec2(U,V)).rgb;
            break;
        case TXT_FLOOR:
        case TXT_CEIL: 
            Kd = texture(TextureImage1, vec2(U,V)).rgb;
            break;
        case TXT_WALL: 
            Kd = texture(TextureImage0, vec2(U,V)).rgb;
            break;
        case HUD_HEALTH:
            lambert = vec3(1.0,0.0,0.0);
            Kd = vec3(1.0,1.0,1.0);
            break;
        case HUD_MANA: 
            lambert = vec3(0.0,0.0,1.0);
            Kd = vec3(1.0,1.0,1.0);
            break;
        case RIGHT_ARM:
        case LEFT_ARM:
            U = texcoords.x;
            V = texcoords.y;
            Kd = texture(TextureImage2, vec2(U,V)).rgb;
            break;
        case TORCH:
            Kd = vec3(1.0,1.0,1.0);
        case ENEMY_TYPE_1:
            Kd = vec3(1.0,0.0,0.0);
            break;
    }
    
    color.rgb = Kd * lambert;
    color.a = 1;
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}

