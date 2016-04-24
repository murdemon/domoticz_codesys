return {
	active = true,
	on = {
		'Handheld Remote'
	},

	execute = function(domoticz, Switch)
		if (Switch.state == 'On') then
		   domoticz.devices['Lamp3'].switchOn().for_min(1) 
		end
	end
}
