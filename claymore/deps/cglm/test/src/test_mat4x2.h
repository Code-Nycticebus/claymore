/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "test_common.h"

#define A_MATRIX4X2 {{1,2},{3,4},{5,6},{7,8}}
#define A_MATRIX4X2_TRANSPOSE {{1,3,5,7}, {2,4,6,8}}

#ifndef CGLM_TEST_MAT4X2_ONCE
#define CGLM_TEST_MAT4X2_ONCE

TEST_IMPL(MACRO_GLM_MAT4X2_ZERO_INIT) {
  mat4x2 mat4x2_zero = GLM_MAT4X2_ZERO_INIT;
  test_assert_mat4x2_eq_zero(mat4x2_zero);
  TEST_SUCCESS
}

TEST_IMPL(MACRO_GLM_MAT4X2_ZERO) {
  mat4x2 mat4x2_zero = GLM_MAT4X2_ZERO;
  test_assert_mat4x2_eq_zero(mat4x2_zero);
  TEST_SUCCESS
}

#endif /* CGLM_TEST_MAT4X2_ONCE */

TEST_IMPL(GLM_PREFIX, mat4x2_copy) {
  mat4x2 m1 = A_MATRIX4X2;
  mat4x2 m2 = GLM_MAT4X2_ZERO_INIT;

  GLM(mat4x2_copy)(m1, m2);

  test_assert_mat4x2_eq(m1, m2);

  TEST_SUCCESS
}

TEST_IMPL(GLM_PREFIX, mat4x2_zero) {
  mat4x2 m1 = GLM_MAT4X2_ZERO_INIT;
  mat4x2 m2 = GLM_MAT4X2_ZERO_INIT;
  mat4x2 m3;

  GLM(mat4x2_zero)(m3);

  ASSERTIFY(test_assert_mat4x2_eq_zero(m1))
  ASSERTIFY(test_assert_mat4x2_eq_zero(m2))
  ASSERTIFY(test_assert_mat4x2_eq_zero(m3))

  TEST_SUCCESS
}

TEST_IMPL(GLM_PREFIX, mat4x2_make) {
  float src[24] = {
    0.5f, 1.7f, 10.3f, 4.2f, 8.9f, 1.1f, 2.3f, 4.2f,
    2.3f, 4.2f, 66.5f, 23.7f, 6.6f, 8.9f, 0.5f, 1.7f,
    5.3f, 4.8f, 96.3f, 13.7f, 4.7f, 5.5f, 2.3f, 4.2f
  };

  mat4x2 dest[3];

  float *srcp = src;
  unsigned int i, j, k;

  for (i = 0, j = 0, k = 0; i < sizeof(src) / sizeof(float); i+=8,j++) {
    GLM(mat4x2_make)(srcp + i, dest[j]);

    ASSERT(test_eq(src[ i ], dest[j][k][0]));
    ASSERT(test_eq(src[i+1], dest[j][k][1]));

    ASSERT(test_eq(src[i+2], dest[j][k+1][0]));
    ASSERT(test_eq(src[i+3], dest[j][k+1][1]));

    ASSERT(test_eq(src[i+4], dest[j][k+2][0]));
    ASSERT(test_eq(src[i+5], dest[j][k+2][1]));

    ASSERT(test_eq(src[i+6], dest[j][k+3][0]));
    ASSERT(test_eq(src[i+7], dest[j][k+3][1]));
  }

  TEST_SUCCESS
}

TEST_IMPL(GLM_PREFIX, mat4x2_mul) {
  mat4x2 m1 = GLM_MAT4X2_ZERO_INIT;
  mat2x4 m2 = GLM_MAT2X4_ZERO_INIT;

  mat2 m3 = GLM_MAT2_ZERO_INIT;
  mat2 m4 = GLM_MAT2_ZERO_INIT;

  int c, r, k;

  test_rand_mat4x2(m1);
  test_rand_mat2x4(m2);

  for (r = 0; r < 2; r++) {
    for (c = 0; c < 2; c++) {
      for (k = 0; k < 4; k++) {
	m4[c][r] += m1[k][r] * m2[c][k];
      }
    }
  }

  GLM(mat4x2_mul)(m1, m2, m3);
  ASSERTIFY(test_assert_mat2_eq(m3, m4))

  TEST_SUCCESS
}

TEST_IMPL(GLM_PREFIX, mat4x2_mulv) {
  mat4x2 mat = A_MATRIX4X2;
  vec4 v = {11.0f, 21.0f, 31.0f, 41.0f};

  int  i;
  vec2 dest;
  float res = 0.0;

  GLM(mat4x2_mulv)(mat, v, dest);

  for (i = 0; i < 2; i++) {
    res = mat[0][i] * v[0] + mat[1][i] * v[1] + mat[2][i] * v[2] + mat[3][i] * v[3];
    ASSERT(test_eq(dest[i], res))
  }

  TEST_SUCCESS
}

TEST_IMPL(GLM_PREFIX, mat4x2_transpose) {
  mat4x2 m1 = A_MATRIX4X2;

  mat2x4 m2;
  mat2x4 m3 = A_MATRIX4X2_TRANSPOSE;
  GLM(mat4x2_transpose)(m1, m2);

  ASSERTIFY(test_assert_mat2x4_eq(m2, m3))

  TEST_SUCCESS
}

TEST_IMPL(GLM_PREFIX, mat4x2_scale) {
  mat4x2 m1 = A_MATRIX4X2;
  mat4x2 m2 = A_MATRIX4X2;
  int i, j, scale;

  scale = rand() % 100;

  GLM(mat4x2_scale)(m1, (float) scale);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 2; j++) {
      ASSERT(test_eq(m1[i][j], m2[i][j] * scale))
    }
  }

  TEST_SUCCESS
}
