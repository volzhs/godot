/*************************************************************************/
/*  style_box.cpp                                                        */
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
#include "style_box.h"

bool StyleBox::test_mask(const Point2 &p_point, const Rect2 &p_rect) const {

	return true;
}

void StyleBox::set_default_margin(Margin p_margin, float p_value) {

	margin[p_margin] = p_value;
	emit_changed();
}
float StyleBox::get_default_margin(Margin p_margin) const {

	return margin[p_margin];
}

float StyleBox::get_margin(Margin p_margin) const {

	if (margin[p_margin] < 0)
		return get_style_margin(p_margin);
	else
		return margin[p_margin];
}

Size2 StyleBox::get_minimum_size() const {

	return Size2(get_margin(MARGIN_LEFT) + get_margin(MARGIN_RIGHT), get_margin(MARGIN_TOP) + get_margin(MARGIN_BOTTOM));
}

Point2 StyleBox::get_offset() const {

	return Point2(get_margin(MARGIN_LEFT), get_margin(MARGIN_TOP));
}

Size2 StyleBox::get_center_size() const {

	return Size2();
}

void StyleBox::_bind_methods() {

	ClassDB::bind_method(D_METHOD("test_mask", "point", "rect"), &StyleBox::test_mask);

	ClassDB::bind_method(D_METHOD("set_default_margin", "margin", "offset"), &StyleBox::set_default_margin);
	ClassDB::bind_method(D_METHOD("get_default_margin", "margin"), &StyleBox::get_default_margin);

	//ClassDB::bind_method(D_METHOD("set_default_margin"),&StyleBox::set_default_margin);
	//ClassDB::bind_method(D_METHOD("get_default_margin"),&StyleBox::get_default_margin);

	ClassDB::bind_method(D_METHOD("get_margin", "margin"), &StyleBox::get_margin);
	ClassDB::bind_method(D_METHOD("get_minimum_size"), &StyleBox::get_minimum_size);
	ClassDB::bind_method(D_METHOD("get_center_size"), &StyleBox::get_center_size);
	ClassDB::bind_method(D_METHOD("get_offset"), &StyleBox::get_offset);

	ClassDB::bind_method(D_METHOD("draw", "canvas_item", "rect"), &StyleBox::draw);

	ADD_GROUP("Content Margin", "content_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_left", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_right", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_top", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "content_margin_bottom", PROPERTY_HINT_RANGE, "-1,2048,1"), "set_default_margin", "get_default_margin", MARGIN_BOTTOM);
}

StyleBox::StyleBox() {

	for (int i = 0; i < 4; i++) {

		margin[i] = -1;
	}
}

void StyleBoxTexture::set_texture(RES p_texture) {

	if (texture == p_texture)
		return;
	texture = p_texture;
	region_rect = Rect2(Point2(), texture->get_size());
	emit_signal("texture_changed");
	emit_changed();
	_change_notify("texture");
}

RES StyleBoxTexture::get_texture() const {

	return texture;
}

void StyleBoxTexture::set_normal_map(RES p_normal_map) {

	if (normal_map == p_normal_map)
		return;
	normal_map = p_normal_map;
	emit_changed();
}

RES StyleBoxTexture::get_normal_map() const {

	return normal_map;
}

void StyleBoxTexture::set_margin_size(Margin p_margin, float p_size) {

	margin[p_margin] = p_size;
	emit_changed();
}
float StyleBoxTexture::get_margin_size(Margin p_margin) const {

	return margin[p_margin];
}

float StyleBoxTexture::get_style_margin(Margin p_margin) const {

	return margin[p_margin];
}

void StyleBoxTexture::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	if (texture.is_null())
		return;

	Rect2 rect = p_rect;
	Rect2 src_rect = region_rect;

	texture->get_rect_region(rect, src_rect, rect, src_rect);

	rect.position.x -= expand_margin[MARGIN_LEFT];
	rect.position.y -= expand_margin[MARGIN_TOP];
	rect.size.x += expand_margin[MARGIN_LEFT] + expand_margin[MARGIN_RIGHT];
	rect.size.y += expand_margin[MARGIN_TOP] + expand_margin[MARGIN_BOTTOM];

	RID normal_rid;
	if (normal_map.is_valid())
		normal_rid = normal_map->get_rid();

	VisualServer::get_singleton()->canvas_item_add_nine_patch(p_canvas_item, rect, src_rect, texture->get_rid(), Vector2(margin[MARGIN_LEFT], margin[MARGIN_TOP]), Vector2(margin[MARGIN_RIGHT], margin[MARGIN_BOTTOM]), VS::NINE_PATCH_STRETCH, VS::NINE_PATCH_STRETCH, draw_center, modulate, normal_rid);
}

