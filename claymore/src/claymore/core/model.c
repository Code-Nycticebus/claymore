#include "model.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t cap;
  size_t len;
  float *data;
} FloatStack;

static FloatStack float_stack_create(size_t capacity) {
  FloatStack stack;
  stack.cap = capacity;
  stack.len = 0;
  stack.data = malloc(capacity * sizeof(float));
  if (stack.data == NULL) {
    fprintf(stderr, "Memory allocation failed: %s\n", strerror(errno));
    exit(-1);
  }
  return stack;
}

static void float_stack_free(FloatStack *stack) {
  free(stack->data);
  stack->data = NULL;
  stack->len = 0;
  stack->cap = 1;
}

static void float_stack_push(FloatStack *stack, float value) {
  if (!(stack->len < stack->cap)) {
    stack->cap *= 2;
    stack->data = realloc(stack->data, stack->cap * sizeof(float));
    if (stack->data == NULL) {
      fprintf(stderr, "Memory reallocation failed: %s\n", strerror(errno));
      exit(-1);
    }
  }
  stack->data[stack->len] = value;
  stack->len++;
}

#define MAX_LINE_LEN 200
// TODO return cm_mesh
CmMesh cm_model_load(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Could not open model file '%s': %s\n", filename,
            strerror(errno));
    return (CmMesh){0};
  }

  CmMesh mesh = {0};

  FloatStack vertex_stack = float_stack_create(1);
  FloatStack vertex_index_stack = float_stack_create(1);
  FloatStack normal_stack = float_stack_create(1);
  FloatStack normal_index_stack = float_stack_create(1);
  FloatStack texture_stack = float_stack_create(1);
  FloatStack texture_index_stack = float_stack_create(1);

  char buffer[MAX_LINE_LEN] = {0};
  for (int line = 0; fgets(buffer, MAX_LINE_LEN, file) != NULL; line++) {
    if (buffer[0] == 'v') {
      // Parsing vectors
      switch (buffer[1]) {
      case ' ': {
        float f[3];
        if (sscanf(buffer, "v %f %f %f", &f[0], &f[1], &f[2]) != 3) {
          cm_log_error("%s:%d: Not enough arguments: %s\n", filename, line,
                       buffer);
          goto DEFER;
        }
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&vertex_stack, f[i]);
        }
        break;
      }
      case 'n': {
        float f[3];
        if (sscanf(buffer, "vn %f %f %f", &f[0], &f[1], &f[2]) != 3) {
          cm_log_error("%s:%d: Not enough arguments: %s", filename, line,
                       buffer);
          goto DEFER;
        }
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&normal_stack, f[i]);
        }
        break;
      }
      case 't': {
        float f[2];
        if (sscanf(buffer, "vt %f %f", &f[0], &f[1]) != 2) {
          cm_log_error("%s:%d: Not enough arguments: %s", filename, line,
                       buffer);
          goto DEFER;
        }
        for (size_t i = 0; i < 2; i++) {
          float_stack_push(&texture_stack, f[i]);
        }
        break;
      }
      default:
        cm_log_error("%s:%d: Unkown format: %s", filename, line, buffer);
        goto DEFER;
      }
    } else if (buffer[0] == 'f') {
      float vi[3];
      float ni[3];
      float ti[3];
      if (sscanf(buffer, "f %f/%f/%f %f/%f/%f %f/%f/%f", &vi[0], &ti[0], &ni[0],
                 &vi[1], &ti[1], &ni[1], &vi[2], &ti[2], &ni[2]) == 9) {
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&vertex_index_stack, vi[i]);
          float_stack_push(&normal_index_stack, ni[i]);
          float_stack_push(&texture_index_stack, ti[i]);
        }
      } else if (sscanf(buffer, "f %f/%f %f/%f %f/%f", &vi[0], &ti[0], &vi[1],
                        &ti[1], &vi[2], &ti[2]) == 6) {
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&vertex_index_stack, vi[i]);
          float_stack_push(&texture_index_stack, ti[i]);
        }
      } else if (sscanf(buffer, "f %f//%f %f//%f %f//%f", &vi[0], &ni[0],
                        &vi[1], &ni[1], &vi[2], &ni[2]) == 6) {
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&vertex_index_stack, vi[i]);
          float_stack_push(&normal_index_stack, ni[i]);
        }
      } else if (sscanf(buffer, "f %f %f %f", &vi[0], &vi[1], &vi[2]) == 3) {
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&vertex_index_stack, vi[i]);
        }
      } else {
        cm_log_error("%s:%d: Unkown format: %s", filename, line, buffer);
        goto DEFER;
      }
    }
  }

  FloatStack real_vertices = float_stack_create(1);
  for (size_t i = 0; i < vertex_index_stack.len; i++) {
    uint32_t index = (uint32_t)vertex_index_stack.data[i] - 1;
    vec3s *vec = (void *)vertex_stack.data;
    for (size_t j = 0; j < 3; j++) {
      float_stack_push(&real_vertices, vec[index].raw[j]);
    }
  }
  mesh = cm_mesh_create((vec3s *)real_vertices.data, real_vertices.len / 3);
  float_stack_free(&real_vertices);

  if (0 < normal_index_stack.len) {
    FloatStack real_normals = float_stack_create(1);
    for (size_t i = 0; i < normal_index_stack.len; i++) {
      uint32_t index = (uint32_t)normal_index_stack.data[i] - 1;
      vec3s *vec = (void *)normal_stack.data;
      for (size_t j = 0; j < 3; j++) {
        float_stack_push(&real_normals, vec[index].raw[j]);
      }
    }
    cm_mesh_attach_normals(&mesh, (vec3s *)real_normals.data,
                           real_normals.len / 3);
    float_stack_free(&real_normals);
  }

  if (0 < texture_index_stack.len) {
    FloatStack real_textures = float_stack_create(1);
    for (size_t i = 0; i < texture_index_stack.len; i++) {
      uint32_t index = (uint32_t)texture_index_stack.data[i] - 1;
      vec2s *vec = (void *)texture_stack.data;
      for (size_t j = 0; j < 2; j++) {
        float_stack_push(&real_textures, vec[index].raw[j]);
      }
    }
    cm_mesh_attach_uv(&mesh, (vec2s *)real_textures.data,
                      real_textures.len / 2);
    // float_stack_dump(&real_textures);
    float_stack_free(&real_textures);
  }

DEFER:
  fclose(file);
  float_stack_free(&vertex_stack);
  float_stack_free(&vertex_index_stack);
  float_stack_free(&normal_stack);
  float_stack_free(&normal_index_stack);
  float_stack_free(&texture_stack);
  float_stack_free(&texture_index_stack);
  return mesh;
}
