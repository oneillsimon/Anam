function move(dir, amount)
	v = transform:getPosition()
	v:setX(v:getX() + (amount * dir:getX()))
	v:setY(v:getY() + (amount * dir:getY()))
	v:setZ(v:getZ() + (amount * dir:getZ()))
	transform:setPosition(v)
end

function sayHello()
	print("Hello")
end

local speed = 2

function input(delta)
	if input:getKey(KEY_UP) then
		move(Math.Vector3(0, 1, 0), speed * delta)
	end
	
	if input:getKey(KEY_DOWN) then
		move(Math.Vector3(0, -1, 0), speed * delta)
	end
	
	if input:getKey(KEY_LEFT) then
		move(Math.Vector3(-1, 0, 0), speed * delta)
	end
	
	if input:getKey(KEY_RIGHT) then
		move(Math.Vector3(1, 0, 0), speed * delta)
	end
end

function update(delta)

	if input:getKey(KEY_RETURN) then
		sayHello()
	end

end