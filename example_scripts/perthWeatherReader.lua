local xml = require "bomWeatherLoader"
local ftp = require "socket.ftp"
local WEATHER_DATA_URL = "ftp://ftp.bom.gov.au/anon/gen/fwo/IDW60920.xml"
local WEATHER_FILE_LOCATION = '/tmp/weather.xml'
-- TODO: get the current location based off ip address
-- to do this for all of aus you will have to figure out how to download the
-- correct xml file. There are different URLs for each state.
local STATION = 'Perth'
local FIELD = 'air_temperature'

-- to see if a previous file exists and how old it is we can use a bash script:
-- if it returns a blank string then we know it could not find or open the file
-- if it does return a string then the resault will be a string with the day,month,year,hour
local programOutputHandle = io.popen('date -r "'.. WEATHER_FILE_LOCATION .. '" "+%d,%m,%y,%H" 2> /dev/null')
local result = programOutputHandle:read('*a')
local fileTimeValues = {}
local fileTime = {}
local needUpdateFile = true -- by default we need to update

-- if we could not open file then the resault string will just be white space
for value in result:gmatch("%d%d")
do
    table.insert(fileTimeValues, value)
end

-- check if we need to update an existing file
if #fileTimeValues == 4 then
   -- this seems a little hacky
   fileTime = os.time{day=fileTimeValues[1], month=fileTimeValues[2], year=('20' .. fileTimeValues[3]), hour=fileTimeValues[4]}
   currentTime = os.time()
   needUpdateFile = currentTime-fileTime > 60*60
end

-- if so then download and save the xml for this, and next time.
local weatherString = ""
if needUpdateFile then
   ftp.TIMEOUT = 5
   local success, weatherFileData = pcall(function()
                                            local weatherFileData = ftp.get(WEATHER_DATA_URL)
                                            local outputFile = io.open(WEATHER_FILE_LOCATION, "wb")
                                            outputFile:write(weatherFileData)
                                            outputFile:close()
                                            return weatherFileData
                                          end)
   needUpdateFile = false
   if not success then
      return ""
   end

   weatherString = weatherFileData
else
   weatherString = nil
end

-- TODO: maybe add some logic to use the string we downloaded rather then reloading the data from file
local weather = {}
local success, retValue = false, {}
if weatherString then
    success, retValue = pcall(function() return parseXmlFromString(weatherString) end)
else
    success, retValue = pcall(function() return parseXmlFromFile(WEATHER_FILE_LOCATION) end)
end

if not success then
    -- TODO: for now we are just muting any errors
    return ""
end

weather = retValue
local success, retValue = pcall(function() return getValueFromStation(weather, STATION, FIELD) end)
local outputString = ""
if(success) then
    outputString = "The current temperature in " .. STATION .. " is " .. retValue[2] .. '°C'
else
    outputString = "Wether data for your current location is unavailable"
end
return outputString
