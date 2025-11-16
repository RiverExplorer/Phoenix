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
			
	protected:
		/**
		 * This object can not be constructed by itself.
		 * So it is here to prevent the default one from being created.
		 *		 
		 * Contstruct with RangeT()
		 */
		Range() {};
		virtual ~Range() {};
	};
	
	template <typename T>
	class RangeT
		: public Range
	{
	public:

		/**
		 * Range - Constructor
		 */
		RangeT(RangeCmp_e LowCmp, const T Min,
					 RangeCmp_e HighCmp, const T Max)
		{
			_LowCmp = LowCmp;
			_Min = Min;
			_HighCmp = HighCmp;
			_Max = Max;

			return;
		}

		/**
		 * Range - Destructor.
		 */
		virtual ~RangeT() {};

		T Lowest() const {
			return(std::numeric_limits<T>::min());
		}
		
		T Highest() const {
			return(std::numeric_limits<T>::max());
		}
		
		/**
		 * Test if ToTest is a valid value and
		 * within range.
		 */
		bool IsOkay(T & ToTest)
		{
			bool Results = false;
			bool LowOk = false;
			
			switch (_LowCmp) {

			case LessThan_t:
				if (ToTest < _Min) {
					LowOk = true;
				}
				break;

			case LessThanOrEqualTo_t:
				if (ToTest <= _Min) {
					LowOk = true;
				}
				break;

			case EqualTo_t:
				if (ToTest == _Min) {
					LowOk = true;
				}
				break;

			case GreaterThanOrEqualTo_t:
				if (ToTest >= _Min) {
					LowOk = true;
				}
				break;

			case GreaterThan_t:
				if (ToTest > _Min) {
					LowOk = true;
				}
			break;
			}

			if (LowOk) {
				switch (_HighCmp) {

				case LessThan_t:
					if (ToTest < _Max) {
						Results = true;
					}
					break;

				case LessThanOrEqualTo_t:
					if (ToTest <= _Max) {
						Results = true;
					}
					break;

				case EqualTo_t:
					if (ToTest == _Max) {
						Results = true;
					}
					break;

				case GreaterThanOrEqualTo_t:
					if (ToTest >= _Max) {
						Results = true;
					}
					break;

				case GreaterThan_t:
				if (ToTest > _Max) {
					Results = true;
				}
				break;
				}
			}
			return(Results);
		}
		
	protected:
		RangeCmp_e	_LowCmp;
		T						_Min;
		RangeCmp_e	_HighCmp;
		T						_Max;
	};
	
}

#endif // __RIVEREXPLORER_PHOENIX_PROTOCOL_RANGE_HPP_
