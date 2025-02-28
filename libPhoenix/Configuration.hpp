/**
 * Project: Phoenix
 * Time-stamp: <2025-02-27 11:30:40 doug>
 *
 * @file Configuration.hpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 */

#include <string>

namespace RiverExplorer::Phoenix
{

	class Configuration
	{

	public:
		
		/**
		 * Initialize or open the client configuration information.
		 *
		 * Initialized it for the current system user.
		 *
		 * @param ClientName The name of the client program.
		 * Used to find and save configuration information.
		 *
		 */
		static void ClientInitializeConfiguration(const std::string  & ClientName);
		
		/**
		 * Initialize or open the server configuration information.
		 *
		 * Initialized it for the current system user.
		 *
		 * @param ServerName The name of the client program.
		 * Used to find and save configuration information.
		 *
		 */
		static void ServerInitializeConfiguration(const std::string & ServerName);
		
		/**
		 * Save client configuration value.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @param Value The value of the configuration variable.
		 */
		static bool	ClientSetConfig(const std::string & Key,
																const std::string & Value);
		
		/**
		 * Save server configuration value.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @param Value The value of the configuration variable.
		 */
		static bool	ServerSetConfig(const std::string & Key,
																const std::string & Value);
		
		/**
		 * Save new client configuration value.
		 * This method allows settings a description of the
		 * new KEY/VALUE pair.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @param Value The value of the configuration variable.
		 *
		 * @param A summary of the description information.
		 * Used to create HTML documentation.
		 *
		 * @param Description A human readable description of
		 * the Key. If not needed, or just updating the value
		 * of an existing KEY/VALUE pair, then
		 * call SetConfig(const std::string &, const std::string &);
		 * Used to create HTML documentation.
		 *
		 * @note
		 * If an existing Key already exists and has a description,
		 * then Description will be ignored.
		 * If an existing Key already exists and has a summary,
		 * then Summary will be ignored.
		 */
		static bool	ClientSetNewConfig(const std::string & Key,
																	 const std::string & Value,
																	 const std::string & Summary,
																	 const std::string & Description);
		
		/**
		 * Save new server configuration value.
		 * This method allows settings a description of the
		 * new KEY/VALUE pair.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @param Value The value of the configuration variable.
		 *
		 * @param A summary of the description information.
		 * Used to create HTML documentation.
		 *
		 * @param Description A human readable description of
		 * the Key. If not needed, or just updating the value
		 * of an existing KEY/VALUE pair, then
		 * call SetConfig(const std::string &, const std::string &);
		 * Used to create HTML documentation.
		 *
		 * @note
		 * If an existing Key already exists and has a description,
		 * then Description will be ignored.
		 * If an existing Key already exists and has a summary,
		 * then Summary will be ignored.
		 */
		static bool	ServerSetNewConfig(const std::string & Key,
																	 const std::string & Value,
																	 const std::string & Summary,
																	 const std::string & Description);
		
		/**
		 * Load a client configuration value.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @return The value of the configuration variable.
		 * When Key does not exist, empty results are returned.
		 */
		static const std::string 	ClientGetConfig(const std::string & Key);

		/**
		 * Load a server configuration value.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @return The value of the configuration variable.
		 * When Key does not exist, empty results are returned.
		 */
		static const std::string 	ServerGetConfig(const std::string & Key);

		/**
		 * Test if a client key exists.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @return True when Key has a saved value.
		 * Returns false if Key is not currently in the persistent
		 * configuration storage.
		 */
		static bool ClientHasConfig(const std::string & Key);

		/**
		 * Test if a server key exists.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @return True when Key has a saved value.
		 * Returns false if Key is not currently in the persistent
		 * configuration storage.
		 */
		static bool ServerHasConfig(const std::string & Key);

		/**
		 * Load client internal configuration information from config file.
		 *
		 * @return true when the configuration file was loaded.
		 * Else returns false.
		 */
		static bool ClientLoadConfig();

		/**
		 * Load server internal configuration information from config file.
		 *
		 * @return true when the configuration file was loaded.
		 * Else returns false.
		 */
		static bool ServerLoadConfig();

	private:

		/**
		 * Configuration - Default Constructor.
		 * All member values and functions are static.
		 *
		 * So the constructor is private:
		 */
		Configuration();

		/**
		 * Save a configuration value.
		 *
		 * @param IsServer When true, process server configuration
		 * information, else process client configuration information.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @param Value The value of the configuration variable.
		 */
		static bool	_SetConfig(bool IsServer,
													 const std::string & Key,
													 const std::string & Value);
		
		/**
		 * Save a new configuration value.
		 * This method allows settings a description of the
		 * new KEY/VALUE pair.
		 *
		 * @param IsServer When true, process server configuration
		 * information, else process client configuration information.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @param Value The value of the configuration variable.
		 *
		 * @param A summary of the description information.
		 * Used to create HTML documentation.
		 *
		 * @param Description A human readable description of
		 * the Key. If not needed, or just updating the value
		 * of an existing KEY/VALUE pair, then
		 * call SetConfig(const std::string &, const std::string &);
		 * Used to create HTML documentation.
		 *
		 * @note
		 * If an existing Key already exists and has a description,
		 * then Description will be ignored.
		 * If an existing Key already exists and has a summary,
		 * then Summary will be ignored.
		 */
		static bool	_SetNewConfig(bool IsServer,
															const std::string & Key,
															const std::string & Value,
															const std::string & Summary,
															const std::string & Description);


		/**
		 * Load a configuration value.
		 *
		 * @param IsServer When true, process server configuration
		 * information, else process client configuration information.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @return The value of the configuration variable.
		 * When Key does not exist, empty results are returned.
		 */
		static const std::string 	_GetConfig(bool IsServer,
																				 const std::string & Key);

		/**
		 * Test if key exists.
		 *
		 * @param IsServer When true, process server configuration
		 * information, else process client configuration information.
		 *
		 * @param Key The name of of the configuration variable.
		 *
		 * @return True when Key has a saved value.
		 * Returns false if Key is not currently in the persistent
		 * configuration storage.
		 */
		static bool _HasConfig(bool IsServer, const std::string & Key);

		/**
		 * Initialize or open the configuration information.
		 *
		 * Initialized it for the current system user.
		 *
		 * @param IsServer When true, process server configuration
		 * information, else process client configuration information.
		 *
		 */
		static void _InitializeConfiguration(bool IsServer);

		/**
		 * Save internal configuration information to config file.
		 *
		 * @param IsServer When true, process server configuration
		 * information, else process client configuration information.
		 *
		 * @return false if it could not be saved.
		 * Otherwise returns true;
		 */
		static bool _SaveConfig(bool IsServer);
		
		/**
		 * Load internal configuration information from config file.
		 *
		 * @param IsServer When true, process server configuration
		 * information, else process client configuration information.
		 *
		 * @return true when the configuration file was loaded.
		 * Else returns false.
		 */
		static bool _LoadConfig(bool IsServer);

		/**
		 * Used by _LoadConfig() to process each node.
		 * I am using (void*) to keep the implementation
		 * details out of the header file.
		 *
		 * @param IsServer When true, process server configuration
		 * information. Else process client information.
		 */
		static void _ProcessNode(bool IsServer, void * Node);

		/**
		 * The client name (or nullptr)
		 */
		static const char * _ClientProgramName;

		/**
		 * The server name (or nullptr)
		 */
		static const char * _ServerProgramName;

	};
}
