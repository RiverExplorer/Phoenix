%
%#ifdef BUILDING_LIBPHOENIX
%#include "Index.hpp"
%#include "PhoenixString.hpp"
%#else
%#include <RiverExplorer/Phoenix/Index.hpp>
%#include <RiverExplorer/Phoenix/PhoenixString.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @enum RiverExplorer::Phoenix::IndexOP_e
% * This is the XDR specification for the Interested header
% * operation (IndexOP) described in the specification:
% * <a href="https://htmlpreview.github.io/?https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/draft-royer-phoenix.html#name-indexoperation-indexop" target="_blank">Phoenix Index Operation (IndexOP)</a>
% *
% * -DefineList_t
% * Defnie a new list.
% *
% * -UseList_t
% * Use an existing list.
% */
#endif
enum IndexOP_e
{
	DefineList_t = 0,
	UseList_t    = 1
};

#ifdef RPC_HDR
%/**
% * @typedef RiverExplorer::Phoenix::LID_t
% * This is the XDR specification for the Interested header
% * list ID (LID) described in the specification:
% * <a href="https://htmlpreview.github.io/?https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/draft-royer-phoenix.html#name-listid-lid" target="_blank">Phoenix Index List ID (LID)</a>
% *
% * -LID_t
% * A type definition for a list ID.
% */
#endif
typedef uint8_t LID_t;

#ifdef RPC_HDR
%/**
% * @typedef RiverExplorer::Phoenix::HID_t
% * This is the XDR specification for the Interested header ID (HID)
% * described in the specification:
% * <a href="https://htmlpreview.github.io/?https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/draft-royer-phoenix.html#name-headerid-hid" target="_blank">Phoenix Index Header ID (HID)</a>
% *
% * -LID_t
% * A type definition for a list ID.
% */
#endif
typedef uint8_t HID_t;

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::SingleEntry
% * This is the XDR specification for the Interested header
% * list described in the specification:
% * <a href="https://htmlpreview.github.io/?https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/draft-royer-phoenix.html#Interisting-S0-02" target="_blank">Phoenix Index</a>
% *
% * -HID:
% * The ID to associate with this header.
% *
% * -HeaderName The name of the header.
% */
#endif
class SingleEntry
{
	HID_t		HID;
	string	HeaderName<>;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::DefineList
% * This is the XDR specification for the Interested header
% * list described in the specification:
% * <a href="https://htmlpreview.github.io/?https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/draft-royer-phoenix.html#name-lists" target="_blank">Phoenix Index Define List</a>
% *
% * -IndexOP a IndexOP_e cast to a (uint8_t)
% *
% * -ListID The list id we are defining.
% *
% * -HeaderDefs An array of SingleEntry;
% */
#endif
class List
{
	uint8_t	IndexOp;
	LID_t		ListID;
	SingleEntry HeaderDefs<>;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::FolderIndex
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% *
% */
#endif

class Index
{
	int foo;
};
