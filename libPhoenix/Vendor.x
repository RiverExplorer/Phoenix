
#ifdef RPC_HDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-02 23:52:28 doug>
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
	uint8_t	Data<>;
};
