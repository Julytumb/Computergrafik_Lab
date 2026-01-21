#include "opengl_renderer.h"
#include <cassert>
#include <span>
#include <utility>
#include <fstream>
#include <vector>

// geometric data as in original game and game coordinates
std::vector<Vector2df> spaceship = {
  Vector2df{-6.0f,  3.0f},
  Vector2df{-6.0f, -3.0f},
  Vector2df{-10.0f, -6.0f},
  Vector2df{14.0f,  0.0f},
  Vector2df{-10.0f,  6.0f},
  Vector2df{ -6.0f,  3.0f}
};

std::vector<Vector2df> flame = { 
  Vector2df{-6, 3},
  Vector2df{-12, 0},
  Vector2df{-6, -3}
};

std::vector<Vector2df> torpedo_points = { 
  Vector2df{0, 0},
  Vector2df{0, 1}
};

std::vector<Vector2df> saucer_points = {
  Vector2df{-16, -6},
  Vector2df{16, -6}, 
  Vector2df{40, 6}, 
  Vector2df{-40, 6},
  Vector2df{-16, 18},
  Vector2df{16, 18},
  Vector2df{40, 6},
  Vector2df{16, -6},
  Vector2df{8, -18},
  Vector2df{-8, -18},
  Vector2df{-16, -6},
  Vector2df{-40, 6}
};

std::vector<Vector2df> asteroid_1 = {
  Vector2df{ 0, -12},
  Vector2df{16, -24},
  Vector2df{32, -12},
  Vector2df{24, 0},
  Vector2df{32, 12},
  Vector2df{8, 24}, 
  Vector2df{-16, 24}, 
  Vector2df{-32, 12}, 
  Vector2df{-32, -12}, 
  Vector2df{-16, -24},
  Vector2df{0, -12}
};

std::vector<Vector2df> asteroid_2 = {
  Vector2df{6, -6},
  Vector2df{32, -12},
  Vector2df{16, -24}, 
  Vector2df{0, -16}, 
  Vector2df{-16, -24}, 
  Vector2df{-24, -12},
  Vector2df{-16, -0}, 
  Vector2df{-32, 12}, 
  Vector2df{-16, 24}, 
  Vector2df{-8, 16}, 
  Vector2df{16, 24}, 
  Vector2df{32, 6}, 
  Vector2df{16, -6},
};

std::vector<Vector2df> asteroid_3 = {
  Vector2df{-16, 0}, 
  Vector2df{-32, 6}, 
  Vector2df{-16, 24}, 
  Vector2df{0, 6}, 
  Vector2df{0, 24}, 
  Vector2df{16, 24},
  Vector2df{32, 6}, 
  Vector2df{32, 6}, 
  Vector2df{16, -24}, 
  Vector2df{-8, -24}, 
  Vector2df{-32, -6}
};

std::vector<Vector2df> asteroid_4 = {
  Vector2df{8,0}, 
  Vector2df{32,-6}, 
  Vector2df{32, -12}, 
  Vector2df{8, -24}, 
  Vector2df{-16, -24}, 
  Vector2df{-8, -12}, 
  Vector2df{-32, -12}, 
  Vector2df{-32, 12}, 
  Vector2df{-16, 24}, 
  Vector2df{8, 16}, 
  Vector2df{16, 24}, 
  Vector2df{32, 12}, 
  Vector2df{8, 0}
};

std::vector<Vector2df> spaceship_debris = {
  Vector2df{-2, -1}, 
  Vector2df{-10, 7}, 
  Vector2df{3, 1}, 
  Vector2df{7, 8},
  Vector2df{0, 3}, 
  Vector2df{6, 1},
  Vector2df{3, -1}, 
  Vector2df{ -5, -7},
  Vector2df{0, -4}, 
  Vector2df{-6, -6},
  Vector2df{-2, 2}, 
  Vector2df{2, 5}
};
    
std::vector<Vector2df> spaceship_debris_direction = {
 Vector2df{-40, -23},
 Vector2df{50, 15},
 Vector2df{0, 45},
 Vector2df{60, -15}, 
 Vector2df{10, -52}, 
 Vector2df{-40, 30}
};

