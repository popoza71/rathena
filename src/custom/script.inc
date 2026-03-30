// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Script Commands
 * Place the body of custom script commands in this file.
 * Format:
 *	BUILDIN_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//BUILDIN_FUNC(example)
//{
//	ShowInfo("buildin_example: It works!\n");
//	script_pushint(st,1);
//	return 0;
//}

BUILDIN_FUNC(recal_status){
	map_session_data *sd;

	if (!script_rid2sd(sd)) {
		script_pushint(st,0);
		return SCRIPT_CMD_SUCCESS;
	}

	status_calc_pc(sd,SCO_FORCE);
	return SCRIPT_CMD_SUCCESS;
}

/*
	magnus_all(0/1);
	1 = magnus all race
	0 = default behavior
*/
BUILDIN_FUNC(magnus_all)
{
	map_session_data *sd;

	if (!script_rid2sd(sd)) {
		script_pushint(st,0);
		return SCRIPT_CMD_SUCCESS;
	}

	if(script_getnum(st,2) == 1)
		sd->state.magnus_all_race = true;
	else
		sd->state.magnus_all_race = false;

	return SCRIPT_CMD_SUCCESS;
}

/*
	grandcross_nodmg_myself(0/1);
	1 = magnus all race
	0 = default behavior
*/
BUILDIN_FUNC(grandcross_nodmg_myself)
{
	map_session_data *sd;

	if (!script_rid2sd(sd)) {
		script_pushint(st,0);
		return SCRIPT_CMD_SUCCESS;
	}

	if(script_getnum(st,2) == 1)
		sd->state.grandcross_nodmg_myself = true;
	else
		sd->state.grandcross_nodmg_myself = false;

	return SCRIPT_CMD_SUCCESS;
}

// === BEGIN: getmapmoblist ===
BUILDIN_FUNC(getmapmoblist)
{
	class map_session_data* sd = map_id2sd(st->rid);
	if (!sd) return SCRIPT_CMD_FAILURE;

	std::map<int, int> mob_counts;
	struct s_mapiterator* it = mapit_geteachmob();
	TBL_MOB* md;

	while ((md = (TBL_MOB*)mapit_next(it)) != NULL) {
		if (md->m != sd->m) continue;
		// ถ้าอยากนับทุกตัว ให้คอมเมนต์สองบรรทัดนี้ทิ้ง
		// if (md->special_state.ai || md->master_id) continue;
		// if (md->spawn_timer != INVALID_TIMER) continue;
		mob_counts[md->mob_id]++;
	}
	mapit_free(it);

	if (mob_counts.empty()) {
		script_pushstrcopy(st, "");
		return SCRIPT_CMD_SUCCESS;
	}

	std::string result;
	result.reserve(1024);

	bool first = true;
	for (const auto& kv : mob_counts) {
		int mob_id = kv.first;
		int cnt = kv.second;
		auto mob = mob_db.find(mob_id);
		if (!mob) continue;

		char line[256];
		// รูปแบบแน่นอน: Name,ID,Count
		// (ใช้ c_str() เพื่อความชัดเจน)
		snprintf(line, sizeof(line), "%s,%d,%d", mob->jname.c_str(), mob_id, cnt);

		if (!first) result += ":";
		first = false;
		result += line;
	}

	script_pushstrcopy(st, result.c_str());
	return SCRIPT_CMD_SUCCESS;
}
// === END: getmapmoblist ===


// pp autoattack
/*
 * skillstrinfo(<skill id>)
 * Returns the description of a skill.
*/
BUILDIN_FUNC(skillstrinfo)
{
	int skill_id;
	TBL_PC* sd;

	if( !script_rid2sd(sd) )
		return SCRIPT_CMD_SUCCESS;// no player attached, report source

	skill_id = (script_isstring(st, 2) ? skill_name2id(script_getstr(st,2)) : script_getnum(st,2));

	std::shared_ptr<s_skill_db> skill = skill_db.find(skill_id);

	if (!skill)
		return SCRIPT_CMD_FAILURE;

	script_pushstrcopy(st, skill->desc);
	return SCRIPT_CMD_SUCCESS;
}

/*
 * skillintinfo(<id>,<skill_id>)
 * id
 * 1 = Check if skill is available for auto buff
 * 2 = Check if skill is available for auto attack
*/
BUILDIN_FUNC(skillintinfo)
{
	int skill_id, id;
	TBL_PC* sd;
	bool skill_status = false;

	if( !script_rid2sd(sd) )
		return SCRIPT_CMD_SUCCESS; // no player attached, report source

	id = script_getnum(st,2);
	skill_id = script_getnum(st,3);
	std::shared_ptr<s_skill_db> skill;
	e_cast_type type = skill_get_casttype(skill_id);
	skill = skill_db.find(skill_id);

	switch(id){

		case SKILL_INFO_ATTACK: // Check if skill is available for auto buff

			if (!skill)
				return skill_status;

			else if(skill && util::vector_exists(skill_ai_attack, skill_id))
				skill_status = true;
			break;

		case SKILL_INFO_SUPPORT: // Check if skill is available for auto attack

			if (!skill)
				return skill_status;

			else if (skill && util::vector_exists(skill_ai_support, skill_id))
				skill_status = true;
			break;

		case SKILL_INFO_HEAL: // Check if skill is available for auto heal

			if (!skill)
				return skill_status;

			else if (skill && util::vector_exists(skill_ai_heal, skill_id))
				skill_status = true;
			break;
	}

	script_pushint(st, skill_status);
	return SCRIPT_CMD_SUCCESS;
}

