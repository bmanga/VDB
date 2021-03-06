#include "DIEVariable.hpp"

#include <stdio.h>

// FOWARD DECLARATION [TODO: REMOVE]
void procmsg(const char* format, ...);

void DIEVariable::onAttributeLoaded(const Dwarf_Attribute &attr,
                                    const Dwarf_Half &attr_code,
                                    const Dwarf_Half &form)
{
	switch (attr_code)
	{
	case DW_AT_name:
	{
		char *cname = 0;
		dwarf_formstring(attr, &cname, 0);
		name = cname;
		procmsg("[DWARF] [DIEVariable] Saving attribute: DW_AT_name (%s)\n", cname);
		break;
	}

	case DW_AT_decl_line:
	{
		Dwarf_Unsigned value;
		dwarf_formudata(attr, &value, 0);
		line_number = value;
		procmsg("[DWARF] [DIEVariable] Saving attribute: DW_AT_decl_line (%d)\n", value);
		break;
	}

	case DW_AT_location:
	{
		Dwarf_Unsigned expr_len = 0;
		Dwarf_Ptr block_ptr = NULL;
		dwarf_formexprloc(attr, &expr_len, &block_ptr, nullptr);
		location_data_length = expr_len;
		location_data = block_ptr;
		procmsg("[DWARF] [DIEVariable] Saving attribute: DW_AT_location (%d byte block)\n", expr_len);
		break;
	}

	case DW_AT_type:
	{
		Dwarf_Off type_offset = 0;
		dwarf_formref(attr, &type_offset, 0);
		this->type_offset = type_offset;
		procmsg("[DWARF] [DIEVariable] Saving attribute: DW_AT_type (0x%llx)\n", type_offset);
		break;
	}

	default:
		procmsg("[DWARF] [DIEVariable] Ignoring attribute...\n");
	}
}