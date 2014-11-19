function moveTransform(dir, amount)
	v = transform:getPosition()
	v:setX(v:getX() + (amount * dir:getX()))
	v:setY(v:getY() + (amount * dir:getY()))
	v:setZ(v:getZ() + (amount * dir:getZ()))
	transform:setPosition(v)
end

local speed = 2

function input(delta)
	if input:getKey(KEY_UP) then
		moveTransform(Math.Vector3(0, 1, 0), speed * delta)
	end

	if input:getKey(KEY_DOWN) then
		moveTransform(Math.Vector3(0, -1, 0), speed * delta)
	end

	if input:getKey(KEY_LEFT) then
		moveTransform(Math.Vector3(-1, 0, 0), speed * delta)
	end

	if input:getKey(KEY_RIGHT) then
		moveTransform(Math.Vector3(1, 0, 0), speed * delta)
	end
end

function update(delta)
end