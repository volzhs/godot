/*************************************************************************/
/*  file_dialog.cpp                                                      */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
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
#include "file_dialog.h"
#include "os/keyboard.h"
#include "print_string.h"
#include "scene/gui/label.h"

FileDialog::GetIconFunc FileDialog::get_icon_func = NULL;
FileDialog::GetIconFunc FileDialog::get_large_icon_func = NULL;

FileDialog::RegisterFunc FileDialog::register_func = NULL;
FileDialog::RegisterFunc FileDialog::unregister_func = NULL;

VBoxContainer *FileDialog::get_vbox() {
	return vbox;
}

void FileDialog::_notification(int p_what) {

	if (p_what == NOTIFICATION_ENTER_TREE) {

		refresh->set_icon(get_icon("reload"));
	}

	if (p_what == NOTIFICATION_DRAW) {

		//RID ci = get_canvas_item();
		//get_stylebox("panel","PopupMenu")->draw(ci,Rect2(Point2(),get_size()));
	}

	if (p_what == NOTIFICATION_POPUP_HIDE) {

		set_process_unhandled_input(false);
	}
}

void FileDialog::_unhandled_input(const Ref<InputEvent> &p_event) {

	Ref<InputEventKey> k = p_event;
	if (k.is_valid() && is_window_modal_on_top()) {

		if (k->is_pressed()) {

			bool handled = true;

			switch (k->get_scancode()) {

				case KEY_H: {

					if (k->get_command()) {
						set_show_hidden_files(!show_hidden_files);
					} else {
						handled = false;
					}

				} break;
				case KEY_F5: {

					invalidate();
				} break;
				case KEY_BACKSPACE: {

					_dir_entered("..");
				} break;
				default: { handled = false; }
			}

			if (handled)
				accept_event();
		}
	}
}

void FileDialog::set_enable_multiple_selection(bool p_enable) {

	tree->set_select_mode(p_enable ? Tree::SELECT_MULTI : Tree::SELECT_SINGLE);
};

Vector<String> FileDialog::get_selected_files() const {

	Vector<String> list;

	TreeItem *item = tree->get_root();
	while ((item = tree->get_next_selected(item))) {

		list.push_back(dir_access->get_current_dir().plus_file(item->get_text(0)));
	};

	return list;
};

void FileDialog::update_dir() {

	dir->set_text(dir_access->get_current_dir());
	if (drives->is_visible()) {
		drives->select(dir_access->get_current_drive());
	}
}

void FileDialog::_dir_entered(String p_dir) {

	dir_access->change_dir(p_dir);
	file->set_text("");
	invalidate();
	update_dir();
}

void FileDialog::_file_entered(const String &p_file) {

	_action_pressed();
}

void FileDialog::_save_confirm_pressed() {
	String f = dir_access->get_current_dir().plus_file(file->get_text());
	emit_signal("file_selected", f);
	hide();
}

void FileDialog::_post_popup() {

	ConfirmationDialog::_post_popup();
	if (invalidated) {
		update_file_list();
		invalidated = false;
	}
	if (mode == MODE_SAVE_FILE)
		file->grab_focus();
	else
		tree->grab_focus();

	set_process_unhandled_input(true);
}

