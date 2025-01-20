
all : Internal Public ID

Internal :
	doxygen Doxyfile-All

Public :
	doxygen Doxyfile-API

ID :
	(cd MyTools;make)
	(cd Internet-Drafts/Phoenix-00;make)

clean:
	$(RM) -r GeneratedDocs/*
	$(RM) Warning-*
	(cd MyTools;make clean)
	(cd Internet-Drafts/Phoenix-00;make clean)


