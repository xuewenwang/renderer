#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct {float x, y;} vec2_t;
typedef struct {float x, y, z;} vec3_t;
typedef struct {float x, y, z, w;} vec4_t;
typedef struct {float m[4][4];} mat4_t;

/* vec2 related functions */
vec2_t vec2_new(float x, float y);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
void vec2_print(const char *name, vec2_t v);

/* vec3 related functions */
vec3_t vec3_new(float x, float y, float z);
vec3_t vec3_from_vec4(vec4_t v);
vec3_t vec3_negative(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_scale(vec3_t v, float scale);
float vec3_length(vec3_t v);
vec3_t vec3_normalize(vec3_t v);
float vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);
void vec3_print(const char *name, vec3_t v);

/* vec4 related functions */
vec4_t vec4_new(float x, float y, float z, float w);
vec4_t vec4_from_vec3(vec3_t v, float w);
vec4_t vec4_saturate(vec4_t v);
void vec4_print(const char *name, vec4_t v);

/* mat4 related functions */
mat4_t mat4_identity(void);
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);
mat4_t mat4_inverse(mat4_t m);
mat4_t mat4_transpose(mat4_t m);
mat4_t mat4_inverse_transpose(mat4_t m);
void mat4_print(const char *name, mat4_t m);

/* transformation matrices */
mat4_t mat4_translate(float dx, float dy, float dz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate(float angle, float vx, float vy, float vz);
mat4_t mat4_rotate_x(float angle);
mat4_t mat4_rotate_y(float angle);
mat4_t mat4_rotate_z(float angle);
mat4_t mat4_camera(vec3_t eye, vec3_t center, vec3_t up);
mat4_t mat4_lookat(vec3_t eye, vec3_t center, vec3_t up);
mat4_t mat4_ortho(float left, float right, float bottom, float top,
                  float near, float far);
mat4_t mat4_frustum(float left, float right, float bottom, float top,
                    float near, float far);
mat4_t mat4_perspective(float fovy, float aspect, float near, float far);

#endif