void FileDialog::_action_pressed() {

	if (mode == MODE_OPEN_FILES) {

		TreeItem *ti = tree->get_next_selected(NULL);
		String fbase = dir_access->get_current_dir();

		PoolVector<String> files;
		while (ti) {

			files.push_back(fbase.plus_file(ti->get_text(0)));
			ti = tree->get_next_selected(ti);
		}

		if (files.size()) {
			emit_signal("files_selected", files);
			hide();
		}

		return;
	}

	String f = dir_access->get_current_dir().plus_file(file->get_text());

	if ((mode == MODE_OPEN_ANY || mode == MODE_OPEN_FILE) && dir_access->file_exists(f)) {
		emit_signal("file_selected", f);
		hide();
	} else if (mode == MODE_OPEN_ANY || mode == MODE_OPEN_DIR) {

		String path = dir_access->get_current_dir();

		path = path.replace("\\", "/");
		TreeItem *item = tree->get_selected();
		if (item) {
			Dictionary d = item->get_metadata(0);
			if (d["dir"] && d["name"] != "..") {
				path = path.plus_file(d["name"]);
			}
		}

		emit_signal("dir_selected", path);
		hide();
	}

	if (mode == MODE_SAVE_FILE) {

		bool valid = false;

		if (filter->get_selected() == filter->get_item_count() - 1) {
			valid = true; //match none
		} else if (filters.size() > 1 && filter->get_selected() == 0) {
			// match all filters
			for (int i = 0; i < filters.size(); i++) {

				String flt = filters[i].get_slice(";", 0);
				for (int j = 0; j < flt.get_slice_count(","); j++) {

					String str = flt.get_slice(",", j).strip_edges();
					if (f.match(str)) {
						valid = true;
						break;
					}
				}
				if (valid)
					break;
			}
		} else {
			int idx = filter->get_selected();
			if (filters.size() > 1)
				idx--;
			if (idx >= 0 && idx < filters.size()) {

				String flt = filters[idx].get_slice(";", 0);
				int filterSliceCount = flt.get_slice_count(",");
				for (int j = 0; j < filterSliceCount; j++) {

					String str = (flt.get_slice(",", j).strip_edges());
					if (f.match(str)) {
						valid = true;
						break;
					}
				}

				if (!valid && filterSliceCount > 0) {
					String str = (flt.get_slice(",", 0).strip_edges());
					f += str.substr(1, str.length() - 1);
					file->set_text(f.get_file());
					valid = true;
				}
			} else {
				valid = true;
			}
		}

		if (!valid) {

			exterr->popup_centered_minsize(Size2(250, 80));
			return;
		}

		if (dir_access->file_exists(f)) {
			confirm_save->set_text(RTR("File Exists, Overwrite?"));
			confirm_save->popup_centered(Size2(200, 80));
		} else {

			emit_signal("file_selected", f);
			hide();
		}
	}
}

void FileDialog::_cancel_pressed() {

	file->set_text("");
	invalidate();
	hide();
}

bool FileDialog::_is_open_should_be_disabled() {

	if (mode == MODE_OPEN_ANY || mode == MODE_SAVE_FILE)
		return false;

	TreeItem *ti = tree->get_selected();
	// We have something that we can't select?
	if (!ti)
		return true;

	Dictionary d = ti->get_metadata(0);

	// Opening a file, but selected a folder? Forbidden.
	if (((mode == MODE_OPEN_FILE || mode == MODE_OPEN_FILES) && d["dir"]) || // Flipped case, also forbidden.
			(mode == MODE_OPEN_DIR && !d["dir"]))
		return true;

	return false;
}

void FileDialog::_tree_selected() {

	TreeItem *ti = tree->get_selected();
	if (!ti)
		return;
	Dictionary d = ti->get_metadata(0);

	if (!d["dir"]) {

		file->set_text(d["name"]);
	}

	get_ok()->set_disabled(_is_open_should_be_disabled());
}

void FileDialog::_tree_dc_selected() {

	TreeItem *ti = tree->get_selected();
	if (!ti)
		return;

	Dictionary d = ti->get_metadata(0);

	if (d["dir"]) {

		dir_access->change_dir(d["name"]);
		if (mode == MODE_OPEN_FILE || mode == MODE_OPEN_FILES || mode == MODE_OPEN_DIR || mode == MODE_OPEN_ANY)
			file->set_text("");
		call_deferred("_update_file_list");
		call_deferred("_update_dir");
	} else {

		_action_pressed();
	}
}

