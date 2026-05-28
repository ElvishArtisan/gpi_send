// config.cpp
//
// Configuration for gpisend(1)
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

#include <syslog.h>

#include "config.h"
#include "profile.h"

Config::Config()
{
  clear();
}


QString Config::filename() const
{
  return d_filename;
}


QString Config::chipDevice() const
{
  return d_chip_device;
}


int Config::debouncePeriod() const
{
  return d_debounce_period;
}


enum gpiod_line_edge Config::triggerCondition() const
{
  return d_trigger_condition;
}


int Config::buttonQuantity() const
{
  return d_button_offsets.size();
}


int Config::buttonOffset(int n) const
{
  return d_button_offsets.at(n);
}


QHostAddress Config::destinationAddress(int n) const
{
  QHostAddress ret=d_default_destination_address;
  if(n>=0) {
    ret=d_destination_addresses.at(n);
  }
  return ret;
}


uint16_t Config::destinationPort(int n) const
{
  uint16_t ret=d_default_destination_port;
  if(n>=0) {
    ret=d_destination_ports.at(n);
  }
  return ret;
}


QString Config::command(int n) const
{
  return d_commands.at(n);
}


int Config::buttonByOffset(int offset) const
{
  return d_offset_to_button_map.value(offset,-1);
}


bool Config::load(const QString &filename)
{
  Profile *p=new Profile();
  bool ret=p->loadFile(filename);
  bool ok=false;
  d_filename=filename;

  d_chip_device=
    p->stringValue("Global","ChipDevice",CONFIG_DEFAULT_CHIP_DEVICE);
  d_debounce_period=
    p->intValue("Global","DebouncePeriod",CONFIG_DEFAULT_DEBOUNCE_PERIOD);
  QString edge=
    p->stringValue("Global","EdgeTrigger",CONFIG_DEFAULT_EDGE_TRIGGER);
  d_trigger_condition=GPIOD_LINE_EDGE_NONE;
  if(edge.toLower()=="rising") {
    d_trigger_condition=GPIOD_LINE_EDGE_RISING;
  }
  if(edge.toLower()=="falling") {
    d_trigger_condition=GPIOD_LINE_EDGE_FALLING;
  }
  if(edge.toLower()=="both") {
    d_trigger_condition=GPIOD_LINE_EDGE_BOTH;
  }
  if(d_trigger_condition==GPIOD_LINE_EDGE_NONE) {
    syslog(LOG_ERR,"invalid TriggerCondition \"%s\" in configuration",
	   edge.toUtf8().constData());
    exit(1);
  }
  d_default_destination_address=
    p->addressValue("Global","DestinationAddress",
		    CONFIG_DEFAULT_DESTINATION_ADDRESS);
  if(d_default_destination_address.isNull()) {
    syslog(LOG_ERR,"invalid DestinationAddress \"%s\" in configuration",
	   p->stringValue("Global","DestinationAddress",
		     CONFIG_DEFAULT_DESTINATION_ADDRESS).toUtf8().constData());
    exit(1);
  }
  d_default_destination_port=
    p->stringValue("Global","DestinationPort",CONFIG_DEFAULT_DESTINATION_PORT).
    toUInt(&ok);
  if(d_default_destination_port>0xFFFF) {
    syslog(LOG_ERR,"invalid DestinationPort \"%s\" in configuration",
	   p->stringValue("Global","DestinationPort",
		     CONFIG_DEFAULT_DESTINATION_PORT).toUtf8().constData());
    exit(1);
  }

  int count=0;
  QString section=QString::asprintf("Button%d",1+count);
  int offset=p->intValue(section,"Offset",-1,&ok);
  while(ok) {
    d_button_offsets.push_back(offset);
    d_offset_to_button_map[offset]=count;
    
    QHostAddress addr;
    addr=p->addressValue(section,"DestinationAddress",
			 d_default_destination_address);
    if(addr.isNull()) {
      syslog(LOG_ERR,"invalid DestinationAddress \"%s\" in configuration",
	   p->stringValue(section,"DestinationAddress",
			  d_default_destination_address.toString()).
	     toUtf8().constData());
      exit(1);
    }
    d_destination_addresses.push_back(addr);

    unsigned portnum=
      p->intValue(section,"DestinationPort",d_default_destination_port,&ok);
    if((portnum>0xFFFF)) {
      syslog(LOG_ERR,"invalid DestinationPort \"%s\" in configuration",
	     p->stringValue(section,"DestinationPort",
			    QString::asprintf("%u",d_default_destination_port)).
	     toUtf8().constData());
      exit(1);
    }
    d_destination_ports.push_back(portnum);

    d_commands.push_back(p->stringValue(section,"Command"));
    
    count++;
    section=QString::asprintf("Button%d",1+count);
    offset=p->intValue(section,"Offset",-1,&ok);
  }
  
  delete p;
  return ret;
}


QString Config::dump()
{
  QString ret;

  ret+=QString("[Global]\n");
  ret+=QString("ChipDevice=")+d_chip_device+"\n";
  ret+=QString::asprintf("DebouncePeriod=%d\n",d_debounce_period);
  switch(d_trigger_condition) {
  case GPIOD_LINE_EDGE_RISING:
    ret+=QString("TriggerCondition=rising\n");
    break;
 
  case GPIOD_LINE_EDGE_FALLING:
    ret+=QString("TriggerCondition=falling\n");
    break;
 
  case GPIOD_LINE_EDGE_BOTH:
    ret+=QString("TriggerCondition=both\n");
    break;
 
  case GPIOD_LINE_EDGE_NONE:
    ret+=QString("TriggerCondition=none\n");
    break;
  }
  ret+=QString("DestinationAddress=")+d_default_destination_address.toString()+
    "\n";
  ret+=QString::asprintf("DestinationPort=%u\n",d_default_destination_port);
  ret+="\n";
  for(int i=0;i<d_button_offsets.size();i++) {
    ret+=QString::asprintf("[Button%d]\n",1+i);
    ret+=QString::asprintf("Offset=%d\n",d_button_offsets.at(i));
    ret+=QString("DestinationAddress=")+d_destination_addresses.at(i).
      toString()+"\n";
    ret+=QString::asprintf("DestinationPort=%u\n",d_destination_ports.at(i));
    ret+=QString("Command=")+d_commands.at(i)+"\n";
    ret+="\n";
  }
  
  return ret;
}


void Config::clear()
{
  d_chip_device=CONFIG_DEFAULT_CHIP_DEVICE;
  d_trigger_condition=GPIOD_LINE_EDGE_RISING;
  d_offset_to_button_map.clear();
}
