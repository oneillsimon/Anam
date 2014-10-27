v = Math.Vector3(10, 2, 3)
n = v.getX(v)
v2 = Math.Vector2(104, 44)
n2 = v2.getY(v2)
v3 = Math.Vector4(10, 10, 100, 1020)
n3 = v3.getW(v3)
print(n3)

function update()
	n = n + 0.002
end