std::vector<Vector2df> debris_points = {
 Vector2df{-32, 32}, 
 Vector2df{-32, -16}, 
 Vector2df{-16, 0}, 
 Vector2df{-16, -32}, 
 Vector2df{-8, 24},
 Vector2df{8, -24}, 
 Vector2df{24, 32}, 
 Vector2df{24, -24}, 
 Vector2df{24, -32}, 
 Vector2df{32, -8}
};
        
std::vector<Vector2df> digit_0 = { {0,-8}, {4,-8}, {4,0}, {0,0}, {0, -8} };
std::vector<Vector2df> digit_1 = { {4,0}, {4,-8} };
std::vector<Vector2df> digit_2 = { {0,-8}, {4,-8}, {4,-4}, {0,-4}, {0,0}, {4,0}  };
std::vector<Vector2df> digit_3 = { {0,0}, {4, 0}, {4,-4}, {0,-4}, {4,-4}, {4, -8}, {0, -8}  };
std::vector<Vector2df> digit_4 = { {4,0}, {4,-8}, {4,-4}, {0,-4}, {0,-8}  };
std::vector<Vector2df> digit_5 = { {0,0}, {4,0}, {4,-4}, {0,-4}, {0,-8}, {4, -8}  };
std::vector<Vector2df> digit_6 = { {0,-8}, {0,0}, {4,0}, {4,-4}, {0,-4} };
std::vector<Vector2df> digit_7 = { {0,-8}, {4,-8}, {4,0} };
std::vector<Vector2df> digit_8 = { {0,-8}, {4,-8}, {4,0}, {0,0}, {0,-8}, {0, -4}, {4, -4} };
std::vector<Vector2df> digit_9 = { {4, 0}, {4,-8}, {0,-8}, {0, -4}, {4, -4} };
       
// std::vector< std::vector<Vector2df> * > vertice_data = {
//   &spaceship, &flame,
//   &torpedo_points, &saucer_points,
//   &asteroid_1, &asteroid_2, &asteroid_3, &asteroid_4,
//   &spaceship_debris, &spaceship_debris_direction,
//   &debris_points,
//   &digit_0, &digit_1, &digit_2, &digit_3, &digit_4, &digit_5, &digit_6, &digit_7, &digit_8, &digit_9 };                                  
std::vector< std::vector<Vector2df> * > digit_data = {
  &digit_0, &digit_1, &digit_2, &digit_3, &digit_4, &digit_5, &digit_6, &digit_7, &digit_8, &digit_9 };

// class OpenGLView

  OpenGLView::OpenGLView(GLuint vbo, unsigned int shaderProgram, size_t vertices_size, GLuint mode)
    : shaderProgram(shaderProgram), vertices_size(vertices_size), mode(mode) {
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    size_t stride = 9 * sizeof(float);

// 1. Position (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // 2. Color (Location 1) - DAS HAT GEFEHLT!
    // Offset ist 6 floats (3 Pos + 3 Normal), da wir im Buffer Layout P P P N N N C C C nutzen (siehe loadModel)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3. Normal (Location 2)
    // Offset ist 3 floats (3 Pos)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO
  }

  OpenGLView::~OpenGLView() {
   glDeleteVertexArrays(1, &vao);
  }

  void OpenGLView::render( SquareMatrix<float,4> & matrice) {
    debug(2, "render() entry...");
    glBindVertexArray(vao);
    glUseProgram(shaderProgram);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &matrice[0][0] );
    glDrawArrays(mode, 0, vertices_size );
    //debug(2, "render() exit.");
    glBindVertexArray(0);
  }

