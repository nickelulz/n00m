#include "state.h"

static const float VERTICES[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};

static const unsigned int INDICES[] = {
  0, 1, 2;
}

void
state_init (state_t *state, graphics_t *gfx)
{
  state->triangle = render_object_create (VERTICES, INDICES, gfx->shader);
}
