# Build script for Void Contingency
param(
    [ValidateSet('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')]
    [string]$BuildType = 'Debug'
)

# Function to check if a command exists
function Test-Command($Command) {
    return [bool](Get-Command -Name $Command -ErrorAction SilentlyContinue)
}

# Get the project root directory (one level up from the scripts directory)
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $ProjectRoot "build"

# Check prerequisites
Write-Host "Checking prerequisites..." -ForegroundColor Cyan

if (-not (Test-Command "cmake")) {
    Write-Host "Error: CMake is not installed or not in PATH" -ForegroundColor Red
    exit 1
}

# Create build directory if it doesn't exist
if (-not (Test-Path $BuildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Cyan
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Navigate to build directory
Push-Location $BuildDir

try {
    # Configure with CMake
    Write-Host "Configuring project with CMake ($BuildType)..." -ForegroundColor Cyan
    cmake -DCMAKE_BUILD_TYPE=$BuildType $ProjectRoot
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed"
    }

    # Build the project
    Write-Host "Building project..." -ForegroundColor Cyan
    cmake --build . --config $BuildType
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }

    Write-Host "`nBuild completed successfully!" -ForegroundColor Green
    Write-Host "Executable location: $BuildDir\bin\$BuildType\VoidContingency.exe" -ForegroundColor Green

    # Ask if user wants to run tests
    $RunTests = Read-Host "Would you like to run the tests? (y/n)"
    if ($RunTests -eq 'y') {
        Write-Host "Running tests..." -ForegroundColor Cyan
        ctest -C $BuildType
    }

} catch {
    Write-Host "Error: $_" -ForegroundColor Red
    exit 1
} finally {
    Pop-Location
} 