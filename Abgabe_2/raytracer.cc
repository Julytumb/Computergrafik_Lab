#include "math.h"
#include "geometry.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>

// Die folgenden Kommentare beschreiben Datenstrukturen und Funktionen
// Die Datenstrukturen und Funktionen die weiter hinten im Text beschrieben sind,
// hängen höchstens von den vorhergehenden Datenstrukturen ab, aber nicht umgekehrt.



// Ein "Bildschirm", der das Setzen eines Pixels kapselt
// Der Bildschirm hat eine Auflösung (Breite x Höhe)
// Kann zur Ausgabe einer PPM-Datei verwendet werden oder
// mit SDL2 implementiert werden.
class Screen {
    int width, height;
    std::vector<Vector3df> buffer; //Liste von Farben die alle Pixel nacheinander speichert

public:
    Screen(int w, int h) : width(w), height(h), buffer(w * h, Vector3df({0.0f, 0.0f, 0.0f})) {} //erstelle Screen und fülle alles vorerst mit schwarz

    void set_pixel(int x, int y, const Vector3df& color) { //schreibe Farbe an Position, 
        if (x >= 0 && x < width && y >= 0 && y < height){   //prüfe ob im festgelegten Bereich
            buffer[y * width + x] = color;
        }
    }

    void write_ppm(const char* filename) const { //ppm erzeugen
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "P3\n" << width << " " << height << "\n255\n"; //Header
            for (int y = 0; y < height; ++y) {  //Umrechnung für die richtige Darstellung (Monitor braucht Werte von 0-255)
                for (int x = 0; x < width; ++x) {
                    Vector3df c = buffer[y * width + x];
                    float r = std::isnan(c[0]) ? 0.0f : std::max(0.0f, std::min(1.0f, c[0]));
                    float g = std::isnan(c[1]) ? 0.0f : std::max(0.0f, std::min(1.0f, c[1]));
                    float b = std::isnan(c[2]) ? 0.0f : std::max(0.0f, std::min(1.0f, c[2]));
                    file << int(r * 255) << " " << int(g * 255) << " " << int(b * 255) << " ";
                }
                file << "\n";
            }
        }
    }
};


// Eine "Kamera", die von einem Augenpunkt aus in eine Richtung senkrecht auf ein Rechteck (das Bild) zeigt.
// Für das Rechteck muss die Auflösung oder alternativ die Pixelbreite und -höhe bekannt sein.
// Für ein Pixel mit Bildkoordinate kann ein Sehstrahl erzeugt werden.
class Camera {
    Vector3df origin, lower_left, horizontal, vertical;
public:
    Camera(Vector3df from, Vector3df at, Vector3df up, float vfov, float aspect)    //Konstruktor: wo steht man, wo schaut man hin, wo ist oben und wie ist der Blickwinkel
        : origin({0,0,0}), lower_left({0,0,0}), horizontal({0,0,0}), vertical({0,0,0}) 
    {
        float theta = vfov * 3.14159f / 180.0f;
        float half_h = std::tan(theta / 2.0f);
        float half_w = aspect * half_h;
        origin = from;  //Koordinatensystem für Kamera
        Vector3df w = from - at; w.normalize(); //nach hinten
        Vector3df u = up.cross_product(w); u.normalize();   //nach rechts
        Vector3df v = w.cross_product(u);   //nach oben
        lower_left = origin - half_w*u - half_h*v - w;
        horizontal = 2*half_w*u; vertical = 2*half_h*v;
    }
    Ray3df get_ray(float s, float t) const {    //Sehstrahl
        Vector3df dir = lower_left + s*horizontal + t*vertical - origin;
        dir.normalize();
        return { origin, dir };
    }
};


// Für die "Farbe" benötigt man nicht unbedingt eine eigene Datenstruktur.
// Sie kann als Vector3df implementiert werden mit Farbanteil von 0 bis 1.
// Vor Setzen eines Pixels auf eine bestimmte Farbe (z.B. 8-Bit-Farbtiefe),
// kann der Farbanteil mit 255 multipliziert  und der Nachkommaanteil verworfen werden.


// Das "Material" der Objektoberfläche mit ambienten, diffusem und reflektiven Farbanteil.
struct Material {
    Vector3df color;
    float ka, kd, ks; //ka = Helligkeit, kd=beschaffenheit (matt/rau) ks= spiegelung
    Material(Vector3df c, float a, float d, float s) : color(c), ka(a), kd(d), ks(s) {}
};


