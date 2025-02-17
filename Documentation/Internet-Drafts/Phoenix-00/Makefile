#
# Build the draft
#
#
.SUFFIXES:	.txt .svg .cpp .xml

TOP			=	../..

TITLE		=	draft-royer-phoenix

VERSION	=	00

SOURCES.xml = \
	ABNF.xml \
	Abstract.xml \
	AdminBinaryValues.xml \
	AdminSummary.xml \
	AuthBinaryValues.xml \
	AuthAnonymous.xml \
	AuthCert.xml \
	AuthMD5.xml \
	AuthSummary.xml \
	CalendarSummary.xml \
	CapabilityIndex.xml \
	CapabilitySummary.xml \
	Commands.xml \
	Definitions.xml \
	EMailSummary.xml \
	draft-00.xml \
	FileBinaryValues.xml \
	Folder.xml \
	Index.xml \
	Introduction.xml \
	KeepAliveSummary.xml \
	MetaData.xml \
	PingSummary.xml \
	ProtocolBinaryValues.xml \
	ProtocolSummary.xml \
	RequirementsLanguage.xml \
	SecurityConsiderations.xml \
	Shared.xml \
	SMimeSummary.xml \
	ServerShutdown.xml \
	ServerLogs.xml \
	ServerKickUser.xml \
	ServerManageBans.xml \
	ServerViewStats.xml \
	ServerConfigure.xml \
	UserCreate.xml \
	UserDelete.xml \
	UserList.xml \
	UserRename.xml

ARTWORK.txt = \
	ABNF-00.txt \
	AnonFlow-00.txt \
	ANONYMOUS-00.txt \
	ANONYMOUS-01.txt \
	AUTHCERT-00.txt \
	AUTHCERT-01.txt \
	AUTHCERT-02.txt \
	IndexList-01.txt \
	IndexList-02.txt \
	IndexList-03.txt \
	IndexList-04.txt \
	IndexList-05.txt \
	IndexList-06.txt \
	IndexList-07.txt \
	IndexList-08.txt \
	IndexList-09.txt \
	IndexList-10.txt \
	MD5-00.txt \
	MD5-01.txt \
	MDN-00.txt \
	MDN-01.txt \
	Packet-00.txt \
	Packet-01.txt \
	Packet-02.txt \
	ServerConfigure-00.txt \
	ServerConfigure-01.txt \
	StringRef-00.txt

REFERENCES.xml = \
	References/reference.RFC.0822.xml \
	References/reference.RFC.5234.xml \
	References/reference.RFC.9051.xml \
	References/reference.RFC.2119.xml \
	References/reference.RFC.8174.xml \
	References/reference.rpcgen++.xml \
	References/reference.rpcgendocs.xml \
	References/reference.RFC.4506.xml \
	References/reference.RFC.8446.xml

# These files get processed by:
#
# cpp -P -nostdinc -C -undef <InFileName> > <OutFileName>
#
# to producte the INCLUDE.xml files.
#
INCLUDE.cpp = \
	include/01-ABNF-XDR.cpp \
	include/02-StringRef-XDR.cpp \
	include/03-PacketHeader-XDR.cpp \
	include/04-PacketBody-XDR.cpp \
	include/05-1-AOID-XDR.cpp \
	include/05-2-AdministrationOperations-XDR.cpp \
	include/06-01-ServerConfigure-XDR.cpp \
	include/06-02-ServerConfigure-XDR.cpp \
	include/06-03-AdminKick-XDR.cpp \
	include/06-04-ServerLogs-XDR.cpp \
	include/06-05-ManangeBans-XDR.cpp \
	include/06-06-Shutdown-XDR.cpp \
	include/06-07-UserCreate-XDR.cpp \
	include/06-08-UserDelete-XDR.cpp \
	include/06-09-UserList-XDR.cpp \
	include/06-10-UserPermissions-XDR.cpp \
	include/06-11-UserRename-XDR.cpp \
	include/06-12-ServerStats-XDR.cpp \
	include/07-MDNRecord-XDR.cpp \
	include/08-MDNEntry-XDR.cpp \
	include/09-AuthMD5-XDR.cpp \
	include/10-Auth_e-XDR.cpp \
	include/10-Auth-XDR.cpp \
	include/10-AuthAnonymous-XDR.cpp

INCLUDE.xml = $(INCLUDE.cpp:.cpp=.xml)

INCLUDES1 := $(shell /bin/ls include)
INCLUDES	= $(addprefix include/,$(INCLUDES1))

ABNF			:= $(shell /bin/ls include/*-ABNF.xml)

# The XDR.cpp files are preprocessed with
# "cpp -P -nostdinc -C -undef"
# to produce GENERATED.XDR
#
# The .cpp files include data from the source code.
# this is done so that the source code and the documentation
# are in sync.
#
XDR.cpp 			:= $(shell /bin/ls include/*-XDR.cpp)
GENERATED.XDR = $(XDR.cpp:.cpp=.xml)

GENERATED_ARTWORK.svg = $(ARTWORK.txt:.txt=.svg)

DOCDIR			= ../..

TOPDOC.xml	= $(TITLE)-$(VERSION).xml

TARGET.txt	=	$(DOCDIR)/$(TITLE).txt

TARGET.html	=	$(DOCDIR)/$(TITLE).html

TARGET.pdf	=	$(DOCDIR)/$(TITLE).pdf

CPP = $(TOP)/MyTools/SimpleCpp

all : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)

txt : $(TARGET.txt) $(ARTWORK.txt)

pdf : $(TARGET.pdf) $(GENERATED_ARTWORK.svg)

html : $(TARGET.html) (ARGWORK.svg)

$(TARGET.txt) : ALL-ABNF.xml ALL-XDR.xml $(ARTWORK.txt) $(TOPDOC.xml)  $(REFERENCES.xml) $(INCLUDES)
	xml2rfc -o $@ $(TOPDOC.xml)

$(TARGET.pdf) : ALL-ABNF.xml ALL-XDR.xml $(GENERATED_ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc --pdf -o $@ $(TOPDOC.xml)

$(TARGET.html): ALL-ABNF.xml ALL-XDR.xml $(GENERATED_ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc --html -o $@ $(TOPDOC.xml)

$(TOPDOC.xml) : draft-00.xml ALL-ABNF.xml ALL-XDR.xml $(SOURCES.xml) $(REFERENCES.xml) $(INCLUDES)
	xmllint --xinclude --format $< > $@

ALL-ABNF.xml : $(ABNF)
	echo "<section>" > $@
	echo "<name>Complete ABNF</name>" >> $@
	cat $(ABNF) >> $@
	echo "</section>" >> $@

ALL-XDR.xml : $(INCLUDE.xml) $(GENERATED.XDR)
	echo "<section>" > $@
	echo "<name>Complete XDR</name>" >> $@
	cat $(GENERATED.XDR) >> $@
	echo "</section>" >> $@

$(INCLUDE.xml) : $(INCLUDE.cpp)

scp : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)
	scp $(TARGET.html) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.pdf) riverexplorer.games:~/public_html/Phoenix
	scp $(TARGET.txt) riverexplorer.games:~/public_html/Phoenix

clean :
	$(RM) tmp.*
	$(RM) $(GENERATED_ARTWORK.svg)
	$(RM) $(TOPDOC.xml)
	$(RM) ALL-ABNF.xml ALL-XDR.xml
	$(RM) $(GENERATED.XDR)

%.svg : %.txt
	aasvg < $< > $@

include/%.xml : include/%.cpp
	cpp -P -nostdinc -C -undef $< > $@
