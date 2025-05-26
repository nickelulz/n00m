#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "log.h"

#include "config.h"
#include "graphics.h"

static void
loop (graphics_t *gfx, state_t *state)
{
  glClearColor(EXPAND_COLOR(gfx->background_color));
  glClear(GL_COLOR_BUFFER_BIT);

  graphics_draw(state->triangle);
  
  glfwSwapBuffers(gfx.window);
  glfwPollEvents();
}

int main(int argc, char* argv[])
{
  config_t config;
  config_load(&config);
  
  graphics_t gfx;
  graphics_init(&gfx);

  state_t state;
  state_init(&state, &gfx);
  
  glGenBuffers(1, &gfx.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gfx.vbo);

  while (!glfwWindowShouldClose(gfx.window))
    loop(gfx, state);

  graphics_close(&gfx);
  config_close(&config);
  
  exit(EXIT_SUCCESS);
}
