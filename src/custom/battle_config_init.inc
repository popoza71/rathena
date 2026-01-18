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

	//[puppy] RefineUI Options
	{ "refine_announce_success",      		&battle_config.refine_announce_success,   		0,      0,      MAX_REFINE,		},
	{ "refine_announce_broken",      		&battle_config.refine_announce_broken,   		0,      0,      MAX_REFINE,		},
	{ "refine_option_announce",      		&battle_config.refine_option_announce,   		0,      0,      1,              },
	{ "refine_option_color",      			&battle_config.refine_option_color,		 0xff0000,      0,      INT_MAX,		},
	{ "refine_option_color_self",			&battle_config.refine_option_color_self, 0xff0000,      0,      INT_MAX,		},
	{ "refine_broken_color",				&battle_config.refine_broken_color, 	 0xff0000,      0,      INT_MAX,		},
	{ "refine_normal_color",				&battle_config.refine_normal_color, 	 0xff0000,      0,      INT_MAX,		},
	{ "blacksmith_downgrade",				&battle_config.blacksmith_downgrade,			0,		0,		1,				},


	{ "trickdead_noexp",						&battle_config.trickdead_noexp,					1,      0,      1, },


	/*
 	////Max Trap [Hyroshima]
	//{ "skill_max_trap",						 &battle_config.skill_max_trap,                  1,      0,      INT_MAX,        },
	//{ "global_max_trap",						&battle_config.global_max_trap,                 1,      0,      INT_MAX,        },

	{ "asura_absorb_cast_cancel",				&battle_config.asura_absorb_cast_cancel,        1,      0,      1, },
	{ "asura_dispell_cast_cancel",				&battle_config.asura_dispell_cast_cancel,       1,      0,      1, },
	{ "block_body_relocation_ankle",			&battle_config.block_body_relocation_ankle,		1,      0,      1, },
	{ "block_body_relocation_spiderweb",		&battle_config.block_body_relocation_spiderweb, 1,      0,      1, },
	//{ "block_body_relocation_confine",		&battle_config.block_body_relocation_confine,   1,      0,      1, },
	{ "block_back_slide_ankle",					&battle_config.block_back_slide_ankle,			1,      0,      1, },
	{ "block_back_slide_spiderweb",				&battle_config.block_back_slide_spiderweb,      1,      0,      1, },
	{ "block_high_jump_ankle",					&battle_config.block_high_jump_ankle,			1,      0,      1, },
	{ "block_high_jump_spiderweb",				&battle_config.block_high_jump_spiderweb,       1,      0,      1, },
	{ "endure_hit",								&battle_config.endure_hit,						7,      0,      INT_MAX, },
	{ "gank_percent",							&battle_config.gank_percent,					0,		0,		100 },	//puppy
	//{ "magnus_all_element",					&battle_config.magnus_all_element,				1,      0,      1,				}, //puppy
	{ "blitz_damage_divide",					&battle_config.blitz_damage_divide,				1,      0,      1, },
	{ "sonic_blow_autoattack",					&battle_config.sonic_blow_autoattack,			1,      0,      1, },
	{ "bowling_bash_multi_hit",					&battle_config.bowling_bash_multi_hit,			1,      0,      1, },
	{ "bowling_bash_min_count_re",				&battle_config.bowling_bash_min_count_re,		2,      0,      INT_MAX, },	//puppy
	{ "bowling_bash_max_count_re",				&battle_config.bowling_bash_max_count_re,		4,      0,      INT_MAX, },	//puppy
	{ "holy_cross_2spear_weapon",				&battle_config.holy_cross_2spear_weapon,		45,      0,      INT_MAX, },	//puppy
	{ "sonic_acceleration_dmg",					&battle_config.sonic_acceleration_dmg,			90,      0,      INT_MAX, },	//puppy
	{ "backstab_dagger_hit",					&battle_config.backstab_dagger_hit,				2,      0,      INT_MAX, },	//puppy
	{ "pvp_mvp_map",							&battle_config.pvp_mvp_map,						1,      0,      1, },
	//{ "broadcast_sound",						&battle_config.broadcast_sound,					1,      0,      1, },
	//{ "sc_buff_effect_boss",					&battle_config.sc_buff_effect_boss,				0,      0,      1, },
	{ "berserk_provork_lv",						&battle_config.berserk_provork_lv,				10,     0,      INT_MAX, },	//puppy
	{ "bowlingbash_reduce_dmg_not_2hsw",		&battle_config.bowlingbash_reduce_dmg_not_2hsw,	100,     0,     INT_MAX, },	//puppy
	//{ "sub_ele_capping",						&battle_config.sub_ele_capping,					100,     0,     INT_MAX, },	//puppy
	{ "sanctuary_disable",						&battle_config.sanctuary_disable,				1,      0,      1, },	//puppy
	//{ "shadow_weapon_view",					&battle_config.shadow_weapon_view,				1,      0,      1, },	//puppy
	//{ "shadow_weapon_allow_empty_hand",		&battle_config.shadow_weapon_allow_empty_hand,	0,		0,		1 },	//puppy
	//{ "shadow_weapon_fallback",				&battle_config.shadow_weapon_fallback,			0,		0,		1, },	//puppy
	//{ "blacksmith_downgrade",					&battle_config.blacksmith_downgrade,			0,		0,		1, },	//puppy
	*/
