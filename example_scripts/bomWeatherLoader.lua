local xml2lua     = require 'xml2lua'
local xmlhandler  = require 'xmlhandler.tree'
local ftp         = require 'socket.ftp'

function parseXmlFromFile(fileNm)
    local weatherdata = xml2lua.loadFile(fileNm)
    local parser = xml2lua.parser(xmlhandler)
    parser:parse(weatherdata)
    return xmlhandler
end

function parseXmlFromString(xmlStr)
    local parser = xml2lua.parser(xmlhandler)
    parser:parse(xmlStr)
    return xmlhandler
end


function downloadXmlToFile(sourceAddr, destFileAddr)
    local data, err = ftp.get(sourceAddr)
    if(err) then return err end
    
    local outputFile, err = io.open(destFileAddr, "wb")
    if(not outputFile) then return err end
    outputFile:write(data)
    outputFile:close()
    return nil
end

function getSourceFromTable(forcastTbl)
    return forcastTbl.root.product.amoc.source.sender
end

function getDateOfForcast(forcastTbl)
    return forcastTbl.root.product.amoc['sent-time']
end

--[[function

function parseDateOfForcast(date)
    local year = ""
    for()
end
--]]

function getValueFromStation(forcastTbl, stationNm, valueTitle)
    for _, station in pairs(forcastTbl.root.product.observations.station)
    do
        if( station['_attr']['description'] == stationNm ) then
            for _, elemnt in pairs(station.period.level.element)
            do
                if(elemnt['_attr']['type'] == valueTitle) then
                    temperature = elemnt[1]
                    units = elemnt['_attr']['units']
                    break
                end
            end
            break
        end
    end
    return {units, temperature}
end

--[[ Example calls:
     local temperature = getValueFromStation(xmlhandler, "Perth", "air_temperature")
     outputString = getSourceFromTable(xmlhandler) .. "\n" .. temperature[2] .. " " .. temperature[1]
     downloadXmlToFile("ftp.bom.gov.au/anon/gen/fwo/IDW60920.xml", "weather.xml")
--]]
ret = {}
ret['parseXmlFromFile'] = parseXmlFromFile
ret['parseXmlFromString'] = parseXmlFromString
ret['downloadXmlToFile'] = downloadXmlToFile
ret['getSourceFromTable'] = getSourceFromTable
ret['getDateOfForcast'] = getDateOfForcast
ret['getValueFromStation'] = getValueFromStation
return ret
