return {
	active = true,
	on = {
		'Switch2'
	},

	execute = function(domoticz, Switch)
		if (Switch.state == 'Open') then
		   domoticz.devices['Lamp_Bedroom'].switchOn()
		else
		   domoticz.devices['Lamp_Bedroom'].switchOff()
		end
	end
}
