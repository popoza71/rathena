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
	int32 berserk_provork_lv;
	int32 endure_hit;
	int32 gank_percent;

 	//Max Trap [Hyroshima]
	int32 skill_max_trap;
	int32 global_max_trap;

	/*
	int32 bowling_bash_min_count_re;
	int32 bowling_bash_max_count_re;
	int32 holy_cross_2spear_weapon;
	int32 sonic_acceleration_dmg;
	int32 backstab_dagger_hit;
	int32 bowlingbash_reduce_dmg_not_2hsw;
	//int32 sub_ele_capping;
	int32 sanctuary_disable;
	//int32 shadow_weapon_view;
	//int32 shadow_weapon_allow_empty_hand;
	//int32 shadow_weapon_fallback;
	*/
