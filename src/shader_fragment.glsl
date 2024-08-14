#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Fontes de luz
uniform float ligthsource;

// Identificador que define qual objeto está sendo desenhado no momento
#define TXT_FLOOR 1
#define TXT_WALL 2
#define TXT_CEIL 3
#define HUD_HEALTH 4
#define HUD_MANA 5
#define HUD_MAPTILE 6
#define SPHERE 7
#define LEFT_ARM 8
#define RIGHT_ARM 9
#define ENEMY_TYPE_1 12
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

float atan2(float p1, float p2)
{
    float theta;
    if(p1 > 0)
    {
            theta = atan(p2/p1);
    }
    else if(p1 < 0)
    {
        if(p2 >= 0)
        {
            theta = atan(p2/p1) + M_PI;
        }
        else
        {
            theta = atan(p2/p1) - M_PI;
        }
    }
    else if(p1 == 0)
    {
        if(p2 > 0)
        {
            theta = M_PI/2;
        }
        else
        {
        theta = -M_PI/2;
        }
    }
    return theta;
}

void main()
{
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(0.1,0.4,0.8,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    vec3 Kd;

    U = texcoords.x;
    V = texcoords.y;

    if ( object_id == SPHERE )
    {
        U = texcoords.x;
        V = texcoords.y;
        Kd = texture(TextureImage3, vec2(U,V)).rgb;
    }
    if (object_id == TXT_FLOOR)
    {
        Kd = texture(TextureImage1, vec2(U,V)).rgb;
    }
    else if (object_id == TXT_WALL)
    {
        Kd = texture(TextureImage0, vec2(U,V)).rgb;
    }
    else if (object_id == TXT_CEIL)
    {
        Kd = texture(TextureImage1, vec2(U,V)).rgb;
    }
    else if (object_id == HUD_HEALTH)
    {
        Kd = vec3(1.0f,0.0f,0.0f);
    }
    else if (object_id == HUD_MANA)
    {
        Kd = vec3(0.0f,0.0f,1.0f);
    }
    else if (object_id == HUD_MAPTILE)
    {
        Kd = vec3(1.0f,1.0f,1.0f);
    }
    else if (object_id == RIGHT_ARM)
    {
        U = texcoords.x;
        V = texcoords.y;
        Kd = texture(TextureImage2, vec2(U,V)).rgb;
    }
    else if (object_id == LEFT_ARM)
    {
        U = texcoords.x;
        V = texcoords.y;
        Kd = texture(TextureImage2, vec2(U,V)).rgb;
    }
    else if(object_id == ENEMY_TYPE_1){
        Kd = vec3(1.0f,0.0f,0.0f);
    }

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));
    float lambert_inv = max(0,-dot(n,l));
    color.rgb = Kd * (lambert + 0.01);

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
}

