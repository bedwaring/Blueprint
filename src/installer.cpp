#include <cstdlib>
#include <iostream>

int main() {
    const char* powershellCommand = R"(powershell.exe -Command "$url = 'https://github.com/bedwaring/Blueprint/releases/download/main/blueprint.exe'; $outputDir = \"$env:USERPROFILE\\.blueprint\"; if (!(Test-Path -Path $outputDir)) { New-Item -ItemType Directory -Path $outputDir }; $output = \"$env:USERPROFILE\\.blueprint\\blueprint.exe\"; Invoke-WebRequest -Uri $url -OutFile $output")";

    int result = system(powershellCommand);

    if (result == 0) {
        const char* pathCommand = R"(powershell.exe -Command "$url = 'https://raw.githubusercontent.com/bedwaring/Blueprint/main/src/path.ps1'; $outputDir = \"$env:TEMP\"; if (!(Test-Path -Path $outputDir)) { New-Item -ItemType Directory -Path $outputDir }; $output = \"$env:TEMP\\path.ps1\"; Invoke-WebRequest -Uri $url -OutFile $output")";
        system(pathCommand);
        system("powershell -Command cd $env:temp; path.ps1");
        std::cout << "Installation succeeded\n";
        return 0;
    } else {
        std::cout << "Installation failed\n";
        return 1;
    }
}
