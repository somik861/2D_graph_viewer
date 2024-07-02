Write-Host Installing conan
python -m pip install conan setuptools
if (!($LastExitCode -eq 0))
{
    exit   
}

Write-Host Setting up conan profile
conan profile detect --force --name graph_2d_viewer_profile
if (!($LastExitCode -eq 0))
{
    exit   
}

Write-Host Using conan to install deps
conan install . --output-folder=build_conan --profile:build graph_2d_viewer_profile --profile:host graph_2d_viewer_profile --build=missing --settings build_type=Debug --settings compiler.cppstd=20
if (!($LastExitCode -eq 0))
{
    exit   
}

conan install . --output-folder=build_conan --profile:build graph_2d_viewer_profile --profile:host graph_2d_viewer_profile --build=missing --settings build_type=Release --settings compiler.cppstd=20
if (!($LastExitCode -eq 0))
{
    exit   
}
