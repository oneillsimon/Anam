function initialise()
	transform:setScale(Math.Vector3(1, 100, 2))
end

function input(delta)
	if input:getKey(KEY_4) then
		print("key pressed")
	end
end