// class TypedBodyView

  TypedBodyView::TypedBodyView(TypedBody * typed_body, GLuint vbo, unsigned int shaderProgram, size_t vertices_size, float scale, GLuint mode,
               std::function<bool()> draw, std::function<void(TypedBodyView *)> modify)
        : OpenGLView(vbo, shaderProgram, vertices_size, mode),  typed_body(typed_body), scale(scale), draw(draw), modify(modify) {
  }
  
  SquareMatrix4df TypedBodyView::create_object_transformation(Vector2df direction, float angle, float scale) {
    SquareMatrix4df  translation= { {1.0f,        0.0f,          0.0f, 0.0f},
                                    {0.0f,        1.0f,          0.0f, 0.0f},
                                    {0.0f,        0.0f,          1.0f, 0.0f},
                                    {direction[0], direction[1], 0.0f, 1.0f}
                                  };
    // Die normale Spiel-Rotation (um Z-Achse)
    SquareMatrix4df rotation = { { std::cos(angle),  std::sin(angle), 0.0f, 0.0f},
                                 {-std::sin(angle),  std::cos(angle), 0.0f, 0.0f},
                                 { 0.0f,             0.0f,            1.0f, 0.0f},
                                 { 0.0f,             0.0f,            0.0f, 1.0f}
                               };
    
   
    // Damit eiern sie im Raum, statt sich nur flach zu drehen.
    SquareMatrix4df tilt = { {1.0f, 0.0f, 0.0f, 0.0f},
                             {0.0f, 1.0f, 0.0f, 0.0f},
                             {0.0f, 0.0f, 1.0f, 0.0f},
                             {0.0f, 0.0f, 0.0f, 1.0f} };

    // Prüfen, ob es ein Asteroid oder Trümmerteil ist
    if (typed_body->get_type() == BodyType::asteroid || typed_body->get_type() == BodyType::debris) {
        float tiltAngle = 0.6f; // ca. 35 Grad kippen
        // Rotation um X-Achse
        tilt = SquareMatrix4df { {1.0f, 0.0f, 0.0f, 0.0f},
                                 {0.0f, std::cos(tiltAngle), std::sin(tiltAngle), 0.0f},
                                 {0.0f, -std::sin(tiltAngle), std::cos(tiltAngle), 0.0f},
                                 {0.0f, 0.0f, 0.0f, 1.0f} };
    }

    SquareMatrix4df  scaling = { { scale,    0.0f, 0.0f,   0.0f},
                                 {  0.0f,   scale, 0.0f,   0.0f},
                                 {  0.0f,    0.0f, scale,  0.0f},
                                 {  0.0f,    0.0f, 0.0f,   1.0f}
                               };                                 
    
    // Reihenfolge: Erst Skalieren -> Dann Kippen (Tilt) -> Dann Drehen (Spiel) -> Dann Bewegen
    return translation * rotation * tilt * scaling;
}

  void TypedBodyView::render( SquareMatrix<float,4> & world) {
    debug(2, "render() entry...");
    if ( draw() ) {
      modify(this);
      auto transform = world * create_object_transformation(typed_body->get_position(), typed_body->get_angle(), scale);
      OpenGLView::render(transform);
    }
    debug(2, "render() exit.");
  }
  
 TypedBody * TypedBodyView::get_typed_body() {
   return typed_body;
 }

 void TypedBodyView::set_scale(float scale) {
   this->scale = scale;
 }

// class OpenGLRenderer

OpenGLRenderer::~OpenGLRenderer() {
    for(auto const& [name, data] : models) {
        glDeleteBuffers(1, &data.vbo);
    }
    glDeleteBuffers(10, digit_vbos);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow( window );
    SDL_Quit();
}

void OpenGLRenderer::loadModel(std::string name, std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        error("Could not open model file: " + filename);
        return; 
    }

    WavefrontImporter importer(file);
    importer.parse();

    std::vector<float> vertex_buffer_data;
    Material default_mat = {{1.0f, 1.0f, 1.0f}}; // Weiß als Fallback

    // Iteriere über alle Faces und erstelle das 9-Float-Layout
    // x y z nx ny nz r g b
    for (const auto& face : importer.get_faces()) {
        Material* mat = face.material ? face.material : &default_mat;
        
        for (const auto& ref : face.reference_groups) {
            // Position
            vertex_buffer_data.push_back(ref.vertice[0]);
            vertex_buffer_data.push_back(ref.vertice[1]);
            vertex_buffer_data.push_back(ref.vertice[2]);
            
            // Normal
            vertex_buffer_data.push_back(ref.normal[0]);
            vertex_buffer_data.push_back(ref.normal[1]);
            vertex_buffer_data.push_back(ref.normal[2]);

            // Color
            vertex_buffer_data.push_back(mat->ambient[0]);
            vertex_buffer_data.push_back(mat->ambient[1]);
            vertex_buffer_data.push_back(mat->ambient[2]);
        }
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float), vertex_buffer_data.data(), GL_STATIC_DRAW);

    // vertex_buffer_data.size() ist float-Anzahl. Ein Vertex hat 9 Floats.
    models[name] = { vbo, vertex_buffer_data.size() / 9 };
    
    debug(1, "Loaded 3D model: " + name + " (" + std::to_string(models[name].vertex_count) + " vertices)");
}

