-- premake5.lua
workspace "LearningRayTracing"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "LearningRayTracing"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "LearningRayTracing"