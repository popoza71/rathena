// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config structure
 * Place the definition of custom battle configuration structure members in this file.
 *
 * Make sure that the name you want is not already taken by looking into battle.hpp
 * Do not forget to add initilization logic to battle_config_init.inc
 *
 * Format:
 *	 <datatype> name;
 **/
 	int32 check_stat_bypass;
	int32 sc_buff_effect_boss;

	int32 mapmvp_enabled;
	int32 mapmvp_miniboss_enabled;

	int32 charm_refine_enable;	// Charm Refine
	int32 pet_refine_enable;	// Pet Refine

	int32 party_bonus_system_enable;
	int32 party_bonus_same_map_minimum;
	int32 party_bonus_update_delay;
	int32 party_bonus_system_count_enable;

	// MvP and Mini-Boss Card Ad and Log System [Bad]
	int32 announcement_and_log_system;
	int32 mvp_card_announce_system;
	int32 mvp_card_announce;
	int32 dropped_mvp_card_log;
	int32 set_drop_announce_color;
	int32 mini_boss_card_announce_system;
	int32 mini_boss_card_announce;
	int32 dropped_mini_boss_card_log;
	int32 set_drop_card_announce_color;

	//[puppy] RefineUI Options
	int32 refine_announce_success;
	int32 refine_announce_broken;
	int32 refine_option_announce;
	int32 refine_option_color;
	int32 refine_option_color_self;
	int32 refine_broken_color;
	int32 refine_normal_color;
	int32 blacksmith_downgrade;

	int32 refine_pass_system_enable;
	int32 refine_costume_pass_system_enable;
	int32 costume_head_refine_def;	// puppy cos refine

	int32 trickdead_noexp;

	int32 asura_absorb_cast_cancel;
	int32 asura_dispell_cast_cancel;
	int32 block_body_relocation_ankle;
	int32 block_body_relocation_spiderweb;
	int32 block_back_slide_ankle;
	int32 block_back_slide_spiderweb;
	int32 block_high_jump_ankle;
	int32 block_high_jump_spiderweb;
	int32 blitz_damage_divide;
	int32 sonic_blow_autoattack;
	int32 bowling_bash_multi_hit;
	int32 bowlingbash_reduce_dmg_not_2hsw;
	int32 berserk_provork_lv;
	int32 endure_hit;
	int32 gank_percent;

 	//Max Trap [Hyroshima]
	int32 skill_max_trap;
	int32 global_max_trap;

	//re-newal skill edit
	int32 sanctuary_disable;
	int32 bowling_bash_min_count_re;
	int32 bowling_bash_max_count_re;
	int32 holy_cross_2spear_weapon;
	int32 sonic_acceleration_dmg;
	int32 backstab_dagger_hit;

	int32 char_bonus_debug;
	int32 char_bonus_same_job_limit;

	int32 show_shadow_weapon;
	int32 tick_dead_get_item;
	
	// auto attack
	int32 autoattack_skillheal;
	int32 autoattack_item_potion;
	int32 autoattack_sittorest;
	int32 autoattack_skill_attack;
	int32 autoattack_skill_buff;
	int32 autoattack_item_buff;
	int32 autoattack_teleport;
	int32 autoattack_teleport_by_skill;
	int32 autoattack_item_pickup;
	int32 autoattack_mselection;
	int32 autoattack_item_range_detection;
	int32 autoattack_mob_detection;
	int32 autoattack_move;
	int32 autoattack_move_type;
	int32 autoattack_interval_timer;
	int32 autoattack_pickup_delay;
	int32 autoattack_clean_logout;
	int32 autoattack_hateffect;
	int32 autoattack_skill_rate_default;
	int32 autoattack_reduce_droprate;
	int32 autoattack_reduce_mode;
	int32 autoattack_max_moblist;
	int32 autoattack_max_itemlist;
	int32 autoattack_steal_reduce_rate;
	int32 autoattack_max_time_per_mob;
	int32 autoattack_mob_struck;
	int32 autoattack_quiver_check;
	int32 autoattack_max_flee_mob;
	int32 autoattack_gunslinger_conflip_fix;
	int32 autoattack_coinflip_coin;
	int32 autoattack_chaincombo_finnish;

	
	int32 population_engine_max_count;
	/// 0 = warn and fix invalid YAML rows; 1 = discard entire equipment DB if any validation error
	int32 population_engine_equipment_strict_load;
	/// Phase 3 chat (db/population_chat.yml + ChatProfile on equipment rows)
	int32 population_engine_chat_enable;
	int32 population_engine_chat_tick_ms;
	int32 population_engine_chat_cooldown_ms;
	int32 population_engine_chat_cooldown_jitter_ms;
	int32 population_engine_chat_max_per_tick;
	/// 1 = population PCs reply when whispered or when a player says their name in map chat (same map)
	int32 population_engine_chat_reply_enable;
	/// Phase 4 wander: random walk on map timer (no autocombat)
	int32 population_engine_wander_enable;
	int32 population_engine_wander_tick_ms;
	int32 population_engine_wander_radius;
	int32 population_engine_wander_cooldown_ms;
	int32 population_engine_wander_cooldown_jitter_ms;
	int32 population_engine_wander_max_per_tick;
	/// ZC_EQUIPMENT_EFFECT hat id for population PC shells when starting autocombat (same idea as HatEffect in db/autocombat_config.yml). 0 = use autocombat HatEffect instead.
	int32 population_engine_hat_effect;
	/// SC_POPULATION_COMBAT shell (population_engine_combat.cpp): movement roam / tick / target scan -- independent of db/autocombat_config.yml
	int32 population_engine_shell_move_min;
	int32 population_engine_shell_move_max;
	/// Global combat timer interval (ms). All bots share one timer; budget_per_tick controls
	/// how many are processed per fire. Replaces per-bot SC_POPULATION_COMBAT reschedule.
	int32 population_engine_shell_timer_ms;
	/// Max bots processed per global combat timer tick. 60 bots/100ms = 600/s -> each of
	/// 300 bots ticks ~2x/s. Raise to process more bots per fire at the cost of larger spikes.
	int32 population_engine_combat_budget_per_tick;
	int32 population_engine_shell_mobtracker_interval_ms;
	int32 population_engine_shell_mdetection_cells;
	int32 population_engine_shell_detection_cache_ms;
	int32 population_engine_shell_movement_owner_sm;
	int32 population_engine_shell_attack_skill_delay_ms;
	int32 population_engine_shell_attackskill; // like autocombat_attackskill: 0 = melee only, 1 = use pop_shell attack list
	/// Per-tick chance (0-100) the shell uses a basic attack instead of evaluating the skill picker.
	/// 0 = always try a skill (legacy: lots of high-Rate skills monopolise every tick = no melee).
	/// 30 = ~30% of attack ticks are forced basic-attack regardless of skill rotation rates.
	/// Bypassed when PSF::SkillOnly is set on the shell.
	int32 population_engine_shell_basic_attack_chance;
	/// SP-reserve floor (percent of max SP, 0-99). Skill picker skips skills whose SP cost would
	/// drop the shell's SP below this percentage. 0 = no floor (legacy behavior -- can cast to 0 SP).
	int32 population_engine_shell_skill_min_sp_pct;
	/// Sticky-target commitment window in ms. After acquiring a target, the shell prefers it
	/// over equidistant alternatives until this many ms elapse, the target dies, or it leaves
	/// detection range. 0 = no stickiness (legacy: pure nearest-mob each tick).
	int32 population_engine_shell_sticky_target_ms;
	/// Pre-cast LOS gate: 0 = legacy (only check on emit), 1 = require path_search_long
	/// (no-wall line) from shell to target before picking a non-self skill in the rotation.
	/// Saves SP/cast attempts that would fail at unit_skilluse_id() against blocked targets.
	int32 population_engine_shell_skill_los_check;
	/// Strict skill-use gating: 0 = only skill_isNotOk() (legacy), 1 = also require
	/// status_check_skilluse() to pass before selecting a skill (covers silence/sleep/sit/etc.).
	int32 population_engine_shell_skill_strict_gate;
	/// 0 = never produce Bot_<id> names (deterministic syllable fallback); 1 = allow Bot_<id> when pool is exhausted
	int32 population_engine_name_bot_fallback;
	/// CPU ceiling: max bots processed per global combat timer tick. Dynamic budget = min(N, this value).
	/// At default 200, all bots tick every timer_ms up to N=200; above that, each bot ticks every (N/200)*timer_ms.
	int32 population_engine_combat_max_budget;
	/// Phase 3 two-tier timer: minimum ms between skill evaluation passes (0 = evaluate every tick).
	/// Movement happens every timer_ms. Skills evaluated every max(timer_ms, skill_interval_ms).
	/// Example: timer_ms=100, skill_interval_ms=500 -> movement 10x/s, skills 2x/s per bot.
	int32 population_engine_shell_skill_interval_ms;
	/// Max shells spawned per autosummon timer tick across all profiles and maps combined.
	/// 0 = unlimited (spawn all missing shells in one tick). Raise to spread load over multiple 10s ticks.
	int32 population_engine_autosummon_batch_size;
	/// AI behavior bitmask (like monster_ai). See conf/battle/population_engine.conf for flag docs.
	int32 population_engine_ai;
	/// Daily cycle: 0=off, 1=on. When enabled, town shells switch behavior by server hour:
	///   06:00-17:59 = configured behavior, 18:00-21:59 = Social, 22:00-05:59 = Sit.
	///   Field/dungeon shells always use their configured behavior.
	int32 population_engine_daily_cycle;
	/// Vending economy: 0=off, 1=on. Vendor-behavior shells open a vend with common items at Buy price.
	int32 population_engine_vending_enable;
	/// Arena PvP job pool: bitmask-encoded or count-based arbitrary pool is not supported in int32.
	/// Instead store the jobs as a packed array via the count field; actual IDs stored in a companion
	/// global in population_engine.cpp. This field signals that a custom pool was configured (1=yes,0=no).
	int32 population_engine_arena_custom_pool;
	int32 population_engine_path_attempts;


	int32 feature_goldpc_active;
	int32 feature_goldpc_time;
	int32 feature_goldpc_max_points;
	int32 feature_goldpc_vip;
