/**
 * Project: Phoenix
 * Time-stamp: <2025-02-28 10:21:32 doug>
 *
 * @file Configuration.cpp
 * @copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * @author Douglas Mark Royer
 * @date 24-FEB-20205
 *
 * licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of Douglas Mark Royer
 *
 * @subpage Configuration
 * A Phoenix implementation may act as a client for one or more
 * protocols and also a server for one or more protocols.
 * So this code supplied the needs of both the client and
 * server configuration information.
 *
 * Client specific information starts with 'Client', and server
 * specific information starts with 'Server'. Everything else
 * is housekeeping or shared.
 *
 * At runtime, this implementation may be client only, server only,
 * or both.
 *
 * @note
 * This configuration information is unique to this implementation
 * and is not a standard or proposed standard.
 *
 * In this sample implementation, the configuration
 * information is stored:
 *
 * <ul>
 *   <li>
 *     Linux: ~/.config/RiverExplorer/Phoenix/{Client,Server}/<ProgramName>/<ProgramName>.xml
 *   </li>
 *   <li>
 *     Windows: APPDATA/RiverExplorer/Phoenix/{Client,Server}/<ProgramName>/<ProgramName>.xml
 *   </li>
 * </ul>
 */

#include "Configuration.hpp"
#include "Log.hpp"
#include <cstdint>
#include <string.h>
#include <mutex>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <regex>

#include <map>

#ifndef W64
#include <pwd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#endif

namespace RiverExplorer::Phoenix
{
	extern const char * const ConfigToHtml;

	std::map<std::string, Configuration::Server*>
	Configuration::_Servers;
	
	std::map<uint16_t, Configuration::Server*>
	Configuration::_ByPosition;

	// A cache of _ByPosition, as a vector.
	//
	std::vector<Configuration::Server*> _UserView;
	
	std::mutex ServerMutex;
	
	std::string Configuration::Server::_DefaultPublicCert;
	std::string Configuration::Server::_DefaultPrivateCert;
	
	/**
	 * Set to true when the client configuration information has been initialized.
	 */
	static bool ClientInitialized = false;

	/**
	 * Set to true when the server configuration information has been initialized.
	 */
	static bool ServerInitialized = false;

	/**
	 * The full path to the client configuration file.
	 */
	static std::string ClientPath;

	/**
	 * The full path to the client XSLT configuration file.
	 */
	static std::string ClientXsltPath;

	/**
	 * The full path to the client HTML configuration file.
	 */
	
	static std::string ClientHtmlPath;

	/**
	 * The full path to the server configuration file.
	 */
	static std::string ServerPath;

	/**
	 * The full path to the server XSLT configuration file.
	 */
	static std::string ServerXsltPath;

	/**
	 * The full path to the server HTML configuration file.
	 */
	static std::string ServerHtmlPath;

	/**
	 * Allow IPv4 addresses
	 */
	static const char * const AllowIpV4_s = "AllowIpV4";

	/**
	 * Allow IPv6 addresses
	 */
	static const char * const AllowIpV6_s = "AllowIpV6";

	/**
	 * The cert to use for a specific Server.
	 * An empty value means to use the default OpenSSL certificats.
	 */
	static const char * const Default_s = "Default";
	static const char * const DefaultPublic_s = "DefaultPublic";
	static const char * const DefaultPrivate_s = "DefaultPrivate";
	
	/**
	 * All configuration information is in an XML "Configure" node.
	 */
	static const char * const ConfigNode_s = "Configure";

	/**
	 * An empty value means use the default OpenSSL cert.
	 * The format is
	 * Certs: The DefaultCert is the default certificate set to use.
	 */
	static const char * const DefaultCertToUse_s = "DefaultCert";

	/**
	 * Detailed information about the Key/Value pair
	 * is stored in a XML "Detail" node.
	 */
	static const char * const Detail_s = "Detail";

	/**
	 * The IP addresses for the server to listen on.
	 */
	static const char * const IpToListenOn_s = "IpToListenOn";

	/**
	 * The interface for the server to listen on.
	 */
	static const char * const InterfacesToListenOn_s = "InterfacesToListenOn";

	/**
	 * Auth: The authentication to use for a specific server
	 */
	static const char * const Auth_s = "Auth";
	static const char * const Account_s = "Account";
	static const char * const Anonymous_s = "Anonymous";
	static const char * const MD5_s = "MD5";
	static const char * const AuthCertUser_s = "CertUser";
	static const char * const AuthCertTls_s = "CertTls";
	static const char * const Password_s = "Password";
	static const char * const Type_s = "Type";
	
	/**
	 * The Key value is stored in an XML Key node.
	 */
	static const char * const Key_s = "Key";
	
	/**
	 * The 'LogDevice' XML node holds the value of where
	 * to store log information.
	 * -When it starts with a '/', it is a full path to the file.
	 * -When it starts with 'syslog:', it goes to syslog.
	 * -When it starts with 'sql:', it goes to a database.
	 */
	static const char * const LogDevice_s = "LogDevice";

	/**
	 * The minimum time that can be set for the client to
	 * check the sockets for incoming data from servers
	 * on the client side is stored in the XML 'MinServerCheck' node.
	 */
	static const char * const ClientMinCheckServerKey_s = "MinServerCheck";

	/**
	 * The currently configured time that can be set for the client to
	 * check the sockets for incoming data from servers
	 * on the client side
	 * is stored in the XML 'ServerCheck' node.
	 */
	static const char * const ClientCheckServerKey_s = "ServerCheck";

	/**
	 * The string 'false'.
	 */
	static const char * const False_s = "false";
	
	/**
	 * Generate HTML file when configuration file updates.
	 */
	static const char * const GenerateHTML_s = "GenerateHTML";
	static const char * const HTMLDirectory_s = "HTMLDirectory";
	/**
	 * The Key/Value, Summary, and detail are stored in an XML 'Name' node.
	 */
	static const char * const Name_s = "Name";

	/**
	 * The string 'Phoenix' needed in the code.
	 */
	static const char * const Phoenix_s = "Phoenix";

	/**
	 * The port the server to listen on.
	 */
	static const char * const PortToListenOn_s = "PortToListenOn";

	/**
	 * The string 'Private'.
	 */
	static const char * const Private_s = "Private";
	
	/**
	 * The string 'Public'.
	 */
	static const char * const Public_s = "Public";

	/**
	 * The string 'ReadOnly' needed in the code.
	 */
	static const char * const ReadOnly_s = "ReadOnly";
	