void FileDialog::update_file_list() {

	tree->clear();
	dir_access->list_dir_begin();

	TreeItem *root = tree->create_item();
	Ref<Texture> folder = get_icon("folder");
	List<String> files;
	List<String> dirs;

	bool isdir;
	bool ishidden;
	bool show_hidden = show_hidden_files;
	String item;

	while ((item = dir_access->get_next(&isdir)) != "") {

		if (item == ".")
			continue;

		ishidden = dir_access->current_is_hidden();

		if (show_hidden || !ishidden) {
			if (!isdir)
				files.push_back(item);
			else
				dirs.push_back(item);
		}
	}

	if (dirs.find("..") == NULL) {
		//may happen if lacking permissions
		dirs.push_back("..");
	}

	dirs.sort_custom<NaturalNoCaseComparator>();
	files.sort_custom<NaturalNoCaseComparator>();

	while (!dirs.empty()) {
		String &dir_name = dirs.front()->get();
		TreeItem *ti = tree->create_item(root);
		ti->set_text(0, dir_name);
		ti->set_icon(0, folder);

		Dictionary d;
		d["name"] = dir_name;
		d["dir"] = true;

		ti->set_metadata(0, d);

		dirs.pop_front();
	}

	dirs.clear();

	List<String> patterns;
	// build filter
	if (filter->get_selected() == filter->get_item_count() - 1) {

		// match all
	} else if (filters.size() > 1 && filter->get_selected() == 0) {
		// match all filters
		for (int i = 0; i < filters.size(); i++) {

			String f = filters[i].get_slice(";", 0);
			for (int j = 0; j < f.get_slice_count(","); j++) {

				patterns.push_back(f.get_slice(",", j).strip_edges());
			}
		}
	} else {
		int idx = filter->get_selected();
		if (filters.size() > 1)
			idx--;

		if (idx >= 0 && idx < filters.size()) {

			String f = filters[idx].get_slice(";", 0);
			for (int j = 0; j < f.get_slice_count(","); j++) {

				patterns.push_back(f.get_slice(",", j).strip_edges());
			}
		}
	}

	String base_dir = dir_access->get_current_dir();

	while (!files.empty()) {

		bool match = patterns.empty();
		String match_str;

		for (List<String>::Element *E = patterns.front(); E; E = E->next()) {

			if (files.front()->get().matchn(E->get())) {
				match_str = E->get();
				match = true;
				break;
			}
		}

		if (match) {
			TreeItem *ti = tree->create_item(root);
			ti->set_text(0, files.front()->get());

			if (get_icon_func) {

				Ref<Texture> icon = get_icon_func(base_dir.plus_file(files.front()->get()));
				ti->set_icon(0, icon);
			}

			if (mode == MODE_OPEN_DIR) {
				ti->set_custom_color(0, get_color("files_disabled"));
				ti->set_selectable(0, false);
			}
			Dictionary d;
			d["name"] = files.front()->get();
			d["dir"] = false;
			ti->set_metadata(0, d);

			if (file->get_text() == files.front()->get() || match_str == files.front()->get())
				ti->select(0);
		}

		files.pop_front();
	}

	if (tree->get_root() && tree->get_root()->get_children() && tree->get_selected() == NULL)
		tree->get_root()->get_children()->select(0);

	files.clear();
}

void FileDialog::_filter_selected(int) {

	update_file_list();
}

void FileDialog::update_filters() {

	filter->clear();

	if (filters.size() > 1) {
		String all_filters;

		const int max_filters = 5;

		for (int i = 0; i < MIN(max_filters, filters.size()); i++) {
			String flt = filters[i].get_slice(";", 0);
			if (i > 0)
				all_filters += ",";
			all_filters += flt;
		}

		if (max_filters < filters.size())
			all_filters += ", ...";

		filter->add_item(RTR("All Recognized") + " ( " + all_filters + " )");
	}
	for (int i = 0; i < filters.size(); i++) {

		String flt = filters[i].get_slice(";", 0).strip_edges();
		String desc = filters[i].get_slice(";", 1).strip_edges();
		if (desc.length())
			filter->add_item(String(tr(desc)) + " ( " + flt + " )");
		else
			filter->add_item("( " + flt + " )");
	}

	filter->add_item(RTR("All Files (*)"));
}

void FileDialog::clear_filters() {

	filters.clear();
	update_filters();
	invalidate();
}
void FileDialog::add_filter(const String &p_filter) {

	filters.push_back(p_filter);
	update_filters();
	invalidate();
}

void FileDialog::set_filters(const Vector<String> &p_filters) {
	filters = p_filters;
	update_filters();
	invalidate();
}

Vector<String> FileDialog::get_filters() const {
	return filters;
}

String FileDialog::get_current_dir() const {

	return dir->get_text();
}
String FileDialog::get_current_file() const {

	return file->get_text();
}
String FileDialog::get_current_path() const {

	return dir->get_text().plus_file(file->get_text());
}
void FileDialog::set_current_dir(const String &p_dir) {

	dir_access->change_dir(p_dir);
	update_dir();
	invalidate();
}
void FileDialog::set_current_file(const String &p_file) {

	file->set_text(p_file);
	update_dir();
	invalidate();
	int lp = p_file.find_last(".");
	if (lp != -1) {
		file->select(0, lp);
		file->grab_focus();
	}
}
void FileDialog::set_current_path(const String &p_path) {

	if (!p_path.size())
		return;
	int pos = MAX(p_path.find_last("/"), p_path.find_last("\\"));
	if (pos == -1) {

		set_current_file(p_path);
	} else {

		String dir = p_path.substr(0, pos);
		String file = p_path.substr(pos + 1, p_path.length());
		set_current_dir(dir);
		set_current_file(file);
	}
}

