/*************************************************************************/
/*  godot_vector3.cpp                                                    */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "godot_vector3.h"

#include "math/vector3.h"

#ifdef __cplusplus
extern "C" {
#endif

void _vector3_api_anchor() {
}

void GDAPI godot_vector3_new(godot_vector3 *p_v, const godot_real p_x, const godot_real p_y, const godot_real p_z) {
	Vector3 *v = (Vector3 *)p_v;
	*v = Vector3(p_x, p_y, p_z);
}

void GDAPI godot_vector3_set_axis(godot_vector3 *p_v, const godot_int p_axis, const godot_real p_val) {
	Vector3 *v = (Vector3 *)p_v;
	v->set_axis(p_axis, p_val);
}

godot_real GDAPI godot_vector3_get_axis(const godot_vector3 *p_v, const godot_int p_axis) {
	Vector3 *v = (Vector3 *)p_v;
	return v->get_axis(p_axis);
}

godot_int GDAPI godot_vector3_min_axis(const godot_vector3 *p_v) {
	Vector3 *v = (Vector3 *)p_v;
	return v->min_axis();
}

godot_int GDAPI godot_vector3_max_axis(const godot_vector3 *p_v) {
	Vector3 *v = (Vector3 *)p_v;
	return v->max_axis();
}

godot_real GDAPI godot_vector3_length(const godot_vector3 *p_v) {
	Vector3 *v = (Vector3 *)p_v;
	return v->length();
}

godot_real GDAPI godot_vector3_length_squared(const godot_vector3 *p_v) {
	Vector3 *v = (Vector3 *)p_v;
	return v->length_squared();
}

void GDAPI godot_vector3_normalize(godot_vector3 *p_v) {
	Vector3 *v = (Vector3 *)p_v;
	v->normalize();
}

void GDAPI godot_vector3_normalized(godot_vector3 *p_dest, const godot_vector3 *p_src) {
	Vector3 *src = (Vector3 *)p_src;
	Vector3 *dest = (Vector3 *)p_dest;
	*dest = src->normalized();
}

/*
 * inverse
 * zero
 * snap
 * snapped
 * rotate
 * rotated
 *
 *
 * linear_interpolate
 * cubic_interpolate
 * cubic_interpolaten
 * cross
 * dot
 * outer
 * to_diagonal_matrix
 * abs
 * floor
 * ceil
 */

godot_real GDAPI godot_vector3_distance_to(const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	return a->distance_to(*b);
}

godot_real GDAPI godot_vector3_distance_squared_to(const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	return a->distance_squared_to(*b);
}

/*
 * slide
 * reflect
 */

void GDAPI godot_vector3_operator_add(godot_vector3 *p_dest, const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *dest = (Vector3 *)p_dest;
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	*dest = *a + *b;
}

void GDAPI godot_vector3_operator_subtract(godot_vector3 *p_dest, const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *dest = (Vector3 *)p_dest;
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	*dest = *a - *b;
}

void GDAPI godot_vector3_operator_multiply_vector(godot_vector3 *p_dest, const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *dest = (Vector3 *)p_dest;
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	*dest = *a * *b;
}

void GDAPI godot_vector3_operator_multiply_scalar(godot_vector3 *p_dest, const godot_vector3 *p_a, const godot_real p_b) {
	Vector3 *dest = (Vector3 *)p_dest;
	Vector3 *a = (Vector3 *)p_a;
	*dest = *a * p_b;
}

void GDAPI godot_vector3_operator_divide_vector(godot_vector3 *p_dest, const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *dest = (Vector3 *)p_dest;
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	*dest = *a / *b;
}

void GDAPI godot_vector3_operator_divide_scalar(godot_vector3 *p_dest, const godot_vector3 *p_a, const godot_real p_b) {
	Vector3 *dest = (Vector3 *)p_dest;
	Vector3 *a = (Vector3 *)p_a;
	*dest = *a / p_b;
}

godot_bool GDAPI godot_vector3_operator_equal(const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	return *a == *b;
}

godot_bool GDAPI godot_vector3_operator_less(const godot_vector3 *p_a, const godot_vector3 *p_b) {
	Vector3 *a = (Vector3 *)p_a;
	Vector3 *b = (Vector3 *)p_b;
	return *a < *b;
}

#ifdef __cplusplus
}
#endif
