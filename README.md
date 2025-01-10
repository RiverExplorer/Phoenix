
To view the protocol documentation (work in progress):

    https://riverexplorer.games/Phoenix/

Email and MIME messages account for one the largest volumes of data on the
internet.
The transfer of these MIME message has not had a major updated in decades.
Part of the reason is that it is very important data and altering it
takes a great deal of care and planning.

Another major concern is security and authentication.
This proposal allows for existing autnentication to continue to work.

This is a MIME message transport that can facilitate
the transfer of any kind of MIME message. Including email, calendaring,
and text, image, or multimedia MIME messages.
It can transfer multipart and simple MIME messages.

The POP and IMAP protocols are overly chatty and now that the Internet
can handle 8-bit transfers, there is no need for the overly complex
text handling of messages.

This proposal includes a sample implementation.
Which also includes a gateway from this proposal to existing system.
Thunderbid and Outlook plugins are part of the sample implementation.

Many years ago I was asked by the IETF to see if I could revamp IMAP and POP.
Just now I am getting to it.