void FileDialog::set_mode(Mode p_mode) {

	mode = p_mode;
	switch (mode) {

		case MODE_OPEN_FILE:
			get_ok()->set_text(RTR("Open"));
			set_title(RTR("Open a File"));
			makedir->hide();
			break;
		case MODE_OPEN_FILES:
			get_ok()->set_text(RTR("Open"));
			set_title(RTR("Open File(s)"));
			makedir->hide();
			break;
		case MODE_OPEN_DIR:
			get_ok()->set_text(RTR("Select Current Folder"));
			set_title(RTR("Open a Directory"));
			makedir->show();
			break;
		case MODE_OPEN_ANY:
			get_ok()->set_text(RTR("Open"));
			set_title(RTR("Open a File or Directory"));
			makedir->show();
			break;
		case MODE_SAVE_FILE:
			get_ok()->set_text(RTR("Save"));
			set_title(RTR("Save a File"));
			makedir->show();
			break;
	}

	if (mode == MODE_OPEN_FILES) {
		tree->set_select_mode(Tree::SELECT_MULTI);
	} else {
		tree->set_select_mode(Tree::SELECT_SINGLE);
	}
}

FileDialog::Mode FileDialog::get_mode() const {

	return mode;
}

void FileDialog::set_access(Access p_access) {

	ERR_FAIL_INDEX(p_access, 3);
	if (access == p_access)
		return;
	memdelete(dir_access);
	switch (p_access) {
		case ACCESS_FILESYSTEM: {

			dir_access = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
		} break;
		case ACCESS_RESOURCES: {

			dir_access = DirAccess::create(DirAccess::ACCESS_RESOURCES);
		} break;
		case ACCESS_USERDATA: {

			dir_access = DirAccess::create(DirAccess::ACCESS_USERDATA);
		} break;
	}
	access = p_access;
	_update_drives();
	invalidate();
	update_filters();
	update_dir();
}

void FileDialog::invalidate() {

	if (is_visible_in_tree()) {
		update_file_list();
		invalidated = false;
	} else {
		invalidated = true;
	}
}

FileDialog::Access FileDialog::get_access() const {

	return access;
}

void FileDialog::_make_dir_confirm() {

	Error err = dir_access->make_dir(makedirname->get_text());
	if (err == OK) {
		dir_access->change_dir(makedirname->get_text());
		invalidate();
		update_filters();
		update_dir();
	} else {
		mkdirerr->popup_centered_minsize(Size2(250, 50));
	}
	makedirname->set_text(""); // reset label
}

void FileDialog::_make_dir() {

	makedialog->popup_centered_minsize(Size2(250, 80));
	makedirname->grab_focus();
}

void FileDialog::_select_drive(int p_idx) {

	String d = drives->get_item_text(p_idx);
	dir_access->change_dir(d);
	file->set_text("");
	invalidate();
	update_dir();
}

void FileDialog::_update_drives() {

	int dc = dir_access->get_drive_count();
	if (dc == 0 || access != ACCESS_FILESYSTEM) {
		drives->hide();
	} else {
		drives->clear();
		drives->show();

		for (int i = 0; i < dir_access->get_drive_count(); i++) {
			drives->add_item(dir_access->get_drive(i));
		}

		drives->select(dir_access->get_current_drive());
	}
}

bool FileDialog::default_show_hidden_files = false;

