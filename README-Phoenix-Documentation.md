

Phoenix © 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0

This Phoenix code is copyright and free to use.

See the LICENSE file.

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

 -----------------------------------------------------------------
To view the protocol documentation (work in progress):

 The Internet draft (PDF):

https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/Internet-Drafts/Phoenix-00/draft-royer-phoenix-00.pdf

 The Internet draft (html):

https://htmlpreview.github.io/?https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/Internet-Drafts/Phoenix-00/draft-royer-phoenix-00.html

 The Internet draft (text):

https://github.com/RiverExplorer/Phoenix/blob/main/Documentation/Internet-Drafts/Phoenix-00/draft-royer-phoenix-00.txt

 The sample implemenation public API:

 TODO

 The sample implemenation internals and public API:

 TODO

 -----------------------------------------------------------------
To build the documentation you will need:

 - dot from https://www.graphviz.org
 - xml2rfc from https://authors.ietf.org/en/author-tools-web-service

 -----------------------------------------------------------------

For build instructions see libPhoenix/README-libphoenix.md

This project builds on Linux for Linux, Windows, and Android.
You will need the mingw cross compilers to build for windows.
You will need the Android NDK and tools to build for Android.

Currently, all platform builds must be done on Linux.

-----------------------------------------------------------------
 Doug Royer
 RiverExplorer.US@gmail.com