/*
 * get info of player on autoattack
 *
 * getautoattackstring(<id>,<value>{,<index>})
 * id
 * 0 = Auto Heal (is_active;skill_id;skill_lv;min_hp)
 * 1 = HP / SP Potions
 * 2 = Auto sit to rest
 * 3 = auto buff skill
 * 4 = auto attack skill
 * 5 = auto buff items
 * 6 = melee attack
 * 7 = teleport
 * 8 = mob
 * 9 = item pickup
 * 10 = skill rate
*/
BUILDIN_FUNC(autoattackstrinfo)
{
	int index = 0, id;
	TBL_PC* sd;
	std::string buf = "";
	std::shared_ptr<s_skill_db> skill;
	std::shared_ptr<item_data> item_data;
	std::shared_ptr<s_mob_db> mob;

	if( !script_rid2sd(sd) )
		return SCRIPT_CMD_SUCCESS;// no player attached, report source

	id = script_getnum(st,2);

	if(script_hasdata(st,3))
		index = script_getnum(st,3);

	char buffer[CHAT_SIZE_MAX]  = { 0 };
	char buffer2[CHAT_SIZE_MAX] = { 0 };

	switch(id){
		case GET_INFO_HEAL:	// heal skills
			if(index == -1){ // Show all buff skills available
				for(int i=0;i<MAX_SKILL;i++){
					if(sd->status.skill[i].id > 0 && sd->status.skill[i].lv > 0){
						skill = skill_db.find(sd->status.skill[i].id);
						if (skill->ai_skill_type&SKILL_TYPE_HEAL){
							safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1621),skill->nameid,skill->desc,sd->status.skill[i].lv);
							buf += buffer;
						}
					}
				}
			} else if(index == 0) { // Show the list of all active buff skill
				if(sd->aa.autoheal.size() > 0){
					for(auto &itAutoheal : sd->aa.autoheal){
						skill = skill_db.find(itAutoheal.skill_id);
						if(skill){
							safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1613),itAutoheal.skill_id,skill->desc,itAutoheal.skill_lv,itAutoheal.min_hp);
							buf += buffer;
						}
					}
				}
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_POTION:	// HP / SP Potions
			if(index == -1) { // Show the list of all potions set
				for(int i = 0; i < MAX_INVENTORY; i++){
					if (sd->inventory.u.items_inventory[i].nameid == 0)
						continue;

					item_data = item_db.find(sd->inventory.u.items_inventory[i].nameid);
					if (item_data == nullptr)
						continue;

					if(item_data->type == IT_HEALING){ // check if item is type healing for potions
						safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1614),item_data->nameid,item_data->name.c_str(),sd->inventory.u.items_inventory[i].amount);
						buf += buffer;
					}
				}
			} else if(index == 0) { // Show the list of all active potions set
				if(sd->aa.autopotion.size() > 0){
					for(auto &itAutopotion : sd->aa.autopotion){
						if( ( item_data = item_db.find(itAutopotion.item_id) ) == NULL )
							break;

						safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1615),itAutopotion.item_id,item_data->name.c_str());
						buf += buffer;
						if(itAutopotion.min_hp){
							safesnprintf(buffer2, sizeof(buffer2), msg_txt(NULL,1616),itAutopotion.min_hp);
							buf += buffer2;
						}
						if(itAutopotion.min_sp){
							safesnprintf(buffer2, sizeof(buffer2), msg_txt(NULL,1617),itAutopotion.min_sp);
							buf += buffer2;
						}
						buf += msg_txt(NULL,1618);
					}
				}
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_SIT:	// Auto sit to rest
			switch(index){
				case 0:
					if(sd->aa.autositregen.is_active)
						buf += msg_txt(NULL,1619);
					else
						buf += msg_txt(NULL,1620);
					break;
				case 1:
					if(sd->aa.autositregen.min_hp){
						safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1629),sd->aa.autositregen.min_hp,sd->aa.autositregen.max_hp);
						buf += buffer;
					}
					break;
				case 2:
					if(sd->aa.autositregen.min_sp){
						safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1629),sd->aa.autositregen.min_sp,sd->aa.autositregen.max_sp);
						buf += buffer;
					}
					break;
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_SUPPORT_SKILL:	// auto buff skill desc
			if(index == -1){ // Show all buff skills available
				for(int i=0;i<MAX_SKILL;i++){
					if(sd->status.skill[i].id > 0 && sd->status.skill[i].lv > 0){
						skill = skill_db.find(sd->status.skill[i].id);
						if (skill->ai_skill_type&SKILL_TYPE_SUPPORT){
							safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1621),skill->nameid,skill->desc,sd->status.skill[i].lv);
							buf += buffer;
						}
					}
				}
			} else if(index == 0) { // Show the list of all active buff skill
				if(sd->aa.autobuffskills.size() > 0){
					for(auto &itAutobuffskills : sd->aa.autobuffskills){
						skill = skill_db.find(itAutobuffskills.skill_id);
						if(skill){
							safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1621),itAutobuffskills.skill_id,skill->desc,itAutobuffskills.skill_lv);
							buf += buffer;
						}
					}
				}
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_ATTACK_SKILL:	// auto attack skill desc
			if(index == -1){ // Show all attack skills available
				for(int i=0;i<MAX_SKILL;i++){
					if(sd->status.skill[i].id > 0 && sd->status.skill[i].lv > 0){
						skill = skill_db.find(sd->status.skill[i].id);
						if (skill->ai_skill_type&SKILL_TYPE_ATTACK){
							safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1621),skill->nameid,skill->desc,sd->status.skill[i].lv);
							buf += buffer;
						}
					}
				}
			} else if(index == 0) { // Show the list of all active attack skill
				if(sd->aa.autoattackskills.size() > 0){
					for(auto &itAutoattackskills : sd->aa.autoattackskills){
						skill = skill_db.find(itAutoattackskills.skill_id);
						if(skill){
							safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1621),itAutoattackskills.skill_id,skill->desc,itAutoattackskills.skill_lv);
							buf += buffer;
						}
					}
				}
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_BUFF_ITEM:	// Buff items
			if(index == 0){
				if(sd->aa.autobuffitems.size()){
					for(auto &itAutobuffitems : sd->aa.autobuffitems){
						if( ( item_data = item_db.find(itAutobuffitems.item_id) ) == NULL )
							break;

						const char* name = nullptr;
						std::string item_name = item_db.create_item_link_for_mes( item_data, true, name );
						safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1633),itAutobuffitems.item_id,item_name.c_str());
						buf += buffer;
					}
				} else {
					safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1634));
					buf += buffer;
				}
			}else if(index == -1){
				if(ai_item_buff.size()){
					std::vector<t_itemid> item_lists = {};

					for(auto &itAiItemBuff : ai_item_buff){
						item_lists.push_back(itAiItemBuff.itemid);
					}

					for (int i = 0; i < MAX_INVENTORY; i++){

						if(sd->inventory.u.items_inventory[i].nameid == 0 || !util::vector_exists(item_lists, sd->inventory.u.items_inventory[i].nameid))
							continue;

						item_data = item_db.find(sd->inventory.u.items_inventory[i].nameid);

						const char* name = nullptr;
						std::string item_name = item_db.create_item_link_for_mes( item_data, true, name );
						safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1633),sd->inventory.u.items_inventory[i].nameid,item_name.c_str());
						buf += buffer;
					}
				}
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		// Melee attack
		case GET_INFO_ATTACK:
			if(!sd->aa.stopmelee)
				buf += msg_txt(NULL,1601);
			else
				buf += msg_txt(NULL,1602);

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_TELEPORT:	// Teleport
			if(!sd->aa.teleport.use_teleport)
				buf += msg_txt(NULL,1603);
			else
				buf += msg_txt(NULL,1604);

			if(!sd->aa.teleport.use_flywing)
				buf += msg_txt(NULL,1605);
			else
				buf += msg_txt(NULL,1606);

			if(!sd->aa.teleport.facing_boss)
				buf += msg_txt(NULL,1640);
			else
				buf += msg_txt(NULL,1641);

			if(sd->aa.teleport.min_hp){
				safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1607), sd->aa.teleport.min_hp);
				buf += buffer;
			}
			if(sd->aa.teleport.delay_nomobmeet){
				safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1608), sd->aa.teleport.delay_nomobmeet/1000);
				buf += buffer;
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_MOB:	// Monsters selection
			if(sd->aa.mobs.aggressive_behavior)
				buf += std::string(msg_txt(NULL,1609));
			else
				buf += std::string(msg_txt(NULL,1610));

			if(sd->aa.mobs.id.size() > 0){
				for (int i=0; i<sd->aa.mobs.id.size(); i++){
					mob = mob_db.find(sd->aa.mobs.id.at(i));
					if (mob == nullptr)
						continue;

					safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1611), mob->id,mob->name.c_str());
					buf += buffer;
				}
			} else {
				safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1612));
				buf += buffer;
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_PICKUP_ITEM:	// Items selection
			switch(sd->aa.pickup_item_config){
				case 0:
					buf += msg_txt(NULL,1624);
					break;
				case 1:
					buf += msg_txt(NULL,1625);
					break;
				case 2:
					buf += msg_txt(NULL,1626);
					break;
			}

			if(sd->aa.pickup_item_id.size()){
				for (int i=0; i<sd->aa.pickup_item_id.size(); i++){
					if( ( item_data = item_db.find(sd->aa.pickup_item_id.at(i)) ) == NULL )
						break;

					safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1627),item_data->nameid,item_data->name.c_str());
					buf += buffer;
				}
			} else
				buf += msg_txt(NULL,1628);

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_SKILL_RATE:
			safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1637),sd->aa.skill_use_rate);
			buf += buffer;

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_FLEE_MOB:
			if(sd->aa.flee_mobs.size()){
				for(const auto &entry : sd->aa.flee_mobs){
					mob = mob_db.find(entry);
					if (mob == nullptr)
						continue;

					safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1611), mob->id, mob->name.c_str());
					buf += buffer;
				}
			} else {
				safesnprintf(buffer, sizeof(buffer), msg_txt(NULL,1645));
				buf += buffer;
			}

			script_pushstrcopy(st, buf.c_str());
			break;

		case GET_INFO_FOCUS_MOB:
			if(sd->aa.focus_mob)
				buf += msg_txt(NULL,1647);
			else
				buf += msg_txt(NULL,1646);

			script_pushstrcopy(st, buf.c_str());
			break;
	}

	return SCRIPT_CMD_SUCCESS;
}

