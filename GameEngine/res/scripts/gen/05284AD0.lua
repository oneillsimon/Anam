s6334aor1 = Math.Vector3(0, 1, 0) 
function final_initialise()
end
function final_input(delta)

	
	if input:getKey(KEY_O) then
		transform:rotate(s6334aor1, Math.toRadians(90) * delta)
	end

	if input:getKey(KEY_P) then
		transform:rotate(s6334aor1, Math.toRadians(-90) * delta)
	end
end
function final_update(delta)
end
function final_render()
end
