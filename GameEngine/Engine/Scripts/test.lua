v = Math.Vector3(10, 2, 3)
n = v.getX(v)

v2 = Math.Vector2(2, 22);
print(v2.getY(v2))

v4 = Math.Vector4(1, 2, 3, 4)
print(v4.getW(v4))

function update()
	n = n + 0.002
end