// void OpenGLRenderer::createVbos() {
//  vbos = new GLuint[vertice_data.size()];
//  glGenBuffers(vertice_data.size(), vbos);

//  for (size_t i = 0; i < vertice_data.size(); i++) {
//    glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
//    glBufferData(GL_ARRAY_BUFFER, vertice_data[i]->size() * sizeof( Vector2df ), vertice_data[i]->data(), GL_STATIC_DRAW);
//  }
// }
void OpenGLRenderer::createVbos() {
    // 1. Lade 3D Modelle
    loadModel("spaceship", "spaceship.obj");
    loadModel("torpedo", "torpedo.obj");
    loadModel("asteroid", "asteroid.obj");
    loadModel("saucer", "saucer.obj");
    loadModel("debris", "teapot.obj"); // Wir nutzen den Teapot oder ähnliches als 3D-Explosionsteilchen

    // 2. Erstelle Ziffern-VBOs (Konvertierung 2D -> 3D Layout)
    glGenBuffers(10, digit_vbos);
    for (size_t i = 0; i < 10; i++) {
        std::vector<float> buffer3d;
        // Jede Ziffer hat 9 floats pro Punkt
        buffer3d.reserve(digit_data[i]->size() * 9);
        
        for (const auto& vec2 : *digit_data[i]) {
            // Position (z=0)
            buffer3d.push_back(vec2[0]); 
            buffer3d.push_back(vec2[1]); 
            buffer3d.push_back(0.0f);    
            
            // Normale (zeigen "nach oben/zum Licht", damit sie sichtbar sind)
            // Licht ist bei (0, 1, -4). Wir setzen die Normale so, dass dot > 0.
            // z.B. (0, 1, -1) normalisiert oder einfach (0,0,1)
            buffer3d.push_back(0.0f);
            buffer3d.push_back(0.0f); 
            buffer3d.push_back(1.0f);

            // Farbe (Weiß)
            buffer3d.push_back(1.0f);
            buffer3d.push_back(1.0f); 
            buffer3d.push_back(1.0f);
        }

        glBindBuffer(GL_ARRAY_BUFFER, digit_vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, buffer3d.size() * sizeof(float), buffer3d.data(), GL_STATIC_DRAW);
    }
}

// void OpenGLRenderer::create(Spaceship * ship, std::vector< std::unique_ptr<TypedBodyView> > & views) {
//   debug(4, "create(Spaceship *) entry...");

//   views.push_back(std::make_unique<TypedBodyView>(ship, vbos[0], shaderProgram, vertice_data[0]->size(), 1.0f, GL_LINE_LOOP,
//                   [ship]() -> bool {return ! ship->is_in_hyperspace();}) // only show ship if outside hyperspace
//                  );   
//   views.push_back(std::make_unique<TypedBodyView>(ship, vbos[1], shaderProgram, vertice_data[1]->size(), 1.0f, GL_LINE_LOOP,
//                   [ship]() -> bool {return ! ship->is_in_hyperspace() && ship->is_accelerating();}) // only show flame if accelerating
//                  );   
  
//   debug(4, "create(Spaceship *) exit.");
// }

// void OpenGLRenderer::create(Saucer * saucer, std::vector< std::unique_ptr<TypedBodyView> > & views) {
//   debug(4, "create(Saucer *) entry...");
//   float scale = 0.5;
//   if ( saucer->get_size() == 0 ) {
//     scale = 0.25;
//   }
//   views.push_back(std::make_unique<TypedBodyView>(saucer, vbos[3], shaderProgram, vertice_data[3]->size(), scale));   
//   debug(4, "create(Saucer *) exit.");
// }


// void OpenGLRenderer::create(Torpedo * torpedo, std::vector< std::unique_ptr<TypedBodyView> > & views) {
//   debug(4, "create(Torpedo *) entry...");
//   views.push_back(std::make_unique<TypedBodyView>(torpedo, vbos[2], shaderProgram, vertice_data[2]->size(), 1.0f)); 
//   debug(4, "create(Torpedo *) exit.");
// }