	/**
	 * The string 'RiverExplorer' needed in the code.
	 */
	static const char * const RiverExplorer_s = "RiverExplorer";

	/**
	 * The configuration key for a specific server.
	 *
	 * The configuration each known server contains
	 * one or more of the following elements in any order:
	 *
	 * @verbatim
	 *	<Server>
	 *   <HostOrIp>host or ip</HostOrIp>
	 *   <Port>Port number</Port>
	 *   <Position>User view order</Position>
 	 *	 <Auth Method="xxx"/>
	 *   <Account>Login account</Account>
	 *   <Password>encrypted password</Account>
	 *   <PublicCert>Path to public cert</PublicCert>
	 *   <PrivateCert>Path to private Key</PrivateCert>
	 *  </Server>
	 * @endverbatim
	 */
	static const char * const Server_s = "Server";
	static const char * const ServerName_s = "ServerName";
	static const char * const HostOrIp_s = "HostOrIp";
	static const char * const Port_s = "Port";
	static const char * const Position_s = "Position";
	
	/**
	 * The short description of the Key/Value pair is stored
	 * int the XML 'Summary' node.
	 */
	static const char * const Summary_s = "Summary";

	/**
	 * The string 'true'.
	 */
	static const char * const True_s = "true";
	
	/**
	 * The XML files are stored in the UTF-8 charset.
	 */
	static const char * const Utf8_s = "utf-8";

	/**
	 * The value part of the Key/Value are stored in the 'Value' XML node.
	 */
	static const char * const Value_s = "Value";

	/**
	 * The vendor unique ID is stored in the 'VendorID' XML node.
	 * This is used identify compatible and non standard
	 * usages of the Phoenix protocol.
	 */
	static const char * const VendorIDKey_s = "VendorID";

	/**
	 * The VendorID for this Phoenix implementation.
	 * Vendors should update this.
	 * It may include comma (,) separated values.
	 */
	static const char * const VendorIDValue_s = "RiverExplorer.Phoenix.20250215";

	/**
	 * The prefix to the XML namespace tag.
	 */
	static const char * const XmlNs_s = "xmlns";

	/**
	 * The namespace used for the Client namespace.
	 */
	static const char * const ClientXmlNsConfig_s = "xmlns:C";

	/**
	 * The namespace used for the Server namespace.
	 */
	static const char * const ServerXmlNsConfig_s = "xmlns:S";

	/**
	 * The namespace used for the RiverExplorer namespace.
	 */
	static const char * const XmlNsRE_s = "xmlns:RE";

	/**
	 * The namespace used for the Server namespace.
	 */
	static const char * const ServerXmlNsRE_s = "xmlns:RE";

	/**
	 * The namespace URL used for this implementations
	 * RiverExplorer namespace.
	 */
	static const char * const XmlNsREUrl_s
	= "http://RiverExplorer.US/Namespaces/XML";
	
	/**
	 * The namespace URL used for this implementations
	 * RiverExplorer client namespace.
	 */
	static const char * const ClientXmlNsConfigUrl_s
	= "http://RiverExplorer.US/Namespaces/XML/Phoenix"
		"/ClientConfiguration.xml/20250215";
	
	/**
	 * The namespace URL used for this implementations
	 * RiverExplorer server namespace.
	 */
	static const char * const ServerXmlNsConfigUrl_s
	= "http://RiverExplorer.US/Namespaces/XML/Phoenix"
		"/ServerConfiguration.xml/20250215";

	/**
	 * The version of the configuration information
	 * is stored in the 'Version' XML node.
	 */
	static const char * const VersionKey_s = "Version";

	/**
	 * This implementations version.
	 */
	static const uint64_t			Version_i = 20250215L;

	/**
	 * The XML Document node for the client.
	 */
	static xmlDoc			* ClientDoc = nullptr;

	/**
	 * The XML Document node for the server.
	 */
	static xmlDoc			* ServerDoc = nullptr;

	/**
	 * The client configuration information was loaded.
	 */
	static bool					ClientLoaded = false;

	/**
	 * The server configuration information was loaded.
	 */
	static bool					ServerLoaded = false;

	/**
	 * The client Configure node was loaded.
	 */
	static bool					ClientHaveConfig = false;

	/**
	 * The server Configure node was loaded.
	 */
	static bool					ServerHaveConfig = false;

	const char				*	Configuration::_ClientProgramName;
	const char				*	Configuration::_ServerProgramName;
	
	/**
	 * The data associated with a configuration Key/Value set.
	 */
	struct Entry
	{
		/**
		 * The configuration name of the data.
		 */
		const char * Key;

		/**
		 * The configuration value.
		 * -May be nullptr.
		 * -When nullptr, it is treated as empty.
		 * -May be an empty string, and treated the same as nullptr.
		 * -Keys can exist, without a value.
		 */
		const char * Value;

		/**
		 * A short summary of the Key/Value pair.
		 * May be nullptr.
		 * This information is used to generate an HTML admin page.
		 */
		const char * Summary;

		/**
		 * A detailed description of the Key/Value pair.
		 * May be nullptr.
		 * This information is used to generate an HTML admin page.
		 */
		const char * Detail;
	};

	Configuration::Server *
	Configuration::NewServer(const std::string & HostOrIp,
													 const std::string & Name,
													 uint16_t Port,
													 CMD_e AuthMethod,
													 const char * Account,
													 const char * Pw)
	{
		Server * Results = nullptr;

		bool Found = true;

		// Check to make sure no such server already exists.
		//
		ServerMutex.lock();
		std::map<std::string,Configuration::Server*>::const_iterator It;
		It = _Servers.find(HostOrIp);
		if (It == _Servers.cend()) {
			Found = false;
		}
		ServerMutex.unlock();

		if (!Found) {
			switch (AuthMethod) {

			case AUTHANONYMOUS:
				Results = new Configuration::Server();
				Results->Auth(AUTHANONYMOUS, nullptr, nullptr);
				break;
			
			case AUTHMD5:
				Results = new Configuration::Server();
				Results->Auth(AUTHMD5, Account, Pw);
				break;
				
			case AUTHCERT_USER:
				Results = new Configuration::Server();
				Results->Auth(AUTHCERT_USER,
											Account,  // Public Cert path.
											Pw);// Private Cert (key) path.
			break;
			
			case AUTHCERT_TLS:
				Results = new Configuration::Server();
				Results->Auth(AUTHCERT_TLS, nullptr, nullptr);
				break;
				
			default:
				/*NOTUSED*/
				break;
			}

			if (Results != nullptr) {
				Results->HostOrIp(HostOrIp.c_str());
				Results->ServerName(Name.c_str());
				Results->Port(Port);
				ServerMutex.lock();
				Results->Position((uint16_t)_Servers.size());
				_Servers.insert(std::make_pair(HostOrIp, Results));
				_ByPosition.insert(std::make_pair(Results->Position(), Results));
				ServerMutex.unlock();
			}
		}
		
		return(Results);
	}