void StyleBoxTexture::set_draw_center(bool p_draw) {

	draw_center = p_draw;
	emit_changed();
}

bool StyleBoxTexture::get_draw_center() const {

	return draw_center;
}

Size2 StyleBoxTexture::get_center_size() const {

	if (texture.is_null())
		return Size2();

	return texture->get_size() - get_minimum_size();
}

void StyleBoxTexture::set_expand_margin_size(Margin p_expand_margin, float p_size) {

	ERR_FAIL_INDEX(p_expand_margin, 4);
	expand_margin[p_expand_margin] = p_size;
	emit_changed();
}

float StyleBoxTexture::get_expand_margin_size(Margin p_expand_margin) const {

	ERR_FAIL_INDEX_V(p_expand_margin, 4, 0);
	return expand_margin[p_expand_margin];
}

void StyleBoxTexture::set_region_rect(const Rect2 &p_region_rect) {

	if (region_rect == p_region_rect)
		return;

	region_rect = p_region_rect;
	emit_changed();
}

Rect2 StyleBoxTexture::get_region_rect() const {

	return region_rect;
}

void StyleBoxTexture::set_modulate(const Color &p_modulate) {
	if (modulate == p_modulate)
		return;
	modulate = p_modulate;
	emit_changed();
}

Color StyleBoxTexture::get_modulate() const {

	return modulate;
}

void StyleBoxTexture::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &StyleBoxTexture::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &StyleBoxTexture::get_texture);

	ClassDB::bind_method(D_METHOD("set_normal_map", "normal_map"), &StyleBoxTexture::set_normal_map);
	ClassDB::bind_method(D_METHOD("get_normal_map"), &StyleBoxTexture::get_normal_map);

	ClassDB::bind_method(D_METHOD("set_margin_size", "margin", "size"), &StyleBoxTexture::set_margin_size);
	ClassDB::bind_method(D_METHOD("get_margin_size", "margin"), &StyleBoxTexture::get_margin_size);

	ClassDB::bind_method(D_METHOD("set_expand_margin_size", "margin", "size"), &StyleBoxTexture::set_expand_margin_size);
	ClassDB::bind_method(D_METHOD("get_expand_margin_size", "margin"), &StyleBoxTexture::get_expand_margin_size);

	ClassDB::bind_method(D_METHOD("set_region_rect", "region"), &StyleBoxTexture::set_region_rect);
	ClassDB::bind_method(D_METHOD("get_region_rect"), &StyleBoxTexture::get_region_rect);

	ClassDB::bind_method(D_METHOD("set_draw_center", "enable"), &StyleBoxTexture::set_draw_center);
	ClassDB::bind_method(D_METHOD("get_draw_center"), &StyleBoxTexture::get_draw_center);

	ClassDB::bind_method(D_METHOD("set_modulate", "color"), &StyleBoxTexture::set_modulate);
	ClassDB::bind_method(D_METHOD("get_modulate"), &StyleBoxTexture::get_modulate);

	ADD_SIGNAL(MethodInfo("texture_changed"));

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "normal_map", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_normal_map", "get_normal_map");
	ADD_PROPERTYNZ(PropertyInfo(Variant::RECT2, "region_rect"), "set_region_rect", "get_region_rect");
	ADD_GROUP("Margin", "margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_left", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_right", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_top", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "margin_bottom", PROPERTY_HINT_RANGE, "0,2048,1"), "set_margin_size", "get_margin_size", MARGIN_BOTTOM);
	ADD_GROUP("Expand Margin", "expand_margin_");
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_left", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_LEFT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_right", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_RIGHT);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_top", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_TOP);
	ADD_PROPERTYI(PropertyInfo(Variant::REAL, "expand_margin_bottom", PROPERTY_HINT_RANGE, "0,2048,1"), "set_expand_margin_size", "get_expand_margin_size", MARGIN_BOTTOM);
	ADD_GROUP("Modulate", "modulate_");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "modulate_color"), "set_modulate", "get_modulate");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_center"), "set_draw_center", "get_draw_center");
}

StyleBoxTexture::StyleBoxTexture() {

	for (int i = 0; i < 4; i++) {
		margin[i] = 0;
		expand_margin[i] = 0;
	}
	draw_center = true;
	modulate = Color(1, 1, 1, 1);
}
StyleBoxTexture::~StyleBoxTexture() {
}

////////////////

void StyleBoxFlat::set_bg_color(const Color &p_color) {

	bg_color = p_color;
	emit_changed();
}