// void OpenGLRenderer::create(Asteroid * asteroid, std::vector< std::unique_ptr<TypedBodyView> > & views) {
//   debug(4, "create(Asteroid *) entry...");
//   GLuint rock_vbo_index = 4 +  asteroid->get_rock_type();

//   float scale = (asteroid->get_size() == 3 ? 1.0 : ( asteroid->get_size() == 2 ? 0.5 : 0.25 ));
 
//   views.push_back(std::make_unique<TypedBodyView>(asteroid, vbos[rock_vbo_index], shaderProgram, vertice_data[rock_vbo_index]->size(), scale)); 
//   debug(4, "create(Asteroid *) exit.");
// }

// void OpenGLRenderer::create(SpaceshipDebris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views) {
//   debug(4, "create(SpaceshipDebris *) entry...");
//   views.push_back(std::make_unique<TypedBodyView>(debris, vbos[10], shaderProgram, vertice_data[10]->size(), 0.1f, GL_POINTS,
//             []() -> bool {return true;},
//             [debris](TypedBodyView * view) -> void { view->set_scale( 0.2f * (SpaceshipDebris::TIME_TO_DELETE - debris->get_time_to_delete()));}));   
//   debug(4, "create(SpaceshipDebris *) exit.");
// }

// void OpenGLRenderer::create(Debris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views) {
//   debug(4, "create(Debris *) entry...");
//   views.push_back(std::make_unique<TypedBodyView>(debris, vbos[10], shaderProgram, vertice_data[10]->size(), 0.1f, GL_POINTS,
//             []() -> bool {return true;},
//             [debris](TypedBodyView * view) -> void { view->set_scale(Debris::TIME_TO_DELETE - debris->get_time_to_delete());}));   
//   debug(4, "create(Debris *) exit.");
// }

// void OpenGLRenderer::createSpaceShipView() {
//   spaceship_view = std::make_unique<OpenGLView>(vbos[0], shaderProgram, vertice_data[0]->size(), GL_LINE_LOOP);
// }

// void OpenGLRenderer::createDigitViews() {
//   for (size_t i = 0; i < 10; i++ ) {
//     digit_views[i] = std::make_unique<OpenGLView>(vbos[11 + i], shaderProgram, vertice_data[11 + i]->size(), GL_LINE_STRIP);
//   }
// }
void OpenGLRenderer::create(Spaceship * ship, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  views.push_back(std::make_unique<TypedBodyView>(
      ship, models["spaceship"].vbo, shaderProgram, models["spaceship"].vertex_count, 
      3.0f, GL_TRIANGLES,
      [ship]() -> bool {return ! ship->is_in_hyperspace();}
  ));   
}

void OpenGLRenderer::create(Saucer * saucer, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  float scale = 1.0f;
  if ( saucer->get_size() == 0 ) scale = 1.0f;

  views.push_back(std::make_unique<TypedBodyView>(
      saucer, models["saucer"].vbo, shaderProgram, models["saucer"].vertex_count, scale, GL_TRIANGLES
  ));   
}

void OpenGLRenderer::create(Torpedo * torpedo, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  views.push_back(std::make_unique<TypedBodyView>(
      torpedo, models["torpedo"].vbo, shaderProgram, models["torpedo"].vertex_count, 5.0f, GL_TRIANGLES
  )); 
}

void OpenGLRenderer::create(Asteroid * asteroid, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  float scale = (asteroid->get_size() == 3 ? 2.5f : ( asteroid->get_size() == 2 ? 2.0f : 1.0f ));
  views.push_back(std::make_unique<TypedBodyView>(
      asteroid, models["asteroid"].vbo, shaderProgram, models["asteroid"].vertex_count, scale, GL_TRIANGLES
  )); 
}

void OpenGLRenderer::create(SpaceshipDebris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  // Explosionen nutzen jetzt das "debris" 3D-Modell (skaliert)
  views.push_back(std::make_unique<TypedBodyView>(
      debris, models["debris"].vbo, shaderProgram, models["debris"].vertex_count, 0.1f, GL_TRIANGLES,
      []() -> bool {return true;},
      [debris](TypedBodyView * view) -> void { 
          // Animation: Skalierung über Zeit
          view->set_scale( 0.2f * (SpaceshipDebris::TIME_TO_DELETE - debris->get_time_to_delete()));
      }
  ));   
}

