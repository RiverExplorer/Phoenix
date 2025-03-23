#if defined RPC_HDR || defined RPC_XDR
%/**
% * Project: Phoenix
% * Time-stamp: <2025-03-03 01:51:48 doug>
% *
% * @file CmdFolder.x
% * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
% * @author Douglas Mark Royer
% * @date 24-FEB-20205
% *
% * licensed under CC BY 4.0.
% *
% * RiverExplorer is a trademark of Douglas Mark Royer
% */
#endif
%
%#ifdef BUILDING_LIBPHOENIX
%#include "Types.hpp"
%#include "MetaData.hpp"
%#else
%#include <RiverExplorer/Phoenix/Types.hpp>
%#include <RiverExplorer/Phoenix/MetaData.hpp>
%#endif

#ifdef RPC_HDR
%/**
% * @enum RiverExplorer::Phoenix::Folder_e
% */
#endif

enum Folder_e
{
	FolderCreate_Cmd = 0,
	FolderCreateReply_Cmd = 1,
	FolderCopy_Cmd = 2,
	FolderCopyReply_Cmd = 3,
	FolderDelete_Cmd = 4,
	FolderDeleteReply_Cmd = 5,
	FolderRename_Cmd = 6,
	FolderRenameReply_Cmd = 7,
	FolderMove_Cmd = 8,
	FolderMoveReply_Cmd = 9,
	FolderShare_Cmd = 10,
	FolderShareReply_Cmd = 11,
	FolderList_Cmd = 12,
	FolderListReply_Cmd = 13,
	FileCreate_Cmd = 14,
	FileCreateReply_Cmd = 15,
	FileCopy_Cmd = 16,
	FileCopyReply_Cmd = 17,
	FileDelete_Cmd = 18,
	FileDeleteReply_Cmd = 19,
	FileRename_Cmd = 20,
	FileRenameReply_Cmd = 21,
	FileMove_Cmd = 22,
	FileMoveReply_Cmd = 23,
	FileShare_Cmd = 24,
	FileShareReply_Cmd = 25,
	FileGet_Cmd = 26,
	FileGetReply_Cmd = 27
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::FolderCreate CmdFolder.hpp <RiverExplorer/Phoenix/CmdFolder.hpp>
% * @addtogroup Folder
% *
% * The full path is the path from the top of the users virtual
% * home folder, and includes the folder name.
% *
% * The path separator is the '/' (UTF-8 value 0x2f) character.
% *
% * The root folder is "/".
% */
#endif
class FolderCreate
{
	string	FullPath<>;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::FolderCreateReply CmdFolder.hpp <RiverExplorer/Phoenix/CmdFolder.hpp>
% * @addtogroup Folder
% * The reply for CreateFolder, Success is set to true if the
% * folder was created. Otherwise it is set to false.
% */
#endif
class FolderCreateReply
{
	bool_t	Success;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::FolderCopyr CmdFolder.hpp <RiverExplorer/Phoenix/CmdFolder.hpp>
% * @addtogroup Folder
% *
% * The full original path is the path from the top of the users virtual
% * home folder, and includes the folder name. And names
% * the folder to copy from.
% *
% * The full destination path is the path from the top of the users virtual
% * home folder, and includes the folder name.
% * And names the folder that will have a copy of FullOriginalPath
% * placed into.
% *
% * When recursive is false, only the contents are copied
% * and not any directories.
% *
% * When recursive is true, the contents are copied
% * and recursively copies all directories in the original path directories
% * to the new destination.
% *
% * The path separator is the '/' (UTF-8 value 0x2f) character.
% *
% * The root folder is "/".
% */
#endif
class FolderCopy
{
	string	FullOriginalPath<>;
	string	FullDestinationPath<>;
	bool_t	Recursive;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phoenix::FolderCopyReply CmdFolder.hpp <RiverExplorer/Phoenix/CmdFolder.hpp>
% * @addtogroup Folder
% *
% * The reply for CopyFolder, Success is set to true if the
% * folder was copied into FillToPath. Otherwise it is set to false.
% *
% * A false indicates that nothing was copied.
% * A true indicates that everything was copied.
% *
% * Must fail and leave the original structure in place
% * on any failure, and return false.
% */
#endif
class FolderCopyReply
{
	bool_t	Success;
};

class FolderDelete
{
	int todo;
};

class FolderDeleteReply
{
	int todo;
};

class FolderRename
{
	int todo;
};

class FolderRenameReply
{
	int todo;
};

class FolderMove
{
	int todo;
};

class FolderMoveReply
{
	int todo;
};

class FolderShare
{
	int todo;
};

class FolderShareReply
{
	int todo;
};

class FolderList
{
	int todo;
};

class FolderListReply
{
	int todo;
};

class FileCreate
{
	int todo;
};

class FileCreateReply
{
	int todo;
};

class FileCopy
{
	int todo;
};

class FileCopyReply
{
	int todo;
};

class FileDelete
{
	int todo;
};

class FileDeleteReply
{
	int todo;
};

class FileRename
{
	int todo;
};

class FileRenameReply
{
	int todo;
};

class FileMove
{
	int todo;
};

class FileMoveReply
{
	int todo;
};

class FileShare
{
	int todo;
};

class FileShareReply
{
	int todo;
};

class FileGet
{
	int todo;
};

class FileGetReply
{
	int todo;
};

union FolderCmdData switch (Folder_e FCmd)
{
 case FolderCreate_Cmd:
	 FolderCreate * FolderCreateFolder;
	 