	const std::vector<Configuration::Server*> &
	Configuration::GetServers()
	{
		std::map<uint16_t,Configuration::Server*>::const_iterator SIt;

		_UserView.clear();

		ServerMutex.lock();
		for (SIt = _ByPosition.cbegin(); SIt != _ByPosition.cend(); SIt++) {
			_UserView.push_back(SIt->second);
		}
		ServerMutex.unlock();

		return(_UserView);
	}

	void
	Configuration::DeleteServer(Server * ToDelete)
	{
		ServerMutex.lock();
		
		std::map<std::string, Configuration::Server*>::iterator SIt;
		Server * Ptr;
		
		for (SIt = _Servers.begin(); SIt != _Servers.end(); SIt++) {
			Ptr = SIt->second;
			if (Ptr != nullptr) {
				if (Ptr == ToDelete) {
					delete Ptr;
					SIt->second = nullptr;
					_Servers.erase(SIt);
					break;
				}
			}
		}

		std::map<uint16_t,Configuration::Server*>::iterator PIt;

		for (PIt = _ByPosition.begin(); PIt != _ByPosition.end(); PIt++) {
			Ptr = PIt->second;
			if (Ptr == ToDelete) {
				PIt->second = nullptr;
				_ByPosition.erase(PIt);
				// We had a copy of the Server pointer, so we do not delete it.
				break;
			}
		}
		
		ServerMutex.unlock();

		return;
	}
		
	static Entry	*	LastEntry = nullptr;
	static Configuration::Server * LastConfiguration = nullptr;

	/**
	 * These are the values an EMPTY configuration are set to.
	 * This set is common to both the server and the client.
	 */
	static Entry  CommonEntries[] = {
		{
			GenerateHTML_s,
			strdup("true"),
			"Create HTML file mirroring the configuration file.",
			"Each time the configuration file is updated,"
			" create an HTML file in the same directory as the configuration"
			" file, or where HTMLDirectory points. "
			" The results can be viewed in a browser, showing the key/values."
		},
		{
			HTMLDirectory_s,
			strdup(""),
			"Where to put the generated HTML.",
			"The full path to where to place the HTML."
			" Defaults to where the user configuration is"
			" The tilde (~) character expands to the users home directory."
		},
		{
			VendorIDKey_s,
			strdup(VendorIDValue_s),
			"This is the vendor ID",
			"The vendor ID allows the Phoenix protocol to be used"
			" by applications that fall outside of any predefined usage."
			" By comparing the client and server vendor ID, then applications"
			" can determine if they have connected with a compatible non-standard"
			" peer."
		},
		{
			LogDevice_s,
			strdup(""), /** Filled in at run time to file, when empty.*/
			"Where this programs logs go.",
			"The full path where the associated programs log file is placed."
			" Or 'syslog' to be logged to syslog. Or 'SQL:...' to be"
			" logged to a database."
			" SQL:Postgres:host:port:acct:pw,"
			" SQL:MySql:host:port:acct:pw,"
			" SQL:MsSQL:host:port:acct:pw"
		},
		{
			DefaultPublic_s,
			strdup(""),
			"The default Public certificate to use.",
			"The default certificate to use, or empty for the default"
			" used by openssl."
			" It is the full path to the default public PEM certificate."
			" Ignored if DefaultPrivateCert is not set, and a matching cert."
		},
		{
			DefaultPrivate_s,
			strdup(""),
			"The default private certificate to use.",
			"The default certificate to use, or empty for the default"
			" used by openssl."
			" It is the full path to the default private PEM certificate."
			" Ignored if DefaultPrivateCert is not set, and a matching cert."
		},
		{nullptr, nullptr, nullptr, nullptr} /** End of list. */
	};

	/**
	 * These are the values an EMPTY configuration are set to.
	 * This set is unique to a client.
	 */
	static Entry  ClientEntries[] = {
		{
			ClientMinCheckServerKey_s, /** MinServerCheck */
			strdup("5"),
			"Minimum Check Server Time",
			"On a client, it checks for new data at intervals."
			" This is the minimum time time that can be set."
			" When 'ServerCheck' (ClientCheckServerKey_s) value is set to a"
			" value less than this, it will be set to this value."
		},
		{
			ClientCheckServerKey_s,						/** ServerCheck */
			strdup("5"),
			"Current Check Server Time",
			"How often to check a server for incoming data."
			" May not be less than 'MinServerCheck' value."
		},
		{nullptr, nullptr, nullptr, nullptr} /** End of list. */
	};

	/**
	 * These are the values an EMPTY configuration are set to.
	 * This set is unique to a server.
	 */
	static Entry  ServerEntries[] = {
		{
			IpToListenOn_s,
			strdup("*"),
			"Which IP address(s) to listen to.",
			"Listen on these IP addresses. Can be a '*' for all."
			"Or a comma separated list of IP addresses."
		},
		{
			InterfacesToListenOn_s,
			strdup("*"),
			"Which hardware interfaces to listen on.",
			"Which hardware interfaces to listen on."
			"'*' for all. Or a comma separated list of interface names."
		},
		{
			PortToListenOn_s,
			strdup("6112"),
			"Which Port Number to listen to.",
			"Listen on these port numbers."
			"One or a comma separated list of ports."
		},
		{
			AllowIpV4_s,
			strdup("true"),
			"When true, allow IPv4 connections.",
			"When set to true allow IPv4 connections."
		},
		{
			AllowIpV6_s,
			strdup("true"),
			"When true, allow IPv6 connections.",
			"When set to true allow IPv6 connections."
		},
		{nullptr, nullptr, nullptr, nullptr} /** End of list. */
	};

	/**
	 * The client configuration information.
	 */
	static std::map<std::string,Entry*> ClientConfig;
	
	/**
	 * The Server configuration information.
	 */
	static std::map<std::string,Entry*> ServerConfig;


	bool
	Configuration::ClientSetNewConfig(const std::string & Key,
																		const std::string & Value,
																		const std::string & Summary,
																		const std::string & Detail)
	{
		return(_SetNewConfig(false, Key, Value, Summary, Detail));
	}
	
