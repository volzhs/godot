#include "networked_multiplayer_peer.h"


void NetworkedMultiplayerPeer::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("set_transfer_mode","mode"), &NetworkedMultiplayerPeer::set_transfer_mode );
	ObjectTypeDB::bind_method(_MD("set_target_peer","id"), &NetworkedMultiplayerPeer::set_target_peer );
	ObjectTypeDB::bind_method(_MD("set_channel","id"), &NetworkedMultiplayerPeer::set_channel );

	ObjectTypeDB::bind_method(_MD("get_packet_peer"), &NetworkedMultiplayerPeer::get_packet_peer );
	ObjectTypeDB::bind_method(_MD("get_packet_channel"), &NetworkedMultiplayerPeer::get_packet_channel );

	ObjectTypeDB::bind_method(_MD("poll"), &NetworkedMultiplayerPeer::poll );

	ObjectTypeDB::bind_method(_MD("get_connection_status"), &NetworkedMultiplayerPeer::get_connection_status );

	BIND_CONSTANT( TRANSFER_MODE_UNRELIABLE );
	BIND_CONSTANT( TRANSFER_MODE_RELIABLE );
	BIND_CONSTANT( TRANSFER_MODE_ORDERED );

	BIND_CONSTANT( CONNECTION_DISCONNECTED );
	BIND_CONSTANT( CONNECTION_CONNECTING );
	BIND_CONSTANT( CONNECTION_CONNECTED );

	ADD_SIGNAL( MethodInfo("peer_connected",PropertyInfo(Variant::STRING,"id")));
	ADD_SIGNAL( MethodInfo("peer_disconnected",PropertyInfo(Variant::STRING,"id")));
}

NetworkedMultiplayerPeer::NetworkedMultiplayerPeer() {


}
