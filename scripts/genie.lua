JN_DIR = path.getabsolute("..")
JN_BUILD_DIR = path.join(JN_DIR, ".build")
JN_LIB_DIR = path.join(JN_DIR, "3rdparty")

solution "jn.exercies" 
	configurations { 
		"debug",
		"release"
	}
	
	platforms {
		"x32",
		"x64"
	}
	
	language "C++"
	
	configuration {} --reset

dofile("toolchain.lua")
toolchain(JN_BUILD_DIR, JN_LIB_DIR)

project "jn"
	uuid(os.uuid("jn"))
	kind "StaticLib"
	
	files {
		path.join(JN_DIR, "include", "**.h")
	}
	
function exerciseProject(name)
	project("ex-" .. name)
		uuid(os.uuid("ex-" .. name))
		kind "ConsoleApp"
		
	configuration {}
	
	includedirs {
		path.join("JN_DIR", "include")
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