/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_DEFAULTVALUE_HPP_
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_DEFAULTVALUE_HPP_

#include <limits>
#include <cstdint>
#include <string>

namespace RiverExplorer::Phoenix::Protocol
{
	/**
	 * This is the base class for any
	 * DefaultValueT() variable.
	 *
	 * It exists solely to be able to create a
	 * pointer to any DefaultValueT() variable.
	 */
	class DefaultValue
	{
	public:
		
		virtual ~DefaultValue() {};

		virtual void Print(std::ostream & /*Out*/) const = 0;
		
	protected:

		/**
		 * This object can not be constructed by itself.
		 * So it is here to prevent the default one from being created.
		 *		 
		 * Contstruct with DefaultValueT()
		 */
		DefaultValue() {};

	};
		
	template <typename T>
	class DefaultValueT
		: public DefaultValue
	{
	public:
			
		/**
		 * Default - Constructor
		 */
		DefaultValueT(T TheValue)
			: Value(TheValue)
		{
			Value = TheValue;
			return;
		}
		
		/**
		 * Default - Destructor.
		 */
		virtual ~DefaultValueT() {};

		T						Value;

	protected:
		
		virtual void Print(std::ostream & Out) const override {
			Out << Value;
		}

	};

	std::ostream & operator<<(std::ostream & Out,
														const DefaultValue & S);
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_DEFAULTVALUE_HPP_
