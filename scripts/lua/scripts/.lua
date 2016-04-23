return {
	active = true,
	on = {
		'Lamp3'
	},

	execute = function(domoticz, mySwitch)
		if (mySwitch.state == 'On') then
			domoticz.notify('Hey!', 'I am on!', domoticz.PRIORITY_NORMAL)
		end
	end
}
