// =============================================================================
// Copyright (C) 2013    Marcos Medeiros
//
// This file is part of "SGEH - Sistema Gerador de Erro Humano"
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
// =============================================================================
#ifndef GLOBALIDS_H
#define GLOBALIDS_H

// Os 1000 primeiros ids são reservados pelo Qt
enum {
    GEH_CTRL_LABEL = 1000,
    GEH_CTRL_BUTTON,
    GEH_CTRL_EDITBOX,
    GEH_CTRL_CHECKBOX,
    GEH_CTRL_RADIOBUTTON,
    GEH_CTRL_GROUPBOX,
    GEH_CTRL_COMBOBOX,

    GEH_CMDID_RESIZEFORM,
    GEH_CMDID_MOVE,
};

#endif // GLOBALIDS_H
