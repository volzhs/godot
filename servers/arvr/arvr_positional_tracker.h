/*************************************************************************/
/*  arvr_positional_tracker.h                                            */
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
#ifndef ARVR_POSITIONAL_TRACKER_H
#define ARVR_POSITIONAL_TRACKER_H

#include "os/thread_safe.h"
#include "servers/arvr_server.h"

/**
	@author Bastiaan Olij <mux213@gmail.com>

	The positional tracker object as an object that represents the position and orientation of a tracked object like a controller or headset.
	An AR/VR Interface will registered the trackers it manages with our AR/VR server and update its position and orientation.
	This is where potentially additional AR/VR interfaces may be active as there are AR/VR SDKs that solely deal with positional tracking.

	@TODO:
	- create subclass of spatial node that uses one of our positional trackers to automatically determine its position
*/

class ARVRPositionalTracker : public Object {
	GDCLASS(ARVRPositionalTracker, Object);
	_THREAD_SAFE_CLASS_

private:
	ARVRServer::TrackerType type; // type of tracker
	StringName name; // (unique) name of the tracker
	int tracker_id; // tracker index id that is unique per type
	int joy_id; // if we also have a related joystick entity, the id of the joystick
	bool tracks_orientation; // do we track orientation?
	Basis orientation; // our orientation
	bool tracks_position; // do we track position?
	Vector3 rw_position; // our position "in the real world, so without world_scale applied"

protected:
	static void _bind_methods();

public:
	void set_type(ARVRServer::TrackerType p_type);
	ARVRServer::TrackerType get_type() const;
	void set_name(const String p_name);
	StringName get_name() const;
	int get_tracker_id() const;
	void set_joy_id(int p_joy_id);
	int get_joy_id() const;
	bool get_tracks_orientation() const;
	void set_orientation(const Basis &p_orientation);
	Basis get_orientation() const;
	bool get_tracks_position() const;
	void set_position(const Vector3 &p_position); // set position with world_scale applied
	Vector3 get_position() const; // get position with world_scale applied
	void set_rw_position(const Vector3 &p_rw_position);
	Vector3 get_rw_position() const;

	Transform get_transform(bool p_adjust_by_reference_frame) const;

	ARVRPositionalTracker();
	~ARVRPositionalTracker();
};

#endif
