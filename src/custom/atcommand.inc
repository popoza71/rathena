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


ACMD_FUNC(reloadrefinepassdb){
	nullpo_retr(-1, sd);

	RefinePassBonusDb.reload();
	RefineCostumePassBonusDb.reload();
	clif_displaymessage(fd, "Refine Pass database has been reloaded.");

	return 0;
}

ACMD_FUNC(reloadpartybonusdb){
	nullpo_retr(-1, sd);

	PartyJobBonusDb.reload();
	PartyCountBonusDb.reload();
	clif_displaymessage(fd, "Party Bonus database has been reloaded."); 

	return 0;
}




/* @populate — population engine (runtime PC shells; alias @fakeplayers) */
/**
 * @reloadpopenginedb — Reload db/population_names.yml, db/population_engine.yml, db/population_chat.yml (does not despawn existing fake PCs).
 */
ACMD_FUNC(reloadpopenginedb)
{
	nullpo_retr(-1, sd);

	uint32_t n = 0;
	if (!population_engine_reload_equipment(&n)) {
		clif_displaymessage(fd, "Population engine equipment reload failed (see map-server console).");
		return -1;
	}

	char buf[128];
	snprintf(buf, sizeof(buf), "Population engine equipment reloaded (%u job profiles).", static_cast<unsigned>(n));
	clif_displaymessage(fd, buf);
	return 0;
}

