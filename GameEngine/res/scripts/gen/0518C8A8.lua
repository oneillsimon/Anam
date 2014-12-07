s24464speed = 2 
s5705aor1 = Math.Vector3(0, 0, 0) 
s28145aor1 = Math.Vector3(0, 1, 0) 
function s24464move(dir, amount) 
	v = transform:getPosition()
	v:setX(v:getX() + (amount * dir:getX()))
	v:setY(v:getY() + (amount * dir:getY()))
	v:setZ(v:getZ() + (amount * dir:getZ()))
	transform:setPosition(v)
end
function s24464sayHello() 
	print("Hello")
end
function s5705move(dir, amount) 
	-- This is from test.lua
	-- This shouldn't conflict with s5705move in s5705move.lua
end
function s5705final_update(delta) 
	-- This is from test.lua
	-- It should have a prefix and not
	-- be copied into the s5705final_update function
end
function s5705printHello() 
	print("Hello World")
end
function s5705add(n, m) 
	return n + m
end
function final_input(delta)

	if input:getKey(KEY_UP) then
		s24464move(Math.Vector3(0, 1, 0), s24464speed * delta)
	end
	
	if input:getKey(KEY_DOWN) then
		s24464move(Math.Vector3(0, -1, 0), s24464speed * delta)
	end
	
	if input:getKey(KEY_LEFT) then
		s24464move(Math.Vector3(-1, 0, 0), s24464speed * delta)
	end
	
	if input:getKey(KEY_RIGHT) then
		s24464move(Math.Vector3(1, 0, 0), s24464speed * delta)
	end

	
	if input:getKey(KEY_O) then
		transform:rotate(s28145aor1, Math.toRadians(90) * delta)
	end

	if input:getKey(KEY_P) then
		transform:rotate(s28145aor1, Math.toRadians(-90) * delta)
	end
end
function final_update(delta)


	if input:getKey(KEY_RETURN) then
		s24464sayHello()
	end


	--s5705printHello()
	--print(s5705add(3, 4))
end
function final_render()
end