/*
 * Set info of player on autoattack
 *
 * autoattackintinfo(<id>,<value>{,<index>})
 * id
 * 0 = Auto Heal
 * 1 = HP / SP Potions
 * 2 = Auto sit to rest
 * 3 = auto buff skill
 * 4 = auto attack skill
 * 5 = auto buff items
 * 6 = melee attack
 * 7 = teleport
 * 8 = mob
 * 9 = item pickup
 * 10 = skill rate
*/
BUILDIN_FUNC(autoattackintinfo)
{
	int index = 0, id;
	TBL_PC* sd;
	int num = 0, i;
	std::shared_ptr<s_skill_db> skill;
	std::shared_ptr<item_data> item_data;

	if( !script_rid2sd(sd) )
		return SCRIPT_CMD_SUCCESS;// no player attached, report source

	id = script_getnum(st,2);

	if (script_hasdata(st, 3))
		index = script_getnum(st,3);

	switch(id){

		case GET_INFO_HEAL:	// auto heal
			script_pushint(st, sd->aa.autoheal.size());
			break;

		case GET_INFO_POTION: // HP / SP Potions available in inventory
			for(i = 0; i < MAX_INVENTORY; i++){
				if (sd->inventory.u.items_inventory[i].nameid == 0)
					continue;

				item_data = item_db.find(sd->inventory.u.items_inventory[i].nameid);
				if (item_data == nullptr)
					continue;

				if(item_data->type == IT_HEALING) // check if item is type healing for potions
					num++;
			}

			script_pushint(st, num);
			break;

		case GET_INFO_SIT: // Sit to Rest is_active
			script_pushint(st, sd->aa.autositregen.is_active);
			break;

		case GET_INFO_SUPPORT_SKILL: // Active auto buffs skills
			script_pushint(st, sd->aa.autobuffskills.size());
			break;

		case GET_INFO_ATTACK_SKILL: // Active auto attack skills
			script_pushint(st, sd->aa.autoattackskills.size());
			break;

		case GET_INFO_BUFF_ITEM: // Buff items
			script_pushint(st, sd->aa.autobuffitems.size());
			break;

		case GET_INFO_ATTACK: // Melee attack
			script_pushint(st, sd->aa.stopmelee);
			break;

		case GET_INFO_TELEPORT: // Teleport
			switch(index){
				case 0:
					num = sd->aa.teleport.use_teleport;
					break;
				case 1:
					num = sd->aa.teleport.use_flywing;
					break;
				case 2:
					num = sd->aa.teleport.min_hp;
					break;
				case 3:
					num = sd->aa.teleport.delay_nomobmeet;
					break;
				case 4:
					num = sd->aa.teleport.facing_boss;
					break;
			}

			script_pushint(st, num);
			break;

		case GET_INFO_MOB: // Monster list
			switch(index){
				case 0:
					num = sd->aa.mobs.aggressive_behavior;
					break;
				case 1:
					num = static_cast<int32>(sd->aa.mobs.id.size());
					break;
			}

			script_pushint(st, num);
			break;

		case GET_INFO_PICKUP_ITEM: // Item list
			switch(index){
				case 1:
					num = static_cast<int32>(sd->aa.pickup_item_id.size());
					break;
				case 2:
					num = static_cast<int32>(sd->aa.pickup_item_config);
					break;
			}

			script_pushint(st, num);
			break;

		case GET_INFO_SKILL_RATE:
			script_pushint(st, sd->aa.skill_use_rate);
			break;

		case GET_INFO_FLEE_MOB:
			num = static_cast<int32>(sd->aa.flee_mobs.size());
			script_pushint(st, num);
			break;

        case GET_INFO_FLEE_OVERWHELM:
            script_pushint(st, sd->aa.flee_overwhelm);
            break;

		case GET_INFO_FOCUS_MOB:
			script_pushint(st, sd->aa.focus_mob);
			break;

		default:
			script_pushint(st, 0);
			break;
	}

	return SCRIPT_CMD_SUCCESS;
}

