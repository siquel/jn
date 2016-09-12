JKN_DIR = path.getabsolute("..")
JKN_BUILD_DIR = path.join(JKN_DIR, ".build")
JKN_LIB_DIR = path.join(JKN_DIR, "3rdparty")

solution "jkn.exercies"
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

dofile("toolchain.lua")
toolchain(JKN_BUILD_DIR, JKN_LIB_DIR)

project "jkn"
	uuid(os.uuid("JKN"))
	kind "StaticLib"

	files {
		path.join(JKN_DIR, "include", "**.h")
	}

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
		path.join(JKN_DIR, "include")
	}

	files {
		path.join(JKN_DIR, "exercises", name, "**.cpp"),
		path.join(JKN_DIR, "exercises", name, "**.h"),
	}

	--links {
	--	"JKN"
	--}

end

group("exercises")
exerciseProject("00-helloworld")
exerciseProject("01-threethreads")
exerciseProject("02-random")
exerciseProject("03-filethread")
