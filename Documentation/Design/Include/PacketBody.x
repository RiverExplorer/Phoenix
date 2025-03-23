/*
 * One command.
 */
struct Command {
	SEQ_t      Sequence;
	OneCommand Payload;
};

/**
 * A packet body.
 */
struct PacketBody {
  Command Commands<>; /* XDR arrays start with the Length */
};

