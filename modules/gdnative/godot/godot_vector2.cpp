/*************************************************************************/
/*  godot_vector2.cpp                                                    */
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
#include "godot_vector2.h"

#include "math/math_2d.h"

#ifdef __cplusplus
extern "C" {
#endif

void _vector2_api_anchor() {
}

void GDAPI godot_vector2_new(godot_vector2 *p_v, godot_real p_x, godot_real p_y) {
	Vector2 *v = (Vector2 *)p_v;
	v->x = p_x;
	v->y = p_y;
}

void GDAPI godot_vector2_set_x(godot_vector2 *p_v, const godot_real p_x) {
	Vector2 *v = (Vector2 *)p_v;
	v->x = p_x;
}

void GDAPI godot_vector2_set_y(godot_vector2 *p_v, const godot_real p_y) {
	Vector2 *v = (Vector2 *)p_v;
	v->y = p_y;
}

godot_real GDAPI godot_vector2_get_x(const godot_vector2 *p_v) {
	Vector2 *v = (Vector2 *)p_v;
	return v->x;
}
godot_real GDAPI godot_vector2_get_y(const godot_vector2 *p_v) {
	Vector2 *v = (Vector2 *)p_v;
	return v->y;
}

void GDAPI godot_vector2_normalize(godot_vector2 *p_v) {
	Vector2 *v = (Vector2 *)p_v;
	v->normalize();
}
void GDAPI godot_vector2_normalized(godot_vector2 *p_dest, const godot_vector2 *p_src) {
	Vector2 *v = (Vector2 *)p_src;
	Vector2 *d = (Vector2 *)p_dest;

	*d = v->normalized();
}

godot_real GDAPI godot_vector2_length(const godot_vector2 *p_v) {
	Vector2 *v = (Vector2 *)p_v;
	return v->length();
}

godot_real GDAPI godot_vector2_length_squared(const godot_vector2 *p_v) {
	Vector2 *v = (Vector2 *)p_v;
	return v->length_squared();
}

godot_real GDAPI godot_vector2_distance_to(const godot_vector2 *p_a, const godot_vector2 *p_b) {
	Vector2 *a = (Vector2 *)p_a;
	Vector2 *b = (Vector2 *)p_b;
	return a->distance_to(*b);
}

godot_real GDAPI godot_vector2_distance_squared_to(const godot_vector2 *p_a, const godot_vector2 *p_b) {
	Vector2 *a = (Vector2 *)p_a;
	Vector2 *b = (Vector2 *)p_b;
	return a->distance_squared_to(*b);
}

void GDAPI godot_vector2_operator_add(godot_vector2 *p_dest, const godot_vector2 *p_a, const godot_vector2 *p_b) {
	Vector2 *dest = (Vector2 *)p_dest;
	const Vector2 *a = (Vector2 *)p_a;
	const Vector2 *b = (Vector2 *)p_b;
	*dest = *a + *b;
}

void GDAPI godot_vector2_operator_subtract(godot_vector2 *p_dest, const godot_vector2 *p_a, const godot_vector2 *p_b) {
	Vector2 *dest = (Vector2 *)p_dest;
	const Vector2 *a = (Vector2 *)p_a;
	const Vector2 *b = (Vector2 *)p_b;
	*dest = *a - *b;
}

void GDAPI godot_vector2_operator_multiply_vector(godot_vector2 *p_dest, const godot_vector2 *p_a, const godot_vector2 *p_b) {
	Vector2 *dest = (Vector2 *)p_dest;
	const Vector2 *a = (Vector2 *)p_a;
	const Vector2 *b = (Vector2 *)p_b;
	*dest = *a * *b;
}

void GDAPI godot_vector2_operator_multiply_scalar(godot_vector2 *p_dest, const godot_vector2 *p_a, const godot_real p_b) {
	Vector2 *dest = (Vector2 *)p_dest;
	const Vector2 *a = (Vector2 *)p_a;
	*dest = *a * p_b;
}

void GDAPI godot_vector2_operator_divide_vector(godot_vector2 *p_dest, const godot_vector2 *p_a, const godot_vector2 *p_b) {
	Vector2 *dest = (Vector2 *)p_dest;
	const Vector2 *a = (Vector2 *)p_a;
	const Vector2 *b = (Vector2 *)p_b;
	*dest = *a / *b;
}

void GDAPI godot_vector2_operator_divide_scalar(godot_vector2 *p_dest, const godot_vector2 *p_a, const godot_real p_b) {
	Vector2 *dest = (Vector2 *)p_dest;
	const Vector2 *a = (Vector2 *)p_a;
	*dest = *a / p_b;
}

godot_bool GDAPI godot_vector2_operator_equal(const godot_vector2 *p_a, const godot_vector2 *p_b) {
	const Vector2 *a = (Vector2 *)p_a;
	const Vector2 *b = (Vector2 *)p_b;
	return *a == *b;
}

godot_bool GDAPI godot_vector2_operator_less(const godot_vector2 *p_a, const godot_vector2 *p_b) {
	const Vector2 *a = (Vector2 *)p_a;
	const Vector2 *b = (Vector2 *)p_b;
	return *a < *b;
}

#ifdef __cplusplus
}
#endif
