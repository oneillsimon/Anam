function update()
	v = transform:getPosition()
	v:setX(v:getX() + 0.1)
	transform:setPosition(v)
end