 case FolderCopy_Cmd:
	 FolderCopy * FolderCopyData;
	 
 case FolderDelete_Cmd:
	 FolderDelete * FolderDeleteData;
	 
 case FolderRename_Cmd:
	 FolderRename * FolderRenameData;
	 
 case FolderMove_Cmd:
	 FolderMove * FolderMoveData;
	 
 case FolderShare_Cmd:
	 FolderShare * FolderShareData;
	 
 case FolderList_Cmd:
	 FolderList * FolderListData;
	 
 case FileCreate_Cmd:
	 FileCreate * FileCreateData;
	 
 case FileCopy_Cmd:
	 FileCopy * FileCopyData;
	 
 case FileDelete_Cmd:
	 FileDelete * FileDeleteData;
	 
 case FileRename_Cmd:
	 FileRename * FileRenameData;
	 
 case FileMove_Cmd:
	 FileMove * FileMoveData;
	 
 case FileShare_Cmd:
	 FileShare * FileShareData;
	 
 case FileGet_Cmd:
	 FileGet * FileGetData;
	 
};

union FolderReplyData switch (Folder_e FCmd)
{
 case FolderCreateReply_Cmd:
	 FolderCreateReply * FolderCreateReplyData;
	 
 case FolderCopyReply_Cmd:
	 FolderCopyReply * FolderCopyReplyData;
	 
 case FolderDeleteReply_Cmd:
	 FolderDeleteReply * FolderDeleteReplyData;
	 
 case FolderRenameReply_Cmd:
	 FolderRenameReply * FolderRenameReplyData;
	 
 case FolderMoveReply_Cmd:
	 FolderMoveReply * FolderMoveReplyData;
	 
 case FolderShareReply_Cmd:
	 FolderShareReply * FolderShareReplyData;
	 
 case FolderListReply_Cmd:
	 FolderListReply * FolderListReplyData;
	 
 case FileCreateReply_Cmd:
	 FileCreateReply * FileCreateReplyData;
	 
 case FileCopyReply_Cmd:
	 FileCopyReply * FileCopyReplyData;
	 
 case FileDeleteReply_Cmd:
	 FileDeleteReply * FileDeleteReplyData;
	 
 case FileRenameReply_Cmd:
	 FileRenameReply * FileRenameReplyData;
	 
 case FileMoveReply_Cmd:
	 FileMoveReply * FileMoveReplyData;
	 
 case FileShareReply_Cmd:
	 FileShareReply * FileShareReplyData;
	 
 case FileGetReply_Cmd:
	 FileGetReply * FileGetReplyData;

};


#ifdef RPC_HDR
%/**
% * An XDR representation of a file, not the
% * contents, but the information about the file.
% *
% * - Meta: Is an array of MetaData.
% * - Name: This is the name excluding the path.
% * - Size: This is the full size of the file.
% * The FLAG_... symbols are symbolic names
% */
#endif

class FileInformation
{
	MetaData	Meta<>;
	string		Name<>;
	uint32_t	Size;
	uint32_t	FlagBits;
};

#ifdef RPC_HDR
%/**
% * @class RiverExplorer::Phonnix::FolderInformation
% * An XDR representation of a folder
% *
% * - Meta: Is an array of MetaData.
% * - Name: This is the name excluding the path.
% * - Folders: An array of information about folders within this folder.
% * - Files: An array of information about files in this folder.
% */
#endif
class FolderInformation
{
	MetaData		Meta<>;
	string			Name<>;
	FolderInformation	Folders<>;
	FileInformation		Files<>;
};

class Folder
{
	FolderCmdData Data;
};

class FolderReply
{
	FolderReplyData Data;
};