	bool
	Configuration::ServerSetNewConfig(const std::string & Key,
																		const std::string & Value,
																		const std::string & Summary,
																		const std::string & Detail)
	{
		return(_SetNewConfig(true, Key, Value, Summary, Detail));
	}
	
	bool
	Configuration::_SetNewConfig(bool IsServer,
															 const std::string & Key,
															 const std::string & Value,
															 const std::string & Summary,
															 const std::string & Detail)
	{
		bool Results = false;

		if ((IsServer && ServerInitialized) || (!IsServer && ClientInitialized)) {

			// Do not allow overwriting the VendorID key.
			//
			if (Key != VendorIDKey_s) {
				// Key must have a value.
				//
				if (Key.length() > 0) {
					// Look for existing key.
					//
					std::map<std::string, Entry*>::const_iterator Found;

					if (IsServer) {
						Found = ServerConfig.find(Key.c_str());
					} else {
						Found = ClientConfig.find(Key.c_str());
					}

					bool WasFound;

					if (IsServer) {
						WasFound = (Found != ServerConfig.cend());
					} else {
						WasFound = (Found != ClientConfig.cend());
					}
				
					if (WasFound) {
						// Found existing key/value.
						//
						// Delete the old value, set the new value.
						//
						delete Found->second->Value;
						Found->second->Value = strdup(Value.c_str());

						// If the summary does not exists, add the supplied one.
						//
						if (Found->second->Summary == nullptr
								|| strlen(Found->second->Summary) == 0) {
								
							if (Found->second->Summary != nullptr) {
								delete Found->second->Summary;
							}
							Found->second->Summary = strdup(Summary.c_str());
						}
				
						// If the details does not exists, add the supplied one.
						//
						if (Found->second->Detail == nullptr
								|| strlen(Found->second->Detail) == 0) {
						
							if (Found->second->Detail != nullptr) {
								delete Found->second->Detail;
							}
							Found->second->Detail = strdup(Detail.c_str());
						}
				
					} else {

						// No such key existed, create a new one.
						//
						Entry * NewEntry = new Entry();

						NewEntry->Key = strdup(Key.c_str());
						NewEntry->Value = strdup(Value.c_str());
						NewEntry->Summary = strdup(Summary.c_str());
						NewEntry->Detail = strdup(Detail.c_str());

						if (IsServer) {
							ServerConfig.insert(std::make_pair(NewEntry->Key, NewEntry));
						} else {
							ClientConfig.insert(std::make_pair(NewEntry->Key, NewEntry));
						}
					}
				}

				Results = _SaveConfig(IsServer);
			}
		}

		return(Results);
	}

	bool
	Configuration::ClientSetConfig(const std::string & Key,
																 const std::string & Value)
	{
		return(_SetNewConfig(false, Key, Value, "", ""));
	}
			
	bool
	Configuration::ServerSetConfig(const std::string & Key,
																 const std::string & Value)
	{
		return(_SetNewConfig(true, Key, Value, "", ""));
	}

	const std::string
	Configuration::ClientGetConfig(const std::string & Key)
	{
		return(_GetConfig(false, Key));
	}

	const std::string
	Configuration::ServerGetConfig(const std::string & Key)
	{
		return(_GetConfig(true, Key));
	}	
	
	const std::string
	Configuration::_GetConfig(bool IsServer, const std::string & Key)
	{
		std::string Results;

		if ((IsServer && ServerInitialized) || (!IsServer && ClientInitialized)) {

			std::map<std::string,Entry*>::const_iterator Found;

			if (IsServer) {
				Found = ServerConfig.find(Key.c_str());
			} else {
				Found = ClientConfig.find(Key.c_str());
			}

			bool WasFound;

			if (IsServer) {
				WasFound = (Found != ServerConfig.cend());
			} else {
				WasFound = (Found != ClientConfig.cend());
			}

			if (WasFound) {
				// Not all keys need or have values.
				//
				if (Found->second->Value != nullptr) {
					Results = Found->second->Value;
				}
			}
		}
		
		return(Results);
	}

	bool
	Configuration::ClientHasConfig(const std::string & Key)
	{
		return(_HasConfig(false, Key));
	}

	bool
	Configuration::ServerHasConfig(const std::string & Key)
	{
		return(_HasConfig(true, Key));
	}
	
	bool
	Configuration::_HasConfig(bool IsServer, const std::string & Key)
	{
		bool Results = false;

		if ((IsServer && ServerInitialized) || (!IsServer && ClientInitialized)) {

			std::map<std::string,Entry*>::const_iterator Found;
			
			if (IsServer) {
				Found = ServerConfig.find(Key.c_str());
			} else {
				Found = ClientConfig.find(Key.c_str());
			}

			bool WasFound;

			if (IsServer) {
				WasFound = (Found != ServerConfig.cend());
			} else {
				WasFound = (Found != ClientConfig.cend());
			}

			if (WasFound) {
				Results = true;
			}
		}

		return(Results);
	}

	static std::string
	MakePath(bool IsServer, const char * HomePath, const char * ProgramName)
	{
		std::string BuildPath;
		
		if (HomePath != nullptr) {
			BuildPath = HomePath;
			
			// Path to the directory where the configuration file exists,
			// under the users home.
			//
			// Linux Path:
			//
			//		~/.config/RiverExplorer/Phoenix/{Client,Server}
			//
			// Windows Path:
			//
			//		(APP_DATA)/RiverExplorer/Phoenix/{Client,Server}
			//
			const char * ClientParts[] = {
#ifndef W64			
				".config",
#endif
				"RiverExplorer",
				"Phoenix",
				"Client",
				nullptr
			};

			const char * ServerParts[] = {
#ifndef W64			
				".config",
#endif
				"RiverExplorer",
				"Phoenix",
				"Server",
				nullptr
			};

			int						POffset = 0;
			const char	*	Part;

			if (IsServer) {
				Part = ServerParts[POffset];
			} else {
				Part = ClientParts[POffset];
			}

			while (Part != nullptr) {
				BuildPath += "/";
				BuildPath += Part;
				
				if (access(BuildPath.c_str(), R_OK|W_OK|X_OK) != F_OK) {
					if (mkdir(BuildPath.c_str(), 0700) != 0) {
						// ERROR
						break;
					}
				}
				if (IsServer) {
					Part = ServerParts[++POffset];
				} else {
					Part = ClientParts[++POffset];
				}
			}
			BuildPath += "/";
			BuildPath += ProgramName;
			mkdir(BuildPath.c_str(), 0700);
			
		}

		return(BuildPath);
	}

	
	void
	Configuration::ClientInitializeConfiguration(const std::string & ClientName)
	{
		_ClientProgramName = strdup(ClientName.c_str());
		_InitializeConfiguration(false);
	}
	
