s26500aor1 = Math.Vector3(0, 1, 0) 
function final_initialise()

	transform:setScale(Math.Vector3(1, 100, 2))
end
function final_input(delta)

	
	if input:getKey(KEY_O) then
		transform:rotate(s26500aor1, Math.toRadians(90) * delta)
	end

	if input:getKey(KEY_P) then
		transform:rotate(s26500aor1, Math.toRadians(-90) * delta)
	end

	if input:getKey(KEY_4) then
		print("key pressed")
	end
end
function final_update(delta)
end
function final_render()
end
