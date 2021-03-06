#include "CUHeader.hpp"

// FOWARD DECLARATION [TODO: REMOVE]
void procmsg(const char* format, ...);

CUHeader::CUHeader(const Dwarf_Debug &dbg)
{
	Dwarf_Die no_die = 0, cu_die;
	Dwarf_Error err;

	// Expect the CU to have a single sibling - a DIE.
	if (dwarf_siblingof(dbg, no_die, &cu_die, &err) == DW_DLV_ERROR)
		procmsg("[DWARF_ERROR] Error getting sibling of CU! %s\n", dwarf_errmsg(err));

	// Initialize the root DIE and assign its internal type
	root_die = std::make_unique<DIECompileUnit>(dbg, cu_die, nullptr);
	root_die->init();
}

// Recursive method to get all children (below the specified DIE) in the DIE
// hierarchy and place them in a vector.
void CUHeader::getChildrenOf(
	std::shared_ptr<DebuggingInformationEntry> root_die,
	SharedPtrVector<DebuggingInformationEntry> &children)
{
	for (auto &child_die : root_die->getChildren())
	{
		children.push_back(child_die);
		getChildrenOf(child_die, children);
	}
}