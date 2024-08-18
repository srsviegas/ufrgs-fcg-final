#version 330 core

layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;

out vec4 color_v;

const vec3 Ia = vec3(1.0, 1.0, 1.0);

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
#define ENTITY_FLYER 13     // Enemy 1
#define POTION_HEALTH 14    // Health potion
#define MAP_POINTER 15      // Map pointer
uniform int object_id;

// Texture variables
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;
uniform sampler2D TextureImage6;
uniform sampler2D TextureImage7;
uniform sampler2D TextureImage8;
uniform sampler2D TextureImage9;
uniform sampler2D TextureImage10;
uniform sampler2D TextureImage11;
uniform sampler2D TextureImage12;

void main()
{
    gl_Position = projection * view * model * model_coefficients;
    position_world = model * model_coefficients;
    position_model = model_coefficients;
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;
    texcoords = texture_coefficients;

    // Use Gouraud
    if (object_id == RIGHT_ARM || object_id == LEFT_ARM)
    {
        vec4 camera_position = inverse(view) * vec4(0.0, 0.0, 0.0, 1.0);
        vec3 camera_direction = normalize((camera_position - position_world).xyz);
        vec3 n = normalize(normal.xyz);

        float U = texcoords.x;
        float V = texcoords.y;

        vec3 Kd = vec3(1.0);
        vec3 Ks = vec3(0.0);
        vec3 Ka = vec3(0.0);
        float q = 1.0;

        // Objects that use Gouraud Shading
        if (object_id == RIGHT_ARM || object_id == LEFT_ARM) {
            vec3 tex = texture(TextureImage2, vec2(U, V)).rgb;
            Kd = tex * 0.5;
            Ks = tex * 0.3;
            Ka = tex * 0.5;
            q = 1.0;
        }

        vec3 diffuse = vec3(0.0);
        vec3 ambient = Ka * Ia;
        vec3 specular = vec3(0.0);

        // Torchlight lighting
        for (int i = 0; i < torchlight_count; i++) {
            float distance = length(torchlight_position[i].xyz - position_world.xyz);
            if (distance > 10.0f)
            {
                continue;
            }

            vec3 torchlight_direction = normalize((torchlight_position[i] - position_world).xyz);
            vec3 halfway_direction = normalize(torchlight_direction + camera_direction);

            float attenuation = 1.0 / (0.25 + 0.1 * distance + 0.1 * distance * distance);

            diffuse += Kd * max(dot(n, torchlight_direction), 0.0) * torchlight_color[i] * attenuation;
            specular += Ks * pow(max(dot(n, halfway_direction), 0.0), q) * torchlight_color[i] * attenuation;
        }

        // Water Projectile Lighting
        for (int i = 0; i < waterproj_count; i++) {
            float distance = length(waterproj_position[i].xyz - position_world.xyz);   
            if (distance > 10.0f)
            {
                continue;
            }

            vec3 waterproj_direction = normalize((waterproj_position[i] - position_world).xyz);
            vec3 halfway_direction = normalize(waterproj_direction + camera_direction);

            float attenuation = 1.0 / (0.1 + 0.01 * distance + 1.0 * distance * distance);

            diffuse += Kd * max(dot(n, waterproj_direction), 0.0) * waterproj_color * attenuation;
            specular += Ks * pow(max(dot(n, halfway_direction), 0.0), q) * waterproj_color * attenuation;
        }

        color_v.rgb = pow(diffuse + ambient + specular, vec3(1.0 / 2.2));
        color_v.a = 1.0;
    }
}

