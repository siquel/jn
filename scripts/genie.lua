JN_DIR = path.getabsolute("..")
JN_BUILD_DIR = path.join(JN_DIR, ".build")
JN_LIB_DIR = path.join(JN_DIR, "3rdparty")

solution "jn"
	configurations {
		"debug",
		"release"
	}

	platforms {
		"x32",
		"x64"
	}

	language "C++"

	startproject "ex-00-helloword"
	configuration {} --reset

	defines {
		"JKN_CONFIG_SOCKETS=1"
	}

	configuration { "debug" }
		defines {
			"JKN_CONFIG_DEBUG=1"
		}

	configuration {}

dofile("toolchain.lua")
toolchain(JN_BUILD_DIR, JN_LIB_DIR)

dofile ("jn.lua")

function exerciseProject(name)
	project("ex-" .. name)
		uuid(os.uuid("ex-" .. name))
		kind "ConsoleApp"

    configuration { "linux" }
        links {
            "pthread"
        }

	configuration {}

	includedirs {
		path.join(JN_DIR, "include")
	}

	files {
		path.join(JN_DIR, "exercises", name, "**.cpp"),
		path.join(JN_DIR, "exercises", name, "**.h"),
	}

	links {
		"jn"
	}

end

group("exercises")
exerciseProject("00-helloworld")
exerciseProject("01-threethreads")
exerciseProject("02-random")
exerciseProject("03-filethread")
