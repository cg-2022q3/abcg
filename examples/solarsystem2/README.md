# Solar System Improved

Rafael Correia de Lima - 21004515 

Renan Gonçalves Miranda - 11069212

## Descrição


## Implementação

### Main
Semelhante aos main.cpp dos projetos apresentados em sala de aula, somente sofrendo alteração no título da janela, Solar System Simulation.

```C++
    Window window;
    window.setOpenGLSettings({.samples = 4});
    window.setWindowSettings({
        .width = 900,
        .height = 900,
        .title = "Solar System Simulation",
    });
```

### Window
Nessa classe definimos o funcionamento geral da cena e da UI.

```C++
class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  Camera m_camera;

  Body sun, mercury, venus, earth, mars, jupiter, saturn, neptune, uranus, moon;

  float m_pedestalSpeed{};
  float m_truckSpeed{};
  float speed{1.0f};
  
  GLuint m_program{};
};

```

Em `Window::onCreate()` temos a criação dos planetas. São passados para cada planeta, sol e lua os seguintes parametros:
* nome;
* escala;
* cor;
* velocidade de translação;
* velocidade de rotação;
* raio orbital;
* referencia de translação.

Em `Window::onPaintUI()` é criado um slider para dar a possibilidade ao usuário de aumentar as velocidades de translação dos planetas, levando em consideração a velocidade inicial multiplicada pelo valor do slider.

Em `Window::onPaint()` temos a renderização dos planetas, sol e lua. 

Em `Window::onEvent` são convertidos eventos do mouse ou teclado para ações no sistema, como as tecladas direcionais do teclado exercem uma ação no sistema, ou ao pressionar o botão do mouse acontece uma ação do sistema solar.

Em `Window::onResize` e `Window::onDestroy()` não temos alteração das versões apresentadas no curso.

``Body sun, mercury, venus, earth, mars, jupiter, saturn, neptune, uranus, moon;``: representação dos corpos celestes a serem simulados na cena; 

m_camera: câmera móvel que gera a visualização da cena;

### Camera

Representa a câmera que pode ser movida pelo usuário através da cena;

```C++
class Camera {
public:
  void mouseMove(glm::ivec2 const &position);
  void mousePress(glm::ivec2 const &position);
  void mouseRelease(glm::ivec2 const &position);
  void resizeViewport(glm::ivec2 const &size);
  void mouseScroll(float scroll);

  void computeViewMatrix();
  void computeProjectionMatrix(glm::vec2 const &size);

  void pedestal(float speed);
  void truck(float speed);

  glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
  glm::mat4 const &getProjMatrix() const { return m_projMatrix; }

private:
  glm::vec3 m_eye{0.0f, 10.0f, 10.0f}; // Camera position
  glm::vec3 m_at{0.0f, 0.0f, 0.0f};  // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction

  glm::vec3 m_axis{1.0f}; // axis for rotating camera when mouse is dragged
  glm::mat4 m_rotation{1.0f}; // angle for rotating camera when mouse is dragged 


  bool m_mouseTracking{};

  glm::vec3 m_lastPosition{};

  glm::ivec2 m_viewportSize{};

  // Matrix to change from world space to camera space
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;

  [[nodiscard]] glm::vec3 project(glm::vec2 const &mousePosition) const;
};
```
As funções e atributos dessa classe são descritos a seguir:
- ``mouseMove()``: registra a movimentação do cursor do mouse e realiza a rotação do ``m_at`` ao redor de ``m_eye`` caso a captura da movimentação esteja ativada;
- ``mousePress()``: inicia a captura da movimentação do cursos do mouse caso esteja pressionado;
- ``mouseRelease()``: para a captira da movimentação do mouse caso o botão seja solto;
- ``mouseScroll()``: movimenta câmera para frente e para trás de acordo com a rolagem do mouse
- ``resizeViewport()``: atualiza a variável que guarda o tamanho da janela, para calcular o aspect da projeção perspectiva;
- ``computeViewMatrix``: calcula a view matrix de acordo com a posição da câmera;
- ``computeProjectionMatrix``: calcula a matriz de projeção perspectiva, de acordo com o tamanho da janela;
- ``pedestal``: move a câmera verticalmente;
- ``truck``: move a câmera horizontalmente para os lados;
- ``getViewMatrix()``: retorna a view matrix calculada;
- ``getProjMatrix()``: retorna a matriz de projeção calculada
- ``m_eye, m_at e m_up``: pontos e vetor utilizados para definição da camera LookAt.
- m_mouseTracking: variável de controle da captura ou não dos movimento do mouse;
- m_lastPosition: última posição do mouse;
- m_viewportSize: tamanho da janela;
- m_viewMatrix: matris de visualização definida de acordo com a posição da camêra;
- m_projMatrix: matriz de projeção definida de acordo com o tamanho da janelta
- project: projeta a movimentação do mouse em 2D para uma rotação em 3D a ser aplicada sobre a orientação da camêra;

### Body
Essa classe representa um corpo celeste qualquer no sistema solar, com sua posição, tamanho e características de movimentação.

