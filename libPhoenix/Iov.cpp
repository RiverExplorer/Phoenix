/**
 * Phoenix (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer) is licensed under CC BY 4.0.
 * RiverExplorer is a trademark of RiverExplorer Games LLC.
 */

#include "Iov.hpp"
#include "PhoenixEvent.hpp"
#include <deque>
#include <sys/mman.h>
#include <memory.h>

namespace RiverExplorer::Phoenix
{
	Iov::Iov()
	{
		return;
	}

	Iov::~Iov()
	{
		Clear();

		return;
	}

	void
	Iov::Add(uint8_t * Data, uint64_t Len, bool IsMMapped)
	{
		Blob * NewBlob = new Blob(Data, Len, IsMMapped);

		_Q.push_back(NewBlob);

		return;		
	}

	void
	Iov::Add(uint64_t Value)
	{
		uint64_t  * Data = new uint64_t(Value);
		
		Blob * NewBlob = new Blob((uint8_t*)Data, sizeof(uint64_t), false);

		_Q.push_back(NewBlob);

		return;		
	}

	Iov::Blob::Blob(uint8_t * NewData, uint64_t NewLen, bool NewIsMMapped)
	{
		if (NewData != nullptr && NewLen > 0) {
			OriginalData = NewData;
			Data = NewData;
			Length = NewLen;
			IsMMapped = NewIsMMapped;
			
		} else {
			Data = nullptr;
			Length = 0;
			IsMMapped = false;
		}

		return;
	}

	Iov::Blob::~Blob()
	{
		if (IsMMapped) {
			if (OriginalData != nullptr && Length > 0) {
				munmap(OriginalData, Length);
				
			} else {
				// Should never get executed.
				//
				PhoenixEvent::InvokeMessage(-1,
																		PhoenixEvent::LogWarning_s,
																		"Iov.cpp:WARNING:MMapped segment, with data or length 0\n");
			}

		} else {
			if (OriginalData != nullptr && Length > 0) {
				memset(OriginalData, 0, Length); // Security;
				delete OriginalData;
			}
		}
		OriginalData = nullptr;
		Data = nullptr;
		Length = 0;
		IsMMapped = false;

		return;
	}

	void
	Iov::Clear()
	{
		std::deque<Blob*>::iterator BIt;
		Blob * Ptr;

		for (BIt = _Q.begin(); BIt != _Q.end(); BIt++) {
			Ptr = _Q.front();
			_Q.pop_front();

			if (Ptr != nullptr) {
				delete Ptr;
			}
		}

		return;
	}
	
	uint8_t *
	Iov::Take(uint64_t & Len)
	{
		uint8_t * Results = nullptr;

		Blob * B = _Q.front();

		Results = B->Data;
		Len = B->Length;
		
		return(Results);
	}

	void
	Iov::WeUsed(uint64_t Used)
	{
		Blob * B = _Q.front();

		if (B != nullptr) {
			if (B->Length == Used) {
				_Q.pop_front();

			} else {
				B->Data = B->OriginalData + Used;

				if (B->Data > B->OriginalData + B->Length) {
					// OOPS, someone returned more than we had - error..
					//
					PhoenixEvent::InvokeMessage(-1,
																			PhoenixEvent::LogError_s,
																			"Iov.cpp::WeUsed():More returned that existed.");
					_Q.pop_front(); // The best hack we can use.
				}
			}
		} else {
			// OOPS, someone returned more than we had - error..
			//
			PhoenixEvent::InvokeMessage(-1,
																	PhoenixEvent::LogError_s,
																	"Iov.cpp::WeUsed():Nothing existed to return unused.");
		}

		return;
	}
	
}


