/**
 * Project: Phoenix
 * Time-stamp: <2025-03-01 16:58:47 doug>
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
#include <map>
#include <vector>

#ifdef BUILDING_LIBPHOENIX
#include "Commands.hpp"
#else
#include <RiverExplorer/Phoenix/Commands.hpp>
#endif
namespace RiverExplorer::Phoenix
{

	/**
	 * @class Configuration Configuration.hpp <RiverExplorer/Phoenix/Configuration.hpp>
	 * An object that manages all configuration for Phoenix clients and servers.
	 */
	class Configuration
	{

	public:

		/**
		 * @struct Server Configuration.hpp <RiverExplorer/Phoenix/Configuration.hpp>
		 * Configuration per remote host is stored in this object.
		 */
		struct Server
		{
		public:

			/**
			 * Server - Default Constructor.
			 */
			Server();

			/**
			 * Server - Destructor.
			 */
			~Server();
			
			/**
			 * Set the Account name.
			 *
			 * @param Account The account to add.
			 */
			void Account(const char * Account);
			const std::string Account() const;
		
			/**
			 * Set the Account Password.
			 *
			 * @param Account The account to add.
			 * In the case of MD5, pass in the MD5 of the password.
			 */
			void Password(const char * Password);
			const std::string Password() const;
			
			/**
			 * Add the default public certificate to use.
			 * When non is configured, then the default OpenSSL
			 * certificate will be used.
			 *
			 * @param CertValue The value in the DefaultCert
			 * configuration setting.
			 */
			void DefaultPublicCert(const char * CertValue);
			const std::string DefaultPublicCert() const;
			 
			/**
			 * Add the default private certificate to use.
			 * When non is configured, then the default OpenSSL
			 * certificate will be used.
			 *
			 * @param CertValue The value in the DefaultCert
			 * configuration setting.
			 */
			void DefaultPrivateCert(const char * CertValue);
			const std::string DefaultPrivateCert() const;

			/**
			 * Set the host name or IP address.
			 *
			 * @param HostOrIp The host name or IP address.
			 */
			void HostOrIp(const char * HostOrIp);
			const std::string HostOrIp() const;
			
			/**
			 * Set the server name shown to the user.
			 *
			 * @param Server The server name shown to the user.
			 */
			void ServerName(const char * Server);

			/**
			 * Get the server name.
			 * This is the user friendly name, not the host name or IP.
			 *
			 * @return The server name.
			 */
			const std::string ServerName() const;
			
			/**
			 * Set the port number.
			 *
			 * @param Port The port number to use for the remote server.
			 */
			void Port(const char * Port);

			/**
			 * Set the port number.
			 *
			 * @param Port The port number to use for the remote server.
			 */
			void Port(uint16_t Port);

			/**
			 * Get the port number.
			 *
			 * @return The port number that was set.
			 */
			uint16_t Port() const;
		
			/**
			 * Set the position as shown to the user.
			 *
			 * @param ViewPosition The position in the list, first is zero (0).
			 */
			void Position(const char * ViewPosition);

			/**
			 * Set the position as shown to the user.
			 *
			 * @param ViewPosition The position in the list, first is zero (0).
			 */
			void Position(uint16_t Pos);

			/**
			 * Get the position number to show this server in the users list.
			 * The first one is at position zero (0).
			 *
			 * @return The postition number.
			 */
			uint16_t Position() const;

			/**
			 * Set the authentication information for the server.
			 *
			 * @param AuthMethod One of the implementations supported
			 * authentication methods (AUTHANONYMOUS, AUTHMD5, AUTHCERT_USER,
			 * AUTHCERT_TLS).
			 *
			 * @param Account For AuthMethods that require an account,
			 * the account name.
			 * Or for AUTHCERT_USER, the public certifcate path.
			 *
			 * @param Pw For accounts that have a password, the
			 * encrypted or MD5 password.
			 * Or for AUTHCERT_USER, the private (key) certifcate path.
			 */
			void Auth(const char * AuthMethod,
								const char * Account = nullptr,
								const char * Pw = nullptr);

			/**
			 * Set the authentication information for the server.
			 *
			 * @param AuthMethod One of the implementations supported
			 * authentication methods (AUTHANONYMOUS, AUTHMD5, AUTHCERT_USER,
			 * AUTHCERT_TLS).
			 *
			 * @param Account For AuthMethods that require an account,
			 * the account name.
			 *
			 * @param Pw For accounts that have a password, the
			 * encrypted or MD5 password.
			 */
			void Auth(CMD_e AuthMethod,
								const char * Account,
								const char * Password);

			/**
			 * Get the authentication information for the server.
			 *
			 * @param[out] AuthMethod One of the implementations supported
			 * authentication methods (AUTHANONYMOUS, AUTHMD5, AUTHCERT_USER,
			 * AUTHCERT_TLS).
			 *
			 * @param[out] Account For AuthMethods that require an account,
			 * the account name.
			 *
			 * @param[out] Pw For accounts that have a password, the
			 * encrypted or MD5 password.
			 */
			void GetAuth(CMD_e & AuthMethod,
									 const char *& Account,
									 const char *& Pw);
		private:

			/**
			 * The server name as shown to the user.
			 */
			std::string	_ServerName;

			/**
			 * The position in the server list to show to the user.
			 */
			uint16_t			_Position;

			/**
			 * The host name or IP address.
			 */
			std::string	_HostOrIp;

			/**
			 * The port number to use.
			 */
			uint16_t			_Port;
												
			/**
			 * The authentication method to use.
			 */
			CMD_e	_Method;
			
			/**
			 * The account name for AUTHMD5.
			 */
			std::string _Account;
			 
			/**
			 * The account password for AUTHMD5.
			 *
			 * @note
			 * This is the MD5 itself, not the password.
			 */
			std::string _Password;

			/**
			 * The full path to use for the public cert
			 * in AUTHCERT, AUTHCERT_USER, or AUTHCERT_TLS.
			 */
			std::string _PublicCert;
			 
			/**
			 * The full path to use for the private cert
			 * in AUTHCERT, AUTHCERT_USER, or AUTHCERT_TLS.
			 */
			std::string _PrivateCert;
			 
			/**
			 * The default public cert to use.
			 * When empty, uses the default OpenSSL cert.
			 * Ignored when _PublicCert is not empty for
			 * the target server.
			 */
			static std::string _DefaultPublicCert;
			 
			/**
			 * The default private cert to use.
			 * When empty, uses the default OpenSSL cert.
			 * Ignored when _PrivateCert is not empty for
			 * the target server.
			 */
			static std::string _DefaultPrivateCert;
			
		};

		/**
		 * Create a new server record.
		 *
		 * @param HostOrIp The hostname or IP address to connect to.
		 * HostOrIp and Port must be unique to the client.
		 *
		 * @param Name The name of the connection as shown to the user.
		 * Must be unique to the client.
		 *
		 * @param Port The port number to connect to.
		 * HostOrIp and Port must be unique to the client.
		 *
		 * @param AuthMethod One of the implementations supported
		 * authentication methods (AUTHANONYMOUS, AUTHMD5, AUTHCERT_USER,
		 * AUTHCERT_TLS).
		 *
		 * @param Account For AuthMethods that require an account,
		 * the account name.
		 *
		 * @param Pw For accounts that have a password, the
		 * encrypted or MD5 password.
		 *
		 * @return The new host configuration.
		 * If (HostOrIP and Port) or Name have already been used,
		 * then nullptr is returned.
		 */
		static Server *
		NewServer(const std::string & HostOrIp,
							const std::string & Name,
							uint16_t Port,
							CMD_e AuthMethod,
							const char * Account = nullptr,
							const char * Pw = nullptr);


		/**
		 * Get a list of server conncetions.
		 *
		 * @return An ordered list of server connections.
		 * The order is the position order supplied by the client.
		 */
		static const std::vector<Server*> & GetServers();

		/**
		 * Delete a server connection.
		 * There is no backup.
		 *
		 * @param ToDelete The server to delete from the client list.
		 */
		static void DeleteServer(Server * ToDelete);
		
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
		
		/**
		 * The list of known servers.
		 *
		 * <Name Shown To User>, HostConfiguation.
		 */
		static std::map<std::string, Server*> _Servers;
		
		/**
		 * The list of known servers.
		 *
		 * <Position in user list, HostConfiguation.
		 */
		static std::map<uint16_t, Server*> _ByPosition;
	};
}
