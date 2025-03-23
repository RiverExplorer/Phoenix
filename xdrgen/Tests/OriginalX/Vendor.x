
#ifdef RPC_HDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 09:19:49 doug>
% *
% * @file Vendor.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif

#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#endif
%/**
% * @class Vendor
% * A blob of data as seen by implementations that
% * do not have matching VENDOR_ID values.
% *
% * Define any vendor specific objects below Vendor.
% */
#endif
struct Vendor
{
	CMD_e Cmd;	/** Set to a vendor value with the VENDOR_BIT set */
	uint8_t	Data<>;
};
