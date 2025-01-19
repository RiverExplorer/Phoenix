/**
 * @mainpage Phoenix
 * @copyright Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer&trade;) is licensed under CC BY 4.0.
 *
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 *
 * <a href="https://RiverExplorer.games">https://RiverExplorer.games</a>
 *
 * <center><h1>Phoenix: Lemonade Risen Again</h1></center>
 *
 * Here you will find the overview of this project (A) Design Overview, (B) Design goals, (C) Sample Implementation design goals. (D) Protocol Overview (E) Protocol Details.
 * 
 * - Overview:
 * 		IMAP and POP are text based protocols developed back when many parts
 * 		of the Internet were 7-bits and slow 1200 to 56k baud connections. 
 * 
 * 		This is an implementation of an 8-bit protocol designed with modern
 * 		Internets speeds.
 * 
 * 		This is divided into two major parts.
 * 
 * 		- The transport of MIME messages to other programs.
 * 			
 * 		- The fetching of MIME messages from some kind of local or remote store.
 * 	</li>
 * 		
 * - Design goals:
 * 		<ul>
 * 			<li>Authentication:
 * 				This protocol leverages on existing authentication methods.</li>
 * 
 * 			<li>Less Chatty:
 * 				POP and IMAP are chatty protocols.
 * 				POP and IMAP are a massive part of the data that flows on
 * 				the Internet.
 * 				This project is designed to optimize the back and forth needed.
 * 			</li>
 * 			<li>Expandable:
 * 				New commands and sub-protocols should be easy to add.
 * 			</li>
 * 					
 * 			<li>Readable:
 * 				This is demo and proof of concept code that is designed
 * 				to be shared as part of a tutorial on this subject.
 * 			</li>
 * 					
 * 			<li>Reuse:
 * 				This does not define any new email headers or values. It is just
 * 				an implementation of a new transport.
 * 				This does not redefine the basic <a href="https://datatracker.ietf.org/doc/html/rfc5321">SMTP</a> protocol.
 * 
 * 				It does provide an extension that signals that XDR requests and replies are available for use.
 * 
 * 				It can use the same ports as existing SMTP, POP, and IMAP servers by extending an implementation to include new a new capability.
 * 			</li>
 * 					
 * 			<li>Migration:
 * 				This implementation provides a migration wrapper for POP, IMAP, SMTP.
 * 				They could be fully replaced with this protocol.
 * 			</li>
 * 					
 * 			<li>Reuseability:
 * 				The MIME message can be email, calendaring, or any other kind of MIME message.
 * 			</li>
 * 					
 * 			<li>MIME encoding:
 * 				By sending 8-bit data, MIME encoding is not addressed.
 * 				So MIME encoding is not needed in the transport.
 * 				This transport treats MIME encoded and non MIME encoded messages the same, as a blob of data.
 * 			</li>
 * 					
 * 			<li>Cross Platform:
 * 				The MIME and EMAIL objects, requests, and replies are transported in eXternmal Data Representation format (XDR <a href="https://datatracker.ietf.org/doc/html/rfc4506">RFC-4506</a>).
 * 			</li>
 * 
 * 			<li>Small device support:
 * 				The client side library needs to be compact and portable.
 * 			</li>
 * 		</ul>
 * 	</li>
 *  - Sample Implementation:
 * 		<ul>
 * 			<li>Server Store Design Goals:
 * 				<ul>
 * 					<li>
 * 						Implementation suitable for small company email systems, with
 * 						expandability for large and massive systems.
 * 					</li>
 * 					<li>File based storage, with a file index</li>
 * 					<li>File based storage, with a file sqllite index</li>
 * 					<li>File based storage, with a file mysql index</li>
 * 					<li>File based storage, with a file postgres index</li>
 * 					<li>Storage compression by only keeping one copy of a message.</li>
 * 					<li>This server side sample implementation is done with Linux.</li>
 * 				</ul>
 * 			</li>
 * 			<li>Client Side Design Goals:
 * 		 		<ul>
 * 					<li>Linux client implementation with support for Thunderbird.</li>
 * 					<li>
 * 						Window client implementation with support for Outlook and Thunderbird.
 * 					</li>
 * 				</ul>
 * 			</li>
 * 		</ul>
 * 	- Protocol Overview:
 * 		<ul>
 * 			<li>Bidirectional Protocol:
 * 				The protocol itself is a bidirectional protocol.
 * 				That is ether endpoint may initiate	a protocol request.
 * 			</li>
 * 			<li>Batching:
 * 				Requests can be batched.
 * 				Multiple commands can be sent in one batch to the other endpoint.
 * 				Multiple replies can be sent in one batch to the other endpoint.
 * 			</li>
 * 			<li>Asynchronous operation:
 * 				Replies to command do not have to be returned in the order
 * 				the command was issued.
 * 			</li>
 * 			<li>Command Identification:
 * 				Each command is tagged with a unique ID that is included
 * 				in each reply.
 * 			<li>Partial results:
 * 				Replies may come in multiple packets. Or all at once.
 * 			</li>
 * 		</ul>
 * 	</li>
 * - Protocol Details:
 * 	</li>
 *
 * @author Douglas Mark Royer (RiverExplorer)
 * @date 2025-JAN-11
 * @version 1.0.0
 */
