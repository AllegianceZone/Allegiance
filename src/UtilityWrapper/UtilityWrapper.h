////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// BT - 7/15
//
// Used by CSS Management web to generate file checksums using the same algorithm that Allegiance uses.
// I tried several different implementation of CRC32 libs out there, and arrived at the conclusion that
// the one Allegiance uses is non-standard. So, this wrapper exposes that funcationality to .Net
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// UtilityWrapper.h
#include <msclr\marshal.h>

#pragma once

using namespace System;

namespace UtilityWrapper {

	public ref class CRCUtils
	{
		public:
			int GetCrc32ForFile(System::String^ filename, System::String^ %errorMessage)
			{
				char szErrorMessage[1024];
				//char szFilename[1024];

				msclr::interop::marshal_context context;

				//marshal_context context;
				const char *  szFilename = context.marshal_as<const char*>(filename);

				int returnValue = FileCRC(szFilename, szErrorMessage);

				errorMessage = gcnew System::String(szErrorMessage);
				return returnValue;
			}
	};
}