void OpenGLRenderer::create(Debris * debris, std::vector< std::unique_ptr<TypedBodyView> > & views) {
  views.push_back(std::make_unique<TypedBodyView>(
      debris, models["debris"].vbo, shaderProgram, models["debris"].vertex_count, 0.05f, GL_TRIANGLES,
      []() -> bool {return true;},
      [debris](TypedBodyView * view) -> void { 
          view->set_scale(0.1f * (Debris::TIME_TO_DELETE - debris->get_time_to_delete()));
      }
  ));   
}

void OpenGLRenderer::createSpaceShipView() {
  spaceship_view = std::make_unique<OpenGLView>(
      models["spaceship"].vbo, shaderProgram, models["spaceship"].vertex_count, GL_TRIANGLES
  );
}

void OpenGLRenderer::createDigitViews() {
  for (size_t i = 0; i < 10; i++ ) {
    digit_views[i] = std::make_unique<OpenGLView>(
        digit_vbos[i], shaderProgram, digit_data[i]->size(), GL_LINE_STRIP
    );
  }
}

void OpenGLRenderer::renderFreeShips(SquareMatrix4df & matrice) {
  constexpr float FREE_SHIP_X = 128;
  constexpr float FREE_SHIP_Y = 64;
  const float PIf = static_cast<float> ( PI );

  // 1. Skalierung definieren (z.B. Faktor 15.0f)
  float scale = 3.0f;
  SquareMatrix4df scaling = { { scale, 0.0f, 0.0f, 0.0f},
                              { 0.0f, scale, 0.0f, 0.0f},
                              { 0.0f, 0.0f, scale, 0.0f},
                              { 0.0f, 0.0f, 0.0f, 1.0f} };

  Vector2df position = {FREE_SHIP_X, FREE_SHIP_Y};
  
  // Rotation (damit sie nach oben zeigen)
  SquareMatrix4df rotation = {   { std::cos(-PIf / 2.0f),  std::sin(-PIf / 2.0f), 0.0f, 0.0f},
                                 {-std::sin(-PIf / 2.0f),  std::cos(-PIf / 2.0f), 0.0f, 0.0f},
                                 { 0.0f,                 0.0f,                1.0f, 0.0f},
                                 { 0.0f,                 0.0f,                0.0f, 1.0f}
                               };

  for (int i = 0; i < game.get_no_of_ships(); i++) {
    SquareMatrix4df  translation= { {1.0f,        0.0f,         0.0f, 0.0f},
                                    {0.0f,        1.0f,         0.0f, 0.0f},
                                    {0.0f,        0.0f,         1.0f, 0.0f},
                                    {position[0], position[1],  0.0f, 1.0f} };
    
    // 2. Skalierung in die Berechnung einfügen:
    // Reihenfolge: View -> Translation -> Rotation -> Skalierung
    SquareMatrix4df render_matrice = matrice * translation * rotation * scaling;
    
    spaceship_view->render( render_matrice );
    
    // 3. Abstand vergrößern (z.B. auf 40.0), damit sie sich nicht überlappen
    position[0] += 40.0; 
  }
}

void OpenGLRenderer::renderScore(SquareMatrix4df & matrice) {
  constexpr float SCORE_X = 128 - 48;
  constexpr float SCORE_Y = 48 - 4;
  

  long long score = game.get_score();
  int no_of_digits = 0;
  if (score > 0) {
    no_of_digits = std::trunc( std::log10( score ) ) + 1;
  }

  Vector2df position = {SCORE_X + 20.0f * no_of_digits,  SCORE_Y};  
  do {
    int d = score % 10;
    score /= 10;
    SquareMatrix4df scale_translation= { {4.0f,        0.0f,         0.0f, 0.0f},
                                         {0.0f,        4.0f,         0.0f, 0.0f},
                                         {0.0f,        0.0f,         1.0f, 0.0f},
                                         {position[0], position[1],  0.0f, 1.0f} };
    SquareMatrix4df render_matrice = matrice * scale_translation;
    digit_views[d]->render( render_matrice );
    no_of_digits--;
    position[0] -= 20;

  } while (no_of_digits > 0);
}


// void OpenGLRenderer::create_shader_programs() {

// static const char *vertexShaderSource = "#version 330 core\n"
//     "layout (location = 0) in vec2 p;\n"
//     "uniform mat4 transform;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = transform * vec4(p, 1.0, 1.0);\n"
//     "}\0";
// static const char *fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
//     "}\n\0";

