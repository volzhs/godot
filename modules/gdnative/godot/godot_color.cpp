/*************************************************************************/
/*  godot_color.cpp                                                      */
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
#include "godot_color.h"
#include "core/variant.h"

#include "core/color.h"

#ifdef __cplusplus
extern "C" {
#endif

void _color_api_anchor() {}

void GDAPI godot_color_new_rgba(godot_color *r_dest, const godot_real p_r, const godot_real p_g, const godot_real p_b, const godot_real p_a) {

	Color *dest = (Color *)r_dest;
	*dest = Color(p_r, p_g, p_b, p_a);
}

void GDAPI godot_color_new_rgb(godot_color *r_dest, const godot_real p_r, const godot_real p_g, const godot_real p_b) {

	Color *dest = (Color *)r_dest;
	*dest = Color(p_r, p_g, p_b);
}

godot_string GDAPI godot_color_as_string(const godot_color *p_self) {
	godot_string ret;
	const Color *self = (const Color *)p_self;
	memnew_placement(&ret, String(*self));
	return ret;
}

godot_int GDAPI godot_color_to_32(const godot_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_32();
}

godot_int GDAPI godot_color_to_ARGB32(const godot_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->to_ARGB32();
}

godot_real GDAPI godot_color_gray(const godot_color *p_self) {
	const Color *self = (const Color *)p_self;
	return self->gray();
}

godot_color GDAPI godot_color_inverted(const godot_color *p_self) {
	godot_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->inverted();
	return dest;
}

godot_color GDAPI godot_color_contrasted(const godot_color *p_self) {
	godot_color dest;
	const Color *self = (const Color *)p_self;
	*((Color *)&dest) = self->contrasted();
	return dest;
}

godot_color GDAPI godot_color_linear_interpolate(const godot_color *p_self, const godot_color *p_b, const godot_real p_t) {
	godot_color dest;
	const Color *self = (const Color *)p_self;
	const Color *b = (const Color *)p_b;
	*((Color *)&dest) = self->linear_interpolate(*b, p_t);
	return dest;
}

godot_color GDAPI godot_color_blend(const godot_color *p_self, const godot_color *p_over) {
	godot_color dest;
	const Color *self = (const Color *)p_self;
	const Color *over = (const Color *)p_over;
	*((Color *)&dest) = self->blend(*over);
	return dest;
}

godot_string GDAPI godot_color_to_html(const godot_color *p_self, const godot_bool p_with_alpha) {
	godot_string dest;
	const Color *self = (const Color *)p_self;

	*((String *)&dest) = self->to_html(p_with_alpha);
	return dest;
}

godot_bool GDAPI godot_color_operator_equal(const godot_color *p_self, const godot_color *p_b) {
	const Color *self = (const Color *)p_self;
	const Color *b = (const Color *)p_b;
	return *self == *b;
}

godot_bool GDAPI godot_color_operator_less(const godot_color *p_self, const godot_color *p_b) {
	const Color *self = (const Color *)p_self;
	const Color *b = (const Color *)p_b;
	return *self < *b;
}

#ifdef __cplusplus
}
#endif
