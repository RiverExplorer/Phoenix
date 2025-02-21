#ifdef RPC_HDR
%#ifdef BUILDING_LIBPHOENIX
%#include "Commands.hpp"
%#include "SEQ_t.hpp"
%#else
%#include <RiverExplorer/Phoenix/Commands.hpp>
%#include <RiverExplorer/Phoenix/SEQ_t.hpp>
%#endif
#endif

struct ByePayload {
	int foo;
};

struct ByeReplyPayload {
	int foo;
};

#ifdef RPC_HDR
%namespace RiverExplorer::Phoenix {
%extern Command * NewBye(SEQ_t Sequence);
%}
#endif