ACMD_FUNC(populate)
{
	nullpo_retr(-1, sd);

	char cmd[50], param[256];

	if (!message || !*message) {
		clif_displaymessage(fd, "Usage: @populate <quantity> [map]");
		clif_displaymessage(fd, "  Spawns population-engine PCs (visible shells, no autocombat).");
		clif_displaymessage(fd, "  @populate start | stop | stats | status  (alias: @fakeplayers)");
		clif_displaymessage(fd, "  @populate timing | timingreset  (wander tick profiler)");
		return -1;
	}

	const char *msg_ptr = message;
	while (*msg_ptr == ' ' || *msg_ptr == '\t')
		msg_ptr++;
	sscanf(msg_ptr, "%49s %255[^\n]", cmd, param);


	if (strcmpi(cmd, "start") == 0) {
		if (population_engine_is_running()) {
			clif_displaymessage(fd, "Population engine is already running.");
			return -1;
		}
	
		population_engine_autosummon_start();
		clif_displaymessage(fd, "Population engine autosummon started.");
		return 0;
	}
	else if (strcmpi(cmd, "stop") == 0) {
		if (!population_engine_is_running()) {
			clif_displaymessage(fd, "Population engine is not running");
			return -1;
		}
		population_engine_stop();
		clif_displaymessage(fd, "Population engine stopped and cleaned up");
	} else if (strcmpi(cmd, "stats") == 0) {
		PopulationEngineStats stats = population_engine_get_stats();
		const size_t live_count = population_engine_get_count();
		char msg[256];
		clif_displaymessage(fd, "=== Population engine stats ===");
		snprintf(msg, sizeof(msg), "  Active / created / errors : %zu / %u / %u",
			live_count, stats.total_created, stats.errors);
		clif_displaymessage(fd, msg);
		snprintf(msg, sizeof(msg), "  Chat lines emitted        : %u", stats.chat_lines_emitted);
		clif_displaymessage(fd, msg);
		snprintf(msg, sizeof(msg), "  Walk failures             : %u", stats.walk_failures);
		clif_displaymessage(fd, msg);
		snprintf(msg, sizeof(msg), "  Name blocklist retries    : %u", stats.name_retries);
		clif_displaymessage(fd, msg);
		// Memory estimate: each live bot holds one map_session_data allocation.
		// sizeof is a compile-time constant so this is safe in a sprintf context.
		const size_t est_kb = (live_count * sizeof(map_session_data) + 512) / 1024;
		snprintf(msg, sizeof(msg), "  Est. heap (map_session_data x %zu) : ~%zu KB",
			live_count, est_kb);
		clif_displaymessage(fd, msg);
		clif_displaymessage(fd, "================================");
	} else if (strcmpi(cmd, "status") == 0) {
		if (population_engine_is_running())
			clif_displaymessage(fd, "Population engine is RUNNING");
		else
			clif_displaymessage(fd, "Population engine is NOT running");
	} else if (strcmpi(cmd, "timing") == 0) {
		const WanderTimingSnapshot ts = population_engine_path_get_timing_snapshot();
		char msg[256];
		clif_displaymessage(fd, "=== Population wander tick profile ===");
		snprintf(msg, sizeof(msg), "  Ticks recorded / window      : %" PRIu64 " / %" PRIu64,
			ts.ticks_recorded, ts.window_size);
		clif_displaymessage(fd, msg);
		if (ts.window_size == 0) {
			clif_displaymessage(fd, "  (no samples yet \u2014 wait a few seconds)");
		} else {
			snprintf(msg, sizeof(msg), "  Avg total / tick             : %.1f microseconds   (%.2f ms)",
				ts.avg_total_us, ts.avg_total_us / 1000.0);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "    avg map_search_freecell    : %.1f microseconds   (%.1f%%)",
				ts.avg_freecell_us,
				ts.avg_total_us > 0 ? 100.0 * ts.avg_freecell_us / ts.avg_total_us : 0.0);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "    avg fallback path_search   : %.1f microseconds   (%.1f%%)",
				ts.avg_fallback_us,
				ts.avg_total_us > 0 ? 100.0 * ts.avg_fallback_us / ts.avg_total_us : 0.0);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "    avg unit_walktoxy          : %.1f microseconds   (%.1f%%)",
				ts.avg_unit_walktoxy_us,
				ts.avg_total_us > 0 ? 100.0 * ts.avg_unit_walktoxy_us / ts.avg_total_us : 0.0);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "  Avg shells examined/walked   : %.1f / %.1f",
				ts.avg_examined, ts.avg_processed);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "  Last tick total              : %" PRIu64 " microseconds  (proc %u, exam %u)",
				ts.last_total_us, ts.last_processed, ts.last_examined);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "    last freecell/fallback/uw  : %" PRIu64 " / %" PRIu64 " / %" PRIu64 " microseconds",
				ts.last_freecell_us, ts.last_fallback_us, ts.last_unit_walktoxy_us);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "  Worst tick total             : %" PRIu64 " microseconds  (proc %u)",
				ts.worst_total_us, ts.worst_processed);
			clif_displaymessage(fd, msg);
			snprintf(msg, sizeof(msg), "  Calls (cumulative) fc/fb/uw  : %" PRIu64 " / %" PRIu64 " / %" PRIu64,
				ts.freecell_calls, ts.fallback_calls, ts.unit_walktoxy_calls);
			clif_displaymessage(fd, msg);
		}
		clif_displaymessage(fd, "=======================================");
		// pe_perf generic section snapshot (chat/spawn/etc).
		{
			pe_perf::SectionStat sects[pe_perf::kMaxSections];
			size_t n = 0;
			pe_perf::snapshot(sects, n);
			clif_displaymessage(fd, "=== Population perf sections ===");
			if (n == 0) {
				clif_displaymessage(fd, "  (no sections recorded yet)");
			} else {
				char hdr[256];
				snprintf(hdr, sizeof(hdr), "  %-28s %10s %12s %10s %10s %10s",
					"section", "calls", "total_us", "avg_us", "last_us", "worst_us");
				clif_displaymessage(fd, hdr);
				for (size_t i = 0; i < n; ++i) {
					const auto &s = sects[i];
					const double avg = s.calls > 0 ? double(s.total_us) / double(s.calls) : 0.0;
					char row[256];
					snprintf(row, sizeof(row), "  %-28s %10" PRIu64 " %12" PRIu64 " %10.1f %10" PRIu64 " %10" PRIu64,
						s.name ? s.name : "?", s.calls, s.total_us, avg, s.last_us, s.worst_us);
					clif_displaymessage(fd, row);
				}
			}
			clif_displaymessage(fd, "================================");
		}
	} else if (strcmpi(cmd, "timingreset") == 0) {
		population_engine_path_reset_timing();
		pe_perf::reset();
		clif_displaymessage(fd, "Population wander timing reset.");
	} else {
		uint32_t quantity = 0;
		char map_name[32] = "";
		int parsed = sscanf(msg_ptr, "%u %31s", &quantity, map_name);

		if (parsed < 1 || quantity == 0) {
			clif_displaymessage(fd, "Invalid quantity. Usage: @populate <quantity> [map]");
			return -1;
		}
		if (population_engine_is_running()) {
			clif_displaymessage(fd, "Population engine is already running. Stop with @populate stop");
			return -1;
		}
		if (quantity > 5000) {
			clif_displaymessage(fd, "Quantity too large (max 5000)");
			return -1;
		}
		int16_t map_id = sd->m;
		if (map_name[0]) {
			map_id = map_mapname2mapid(map_name);
			if (map_id < 0) {
				clif_displaymessage(fd, "Invalid map name");
				return -1;
			}
		}
		PopulationEngineConfig config;
		config.num_units = quantity;
		config.map_id = map_id;
		config.spread_units = true;
		config.spawn_x = 0;
		config.spawn_y = 0;
		PopulationEngineStats stats;
		if (population_engine_start(config, stats)) {
			char msg[256];
			sprintf(msg, "Population PCs spawned: %u on map %d", stats.total_created, map_id);
			clif_displaymessage(fd, msg);
		} else {
			clif_displaymessage(fd, "Failed to spawn population PCs");
			return -1;
		}
	}
	return 0;
}