/*
 * id = 1 // autopotion (is_active;item_id;min_hp;min_sp)
 * id = 2 // sit regen (is_active;min_hp;max_hp;min_sp;max_sp)
 * id = 3 // auto buff skill (is_active;skill_id;skill_lv)
 * id = 4 // auto attack skill (is_active;skill_id;skill_lv)
 * id = 5 // auto buff items (is_active;item_id;delay)
 * id = 6 // melee attack (is_active)
 * id = 7 // teleport (use_teleport;use_flywing;min_hp;delay_nomobmeet)
 * id = 8 // mob (aggressive_behavior;mob_id)
 * id = 9 // item pickup (item_id;config)
 * id = 10 // skill_rate
*/
BUILDIN_FUNC(autoattackset)
{
	TBL_PC* sd;
	const char delim = ';';
	std::vector<std::string> result;
	std::string item, str;
	int id = -1;

	std::shared_ptr<s_skill_db> skill;
	std::shared_ptr<item_data> item_data;
	std::shared_ptr<s_mob_db> mob;

	if( !script_rid2sd(sd) )
		return SCRIPT_CMD_SUCCESS;// no player attached, report source

	str = script_getstr(st, 2);
	std::stringstream ss(str);

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

	if(result.size() > 0){
		id = std::stoi(result.at(0)); // id
		switch(id){

			case GET_INFO_HEAL:
				if(result.size() == 5){ // id = 0 - autoheal (is_active;skill_id;skill_lv;min_hp)
					struct s_autoheal autoheal = {};
					int skill_id = std::stoi(result.at(2));
					int is_active = std::stoi(result.at(1));

					//check if skill exist and is support type
					skill = skill_db.find(skill_id);
					if (!skill || ~skill->ai_skill_type&SKILL_TYPE_HEAL)
						return SCRIPT_CMD_FAILURE;

					if(!is_active){
						sd->aa.autoheal.erase(
						std::remove_if(sd->aa.autoheal.begin(), sd->aa.autoheal.end(), [skill_id](s_autoheal const &v) {
							return v.skill_id == skill_id;
						}),
						sd->aa.autoheal.end());
						break;
					}
					auto itAutoheal = std::find_if(sd->aa.autoheal.begin(), sd->aa.autoheal.end(), [skill_id] ( s_autoheal const &v) {return v.skill_id == skill_id;});
					if(itAutoheal != sd->aa.autoheal.end()){
						itAutoheal->is_active = is_active;
						itAutoheal->skill_lv = std::stoi(result.at(3));
						itAutoheal->min_hp = std::stoi(result.at(4));
					} else {
						autoheal.is_active = is_active;
						autoheal.skill_id = skill_id;
						autoheal.skill_lv = std::stoi(result.at(3));
						autoheal.min_hp = std::stoi(result.at(4));
						autoheal.last_use = 1;
						sd->aa.autoheal.push_back(autoheal);
					}
				}
				break;

			case GET_INFO_POTION:
				if(result.size() == 5){ // id = 1 - autopotion (is_active;item_id;min_hp;min_sp)
					struct s_autopotion autopotion = {};
					t_itemid nameid = std::stoi(result.at(2));
					int is_active = std::stoi(result.at(1));
					int min_hp = std::stoi(result.at(3));
					int min_sp = std::stoi(result.at(4));

					//check if item exist
					if( ( item_data = item_db.find(nameid) ) == NULL )
						return SCRIPT_CMD_FAILURE;
					if(item_data->type != IT_HEALING) // check if item is type healing for potions
						return SCRIPT_CMD_FAILURE;

					if(!is_active || (min_hp == 0 && min_sp == 0) ){
						sd->aa.autopotion.erase(
						std::remove_if(sd->aa.autopotion.begin(), sd->aa.autopotion.end(), [nameid](s_autopotion const &v) {
							return v.item_id == nameid;
						}),
						sd->aa.autopotion.end());
						break;
					}

					auto itAutopotion = std::find_if(sd->aa.autopotion.begin(), sd->aa.autopotion.end(), [nameid] ( s_autopotion const &v) {return v.item_id == nameid;});
					if(itAutopotion != sd->aa.autopotion.end()){
						itAutopotion->is_active = is_active;
						itAutopotion->min_hp = min_hp;
						itAutopotion->min_sp = min_sp;
					} else {
						autopotion.is_active = is_active;
						autopotion.item_id = nameid;
						autopotion.min_hp = min_hp;
						autopotion.min_sp = min_sp;
						sd->aa.autopotion.push_back(autopotion);
					}
				}
				break;

			case GET_INFO_SIT:
				if(result.size() == 6){ // id = 2 - sit regen (is_active;min_hp;max_hp;min_sp;max_sp)
					int is_active = std::stoi(result.at(1));
					int min_hp = std::stoi(result.at(2));
					int max_hp = std::stoi(result.at(3));
					int min_sp = std::stoi(result.at(4));
					int max_sp = std::stoi(result.at(5));

					if(!is_active)
						sd->aa.autositregen.is_active = false;
					else {
						sd->aa.autositregen.is_active = true;
						sd->aa.autositregen.min_hp = min_hp;
						sd->aa.autositregen.max_hp = max_hp;
						sd->aa.autositregen.min_sp = min_sp;
						sd->aa.autositregen.max_sp = max_sp;
					}
				}
				break;

			case GET_INFO_SUPPORT_SKILL:
				if(result.size() == 4){ // id = 3 - autobuffskills (is_active;skill_id;skill_lv)
					struct s_autobuffskills autobuffskills = {};
					int skill_id = std::stoi(result.at(2));
					int is_active = std::stoi(result.at(1));

					//check if skill exist and is support type and has status change
					skill = skill_db.find(skill_id);
					e_cast_type type = skill_get_casttype(skill_id);
					if (!skill || ~skill->ai_skill_type&SKILL_TYPE_SUPPORT)
						return SCRIPT_CMD_FAILURE;

					if(!is_active){
						sd->aa.autobuffskills.erase(
						std::remove_if(sd->aa.autobuffskills.begin(), sd->aa.autobuffskills.end(), [skill_id](s_autobuffskills const &v) {
							return v.skill_id == skill_id;
						}),
						sd->aa.autobuffskills.end());
						break;
					}
					auto itAutobuffskills = std::find_if(sd->aa.autobuffskills.begin(), sd->aa.autobuffskills.end(), [skill_id] ( s_autobuffskills const &v) {return v.skill_id == skill_id;});
					if(itAutobuffskills != sd->aa.autobuffskills.end()){
						itAutobuffskills->is_active = is_active;
						itAutobuffskills->skill_lv = std::stoi(result.at(3));
					} else {
						autobuffskills.is_active = is_active;
						autobuffskills.skill_id = skill_id;
						autobuffskills.skill_lv = std::stoi(result.at(3));
						autobuffskills.last_use = 1;
						sd->aa.autobuffskills.push_back(autobuffskills);
					}
				}
				break;

			case GET_INFO_ATTACK_SKILL:
				if(result.size() == 4){ // id = 4 - autoattackskills (is_active;skill_id;skill_lv)
					struct s_autoattackskills autoattackskills = {};
					int skill_id = std::stoi(result.at(2));
					int is_active = std::stoi(result.at(1));

					//check if skill exist and is support type and has status change
					skill = skill_db.find(skill_id);
					e_cast_type type = skill_get_casttype(skill_id);
					if (!skill || ~skill->ai_skill_type&SKILL_TYPE_ATTACK)
						return SCRIPT_CMD_FAILURE;

					if(!is_active){
						sd->aa.autoattackskills.erase(
						std::remove_if(sd->aa.autoattackskills.begin(), sd->aa.autoattackskills.end(), [skill_id](s_autoattackskills const &v) {
							return v.skill_id == skill_id;
						}),
						sd->aa.autoattackskills.end());
						break;
					}
					auto itAutoattackskills = std::find_if(sd->aa.autoattackskills.begin(), sd->aa.autoattackskills.end(), [skill_id] ( s_autoattackskills const &v) {return v.skill_id == skill_id;});
					if(itAutoattackskills != sd->aa.autoattackskills.end()){
						itAutoattackskills->is_active = is_active;
						itAutoattackskills->skill_lv = std::stoi(result.at(3));
					} else {
						autoattackskills.is_active = is_active;
						autoattackskills.skill_id = skill_id;
						autoattackskills.skill_lv = std::stoi(result.at(3));
						autoattackskills.last_use = 1;
						sd->aa.autoattackskills.push_back(autoattackskills);
					}
				}
				break;

			case GET_INFO_BUFF_ITEM:
				if(result.size() == 3){ // id = 5 - autobuffitems (is_active;item_id)
					struct s_autobuffitems autobuffitems = {};
					t_itemid nameid = std::stoi(result.at(2));
					int is_active = std::stoi(result.at(1));

					//check if item exist
					if( ( item_data = item_db.find(nameid) ) == NULL )
						return SCRIPT_CMD_FAILURE;

					bool check = false;
					if(ai_item_buff.size()){
						for(auto &itentry : ai_item_buff){
							if(nameid == itentry.itemid)
								check = true;
						}
					}

					if(!check)
						return SCRIPT_CMD_FAILURE;

					if(!is_active){
						sd->aa.autobuffitems.erase(
						std::remove_if(sd->aa.autobuffitems.begin(), sd->aa.autobuffitems.end(), [nameid](s_autobuffitems const &v) {
							return v.item_id == nameid;
						}),
						sd->aa.autobuffitems.end());
						break;
					}
					auto itAutobuffitem = std::find_if(sd->aa.autobuffitems.begin(), sd->aa.autobuffitems.end(), [nameid] ( s_autobuffitems const &v) {return v.item_id == nameid;});
					if(itAutobuffitem != sd->aa.autobuffitems.end()){
						itAutobuffitem->is_active = is_active;
					} else {
						autobuffitems.is_active = is_active;
						autobuffitems.item_id = nameid;
						autobuffitems.last_use = 1;
						sd->aa.autobuffitems.push_back(autobuffitems);
					}
				}
				break;

			case GET_INFO_ATTACK:
				if(result.size() == 2) // id = 6 - melee attack (status)
					sd->aa.stopmelee = std::stoi(result.at(1));
				break;

			case GET_INFO_TELEPORT:
				if(result.size() == 3){ // id = 7 - teleport (index,value)
					int index = std::stoi(result.at(1));
					switch(index){
						case 0:
							sd->aa.teleport.use_teleport = std::stoi(result.at(2));
							break;
						case 1:
							sd->aa.teleport.use_flywing = std::stoi(result.at(2));
							break;
						case 2:
							sd->aa.teleport.min_hp = std::stoi(result.at(2));
							break;
						case 3:
							sd->aa.teleport.delay_nomobmeet = std::stoi(result.at(2))*1000;
							break;
						case 4:
							sd->aa.teleport.facing_boss = std::stoi(result.at(2));
							break;
					}
				}
				break;

			case GET_INFO_MOB:
				if(result.size() == 4){ // id = 8 - monster (is_active;monster_id;aggressive_behavior)
					uint32 mob_id = std::stoi(result.at(2));
					int status = std::stoi(result.at(1));
					bool monster_found = false;
					int i;

					if(status == -1){
						sd->aa.mobs.id.clear();
						break;
					}

					//check if mob exist
					if(mob_id > 0){
						if( ( mob = mob_db.find(mob_id) ) == NULL )
							return SCRIPT_CMD_FAILURE;
					}

					if(sd->aa.mobs.id.size() > battle_config.autoattack_max_moblist)
						break;

					if(!status){
						if(mob_id > 0){
							if(sd->aa.mobs.id.size() > 0){
								for(i=0;i<sd->aa.mobs.id.size(); i++){
									if(sd->aa.mobs.id.at(i) == mob_id){
										sd->aa.mobs.id.erase(sd->aa.mobs.id.begin()+i);
										break;
									}
								}
							}
						} else
							sd->aa.mobs.aggressive_behavior = std::stoi(result.at(3));
					} else if(mob_id > 0 && sd->aa.mobs.id.size() < 20){
						for(i=0;i<sd->aa.mobs.id.size(); i++){
							if(sd->aa.mobs.id.at(i) == mob_id){
								monster_found = true;
								break;
							}
						}
						if(!monster_found)
							sd->aa.mobs.id.push_back(mob_id);
					}
				}
				break;

			case GET_INFO_PICKUP_ITEM:
				if(result.size() == 3){ // id = 9 - item pickup (is_active;item_id)
					int i;
					t_itemid nameid = std::stoi(result.at(2));
					int status = std::stoi(result.at(1));
					bool item_found = false;

					switch(status){
						case -1:
							sd->aa.pickup_item_id.clear();
							break;
						case 0:
							if(!sd->aa.pickup_item_id.empty()){
								for(i=0;i<sd->aa.pickup_item_id.size(); i++){
									if(sd->aa.pickup_item_id.at(i) == nameid){
										sd->aa.pickup_item_id.erase(sd->aa.pickup_item_id.begin()+i);
										break;
									}
								}
							}
							break;
						case 1:
							//check if item exist
							if( ( item_data = item_db.find(nameid) ) == NULL )
								return SCRIPT_CMD_FAILURE;

							if(sd->aa.pickup_item_id.size() >= battle_config.autoattack_max_itemlist)
								break;

							if(!sd->aa.pickup_item_id.empty()){
								for(i=0;i<sd->aa.pickup_item_id.size(); i++){
									if(sd->aa.pickup_item_id.at(i) == nameid){
										item_found = true;
										break;
									}
								}
							}
							if(!item_found)
								sd->aa.pickup_item_id.push_back(nameid);

							break;
						case 2:
							sd->aa.pickup_item_config = (int)nameid;
							break;
					}
				}
				break;

			case GET_INFO_SKILL_RATE:
				if(result.size() == 2){ // id = 10 - skill rate (rate)
					sd->aa.skill_use_rate = std::stoi(result.at(1));
				}
				break;

			case GET_INFO_FLEE_MOB:
				if(result.size() == 3){ // id = 13 - flee mob (is_active;monster_id)
					int i;
					int status = std::stoi(result.at(1));
					uint32 mob_id = std::stoi(result.at(2));
					bool monster_found = false;

					if(sd->aa.flee_mobs.size() >= battle_config.autoattack_max_flee_mob)

					if(mob_id > 0){
						if( ( mob = mob_db.find(mob_id) ) == NULL )
							return SCRIPT_CMD_FAILURE;
					}

					if(!status){
						if(mob_id > 0){
							if(sd->aa.flee_mobs.size() > 0){
								for(i=0;i<sd->aa.flee_mobs.size(); i++){
									if(sd->aa.flee_mobs.at(i) == mob_id){
										sd->aa.flee_mobs.erase(sd->aa.flee_mobs.begin()+i);
										break;
									}
								}
							}
						}
					} else if(mob_id > 0 && sd->aa.flee_mobs.size() < battle_config.autoattack_max_flee_mob){
						if(!util::vector_exists(sd->aa.flee_mobs,mob_id))
							sd->aa.flee_mobs.push_back(mob_id);
					}
				}
				break;

			case GET_INFO_FOCUS_MOB:
				if(result.size() == 2){
					sd->aa.focus_mob = std::stoi(result.at(1));
				}
				break;

            case GET_INFO_FLEE_OVERWHELM:
                // รูปแบบ: autoattackset("24;<threshold>")
                if (result.size() >= 2) {
                    int th = std::stoi(result.at(1));
                    if (th < 0) th = 0;
                    // ตั้งเพดานพื้นฐาน เช่น ไม่เกิน 50 (หรือเพิ่ม battleconf ใหม่ในภายหลัง)
                    if (th > 50) th = 50;
                    sd->aa.flee_overwhelm = th;
                }
                break;
		}
	}

	return SCRIPT_CMD_SUCCESS;
}

