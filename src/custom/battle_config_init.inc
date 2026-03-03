// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config initializations
 * Place the definition of custom battle configuration initializations in this file.
 *
 * Do not forget to add a comma at the end of the line, if you want to add another config
 *
 * Format:
 *	 { "name", &battle_config.<variable name>, <default value>, <minimum value>, <maximum value> },
 **/
	{ "check_stat_bypass",					&battle_config.check_stat_bypass,				0,		0,		1				},
	{ "sc_buff_effect_boss",				&battle_config.sc_buff_effect_boss,				0,      0,      1,				},

	{ "mod.mapmvp_enabled",                 &battle_config.mapmvp_enabled,                  1,      0,      1,              },
	{ "mod.mapmvp_miniboss_enabled",        &battle_config.mapmvp_miniboss_enabled,         0,      0,      1,              },

	{ "charm_refine_enable",				&battle_config.charm_refine_enable,	0,	0,	1,	},	// Charm Refine
	{ "pet_refine_enable",					&battle_config.pet_refine_enable,	0,	0,	1,	},	// Pet Refine

	{ "party_bonus_system_enable",			&battle_config.party_bonus_system_enable,		0,	0,	1,	},
	{ "party_bonus_same_map_minimum",		&battle_config.party_bonus_same_map_minimum,	1,	0,	MAX_PARTY,	},
	{ "party_bonus_update_delay",			&battle_config.party_bonus_update_delay,	  500,	0,	INT_MAX,	},
	{ "party_bonus_system_count_enable",	&battle_config.party_bonus_system_count_enable,		0,	0,	1,	},

	// MvP and Mini-Boss Card Ad and Log System [Bad]
	{ "announcement_and_log_system",        &battle_config.announcement_and_log_system,     1,      0,      1,              },
	{ "mvp_card_announce_system",           &battle_config.mvp_card_announce_system,        1,      0,      1,              },
	{ "mvp_card_announce",                  &battle_config.mvp_card_announce,               1,      0,      1,              },
	{ "dropped_mvp_card_log",               &battle_config.dropped_mvp_card_log,            1,      0,      1,              },
	{ "set_drop_announce_color",            &battle_config.set_drop_announce_color,         0xFFBB00,      0,INT_MAX,       },
	{ "mini_boss_card_announce_system",     &battle_config.mini_boss_card_announce_system,  1,      0,      1,              },
	{ "mini_boss_card_announce",            &battle_config.mini_boss_card_announce,         1,      0,      1,              },
	{ "dropped_mini_boss_card_log",         &battle_config.dropped_mini_boss_card_log,      1,      0,      1,              },
	{ "set_drop_card_announce_color",       &battle_config.set_drop_card_announce_color,    0xFFBB00,      0,INT_MAX,       },

	//[puppy] RefineUI Options
	{ "refine_announce_success",      		&battle_config.refine_announce_success,   		0,      0,      MAX_REFINE,		},
	{ "refine_announce_broken",      		&battle_config.refine_announce_broken,   		0,      0,      MAX_REFINE,		},
	{ "refine_option_announce",      		&battle_config.refine_option_announce,   		0,      0,      1,              },
	{ "refine_option_color",      			&battle_config.refine_option_color,		 0xff0000,      0,      INT_MAX,		},
	{ "refine_option_color_self",			&battle_config.refine_option_color_self, 0xff0000,      0,      INT_MAX,		},
	{ "refine_broken_color",				&battle_config.refine_broken_color, 	 0xff0000,      0,      INT_MAX,		},
	{ "refine_normal_color",				&battle_config.refine_normal_color, 	 0xff0000,      0,      INT_MAX,		},
	{ "blacksmith_downgrade",				&battle_config.blacksmith_downgrade,			0,		0,		1,				},

	{ "refine_pass_system_enable",			&battle_config.refine_pass_system_enable,		0,	0,	1,			},
	{ "refine_costume_pass_system_enable",	&battle_config.refine_costume_pass_system_enable, 0, 0, 1, },
	{ "costume_head_refine_def",			&battle_config.costume_head_refine_def,		    1,	0,	1,			},	// puppy cos refine

	{ "trickdead_noexp",						&battle_config.trickdead_noexp,					1,      0,      1, },

	{ "asura_absorb_cast_cancel",				&battle_config.asura_absorb_cast_cancel,        1,      0,      1, },
	{ "asura_dispell_cast_cancel",				&battle_config.asura_dispell_cast_cancel,       1,      0,      1, },
	{ "block_body_relocation_ankle",			&battle_config.block_body_relocation_ankle,		1,      0,      1, },
	{ "block_body_relocation_spiderweb",		&battle_config.block_body_relocation_spiderweb, 1,      0,      1, },
	{ "block_back_slide_ankle",					&battle_config.block_back_slide_ankle,			1,      0,      1, },
	{ "block_back_slide_spiderweb",				&battle_config.block_back_slide_spiderweb,      1,      0,      1, },
	{ "block_high_jump_ankle",					&battle_config.block_high_jump_ankle,			1,      0,      1, },
	{ "block_high_jump_spiderweb",				&battle_config.block_high_jump_spiderweb,       1,      0,      1, },
	{ "blitz_damage_divide",					&battle_config.blitz_damage_divide,				1,      0,      1, },
	{ "sonic_blow_autoattack",					&battle_config.sonic_blow_autoattack,			1,      0,      1, },
	{ "bowling_bash_multi_hit",					&battle_config.bowling_bash_multi_hit,			1,      0,      1, },
	{ "bowlingbash_reduce_dmg_not_2hsw",		&battle_config.bowlingbash_reduce_dmg_not_2hsw,	100,     0,     INT_MAX, },	//puppy
	{ "berserk_provork_lv",						&battle_config.berserk_provork_lv,				10,     0,      INT_MAX, },	//puppy
	{ "endure_hit",								&battle_config.endure_hit,						7,      0,      INT_MAX, },
	{ "gank_percent",							&battle_config.gank_percent,					0,		0,		100 },	//puppy

 	//Max Trap [Hyroshima]
	{ "skill_max_trap",							&battle_config.skill_max_trap,                  1,      0,      INT_MAX,        },
	{ "global_max_trap",						&battle_config.global_max_trap,                 1,      0,      INT_MAX,        },

	//re-newal skill edit
	{ "sanctuary_disable",						&battle_config.sanctuary_disable,				1,      0,      1, },	//puppy
	{ "bowling_bash_min_count_re",				&battle_config.bowling_bash_min_count_re,		2,      0,      INT_MAX, },	//puppy
	{ "bowling_bash_max_count_re",				&battle_config.bowling_bash_max_count_re,		4,      0,      INT_MAX, },	//puppy
	{ "holy_cross_2spear_weapon",				&battle_config.holy_cross_2spear_weapon,		45,      0,      INT_MAX, },	//puppy
	{ "sonic_acceleration_dmg",					&battle_config.sonic_acceleration_dmg,			90,      0,      INT_MAX, },	//puppy
	{ "backstab_dagger_hit",					&battle_config.backstab_dagger_hit,				2,      0,      INT_MAX, },	//puppy

	{ "char_bonus_debug",						&battle_config.char_bonus_debug,        		0,      0,      1,              },
	
	{ "show_shadow_weapon",						&battle_config.show_shadow_weapon,				0, 0, 1, },

	//autoattack
	{ "autoattack_skillheal",				&battle_config.autoattack_skillheal,			1,		0,		1,				},
	{ "autoattack_item_potion",				&battle_config.autoattack_item_potion,			1,		0,		1,				},
	{ "autoattack_sittorest",				&battle_config.autoattack_sittorest,			1,		0,		1,				},
	{ "autoattack_skill_attack",			&battle_config.autoattack_skill_attack,			1,		0,		1,				},
	{ "autoattack_skill_buff",				&battle_config.autoattack_skill_buff,			1,		0,		1,				},
	{ "autoattack_item_buff",				&battle_config.autoattack_item_buff,			1,		0,		1,				},
	{ "autoattack_teleport",				&battle_config.autoattack_teleport,				1,		0,		1,				},
	{ "autoattack_item_pickup",				&battle_config.autoattack_item_pickup,			1,		0,		1,				},
	{ "autoattack_mselection",				&battle_config.autoattack_mselection,			1,      1,      1,      		},
	{ "autoattack_item_range_detection",	&battle_config.autoattack_item_range_detection,	15,		0,		30,				},
	{ "autoattack_mob_detection",			&battle_config.autoattack_mob_detection,		15,     1,      30,      		},
	{ "autoattack_move",					&battle_config.autoattack_move,					7,      1,      20,      		},
	{ "autoattack_move_type",				&battle_config.autoattack_move_type,			0,		0,		1,				},
	{ "autoattack_interval_timer",			&battle_config.autoattack_interval_timer,		100,	0,		INT_MAX,		},
	{ "autoattack_pickup_delay",			&battle_config.autoattack_pickup_delay,			100,	0,		INT_MAX,		},
	{ "autoattack_clean_logout",			&battle_config.autoattack_clean_logout,			0,		0,		1,				},
	{ "autoattack_hateffect",				&battle_config.autoattack_hateffect,			0,		0,		INT_MAX,		},
	{ "autoattack_skill_rate_default",		&battle_config.autoattack_skill_rate_default,	0,		0,		100,			},
	{ "autoattack_reduce_droprate",			&battle_config.autoattack_reduce_droprate,		0,		0,		100,  			},
	{ "autoattack_reduce_mode",				&battle_config.autoattack_reduce_mode,			0,		0,		1|2|4|8|16|32,  },
	{ "autoattack_max_moblist",				&battle_config.autoattack_max_moblist,			0,		0,		INT_MAX,		},
	{ "autoattack_max_itemlist",			&battle_config.autoattack_max_itemlist,			0,		0,		INT_MAX,		},
	{ "autoattack_steal_reduce_rate",		&battle_config.autoattack_steal_reduce_rate,	0,		0,		100,			},
	{ "autoattack_max_time_per_mob",		&battle_config.autoattack_max_time_per_mob,		0,		0,		INT_MAX,		},
	{ "autoattack_mob_struck",				&battle_config.autoattack_mob_struck,			0,		0,		INT_MAX,		},
	{ "autoattack_quiver_check",			&battle_config.autoattack_quiver_check,			0,		0,		INT_MAX,		},
	{ "autoattack_max_flee_mob",			&battle_config.autoattack_max_flee_mob,			0,		0,		INT_MAX,		},
	{ "autoattack_gunslinger_conflip_fix",	&battle_config.autoattack_gunslinger_conflip_fix,	0,		0,		1,			},
	{ "autoattack_coinflip_coin",			&battle_config.autoattack_coinflip_coin,		0,		0,		INT_MAX,		},
	{ "autoattack_chaincombo_finnish",		&battle_config.autoattack_chaincombo_finnish,	1,		0,		1,				},