void StyleBoxFlat::set_light_color(const Color &p_color) {

	light_color = p_color;
	emit_changed();
}
void StyleBoxFlat::set_dark_color(const Color &p_color) {

	dark_color = p_color;
	emit_changed();
}

Color StyleBoxFlat::get_bg_color() const {

	return bg_color;
}
Color StyleBoxFlat::get_light_color() const {

	return light_color;
}
Color StyleBoxFlat::get_dark_color() const {

	return dark_color;
}

void StyleBoxFlat::set_border_size(int p_size) {

	border_size = p_size;
	emit_changed();
}
int StyleBoxFlat::get_border_size() const {

	return border_size;
}

void StyleBoxFlat::_set_additional_border_size(Margin p_margin, int p_size) {
	additional_border_size[p_margin] = p_size;
	emit_changed();
}

int StyleBoxFlat::_get_additional_border_size(Margin p_margin) const {
	return additional_border_size[p_margin];
}

void StyleBoxFlat::set_border_blend(bool p_blend) {

	blend = p_blend;
	emit_changed();
}

bool StyleBoxFlat::get_border_blend() const {

	return blend;
}

void StyleBoxFlat::set_draw_center(bool p_draw) {

	draw_center = p_draw;
	emit_changed();
}
bool StyleBoxFlat::get_draw_center() const {

	return draw_center;
}
Size2 StyleBoxFlat::get_center_size() const {

	return Size2();
}

void StyleBoxFlat::draw(RID p_canvas_item, const Rect2 &p_rect) const {

	VisualServer *vs = VisualServer::get_singleton();
	Rect2i r = p_rect;

	for (int i = 0; i < border_size; i++) {

		Color color_upleft = light_color;
		Color color_downright = dark_color;

		if (blend) {

			color_upleft.r = (border_size - i) * color_upleft.r / border_size + i * bg_color.r / border_size;
			color_upleft.g = (border_size - i) * color_upleft.g / border_size + i * bg_color.g / border_size;
			color_upleft.b = (border_size - i) * color_upleft.b / border_size + i * bg_color.b / border_size;

			color_downright.r = (border_size - i) * color_downright.r / border_size + i * bg_color.r / border_size;
			color_downright.g = (border_size - i) * color_downright.g / border_size + i * bg_color.g / border_size;
			color_downright.b = (border_size - i) * color_downright.b / border_size + i * bg_color.b / border_size;
		}

		vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r.position.x, r.position.y + r.size.y - 1), Size2(r.size.x, 1)), color_downright);
		vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r.position.x + r.size.x - 1, r.position.y), Size2(1, r.size.y)), color_downright);

		vs->canvas_item_add_rect(p_canvas_item, Rect2(r.position, Size2(r.size.x, 1)), color_upleft);
		vs->canvas_item_add_rect(p_canvas_item, Rect2(r.position, Size2(1, r.size.y)), color_upleft);

		r.position.x++;
		r.position.y++;
		r.size.x -= 2;
		r.size.y -= 2;
	}

	if (draw_center)
		vs->canvas_item_add_rect(p_canvas_item, Rect2(r.position, r.size), bg_color);

	Rect2i r_add = p_rect;
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.position.x - additional_border_size[MARGIN_LEFT], r_add.position.y - additional_border_size[MARGIN_TOP]), Size2(r_add.size.width + additional_border_size[MARGIN_LEFT] + additional_border_size[MARGIN_RIGHT], additional_border_size[MARGIN_TOP])), light_color);
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.position.x - additional_border_size[MARGIN_LEFT], r_add.position.y), Size2(additional_border_size[MARGIN_LEFT], r_add.size.height)), light_color);
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.position.x + r_add.size.width, r_add.position.y), Size2(additional_border_size[MARGIN_RIGHT], r_add.size.height)), dark_color);
	vs->canvas_item_add_rect(p_canvas_item, Rect2(Point2i(r_add.position.x - additional_border_size[MARGIN_LEFT], r_add.position.y + r_add.size.height), Size2(r_add.size.width + additional_border_size[MARGIN_LEFT] + additional_border_size[MARGIN_RIGHT], additional_border_size[MARGIN_BOTTOM])), dark_color);
}

