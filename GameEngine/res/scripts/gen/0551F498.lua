test_aor1 = Math.Vector3(0, 0, -1) 
function test_printHello() 
	print("Hello World")
end
function test_add(n, m) 
	return n + m
end
function final_input(delta)
end
function final_update(delta)

	test_printHello()
	transform:rotate(test_aor1, (90 * 3.14 / 180.0) * delta)
end
function final_render()
end
