// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//ACMD_FUNC(newcommand)
//{
//	clif_displaymessage(fd, "It works!");
//	clif_specialeffect(&sd->bl, EF_HEARTCASTING, AREA);
//	return 0;
//}


/*==========================================
 * @mapmob by KAN Channel
 *------------------------------------------*/
ACMD_FUNC(mapmob)
{
	nullpo_retr(-1, sd);

	std::map<int, int> mob_counts;
	struct s_mapiterator* it = mapit_geteachmob();
	TBL_MOB* md;
	bool found = false;

	while ((md = (TBL_MOB*)mapit_next(it)) != NULL) {
		if (md->m != sd->m)
			continue;
		if (md->special_state.ai || md->master_id)
			continue; // เธเนเธญเธ summon/pet/slave
		if (md->spawn_timer != INVALID_TIMER)
			continue; // เธขเธฑเธเนเธกเน spawn

		mob_counts[md->mob_id]++;
		found = true;
	}
	mapit_free(it);

	clif_displaymessage(fd, "--------Monster List--------");

	if (!found) {
		clif_displaymessage(fd, "No Monster In This Map");
		clif_displaymessage(fd, "------------------------------");
		return 0;
	}

	for (const auto& pair : mob_counts) {
		int mob_id = pair.first;
		int count = pair.second;

		std::shared_ptr<s_mob_db> mob = mob_db.find(mob_id);
		if (!mob)
			continue;

		char output[256];
		if (mob->status.mode & MD_STATUSIMMUNE) {

			sprintf(output, "%s (%d) (MVP) : %d", mob->jname.c_str(), mob_id, count);
		}
		else {
			sprintf(output, "%s (%d) : %d", mob->jname.c_str(), mob_id, count);
		}
		clif_displaymessage(fd, output);
	}
	clif_displaymessage(fd, "------------------------------");
	return 0;
}
//puppy mapmob

/*==========================================
 * @itemmap command (usage: @itemmap <itemid> <quantity>)
 *------------------------------------------*/
//puppy
static int buildin_itemmap_sub(block_list* bl, va_list ap) {
	int nameid, amount, bound;

	nameid = va_arg(ap, int);
	amount = va_arg(ap, int);
	bound = va_arg(ap, int);

	struct item item_tmp = {};

	item_tmp.nameid = nameid;
	item_tmp.identify = 1;
	item_tmp.bound = bound;

	pc_additem((map_session_data*)bl, &item_tmp, amount, LOG_TYPE_COMMAND);
	return 0;
}

/*==========================================
 * @itemmap command (usage: @itemmap <itemid> <quantity>)
 *------------------------------------------*/
ACMD_FUNC(itemmap)
{
	char item_name[100];
	int number = 0, bound = BOUND_NONE;
	char flag = 0;

	nullpo_retr(-1, sd);
	memset(item_name, '\0', sizeof(item_name));

	parent_cmd = atcommand_alias_db.checkAlias(command + 1);

	if (!strcmpi(parent_cmd, "itemmapbound")) {
		if (!message || !*message || (
			sscanf(message, "\"%99[^\"]\" %11d %11d", item_name, &number, &bound) < 3 &&
			sscanf(message, "%99s %11d %11d", item_name, &number, &bound) < 3))
		{
			clif_displaymessage(fd, "(usage: @itemmap <itemid> <quantity>)"); // Please enter an item name or ID (usage: @item <item name/ID> <quantity> <bound_type>).
			clif_displaymessage(fd, msg_txt(sd, 298)); // Invalid bound type
			return -1;
		}
		if (bound <= BOUND_NONE || bound >= BOUND_MAX) {
			clif_displaymessage(fd, msg_txt(sd, 298)); // Invalid bound type
			return -1;
		}
	}
	else if (!message || !*message || (
		sscanf(message, "\"%99[^\"]\" %11d", item_name, &number) < 1 &&
		sscanf(message, "%99s %11d", item_name, &number) < 1
		)) {
		clif_displaymessage(fd, msg_txt(sd, 983)); // Please enter an item name or ID (usage: @item <item name/ID> <quantity>).
		return -1;
	}

	std::shared_ptr<item_data> item_data = item_db.search_aegisname(item_name);

	if (item_data == nullptr)
		item_data = item_db.find(strtoul(item_name, nullptr, 10));

	if (item_data == nullptr)
		return -1;

	map_foreachinmap(buildin_itemmap_sub, sd->m, BL_PC, item_data->nameid, number, bound);
	return 0;
}



