%/**
%* Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0                                                                   
%* RiverExplorer is a trademark of RiverExplorer Games LLC                      
%*/
%

/**
 * These are just used below to make the 'program' values
 * more readable.
 */
#define PhoenixProgramNumber 1
#define PhoenixProgramVersion 1
#define PhoenixSendPacket 1
#define PhoenixNotifyPacket 2
#define PhoenixBroadcastPacket 3

program PhoenixProgram
{
	version PhoenixVersion
	{
		CmdPacket Send(CmdPacket) = PhoenixSendPacket;
		void Notifiy(CmdPacket) = PhoenixNotifyPacket;
		void Broadcast(CmdPacket) = PhoenixBoradcastPacket;
		
	} = PhoenixProgramVersion;
} = PhoenixProgramNumber;
