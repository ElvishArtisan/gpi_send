-- ------------------------------------------------------------------
-- Metadata filter for Paravel System's PadPoint Processor
--
-- (C) Copyright 2015 Fred Gleason <fredg@paravelsystems.com>
--
-- Adapted from 'Line Parser Sample.lua'
-- Copyright (C) 2009 Tls Corporation
-- Author: Ioan L. Rus
-- ------------------------------------------------------------------

-- This tells the application to use the line parsing mode
UseLineParser()

-- This function is called by the application each time a line of
-- metadata is received. A line can be a maximum of 511 characters.
-- If no line break is received before the buffer fills up then
-- this function will be called with the first 511 characters.
-- The endOfLine will be true if an end of line was received and
-- false if no end of line was received and this function was
-- called because the buffer was full.
function OnLineReceived(text,endOfLine)
  LogInfo("LineParser Script: Line='",text,"'\n")

  -- Initialize the title and url variable as blank by default
  local title=""
  local url=""
  
  -- Find the start of the title
  local titleStartPosition=string.find(text,"t=")
  if titleStartPosition~=nil then -- we found the start of the title
    LogInfo("LineParse Script: Title starts at ",titleStartPosition,"\n")
    
    -- Now find the end of the title (marked by the | character)
    local titleEndPosition=string.find(text,"|")
    if titleEndPosition~=nil then -- we found the end of the title
      LogInfo("LineParse Script: Title ends at ",titleEndPosition,"\n")
      
      -- Extract the title string. NOTE: We add 2 to skip the "t=" 
      -- characters and we subtract 1 to not include the | separator.
      title=string.sub(text,titleStartPosition+2,titleEndPosition-1)
    else
      LogInfo("LineParse Script: Title end not found!\n")
    end
  else
    LogInfo("LineParse Script: Title start not found!\n")
  end

  -- Find the start of the url
  local urlStartPosition=string.find(text,"u=")
  if urlStartPosition~=nil then -- we found the start of the url
    LogInfo("LineParse Script: URL found at ",urlStartPosition,"\n")
    
    -- The url extends all the way to the end of the line
    url=string.sub(text,urlStartPosition+2)
  else
    LogInfo("LineParse Script: URL not found!\n")
  end

  -- NOTE: The LogInfo function writes the arguments given to it
  -- to the log file. This can assist with script debugging.
  -- You may also use LogWarning and LogError functions.
  LogInfo("LineParse Script: title='",title,"', url='",url,"'\n")
  
  -- The SendMetaDataSong(title,url) function sends the metadata
  -- to the application. This info is then included in any streams
  -- that use the metadata source.
  SendMetaDataSong(title,url)   -- Send metadata to application
end
