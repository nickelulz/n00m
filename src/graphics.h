#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include <glad/gl.h>

#define EXPAND_COLOR(color) color.r, color.g, color.b, color.a

typedef struct _color
{
  float r, g, b, a;
} color_t;

typedef unsigned int shader_t;

typedef struct _render_object
{
  unsigned int vao, vbo;
  shader_t shader;
  int vertex_count;
  color_t color;
} render_object_t;

enum { SHADER_MAIN, NUM_SHADERS };

typedef struct _graphics
{
  GLFWwindow *window;

  // rendering objects
  unsigned int vbo;

  // palette
  color_t background_color;
  shader_t shaders[NUM_SHADERS];
} graphics_t;

void graphics_init (graphics_t *gfx);

/* create/destroy/render render objects */
render_object_t render_object_create (float *vertices, unsigned int *indices,
                                      unsigned int vertex_count, color_t color, shader_t shader);
void render_object_destroy (render_object_t *obj);
void draw_render_object (render_object_t *obj);

#endif
