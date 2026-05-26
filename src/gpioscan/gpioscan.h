// gpioscan.h
//
// Emit network messages in response to GPI events
//
//   (C) Copyright 2026 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef GPI_SEND_H
#define GPI_SEND_H

#include <QObject>

#include <gpiod.h>

#define USAGE "--device=<dev-name> [--device=<dev-name-2>] [...]\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject();

 private:
  void ListChipInfo(struct gpiod_chip *chip);
  void ListChipLine(struct gpiod_chip *chip,unsigned offset);
  struct gpiod_chip *d_chip;
  struct gpiod_chip_info *d_info;
  struct gpiod_line *d_lines;
};


#endif  // GPI_SEND_H
