#include <stdlib.h>
#include <stdint.h>
#include "log.h"
#include "graphics.h"
#include "resources.h"
#include "util.h"

static void 
error_callback (int error, const char *description)
{
  log_error("%s", description);
}

static void 
key_callback (GLFWwindow *window, int key,
              int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

#define INFO_BUFFER_SIZE 512

static void
compile_shader (shader_t *shader, const char *source, int *success, char *info_log)
{
  /* build and compile the vertex shader */
  *vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(*vertex, 1, &vertex_source, NULL);
  glCompileShader(*vertex);

  /* check for errors */
  glGetShaderiv(*vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(*vertex, INFO_BUFFER_SIZE, NULL, info_log);
    log_error("Could not load shader at file %s: %s",
              vertex_filepath, info_log);
  }
}

static shader_t
read_shader (const char *vertex_filepath, const char *fragment_filepath)
{
  /* read vertex and fragment shader source files */
  char *vertex_source = read_file_to_str(vertex_filepath);
  char *fragment_source = read_file_to_str(fragment_filepath);

  shader_t vertex, fragment;
  int success;
  
  char info_log[INFO_BUFFER_SIZE];
  compile_shader(&vertex, &vertex_source, &success, &info_log);
  compile_shader(&fragment, &fragment_source, &success, &info_log);

  /* link both shaders into a combined program */
  shader_t program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  /* check for linking errors */
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, INFO_BUFFER_SIZE, NULL, info_log);
    log_error("Failed to link shaders %s and %s: %s",
              vertex_filepath, fragment_filepath, info_log);
    return NULL;
  }

  /* delete the previous shaders */
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  
  return program;
}

#undef INFO_BUFFER_SIZE

static void
load_shaders (graphics_t *gfx)
{
  gfx->shaders[SHADER_MAIN] = read_shader(SHADER_FILE_vertex, SHADER_FILE_fragment);
}

void
graphics_init (graphics_t *gfx, config_t *config)
{
  if (!glfwInit())
    exit(EXIT_FAILURE);
  
  gfx->window = glfwCreateWindow(config->window_width, config->window_height,
                                 config->window_title, NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  glfwSwapInterval(1);

  load_shaders(gfx);
}

void
graphics_close (graphics_t *gfx)
{
  glfwDestroyWindow(window);
  glDeleteProgram(gfx->shader);
  glfwTerminate();
}

void
draw_render_object (render_object_t *obj)
{
  glUseProgram(obj->shader);

  /* send color uniform if it exists */
  GLint color_location = glGetUniformLocation(obj->shader, "uColor");
  if (color_location != -1)
    glUniform3fv(color_location, 1, obj->color);

  glBindVertexArray(obj->vao);
  glDrawArrays(GL_TRIANGLES, 0, obj->vertex_count);
  glBindVertexArray(0);
  glUseProgram(0);
}

render_object_t
render_object_create (float *vertices, unsigned int *indices,
                      unsigned int vertex_count, color_t color, shader_t shader)
{
  render_object_t obj;
  obj.vertex_count = vertex_count;
  obj.shader = shader;

  glGenVertexArrays(1, &obj.vao);
  glGenBuffers(1, &obj.vbo);

  glBindVertexArray(obj.vao);
  glBindBuffer(GL_ARRAY_BUFFER, obj.vao);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  return obj;
}

void
render_object_destroy (render_object_t *obj)
{
  glDeleteVertexArrays(1, &obj->vao);
  glDeleteBuffers(1, &obj->vbo);
}