void FileDialog::_bind_methods() {

	ClassDB::bind_method(D_METHOD("_unhandled_input"), &FileDialog::_unhandled_input);

	ClassDB::bind_method(D_METHOD("_tree_selected"), &FileDialog::_tree_selected);
	ClassDB::bind_method(D_METHOD("_tree_db_selected"), &FileDialog::_tree_dc_selected);
	ClassDB::bind_method(D_METHOD("_dir_entered"), &FileDialog::_dir_entered);
	ClassDB::bind_method(D_METHOD("_file_entered"), &FileDialog::_file_entered);
	ClassDB::bind_method(D_METHOD("_action_pressed"), &FileDialog::_action_pressed);
	ClassDB::bind_method(D_METHOD("_cancel_pressed"), &FileDialog::_cancel_pressed);
	ClassDB::bind_method(D_METHOD("_filter_selected"), &FileDialog::_filter_selected);
	ClassDB::bind_method(D_METHOD("_save_confirm_pressed"), &FileDialog::_save_confirm_pressed);

	ClassDB::bind_method(D_METHOD("clear_filters"), &FileDialog::clear_filters);
	ClassDB::bind_method(D_METHOD("add_filter", "filter"), &FileDialog::add_filter);
	ClassDB::bind_method(D_METHOD("set_filters", "filters"), &FileDialog::set_filters);
	ClassDB::bind_method(D_METHOD("get_filters"), &FileDialog::get_filters);
	ClassDB::bind_method(D_METHOD("get_current_dir"), &FileDialog::get_current_dir);
	ClassDB::bind_method(D_METHOD("get_current_file"), &FileDialog::get_current_file);
	ClassDB::bind_method(D_METHOD("get_current_path"), &FileDialog::get_current_path);
	ClassDB::bind_method(D_METHOD("set_current_dir", "dir"), &FileDialog::set_current_dir);
	ClassDB::bind_method(D_METHOD("set_current_file", "file"), &FileDialog::set_current_file);
	ClassDB::bind_method(D_METHOD("set_current_path", "path"), &FileDialog::set_current_path);
	ClassDB::bind_method(D_METHOD("set_mode", "mode"), &FileDialog::set_mode);
	ClassDB::bind_method(D_METHOD("get_mode"), &FileDialog::get_mode);
	ClassDB::bind_method(D_METHOD("get_vbox"), &FileDialog::get_vbox);
	ClassDB::bind_method(D_METHOD("set_access", "access"), &FileDialog::set_access);
	ClassDB::bind_method(D_METHOD("get_access"), &FileDialog::get_access);
	ClassDB::bind_method(D_METHOD("set_show_hidden_files", "show"), &FileDialog::set_show_hidden_files);
	ClassDB::bind_method(D_METHOD("is_showing_hidden_files"), &FileDialog::is_showing_hidden_files);
	ClassDB::bind_method(D_METHOD("_select_drive"), &FileDialog::_select_drive);
	ClassDB::bind_method(D_METHOD("_make_dir"), &FileDialog::_make_dir);
	ClassDB::bind_method(D_METHOD("_make_dir_confirm"), &FileDialog::_make_dir_confirm);
	ClassDB::bind_method(D_METHOD("_update_file_list"), &FileDialog::update_file_list);
	ClassDB::bind_method(D_METHOD("_update_dir"), &FileDialog::update_dir);

	ClassDB::bind_method(D_METHOD("invalidate"), &FileDialog::invalidate);

	ADD_SIGNAL(MethodInfo("file_selected", PropertyInfo(Variant::STRING, "path")));
	ADD_SIGNAL(MethodInfo("files_selected", PropertyInfo(Variant::POOL_STRING_ARRAY, "paths")));
	ADD_SIGNAL(MethodInfo("dir_selected", PropertyInfo(Variant::STRING, "dir")));

	BIND_ENUM_CONSTANT(MODE_OPEN_FILE);
	BIND_ENUM_CONSTANT(MODE_OPEN_FILES);
	BIND_ENUM_CONSTANT(MODE_OPEN_DIR);
	BIND_ENUM_CONSTANT(MODE_OPEN_ANY);
	BIND_ENUM_CONSTANT(MODE_SAVE_FILE);

	BIND_ENUM_CONSTANT(ACCESS_RESOURCES);
	BIND_ENUM_CONSTANT(ACCESS_USERDATA);
	BIND_ENUM_CONSTANT(ACCESS_FILESYSTEM);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode", PROPERTY_HINT_ENUM, "Open one,Open many,Open folder,Open any,Save"), "set_mode", "get_mode");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "access", PROPERTY_HINT_ENUM, "Resources,User data,File system"), "set_access", "get_access");
	ADD_PROPERTY(PropertyInfo(Variant::POOL_STRING_ARRAY, "filters"), "set_filters", "get_filters");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "show_hidden_files"), "set_show_hidden_files", "is_showing_hidden_files");
}

void FileDialog::set_show_hidden_files(bool p_show) {
	show_hidden_files = p_show;
	invalidate();
}

bool FileDialog::is_showing_hidden_files() const {
	return show_hidden_files;
}

void FileDialog::set_default_show_hidden_files(bool p_show) {
	default_show_hidden_files = p_show;
}

