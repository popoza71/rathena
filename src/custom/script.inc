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