//     // build and compile vertex shader
//     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//     glCompileShader(vertexShader);
//     // check for shader compile errors
//     int success;
//     char infoLog[512];
//     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//         error( std::string("vertex shader compilation failed") + infoLog);
//     }
//     // build and compiler fragment shader
//     unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//     glCompileShader(fragmentShader);
//     // check for shader compile errors
//     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//         error( std::string("fragment shader compilation failed") + infoLog);
//     }
    

//     // link both shaders
//     shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);
//     // check for linking errors
//     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//     if (!success) {
//         glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//         error( std::string("linking shader programs failed") + infoLog);
//     }
// }
void OpenGLRenderer::create_shader_programs() {
    // Shader exakt aus viewer.cc übernommen
    const char *vertexShaderSource = "#version 330 core\n"
      "layout (location = 0) in vec3 position;\n" 
      "layout (location = 1) in vec3 incolor;\n"
      "layout (location = 2) in vec3 innormal;\n"
      "out vec3 color;\n"
      "out vec4 normal;\n"
      "uniform mat4 model;\n"
      "void main()\n"
      "{\n"
      "gl_Position = model * vec4(position, 1.0);\n"
      "color = incolor;\n"
      "normal = normalize( model * vec4(innormal, 0.0));\n"
      "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 outColor;\n"
  "in vec3 color;\n"
  "in vec4 normal;\n"
  "void main () {\n"
  // 1. Lichtrichtung (Sonne von rechts-oben)
  "  vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));\n"
  "  vec3 norm = normalize(normal.xyz);\n"
  
  // 2. Diffuse: Basis-Helligkeit (Matt)
  "  float diff = max(dot(norm, lightDir), 0.0);\n"
  

  "  vec3 viewDir = vec3(0.0, 0.0, 1.0);\n" // Wir schauen von oben (Z)
  "  vec3 reflectDir = reflect(-lightDir, norm);\n"
  // 32.0 ist die "Härte" des Glanzes. Je höher, desto kleiner/schärfer der Punkt.
  "  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);\n" 


  "  vec3 result = (0.4 + 0.6 * diff) * color + vec3(0.5 * spec);\n"
  "  outColor = vec4(result, 1.0);\n"
  "}\n\0";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        error( std::string("vertex shader compilation failed") + infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        error( std::string("fragment shader compilation failed") + infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor"); // explizit aus viewer.cc
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        error( std::string("linking shader programs failed") + infoLog);
    }
}


bool OpenGLRenderer::init() {
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    error( std::string("Could not initialize SDL. SDLError: ") + SDL_GetError() );
  } else {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if( window == nullptr ) {
      error( std::string("Could not create Window. SDLError: ") + SDL_GetError() );
    } else {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
      SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

      context = SDL_GL_CreateContext(window);
      
      GLenum err = glewInit(); // to be called after OpenGL render context is created
      if (GLEW_OK != err) {
        error( "Could not initialize Glew. Glew error message: " );
        error( glewGetErrorString(err) );
      }
      debug(1, "Using GLEW Version: ");
      debug(1, glewGetString(GLEW_VERSION) );

      SDL_GL_SetSwapInterval(1);

      glEnable(GL_DEPTH_TEST); //Tiefentest

      create_shader_programs();
      createVbos();
      createSpaceShipView();
      createDigitViews();
      return true;
    }
  }
  return false;
}

/* tile positions in world coordinates
   used to draw objects seemless between boundary
  +---+---+---+   
  | 5 | 7 | 2 |
  +---+---+---+
  | 4 | 0 | 1 |
  +---+---+---+
  | 6 | 8 | 3 |
  +---+---+---+
*/
static Vector2df tile_positions [] = {
                         {0.0f, 0.0f},
                         {1024.0f, 0.0f},
                         {1024.0f, 768.0f},
                         {1024.0f, -768.0f},
                         {-1024.0f, 0.0f},
                         {-1024.0f, 768.0f},
                         {-1024.0f, -768.0f},
                         {0.0f, 768.0f},
                         {0.0f, -768.0f} };

