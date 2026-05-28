// gpisend.cpp
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include <QCoreApplication>
#include <QString>

#include "cmdswitch.h"
#include "gpisend.h"

MainObject::MainObject()
{
  bool debug=false;
  bool dump_config=false;
  d_chip=NULL;
  d_info=NULL;
  d_request_config=NULL;
  d_chip=NULL;
  d_info=NULL;
  d_line_config=NULL;
  d_line_settings=NULL;
  d_request_config=NULL;
  d_line_request=NULL;
  d_edge_event_buffer=NULL;

  CmdSwitch *cmd=new CmdSwitch("gpisend",VERSION,USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="-d") {
      debug=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--dump-config") {
      dump_config=true;
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"gpisend: unknown option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }

  //
  // Open Syslog
  //
  if(debug) {
    openlog("gpisend",LOG_PERROR,LOG_USER);
  }
  else {
    openlog("gpisend",0,LOG_USER);
  }

  //
  // Open Configuration
  //
  d_config=new Config();
  if(d_config->load()) {
    syslog(LOG_DEBUG,"loaded configuration from \"%s\"",
	   d_config->filename().toUtf8().constData());
  }
  else {
    syslog(LOG_WARNING,"failed to load configuration from \"%s\"",
	   d_config->filename().toUtf8().constData());
  }
  if(dump_config) {
    printf("%s",d_config->dump().toUtf8().constData());
    exit(0);
  }

  InitializeGpio();

  InitializeNetworking();

  EventLoop();
}


void MainObject::EventLoop()
{
  int n;
  struct gpiod_edge_event *event;
  while(1==1) {
    switch(gpiod_line_request_wait_edge_events(d_line_request,-1)) {
    case 0:  // This should never happen.
      syslog(LOG_WARNING,"GPIO device \"%s\" timed out",
	     d_config->chipDevice().toUtf8().constData());
      break;

    case 1:
      if((n=gpiod_line_request_read_edge_events(d_line_request,d_edge_event_buffer,4))>0) {
	for(unsigned long i=0;i<(unsigned long)n;i++) {
	  event=gpiod_edge_event_buffer_get_event(d_edge_event_buffer,i);
	  int offset=gpiod_edge_event_get_line_offset(event);
	  int button=d_config->buttonByOffset(offset);
	  d_send_socket->writeDatagram(d_config->command(button).toUtf8(),
				       d_config->destinationAddress(button),
				       d_config->destinationPort(button));
	  syslog(LOG_DEBUG,"sending %s to %s:%u",
		 d_config->command(button).toUtf8().constData(),
		 d_config->destinationAddress(button).toString().
		 toUtf8().constData(),
		 0xFFFF&d_config->destinationPort(button));
	}
      }
      break;

    case -1:
      syslog(LOG_ERR,"GPIO device \"%s\" threw an error [%s]",
	     d_config->chipDevice().toUtf8().constData(),strerror(errno));
      exit(1);
      break;
    }
  }
}


void MainObject::InitializeGpio()
{
  //
  // Open GPIO Device
  //
  if((d_chip=gpiod_chip_open(d_config->chipDevice().toUtf8()))==NULL) {
    syslog(LOG_ERR,"failed to open device \"%s\" [%s]",
	   d_config->chipDevice().toUtf8().constData(),strerror(errno));
    exit(1);
  }
  if((d_line_config=gpiod_line_config_new())==NULL) {
    syslog(LOG_ERR,"failed to create line configuration [%s]",strerror(errno));
    exit(1);
  }
  if((d_line_settings=gpiod_line_settings_new())==NULL) {
    syslog(LOG_ERR,"failed to create line settings [%s]",strerror(errno));
    exit(1);
  }
  gpiod_line_settings_set_direction(d_line_settings,GPIOD_LINE_DIRECTION_INPUT);
  gpiod_line_settings_set_edge_detection(d_line_settings,
					 d_config->triggerCondition());
  gpiod_line_settings_set_bias(d_line_settings,d_config->lineBias());
  gpiod_line_settings_set_active_low(d_line_settings,d_config->activeLow());
  gpiod_line_settings_set_debounce_period_us(d_line_settings,
					     d_config->debouncePeriod());
  gpiod_line_settings_set_event_clock(d_line_settings,
				      GPIOD_LINE_CLOCK_MONOTONIC);
  unsigned offsets[d_config->buttonQuantity()];
  for(int i=0;i<d_config->buttonQuantity();i++) {
    offsets[i]=d_config->buttonOffset(i);
  }

  d_request_config=gpiod_request_config_new();
  if(gpiod_line_config_add_line_settings(d_line_config,offsets,
					 d_config->buttonQuantity(),
					 d_line_settings)!=0) {
    syslog(LOG_ERR,"failed to add line settings [%s]",strerror(errno));
    exit(1);
  }
  
  gpiod_request_config_set_consumer(d_request_config,"gpisend");
  gpiod_request_config_set_event_buffer_size(d_request_config,0); // Use default
  if((d_line_request=gpiod_chip_request_lines(d_chip,d_request_config,d_line_config))==NULL) {
    syslog(LOG_ERR,"failed to create request [%s]",strerror(errno));
    exit(1);
  }

  d_edge_event_buffer=gpiod_edge_event_buffer_new(4);
}


void MainObject::InitializeNetworking()
{
  d_send_socket=new QUdpSocket(this);
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();

  return a.exec();
}
