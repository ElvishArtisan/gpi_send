// gpisend.h
//
// Send network messages in response to GPIO events
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

#ifndef GPISEND_H
#define GPISEND_H

#include <QObject>
#include <QUdpSocket>

#include <gpiod.h>

#include "config.h"

#define USAGE "[-d] [--dump-config]\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject();

 private:
  void EventLoop();
  void InitializeGpio();
  struct gpiod_chip *d_chip;
  struct gpiod_chip_info *d_info;
  struct gpiod_line_config *d_line_config;
  struct gpiod_line_settings *d_line_settings;
  struct gpiod_request_config *d_request_config;
  struct gpiod_line_request *d_line_request;
  struct gpiod_edge_event_buffer *d_edge_event_buffer;
  void InitializeNetworking();
  QUdpSocket *d_send_socket;
  Config *d_config;
};


#endif  // GPISEND_H