```C++
struct Vertex {
  glm::vec3 position{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Body {
public:
  void create(GLuint program);
  void destroy();
  void update(float deltaTime,float speed);
  void computeModelMatrix();
  void render() const;
  void generateUVSphere(int stacks, int slices);

  std::string name{""};
  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<GLuint> m_lines_indices;

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  
  glm::vec4 color{};
  float scale{1.0f};
  Body *satellite_of;

  Path path;

  glm::vec3 position{0.0f};
  float orbit_radius{};
  float translation_angle{0.0f};
  float translation_speed{0.0f};
  float rotation_angle{0.0f};
  float rotation_speed{1.0f};

  glm::mat4 modelMatrix{1.0f};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};
```
- create(): cria a esfera e os buffers que resgistrarão os atributos a serem passados ao OpenGL;
- destroy(): descarta os buffers utilizados;
- update(): atualiza a posição do corpo de acordo com as velocidades de rotação e translação, e ainda a velocidade de execução da animação;
- computeModelMatrix(): calcula a matriz de modelo de acordo com a translação e rotação calculados;
- render(): desenha o corpo na cena;
- generateUVSphere(): gera um vetor de pontos e índices que formam uma esfera utilizando o sistema de coordenadas UV;
- name: nome do corpo celeste;
- m_vertices: vetor dos vértices da esfera;
- m_indices: índices que formam os triângulos que compõem a esfera;
- m_lines_indices: índices que formam a malha de linhas da esfera;
- m_VAO, m_VBO e m_EBO: são os buffers que representam o corpo para o OpenGL;
- color: cor do corpo celeste;
- scale: escala de tamanho do corpo;
- *satellite_of: ponteiro para o corpo celeste de qual o corpo em questão é um satélite
- path: objeto que representa a órbita que o corpo percorre;
- position: posição do centro do corpo no espaço;
- orbit_radius: raio da órbita do corpo ao redor do corpo do qual é um satélite;
- translation_angle: ângulo de translação atual do corpo em sua órbita;
- translation_speed: velocidade de translação do corpo;
- rotation_angle: ângulo de rotação atual do corpo;
- rotation_speed: velocidade de rotação do corpo;
- modelMatrix: matriz de modelo calculada a partir da translação e rotação do corpo;
- m_modelMatrixLoc e m_colorLoc: localização das variáveis passadas ao OpenGL;

### Path
Representa o percurso da órbita de um corpo. Desenhado na cena na forma de uma linha tracejada.

```C++
class Path {
public:
  void create(GLuint program);
  void destroy();
  void update(glm::vec3 pos);
  void computeModelMatrix();
  void render() const;
  void generateCircle(int num_vertices);

  float orbit_radius{};

private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  std::vector<glm::vec3> m_vertices;
  std::vector<GLuint> m_indices;

  glm::vec3 position{0.0f};

  glm::vec4 color{};

  glm::mat4 modelMatrix{1.0f};

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};
```
- create(): cria a órbita circular e os buffers que resgistrarão os atributos a serem passados ao OpenGL;
- destroy(): descarta os buffers utilizados;
- update(): atualiza a posição da órbita de acordo com a posição do corpo que representa;
- computeModelMatrix(): calcula a matriz de modelo de acordo com a translação e rotação calculados;
- render(): desenha a órbita na cena;
- generateCircle(): gera um vetor de pontos e índices que formam uma esfera utilizando o sistema de coordenadas UV;
- m_vertices: vetor dos vértices da circunferência;
- m_indices: índices as linhas que compõem o círculo;
- m_VAO, m_VBO: são os buffers que representam o trajeto para o OpenGL;
- color: cor da linha;
- position: posição do centro da órbita no espaço;
- orbit_radius: raio da órbita do corpo ao redor do corpo do qual é um satélite;
- modelMatrix: matriz de modelo calculada a partir da posição da órbita;
- m_modelMatrixLoc e m_colorLoc: localização das variáveis passadas ao OpenGL;


## Melhorias para próxima entrega
Além de aplicação dos conteúdos de iluminação e textura que são esperados para a quarta entrega, pretendemos melhorar alguns pontos gerais no projeto, como:
* Anéis ao redor de Saturno;
* Melhorar os controles de movimentação da câmera;
* Adicionar mais corpos, como algumas das maiores luas dos outros planetas, e o cinturão de asteròides;
* Realizar a movimentação dos planetas de acordo com os ângulos reais de inclinação, e não apenas no plano XZ;
* Aprimorar as proporções de tamanho, distância, e velocidade de movimentação entre os planetas, prezando não pela proximidade com a realizade, mas um melhor resultado visual;
* Adicionar mais opções de interação na interface gráfica para controle de aspectos da cena.

## Referências
* https://planetario.ufsc.br/o-sistema-solar/
* https://www.tecmundo.com.br/ciencia/243351-velocidade-planetas-sistema-solar-veja-animacao.htm
* https://www.infoescola.com/astronomia/planetas-do-sistema-solar/
* https://mundoeducacao.uol.com.br/geografia/sistema-solar.htm