	void
	Configuration::ServerInitializeConfiguration(const std::string & ServerName)
	{
		_ServerProgramName = strdup(ServerName.c_str());
		_InitializeConfiguration(true);
	}

	static void
	MakeHtml(const std::string XmlFile)
	{
		std::string HtmlFile = XmlFile;
		
		HtmlFile = std::regex_replace(HtmlFile, std::regex(".xml"), ".html");

		xmlDoc					* Doc = xmlReadMemory(ConfigToHtml,
																					(int)strlen(ConfigToHtml),
																					"stylesheet",
																					nullptr,
																					0);
		xsltStylesheet	* Xslt = xsltParseStylesheetDoc(Doc);
		xmlDoc					* Xml = xmlParseFile(XmlFile.c_str());
		xmlDoc					*	Html = xsltApplyStylesheet(Xslt, Xml, nullptr);

		xmlSaveFormatFileEnc(HtmlFile.c_str(), Html, Utf8_s, 1);

		xmlFreeDoc(Doc);
		xmlCleanupParser();
		
		return;
	}

	void
	Configuration::_InitializeConfiguration(bool IsServer)
	{
		
#ifndef W64
		struct passwd	*	Pwd;
		
		Pwd = getpwuid(getuid());

		if (Pwd != nullptr) {
			if (IsServer) {
				ServerPath = MakePath(true, Pwd->pw_dir, _ServerProgramName);
			} else {
				ClientPath = MakePath(false, Pwd->pw_dir, _ClientProgramName);
			}
		}
		
#else
		ERROR IMPLEMENT W64;
#endif
		std::string ServerCssPath;

		std::string ClientCssPath;
		
		if (IsServer) {
			ServerPath += "/";
			ServerPath += _ServerProgramName;
			ServerXsltPath = ServerPath;
			ServerHtmlPath = ServerPath;
			ServerCssPath = ServerPath;
			ServerPath += ".xml";
			ServerXsltPath += ".xslt";
			ServerHtmlPath += ".html";
			ServerCssPath += ".css";
		} else {
			ClientPath += "/";
			ClientPath += _ClientProgramName;
			ClientXsltPath = ClientPath;
			ClientHtmlPath = ClientPath;
			ClientCssPath = ClientPath;
			ClientPath += ".xml";
			ClientXsltPath += ".xslt";
			ClientHtmlPath += ".html";
			ClientCssPath += ".css";
		}
		
		// If the XSLT file does not exist, create the default.
		//
		if (IsServer) {
			if (!access(ServerXsltPath.c_str(), R_OK | W_OK) == F_OK) {
				FILE * Fp = fopen(ServerXsltPath.c_str(), "w");

				fprintf(Fp, "%s", ConfigToHtml);
				fclose(Fp);

				Fp = fopen(ServerCssPath.c_str(), "w");
				fclose(Fp);
			}
		} else {
			if (!access(ClientXsltPath.c_str(), R_OK | W_OK) == F_OK) {
				FILE * Fp = fopen(ClientXsltPath.c_str(), "w");

				fprintf(Fp, "%s", ConfigToHtml);
				fclose(Fp);
				
				Fp = fopen(ClientCssPath.c_str(), "w");
				fclose(Fp);
			}
		}

		// If a configuration file exists, and was loadable.
		//
		if (IsServer) {
			if (access(ServerPath.c_str(), R_OK | W_OK) == F_OK) {
				ServerLoaded = _LoadConfig(IsServer);
			}
		} else {
			if (access(ClientPath.c_str(), R_OK | W_OK) == F_OK) {
				ClientLoaded = _LoadConfig(IsServer);
			}
		}

		// If the configuration file did not not exist,
		// or if it could not be parsed.
		//
		bool Loaded;

		if (IsServer) {
			Loaded = ServerLoaded;
		} else {
			Loaded = ClientLoaded;
		}
		
		if (!Loaded) {
			// Create path, if needed.
			//
			// Place it in memory, then create the XML configuration file.
			//
			int EOffset = 0;
			Entry * Ptr;

			for (Ptr = &CommonEntries[EOffset]
						 ; Ptr->Key != nullptr
						 ; Ptr = &CommonEntries[++EOffset]) {

				if (IsServer) {
					ServerConfig.insert(std::make_pair(Ptr->Key, Ptr));
				} else {
					ClientConfig.insert(std::make_pair(Ptr->Key, Ptr));
				}
			}

			if (IsServer) {
				EOffset = 0;
				for (Ptr = &ServerEntries[EOffset]
							 ; Ptr->Key != nullptr
							 ; Ptr = &ServerEntries[++EOffset]) {

					ServerConfig.insert(std::make_pair(Ptr->Key, Ptr));
				}

			} else {
				EOffset = 0;
				for (Ptr = &ClientEntries[EOffset]
							 ; Ptr->Key != nullptr
							 ; Ptr = &ClientEntries[++EOffset]) {

					ClientConfig.insert(std::make_pair(Ptr->Key, Ptr));
				}
			}
			
			//
			_SaveConfig(IsServer);

			if (IsServer) {
				ServerInitialized = true;
			} else {
				ClientInitialized = true;
			}
		}

		return;
	}

