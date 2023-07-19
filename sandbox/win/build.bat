echo off
set cwd=%~dp0

echo %cwd%

C:\Windows\SysWOW64\WindowsPowerShell\v1.0\powershell.exe -noe -c "&{Import-Module """C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"""; Enter-VsDevShell 3184d1f8}; cd %cwd%; msbuild; exit"

