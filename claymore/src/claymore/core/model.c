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

static void float_stack_dump(FloatStack *stack) {
  for (size_t i = 0; i < stack->len; i += 3) {
    printf("%f %f %f\n", stack->data[i + 0], stack->data[i + 1],
           stack->data[i + 2]);
  }
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

  char buffer[MAX_LINE_LEN] = {0};
  for (int line = 0; fgets(buffer, MAX_LINE_LEN, file) != NULL; line++) {
    if (buffer[0] == 'v') {
      // Parsing vectors
      switch (buffer[1]) {
      case ' ': {
        float f[3];
        if (sscanf(buffer, "v %f %f %f", &f[0], &f[1], &f[2]) != 3) {
          fprintf(stderr, "%s:%d: Not enough arguments: %s\n", filename, line,
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
          fprintf(stderr, "%s:%d: Not enough arguments: %s\n", filename, line,
                  buffer);
          goto DEFER;
        }
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&normal_stack, f[i]);
        }
        break;
      }
      default:
        fprintf(stderr, "%s:%d: Unkown option: %s\n", filename, line, buffer);
        goto DEFER;
      }
    } else if (buffer[0] == 'f') {
      float vi[3];
      float ni[3];
      if (sscanf(buffer, "f %f//%f %f//%f %f//%f", &vi[0], &ni[0], &vi[1],
                 &ni[1], &vi[2], &ni[2]) == 6) {
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&vertex_index_stack, vi[i]);
          float_stack_push(&normal_index_stack, ni[i]);
        }
      } else if (sscanf(buffer, "f %f %f %f", &vi[0], &vi[1], &vi[2]) == 3) {
        for (size_t i = 0; i < 3; i++) {
          float_stack_push(&vertex_index_stack, vi[i]);
        }
      } else {
        fprintf(stderr, "%s:%d: Unkown format: %s\n", filename, line, buffer);
        goto DEFER;
      }
    }
  }

  float_stack_dump(&vertex_stack);
  float_stack_dump(&vertex_index_stack);
  float_stack_dump(&normal_stack);
  float_stack_dump(&normal_index_stack);

  uint32_t *indices = malloc(sizeof(uint32_t) * vertex_index_stack.len);
  for (size_t i = 0; i < vertex_index_stack.len; i++) {
    indices[i] = (uint32_t)vertex_index_stack.data[i] - 1;
  }
  mesh = cm_mesh_create((vec3s *)vertex_stack.data, vertex_stack.len / 3,
                        indices, vertex_index_stack.len);
  free(indices);

DEFER:
  fclose(file);
  return mesh;
}