	// Only call from _InitialzieConfiguration
	//
	bool
	Configuration::_SaveConfig(bool IsServer)
	{
		bool Results = false;
		
		std::map<std::string, Entry*>::const_iterator It;
		Entry * Ptr;
		
		// Create new configuration file.
		// Delete any old file.
		//
		if (IsServer) {
			unlink(ServerPath.c_str());
		} else {
			unlink(ClientPath.c_str());
		}

		xmlTextWriter * Writer;

		if (IsServer) {
			Writer = xmlNewTextWriterDoc(&ServerDoc, 0);
		} else {
			Writer = xmlNewTextWriterDoc(&ClientDoc, 0);
		}

		xmlTextWriterStartDocument(Writer, nullptr, Utf8_s, nullptr);

		xmlTextWriterStartElement(Writer, (xmlChar*)RiverExplorer_s);
		xmlTextWriterWriteAttribute(Writer,
																(xmlChar*)XmlNs_s,
																(xmlChar*)XmlNsREUrl_s);
		xmlTextWriterStartElement(Writer, (xmlChar*)ConfigNode_s);

		if (IsServer) {
			xmlTextWriterWriteAttribute(Writer,
																	(xmlChar*)ServerXmlNsConfig_s,
																	(xmlChar*)ServerXmlNsConfigUrl_s);
		} else {
			xmlTextWriterWriteAttribute(Writer,
																	(xmlChar*)ClientXmlNsConfig_s,
																	(xmlChar*)ClientXmlNsConfigUrl_s);
		}
		xmlTextWriterWriteAttribute(Writer,
																(xmlChar*)VersionKey_s,
																(xmlChar*)VendorIDValue_s);

		std::map<std::string,Entry*> * Config;

		if (IsServer) {
			Config = &ServerConfig;
		} else {
			Config = &ClientConfig;
		}

		for (It = Config->cbegin(); It != Config->cend(); It++) {
			Ptr = It->second;

			if (Ptr->Key != nullptr) {
				// Write KEY element, as child of Configure.
				//
				xmlTextWriterStartElement(Writer, (xmlChar*)Key_s);
				xmlTextWriterWriteElement(Writer, (xmlChar*)Name_s, (xmlChar*)Ptr->Key);
				if (Ptr->Value != nullptr) {
					xmlTextWriterStartElement(Writer, (xmlChar*)Value_s);
					if (strcmp(VendorIDKey_s, Ptr->Key) == 0) {
						xmlTextWriterWriteAttribute(Writer,
																				(xmlChar*)ReadOnly_s,
																				(xmlChar*)True_s);
					}
					xmlTextWriterWriteString(Writer, (xmlChar*)Ptr->Value);
					xmlTextWriterEndElement(Writer);
				}
				if (Ptr->Summary != nullptr) {
					xmlTextWriterWriteElement(Writer, (xmlChar*)Summary_s,
																		(xmlChar*)Ptr->Summary);
				}
				if (Ptr->Detail != nullptr) {
					xmlTextWriterWriteElement(Writer, (xmlChar*)Detail_s,
																		(xmlChar*)Ptr->Detail);
				}
				xmlTextWriterEndElement(Writer);
			}
		}

		// If the client, save the known servers.
		//
		if (!IsServer) {
			const std::vector<Server*> & KnownServers = GetServers();
			std::vector<Server*>::const_iterator SIt;
			char Buf[128];
			Server * SPtr;
			
			for (SIt = KnownServers.cbegin(); SIt != KnownServers.cend(); SIt++) {
				SPtr = *SIt;
				xmlTextWriterStartElement(Writer, (xmlChar*)Server_s);

				// Position.
				//
				snprintf(Buf, sizeof(Buf), "%u", SPtr->Position());
				xmlTextWriterWriteElement(Writer,
																	(xmlChar*)Position_s,
																	(xmlChar*)Buf);

				// Host or IP
				//
				xmlTextWriterWriteElement(Writer,
																	(xmlChar*)
																	HostOrIp_s,
																	(xmlChar*)SPtr->HostOrIp().c_str());

				// Port
				//
				snprintf(Buf, sizeof(Buf), "%u", SPtr->Port());
				xmlTextWriterWriteElement(Writer,
																	(xmlChar*)Port_s,
																	(xmlChar*)Buf);

				CMD_e AuthMethod = (CMD_e)0xff;
				const char * Account = nullptr;
				const char * Pw = nullptr;

				SPtr->GetAuth(AuthMethod, Account, Pw);

				switch (AuthMethod)	{

				case AUTHANONYMOUS:
					xmlTextWriterWriteElement(Writer,
																		(xmlChar*)Auth_s,
																		(xmlChar*)Anonymous_s);
					break;
					
				case AUTHMD5:
					xmlTextWriterWriteElement(Writer,
																		(xmlChar*)Auth_s,
																		(xmlChar*)MD5_s);

					if (Account != nullptr) {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Account_s,
																			(xmlChar*)Account);
					} else {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Account_s,
																			(xmlChar*)"");
					}
					if (Pw != nullptr) {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Account_s,
																			(xmlChar*)Pw);
					} else {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Account_s,
																			(xmlChar*)"");
					}
					break;
					
