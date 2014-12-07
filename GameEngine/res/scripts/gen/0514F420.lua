s26500aor1 = Math.Vector3(1, 0, 0) 
s26500rotate = 0 
s19169speed = 2 
s15724aor1 = Math.Vector3(0, 1, 0) 
function s19169move(dir, amount) 
	v = transform:getPosition()
	v:setX(v:getX() + (amount * dir:getX()))
	v:setY(v:getY() + (amount * dir:getY()))
	v:setZ(v:getZ() + (amount * dir:getZ()))
	transform:setPosition(v)
end
function s19169sayHello() 
	print("Hello")
end
function final_input(delta)

	if input:getKey(KEY_UP) then
		s19169move(Math.Vector3(0, 1, 0), s19169speed * delta)
	end
	
	if input:getKey(KEY_DOWN) then
		s19169move(Math.Vector3(0, -1, 0), s19169speed * delta)
	end
	
	if input:getKey(KEY_LEFT) then
		s19169move(Math.Vector3(-1, 0, 0), s19169speed * delta)
	end
	
	if input:getKey(KEY_RIGHT) then
		s19169move(Math.Vector3(1, 0, 0), s19169speed * delta)
	end

	
	if input:getKey(KEY_O) then
		transform:rotate(s15724aor1, Math.toRadians(90) * delta)
	end

	if input:getKey(KEY_P) then
		transform:rotate(s15724aor1, Math.toRadians(-90) * delta)
	end
end
function final_update(delta)

	transform:rotate(s26500aor1, (200 * 3.14 / 180.0) * delta)


	if input:getKey(KEY_RETURN) then
		s19169sayHello()
	end

end
function final_render()
end
