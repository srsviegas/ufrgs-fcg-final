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

#define MAX_PROJECTILES 10
uniform vec4 waterproj_position[MAX_PROJECTILES];
const vec3 waterproj_color = vec3(0.0,0.1,1.0);
uniform int waterproj_count;

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
#define OBJECTIVE_PORTAL 11
#define ENEMY_TYPE_1 12
#define POTION_HEALTH 13
uniform int object_id;

// Model's bounding box parameters
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Texture variables
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;
uniform sampler2D TextureImage6;
uniform sampler2D TextureImage7;

// Fragment's color
out vec4 color;

const vec3 Ia = vec3(1.0, 1.0, 1.0);

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

float atan2(float x, float y)
{
    if (x > 0) return atan(y / x);
    else if (x < 0) return atan(y / x) + ((y >= 0) ? M_PI : -M_PI);
    return (y > 0) ? M_PI/2 : -M_PI/2;
}

void main() {
    vec4 camera_position = inverse(view) * vec4(0.0, 0.0, 0.0, 1.0);
    vec3 n = normalize(normal.xyz);
    vec3 camera_direction = normalize((camera_position - position_world).xyz);
    
    float U = texcoords.x;
    float V = texcoords.y;

    vec3 Kd = vec3(0.0);
    vec3 Ks = vec3(0.0);
    vec3 Ka = vec3(0.0);
    float q = 1.0;

    if (object_id == PROJECTILE_WATER) {
        vec3 bbox_center = (bbox_min.xyz + bbox_max.xyz) * 0.5;
        float radius = length(position_model.xyz - bbox_center);
        float theta = atan2(position_model.x, position_model.z);
        float pho = asin(position_model.y / radius);

        U = (theta + M_PI) / (2.0 * M_PI);
        V = (pho + M_PI_2) / M_PI;

        Ks = vec3(0.05, 0.05, 1.0);
        Ka = texture(TextureImage3, vec2(U, V)).rgb;
    } else if (object_id == TXT_FLOOR || object_id == TXT_CEIL) {
        vec3 tex = texture(TextureImage1, vec2(U, V)).rgb;
        Kd = tex * 0.6;
        Ks = tex * 0.8;
        Ka = tex * 0.01;
        q = 8.0;
    } else if (object_id == TXT_WALL) {
        vec3 tex = texture(TextureImage0, vec2(U, V)).rgb;
        Kd = tex * 0.6;
        Ks = tex * 0.8;
        Ka = tex * 0.01;
        q = 8.0;
    } else if (object_id == HUD_HEALTH) {
        vec3 tex = texture(TextureImage6, vec2(U,V)).rgb;
                Kd = tex;
                Ks = tex;
                Ka = tex;
                q = 0.0;
    } else if (object_id == HUD_MANA) {
        vec3 tex = texture(TextureImage7, vec2(U,V)).rgb;
        Kd = tex;
        Ks = tex;
        Ka = tex;
        q = 0.0;
    } else if (object_id == RIGHT_ARM || object_id == LEFT_ARM) {
        vec3 tex = texture(TextureImage2, vec2(U, V)).rgb;
        Kd = tex * 0.5;
        Ks = tex * 0.3;
        Ka = tex * 0.5;
        q = 1.0;
    } else if (object_id == TORCH) {
        Kd = vec3(1.0);
        Ka = vec3(1.0);
    } else if (object_id == OBJECTIVE_PORTAL) {
        vec3 tex = texture(TextureImage5, vec2(U, V)).rgb;
        Kd = tex;
        Ks = tex;
        Ka = tex * 0.25;
    } else if (object_id == ENEMY_TYPE_1) {
        Kd = vec3(1.0, 0.0, 0.0);
        Ks = vec3(0.4);
    }
    else if (object_id == POTION_HEALTH) {
            vec3 tex = texture(TextureImage4, vec2(U, V)).rgb;
            Kd = tex;
            Ks = tex;
            Ka = tex * 0.3;
    }

    vec3 lambert = vec3(0.0);
    vec3 ambient = Ka * Ia;
    vec3 phong = vec3(0.0);

    // Torchlight lighting
    for (int i = 0; i < torchlight_count; i++) {
        float distance = length(torchlight_position[i].xyz - position_world.xyz);
        if (distance > 10.0f)
        {
            continue;
        }
        vec3 torchlight_direction = normalize((torchlight_position[i] - position_world).xyz);
        vec3 reflection = -torchlight_direction + 2 * n * dot(n, torchlight_direction);
        float attenuation = 1.0 / (0.25 + 0.1 * distance + 0.1 * distance * distance);

        lambert += Kd * max(dot(n, torchlight_direction), 0.0) * torchlight_color[i] * attenuation;
        phong += Ks * pow(max(dot(reflection, camera_direction), 0.0), q) * torchlight_color[i] * attenuation;
    }

    // Water Projectile Lighting
    for (int i = 0; i < waterproj_count; i++) {
        float distance = length(waterproj_position[i].xyz - position_world.xyz);   
        vec3 waterproj_direction = normalize((waterproj_position[i] - position_world).xyz);
        vec3 reflection = -waterproj_direction + 2 * n * dot(n, waterproj_direction);
        float attenuation = 1.0 / (0.1 + 0.01 * distance + 1.0 * distance * distance);

        lambert += Kd * max(dot(n, waterproj_direction), 0.0) * waterproj_color * attenuation;
        phong += Ks * pow(max(dot(reflection, camera_direction), 0.0), q) * waterproj_color * attenuation;
    }

    color.rgb = pow(lambert + ambient + phong, vec3(1.0 / 2.2));

    color.a = 1.0;
}

