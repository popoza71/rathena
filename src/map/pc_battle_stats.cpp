#include "pc_battle_stats.hpp"

#include "../common/nullpo.hpp"
#include "../common/socket.hpp"
#include "atcommand.hpp"
#include "clif.hpp"
#include "pc.hpp"
#include "battle.hpp"

#include <cstring>

#define CHECK_STATUS_MENU_COUNT 4
#define CS_EQUIPMENT 785
#define CS_STATS     786
#define CS_ZENY      787
#define CS_ITEMLIST  788

// 0442 <Length>.W <count>.L <Skill_list>.W (ZC_SKILL_SELECT_REQUEST).
int pcb_display_menu(map_session_data* sd) {
	nullpo_ret(sd);

	int fd = sd->fd;
	if (!fd)
		return 0;

	uint16 skills[CHECK_STATUS_MENU_COUNT];
	skills[0] = CS_EQUIPMENT;
	skills[1] = CS_STATS;
	skills[2] = CS_ZENY;
	skills[3] = CS_ITEMLIST;

	WFIFOHEAD(fd, 8 + CHECK_STATUS_MENU_COUNT * 2);
	WFIFOW(fd, 0) = 0x442;
	WFIFOW(fd, 2) = 8 + CHECK_STATUS_MENU_COUNT * 2;
	WFIFOL(fd, 4) = CHECK_STATUS_MENU_COUNT;

	for (int i = 0; i < CHECK_STATUS_MENU_COUNT; i++)
		WFIFOW(fd, 8 + i * 2) = skills[i];

	WFIFOSET(fd, WFIFOW(fd, 2));

	// route response ผ่าน skill select handler
	sd->menuskill_id = SC_AUTOSHADOWSPELL;
	sd->menuskill_val = 3;

	sd->state.check_equip_skill = true;
	sd->state.workinprogress = WIP_DISABLE_ALL;

	return 1;
}