/*
*/
BUILDIN_FUNC(autoattackclear)
{
	TBL_PC* sd;

	if( !script_rid2sd(sd) )
		return SCRIPT_CMD_SUCCESS;

	int mode = script_getnum(st,2);

	switch (mode)
	{
		case GET_INFO_MOB:
			sd->aa.mobs.id.clear();
			break;
		case GET_INFO_PICKUP_ITEM:
			sd->aa.pickup_item_id.clear();
			break;
		case GET_INFO_FLEE_MOB:
			sd->aa.flee_mobs.clear();
			break;
	}

	return SCRIPT_CMD_SUCCESS;
}

/// Clears the dialog and continues the script without a next button.
///
/// clear;
BUILDIN_FUNC(clear2)
{
	TBL_PC* sd;

	if (!script_rid2sd(sd))
		return SCRIPT_CMD_FAILURE;

	clif_scriptclear( *sd, st->oid );
	return SCRIPT_CMD_SUCCESS;
}

BUILDIN_FUNC(sc_check)
{
	TBL_PC* sd;

	if (!script_rid2sd(sd))
		return SCRIPT_CMD_FAILURE;

	int type = script_getnum(st, 2);

	if(sd->sc.getSCE((sc_type)type))
		script_pushint(st, 1);
	else
		script_pushint(st, 0);

	return SCRIPT_CMD_SUCCESS;
}

