return {
	active = true,
	on = {
		'Sensor_Entrance'
	},

	execute = function(domoticz, Switch)
		if (Switch.state == 'On') then
		   domoticz.devices['Lamp2'].switchOn()
		else
		   domoticz.devices['Lamp2'].switchOff()
		end
	end
}
