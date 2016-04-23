-- make sure we can find our modules
local currentPath = debug.getinfo(1).source:match("@?(.*/)")
scriptsFolderPath = currentPath .. 'scripts'
package.path = package.path .. ';' .. currentPath .. '?.lua'
package.path = package.path .. ';' .. currentPath .. 'dzVents/?.lua'
package.path = package.path .. ';' .. currentPath .. 'scripts/?.lua'


local EventHelpers = require('EventHelpers')
local helpers = EventHelpers()

commandArray = helpers.dispatchTimerEventsToScripts()

return commandArray