float StyleBoxFlat::get_style_margin(Margin p_margin) const {

	return border_size;
}
void StyleBoxFlat::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_bg_color", "color"), &StyleBoxFlat::set_bg_color);
	ClassDB::bind_method(D_METHOD("get_bg_color"), &StyleBoxFlat::get_bg_color);
	ClassDB::bind_method(D_METHOD("set_light_color", "color"), &StyleBoxFlat::set_light_color);
	ClassDB::bind_method(D_METHOD("get_light_color"), &StyleBoxFlat::get_light_color);
	ClassDB::bind_method(D_METHOD("set_dark_color", "color"), &StyleBoxFlat::set_dark_color);
	ClassDB::bind_method(D_METHOD("get_dark_color"), &StyleBoxFlat::get_dark_color);
	ClassDB::bind_method(D_METHOD("set_border_size", "size"), &StyleBoxFlat::set_border_size);
	ClassDB::bind_method(D_METHOD("get_border_size"), &StyleBoxFlat::get_border_size);
	ClassDB::bind_method(D_METHOD("set_border_blend", "blend"), &StyleBoxFlat::set_border_blend);
	ClassDB::bind_method(D_METHOD("get_border_blend"), &StyleBoxFlat::get_border_blend);
	ClassDB::bind_method(D_METHOD("set_draw_center", "size"), &StyleBoxFlat::set_draw_center);
	ClassDB::bind_method(D_METHOD("get_draw_center"), &StyleBoxFlat::get_draw_center);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "bg_color"), "set_bg_color", "get_bg_color");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "light_color"), "set_light_color", "get_light_color");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "dark_color"), "set_dark_color", "get_dark_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "border_size", PROPERTY_HINT_RANGE, "0,4096"), "set_border_size", "get_border_size");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "border_blend"), "set_border_blend", "get_border_blend");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "draw_bg"), "set_draw_center", "get_draw_center");
}

StyleBoxFlat::StyleBoxFlat() {

	bg_color = Color(0.6, 0.6, 0.6);
	light_color = Color(0.8, 0.8, 0.8);
	dark_color = Color(0.8, 0.8, 0.8);
	draw_center = true;
	blend = true;
	border_size = 0;
	additional_border_size[0] = 0;
	additional_border_size[1] = 0;
	additional_border_size[2] = 0;
	additional_border_size[3] = 0;
}
StyleBoxFlat::~StyleBoxFlat() {
}

void StyleBoxLine::set_color(const Color &p_color) {
	color = p_color;
	emit_changed();
}
Color StyleBoxLine::get_color() const {
	return color;
}

void StyleBoxLine::set_thickness(int p_thickness) {
	thickness = p_thickness;
	emit_changed();
}
int StyleBoxLine::get_thickness() const {
	return thickness;
}

void StyleBoxLine::set_vertical(bool p_vertical) {
	vertical = p_vertical;
}
bool StyleBoxLine::is_vertical() const {
	return vertical;
}

void StyleBoxLine::set_grow(float p_grow) {
	grow = p_grow;
	emit_changed();
}
float StyleBoxLine::get_grow() const {
	return grow;
}

void StyleBoxLine::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_color", "color"), &StyleBoxLine::set_color);
	ClassDB::bind_method(D_METHOD("get_color"), &StyleBoxLine::get_color);
	ClassDB::bind_method(D_METHOD("set_thickness", "thickness"), &StyleBoxLine::set_thickness);
	ClassDB::bind_method(D_METHOD("get_thickness"), &StyleBoxLine::get_thickness);
	ClassDB::bind_method(D_METHOD("set_grow", "grow"), &StyleBoxLine::set_grow);
	ClassDB::bind_method(D_METHOD("get_grow"), &StyleBoxLine::get_grow);
	ClassDB::bind_method(D_METHOD("set_vertical", "vertical"), &StyleBoxLine::set_vertical);
	ClassDB::bind_method(D_METHOD("is_vertical"), &StyleBoxLine::is_vertical);

	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "thickness", PROPERTY_HINT_RANGE, "0,10"), "set_thickness", "get_thickness");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "vertical"), "set_vertical", "is_vertical");
}
float StyleBoxLine::get_style_margin(Margin p_margin) const {
	return thickness;
}
Size2 StyleBoxLine::get_center_size() const {
	return Size2();
}

void StyleBoxLine::draw(RID p_canvas_item, const Rect2 &p_rect) const {
	VisualServer *vs = VisualServer::get_singleton();
	Rect2i r = p_rect;

	if (vertical) {
		r.position.y -= grow;
		r.size.y += grow * 2;
		r.size.x = thickness;
	} else {
		r.position.x -= grow;
		r.size.x += grow * 2;
		r.size.y = thickness;
	}

	vs->canvas_item_add_rect(p_canvas_item, r, color);
}

StyleBoxLine::StyleBoxLine() {
	grow = 1.0;
	thickness = 1;
	color = Color(0.0, 0.0, 0.0);
	vertical = false;
}
StyleBoxLine::~StyleBoxLine() {}
