// gpioscan.cpp
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

#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>
#include <QString>

#include "cmdswitch.h"
#include "gpioscan.h"

#include <gpiod.h>

MainObject::MainObject()
{
  int count=0;
  QString path=QString::asprintf("/dev/gpiochip%d",count);
  QStringList devices;
  d_chip=NULL;
  d_info=NULL;

  CmdSwitch *cmd=new CmdSwitch("gpioscan",VERSION,USAGE);
  for(int i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--device") {
      devices.push_back(cmd->value(i));
      cmd->setProcessed(i,true);
    }
    if(!cmd->processed(i)) {
      fprintf(stderr,"gpioscan: unknown option \"%s\"\n",
	      cmd->key(i).toUtf8().constData());
      exit(1);
    }
  }
  for(int i=0;i<devices.size();i++) {
    if((d_chip=gpiod_chip_open(devices.at(i).toUtf8()))!=NULL) {
      ListChipInfo(d_chip);
      gpiod_chip_close(d_chip);
    }
  }
  
  exit(0);
}


void MainObject::ListChipInfo(struct gpiod_chip *chip)
{
  d_info=gpiod_chip_get_info(d_chip);
  printf("|---------------------------------------------------------------------------------|\n");
  printf("| Name: %-16s  Label: %-35s  Lines: %-4lu |\n",
	 gpiod_chip_info_get_name(d_info),
	 gpiod_chip_info_get_label(d_info),
	 gpiod_chip_info_get_num_lines(d_info));
  printf("|---------------------------------------------------------------------------------|\n");
  printf("|Line|Name        |Dir|Used By         |Edge|Bias   |Drive    |Actv|Dbnc|Clock    |\n");
  printf("|----|------------|---|----------------|----|-------|---------|----|----|---------|\n");
  unsigned lines=gpiod_chip_info_get_num_lines(d_info);
  for(unsigned i=0;i<lines;i++) {
    ListChipLine(d_chip,i);
  }
  printf("|---------------------------------------------------------------------------------|\n");
  printf("\n");
}


void MainObject::ListChipLine(struct gpiod_chip *chip,unsigned offset)
{
  struct gpiod_line_info *linfo=gpiod_chip_get_line_info(chip,offset);
  printf("| %02d |",offset);
  if(linfo==NULL) {
    printf("NULL");
  }
  else {
    //
    // Line Name
    //
    printf("%-12s|",gpiod_line_info_get_name(linfo));
    if(gpiod_line_info_get_direction(linfo)==GPIOD_LINE_DIRECTION_INPUT) {
      printf("in |");
    }
    else {
      printf("out|");
    }

    //
    // In Use / Free
    //
    if(gpiod_line_info_is_used(linfo)) {
      printf("%-16s|",gpiod_line_info_get_consumer(linfo));
    }
    else {
      printf("[free]          |");
    }

    //
    // Edge Detection
    //
    switch(gpiod_line_info_get_edge_detection(linfo)) {
    case GPIOD_LINE_EDGE_NONE:
      printf("none|");
      break;

    case GPIOD_LINE_EDGE_RISING:
      printf("rise|");
      break;

    case GPIOD_LINE_EDGE_FALLING:
      printf("fall|");
      break;

    case GPIOD_LINE_EDGE_BOTH:
      printf("both|");
      break;
    }

    //
    // Bias
    //
    switch(gpiod_line_info_get_bias(linfo)) {
    case GPIOD_LINE_BIAS_PULL_UP:
      printf("up     |");
      break;

    case GPIOD_LINE_BIAS_PULL_DOWN:
      printf("down   |");
      break;

    case GPIOD_LINE_BIAS_AS_IS:
      printf("as-is  |");
      break;

    case GPIOD_LINE_BIAS_DISABLED:
      printf("disable|");
      break;

    case GPIOD_LINE_BIAS_UNKNOWN:
      printf("unknown|");
      break;

    }

    //
    // Drive
    //
    switch(gpiod_line_info_get_drive(linfo)) {
    case GPIOD_LINE_DRIVE_PUSH_PULL:
      printf("push-pull|");
      break;

    case GPIOD_LINE_DRIVE_OPEN_DRAIN:
      printf("open-drn |");
      break;

    case GPIOD_LINE_DRIVE_OPEN_SOURCE:
      printf("open-src |");
      break;
    }

    //
    // Active Low/Hi
    //
    if(gpiod_line_info_is_active_low(linfo)) {
      printf("low |");
    }
    else {
      printf("high|");
    }

    //
    // Debouncing
    //
    if(gpiod_line_info_is_debounced(linfo)) {
      printf("%lu|",gpiod_line_info_get_debounce_period_us(linfo));
    }
    else {
      printf("n/a |");
    }

    //
    // Event Clock
    //
    switch(gpiod_line_info_get_event_clock(linfo)) {
    case GPIOD_LINE_CLOCK_MONOTONIC:
      printf("MONOTONIC|");
      break;

    case GPIOD_LINE_CLOCK_HTE:
      printf("HTE      |");
      break;

    case GPIOD_LINE_CLOCK_REALTIME:
      printf("REALTIME |");
      break;
    }

    gpiod_line_info_free(linfo);
  }
  printf("\n");
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);

  new MainObject();

  return a.exec();
}
