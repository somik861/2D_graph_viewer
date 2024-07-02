if (!(Test-Path .env)){
    Write-Host Installing python environment
    py -m venv --upgrade-deps .env
}

Write-Host Installing conan
.env\scripts\python -m pip install --require-virtualenv conan
if (!($LastExitCode -eq 0))
{
    exit   
}

Write-Host Setting up conan profile
.env\scripts\conan profile detect --force --name graph_2d_viewer_profile
if (!($LastExitCode -eq 0))
{
    exit   
}

Write-Host Using conan to install deps
.env\scripts\conan install . --output-folder=build_conan --profile:build graph_2d_viewer_profile --profile:host graph_2d_viewer_profile --build=missing --settings build_type=Debug --settings compiler.cppstd=20
if (!($LastExitCode -eq 0))
{
    exit   
}

.env\scripts\conan install . --output-folder=build_conan --profile:build graph_2d_viewer_profile --profile:host graph_2d_viewer_profile --build=missing --settings build_type=Release --settings compiler.cppstd=20
if (!($LastExitCode -eq 0))
{
    exit   
}
