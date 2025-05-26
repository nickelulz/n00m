#ifndef RESOURCES_HEADER
#define RESOURCES_HEADER

#define DEFINE_SHADER_SOURCE(name) \
  const char *SHADER_FILE_(name) = "./res/shaders/(name).glsl";

DEFINE_SHADER_SOURCE(vertex)
DEFINE_SHADER_SOURCE(fragment)

#undef DEFINE_SHADER_SOURCE
     
#endif
