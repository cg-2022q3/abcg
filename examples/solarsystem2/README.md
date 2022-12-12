# Sistema Solar

Rafael Correia de Lima - 21004515 

Renan Gonçalves Miranda - 11069212

## Funcionalidade

Localizado na Via Láctea, o Sistema Solar é formado por 8 planetas (Mercúrio, Vênus, Terra, Marte, Júpiter, Saturno, Urano, Netuno) que orbitam uma estrela, o Sol, daí seu nome. Além desses 8 planetas também temos diversos corpos celestes, como satélites naturais, asteroides, cometas e meteoroides.

Na figura abaixo temos a representação do sistema solar. Nessa firura estão numeradas as principais funcionalidades que o usuário poderá manipular.

![sistemaSolar](https://user-images.githubusercontent.com/47281465/206933302-8e1e4739-8d20-4150-ab3f-cae58b131a66.png)

**1. Controles de Movimento:** Através dos sliders o usuário conseguirá aumentar ou diminuir as velocidades de rotação e translação dos planetas (mudação de velocidade do sistema, ou seja, ao alterar o mutiplicar da velocidade, será alterado para todos os planetas). Como padrão, essas velocidades estão setadas para iniciar zeradas.

**2. Controles de iluminação:** Através dos sliders o usuário conseguirá aumentar ou diminuir os fatores das componentes de reflexão, que são ambiente (Ia), difusa (Id) e especular (Is). Além disso pode também aumentar ou diminuir o expoente de brilho especular (Shineness).

**3. O Sistema Solar:** Nele o usuário pode selecionar o planeta ou o sol, após a seleção será apresentado ao usuário informações do astro selecionado e o planeta entrará em foco pela câmera, caso a translação esteja ligada, a câmera acompanhará o planeta selecionado por todo o seu trajeto (presente no item 4).

**4. Informações:** Após o usuário selecionar o astro de sua escolha, será apresentada uma caixa com informações relevantes do astro, como diâmetro, velocidade de translação, temperatura, composição, entre outros.

Abaixo temos um exemplo de foco no planeta Júpeter: 
![Captura de tela de 2022-12-11 21-58-09](https://user-images.githubusercontent.com/47281465/206940352-e31109c9-d9da-4d88-adc9-958b9bcce505.png)

Nesta representação o usuário poderia aproximar-se ou afastar-se dos astros utilizando o scroll do mouse, ao clicar com o botão esquerdo na tela, o usuário pode ser movimentar para cima, para baixo, para a esquerda ou para a direita através das teclas de movimentação do teclado ou as teclas w,s,a,d.


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
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "body.hpp"
#include "camera.hpp"
#include "skydome.hpp"

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

  Skydome skydome;

  float m_pedestalSpeed{};
  float m_truckSpeed{};
  
  float m_rotation_speed{0.0f};
  float m_translation_speed{0.0f};

  struct Light
  {
    glm::vec4 m_Ia{1.0f};
    glm::vec4 m_Id{1.0f};
    glm::vec4 m_Is{1.0f};
  };

  Light light;

  float m_Ia{1.0f};
  float m_Id{1.0f};
  float m_Is{1.0f};
  float m_shininess{50.0f};

  int radio_selected{-1};
  
  ImFont *m_font{};

  GLuint program_body{}, program_path{}, program_skydome{};
};

#endif


```


##### `Window::onCreate()` 
Temos a criação dos astros do sistema solar. São passados para cada astro os seguintes parametros:
* nome;
* escala;
* cor;
* velocidade de translação;
* velocidade de rotação;
* raio orbital;
* referencia de translação;
* textura.

Conforme exemplo abaixo:


```C++
  // create earth
  earth.name = "Earth";
  earth.scale = 0.1f;
  earth.texture_path = assetsPath + "maps/8k_earth_daymap.jpg";
  earth.night_map_path = assetsPath + "maps/8k_earth_nightmap.jpg";
  earth.specular_map_path = assetsPath + "maps/8k_earth_specular_map.tif";
  earth.cloud_map_path = assetsPath + "maps/8k_earth_clouds.jpg";
  earth.translation_speed = 1.0f/365.0f;
  earth.rotation_speed = 1.0f;
  earth.orbit_radius = 2.5f;
  earth.satellite_of = &sun;
  earth.create(program_body);
```

Observação: Para o planeta Terra foi aplicada três texturas: a primeira textura é a representação do planeta terra, a segunda com nuvens e a terceira é aplicada somente na parte que está escura da Terra, mostrando as luzes elétricas.

Aqui também é criado a doma de estrelas:

```C++
  skydome.texture_path = assetsPath + "maps/8k_stars_milky_way.jpg";
  skydome.create(program_skydome);
```

##### `Window::onPaintUI()`
* São criados sliders (conforme apresentados em sala de aula) para dar a possibilidade ao usuário de aumentar ou diminuir as velocidades de translação dos planetas e o controle de luminosidade dos sistema.


```C++
    ImGui::SetNextWindowBgAlpha(0.7f);
    ImGui::Begin("Controles de Movimento", nullptr,ImGuiWindowFlags_NoResize);

    {
      ImGui::PushItemWidth(-1);
      ImGui::PushID(1);
      ImGui::SliderFloat("", &m_rotation_speed, 0.0f, 10.0f,"%.1fx Vel. Rotação");
      ImGui::PopID();
      
      ImGui::PushID(2);
      ImGui::SliderFloat("", &m_translation_speed, 0.0f, 10.0f,"%.1fx Vel. Translação");
      ImGui::PopID();
      ImGui::PopItemWidth();
      
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(5, 200));
    ImGui::SetNextWindowSize(ImVec2(300, -1));
    ImGui::SetNextWindowBgAlpha(0.7f);
  
    ImGui::Begin("Controles de Iluminação", nullptr,ImGuiWindowFlags_NoResize);

    {
      ImGui::PushItemWidth(-1);
      ImGui::PushID(3);
      ImGui::SliderFloat("", &m_Ia, 0.0f, 2.0f,"Ia: %.1f");
      light.m_Ia = glm::vec4(m_Ia);
      ImGui::PopID();

      ImGui::PushID(4);
      ImGui::SliderFloat("", &m_Id, 0.0f, 2.0f,"Id: %.1f");
      light.m_Id = glm::vec4(m_Id);
      ImGui::PopID();

      ImGui::PushID(5);
      ImGui::SliderFloat("", &m_Is, 0.0f, 2.0f,"Is: %.1f");
      light.m_Is = glm::vec4(m_Is);
      ImGui::PopID();
      
      ImGui::PushID(6);
      ImGui::SliderFloat("", &m_shininess, 0.0f, 100.0f,"Shininess: %.1f");
      ImGui::PopID();

    }

    ImGui::End();
  }
