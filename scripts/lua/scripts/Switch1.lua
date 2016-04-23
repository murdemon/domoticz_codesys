return {
	active = true,
	on = {
		'Switch1'
	},

	execute = function(domoticz, Switch)
		if (Switch.state == 'Open') then
		   domoticz.devices['Switch_Wall_Socket'].switchOn()
		else
		   domoticz.devices['Switch_Wall_Socket'].switchOff()
		end
	end
}
