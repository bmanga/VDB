#include "DebugInfo.hpp"

// FOWARD DECLARATION [TODO: REMOVE]
void procmsg(const char* format, ...);

DebugInfo::DebugInfo(Dwarf_Debug dbg)
{	
	// Load the compilation unit headers
	loadCUHeaders(dbg);
}

void DebugInfo::loadCUHeaders(Dwarf_Debug dbg)
{
	Dwarf_Error err;
	int result = DW_DLV_OK;
	while (result == DW_DLV_OK)
	{
		Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
		Dwarf_Half version_stamp, address_size;
		Dwarf_Error err;
		result = dwarf_next_cu_header(
	            dbg,
		        &cu_header_length,
		        &version_stamp,
		        &abbrev_offset,
		        &address_size,
		        &next_cu_header,
		        &err);

		if (result == DW_DLV_OK)
		{
	        std::shared_ptr<CUHeader> header(new CUHeader(dbg));
			header->length = cu_header_length;
			header->version_stamp = version_stamp;
			header->abbrev_offset = abbrev_offset;
			header->address_size = address_size;
			header->next_cu_header = next_cu_header;

			headers.push_back(std::move(header));
        }
	}

	// Check in case of error loading
	if (result == DW_DLV_ERROR)
		procmsg("[DWARF_ERROR] Error loading CU headers!\n");
}

std::vector<std::shared_ptr<CUHeader>> &DebugInfo::getCUHeaders()
{
	return headers;
}