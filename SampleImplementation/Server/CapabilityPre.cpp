/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

/**
 * @file CapabilityPre
 * This code is responsible for defining and when asked, sending
 * the CAPABILITY_PRE command to the client.
 */
 
#include <RiverExplorer/Phoenix/PhoenixServer.hpp>
#include <RiverExplorer/Phoenix/PhoenixEvent.hpp>
#include <RiverExplorer/Phoenix/IPPeer.hpp>
#include <exception>

