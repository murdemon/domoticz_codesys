return {
	active = true,
	on = {
		'Sensor_Terasse'
	},

	execute = function(domoticz, Switch)
		if (Switch.state == 'On') then
		   domoticz.devices['Lamp1'].switchOn()
		else
		   domoticz.devices['Lamp1'].switchOff()
		end
	end
}
