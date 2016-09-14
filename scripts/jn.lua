project "jn"
	uuid(os.uuid("jn"))
	kind "StaticLib"

	files {
		path.join("..", "include", "**.h")
	}
