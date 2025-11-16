
#ifndef __RIVEREXPLORER_PROTOCOL_PROTOCOL_HPP__
#define __RIVEREXPLORER_PROTOCOL_PROTOCOL_HPP__

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "Parser.tab.hpp"
//#include "location.hh"

namespace RiverExplorer::Phoenix::Protocol
{

	class Scanner
		: public  yyFlexLexer
	{
	public:
		
		Scanner(std::istream * In) : yyFlexLexer(In)
		{
			Loc = new RiverExplorer::Phoenix::Protocol::Parser::location_type();
		};

		using FlexLexer::yylex;

		virtual int
		yylex(RiverExplorer::Phoenix::Protocol::Parser::semantic_type * const LVal,
					RiverExplorer::Phoenix::Protocol::Parser::location_type * Location);


		inline void
		Error(RiverExplorer::Phoenix::Protocol::Parser::location_type & L,
					const std::string & ErrorMessage)
		{
			std::cerr << "Error: " << ErrorMessage << " at " << L << std::endl;

			return;
		}
		
	private:
		RiverExplorer::Phoenix::Protocol::Parser::semantic_type * yylval = nullptr;

		RiverExplorer::Phoenix::Protocol::Parser::location_type * Loc = nullptr;
		
	};
}

#endif // __RIVEREXPLORER_PROTOCOL_PROTOCOL_HPP__
