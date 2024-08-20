# Trabalho Final da Disciplina de Fundamentos de Computação Gráfica da UFRGS

A ideia do projeto era desenvolver um jogo dungeon crawler em primeira pessoa, inspirado em clássicos como DOOM e Wolfenstein 3D, com elementos de fantasia inspirados em Skyrim. O código fonte da aplicação foi escrita completamente em C++, utilizando a [OpenGL API](https://www.khronos.org/opengl/wiki/Getting_Started), a biblioteca de cálculos matemáticos [GLM](https://github.com/g-truc/glm) e a biblioteca de controle de janelas [GLFW](https://github.com/glfw/glfw).

## Contribuições dos Membros

A aplicação foi desenvolvida pela dupla Sandro Viégas ([srsviegas](https://github.com/srsviegas)) e Walter Frank ([WalterINF](https://github.com/WalterINF)). Não foi feita uma divisão clara das funções entre os integrantes, mas, em termos gerais, as contribuições foram as seguintes.

### Sandro Viégas

- Implementação das câmeras (first person e look-at)
- Implementação do sistema de construção do mapa
- Implementação das colisões Esfera/Plano e Esfera/AABB
- Implementação dos cálculos de iluminação no Shader Fragment e Shader Vertex
- Implementação de billboarding para portal e HP dos inimigos

### Walter Frank

- Implementação do controlador de jogador e seus modelos
- Implementação do controlador de projéteis (sistema de spellcasting)
- Implementação do controlador de entidades e seus modelos
- Implementação da colisão AABB/AABB
- Implementação do controlador de poções e sua curva de Bezier

## Uso de Assistentes de Inteligência Artificial

Uma ferramenta muito útil durante o desenvolvimento do projeto foi o [Gemini Advanced](https://gemini.google.com/app), da Google. O assistente demonstrou capacidade de explicar conceitos de computação gráfica em um nível técnico elevado, transmitindo boa compreensão da documentação da API OpenGL, da biblioteca GLM e da linguagem GLSL. No entando, sua utilidade reside mais nas suas capacidades didáticas do que na geração de código funcional, já que o modelo apresenta limitações em relação à compreensão de lógica necessária para o desenvolvimento de aplicações complexas.

## Desenvolvimento e Atendimento de Requisitos

O desenvolvimento foi iniciado em cima do código do Laboratório 5 da disciplina, que possuia a implementação de elementos fundamentais da aplicação. Algumas das funcionalidades já implementadas eram as transformações geométricas, projeções, sistema de coordenadas da câmera, carregamento dos shaders e criação de janelas com pelo GLFW.

Múltiplos objetos virtuais com malhas poligonais complexas tiveram seus modelos no formato OBJ carregados utilizando a biblioteca [tinyobjloader](https://github.com/syoyo/tinyobjloader). Alguns destes tiveram suas transformações geométricas definidas através do controle do teclado e mouse, como os braços do protagonista e as esferas de magia.

A câmera em primeira pessoa foi uma adaptação da *Free Camera* desenvolvida no Laboratório 2, com sua posição Y fixa. Para a visualização do mapa (acessada pelo atalho **M**), foi implementada uma *Look-at Camera*, com posição fixada no centro do mapa.

O modelo do mapa é construido através da cópia do objeto virtual de plano em múltiplas instâncias, alterando apenas sua *Model matrix*. Semelhantemente, os inimigos, as tochas e os projéteis são cópias de objetos virtuais com transformações geométricas diferentes.

No mínimo três tipos de testes de intersecção precisavam ser realizados na aplicação. Então, foram implementadas a intersecção Esfera-Plano, na colisão entre jogador e mapa, Esfera-AABB, na colisão entre jogador e inimigos, e AABB-AABB, na colisão entre projéteis e inimigos. Todas as colisões estão disponíveis no arquivo [`/src/core/Collisions.cpp`](https://github.com/srsviegas/ufrgs-fcg-final/blob/main/src/core/Collisions.cpp) e no header [`/include/Collisions.h`](https://github.com/srsviegas/ufrgs-fcg-final/blob/main/include/Collisions.h).

Todos os objetos virtuais possuem iluminação difusa e Blinn-Phong, com suas texturas e coeficientes de material definidos caso a caso no Shader Fragment ([`shader_fragment.glsl`](https://github.com/srsviegas/ufrgs-fcg-final/blob/main/src/shader_fragment.glsl)). A interpolação no modelo de Phong foi utilizada em quase todos os objetos, com exceção dos modelos dos braços do jogador, que utilizam interpolação no modelo de Gouraud ([`shader_vertex.glsl`](https://github.com/srsviegas/ufrgs-fcg-final/blob/main/src/shader_vertex.glsl)). O modelo de Gouraud foi testado no modelo do mapa, com o objetivo de otimizar a performance dos cálculos de iluminação, mas os resultados não foram satisfatórios, então a possibilidade foi descartada.

Uma curva de Bezier foi implementada na animação de coleta das poções derrubadas pelos inimigos, que lança o modelo na direção do jogador de forma suave.

Algumas animações foram desenvolvidas, incluindo a movimentação do jogador e inimigos, o impulso dos projéteis e o ataque da espada. Adicionalmente, a tela de *Game Over* possui uma animação de *fade-in*, através da variação da opacidade de sua textura. Todas as animações são computadas baseadas no tempo (variável `timeDelta` na função principal), garantindo que elas ocorram sempre na mesma velocidade, independentemente da velocidade da CPU onde o programa está sendo executado.

## Screenshots
<img src="https://github.com/user-attachments/assets/b3a5b284-c013-4749-85c4-14bccc4878d0" width="500" />
<img src="https://github.com/user-attachments/assets/4a0db392-14d8-40c9-bc0a-90ae5f22df1a" width="500" />
<img src="https://github.com/user-attachments/assets/537d8909-c48d-4476-9acd-d52a9273fcaa" width="500" />
<img src="https://github.com/user-attachments/assets/d0860355-b5c6-4b14-a1de-d18907d99112" width="500" />


## Manual de Uso da Aplicação

A aplicação é controlada através do teclado e mouse, com os comandos existentes listados abaixo.

### Movimentação

- **W:** Movimentação para a frente.
- **A:** Movimentação para a esquerda.
- **S:** Movimentação para trás.
- **D:** Movimentação para a direita.

### Combate

- **Botão esquerdo do mouse:** Ataque com a espada.
- **Botão direito do mouse:** Ataque mágico.

### Menus

- **M:** Pausa, visualização do mapa

### Atalhos para Debugging

- **F5:** Recarrega os shaders.

## Compilação e Execução

### Compilação pelo VScode

1) Instale o VSCode seguindo as instruções em https://code.visualstudio.com/ .

2) Instale o compilador GCC no Windows seguindo as instruções em
https://code.visualstudio.com/docs/cpp/config-mingw#_installing-the-mingww64-toolchain .

3) Instale o CMake seguindo as instruções em https://cmake.org/download/ .
Alternativamente, você pode utilizar algum package manager do
Windows para fazer esta instalação, como https://chocolatey.org/ .

4) Instale as extensões "ms-vscode.cpptools" e "ms-vscode.cmake-tools"
no VSCode. Se você abrir o diretório deste projeto no VSCode,
automaticamente será sugerida a instalação destas extensões (pois
estão listadas no arquivo ".vscode/extensions.json").

5) Abra as configurações da extensão cmake-tools (Ctrl-Shift-P e
busque por "CMake: Open CMake Tools Extension Settings"), e adicione o
caminho de instalação do GCC na opção de configuração "additionalCompilerSearchDirs".

6) Clique no botão de "Play" na barra inferior do VSCode para compilar
e executar o projeto. Na primeira compilação, a extensão do CMake para
o VSCode irá perguntar qual compilador você quer utilizar. Selecione
da lista o compilador GCC que você instalou com o MSYS/MinGW.

### Execução

Alternativamente, uma versão já compilada da aplicação está disponível na pasta `/bin/Release/main.exe`. Para executá-la, basta clonar o repositório em sua máquina local e abrir o executável.

## Lista de Requisitos

A especificação do projeto continha uma lista de requisitos sobre a implementação. Abaixo está a lista de tarefas utilizada pela dupla durante o desenvolvimento.

- Objetos virtuais representados através de malhas poligonais complexas
    - [x] No mínimo 1 modelo de complexidade maior que "cow.obj"
    - [x] Transformações geométricas de objetos virtuais através do teclado/mouse
    - [x] No mínimo um objeto copiado com duas ou mais instâncias

- Controle de câmeras virtuais
    - [x] Câmera look-at
    - [x] Câmera livre

- No mínimo 3 testes de intersecção entre objetos virtuais
    - [x] Intersecção Esfera / Plano
    - [x] Intersecção Esfera / AABB
    - [x] Intersecção AABB / AABB

- Modelos de iluminação de objetos geométricos
    - [x] Objeto com iluminação difusa (lambert)
    - [x] Objeto com iluminação Blinn-Phong
    - [x] Objeto com interpolação no modelo de Gourad
    - [x] Objeto com interpolação no modelo de Phong

- Mapeamento de texturas
    - [x] Todos os objetos devem ter suas cores definidas através de texturas

- Curvas de Bézier
    - [x] No mínimo um objeto com movimentação por curva de Bézier.

- Animação do movimento baseada no tempo
    - [x] Todas as movimentações devem ser computadas no tempo.
