v = Math.Vector3(14, 2, 3)
n = v:getX()

v2 = Math.Vector2(2, 22);
print(v2:getY())

v4 = Math.Vector4(1, 2, 3, 4)
print(v4:getW())

p = Core.Transform(v, Math.Quaternion(Math.Vector3(0, 0, 0), 10), 2)
pA = Object.GameObject(v, Math.Quaternion(Math.Vector3(0, 0, 0), 10), 2)
pP = pA:parentTransform()

--hope = g
print(gg:getX_())
print('past hope')

--function update()
--	n = n + 0.002
--	v:setX(n)
--	--print(pP:getPosition():getX())
--end