```

* É criado um widget (radio button) contendo os planetas para seleção dos astros. Essa criação se tá através do `ImGui::RadioButton`, onde passamos o que estará escrito no radio button, a variaável que recebera o resultado e seu "identificador":

```C++
  ImGui::SetNextWindowPos(ImVec2(5, 400));
  ImGui::SetNextWindowSize(ImVec2(200, -1));
  ImGui::SetNextWindowBgAlpha(0.7f);

  ImGui::Begin("O Sistema Solar", nullptr,ImGuiWindowFlags_NoResize);
    
  {
    ImGui::RadioButton("Câmera Livre", &radio_selected, -1);
    ImGui::RadioButton("Sol", &radio_selected, 0);
    ImGui::RadioButton("Mercúrio", &radio_selected, 1);
    ImGui::RadioButton("Vênus", &radio_selected, 2);
    ImGui::RadioButton("Terra", &radio_selected, 3);
    ImGui::RadioButton("Marte", &radio_selected, 4);
    ImGui::RadioButton("Júpiter", &radio_selected, 5); 
    ImGui::RadioButton("Saturno", &radio_selected, 6);
    ImGui::RadioButton("Urano", &radio_selected, 7);
    ImGui::RadioButton("Neturno", &radio_selected, 8);
  }

  ImGui::End();
```
* Através do valor que é setado no radio button é mostrado a informação do astro selecionado. Aqui utilizamos `ImGui::Text`para passar na tela tais informações:


```C++
mGui::SetNextWindowPos(ImVec2(m_viewportSize.x -505, 5));
  ImGui::SetNextWindowSize(ImVec2(500, -1));
    ImGui::SetNextWindowBgAlpha(0.7f);

  ImGui::Begin("Informações", nullptr,ImGuiWindowFlags_NoResize);
  switch (radio_selected) 
  {
    case 0:{
      ImGui::Text("Sol");
      ImGui::Text("********************************************");
      ImGui::Text("Diâmetro:           1.391.016 km");
      ImGui::Text("Temperature:        5.500°C");
      ImGui::Text("Composição:");
      ImGui::Text("   * Gases hélio e hidrogênio");
      break;
    }
    case 1:{
      ImGui::Text("Mercúrio");
      ImGui::Text("********************************************");
      ImGui::Text("Diâmetro:           4.879,4 Km");
      ImGui::Text("Dist. Sol:          57,9 milhões Km");
      ImGui::Text("Vel. Translação:    47,87 km/s");
      ImGui::Text("1 Dia:              58,6 dias terrestre");
      ImGui::Text("1 Ano:              88 dias terrestre");
      ImGui::Text("Temp. Dia:          430°C");
      ImGui::Text("Temp. Noite:        -170°C");
      ImGui::Text("Composição:");
      ImGui::Text("   * Atomos de argônio, neônio e hélio");
      break;
    }
    
    .
    .
    .
    
    case -1:{
      ImGui::Text("Olá, seja bem vindo ao Modelo Sistema Solar!");
      ImGui::Text("Selecione um planeta e veja suas informações");
      ImGui::Text("Divirta-se navegando e explorando o no sistema.");
      break;
    }
```

* Para a focalização da câmera no astro selecionado no radio button, focamos a câmera a uma certa distância do astro selecionado, com isso a câmera seguirà ele em sua trajettória (caso a velocidade de translação seja diferente de zero, caso contrario a câmera ficará parada focando o astro):


```C++
witch (radio_selected) 
  {
    case 0:{
      m_camera.trackBody(sun);
      break;
    }
    case 1:{
      m_camera.trackBody(mercury);
      break;
    }
    
    .
    .
    .
    case 8:{
      m_camera.trackBody(neptune);
      break;
    }
    case -1:{
      m_camera.isTrackingBody = false;
      break;
    }
  }
```

##### `Window::onPaint()`
Temos a renderização dos planetas, sol e lua e a aplicação da iluminação. 

##### `Window::onEvent` 
São convertidos eventos do mouse ou teclado para ações no sistema, como as teclas direcionais ou as teclas w,s,a,d do teclado exercem uma ação no sistema, ou ao pressionar o botão do mouse acontece uma ação do sistema solar.

##### `Window::onResize` e `Window::onDestroy()` 
Não temos alteração das versões apresentadas no curso.

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