// Ein "Objekt", z.B. eine Kugel oder ein Dreieck, und dem zugehörigen Material der Oberfläche.
// Im Prinzip ein Wrapper-Objekt, das mindestens Material und geometrisches Objekt zusammenfasst.
// Kugel und Dreieck finden Sie in geometry.h/tcc
struct SceneObject {    //verbinde Objekt (Kugel) mit Material
    Sphere3df sphere;
    Material material;
    SceneObject(Vector3df p, float r, Material m) : sphere(p, r), material(m) {}
};


// verschiedene Materialdefinition, z.B. Mattes Schwarz, Mattes Rot, Reflektierendes Weiss, ...
// im wesentlichen Variablen, die mit Konstruktoraufrufen initialisiert werden.
// (Werden in main initialisiert)


// Die folgenden Werte zur konkreten Objekten, Lichtquellen und Funktionen, wie Lambertian-Shading
// oder die Suche nach einem Sehstrahl für das dem Augenpunkt am nächsten liegenden Objekte,
// können auch zusammen in eine Datenstruktur für die gesammte zu
// rendernde "Szene" zusammengefasst werden.

// Die Cornelbox aufgebaut aus den Objekten
// Am besten verwendet man hier einen std::vector< ... > von Objekten.

// Punktförmige "Lichtquellen" können einfach als Vector3df implementiert werden mit weisser Farbe,
// bei farbigen Lichtquellen müssen die entsprechenden Daten in Objekt zusammengefaßt werden
// Bei mehreren Lichtquellen können diese in einen std::vector gespeichert werden.
struct PointLight {     //Lichtquelle mit Position und Farbe(Lichtstärke)
    Vector3df position;
    Vector3df color;
};

// Sie benötigen eine Implementierung von Lambertian-Shading, z.B. als Funktion
// Benötigte Werte können als Parameter übergeben werden, oder wenn diese Funktion eine Objektmethode eines
// Szene-Objekts ist, dann kann auf die Werte teilweise direkt zugegriffen werden.
// Bei mehreren Lichtquellen muss der resultierende diffuse Farbanteil durch die Anzahl Lichtquellen geteilt werden.
Vector3df lambertian_shading(const SceneObject* obj, const Intersection_Context<float, 3u>& ctx, 
                             const std::vector<PointLight>& lights, const std::vector<SceneObject>& objects) {
    
    // Ambient
    Vector3df color = obj->material.ka * obj->material.color;
    
    // Diffus (Summieren)
    Vector3df diffuse_acc = {0.0f, 0.0f, 0.0f};

    for (const auto& light : lights) {  //berechne Vektor L von getroffenem Punkt zur Lampe
        Vector3df L = light.position - ctx.intersection;
        float dist = L.length();
        L.normalize();

        // Schatten
        Ray3df shadow_ray = { ctx.intersection + 0.01f * ctx.normal, L };   //steht was im Weg?
        Intersection_Context<float, 3u> s_ctx = {0,0,0,{0,0,0},{0,0,0}};
        bool shadow = false;
        
        for (const auto& o : objects) {
            if (o.sphere.intersects(shadow_ray, s_ctx) && s_ctx.t < dist) {
                shadow = true; break;
            }
        }

        if (!shadow) {
            float n_dot_l = std::max(0.0f, ctx.normal * L);
            diffuse_acc[0] += obj->material.color[0] * light.color[0] * n_dot_l;
            diffuse_acc[1] += obj->material.color[1] * light.color[1] * n_dot_l;
            diffuse_acc[2] += obj->material.color[2] * light.color[2] * n_dot_l;
        }
    }

    // "muss der resultierende diffuse Farbanteil durch die Anzahl Lichtquellen geteilt werden" 
    if (!lights.empty()) {
        float inv_n = 1.0f / lights.size();
        diffuse_acc = inv_n * diffuse_acc;
    }

    color += obj->material.kd * diffuse_acc;
    return color;
}

