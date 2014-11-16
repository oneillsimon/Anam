local aor1 = Math.Vector3(1, 0, 0)

function update(delta)
	transform:rotate(aor1, (200 * 3.14 / 180.0) * delta)
end