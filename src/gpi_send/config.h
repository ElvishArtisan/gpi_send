// config.h
//
// Configuration for gpi_send(1)
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

#ifndef CONFIG_H
#define CONFIG_H

#include <QHostAddress>
#include <QList>
#include <QString>

#include <gpiod.h>

#define CONFIG_DEFAULT_CONFIG_FILENAME "/etc/gpi_send.conf"
#define CONFIG_DEFAULT_CHIP_DEVICE "/dev/gpiochip0"
#define CONFIG_DEFAULT_DEBOUNCE_PERIOD 1000
#define CONFIG_DEFAULT_EDGE_TRIGGER "rising"
#define CONFIG_DEFAULT_DESTINATION_ADDRESS "127.0.0.1"
#define CONFIG_DEFAULT_DESTINATION_PORT "1234"

class Config
{
 public:
  Config();
  QString filename() const;
  QString chipDevice() const;
  int debouncePeriod() const;
  enum gpiod_line_edge triggerCondition() const;
  int buttonQuantity() const;
  int buttonOffset(int n) const;
  QHostAddress destinationAddress(int n=-1) const;
  uint16_t destinationPort(int n=-1) const;
  QString command(int n) const;
  bool load(const QString &filename=CONFIG_DEFAULT_CONFIG_FILENAME);
  QString dump();
  void clear();

 private:
  QString d_filename;
  QString d_chip_device;
  int d_debounce_period;
  enum gpiod_line_edge d_trigger_condition;
  QList<int> d_button_offsets;
  QHostAddress d_default_destination_address;
  QList<QHostAddress> d_destination_addresses;
  unsigned d_default_destination_port;
  QList<unsigned> d_destination_ports;
  QStringList d_commands;
};


#endif  // CONFIG_H