// Für einen Sehstrahl aus allen Objekte, dasjenige finden, das dem Augenpunkt am nächsten liegt.
// Am besten einen Zeiger auf das Objekt zurückgeben. Wenn dieser nullptr ist, dann gibt es kein sichtbares Objekt.
const SceneObject* find_nearest(const Ray3df& ray, const std::vector<SceneObject>& objects, Intersection_Context<float, 3u>& hit_ctx) {
    const SceneObject* hit = nullptr;
    float min_t = 1e9f;
    Intersection_Context<float, 3u> ctx = {0,0,0,{0,0,0},{0,0,0}};
    
    for (const auto& obj : objects) {
        if (obj.sphere.intersects(ray, ctx)) {
            if (ctx.t > 0.01f && ctx.t < min_t) {
                min_t = ctx.t;
                hit = &obj;
                hit_ctx = ctx;
            }
        }
    }
    return hit;
}

// Die rekursive raytracing-Methode. Am besten ab einer bestimmten Rekursionstiefe (z.B. als Parameter übergeben) abbrechen.
Vector3df trace(const Ray3df& ray, const std::vector<SceneObject>& objects, const std::vector<PointLight>& lights, int depth) {
    if (depth <= 0) return {0.0f, 0.0f, 0.0f};

    Intersection_Context<float, 3u> ctx = {0,0,0,{0,0,0},{0,0,0}};
    const SceneObject* hit = find_nearest(ray, objects, ctx);

    if (!hit) return {0.0f, 0.0f, 0.0f};

    // Shading
    Vector3df col = lambertian_shading(hit, ctx, lights, objects);

    // Reflexion
    if (hit->material.ks > 0.0f) {
        Vector3df r_dir = ray.direction.get_reflective(ctx.normal);
        r_dir.normalize();
        
        Vector3df r_col = trace({ctx.intersection + 0.01f * ctx.normal, r_dir}, objects, lights, depth - 1);
        col = (1.0f - hit->material.ks) * col + hit->material.ks * r_col;
    }
    return col;
}


int main(void) {
  // Bildschirm erstellen
  int width = 800, height = 600;
  Screen screen(width, height);

  // Kamera erstellen
  Camera cam({0,0,3}, {0,0,-1}, {0,1,0}, 45.0f, float(width)/height);

  // Materialien
  Material m_white({0.9f, 0.9f, 0.9f}, 0.1f, 0.9f, 0.0f);
  Material m_red  ({0.9f, 0.1f, 0.1f}, 0.1f, 0.9f, 0.0f);
  Material m_green({0.1f, 0.9f, 0.1f}, 0.1f, 0.9f, 0.0f);
  Material m_mirror({0.9f, 0.9f, 0.9f}, 0.0f, 0.1f, 0.9f);

  // Die Cornelbox aufgebaut aus den Objekten
  std::vector<SceneObject> objects;
  float R = 10000.0f; 

  // Cornell Box (Kamera bei z=3 schaut nach -z)
  objects.push_back(SceneObject({-1.0f-R, 0, -2}, R, m_red));   // Links
  objects.push_back(SceneObject({ 1.0f+R, 0, -2}, R, m_green)); // Rechts
  objects.push_back(SceneObject({0, -1.0f-R, -2}, R, m_white)); // Boden
  objects.push_back(SceneObject({0,  1.0f+R, -2}, R, m_white)); // Decke
  objects.push_back(SceneObject({0, 0, -3.0f-R}, R, m_white));  // Hinten

  // Objekte in der Box
  objects.push_back(SceneObject({-0.4f, -0.7f, -1.5f}, 0.3f, m_white));  
  objects.push_back(SceneObject({ 0.4f, -0.7f, -2.0f}, 0.3f, m_mirror)); 

  // Punktförmige "Lichtquellen"
  std::vector<PointLight> lights;
  lights.push_back({{0, 0.8f, -1.5f}, {1, 1, 1}});

  std::cout << "Rendering started..." << std::endl;

  // Für jede Pixelkoordinate x,y
  for (int j = 0; j < height; ++j) {
      for (int i = 0; i < width; ++i) {
          // Sehstrahl für x,y mit Kamera erzeugen
          float u = float(i) / width;
          // FLIP V: Wenn es vorher auf dem Kopf war, nutzen wir jetzt diese Richtung
          float v = float(j) / height; 

          Ray3df r = cam.get_ray(u, v);
          
          // Farbe mit raytracing-Methode bestimmen
          Vector3df col = trace(r, objects, lights, 5);
          
          // Beim Bildschirm die Farbe für Pixel x,y, setzten
          screen.set_pixel(i, j, col);
      }
  }

  screen.write_ppm("cornell_box.ppm");
  return 0;   
}