/**
 * RiverExplorer is a trademark of RiverExplorer LLC.
 * This program is free to use as long as this notice is
 * kept intact and visible.
 *
 * Copyright (C) 2025, RiverExplorer LLC
 */
#ifndef __RIVEREXPLORER_PHOENIX_PROTOCOL_RANGE_HPP_
#define __RIVEREXPLORER_PHOENIX_PROTOCOL_RANGE_HPP_

#include <limits>
#include <cstdint>
#include <string>

namespace RiverExplorer::Phoenix::Protocol
{

	/**
	 * This is the base class for any
	 * RangeT() variable.
	 *
	 * It exists solely to be able to create a
	 * pointer to any RangeT() variable.
	 */
	class Range
	{
	public:

		/**
		 * Used in definitions of various Range
		 *
		 * i.e. [Range:>-3,<4] ...
		 */
		enum RangeCmp_e {
			LessThan_t,
			LessThanOrEqualTo_t,
			EqualTo_t,
			GreaterThanOrEqualTo_t,
			GreaterThan_t,
		};
			
		virtual ~Range() {};
		
		std::string ToString(RangeCmp_e C) const
		{
			std::string Results;
			
			switch (C) {

			case LessThan_t:
				Results = "<";
				break;
				
			case LessThanOrEqualTo_t:
				Results = "<=";
				break;
				
			case EqualTo_t:
				Results = "=";
				break;
				
			case GreaterThanOrEqualTo_t:
				Results = ">=";
				break;
				
			case GreaterThan_t:
				Results = ">";
				break;
			}

			return(Results);
		}
		
		std::string ComparerLow()
		{
			return(ToString(LowCmp));
		}
		
		std::string ComparerHigh()
		{
			return(ToString(HighCmp));
		}

		virtual void PrintMin(std::ostream & Out) const = 0;
		virtual void PrintMax(std::ostream & Out) const = 0;
		
		RangeCmp_e	LowCmp;
		RangeCmp_e	HighCmp;

	protected:
		/**
		 * This object can not be constructed by itself.
		 * So it is here to prevent the default one from being created.
		 *		 
		 * Contstruct with RangeT()
		 */
		Range() {};

	};
	
	template <typename T>
	class RangeT
		: public Range
	{
	public:

		/**
		 * Range - Constructor
		 */
		RangeT(RangeCmp_e LowCmpValue, const T MinValue,
					 RangeCmp_e HighCmpValue, const T MaxValue)
		{
			LowCmp = LowCmpValue;
			Min = MinValue;
			HighCmp = HighCmpValue;
			Max = MaxValue;

			return;
		}

		/**
		 * Range - Destructor.
		 */
		virtual ~RangeT() {};

		/**
		 * Test if ToTest is a valid value and
		 * within range.
		 */
		bool IsOkay(T & ToTest)
		{
			bool Results = false;
			bool LowOk = false;
			
			switch (LowCmp) {

			case LessThan_t:
				if (ToTest < Min) {
					LowOk = true;
				}
				break;

			case LessThanOrEqualTo_t:
				if (ToTest <= Min) {
					LowOk = true;
				}
				break;

			case EqualTo_t:
				if (ToTest == Min) {
					LowOk = true;
				}
				break;

			case GreaterThanOrEqualTo_t:
				if (ToTest >= Min) {
					LowOk = true;
				}
				break;

			case GreaterThan_t:
				if (ToTest > Min) {
					LowOk = true;
				}
			break;
			}

			if (LowOk) {
				switch (HighCmp) {

				case LessThan_t:
					if (ToTest < Max) {
						Results = true;
					}
					break;

				case LessThanOrEqualTo_t:
					if (ToTest <= Max) {
						Results = true;
					}
					break;

				case EqualTo_t:
					if (ToTest == Max) {
						Results = true;
					}
					break;

				case GreaterThanOrEqualTo_t:
					if (ToTest >= Max) {
						Results = true;
					}
					break;

				case GreaterThan_t:
				if (ToTest > Max) {
					Results = true;
				}
				break;
				}
			}
			return(Results);
		}

		virtual void PrintMin(std::ostream & Out) const override {
			Out << Min;
		}
		
		virtual void PrintMax(std::ostream & Out) const override {
			Out << Max;
		}
		
		T						Min;
		T						Max;
	};

	std::ostream & operator<<(std::ostream & Out,
														const Range & R);
	
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_RANGE_HPP_
