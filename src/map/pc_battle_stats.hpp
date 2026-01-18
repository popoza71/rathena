#pragma once

#include "pc.hpp"

int pcb_display_menu(map_session_data * sd);
void pcb_display(int fd, map_session_data * sd);
bool pcb_process_selection(map_session_data * sd, int skill);