FileDialog::FileDialog() {

	show_hidden_files = default_show_hidden_files;

	VBoxContainer *vbc = memnew(VBoxContainer);
	add_child(vbc);

	mode = MODE_SAVE_FILE;
	set_title(RTR("Save a File"));

	HBoxContainer *hbc = memnew(HBoxContainer);
	hbc->add_child(memnew(Label(RTR("Path:"))));
	dir = memnew(LineEdit);
	hbc->add_child(dir);
	dir->set_h_size_flags(SIZE_EXPAND_FILL);

	refresh = memnew(ToolButton);
	refresh->connect("pressed", this, "_update_file_list");
	hbc->add_child(refresh);

	drives = memnew(OptionButton);
	hbc->add_child(drives);
	drives->connect("item_selected", this, "_select_drive");

	makedir = memnew(Button);
	makedir->set_text(RTR("Create Folder"));
	makedir->connect("pressed", this, "_make_dir");
	hbc->add_child(makedir);
	vbc->add_child(hbc);

	tree = memnew(Tree);
	tree->set_hide_root(true);
	vbc->add_margin_child(RTR("Directories & Files:"), tree, true);

	hbc = memnew(HBoxContainer);
	hbc->add_child(memnew(Label(RTR("File:"))));
	file = memnew(LineEdit);
	file->set_stretch_ratio(4);
	file->set_h_size_flags(SIZE_EXPAND_FILL);
	hbc->add_child(file);
	filter = memnew(OptionButton);
	filter->set_stretch_ratio(3);
	filter->set_h_size_flags(SIZE_EXPAND_FILL);
	filter->set_clip_text(true); //too many extensions overflow it
	hbc->add_child(filter);
	vbc->add_child(hbc);

	dir_access = DirAccess::create(DirAccess::ACCESS_RESOURCES);
	access = ACCESS_RESOURCES;
	_update_drives();

	connect("confirmed", this, "_action_pressed");
	//cancel->connect("pressed", this,"_cancel_pressed");
	tree->connect("cell_selected", this, "_tree_selected", varray(), CONNECT_DEFERRED);
	tree->connect("item_activated", this, "_tree_db_selected", varray());
	dir->connect("text_entered", this, "_dir_entered");
	file->connect("text_entered", this, "_file_entered");
	filter->connect("item_selected", this, "_filter_selected");

	confirm_save = memnew(ConfirmationDialog);
	confirm_save->set_as_toplevel(true);
	add_child(confirm_save);

	confirm_save->connect("confirmed", this, "_save_confirm_pressed");

	makedialog = memnew(ConfirmationDialog);
	makedialog->set_title(RTR("Create Folder"));
	VBoxContainer *makevb = memnew(VBoxContainer);
	makedialog->add_child(makevb);

	makedirname = memnew(LineEdit);
	makevb->add_margin_child(RTR("Name:"), makedirname);
	add_child(makedialog);
	makedialog->register_text_enter(makedirname);
	makedialog->connect("confirmed", this, "_make_dir_confirm");
	mkdirerr = memnew(AcceptDialog);
	mkdirerr->set_text(RTR("Could not create folder."));
	add_child(mkdirerr);

	exterr = memnew(AcceptDialog);
	exterr->set_text(RTR("Must use a valid extension."));
	add_child(exterr);

	//update_file_list();
	update_filters();
	update_dir();

	set_hide_on_ok(false);
	vbox = vbc;

	invalidated = true;
	if (register_func)
		register_func(this);
}

FileDialog::~FileDialog() {

	if (unregister_func)
		unregister_func(this);
	memdelete(dir_access);
}

void LineEditFileChooser::_bind_methods() {

	ClassDB::bind_method(D_METHOD("_browse"), &LineEditFileChooser::_browse);
	ClassDB::bind_method(D_METHOD("_chosen"), &LineEditFileChooser::_chosen);
	ClassDB::bind_method(D_METHOD("get_button"), &LineEditFileChooser::get_button);
	ClassDB::bind_method(D_METHOD("get_line_edit"), &LineEditFileChooser::get_line_edit);
	ClassDB::bind_method(D_METHOD("get_file_dialog"), &LineEditFileChooser::get_file_dialog);
}

void LineEditFileChooser::_chosen(const String &p_text) {

	line_edit->set_text(p_text);
	line_edit->emit_signal("text_entered", p_text);
}

void LineEditFileChooser::_browse() {

	dialog->popup_centered_ratio();
}

LineEditFileChooser::LineEditFileChooser() {

	line_edit = memnew(LineEdit);
	add_child(line_edit);
	line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
	button = memnew(Button);
	button->set_text(" .. ");
	add_child(button);
	button->connect("pressed", this, "_browse");
	dialog = memnew(FileDialog);
	add_child(dialog);
	dialog->connect("file_selected", this, "_chosen");
	dialog->connect("dir_selected", this, "_chosen");
	dialog->connect("files_selected", this, "_chosen");
}
