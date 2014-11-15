function move(dir, amount)
	v = transform:getPosition()
	v:setX(v:getX() + (amount * dir:getX()))
	v:setY(v:getY() + (amount * dir:getY()))
	v:setZ(v:getZ() + (amount * dir:getZ()))
	transform:setPosition(v)
end

key = 1
speed = 0.01

function update()
	print("I am move.lua")
	--if key == 1 then
	--	move(Math.Vector3(0, 1, 0), speed)
	--elseif key == 2 then
	--	move(Math.Vector3(0, -1, 0), speed)
	--elseif key == 3 then
	--	move(Math.Vector3(-1, 0, 0), speed)
	--elseif key == 4 then
	--	move(Math.Vector3(1, 0, 0), speed)
	--end
end