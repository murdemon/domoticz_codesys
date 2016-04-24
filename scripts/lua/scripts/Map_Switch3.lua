-- Map Codesys Virtual Outputs - to Domoticz hardware

return {
	active = true,
	on = {
		'Switch3'
	},

	execute = function(domoticz, Switch)
	   domoticz.devices['Switch_Wall_Socket'].setState(Switch(State))
	end
}

-- setState(newState): Function. Generic update method for switch-like devices. 
--       E.g.: device.setState('On'). Supports timing options. See below.

--state: String. For switches this holds the state like 'On' or 'Off'. 
--       For dimmers that are on, it is also 'On' but there is a level attribute holding the dimming level. 
--       For selector switches (Dummy switch) the state holds the name of the currently selected level. 
--       The corresponding numeric level of this state can be found in the rawData attribute: device.rawData[1].