void pcb_display(int fd, map_session_data* sd) {
	if (!sd) return;

	const status_data* status = status_get_status_data(*sd);
	char output[CHAT_SIZE_MAX];

#define battlestats_msg(expr) do { (expr); clif_displaymessage(fd, output); } while(0)

	clif_displaymessage(fd, "============ Player Status ============");
	battlestats_msg(sprintf(output, "Name: %s (%s) Lv. %d", sd->status.name, job_name(sd->status.class_), sd->status.base_level));
	clif_displaymessage(fd, "|--- Basic Info ---|");
	battlestats_msg(sprintf(output, "   -> Hp: %d", sd->status.hp));
	battlestats_msg(sprintf(output, "   -> Maxhp: %d", sd->status.max_hp));
	battlestats_msg(sprintf(output, "   -> Sp: %d", sd->status.sp));
	battlestats_msg(sprintf(output, "   -> Maxsp: %d", sd->status.max_sp));
	battlestats_msg(sprintf(output, "   -> Str: %d", sd->status.str));
	battlestats_msg(sprintf(output, "   -> agi: %d", sd->status.agi));
	battlestats_msg(sprintf(output, "   -> Vit: %d", sd->status.vit));
	battlestats_msg(sprintf(output, "   -> Int: %d", sd->status.int_));
	battlestats_msg(sprintf(output, "   -> Dex: %d", sd->status.dex));
	battlestats_msg(sprintf(output, "   -> Luk: %d", sd->status.luk));
	battlestats_msg(sprintf(output, "   -> ATK: %d + %d", pc_leftside_atk(sd), pc_rightside_atk(sd)));
	battlestats_msg(sprintf(output, "   -> MATK: %d + %d", pc_leftside_matk(sd), pc_rightside_matk(sd)));
	battlestats_msg(sprintf(output, "   -> HIT: %d", status->hit));
	battlestats_msg(sprintf(output, "   -> CRIT: %d", status->cri / 10));
	battlestats_msg(sprintf(output, "   -> DEF: %d + %d", pc_leftside_def(sd), pc_rightside_def(sd)));
	battlestats_msg(sprintf(output, "   -> MDEF: %d + %d", pc_leftside_mdef(sd), pc_rightside_mdef(sd)));
	battlestats_msg(sprintf(output, "   -> FLEE: %d + %d", status->flee, status->flee2 / 10));
	battlestats_msg(sprintf(output, "   -> ASPD: %d", (2000 - status->amotion) / 10));

	clif_displaymessage(fd, "|--- Bonus Physical Damages ---|");
	if (sd->right_weapon.addsize[SZ_MEDIUM] + sd->left_weapon.addsize[SZ_MEDIUM]) {
		battlestats_msg(sprintf(output, "   -> Medium Size: %d%%", sd->right_weapon.addsize[SZ_MEDIUM] + sd->left_weapon.addsize[SZ_MEDIUM]));
	}
	if (sd->right_weapon.addsize[SZ_ALL] + sd->left_weapon.addsize[SZ_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Sizes: %d%%", sd->right_weapon.addsize[SZ_ALL] + sd->left_weapon.addsize[SZ_ALL]));
	}
	if (sd->right_weapon.addrace[RC_PLAYER_HUMAN] + sd->left_weapon.addrace[RC_PLAYER_HUMAN]) {
		battlestats_msg(sprintf(output, "   -> Player Human: %d%%", sd->right_weapon.addrace[RC_PLAYER_HUMAN] + sd->left_weapon.addrace[RC_PLAYER_HUMAN]));
	}
	if (sd->right_weapon.addrace[RC_PLAYER_DORAM] + sd->left_weapon.addrace[RC_PLAYER_DORAM]) {
		battlestats_msg(sprintf(output, "   -> Player Doram: %d%%", sd->right_weapon.addrace[RC_PLAYER_DORAM] + sd->left_weapon.addrace[RC_PLAYER_DORAM]));
	}
	if (sd->right_weapon.addrace[RC_ALL] + sd->left_weapon.addrace[RC_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Races: %d%%", sd->right_weapon.addrace[RC_ALL] + sd->left_weapon.addrace[RC_ALL]));
	}
	if (sd->right_weapon.addclass[CLASS_NORMAL] + sd->left_weapon.addclass[CLASS_NORMAL]) {
		battlestats_msg(sprintf(output, "   -> Normal Class: %d%%", sd->right_weapon.addclass[CLASS_NORMAL] + sd->left_weapon.addclass[CLASS_NORMAL]));
	}
	if (sd->right_weapon.addclass[CLASS_ALL] + sd->left_weapon.addclass[CLASS_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Classes: %d%%", sd->right_weapon.addclass[CLASS_ALL] + sd->left_weapon.addclass[CLASS_ALL]));
	}

	clif_displaymessage(fd, "|--- Bonus Magic Damages ---|");
	if (sd->indexed_bonus.magic_addsize[SZ_MEDIUM]) {
		battlestats_msg(sprintf(output, "   -> Medium Size: %d%%", sd->indexed_bonus.magic_addsize[SZ_MEDIUM]));
	}
	if (sd->indexed_bonus.magic_addsize[SZ_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Sizes: %d%%", sd->indexed_bonus.magic_addsize[SZ_ALL]));
	}
	if (sd->indexed_bonus.magic_addrace[RC_PLAYER_HUMAN]) {
		battlestats_msg(sprintf(output, "   -> Player Human: %d%%", sd->indexed_bonus.magic_addrace[RC_PLAYER_HUMAN]));
	}
	if (sd->indexed_bonus.magic_addrace[RC_PLAYER_DORAM]) {
		battlestats_msg(sprintf(output, "   -> Player Doram: %d%%", sd->indexed_bonus.magic_addrace[RC_PLAYER_DORAM]));
	}
	if (sd->indexed_bonus.magic_addrace[RC_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Races: %d%%", sd->indexed_bonus.magic_addrace[RC_ALL]));
	}
	if (sd->indexed_bonus.magic_addclass[CLASS_NORMAL]) {
		battlestats_msg(sprintf(output, "   -> Normal Class: %d%%", sd->indexed_bonus.magic_addclass[CLASS_NORMAL]));
	}
	if (sd->indexed_bonus.magic_addclass[CLASS_ALL]) {
		battlestats_msg(sprintf(output, "   -> All Classes: %d%%", sd->indexed_bonus.magic_addclass[CLASS_ALL]));
	}

	clif_displaymessage(fd, "|--- Elemental Resistance ---|");
	struct {
		const char* format;
		int value;
	} ele_resist_info[] = {
		{ "   -> Neutral: %d%%", sd->indexed_bonus.subele_script[ELE_NEUTRAL] },
		{ "   -> Water: %d%%", sd->indexed_bonus.subele_script[ELE_WATER] },
		{ "   -> Earth: %d%%", sd->indexed_bonus.subele_script[ELE_EARTH] },
		{ "   -> Fire: %d%%", sd->indexed_bonus.subele_script[ELE_FIRE] },
		{ "   -> Wind: %d%%", sd->indexed_bonus.subele_script[ELE_WIND] },
		{ "   -> Poison: %d%%", sd->indexed_bonus.subele_script[ELE_POISON] },
		{ "   -> Holy: %d%%", sd->indexed_bonus.subele_script[ELE_HOLY] },
		{ "   -> Dark: %d%%", sd->indexed_bonus.subele_script[ELE_DARK] },
		{ "   -> Ghost: %d%%", sd->indexed_bonus.subele_script[ELE_GHOST] },
		{ "   -> Undead: %d%%", sd->indexed_bonus.subele_script[ELE_UNDEAD] },
		{ "   -> ALL: %d%%", sd->indexed_bonus.subele_script[ELE_ALL] },
	};
	for (int i = 0; i < ARRAYLENGTH(ele_resist_info); i++) {
		if (ele_resist_info[i].value == 0)
			continue;
		battlestats_msg(sprintf(output, ele_resist_info[i].format, ele_resist_info[i].value));
	}

	clif_displaymessage(fd, "|--- Race Resistance ---|");
	struct {
		const char* format;
		int value;
	} race_resist_info[] = {
		{ "   -> Formless: %d%%", sd->indexed_bonus.subrace[RC_FORMLESS] },
		{ "   -> Undead: %d%%", sd->indexed_bonus.subrace[RC_UNDEAD] },
		{ "   -> Beast: %d%%", sd->indexed_bonus.subrace[RC_BRUTE] },
		{ "   -> Plant: %d%%", sd->indexed_bonus.subrace[RC_PLANT] },
		{ "   -> Insect: %d%%", sd->indexed_bonus.subrace[RC_INSECT] },
		{ "   -> Fish: %d%%", sd->indexed_bonus.subrace[RC_FISH] },
		{ "   -> Demon: %d%%", sd->indexed_bonus.subrace[RC_DEMON] },
		{ "   -> DemiHuman: %d%%", sd->indexed_bonus.subrace[RC_DEMIHUMAN] },
		{ "   -> Angel: %d%%", sd->indexed_bonus.subrace[RC_ANGEL] },
		{ "   -> Dragon: %d%%", sd->indexed_bonus.subrace[RC_DRAGON] },
		{ "   -> Player Human: %d%%", sd->indexed_bonus.subrace[RC_PLAYER_HUMAN] },
		{ "   -> Player Doram: %d%%", sd->indexed_bonus.subrace[RC_PLAYER_DORAM] },
		{ "   -> ALL: %d%%", sd->indexed_bonus.subrace[RC_ALL] },
		{ "   -> Normal Class: %d%%", sd->indexed_bonus.subclass[CLASS_NORMAL] },
	};
	for (int i = 0; i < ARRAYLENGTH(race_resist_info); i++) {
		if (race_resist_info[i].value == 0)
			continue;
		battlestats_msg(sprintf(output, race_resist_info[i].format, race_resist_info[i].value));
	}


#undef battlestats_msg
}

bool pcb_process_selection(map_session_data* sd, int skill) {
	if (!sd) return false;

	char output[CHAT_SIZE_MAX];

	// clear menu state
	sd->state.check_equip_skill = false;
	sd->state.workinprogress = WIP_DISABLE_NONE;
	clif_menuskill_clear(sd);

	map_session_data* tsd = map_id2sd(sd->ce_gid);
	if (!tsd) {
		clif_displaymessage(sd->fd, "Player not found.");
		return false;
	}

	if (skill < CS_EQUIPMENT || skill >= CS_EQUIPMENT + CHECK_STATUS_MENU_COUNT)
		return false;

	switch (skill) {
	case CS_EQUIPMENT:
		if (battle_config.check_stat_bypass || tsd->status.show_equip || pc_has_permission(sd, PC_PERM_VIEW_EQUIPMENT)) {
			clif_viewequip_ack(*sd, *tsd);
		} else {
			clif_msg(*sd, MSI_OPEN_EQUIPEDITEM_REFUSED);
		}
		return true;

	case CS_STATS:
		pcb_display(sd->fd, tsd);
		return true;

	case CS_ZENY:
		// ถ้าเครื่องคุณลิงก์ปัญหา ให้สลับเป็น clif_displaymessage แทน
		clif_messagecolor(sd, color_table[COLOR_CYAN], "============ Check Zeny Info ============", false, SELF);
		sprintf(output, "Zeny: %d", tsd->status.zeny);
		clif_messagecolor(sd, color_table[COLOR_CYAN], output, false, SELF);
		return true;

	case CS_ITEMLIST: {
		char command[CHAT_SIZE_MAX];
		safesnprintf(command, sizeof(command), "%citemlist %s", charcommand_symbol, tsd->status.name);
		is_atcommand(sd->fd, sd, command, 0);
		return true;
	}

	default:
		return false;
	}
}