static int sub_pc_send_hattect(struct block_list* bl,va_list ap)
{
	map_session_data *msd;

	nullpo_ret(bl);

	msd=va_arg(ap,map_session_data*);

	map_session_data *sd = (map_session_data *)bl;

	if(!sd)
		return 0;

	if(msd == sd)
		return 0;

	clif_getareachar_unit(sd, msd);

	return 0;
}

int sub_pc_clear_hattect(struct block_list* bl,va_list ap)
{
	map_session_data *msd;

	nullpo_ret(bl);

	msd=va_arg(ap,map_session_data*);

	map_session_data *sd = (map_session_data *)bl;

	if(!sd)
		return 0;

	if(msd == sd)
		return 0;

	clif_getareachar_unit(sd, msd);

	return 0;
}

BUILDIN_FUNC(autostart)
{
	TBL_PC* sd;

	if (!script_rid2sd(sd))
		return SCRIPT_CMD_FAILURE;

	if(map_getmapflag(sd->m, MF_NOAUTOATTACK)){
		clif_showscript(sd, msg_txt(NULL,1635), SELF);
		return SCRIPT_CMD_SUCCESS;
	}

	int mode = script_getnum(st, 2);
	t_tick duration = 0;

	if (script_hasdata(st,3))
		duration = script_getnum(st,3);
	else
		duration = 86400000;

	// start
	if(mode == 1){
		status_change_start(sd, sd, SC_AUTOATTACK, 10000, 1, 0, 0, 0, duration, SCSTART_NOAVOID);
		clif_showscript(sd, msg_txt(NULL, 1631), SELF);
		map_foreachinallrange(sub_pc_send_hattect,sd,AREA_SIZE,BL_PC,sd);
	// stop
	}else{
		status_change_end(sd, SC_AUTOATTACK);
		clif_showscript(sd, msg_txt(NULL,1632), SELF);
		clif_autoattack_effect_off(sd);
		map_foreachinallrange(sub_pc_clear_hattect,sd,AREA_SIZE,BL_PC,sd);
	}
	return SCRIPT_CMD_SUCCESS;
}

