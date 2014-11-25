local aor1 = Math.Vector3(0, 1, 0)

function input(delta)
	
	if input:getKey(KEY_O) then
		transform:rotate(aor1, Math.toRadians(90) * delta)
	end

	if input:getKey(KEY_P) then
		transform:rotate(aor1, Math.toRadians(-90) * delta)
	end
end