				case AUTHCERT_USER:
					if (Account != nullptr) {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Public_s,
																			(xmlChar*)Account);
					} else {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Public_s,
																			(xmlChar*)"");
					}
					if (Pw != nullptr) {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Private_s,
																			(xmlChar*)Pw);
					} else {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Private_s,
																			(xmlChar*)"");
					}
					break;
					
				case AUTHCERT_TLS:
					if (Account != nullptr) {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Public_s,
																			(xmlChar*)Account);
					} else {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Public_s,
																			(xmlChar*)"");
					}
					if (Pw != nullptr) {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Private_s,
																			(xmlChar*)Pw);
					} else {
						xmlTextWriterWriteElement(Writer,
																			(xmlChar*)Private_s,
																			(xmlChar*)"");
					}
					break;

				default:
					/*EMPTY*/
					break;
						
				}

				xmlTextWriterEndElement(Writer); // Server
			}
		}
		
		xmlTextWriterEndElement(Writer); // Close Configure node.
		xmlTextWriterEndElement(Writer); // Close root node.
		xmlFreeTextWriter(Writer);

		if (IsServer) {
			if (xmlSaveFormatFileEnc(ServerPath.c_str(), ServerDoc, Utf8_s, 1) > -1) {
				Results = true;
			}
		} else {
			if (xmlSaveFormatFileEnc(ClientPath.c_str(), ClientDoc, Utf8_s, 1) > -1) {
				Results = true;
			}
		}
		
		if (IsServer) {
			MakeHtml(ServerPath);
		} else {
			MakeHtml(ClientPath);
		}
		
		return(Results);
	}

	// Only call from _LoadConfig.
	//
	void
	Configuration::_ProcessNode(bool IsServer, void * VNode)
	{
		xmlNode	*	Current = (xmlNode*)VNode;
		
		while (Current != nullptr) {
			Entry		*	KeyValue = nullptr;
		
			// See if it is an element node.
			//
			if (Current->type == XML_ELEMENT_NODE) {

				// The Configure node must have a version attribute
				// with a value we understand.
				//
				// All of the Key/Value pairs are children of the
				// Configure node.
				//
				if (strcmp((char*)Current->name, ConfigNode_s) == 0) {

					if (IsServer) {
						ServerLoaded = true;
						ServerHaveConfig = true;
					} else {
						ClientLoaded = true;
						ClientHaveConfig = true;
					}
					
					// Get the Version attribute.
					//
					xmlChar * Version = xmlGetProp(Current, (xmlChar*)VersionKey_s);

					if (Version != nullptr) {
						// This will have to be updated as new
						// versions happen.
						//
						// The RiverExplorer version value (and vendor ID) is:
						//
						//  RiverExplorer.Phoenix.YYYYMMDD.
						//
						const size_t PLen = 22;

						// VendorID
						//
						if (strncmp((char*)Version, VendorIDValue_s, PLen) == 0) {
							if (Version_i == atoi((char*)&Version[PLen])) {
								// Match, looks like we can load the file.
								// And we have the Configure node.
								// Start processing the child nodes.
								//

								/** @todo Do something with matching VendorID. */
							}
						}
					} else {
						// No version attribute.
						// Abort this parse.
						//
						if (IsServer) {
							ServerLoaded = false;
							ServerHaveConfig = false;
						} else {
							ClientLoaded = false;
							ClientHaveConfig = false;
						}
						Current = nullptr;
						continue;
					}

					// Key
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && strcmp((char*)Current->name, Key_s) == 0) {
					KeyValue = new Entry();
					LastEntry = KeyValue;

					// Value
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && strcmp((char*)Current->name, Value_s) == 0) {
					// Get the Key Value.
					//
					xmlChar * Value = xmlNodeGetContent(Current);

					if (Value != nullptr) {
						LastEntry->Value = strdup((char*)Value);
					}

					// Name
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && strcmp((char*)Current->name, Name_s) == 0) {
					// Get the Key Value.
					//
					char * Name = (char*)xmlNodeGetContent(Current);

					if (Name != nullptr) {
						// Create new storage for what we load.
						//
						LastEntry->Key = strdup(Name);
					}

					// Summary
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && LastEntry != nullptr
									 && strcmp((char*)Current->name, Summary_s) == 0) {

					xmlChar * Desc = xmlNodeGetContent(Current);

					if (Desc != nullptr && LastEntry != nullptr) {
						LastEntry->Summary = strdup((char*)Desc);
					}

					// Detail
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && LastEntry != nullptr
									 && strcmp((char*)Current->name, Detail_s) == 0) {

					xmlChar * Desc = xmlNodeGetContent(Current);

					if (Desc != nullptr && LastEntry != nullptr) {
						LastEntry->Detail = strdup((char*)Desc);
					}

					// AUTH
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && LastEntry != nullptr
									 && strcmp((char*)Current->name, Auth_s) == 0) {

					char * Type = (char*)xmlGetProp(Current, (xmlChar*)Type_s);

					LastConfiguration->Auth(Type, nullptr, nullptr);
					
					// Account
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && LastEntry != nullptr
									 && strcmp((char*)Current->name, Account_s) == 0) {

					xmlChar * Value = xmlNodeGetContent(Current);
					LastConfiguration->Account((char*)Value);
				
					// Password
					//
				} else if (((IsServer && ServerHaveConfig)
										|| (!IsServer && ClientHaveConfig))
									 && LastEntry != nullptr
									 && strcmp((char*)Current->name, Password_s) == 0) {

					xmlChar * Value = xmlNodeGetContent(Current);
					LastConfiguration->Password((char*)Value);
				}
				
				// DefaultPublic
				//
			} else if (((IsServer && ServerHaveConfig)
									|| (!IsServer && ClientHaveConfig))
								 && LastEntry != nullptr
								 && strcmp((char*)Current->name, DefaultPublic_s) == 0) {

				xmlChar * Value = xmlNodeGetContent(Current);
				LastConfiguration->DefaultPublicCert((char*)Value);
				
				// DefaultPrivate
				//
			} else if (((IsServer && ServerHaveConfig)
									|| (!IsServer && ClientHaveConfig))
								 && LastEntry != nullptr
								 && strcmp((char*)Current->name, DefaultPrivate_s) == 0) {

				xmlChar * Value = xmlNodeGetContent(Current);
				LastConfiguration->DefaultPrivateCert((char*)Value);
				
				// Host or IP address
				//
			} else if (((IsServer && ServerHaveConfig)
									|| (!IsServer && ClientHaveConfig))
								 && LastEntry != nullptr
								 && strcmp((char*)Current->name, HostOrIp_s) == 0) {

				xmlChar * Value = xmlNodeGetContent(Current);
				LastConfiguration->HostOrIp((char*)Value);
					
				// Port
				//
			} else if (((IsServer && ServerHaveConfig)
									|| (!IsServer && ClientHaveConfig))
								 && LastEntry != nullptr
								 && strcmp((char*)Current->name, Port_s) == 0) {

				xmlChar * Value = xmlNodeGetContent(Current);
				LastConfiguration->Port((char*)Value);
				
				// Position
				//
			} else if (((IsServer && ServerHaveConfig)
									|| (!IsServer && ClientHaveConfig))
								 && LastEntry != nullptr
								 && strcmp((char*)Current->name, Position_s) == 0) {

				xmlChar * Value = xmlNodeGetContent(Current);
				LastConfiguration->Position((char*)Value);
			}
		}

		// Go get next ...
		//
		if (Current->children != nullptr) {
			_ProcessNode(IsServer, Current->children);
		}
		
		return;
	}

	bool
	Configuration::ServerLoadConfig()
	{
		return(_LoadConfig(true));
	}
	
	bool
	Configuration::ClientLoadConfig()
	{
		return(_LoadConfig(false));
	}
	
	// Only call from _InitialzieConfiguration.
	//
	bool
	Configuration::_LoadConfig(bool IsServer)
	{
		xmlNode * Root;
		
		// Load existing configuration file.
		//
		if (IsServer) {
			ServerDoc = xmlParseFile(ServerPath.c_str());
			Root = xmlDocGetRootElement(ServerDoc);
		} else {
			ClientDoc = xmlParseFile(ClientPath.c_str());
			Root = xmlDocGetRootElement(ClientDoc);
		}

		// We only know how to read our own configuration files.
		//
		if (Root != nullptr) {
			if (strcmp((char*)Root->name, RiverExplorer_s) == 0) {

				// Must be in our namespace.
				//
				xmlChar * Ns = xmlGetProp(Root, (xmlChar*)XmlNsRE_s);

				if (Ns != nullptr) {
					if (strcmp((char*)Ns, XmlNsREUrl_s) == 0) {
						if (Root->children != nullptr) {
							_ProcessNode(IsServer, Root->children);
						}
					}
				}
			}
		}		

		if (IsServer) {
			xmlFreeDoc(ServerDoc);
		} else {
			xmlFreeDoc(ClientDoc);
		}
		xmlCleanupParser();

		bool Results = false;

		if (IsServer && ServerLoaded) {
			Results = true;
		} else if (!IsServer && ClientLoaded) {
			Results = true;
		}

		return(Results);
	}

	const std::string
	Configuration::Server::Account() const
	{
		return(_Account);
	}
	
	void
	Configuration::Server::Account(const char * CertValue)
	{
		if (CertValue != nullptr) {
			_Account = CertValue;
		} else {
			_Account = "";
		}

		return;
	}
	
	const std::string
	Configuration::Server::Password() const
	{
		return(_Password);
	}
	
	void
	Configuration::Server::Password(const char * CertValue)
	{
		if (CertValue != nullptr) {
			_Password = CertValue;
		} else {
			_Password = "";
		}

		return;
	}

	void
	Configuration::Server::Auth(const char * AuthMethod,
															const char * Account,
															const char * Password)
	{
		if (AuthMethod != nullptr) {
			if (strcmp(AuthMethod, Anonymous_s) == 0) {
				_Method = AUTHANONYMOUS;
				
			} else if (strcmp(AuthMethod, MD5_s) == 0) {
				_Method = AUTHMD5;
				if (Account != nullptr) {
					_Account = Account;
				} else {
					_Account = "";
				}
				if (Password != nullptr) {
					_Password = Password;
				} else {
					_Password = "";
				}
				
			} else if (strcmp(AuthMethod, AuthCertUser_s) == 0) {
				_Method = AUTHCERT_USER;
				if (Account != nullptr) {
					_PublicCert = Account;
				} else {
					_PublicCert = "";
				}
				if (Password != nullptr) {
					_PrivateCert = Password;
				} else {
					_PrivateCert = "";
				}
				
			} else if (strcmp(AuthMethod, AuthCertTls_s) == 0) {
				_Method = AUTHCERT_TLS;
			}
		}

		return;
	}

	
	void
	Configuration::Server::Auth(CMD_e AuthMethod,
															const char * Account,
															const char * Password)
	{
		switch (AuthMethod) {

		case AUTHANONYMOUS:
			_Method = AuthMethod;
			break;
			
		case AUTHMD5:
			_Method = AuthMethod;
			if (Account != nullptr) {
				_Account = Account;
			} else {
				_Account = "";
			}
			if (Password != nullptr) {
				_Password = Password;
			} else {
				_Password = "";
			}
			break;
				
		case AUTHCERT_USER:
			_Method = AuthMethod;
			if (Account != nullptr) {
				_PublicCert = Account;
			} else {
				_PublicCert = "";
			}
			if (Password != nullptr) {
				_PrivateCert = Password;
			} else {
				_PrivateCert = "";
			}
			break;
			
		case AUTHCERT_TLS:
			_Method = AuthMethod;
			break;

		default:
			_Method = (CMD_e)0xff;
			break;
		}

		return;
	}

	
	void
	Configuration::Server::GetAuth(CMD_e & AuthMethod,
																 const char *& Account,
																 const char *& Password)
	{
		AuthMethod = _Method;

		switch (_Method) {

		case AUTHANONYMOUS:
			Account = nullptr;
			Password = nullptr;
			break;

		case AUTHMD5:
			Account = _Account.c_str();
			Password = _Password.c_str();
			break;

		case AUTHCERT_USER:
			Account = _PublicCert.c_str();
			Password = _PrivateCert.c_str();
			break;

		case AUTHCERT_TLS:
			Account = nullptr;
			Password = nullptr;
			break;

		default:
			/*EMPTY*/
			break;
		}

		return;
	}

	
	const std::string
	Configuration::Server::DefaultPrivateCert() const
	{
		return(_DefaultPrivateCert);
	}
	
	void
	Configuration::Server::DefaultPrivateCert(const char * CertValue)
	{
		if (CertValue != nullptr) {
			_DefaultPrivateCert = CertValue;
		} else {
			_DefaultPrivateCert = "";
		}

		return;
	}
	
	const std::string
	Configuration::Server::DefaultPublicCert() const
	{
		return(_DefaultPublicCert);
	}
	
	void
	Configuration::Server::DefaultPublicCert(const char * CertValue)
	{
		if (CertValue != nullptr) {
			_DefaultPublicCert = CertValue;
		} else {
			_DefaultPublicCert = "";
		}

		return;
	}
	
	const std::string
	Configuration::Server::HostOrIp() const
	{
		return(_HostOrIp);
	}
	
	void
	Configuration::Server::HostOrIp(const char * ConfigValue)
	{
		if (ConfigValue != nullptr) {
			_HostOrIp = ConfigValue;
			
		} else {
			_HostOrIp = "";
		}

		return;
	}

	const std::string
	Configuration::Server::ServerName() const
	{
		return(_ServerName);
	}
	
	void
	Configuration::Server::ServerName(const char * ConfigValue)
	{
		std::map<std::string, Server*>::iterator ItByName;

		if (_ServerName != "") {
			// If we had an old server name, update it in the lookup table.
			//
			ServerMutex.lock();
			ItByName = _Servers.find(_ServerName);

			if (ItByName != _Servers.end()) {
				Server  * Config = ItByName->second;

				_Servers.erase(ItByName);
				_Servers.insert(std::make_pair(ConfigValue, Config));
			}
			ServerMutex.unlock();
		}
		// Update the  value.
		//
		_ServerName = ConfigValue;

		return;
	}

	uint16_t
	Configuration::Server::Port() const
	{
		return(_Port);
	}
	
	void
	Configuration::Server::Port(const char * ConfigValue)
	{
		if (ConfigValue != nullptr) {
			_Port = (uint16_t)atoi(ConfigValue);
		}

		return;
	}

	void
	Configuration::Server::Port(uint16_t Port)
	{
		_Port = Port;

		return;
	}

	uint16_t
	Configuration::Server::Position() const
	{
		return(_Position);
	}
	
	void
	Configuration::Server::Position(const char * ConfigValue)
	{
		if (ConfigValue != nullptr) {
			_Position = (uint16_t)atoi(ConfigValue);
		}

		return;
	}

	void
	Configuration::Server::Position(uint16_t Pos)
	{
		_Position = Pos;

		return;
	}

	Configuration::Server::Server()
	{
		_Position = (uint16_t)-1;
		_Method = (CMD_e) 0xff;
		_Port = 0xff;

		return;
	}

	Configuration::Server::~Server()
	{
		_ServerName = "";
		_Position = (uint16_t)-1;
		_HostOrIp = "";
		_Method = (CMD_e) 0xff;
		_Port = 0xff;
		_Account = "";
		_Password = "";
		_PublicCert = "";
		_PrivateCert = "";

		return;
	}
}
