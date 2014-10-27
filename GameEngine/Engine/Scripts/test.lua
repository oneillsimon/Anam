function update()
	v = transform:getPosition()
	v:setX(v:getX() + n)
	transform:setPosition(v)
end