void OpenGLRenderer::render() {
  debug(2, "render() entry...");

  // transformation to canonical view and from left handed to right handed coordinates
  SquareMatrix4df world_transformation =
                         SquareMatrix4df{
                           { 2.0f / 1024.0f,           0.0f,            0.0f,  0.0f},
                           {       0.0f,     -2.0f / 768.0f,            0.0f,  0.0f}, // (negative, because we have a left handed world coord. system)
                           {       0.0f,               0.0f,  2.0f / 1024.0f,  0.0f},
                           {      -1.0f,               1.0f,           0.0f,  1.0f}
                         };
                                                 
  glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
  //glClear ( GL_COLOR_BUFFER_BIT );
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  debug(2, "remove views for deleted objects");

  // remove all views for typed bodies that have to be deleted 
  erase_if(views, []( std::unique_ptr<TypedBodyView> & view) { return view->get_typed_body()->is_marked_for_deletion();}); 

  auto new_bodies = game.get_physics().get_recently_added_bodies();
  for (Body2df * body : new_bodies) {
    assert(body != nullptr);
    TypedBody * typed_body = static_cast<TypedBody *>(body);
    auto type = typed_body->get_type();
    if (type == BodyType::spaceship) {
      create( static_cast<Spaceship *>(typed_body), views );
    } else if (type == BodyType::torpedo ) {
      create( static_cast<Torpedo *>(typed_body), views );
    } else  if (type == BodyType::asteroid) {
      create( static_cast<Asteroid *>(typed_body), views );
    } else if (type == BodyType::saucer) {
      create( static_cast<Saucer *>(typed_body), views );
    } else if (type == BodyType::spaceship_debris ) {
      create( static_cast<SpaceshipDebris *>(typed_body), views );
    } else if (type == BodyType::debris) {
      create( static_cast<Debris *>(typed_body), views );
    }
  }

  debug(2, "render all views");
  // for (auto & view : views) {
  //   view->render( world_transformation );
  // }

  // -----------------------------------------------------------------------
  // AUFGABE 1: 
  // -----------------------------------------------------------------------
  SquareMatrix4df view_transformation = world_transformation;

  // Wenn das Schiff existiert -> Kamera zum Schiff verschieben
  if (game.ship_exists()) {
      TypedBody* ship = game.get_ship();
      Vector2df ship_pos = ship->get_position();

      // Schiff ist in Mitte des Fensters (1024/2, 768/2)
      float target_x = 512.0f;
      float target_y = 384.0f;

      // Verschiebung berechnen
      float dx = target_x - ship_pos[0];
      float dy = target_y - ship_pos[1];

      // Translationsmatrix erstellen
      SquareMatrix4df camera_translation = {
          {1.0f, 0.0f, 0.0f, 0.0f},
          {0.0f, 1.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 1.0f, 0.0f},
          {dx,   dy,   0.0f, 1.0f}
      };

      // Kamera-Verschiebung in View-Matrix einrechnen
      view_transformation = view_transformation * camera_translation;
  }

  // -----------------------------------------------------------------------
  // AUFGABE 2: 
  // -----------------------------------------------------------------------
  
  //9 Positionen für die Kacheln
  std::vector<Vector2df> tile_positions = {
      {0.0f, 0.0f},      {1024.0f, 0.0f},      {-1024.0f, 0.0f},
      {0.0f, 768.0f},    {1024.0f, 768.0f},    {-1024.0f, 768.0f},
      {0.0f, -768.0f},   {1024.0f, -768.0f},   {-1024.0f, -768.0f}
  };

  for (const auto& offset : tile_positions) {
      
      // Matrix für die Kachel-Verschiebung
      SquareMatrix4df tile_translation = { 
          {1.0f, 0.0f, 0.0f, 0.0f},
          {0.0f, 1.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 1.0f, 0.0f},
          {offset[0], offset[1], 0.0f, 1.0f} 
      };

      // Erst Kamera, dann Kachel-Verschiebung anwenden
      // Reihenfolge der Multiplikation: ViewTrans * TileTrans
      SquareMatrix4df current_transform = view_transformation * tile_translation;

      // Alle Spielobjekte für diese Kachel zeichnen
      for (auto & view : views) {
        view->render( current_transform );
      }
  }
  
  renderFreeShips(world_transformation);
  renderScore(world_transformation);

  SDL_GL_SwapWindow(window);
  debug(2, "render() exit.");

}

void OpenGLRenderer::exit() {
  views.clear();
}
 
