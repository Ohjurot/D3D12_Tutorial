-- === Configuration ===
-- Change the following lines to complete your setup

-- PRODUCT NAME
-- This the name used for the product provided by this repo
-- (On windows used as the solution name)
cmox_product_name = "D3D12Tutorial"

-- CONFIGURATIONS
-- The first array _n are the configuration names
-- The second array _d are the configuration debug flags 
-- (use true on all debugging configurations)
cmox_configurations_n = { 
    "Debug",
    "Develop",
    "Release",   
}
cmox_configurations_d = { 
    true,
    false,
    false,
}

-- SOURCE FOLDER NAME
cmox_src_folder = "src"

-- PROJECT ARCHITECTURE
-- "single"         Only one build.lua file will be loaded.
--                  The file needs to be in the src folder.
--                  Used for singel project repos
--
-- "flat"           The hirarch of the project is flat.
--                  build.lua files will be loaded from from
--                  subdirs of the src folder. Each dir can 
--                  be a project.      
-- 
-- "hierarchical"   Adds one more indirection layer to the
--                  "flat" architecture. The first folder 
--                  will be the group name. Groups are treatet
--                  similar to the flat modell
--
-- "manual"         Projects are not loaded by the MoxPP
--                  provide the "cmox_function_includeprojects"
--                  function.
cmox_project_architecture = "flat" 

-- DLL DISTRIBUTING
-- When enabled, dlls will be copied as a postbuild-step to the output directory
cmox_copy_dlls = false

-- MACRO PREFIX
-- This will be prepended to ALL non default macros
cmox_macro_prefix = "DXT_"

-- UNITTEST
-- This defines where to find the unit test code.
-- You can set this to nil. When set to nil no test will be configured. 
-- You can delete the default test folder then
cmox_unit_test_src = nil

-- === Custom callback functions ===
-- This is the way to go when implementing custom features

-- This function is called when the workspace is configured
-- function cmox_function_setupworkspace()
--    ...
-- end

-- This function is called for each project when it's beeing configured
function cmox_function_setupproject()
    includedirs {
        "%{wks.location}src/DXTExternal",
        "%{wks.location}vendor/d3d12_sdk/include",
    }
end

-- This function is called in manual configuration to include 
-- the project
-- use: include "my-build-file.lua"
function cmox_function_includeprojects()
    startproject "DXT"
end
