$userPath = [System.Environment]::GetEnvironmentVariable("Path", "User")

$dir = "%USERPROFILE%\.blueprint"
$userPath += ";$dir"

[System.Environment]::SetEnvironmentVariable("Path", $userPath, "User")
