-- Unified executable for the tutorial project
mox_project("DXT", "directx_tutorial")
mox_cpp("C++23")
mox_console()

mox_runpy_postbuild "copy_d3d12_sdk %{wks.location} %{cfg.buildtarget.directory}"

-- Project links
links {
    "DXTExternal"
}

-- Vendor and OS libs
links {
    "dxgi.lib",
    "d3d12.lib",
    "dxguid.lib",
}
