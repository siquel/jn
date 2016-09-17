project "jn"
	uuid(os.uuid("jn"))
	kind "StaticLib"

	files {
    path.join("..", "src", "**.cpp"),
		path.join("..", "include", "**.h")
	}

  includedirs {
    path.join("..", "include")
  }

  defines {
    "JKN_CONFIG_SOCKETS=1"
  }