/*
 * skillinfocheck(<type>,<skill_id>)
*/
BUILDIN_FUNC(skillinfocheck)
{
	int type,skill_id;

	TBL_PC* sd;

	if( !script_rid2sd(sd) )
		return SCRIPT_CMD_SUCCESS;

	type = script_getnum(st,2);
	skill_id = script_getnum(st,3);

	std::shared_ptr<s_skill_db> skill;
	std::vector<int16> skill_lists = {};

	switch(type){
		case SKILL_INFO_ATTACK:
				for(int i=0;i<MAX_SKILL;i++){
					if(sd->status.skill[i].id > 0 && sd->status.skill[i].lv > 0){
						skill = skill_db.find(sd->status.skill[i].id);
						if (skill && skill->ai_skill_type&SKILL_TYPE_ATTACK){
								skill_lists.push_back(skill->nameid);
						}
					}
				}
			break;

		case SKILL_INFO_SUPPORT:
				for(int i=0;i<MAX_SKILL;i++){
					if(sd->status.skill[i].id > 0 && sd->status.skill[i].lv > 0){
						skill = skill_db.find(sd->status.skill[i].id);
						if (skill && skill->ai_skill_type&SKILL_TYPE_SUPPORT){
								skill_lists.push_back(skill->nameid);
						}
					}
				}
			break;

		case SKILL_INFO_HEAL:
				for(int i=0;i<MAX_SKILL;i++){
					if(sd->status.skill[i].id > 0 && sd->status.skill[i].lv > 0){
						skill = skill_db.find(sd->status.skill[i].id);
						if (skill && skill->ai_skill_type&SKILL_TYPE_HEAL){
								skill_lists.push_back(skill->nameid);
						}
					}
				}
			break;
	}

	if(util::vector_exists(skill_lists,skill_id))
		script_pushint(st, 1);
	else
		script_pushint(st, 0);
	return SCRIPT_CMD_SUCCESS;
}
